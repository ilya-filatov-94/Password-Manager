#include "datamanagement.h"


DataManagement::DataManagement()
{
    generatorObj = QRSAEncryption::Rsa::RSA_256;
}

DataManagement:: ~DataManagement()
{
}

bool DataManagement::readDataSQL(QSqlDatabase& db, QSqlQuery& query1, int id, QString& name, QString& login, QString& pas)
{
    QByteArray decArr[3] = {"", "", ""};
    if (!db.tables().contains(QLatin1String("datatable")))  //Проверка существует ли такая таблица в БД
    {
        return false;
    }
    query1.exec(QString("select exists (select * from datatable where id = '%1');").arg(id));
    query1.next();
    if (query1.value(0) !=0)    //Запись с данным id существует в таблице
    {
        query1.exec(QString("select * from datatable where id = '%1'").arg(id));
        QSqlRecord rec = query1.record();
        int i=0;
        while ((query1.next())&&(i<id))
        {
            decArr[i] = query1.value(rec.indexOf("name_resource")).toByteArray();
            decArr[i+1] = query1.value(rec.indexOf("login_resource")).toByteArray();
            decArr[i+2] = query1.value(rec.indexOf("password_resource")).toByteArray();
            i+=3;
        }
        QByteArray keyN, vectorN;
        if (readDataSQL_vectorsTable(db, query1, id, keyN, vectorN))
        {
            name = decryptAES(decArr[0], keyN, vectorN);
            login = decryptAES(decArr[1], keyN, vectorN);
            pas = decryptAES(decArr[2], keyN, vectorN);
            query1.clear();
            return true;
        }
        else {
            query1.clear();
            return false;
        }
    }
    else
    {
        query1.clear();
        return false;
    }
}

void DataManagement::writeDataSQL(QSqlDatabase& db, QSqlQuery& query1, int id, QString& name, QString& login, QString& pas)
{
    QByteArray encdArr[3] = {"", "", ""};
    QByteArray keyN, vectorN;
    generatorObj.generatePairKey(keyN, vectorN);     //Генерируем ключ и вектор

    encdArr[0] = encryptAES(name, keyN, vectorN);
    encdArr[1] = encryptAES(login, keyN, vectorN);
    encdArr[2] = encryptAES(pas, keyN, vectorN);

    if (!db.tables().contains(QLatin1String("datatable")))  //Проверка существует ли такая таблица в БД
    {
        query1.exec("create table datatable (id int primary key, name_resource nvarchar(300), login_resource nvarchar(300), password_resource nvarchar(300))");
    }
    //Проверка существования записи с нужным id
    query1.exec(QString("select exists (select name_resource from datatable where id = '%1');").arg(id));
    query1.next();
    if (query1.value(0) !=0)    //Запись с данным id существовала уже в БД
    {
        query1.prepare(QString("update datatable set name_resource=?, login_resource=?, password_resource=? where id ='%1'").arg(id));
        query1.addBindValue(encdArr[0]);
        query1.addBindValue(encdArr[1]);
        query1.addBindValue(encdArr[2]);
        if (!query1.exec()) {
            query1.clear();
        }
    }
    else
    {
        query1.prepare("insert into datatable (id, name_resource, login_resource, password_resource) values (?, ?, ?, ?)");
        query1.addBindValue(id);
        query1.addBindValue(encdArr[0]);
        query1.addBindValue(encdArr[1]);
        query1.addBindValue(encdArr[2]);
        if (!query1.exec()) {
            query1.clear();
        }
    }
    if (writeDataSQL_vectorsTable(db, query1, id, "key"+QString::number(id), keyN, "iv"+QString::number(id), vectorN))  {
        query1.clear();
    }
    else {
        query1.clear();
    }
}

bool DataManagement::deleteDataSQL(QSqlDatabase& db, QSqlQuery& query1, int id)
{
    if (!db.tables().contains(QLatin1String("datatable")))  //Проверка существует ли такая таблица в БД
    {
        query1.clear();
        return true;
    }
    query1.exec(QString("select exists (select * from datatable where id = '%1');").arg(id));
    query1.next();
    if (query1.value(0) !=0)
    {
        if (!query1.exec(QString("delete from datatable where id = '%1'").arg(id))) {
            query1.clear();
            return false;
        }
        else {
            if (deleteDataSQL_vectorsTable(db, query1, id))
                {
                query1.clear();
                return true;}
            else {
                query1.clear();
                return false;}
        }
    }
    else
    {
        query1.clear();
        return true;
    }
}

