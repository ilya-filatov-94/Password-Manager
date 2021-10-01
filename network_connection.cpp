#include "network_connection.h"
//#include <QDebug>

Network_connection::Network_connection(QObject *parent) : QObject(parent)
{
    connection_http=false;
    connection_dns=false;
    internet_connection=false;
    manager = new QNetworkAccessManager(this);
    /*подключение сигнала о завершении получения данных к обработчику полученного ответа*/
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
    date_time = new QDateTime();                            //Текущие дата/время системы для сообщения
    manager2 = new QNetworkAccessManager(this);
    connect(manager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(response_dataBase(QNetworkReply*)));
}

void Network_connection::check_http_connection(QString received_command)
{
    command=received_command;   //строковая команда на выполнение действия из другого потока

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
        emit failure_internet_connection(tr("Проверьте соединение с интернетом.\nОтправка сообщения с кодом проверки невозможна\n"));
        connection_http=false;
    }
    else
    {
        //Если запрос выполнен успешно, то читаем строку из файла и проверяем её на соответсвие с эталонной
        QString str1=reply->readAll();
        if (str1=="Microsoft NCSI")
        {
            connection_http=true;
            //2-ая проверка: работоспособность DNS. Для этого запрашиваем IP-адреса хоста dns.msftncsi.com
            request_dns();
        }
        else
        {
            emit failure_internet_connection(tr("Проверьте соединение с интернетом.\nОтправка сообщения с кодом проверки невозможна\n"));
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
       //qDebug() << "запрос к dns завершился ошибкой" << search_host.errorString();
       emit failure_internet_connection(tr("Проверьте соединение с интернетом.\nВозможно требуется авторизация в браузере\n"));
       connection_dns=false;
   }
   QList <QString> list;
   foreach (const QHostAddress &address, search_host.addresses())
   {
       list << address.toString();
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
       //qDebug() << "хост не определён" << list;
       emit failure_internet_connection(tr("Проверьте соединение с интернетом.\nВозможно требуется авторизация в браузере\n"));
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

//выбор действия: отправка сообщения с кодом, отправка тестового письма на почту, запрос пароля в базе данных
void Network_connection::choose_action()
{
    bool not_found=true;        //команда не найдена или не распознана
    int index_command=0;        //индекс команды
    int index_position=0;       //позиция символа в строке команды

    for (int i=0; i<3; i++)
    {
        if (command.indexOf(array_comand[i], Qt::CaseInsensitive)!=-1)
        {
            not_found=false;
            index_command=i;
            break;
        }
    }

    if (not_found==false && !command.isEmpty())
    {
        switch(index_command)
        {
            case 0:     //mail:@mail.ru - Отправить письмо на почту для проверки адреса и привязки почты
            mail_destionation=command.remove(0, array_comand[index_command].size());
            generating_test_message();
            send_message();
            break;

            case 1:     //verification:@mail.ru code_mes:12345 - Отправить письмо на почту с кодом подтверждения
            command.remove(0, array_comand[index_command].size());
            index_position=command.indexOf(array_comand[3], Qt::CaseInsensitive);
            if (index_position!=-1)
            {
                mail_destionation=command;
                mail_destionation.remove(index_position-1,(command.size()));

                code_from_message=command;
                code_from_message.remove(0, (index_position+array_comand[3].size()));
                Sleep(3000);    //ожидание результата проверки интернет-соединения
                generating_message_for_auth();
                send_message();
            }
            break;

            case 2:     //request_dataBase:my_password - отправить запрос в базу данных паролей
            password_for_check=command.remove(0, array_comand[index_command].size());
            execute_request_to_db();
            break;
        }
    }
    /*else if (not_found==true && !command.isEmpty())
    {
        QMessageBox* msg2 = new QMessageBox(QMessageBox::Critical,
                                                     tr("Ошибка!"),
                                                     tr("Команда не распознана!"));
                           if (msg2->exec()==QMessageBox::Ok)
                           {delete msg2;}
    }*/
}

void Network_connection::execute_request_to_db()
{
    preffix_pas.clear();
    suffix_pas.clear();
    QString string_url="https://api.pwnedpasswords.com/range/";     //запрос к API
    QString string_pas=password_for_check;
    QString hash = (QCryptographicHash::hash(string_pas.toUtf8(), QCryptographicHash::Sha1).toHex());

    //Берём первые 5 символов хэша - prefix
    for (int i=0; i<5; i++)
    {
        preffix_pas[i]=hash[i];
    }
    //Получение хэша без пяти первых символов - suffix
    for (int i=5; i<(hash.size()); i++)
    {
        suffix_pas[i-5]=hash[i];
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


void Network_connection::send_message()
{
    //Адрес электронной почты получателя - mail_destionation
    QString name_sender="";                               //Имя получателя

    //Адрес SMTP_сервера и его порт, тип шифрования соединения ssl
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    //Аутентификация в smtp сервере от gmail
    smtp.setUser("@gmail.com");         //почтовый адрес smtp-сервера
    smtp.setPassword("password");       //пароль

    //Создание сообщения
    MimeMessage message;
    message.setSender(new EmailAddress("@gmail.com", "corp\n"));    //адрес отправителя и имя отправителя
    //Ввод адреса электронной почты получателя и имени получателя
    message.addRecipient(new EmailAddress(mail_destionation, name_sender));
    //Ввод темы сообщения
    message.setSubject(theme_message);

    //Заполнение текста сообщения
    MimeText text;
    text.setText(text_message);

    // Добавление текста сообщения в почту
    message.addPart(&text);

    // Соединение с хостом, логин в smtp сервере и отправка сообщения
    smtp.connectToHost();
    smtp.login();

    //Проверка отправки сообщения
    if (!(smtp.sendMail(message)))
    {
        //qDebug() << "Сообщение не отправлено!";
        result_send_message(tr("Ошибка отправления письма!"));
    }
    else
    {
        //qDebug() << "Сообщение успешно отправлено!";
        result_send_message(tr("Письмо успешно отправлено!"));
    }
    smtp.quit();
}


void Network_connection::generating_message_for_auth()
{
    theme_message="authentication";                 //Тема сообщения
    text_message.clear();
    text_message=tr("Выполнена попытка входа с ОС ");
    text_message+=QSysInfo::productType()+" "+QSysInfo::productVersion();       //Название и версия ОС в которой запущено приложение
    text_message+=tr("\r\nДата и время попытки входа: ");
    text_message+=(date_time->currentDateTime()).toString("dd-MM-yyyy hh:mm:ss");
    text_message+=tr("\r\nОдноразовый код для входа в приложение: ");
    text_message+=code_from_message;
    text_message+=tr("\r\nНикому не сообщайте этот код\r\n");
    text_message+=tr("\r\n");
}

void Network_connection::generating_test_message()
{
    theme_message="confirm email";                 //Тема сообщения
    text_message.clear();
    text_message=tr("Данный адрес электронной почты был успешно привязан к приложению Password Manager\r\n");
    text_message+=tr("и будет использоваться для дополнительной защиты при авторизации в приложении.\r\n");
    text_message+=tr("Данное письмо сгенерировано автоматически, пожалуйста не отвечайте на него");

}


Network_connection::~Network_connection()
{
    //qDebug() << "thread was destroyed";
}
