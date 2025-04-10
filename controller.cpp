#include "controller.h"
#include <QRegularExpression>
#include <QDebug>          // For qCritical(), qDebug()
#include <QSqlError>       // For QSqlError (used by lastError())
#include <QSqlQuery>
#include <QMessageBox>

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

    // Basic field validation
    QString nom = uiUser.getNom();
    QString login = uiUser.getLogin();
    QString password = uiUser.getPassword();
    QString email = uiUser.getEmail();
    QString statut = uiUser.getStatut();

    if (nom.isEmpty() || login.isEmpty() || password.isEmpty() ||
        email.isEmpty())
    {
        uiUser.warning("Champs obligatoires",
                       "Veuillez renseigner tous les champs obligatoires svp.");
        return;
    }

    // Email format validation
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!emailRegex.match(email).hasMatch())
    {
        uiUser.warning("Format email invalide",
                       "L'adresse email doit être au format valide (ex: user@example.com)");
        return;
    }

    // Password strength validation
    if (password.length() < 6)
    {
        uiUser.warning("Mot de passe faible",
                       "Le mot de passe doit contenir au moins 6 caractères");
        return;
    }

    if (!password.contains(QRegularExpression("[0-9]")))
    {
        uiUser.warning("Mot de passe faible",
                       "Le mot de passe doit contenir au moins un chiffre");
        return;
    }

    // Check if login already exists
    if (service.loginExists(login))
    {
        uiUser.warning("Login existant",
                       "Ce nom d'utilisateur est déjà utilisé, veuillez en choisir un autre");
        return;
    }

    // Additional fields
    QString country = uiUser.getCountry();
    QString birthdate = uiUser.getBirthdate();
    QString role = uiUser.getRole();

    // All validations passed - proceed with creation
    service.ajouterUnUser(nom, login, password, country, birthdate, email, role, statut);
    uiUser.reinit();
    uiUser.information("Succès", "Utilisateur créé avec succès");
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
    // Basic field validation
    QString nom = uiUser.getNom();
    QString login = uiUser.getLogin();
    QString email = uiUser.getEmail();
    QString statut = uiUser.getStatut();

    if (nom.isEmpty() || login.isEmpty() || email.isEmpty())
    {
        uiUser.warning("Champs obligatoires",
                       "Veuillez renseigner tous les champs obligatoires svp.");
        return;
    }

    // Email format validation
    QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!emailRegex.match(email).hasMatch())
    {
        uiUser.warning("Format email invalide",
                       "L'adresse email doit être au format valide (ex: user@example.com)");
        return;
    }

    // Password validation (only if changed)
    QString password = uiUser.getPassword();
    if (!password.isEmpty())
    {
        if (password.length() < 6)
        {
            uiUser.warning("Mot de passe faible",
                           "Le mot de passe doit contenir au moins 6 caractères");
            return;
        }

        if (!password.contains(QRegularExpression("[0-9]")))
        {
            uiUser.warning("Mot de passe faible",
                           "Le mot de passe doit contenir au moins un chiffre");
            return;
        }
    }

    // Additional fields
    QString country = uiUser.getCountry();
    QString birthdate = uiUser.getBirthdate();
    QString role = uiUser.getRole();

    // Birthdate validation (optional)
    if (!birthdate.isEmpty())
    {
        QDate date = QDate::fromString(birthdate, "dd/MM/yyyy");
        if (!date.isValid() || date > QDate::currentDate())
        {
            uiUser.warning("Date invalide",
                           "La date de naissance doit être une date valide");
            return;
        }
    }

    // All validations passed - proceed with update
    service.modifierUnUser(nom, login, password, country, birthdate, email, role, statut);

    uiUser.reinit();
    uiUser.hide();
    uiUser.information("Succès", "Utilisateur modifié avec succès");

    uiUser.deactivateUpdate();
    uiListUser.show();
    uiListUser.top();
    uiListUser.updateTitle(connectedUser.getNom());
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
    if (!index.isValid())
    {
        uiClient.warning("Ouverture de vos comptes",
                         "Veuillez sélectionner un de vos comptes svp...");
        return;
    }

    // Get account information
    QSqlRecord record = accountModel->record(index.row());

    // Check if account is frozen (GELER)
    QString statut = record.value("statut").toString().trimmed().toUpper();
    if (statut == "GELER") {
        uiClient.warning("Compte gelé",
                         "Ce compte est gelé. Aucune opération n'est autorisée.");
        return;
    }

    uiClient.hide();
    uiListTransaction.setTableViewModel(transactionModel);
    uiListTransaction.show();
    service.listerLesTransactionsDuCompte();
    uiListTransaction.top();

    QString role = connectedUser.getRole();

    if (role == "CLIENT" || role == "ADMINISTRATEUR") {
        uiListTransaction.disableActionButtons();
    } else if (role == "GESTIONNAIRE"){
        uiListTransaction.enableActionButtons();
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

    QString role = connectedUser.getRole();

    if (role == "CLIENT" || role == "ADMINISTRATEUR") {
        uiListTransaction.disableActionButtons();
    } else if (role == "GESTIONNAIRE"){
        uiListTransaction.enableActionButtons();
    }
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

void Controller::onProfile_UIClient()
{
    qDebug() << "Profile button clicked";

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database not open!";
        uiClient.critical("Database Error", "Database connection is not available");
        return;
    }

    qDebug() << "Fetching client data for ID:" << connectedUser.getId();

    QMap<QString, QString> clientData;
    if (service.getClientInfo(connectedUser.getId(), clientData)) {
        qDebug() << "Client data retrieved:" << clientData;
        uiClient.showProfileForm(clientData);
    } else {
        qDebug() << "Failed to get client info"; // Debug 5
        uiClient.warning("Profil", "Impossible de charger les informations du client");
    }
}

