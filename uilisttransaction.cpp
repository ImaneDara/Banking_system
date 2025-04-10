#include "uilisttransaction.h"
#include "ui_uilisttransaction.h"
#include "controller.h"
#include <QDate>
#include <QDateEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlError>

UIListTransaction::UIListTransaction(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UIListTransaction)
{
    ui->setupUi(this);
}

UIListTransaction::UIListTransaction(QObject* controller) :
    ui(new Ui::UIListTransaction)
{
    qDebug("Contruction/Initialisation de la fenêtre UIListTransaction ...");
    ui->setupUi(this);

    connect(ui->pushButtonClose, SIGNAL(clicked()), controller, SLOT(onClose_UIListTransaction()));
    connect(ui->pushButtonFiltrer, SIGNAL(clicked()), controller , SLOT(onFiltrerClicked()));
    connect(ui->pushButtonValider, SIGNAL(clicked()), controller, SLOT(onValiderClicked()));
    connect(ui->pushButtonRejeter, SIGNAL(clicked()), controller, SLOT(onRejeterClicked()));
}

void UIListTransaction::setTableViewModel(TransactionModel* transactionModel)
{
    ui->tableViewTransactions->setModel(dynamic_cast<QAbstractItemModel*> (transactionModel));
    ui->tableViewTransactions->setSelectionModel(transactionModel->getSelectionModel());

    ui->tableViewTransactions->show();
}

void UIListTransaction::top()
{
    ui->tableViewTransactions->setCurrentIndex(ui->tableViewTransactions->model()->index(0, 0));
}

void UIListTransaction::updateTitle(QString suite)
{
    this->setWindowTitle("Historique des transactions de : " + suite);
}

void UIListTransaction::updateTitle(QString suite, QString transactionTitle)
{
    updateTitle(suite);
    ui->labelTitle->setText(transactionTitle);
}

void UIListTransaction::setAccountNumber(const QString& accountNumber) {
    currentAccountNumber = accountNumber;
}

QString UIListTransaction::getSelectedFilter()
{
    return ui->comboBoxType->currentText();
}

QString UIListTransaction::getTransactionType()
{
    bool ok;
    QString type = QInputDialog::getItem(
        this,
        "Filtrer par type",
        "Sélectionnez le type de transaction:",
        QStringList() << "Versement" << "Retrait" << "Virement",
        0,
        false,
        &ok
        );

    return ok ? type : QString();
}

QString UIListTransaction::getDateStr()
{
    bool ok;
    QString date = QInputDialog::getText(
        this,
        "Filtrer par date",
        "Entrez la date (AAAA-MM-JJ):",
        QLineEdit::Normal,
        QDate::currentDate().toString("yyyy-MM-dd"),
        &ok
        );

    return ok ? date : QString();
}
TransactionModel* UIListTransaction::getTransactionModel() const
{
    return dynamic_cast<TransactionModel*>(ui->tableViewTransactions->model());
}

void UIListTransaction::disableActionButtons()
{
    ui->pushButtonValider->setEnabled(false);
    ui->pushButtonRejeter->setEnabled(false);
}

void UIListTransaction::enableActionButtons()
{
    ui->pushButtonValider->setEnabled(true);
    ui->pushButtonRejeter->setEnabled(true);
}

QModelIndex UIListTransaction::getSelectedTransactionIndex() const
{
    return ui->tableViewTransactions->selectionModel()->currentIndex();
}

Transaction* UIListTransaction::getSelectedTransaction() const
{
    QModelIndex index = ui->tableViewTransactions->selectionModel()->currentIndex();
    if (!index.isValid()) {
        return nullptr;
    }

    TransactionModel* model = dynamic_cast<TransactionModel*>(ui->tableViewTransactions->model());
    if (!model) {
        return nullptr;
    }

    QSqlRecord record = model->record(index.row());

    return new Transaction(
        record.value("id").toInt(),
        record.value("idClient").toInt(),
        record.value("type").toString(),
        record.value("idCompteTire").toInt(),
        record.value("idCompteBeneficiaire").toInt(),
        record.value("numeroCompteTire").toString(),
        record.value("numeroCompteBeneficiaire").toString(),
        record.value("montant").toDouble(),
        record.value("date").toString(),
        record.value("statut").toString()
        );
}

