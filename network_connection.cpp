#include "network_connection.h"


Network_connection::Network_connection(QObject *parent) : QObject(parent)
{
    connection_http=false;
    connection_dns=false;
    internet_connection=false;
    manager = new QNetworkAccessManager(this);
    /*подключение сигнала о завершении получения данных к обработчику полученного ответа*/
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
    manager2 = new QNetworkAccessManager(this);
    connect(manager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(response_dataBase(QNetworkReply*)));
}

Network_connection::~Network_connection()
{
    delete manager;
    delete manager2;
}

void Network_connection::check_http_connection(QString received_command, QString parameter1, QString parameter2)
{
    command=received_command;   //строковая команда на выполнение действия из другого потока
    cmd_parameter1 = parameter1;
    cmd_parameter2 = parameter2;

    //1-ая проверка: отправляем http-запрос get "http://www.msftncsi.com/ncsi.txt"
    //в случае доступности интернет-соединения должен быть загружен файл со строкой "Microsoft NCSI"
    getData();
}

//Метод HTTP запроса
void Network_connection::getData()
{
    QUrl url("http://www.msftncsi.com/ncsi.txt");
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
}

//Метод обработки ответа HTTP Запроса
void Network_connection::onResult(QNetworkReply* reply)
{
    if (reply->error())
    {
        emit failure_internet_connection(tr("Проверьте соединение с интернетом"));
        connection_http=false;
    }
    else
    {
        //Если запрос выполнен успешно, то читаем строку из файла и проверяем её на соответсвие с эталонной: "Microsoft NCSI"
        QString str1=reply->readAll();
        if (str1=="Microsoft NCSI")
        {
            connection_http=true;
            //2-ая проверка: работоспособность DNS. Для этого запрашиваем IP-адреса хоста dns.msftncsi.com
            request_dns();
        }
        else
        {
            emit failure_internet_connection(tr("Проверьте соединение с интернетом"));
            connection_http=false;
        }
    }
}

void Network_connection::request_dns()
{
    QHostInfo::lookupHost("dns.msftncsi.com",this, SLOT(lookedUp(QHostInfo)));
}

//Метод обработки ответа dns
void Network_connection::lookedUp(const QHostInfo &search_host)
{
   if (search_host.error() != QHostInfo::NoError)
   {
       emit failure_internet_connection(tr("Проверьте соединение с интернетом."));
       connection_dns=false;
   }
   foreach (const QHostAddress &address, search_host.addresses())
   {
       if (ip_list.contains(address.toString()))
       {
           connection_dns=true;
           break;
       }
   }
   if (connection_dns==true)
   {
        network_action();   //2-ая проверка интернет соединения прошла успешно
   }
   else
   {
       emit failure_internet_connection(tr("Проверьте соединение с интернетом."));
   }
}

void Network_connection::network_action()
{
    if (connection_http==true && connection_dns==true)
        internet_connection=true;
    if (internet_connection==true)
    {
        choose_action();
    }
}

//выбор действия: отправка сообщения с кодом на почту/отправка тестового письма на почту/запрос пароля в базе данных
void Network_connection::choose_action()
{
    if (command == "test_message")
    {
        generating_test_message();
        getDataConnect();
    }
    if (command == "verification")
    {
        Sleep(3000);
        generating_message_for_auth(cmd_parameter2);
        getDataConnect();
    }
    if (command == "request_db")
    {
        execute_request_to_db(cmd_parameter1);
    }
}

void Network_connection::execute_request_to_db(QString& checkPassword)
{
    preffix_pas.clear();
    suffix_pas.clear();
    QString string_url="https://api.pwnedpasswords.com/range/";     //запрос к API
    QString hash = (QCryptographicHash::hash(checkPassword.toUtf8(), QCryptographicHash::Sha1).toHex());

    for (int i=0; i<(hash.size()); i++)
    {
        if (i<5)    //первые 5 символов хэша - prefix
        {
            preffix_pas[i]=hash[i];
        }
        if (i>=5)   //хэш без 5 первых символов - suffix
        {
            suffix_pas[i-5]=hash[i];
        }
    }
    string_url+=preffix_pas;
    QNetworkRequest request2;
    request2.setUrl(string_url);
    //Выполняем GET запрос
    manager2->get(request2);
}

