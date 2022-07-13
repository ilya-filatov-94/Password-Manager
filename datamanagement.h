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

    bool readDataSQL(QSqlDatabase&, QSqlQuery&, int, QString&, QString&, QString&);      //чтение данных приложения при открытии
    void writeDataSQL(QSqlDatabase&, QSqlQuery&, int, QString&, QString&, QString&);     //запись данных приложения по нажатию кнопки
    //bool writeDataSQL(QSqlDatabase&, QSqlQuery&, int, QString&, QString&, QString&);     //запись данных приложения по нажатию кнопки
    bool deleteDataSQL(QSqlDatabase&, QSqlQuery&, int);  //Удаление записей из БД

    QByteArray readDataSQL_settingTable(QSqlDatabase&, QSqlQuery&, QString);                 //чтение данных из таблицы настроек
    void writeDataSQL_settingTable(QSqlDatabase&, QSqlQuery&, int, QString, QByteArray&);     //запись данных в таблицу настроек

    bool readDataSQL_vectorsTable(QSqlDatabase&, QSqlQuery&, int, QByteArray&, QByteArray&);   //чтение из таблицы векторов
    bool writeDataSQL_vectorsTable(QSqlDatabase&, QSqlQuery&, int, QString, QByteArray, QString, QByteArray); //запись в таблицу векторов

    bool deleteDataSQL_vectorsTable(QSqlDatabase&, QSqlQuery&, int);  //Удаление записей из таблицы векторов

    QByteArray encryptAES(QString, QByteArray&, QByteArray&);
    QString decryptAES(QByteArray&, QByteArray&, QByteArray&);

    void generateKey(QByteArray&, QByteArray&);

private:

    QRSAEncryption generatorObj;

};

#endif // DATAMANAGEMENT_H

