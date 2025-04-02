#include "controller.h"

Controller::Controller(QObject* parent) :
    QObject(parent)
{
    qDebug("Controller Started.");
}

void Controller::execute()
{
    uiLoginIn.show();
}

void Controller::onSubmit_UILoginIn()
{
    QString login = uiLoginIn.getLogin();
    QString password = uiLoginIn.getPassword();

    qDebug () << "Controller::onSubmit_UILoginIn " << login << "-" << password << "";
    bool status = service.authentifier(login, password, connectedUser);

    if (status == true)
    {
        uiLoginIn.hide();

        switch (connectedUser.getEnumRole())
        {
            case ADMINISTRATEUR:
                uiUser.show();
                uiUser.reinit();
                uiUser.deactivateUpdate();
                uiUser.updateTitle (connectedUser.getNom());

            break;

            case CLIENT:
                uiClient.setTableViewModel(accountModel);
                uiClient.show();
                uiClient.hideTransactionForm();
                uiClient.updateTitle (connectedUser.getNom());
                service.listerLesComptes(connectedUser.getId());
                uiListAccount.setTableViewModel(accountModel);
                uiListAccount.top();
            break;

            case GESTIONNAIRE:
                uiListAccount.setTableViewModel(accountModel);
                uiListClient.setTableViewModel(userModel);
                uiListClient.show();
                service.listerLesClients();
                uiListClient.updateTitle (connectedUser.getNom());
                uiListClient.top();

            break;

            default:;
                // Nothing to be done!
        }
    }
    else
    {
        uiLoginIn.critical("Sign in",
            "Incorrect Login/Password, please try again !");
    }
}

void Controller::onExit_UILoginIn()
{
    if (true == uiLoginIn.question("Exit confirmation",
            "Do you really want to exit the application ?"))
        {
            uiLoginIn.close();
        }
}

/* Les slots de la fenêtre UIListUser
 *
 */
void Controller::onUpdate_UIListUser()
{
    QModelIndex index = userModel->getSelectionModel()->currentIndex();

    if (index.isValid() == false)
    {
        uiListUser.warning("Mise a jour des informations d'une User",
                              "Veuillez selectionner une ligne svp ...");
    }
    else
    {
        uiListUser.close();

        int selectedLine = index.row();
        QSqlRecord selectedRecord = userModel->record(selectedLine);

        // Récupération des informations du record sélectionné !
        // Identifiant
        QSqlField field = selectedRecord.field(0);
        QString id = field.value().toString();

        // Nom
        field = selectedRecord.field(1);
        QString nom = field.value().toString();

        // Country
        field = selectedRecord.field(2);
        QString country = field.value().toString();

        // Birthdate
        field = selectedRecord.field(3);
        QString birthdate = field.value().toString();

        // Login
        field = selectedRecord.field(4);
        QString login = field.value().toString();

        // Password
        field = selectedRecord.field(5);
        QString password = field.value().toString();

        // Email
        field = selectedRecord.field(6);
        QString email = field.value().toString();

        // Rôle
        field = selectedRecord.field(7);
        QString role = field.value().toString();

        // Statut
        field = selectedRecord.field(8);
        QString statut = field.value().toString();

        // Remplissage des champs du formulaire avec les informations récupérés de la ligne sélectionnée
        // sur le QTableView de la fenêtre UIListUser
        uiUser.populate(id, nom, country, birthdate, login, password, email, role, statut);

        uiUser.show();
        uiUser.activateUpdate();
        uiUser.setRoleEditable(false);
    }
}

void Controller::onDelete_UIListUser()
{
    qDebug("Enter in function onDelete_UIListUser.");

    QModelIndex index = userModel->getSelectionModel()->currentIndex();
    if (index.isValid() == false)
    {
        uiListUser.warning("Suppression d'une User",
                              "Veuillez selectionner une ligne svp ...");
    }
    else
    {
        bool response = uiListUser.question("Suppression d'une User", "Voulez-vous vraiment supprimer cette User ?");
        if (response == true)
        {
            service.supprimerUnUser();
        }
    }
}

void Controller::onClose_UIListUser()
{
    uiListUser.hide();

    uiUser.reinit();
    uiUser.show();
    uiUser.deactivateUpdate();
    uiUser.setRoleEditable(true);
}


/*...
 * Les slots de la fenêtre UIUser
 */
