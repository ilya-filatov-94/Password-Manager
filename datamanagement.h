#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QByteArray>
#include <qaesencryption.h>
#include <qrsaencryption.h>

class DataManagement
{

public:
    explicit DataManagement();
    virtual ~DataManagement();

protected:

    bool readDataSQL(QSqlDatabase&, QSqlQuery&, int, QString&, QString&, QString&);
    void writeDataSQL(QSqlDatabase&, QSqlQuery&, int, QString&, QString&, QString&);
    bool deleteDataSQL(QSqlDatabase&, QSqlQuery&, int);

    QByteArray readDataSQL_settingTable(QSqlDatabase&, QSqlQuery&, QString);
    void writeDataSQL_settingTable(QSqlDatabase&, QSqlQuery&, int, QString, QByteArray&);

    bool readDataSQL_dataTable(QSqlDatabase&, QSqlQuery&, int, QByteArray&, QByteArray&, QByteArray&);
    bool writeDataSQL_dataTable(QSqlDatabase&, QSqlQuery&, int, QByteArray, QByteArray, QByteArray);

    bool readDataSQL_keysTable(QSqlDatabase&, QSqlQuery&, int, QByteArray&, QByteArray&);
    bool writeDataSQL_keysTable(QSqlDatabase&, QSqlQuery&, int, QByteArray, QByteArray);
    bool deleteDataSQL_keysTable(QSqlDatabase&, QSqlQuery&, int);

    QByteArray encryptAES(QString, QByteArray&, QByteArray&);
    QString decryptAES(QByteArray&, QByteArray&, QByteArray&);

    void generateKey(QByteArray&, QByteArray&);

private:

    QRSAEncryption generatorObj;

};

#endif // DATAMANAGEMENT_H

