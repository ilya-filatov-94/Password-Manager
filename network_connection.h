#ifndef NETWORK_CONNECTION_H
#define NETWORK_CONNECTION_H

#include "smtp/SmtpMime"
#include "windows.h"

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QHostInfo>
#include <QCryptographicHash>
#include <QList>
#include <QSysInfo>
#include <QDateTime>


class Network_connection : public QObject
{
    Q_OBJECT
public:
    explicit Network_connection(QObject *parent = nullptr);
    ~Network_connection();
    void check_http_connection(QString, QString, QString);
    void getDataSmtp(QString, QString);

private:

    QList<QString> ip_list={"131.107.255.255"};
    bool connection_http;
    bool connection_dns;
    bool internet_connection;
    QString command;            //команда на выполнение действия отправленная из другого потока
    QString cmd_parameter1;
    QString cmd_parameter2;
    QString theme_message;
    QString text_message;
    QString preffix_pas;       //sha1 хэш префикс пароля
    QString suffix_pas;        //sha1 хэш суффикс пароля
    QDateTime date_time;       //текущие дата-время системы

    QNetworkAccessManager* manager;
    QNetworkAccessManager* manager2;
    void getData();                          //http get запрос
    void request_dns();                      //запрос в dns
    void generating_message_for_auth(QString&);      //генерация текста сообщения с одноразовым паролем для входа в приложение
    void generating_test_message();                  //генерация тестового письма для подтверждения email
    void choose_action();                               //выбор действия в зависимости от переданной команды
    void network_action();                              //выполнение команд работы с сетью при успешном интернет соединении
    void getDataConnect();
    void send_message(QString&, QString&, QString&, QString&);
    void execute_request_to_db(QString&);               //выполнить http get-запрос в базу данных паролей

signals:

    void failure_internet_connection(QString);  //Результат проверки соединения с интернетом
    void response_mes_db(QString);              //ответ от базы данных паролей
    void result_send_message(QString);          //Результат отправки письма
    void signal_smtp_data();

private slots:

    void onResult(QNetworkReply* reply);
    void lookedUp(const QHostInfo &search_host);
    void response_dataBase(QNetworkReply *reply);

};

#endif // NETWORK_CONNECTION_H