void Controller::onCreate_UIUser()
{
    qDebug("Enter the function onCreate_UIUser.");

    QString nom = uiUser.getNom();
    QString login = uiUser.getLogin();

    if (nom.isEmpty() || login.isEmpty())
    {

        uiUser.warning("Ajout d'une nouvelle User",
                            "Veuillez renseigner tous les champs svp.");
    }
    else
    {
        QString password = uiUser.getPassword();
        QString country = uiUser.getCountry();
        QString birthdate = uiUser.getBirthdate();
        QString email = uiUser.getEmail();
        QString role = uiUser.getRole();
        QString statut = uiUser.getStatut();

        service.ajouterUnUser (nom, login, password, country, birthdate, email, role, statut);
        uiUser.reinit();
    }
}

void Controller::onFind_UIUser()
{
    qDebug("Enter in function onFind_UIUser.");

    bool ok;
    int id = QInputDialog::getInt(nullptr, tr("Rechercher une User"),
                                             tr("Entrer l'ID de la User : "), 25, 0, 225, 1, &ok);

    if (ok == true) // Click sur le bouton OK
    {
        bool status = service.rechercherUnUser(id);
        if (status == true)
        {
            // La User existe
            uiListUser.setTableViewModel(userModel);
            uiListUser.show();
            uiUser.hide();
        }
        else
        {
            // La User n'existe pas
            uiUser.warning("Rechercher une User", "La User recherchee n'existe pas.");
        }
    }
}

void Controller::onUpdate_UIUser()
{
    QString nom = uiUser.getNom();
    QString login = uiUser.getLogin();

    if (nom.isEmpty() || login.isEmpty())
    {
        uiUser.warning("Mise a jour d'une User",
                            "Veuillez renseigner tous les champs svp.");
    }
    else
    {
        QString password = uiUser.getPassword();
        QString country = uiUser.getCountry();
        QString birthdate = uiUser.getBirthdate();
        QString email = uiUser.getEmail();
        QString role = uiUser.getRole();
        QString statut = uiUser.getStatut();

        service.modifierUnUser (nom, login, password, country, birthdate, email, role, statut);

        uiUser.reinit();
        uiUser.hide();

        uiUser.deactivateUpdate();
        uiListUser.show();
        uiListUser.top();
        uiListUser.updateTitle (connectedUser.getNom());
    }
}

void Controller::onList_UIUser()
{
    uiUser.hide();
    uiListUser.setTableViewModel(userModel);
    uiListUser.show();
    uiListUser.top();
    service.listerLesUsers();
}

void Controller::onClose_UIUser()
{
    uiUser.hide();

    uiLoginIn.reinit();
    uiLoginIn.show();
}

void Controller::onComboBoxRoleChanged_UIUser()
{
    QString role = uiUser.getRole();
    if (role.compare("ADMINISTRATEUR") == 0)
    {
        uiUser.hideStatut();
    }
    else
    {
        uiUser.showStatut();
    }
}

/*
 * Les slots de la fenêtre UIClient
 */
void Controller::onClose_UIClient()
{
    uiClient.hide();

    uiLoginIn.reinit();
    uiLoginIn.show();
}

void Controller::onOuvrir_UIClient()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (index.isValid() == false)
    {
        uiClient.warning("Ouverture de vos comptes",
                              "Veuillez selectionner un de vos comptes svp ...");
    }
    else
    {
        uiClient.hide();
        uiListTransaction.setTableViewModel(transactionModel);
        uiListTransaction.show();
        service.listerLesTransactionsDuCompte();
        uiListTransaction.top();
    }
}

void Controller::onVirement_UIClient()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (index.isValid() == false)
    {
        uiClient.warning("Opération de virement",
                              "Veuillez selectionner un de vos comptes svp ...");
    }
    else
    {
        uiClient.hideTransactionForm();
        uiClient.showTransactionForm("Entrer les informations du virement :", "VIREMENT");
    }
}

void Controller::onRetrait_UIClient()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (index.isValid() == false)
    {
        uiClient.warning("Opération de retrait",
                              "Veuillez selectionner un de vos comptes svp ...");
    }
    else
    {
        uiClient.hideTransactionForm();
        uiClient.showTransactionForm("Entrer les informations du retrait :", "RETRAIT");
    }
}

void Controller::onVersement_UIClient()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (index.isValid() == false)
    {
        uiClient.warning("Opération de depôt",
                              "Veuillez selectionner un de vos comptes svp ...");
    }
    else
    {
        uiClient.hideTransactionForm();
        uiClient.showTransactionForm("Entrer les informations du versement :", "VERSEMENT");
    }
}