void Controller::onNotif_UIClient()
{
    uiClient.hide();
    uiListNotif.setNotificationModel(notificationModel);
    uiListNotif.refreshNotifications(connectedUser.getId());

    uiListNotif.show();
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

        // Debug checks
        qDebug() << "Account model row count:" << accountModel->rowCount();
        qDebug() << "Account model column count:" << accountModel->columnCount();


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
    uiAccount.populate("-1", QString::number(idClient), "", "COURANT", "0", "ACTIF");

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

    accountField = selectedAccountRecord.field(5);
    QString statut = accountField.value().toString();

    uiAccount.updateTitle (nomClient);
    uiListAccount.hide();
    uiAccount.show();
    uiAccount.populate(accountId, idClient, number, type, balance, statut);

    uiAccount.deactivateCreate();
}

void Controller::onOuvrir_UIListAccount()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (!index.isValid()) {
        uiClient.warning("Ouverture de vos comptes",
                         "Veuillez sélectionner un de vos comptes svp...");
        return;
    }

    // Get account information
    int selectedLine = index.row();
    QSqlRecord record = accountModel->record(selectedLine);

    // Check account status
    QString statut = record.value("statut").toString().trimmed().toUpper();
    if (statut == "GELER") {
        uiClient.warning("Compte gelé",
                         "Ce compte est gelé. Aucune opération n'est autorisée.");
        return;
    }

    // Get the account number
    QString accountNumber = record.value("number").toString();

    // Get client name
    QModelIndex userIndex = userModel->getSelectionModel()->currentIndex();
    if (!userIndex.isValid()) {
        uiClient.warning("Erreur", "Impossible de récupérer les informations du client.");
        return;
    }

    QString nomClient = userModel->record(userIndex.row()).value("nom").toString();

    // Configure transaction list
    uiListAccount.hide();
    uiListTransaction.setTableViewModel(transactionModel);
    uiListTransaction.setAccountNumber(accountNumber);
    uiListTransaction.updateTitle(nomClient,
                                  "Les transactions effectuées sur le compte : " + accountNumber);

    // Load transactions for this specific account
    service.listerLesTransactionsDuCompte();

    uiListTransaction.show();
    uiListTransaction.top();
}

void Controller::onGeler_UIListAccount()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (!index.isValid()) {
        uiClient.warning("Sélection requise", "Veuillez sélectionner un compte");
        return;
    }

    int selectedLine = index.row();
    QSqlRecord record = accountModel->record(selectedLine);
    QString accountId = record.field(0).value().toString();

    QMap<QString, QString> input;
    input.insert("accountId", accountId);
    input.insert("statut", "GELER");

    // Keep other values unchanged
    input.insert("idClient", record.field(1).value().toString());
    input.insert("number", record.field(2).value().toString());
    input.insert("type", record.field(3).value().toString());
    input.insert("balance", record.field(4).value().toString());

    service.modifierUnCompte(input);
    uiClient.information("Succès", "Le compte a été gelé avec succès");
}

void Controller::onActiver_UIListAccount()
{
    QModelIndex index = accountModel->getSelectionModel()->currentIndex();
    if (!index.isValid()) {
        uiClient.warning("Sélection requise", "Veuillez sélectionner un compte");
        return;
    }

    int selectedLine = index.row();
    QSqlRecord record = accountModel->record(selectedLine);
    QString accountId = record.field(0).value().toString();

    QMap<QString, QString> input;
    input.insert("accountId", accountId);
    input.insert("statut", "ACTIF");

    // Keep other values unchanged
    input.insert("idClient", record.field(1).value().toString());
    input.insert("number", record.field(2).value().toString());
    input.insert("type", record.field(3).value().toString());
    input.insert("balance", record.field(4).value().toString());

    service.modifierUnCompte(input);
    uiClient.information("Succès", "Le compte a été activé avec succès");
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
    QMap<QString, QString> input;
    bool ok = true;

    QString idClient = uiAccount.getIdClient();
    input.insert("idClient", idClient);
    input.insert("type", uiAccount.getType());

    // Add status to input (default to "ACTIF" for new accounts)
    input.insert("statut", "ACTIF");

    QString accountNumber = uiAccount.getAccountNumber();
    if (accountNumber.isEmpty())
    {
        uiClient.critical("Contrôle de saisie", "Le numero de compte n'est pas renseigné !!");
        ok = false;
    }
    else
    {
        input.insert("number", accountNumber);

        QString balance = uiAccount.getBalance();
        if (balance.isEmpty())
        {
            uiClient.critical("Contrôle de saisie", "Le solde initial n'est pas renseigné !!");
            ok = false;
        }
        else
        {
            bool conversionOk;
            double balanceValue = balance.toDouble(&conversionOk);
            if (conversionOk && balanceValue < 0)
            {
                uiClient.critical("Contrôle de saisie", "Le solde ne peut pas être négatif");
                ok = false;
            }
            else
            {
                input.insert("balance", balance);
            }
        }
    }

    if (ok)
    {
        service.ajouterUnCompte(input);
        uiAccount.information("Fin de la transaction", "Compte ajouté avec succès.");
        uiAccount.populate("-1", idClient, "", "COURANT", "0", "ACTIF");
    }
}

