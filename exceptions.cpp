#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class InsufficientBalanceException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Erreur : Solde insuffisant pour effectuer ce virement.";
    }
};

#endif // EXCEPTIONS_H