void Controller::onHistorique_UIClient()
{
    uiClient.hide();
    uiListTransaction.setTableViewModel(transactionModel);
    uiListTransaction.show();
    service.listerLesTransactions(connectedUser.getId());
    uiListTransaction.top();
}

void Controller::onOK_UIClient()
{
    QMap <QString, QString> input;
    bool ok {true};

    QString typeTransaction = uiClient.getTypeTransaction();
    input.insert("connectedUserId", QString::number(connectedUser.getId()));
    input.insert("typeTransaction", typeTransaction);

    QString montant = uiClient.getMontant();
    if (montant.compare("") == 0)
    {
        uiClient.critical("Contrôle de saisie", "Le montant n'est pas renseigné !!");
        ok = false;
    }
    else
    {
        input.insert("montant", montant);
        // -
        if (typeTransaction.compare("VIREMENT") == 0)
        {
            QString numeroBeneficiaire = uiClient.getNumeroBeneficiaire();
            if (numeroBeneficiaire.compare("") == 0)
            {
                uiClient.critical("Contrôle de saisie", "Le numéro du bénéficiaire n'est pas renseigné !!");
                ok = false;
            }
            else
            {
                qDebug () << numeroBeneficiaire << "- Controller::onOK_UIClient";
                input.insert("numeroBeneficiaire", numeroBeneficiaire);
            }
        }
    }

    if (ok)
    {        
        // -
        QString transaction_message;
        bool status {true};
        qDebug () << typeTransaction << "- Controller::onOK_UIClient";
        service.executeTransaction(input, status, transaction_message);
        if (status == true)
        {
            // -
            uiClient.hideTransactionForm();
            uiClient.information("Fin de la transation", transaction_message);
        }
        else
        {
            uiClient.critical("Echec de la transation", transaction_message);
        }
    }
}

void Controller::onCancel_UIClient()
{
    uiClient.hideTransactionForm();
}

/*
 * Les slots de la fenêtre UIListClient
 */
void Controller::onClose_UIListClient()
{
    uiListClient.hide();
    uiLoginIn.reinit();
    uiLoginIn.show();
}

void Controller::onOuvrir_UIListClient()
{
    QModelIndex index = userModel->getSelectionModel()->currentIndex();

    if (index.isValid() == false)
    {
        uiListUser.warning("Mise a jour des informations d'une User",
                              "Veuillez selectionner une ligne svp ...");
    }
    else
    {
        uiListClient.hide();
        // -
        int selectedLine = index.row();
        QSqlRecord selectedRecord = userModel->record(selectedLine);
        QSqlField field = selectedRecord.field(0);
        int clientId = field.value().toInt();

        field = selectedRecord.field(1);
        QString nomClient = field.value().toString();

        uiListAccount.show();
        uiListAccount.updateTitle (connectedUser.getNom());
        uiListAccount.updateAccountTitle(nomClient);
        service.listerLesComptes(clientId);
        uiListAccount.top();
    }
}

/*
 * Les slots de la fenêtre UIListAccount
 */
void Controller::onNouveau_UIListAccount()
{
    QModelIndex index = userModel->getSelectionModel()->currentIndex();
    // -
    int selectedLine = index.row();
    QSqlRecord selectedRecord = userModel->record(selectedLine);
    QSqlField field = selectedRecord.field(0);
    int idClient = field.value().toInt();

    field = selectedRecord.field(1);
    QString nomClient = field.value().toString();

    uiAccount.updateTitle (nomClient);
    uiListAccount.hide();
    uiAccount.show();
    uiAccount.populate("-1", QString::number(idClient), "", "COURANT", "0");

    uiAccount.deactivateUpdate();
}

