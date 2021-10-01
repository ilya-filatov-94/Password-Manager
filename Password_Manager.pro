#-------------------------------------------------
#
# Project created by QtCreator 2021-07-19T12:17:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 2.0.0.1
QMAKE_TARGET_COMPANY = Filatov Corp
QMAKE_TARGET_PRODUCT = Password Manager
QMAKE_TARGET_DESCRIPTION = Password Manager
QMAKE_TARGET_COPYRIGHT = Ilya Filatov

TARGET = Password_Manager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += SMTP_BUILD

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        client_window.cpp \
        main.cpp \
        mainwindow.cpp \
        my_encryption.cpp \
        network_connection.cpp \
        smtp/emailaddress.cpp \
        smtp/mimeattachment.cpp \
        smtp/mimecontentformatter.cpp \
        smtp/mimefile.cpp \
        smtp/mimehtml.cpp \
        smtp/mimeinlinefile.cpp \
        smtp/mimemessage.cpp \
        smtp/mimemultipart.cpp \
        smtp/mimepart.cpp \
        smtp/mimetext.cpp \
        smtp/quotedprintable.cpp \
        smtp/smtpclient.cpp \
        tab_template.cpp \
        window_setting.cpp

HEADERS += \
        client_window.h \
        mainwindow.h \
        my_encryption.h \
        network_connection.h \
        smtp/SmtpMime \
        smtp/emailaddress.h \
        smtp/mimeattachment.h \
        smtp/mimecontentformatter.h \
        smtp/mimefile.h \
        smtp/mimehtml.h \
        smtp/mimeinlinefile.h \
        smtp/mimemessage.h \
        smtp/mimemultipart.h \
        smtp/mimepart.h \
        smtp/mimetext.h \
        smtp/quotedprintable.h \
        smtp/smtpclient.h \
        smtp/smtpexports.h \
        tab_template.h \
        window_setting.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_ICONS = "ico4017.ico"