void Controller::onUpdate_UIAccount()
{
    QMap<QString, QString> input;
    bool ok = true;

    QString accountId = uiAccount.getAccountId();
    if (accountId.isEmpty()) {
        uiClient.critical("Contrôle de saisie", "Veuillez sélectionner un compte à modifier !");
        return;
    }
    input.insert("accountId", accountId);

    QString idClient = uiAccount.getIdClient();
    input.insert("idClient", idClient);

    // Add status from combo box
    input.insert("statut", uiAccount.getStatut());

    // Balance validation
    QString balance = uiAccount.getBalance();
    if (balance.isEmpty()) {
        uiClient.critical("Contrôle de saisie", "Le solde n'est pas renseigné !");
        ok = false;
    } else {
        bool conversionOk;
        double balanceValue = balance.toDouble(&conversionOk);
        if (!conversionOk) {
            uiClient.critical("Contrôle de saisie", "Le solde doit être un nombre valide !");
            ok = false;
        } else if (balanceValue < 0) {
            uiClient.critical("Contrôle de saisie", "Le solde ne peut pas être négatif !");
            ok = false;
        } else {
            input.insert("balance", balance);
        }
    }

    input.insert("type", uiAccount.getType());

    QString accountNumber = uiAccount.getAccountNumber();
    if (accountNumber.isEmpty()) {
        uiClient.critical("Contrôle de saisie", "Le numéro de compte n'est pas renseigné !");
        ok = false;
    } else {
        input.insert("number", accountNumber);
    }

    if (ok) {
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

void Controller::onFiltrerClicked()
{
    QString selectedFilter = uiListTransaction.getSelectedFilter();// "Type", "Date", or "Balance"

    TransactionModel* model = uiListTransaction.getTransactionModel();
    if (!model) return;

    int currentUserId = connectedUser.getId();

    if (selectedFilter == "Type") {
        QString transactionType = uiListTransaction.getTransactionType();
        if (!transactionType.isEmpty()) {
            model->filtrerTransactions(transactionType, "", false, currentUserId);
        }
    }
    else if (selectedFilter == "Date") {
        // Use QDateEdit or a simple QInputDialog with text validation
        bool ok;
        QString dateStr = uiListTransaction.getDateStr();

        if (ok && !dateStr.isEmpty()) {
            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            if (date.isValid()) {
                model->filtrerTransactions("Tous", dateStr, false, currentUserId);
            } else {
                uiListTransaction.warning("Invalid Date", "Please enter a valid date in YYYY-MM-DD format.");
            }
        }
    }
    else if (selectedFilter == "Balance") {
        model->filtrerTransactions("Tous", "", true, currentUserId); // Sort by balance ascending
    }
}

void Controller::onValiderClicked()
{
    if (!uiListTransaction.getSelectedTransactionIndex().isValid()) {
        QMessageBox::warning(&uiListTransaction, "Sélection requise", "Veuillez sélectionner une transaction");
        return;
    }

    if (uiListTransaction.validateSelectedTransaction()) {
        QMessageBox::information(&uiListTransaction, "Succès", "Transaction validée avec succès.");
    }
}

void Controller::onRejeterClicked()
{
    if (!uiListTransaction.getSelectedTransactionIndex().isValid()) {
        QMessageBox::warning(&uiListTransaction, "Sélection requise", "Veuillez sélectionner une transaction à rejeter.");
        return;
    }

    if (uiListTransaction.rejectSelectedTransaction()) {
        QMessageBox::information(&uiListTransaction, "Succès", "Transaction rejetée.");
    }
}

/*
 * Les slots de la fenêtre UIListNotif
 */
void Controller::onClose_UIListNotif()
{
    uiListNotif.hide();
    uiListClient.show();

}

void Controller::onOuvrir_UIListNotif()
{
    uiListNotif.show();
}
void Controller::onMessage_UIListClient()
{

     // uiListNotif.show();
}

Controller::~Controller()
{
    qDebug("Controller Destroyed...");

    delete userModel;
}