void Controller::onModifier_UIListAccount()
{
    QModelIndex userIndex = userModel->getSelectionModel()->currentIndex();
    // -
    int selectedUserLine = userIndex.row();
    QSqlRecord selectedUserRecord = userModel->record(selectedUserLine);
    QSqlField userField = selectedUserRecord.field(0);
    QString idClient = userField.value().toString();
    userField = selectedUserRecord.field(1);
    QString nomClient = userField.value().toString();

    QModelIndex accountIndex = accountModel->getSelectionModel()->currentIndex();
    // -
    int selectedAccountLine = accountIndex.row();
    QSqlRecord selectedAccountRecord = accountModel->record(selectedAccountLine);
    QSqlField accountField = selectedAccountRecord.field(0);
    QString accountId = accountField.value().toString();

    accountField = selectedAccountRecord.field(2);
    QString number = accountField.value().toString();

    accountField = selectedAccountRecord.field(3);
    QString type = accountField.value().toString();

    accountField = selectedAccountRecord.field(4);
    QString balance = accountField.value().toString();

    uiAccount.updateTitle (nomClient);
    uiListAccount.hide();
    uiAccount.show();
    uiAccount.populate(accountId, idClient, number, type, balance);

    uiAccount.deactivateCreate();
}

void Controller::onOuvrir_UIListAccount()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (index.isValid() == false)
    {
        uiClient.warning("Ouverture de vos comptes",
                              "Veuillez selectionner un de vos comptes svp ...");
    }
    else
    {
        uiListAccount.hide();
        uiListTransaction.setTableViewModel(transactionModel);
        uiListTransaction.show();
        service.listerLesTransactionsDuCompte();

        QModelIndex userIndex = userModel->getSelectionModel()->currentIndex();
        // -
        int selectedUserLine = userIndex.row();
        QSqlRecord selectedUserRecord = userModel->record(selectedUserLine);
        QSqlField userField = selectedUserRecord.field(1);
        QString nomClient = userField.value().toString();

        int selectedAccountLine = index.row();
        QSqlRecord selectedAccountRecord = accountModel->record(selectedAccountLine);
        QSqlField accountField = selectedAccountRecord.field(2);
        QString accountNumber = accountField.value().toString();
        uiListTransaction.updateTitle(nomClient, "Les transactions effectuées sur le compte : " + accountNumber);

        uiListTransaction.top();
    }
}

void Controller::onClose_UIListAccount()
{
    uiListAccount.hide();        
    uiListClient.show();
}

void Controller::onClose_UIListTransaction()
{
    uiListTransaction.hide();
    if (connectedUser.getRole().compare("CLIENT") == 0)
        uiClient.show();
    else if (connectedUser.getRole().compare("GESTIONNAIRE") == 0)
        uiListAccount.show();
}

/*
 * Les slots de la fenêtre UIAccount
 */
void Controller::onCreate_UIAccount()
{
    QMap <QString, QString> input;
    bool ok {true};

    QString idClient = uiAccount.getIdClient();
    input.insert("idClient", idClient);
    input.insert("type", uiAccount.getType());
    QString accountNumber = uiAccount.getAccountNumber();
    if (accountNumber.compare("") == 0)
    {
        uiClient.critical("Contrôle de saisie", "Le numero de compte n'est pas renseigné !!");
        ok = false;
    }
    else
    {
        input.insert("number", accountNumber);
        // -
        QString balance = uiAccount.getBalance();
        if (balance.compare("") == 0)
        {
            uiClient.critical("Contrôle de saisie", "Le solde initial n'est pas renseigné !!");
            ok = false;
        }
        else
        {
            input.insert("balance", balance);
        }
    }

    if (ok)
    {
        // -
        QString message;
        service.ajouterUnCompte(input, message);

        uiAccount.information("Fin de la transaction", message);

        uiAccount.populate("-1", idClient, "", "COURANT", "0");
    }
}

void Controller::onUpdate_UIAccount()
{
    QMap <QString, QString> input;
    bool ok {true};

    QString accountId = uiAccount.getAccountId();
    input.insert("accountId", accountId);

    QString idClient = uiAccount.getIdClient();
    input.insert("idClient", idClient);

    QString balance = uiAccount.getBalance();
    input.insert("balance", balance);

    input.insert("type", uiAccount.getType());
    QString accountNumber = uiAccount.getAccountNumber();
    if (accountNumber.compare("") == 0)
    {
        uiClient.critical("Contrôle de saisie", "Le numero de compte n'est pas renseigné !!");
        ok = false;
    }
    else
    {
        input.insert("number", accountNumber);
    }

    if (ok)
    {
        // -
        service.modifierUnCompte(input);
        uiAccount.information("Fin de l'opération", "Compte modifié avec succès.");
        uiAccount.hide();

        uiListAccount.show();
        uiListAccount.top();
    }
}

void Controller::onClose_UIAccount()
{
    uiAccount.hide();
    uiListAccount.show();
}



Controller::~Controller()
{
    qDebug("Controller Destroyed...");

    delete userModel;
}