bool DataManagement::deleteDataSQL_vectorsTable(QSqlDatabase& db, QSqlQuery& query, int id)
{
    if (!db.tables().contains(QLatin1String("vectorstable")))  //Проверка существует ли такая таблица в БД
    {
        query.clear();
        return true;
    }
    query.exec(QString("select exists (select * from vectorstable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) !=0)
    {
        if (!query.exec(QString("delete from vectorstable where id = '%1'").arg(id))) {
            query.clear();
            return false;
        }
        else {
            query.clear();
            return true;}
    }
    else {
        query.clear();
        return true;
    }
}

bool DataManagement::writeDataSQL_vectorsTable(QSqlDatabase& db, QSqlQuery& query, int id, QString parameter1, QByteArray value1, QString parameter2, QByteArray value2)
{
    if (!db.tables().contains(QLatin1String("vectorstable")))  //Проверка существует ли такая таблица в БД
    {
        query.exec("create table vectorstable (id int primary key, parameter1 nvarchar(300), value1 nvarchar(300), parameter2 nvarchar(300), value2 nvarchar(300))");
    }
     //Проверка существования записи с нужным id
     query.exec(QString("select exists (select * from vectorstable where id = '%1');").arg(id));
     query.next();
     if (query.value(0) !=0)
     {
        query.prepare(QString("update vectorstable set parameter1=?, value1=?, parameter2=?, value2=? where id ='%1'").arg(id));
        query.addBindValue(parameter1);
        query.addBindValue(value1);
        query.addBindValue(parameter2);
        query.addBindValue(value2);
        if (!query.exec()) {
            query.clear();
            return false;
        }
        query.clear();
        return true;
     }
     else
     {
        query.prepare("insert into vectorstable (id, parameter1, value1, parameter2, value2) values (?, ?, ?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(parameter1);
        query.addBindValue(value1);
        query.addBindValue(parameter2);
        query.addBindValue(value2);

        if (!query.exec()) {
            query.clear();
            return false;
        }
        query.clear();
        return true;
     }
}

bool DataManagement::readDataSQL_vectorsTable(QSqlDatabase & db, QSqlQuery& query, int id, QByteArray& value1, QByteArray& value2)
{
    if (!db.tables().contains(QLatin1String("vectorstable")))  //Проверка существует ли такая таблица в БД
    {
        query.clear();
        return false;
    }
    else {
        query.exec(QString("select exists (select * from vectorstable where id = '%1');").arg(id));
        query.next();
        if (query.value(0) !=0)
        {
            //Читаем записи с нужным параметром
            query.exec(QString("select * from vectorstable where id = '%1'").arg(id));
            QSqlRecord rec = query.record();
            while (query.next())
            {
                value1 = query.value(rec.indexOf("value1")).toByteArray();
                value2 = query.value(rec.indexOf("value2")).toByteArray();
            }
            query.clear();
            return true;
        }
        else {
            query.clear();
            return false;
        }
    }
}

void DataManagement::writeDataSQL_settingTable(QSqlDatabase& db, QSqlQuery& query, int id, QString parameter, QByteArray& value)
{
    if (!db.tables().contains(QLatin1String("settingtable")))  //Проверка существует ли такая таблица в БД
    {
        query.exec("create table settingtable (id int primary key, parameter nvarchar(300), value nvarchar(300))");
    }
     //Проверка существования записи с нужным id
     query.exec(QString("select exists (select * from settingtable where id = '%1');").arg(id));
     query.next();
     if (query.value(0) !=0)    //Запись с данным id существовала уже в БД
     {
        query.prepare(QString("update settingtable set parameter=?, value=? where id ='%1'").arg(id));
        query.addBindValue(parameter);
        query.addBindValue(value);

        if (!query.exec()) {
            query.clear();
        }
        query.clear();
     }
     else    //Запись с данным id НЕ СУЩЕСТВОВУЕТ в БД
     {
        query.prepare("insert into settingtable (id, parameter, value) values (?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(parameter);
        query.addBindValue(value);

        if (!query.exec()) {
            query.clear();
        }
        query.clear();
     }
}

QByteArray DataManagement::readDataSQL_settingTable(QSqlDatabase& db, QSqlQuery& query, QString parameter)
{
    QByteArray value;
    if (!db.tables().contains(QLatin1String("settingtable")))  //Проверка существует ли такая таблица в БД
    {
        return QString("Ошибка чтения, таблица не существует!").toLocal8Bit();
    }
    else {
        query.exec(QString("select exists (select * from settingtable where parameter = '%1');").arg(parameter));
        query.next();
        if (query.value(0) !=0)    //Запись с данным параметром существует в таблице
        {
            query.exec(QString("select * from settingtable where parameter = '%1'").arg(parameter));
            QSqlRecord rec = query.record();
            while (query.next())
            {
                value = query.value(rec.indexOf("value")).toByteArray();
            }
            query.clear();
            return value;
        }
        else {
            query.clear();
            return QString("Ошибка, параметр не существует в запрашиваемой таблице").toLocal8Bit();
        }
    }
}

void DataManagement::generateKey(QByteArray& key, QByteArray& iv)
{
    generatorObj.generatePairKey(key, iv);
}

QByteArray DataManagement::encryptAES(QString inputStr, QByteArray& key, QByteArray& iv)
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QByteArray hashKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv, QCryptographicHash::Md5);

    QByteArray encodeText = encryption.encode(inputStr.toLocal8Bit(), hashKey, hashIV);

    return encodeText;
}

QString DataManagement::decryptAES(QByteArray& text, QByteArray& key, QByteArray& iv)
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QByteArray hashKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv, QCryptographicHash::Md5);

    QByteArray decodeText = encryption.decode(text, hashKey, hashIV);

    QString decodedString = QString::fromLocal8Bit(decodeText);
    decodedString.chop(1);

    return decodedString;
}
