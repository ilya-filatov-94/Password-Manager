#include "datamanagement.h"


DataManagement::DataManagement()
{
    generatorObj = QRSAEncryption::Rsa::RSA_256;
}

DataManagement:: ~DataManagement()
{
}

bool DataManagement::readDataSQL(QSqlDatabase& db, QSqlQuery& query, int id, QString& name, QString& login, QString& pas)
{
    QByteArray data1, data2, data3, key, vector;
    if (!readDataSQL_dataTable(db, query, id, data1, data2, data3)) {
        query.clear();
        return false;
    }
    if (!readDataSQL_keysTable(db, query, id, key, vector)) {
        query.clear();
        return false;
    }
    name = decryptAES(data1, key, vector);
    login = decryptAES(data2, key, vector);
    pas = decryptAES(data3, key, vector);
    query.clear();
    return true;
}

void DataManagement::writeDataSQL(QSqlDatabase& db, QSqlQuery& query, int id, QString& name, QString& login, QString& pas)
{
    QByteArray data1, data2, data3, key, vector;
    generatorObj.generatePairKey(key, vector);
    data1 = encryptAES(name, key, vector);
    data2 = encryptAES(login, key, vector);
    data3 = encryptAES(pas, key, vector);
    if (!writeDataSQL_dataTable(db, query, id, data1, data2, data3)) {
        query.clear();
    }
    if (!writeDataSQL_keysTable(db, query, id, key, vector))  {
        query.clear();
    }
    query.clear();
}

bool DataManagement::readDataSQL_dataTable(QSqlDatabase& db, QSqlQuery& query, int id, QByteArray& name, QByteArray& login, QByteArray& pas)
{
    if (!db.tables().contains(QLatin1String("datatable")))
    {
        return false;
    }
    query.exec(QString("select exists (select name_resource, login_resource, password_resource from datatable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) !=0)
    {
        query.exec(QString("select name_resource, login_resource, password_resource from datatable where id = '%1'").arg(id));
        QSqlRecord rec = query.record();
        while (query.next())
        {
            name = query.value(rec.indexOf("name_resource")).toByteArray();
            login = query.value(rec.indexOf("login_resource")).toByteArray();
            pas = query.value(rec.indexOf("password_resource")).toByteArray();
        }
        query.clear();
        return true;
    }
    query.clear();
    return false;
}

bool DataManagement::writeDataSQL_dataTable(QSqlDatabase& db, QSqlQuery& query, int id, QByteArray name, QByteArray login, QByteArray pas)
{
    if (!db.tables().contains(QLatin1String("datatable")))  
    {
        query.exec("create table datatable (id int primary key, name_resource nvarchar(300), login_resource nvarchar(300), password_resource nvarchar(300))");
    }
    
    query.exec(QString("select exists (select name_resource, login_resource, password_resource from datatable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) !=0) {
        query.prepare(QString("update datatable set name_resource=?, login_resource=?, password_resource=? where id ='%1'").arg(id));
        query.addBindValue(name);
        query.addBindValue(login);
        query.addBindValue(pas);
        if (!query.exec()) {
            query.clear();
            return false;
        }
        query.clear();
        return true;
    }
    else {
        query.prepare("insert into datatable (id, name_resource, login_resource, password_resource) values (?, ?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(name);
        query.addBindValue(login);
        query.addBindValue(pas);
        if (!query.exec()) {
            query.clear();
            return false;
        }
        query.clear();
        return true;
    }
}

bool DataManagement::deleteDataSQL(QSqlDatabase& db, QSqlQuery& query, int id)
{
    if (!db.tables().contains(QLatin1String("datatable"))) {
        query.clear();
        return true;
    }
    query.exec(QString("select exists (select * from datatable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) == 0) {
        query.clear();
        return true;
    }
    if (query.exec(QString("delete from datatable where id = '%1'").arg(id))) {
        if (deleteDataSQL_keysTable(db, query, id)) {
            query.clear();
            return true;
        }
    }
    query.clear();
    return false;
}

bool DataManagement::deleteDataSQL_keysTable(QSqlDatabase& db, QSqlQuery& query, int id)
{
    if (!db.tables().contains(QLatin1String("keystable"))) {
        return true;
    }
    query.exec(QString("select exists (select * from keystable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) == 0) {
        query.clear();
        return true;
    }
    if (!query.exec(QString("delete from keystable where id = '%1'").arg(id))) {
        query.clear();
        return false;
    }
    query.clear();
    return true;
}

bool DataManagement::writeDataSQL_keysTable(QSqlDatabase& db, QSqlQuery& query, int id, QByteArray value1, QByteArray value2)
{
    if (!db.tables().contains(QLatin1String("keystable"))) {
        query.exec("create table keystable (id int primary key, value1 nvarchar(300), value2 nvarchar(300))");
    }
    query.exec(QString("select exists (select value1, value2 from keystable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) !=0) {
        query.prepare(QString("update keystable set value1=?, value2=? where id ='%1'").arg(id));
        query.addBindValue(value1);
        query.addBindValue(value2);
        if (!query.exec()) {
            query.clear();
            return false;
        }
        query.clear();
        return true;
    }
    else {
        query.prepare("insert into keystable (id, value1, value2) values (?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(value1);
        query.addBindValue(value2);
        if (!query.exec()) {
            query.clear();
            return false;
        }
        query.clear();
        return true;
     }
}

bool DataManagement::readDataSQL_keysTable(QSqlDatabase& db, QSqlQuery& query, int id, QByteArray& value1, QByteArray& value2)
{
    if (!db.tables().contains(QLatin1String("keystable")))  {
        return false;
    }
    query.exec(QString("select exists (select value1, value2 from keystable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) !=0) {
        query.exec(QString("select value1, value2 from keystable where id = '%1'").arg(id));
        QSqlRecord rec = query.record();
        while (query.next())
        {
            value1 = query.value(rec.indexOf("value1")).toByteArray();
            value2 = query.value(rec.indexOf("value2")).toByteArray();
        }
        query.clear();
        return true;
    }
    query.clear();
    return false;
}

void DataManagement::writeDataSQL_settingTable(QSqlDatabase& db, QSqlQuery& query, int id, QString parameter, QByteArray& value)
{
    if (!db.tables().contains(QLatin1String("settingtable")))   {
        query.exec("create table settingtable (id int primary key, parameter nvarchar(300), value nvarchar(300))");
    }
    query.exec(QString("select exists (select parameter, value from settingtable where id = '%1');").arg(id));
    query.next();
    if (query.value(0) != 0) {
        query.prepare(QString("update settingtable set parameter=?, value=? where id ='%1'").arg(id));
        query.addBindValue(parameter);
        query.addBindValue(value);
        query.exec();
        query.clear();
     }
     else {
        query.prepare("insert into settingtable (id, parameter, value) values (?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(parameter);
        query.addBindValue(value);
        query.exec();
        query.clear();
    }
}

QByteArray DataManagement::readDataSQL_settingTable(QSqlDatabase& db, QSqlQuery& query, QString parameter)
{
    QByteArray value;
    if (!db.tables().contains(QLatin1String("settingtable")))  {
        return QString("Ошибка чтения, данных не существует!").toLocal8Bit();
    }
    query.exec(QString("select exists (select parameter, value from settingtable where parameter = '%1');").arg(parameter));
    query.next();
    if (query.value(0) != 0) {
        query.exec(QString("select parameter, value from settingtable where parameter = '%1'").arg(parameter));
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
