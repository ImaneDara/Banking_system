#ifndef NOTIFICATIONTYPE_H
#define NOTIFICATIONTYPE_H

#include <QString>

enum class NotificationType {
    TRANSACTION_SUCCESS,
    VIREMENT_VALIDE,
    VIREMENT_REJETE,
    VERSEMENT_VALIDE,
    VERSEMENT_REJETE,
    RETRAIT_VALIDE,
    RETRAIT_REJETE,
    PRIVATE_MESSAGE
};


namespace NotificationHelper {
static QString typeToString(NotificationType type) {
    switch(type) {
    case NotificationType::TRANSACTION_SUCCESS: return "TRANSACTION_SUCCESS";
    case NotificationType::VIREMENT_VALIDE: return "VIREMENT_VALIDE";
    case NotificationType::VIREMENT_REJETE: return "VIREMENT_REJETE";
    case NotificationType::VERSEMENT_VALIDE: return "VERSEMENT_VALIDE";
    case NotificationType::VERSEMENT_REJETE: return "VERSEMENT_REJETE";
    case NotificationType::RETRAIT_VALIDE: return "RETRAIT_VALIDE";
    case NotificationType::RETRAIT_REJETE: return "RETRAIT_REJETE";
    case NotificationType::PRIVATE_MESSAGE: return "PRIVATE_MESSAGE";
    default: return "UNKNOWN";
    }
}

static QString getTitle(NotificationType type) {
    switch(type) {
    case NotificationType::TRANSACTION_SUCCESS: return "Transaction réussie";
    case NotificationType::VIREMENT_VALIDE: return "Virement validé";
    case NotificationType::VIREMENT_REJETE: return "Virement rejeté";
    case NotificationType::VERSEMENT_VALIDE: return "VERSEMENT_VALIDE";
    case NotificationType::VERSEMENT_REJETE: return "VERSEMENT_REJETE";
    case NotificationType::RETRAIT_VALIDE: return "Retrait validé";
    case NotificationType::RETRAIT_REJETE: return "Retrait rejeté";
    case NotificationType::PRIVATE_MESSAGE: return "Nouveau message";
    default: return "Notification";
    }
}

static QString getMessage(NotificationType type, const QString& additionalInfo = "") {
    switch(type) {
    case NotificationType::TRANSACTION_SUCCESS:
        return "Votre transaction a été effectuée avec succès. " + additionalInfo;
    case NotificationType::VIREMENT_VALIDE:
        return "Votre virement a été validé. " + additionalInfo;
    case NotificationType::VIREMENT_REJETE:
        return "Votre virement a été rejeté. " + additionalInfo;
    case NotificationType::VERSEMENT_VALIDE:
        return "Votre versement a été validé. " + additionalInfo;
    case NotificationType::VERSEMENT_REJETE:
        return "Votre versement a été rejeté. " + additionalInfo;
    case NotificationType::RETRAIT_VALIDE:
        return "Votre retrait a été validé. " + additionalInfo;
    case NotificationType::RETRAIT_REJETE:
        return "Votre retrait a été rejeté. " + additionalInfo;
    case NotificationType::PRIVATE_MESSAGE:
        return "Vous avez reçu un nouveau message. " + additionalInfo;
    default: return "Nouvelle notification. " + additionalInfo;
    }
}
}

#endif // NOTIFICATIONTYPE_H
