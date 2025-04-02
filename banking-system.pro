QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    account.cpp \
    accountmodel.cpp \
    exceptions.cpp \
    main.cpp \
    transaction.cpp \
    uiabstractwindow.cpp \
    controller.cpp \
    dbmanager.cpp \
    service.cpp \
    uiclient.cpp \
    uilistuser.cpp \
    uilistclient.cpp \
    uilistaccount.cpp \
    uiloginin.cpp \
    uiuser.cpp \
    user.cpp \
    usermodel.cpp \
    transactionmodel.cpp \
    uilisttransaction.cpp \
    uiaccount.cpp

HEADERS += \
    account.h \
    accountmodel.h \
    exceptions.h \
    transaction.h \
    uiabstractwindow.h \
    controller.h \
    dbmanager.h \
    service.h \
    uiclient.h \
    uilistuser.h \
    uilistclient.h \
    uilistaccount.h \
    uiloginin.h \
    uiuser.h \
    user.h \
    usermodel.h \
    transactionmodel.h \
    uilisttransaction.h \
    uiaccount.h

FORMS += \
    uiclient.ui \
    uilistuser.ui \
    uilistclient.ui \
    uilistaccount.ui \
    uiloginin.ui \
    uiuser.ui \
    uilisttransaction.ui \
    uiaccount.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