bool UIListTransaction::validateSelectedTransaction()
{
    std::unique_ptr<Transaction> transaction(getSelectedTransaction());
    if (!transaction) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une transaction");
        return false;
    }

    if (transaction->getStatut() != "In progress") {
        QMessageBox::warning(this, "Statut incorrect", "Transaction déjà effectuée");
        return false;
    }

    dbManager = DBManager::getInstance();
    if (!dbManager->open()) {
        QMessageBox::critical(this, "Erreur", "Connexion base de données échouée");
        return false;
    }

    QSqlDatabase database = dbManager->database();
    database.transaction();

    try {
        // Process account balances
        if (transaction->getType() == "Retrait" || transaction->getType() == "Virement") {
            // Verify account balance
            QSqlQuery checkQuery(database);
            checkQuery.prepare("SELECT balance FROM t_accounts WHERE number = ?");
            checkQuery.addBindValue(transaction->getNumeroCompteTire());

            if (!checkQuery.exec() || !checkQuery.next()) {
                throw std::runtime_error("Impossible de vérifier le solde du compte");
            }

            double solde = checkQuery.value(0).toDouble();
            if (solde < transaction->getMontant()) {
                throw std::runtime_error(
                    QString("Solde insuffisant (%1 € pour %2 €)")
                        .arg(solde, 0, 'f', 2)
                        .arg(transaction->getMontant(), 0, 'f', 2).toStdString());
            }

            // Perform debit
            QSqlQuery debitQuery(database);
            debitQuery.prepare("UPDATE t_accounts SET balance = balance - ? WHERE number = ?");
            debitQuery.addBindValue(transaction->getMontant());
            debitQuery.addBindValue(transaction->getNumeroCompteTire());

            if (!debitQuery.exec() || debitQuery.numRowsAffected() != 1) {
                throw std::runtime_error("Échec du débit sur le compte");
            }
        }

        // For deposits or transfers, credit the account
        if (transaction->getType() == "Versement" || transaction->getType() == "Virement") {
            QString targetAccount = (transaction->getType() == "Versement")
            ? transaction->getNumeroCompteTire()
            : transaction->getNumeroCompteBeneficiaire();

            QSqlQuery creditQuery(database);
            creditQuery.prepare("UPDATE t_accounts SET balance = balance + ? WHERE number = ?");
            creditQuery.addBindValue(transaction->getMontant());
            creditQuery.addBindValue(targetAccount);

            if (!creditQuery.exec() || creditQuery.numRowsAffected() != 1) {
                throw std::runtime_error("Échec du crédit sur le compte bénéficiaire");
            }
        }

        // Update transaction status
        QSqlQuery updateTransaction(database);
        updateTransaction.prepare(
            "UPDATE t_transactions SET "
            "statut = :statut, "
            "date = :date "
            "WHERE id = :id");

        updateTransaction.bindValue(":statut", "Completed");
        updateTransaction.bindValue(":date", QDateTime::currentDateTime());
        updateTransaction.bindValue(":id", transaction->getId());

        if (!updateTransaction.exec()) {
            throw std::runtime_error(
                QString("Échec de la mise à jour de la transaction: %1")
                    .arg(updateTransaction.lastError().text()).toStdString());
        }

        if (!database.commit()) {
            throw std::runtime_error("Échec de la validation de la transaction");
        }

        this->update();
        return true;

    } catch (const std::exception& e) {
        database.rollback();
        QMessageBox::critical(this, "Erreur", e.what());
        qDebug() << "Error:" << e.what();
        return false;
    }
}

bool UIListTransaction::rejectSelectedTransaction()
{
    std::unique_ptr<Transaction> transaction(getSelectedTransaction());
    if (!transaction) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une transaction à rejeter.");
        return false;
    }

    if (transaction->getStatut() != "In progress") {
        QMessageBox::warning(this, "Statut incorrect", "Transaction déjà traitée");
        return false;
    }

    dbManager = DBManager::getInstance();
    if (!dbManager->open()) {
        QMessageBox::critical(this, "Erreur", "Connexion base de données échouée");
        return false;
    }

    QSqlDatabase database = dbManager->database();
    database.transaction();

    try {
        // Update transaction status
        QSqlQuery updateQuery(database);
        updateQuery.prepare(
            "UPDATE t_transactions SET "
            "statut = 'Rejected', "
            "date = :date "
            "WHERE id = :id");

        updateQuery.bindValue(":date", QDateTime::currentDateTime());
        updateQuery.bindValue(":id", transaction->getId());

        if (!updateQuery.exec()) {
            throw std::runtime_error(
                QString("Échec de la mise à jour en base: %1")
                    .arg(updateQuery.lastError().text()).toStdString());
        }

        // Update the model
        TransactionModel* model = dynamic_cast<TransactionModel*>(ui->tableViewTransactions->model());
        QModelIndex statusIndex = model->index(ui->tableViewTransactions->selectionModel()->currentIndex().row(), 6);
        model->setData(statusIndex, "Rejected");

        if (!database.commit()) {
            throw std::runtime_error("Échec du commit");
        }

        this->update();
        return true;

    } catch (const std::exception& e) {
        database.rollback();
        if (QString(e.what()).contains("actualisée")) {
            QMessageBox::information(this, "Succès", "Transaction rejetée (rafraîchissement nécessaire)");
            return true;
        } else {
            QMessageBox::critical(this, "Erreur", e.what());
            return false;
        }
    }
}
UIListTransaction::~UIListTransaction()
{
    delete ui;
}