void Network_connection::response_dataBase(QNetworkReply *reply)
{
    bool pas_found=false;	//Переменная показывающая, найден ли такой пароль в базе данных
    int position;           //позиция символа в списке
    QString mes;            //Ответ от БД
    if (reply->error())
    {
        emit failure_internet_connection(tr("Проверьте соединение с интернетом.\n"));
    }
    else
    {
        //Записываем ответ в строку
        QString response;
        response=reply->readAll();
        //разделителем между элементами списка будет служить символ "\r\n"
        QStringList list1 = response.split("\r\n");
        for (int i=0; i<list1.size(); i++)
        {
            pas_found=list1[i].contains(suffix_pas, Qt :: CaseInsensitive);
            if (pas_found==true)
            {
                position=list1[i].indexOf(":");
                QString for_count=list1[i].right(list1[i].size()-position-1);
                mes=(tr("Результат запроса: пароль встречается в базе "));
                mes+=for_count;
                mes+=(tr(" раз"));
                emit response_mes_db(mes);
                break;
            }
        }
        if (pas_found==false)
        {
            emit response_mes_db(tr("Результат запроса: пароль не найден в базе данных"));
        }
    }
}

void Network_connection::getDataConnect()
{
    emit signal_smtp_data();
}

void Network_connection::getDataSmtp(QString login, QString password)
{
    send_message(login, password, cmd_parameter1, text_message);
}

void Network_connection::send_message(QString& login, QString& passw, QString& mail_destionation, QString& text_message)
{
    //Имя адресата
    QString name_sender="";

    //Адрес SMTP_сервера и его порт, тип шифрования соединения TLS
    SmtpClient smtp("smtp.gmail.com", 587, SmtpClient::TlsConnection);

    //Аутентификация в smtp сервере от gmail
    smtp.setUser(login);
    smtp.setPassword(passw);

    //Создание сообщения
    MimeMessage message;
    message.setSender(new EmailAddress(login, "Filatov IV corp\n"));
    //Ввод адреса электронной почты адресата mail_destionation и имени адресата
    message.addRecipient(new EmailAddress(mail_destionation, name_sender));
    //Ввод темы сообщения
    message.setSubject(theme_message);

    //Заполнение текста сообщения
    MimeText text;
    text.setText(text_message);

    // Добавление текста сообщения в почту
    message.addPart(&text);

    // Соединение с хостом, логин в smtp сервере
    smtp.connectToHost();
    smtp.login();

    //Отправка и проверка отправки сообщения
    if (!(smtp.sendMail(message)))
    {
        emit result_send_message(tr("Ошибка отправления письма!"));
    }
    else
    {
        emit result_send_message(tr("Письмо успешно отправлено!"));
    }
    smtp.quit();
}

void Network_connection::generating_message_for_auth(QString& code_from_message)
{
    text_message.clear();
    theme_message="authentication";
    text_message=tr("Выполнена попытка входа с ОС ");
    text_message+=QSysInfo::productType()+" "+QSysInfo::productVersion();       //Название и версия ОС в которой запущено приложение
    text_message+=tr("\r\nДата и время попытки входа: ");
    text_message+=(date_time.currentDateTime()).toString("dd-MM-yyyy hh:mm:ss");
    text_message+=tr("\r\nОдноразовый код для входа в приложение: ");
    text_message+=code_from_message;
    text_message+=tr("\r\nНикому не сообщайте этот код\r\n");
    text_message+=tr("\r\n");
}

void Network_connection::generating_test_message()
{
    text_message.clear();
    theme_message="confirm email";
    text_message=tr("Данный адрес электронной почты был успешно привязан к приложению Password Manager\r\n");
    text_message+=tr("и будет использоваться для дополнительной защиты при авторизации в приложении.\r\n");
    text_message+=tr("Данное письмо сгенерировано автоматически, пожалуйста не отвечайте на него");
}

