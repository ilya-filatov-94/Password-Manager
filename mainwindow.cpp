#include "mainwindow.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    counter_attempts_autoriz=0;                           //счётчик кол-ва попыток авторизации
    var_timer_of_attempts=max_time_delay;                 //инициализация таймера повторной отправки сообщения
    authorization_widget1 = new QWidget(this);            //Виджет окна
    authorization_widget1->setObjectName("check_window");
    window_app = new Client_window();                     //Главное окно приложения
    start_read=true;                                      //Инициализация
    blocked_app=0;                                        //инициализация
    setting = new QSettings("My_Corp", "Spreadsheet");    //раздел реестра из которого будет читаться переменная блокировки
    file1 = new QFile("data1.bin");                       //файл чтения данных
    verification_pas=false;                               //инициализация
    verification_code_from_message=false;                 //инициализация
    no_internet_connection=false;

    //Виджет ввода пароля
    line_enter_pas = new QLineEdit(this);
    line_enter_pas->setEchoMode(QLineEdit::Password);
    line_enter_pas->setStyleSheet("background-color:rgb(255,255,255);");
    connect(line_enter_pas, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста

    //Состояние Caps Lock
    label_state_capslock = new QLabel(this);
    label_state_capslock ->setText(tr("                        "));   //Инициализация по умолчанию
    key_capslock = new QShortcut(this);
    key_capslock->setKey(Qt::Key_CapsLock);
    connect(key_capslock, SIGNAL(activated()), this, SLOT(push_capslock()));      //По нажатию на клавишу capslock выводить надпись
    QFont font("Times", 8, QFont:: Bold);                                         //параметры шрифта
    label_state_capslock->setFont(font);

    //Виджеты ввода кода подтвеждения
    label_email_confrmation = new QLabel(this);                             //Виджет запроса ввода кода подтверждения
    line_enter_code = new QLineEdit(this);                                  //поле ввода кода подтверждения
    line_enter_code->setStyleSheet("background-color:rgb(255,255,255);");
    connect(line_enter_code, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    timer1 = new QTimer(this);                                          //таймер обратного отсчёта времени
    connect(timer1, SIGNAL(timeout()), this, SLOT(slotTimeout()));      //Соединение со слотом отсчёта времени

    //Время обратного отсчёта|кнопка повторной отправки сообщения
    countdown_timer = new QLabel(this);
    countdown_timer->setText(tr("                        "));
    countdown_timer->setMargin(6);
    resending_message = new QPushButton(this);
    resending_message->setText(tr("Отправить код"));
    resending_message->setFixedWidth(195);
    connect(resending_message, SIGNAL(clicked()), this, SLOT(slot_restart_Timer()));
    resending_message->setVisible(false);              //Инициализация видимости

    //Кнопки ОК и Cancel
    ok = new QPushButton(this);
    ok->setText(tr("Ok"));      //Подтверждение ввода
    connect(ok, SIGNAL(clicked()), this, SLOT(check_password()));
    key_enter = new QShortcut(this);
    key_enter->setKey(Qt::Key_Return);
    connect(key_enter, SIGNAL(activated()), this, SLOT(check_password()));

    cancel = new QPushButton(this);
    cancel->setText(tr("Cancel"));      //Закрытие приложения
    connect(cancel, SIGNAL(clicked()), this, SLOT(close_window()));

    groupBox1 = new QGroupBox(this);
    groupBox1->setTitle(tr(" Введите пароль:"));        //группа с рамкой
    //Компоновка виджетов ввода мастер-пароля
    QVBoxLayout* vertical0 = new QVBoxLayout();     //вертикальное размещение 1
    vertical0->setMargin(15);                       //Толщина рамки
    vertical0->setSpacing(5);                       //Расстояние между виджетами
    vertical0->addStretch(1);
    vertical0->addWidget(line_enter_pas);
    vertical0->addWidget(label_state_capslock);
    vertical0->setAlignment(label_state_capslock, Qt::AlignCenter);
    groupBox1->setLayout(vertical0);        //группа с рамкой

    //компоновка виджетов ввода одноразового кода
    groupBox2 = new QGroupBox(this);
    groupBox2->setTitle(tr(" Введите код подтверждения:"));        //группа с рамкой
    QVBoxLayout* vertical01 = new QVBoxLayout;          //вертикальное размещение 2
    vertical01->setMargin(15);                          //Толщина рамки
    vertical01->setSpacing(10);                         //Расстояние между виджетами
    vertical01->addStretch(1);
    vertical01->addWidget(label_email_confrmation);
    vertical01->addWidget(line_enter_code);
    vertical01->addWidget(countdown_timer);
    countdown_timer->setAlignment(Qt::AlignCenter);
    vertical01->addStretch(5);
    vertical01->addWidget(resending_message);
    vertical01->setAlignment(resending_message, Qt::AlignCenter);
    groupBox2->setLayout(vertical01);        //группа с рамкой
    groupBox2->setVisible(false);            //Инициализация

    //Компоновка кнопок
    QHBoxLayout* horizontal0 = new QHBoxLayout();       //горизонтальное размещение 1
    horizontal0->setMargin(5);                          //Толщина рамки
    horizontal0->setSpacing(10);                        //Расстояние между виджетами
    horizontal0->addStretch(1);
    horizontal0->addWidget(ok);
    horizontal0->addWidget(cancel);
    horizontal0->addStretch(1);

    //Размещение компоновок на на виджете окна
    QVBoxLayout* vertical02 = new QVBoxLayout();        //вертикальное размещение 2
    vertical02->setMargin(5);                           //Толщина рамки
    vertical02->setSpacing(5);                          //Расстояние между виджетами
    vertical02->addStretch(1);
    vertical02->addWidget(groupBox1);
    vertical02->addWidget(groupBox2);
    vertical02->addLayout(horizontal0);
    vertical02->addStretch(1);


    network_connection = new Network_connection();        //объект передаваемый в поток не должен иметь родителя
    thread = new QThread(this);
    connect(this, &MainWindow::call_send_message, network_connection, &Network_connection::check_http_connection);
    //connect(this, SIGNAL(destroyed()), thread, SLOT(deleteLater()));
    connect(this, SIGNAL(destroyed()), thread, SLOT(terminate()));
    //connect(this, SIGNAL(destroyed_thread()), thread, SLOT(terminate()));
    thread->start();
    network_connection->moveToThread(thread);

    //Результат проверки интернет соединения
    connect(network_connection, &Network_connection::failure_internet_connection, this, &MainWindow::result_connection);

    setWindowTitle(tr("Password Manager"));                       //Заголовок окна
    authorization_widget1->setLayout(vertical02);
    setCentralWidget(authorization_widget1);
    setMinimumSize(500, 180);
    setMaximumSize(947, 509);
    resize(500, 180);
}

MainWindow::~MainWindow()
{
    //qDebug() << "destroyed";
    delete window_app;
}

//Метод для вывода 3-ёх разных типов диалоговых сообщений
void MainWindow::dialog_message(int arg, QString str1, QString str2)
{
    if (arg==1)
    {
            QMessageBox* f_msg1 = new QMessageBox(QMessageBox::Information,str1,str2);
            if (f_msg1->exec()==QMessageBox::Ok)
            {delete f_msg1;}
    }
    if (arg==2)
    {
            QMessageBox* f_msg2 = new QMessageBox(QMessageBox::Warning,str1,str2);
            if (f_msg2->exec()==QMessageBox::Ok)
            {delete f_msg2;}
    }
    if (arg==3)
    {
            QMessageBox* f_msg3 = new QMessageBox(QMessageBox::Critical,str1,str2);
            if (f_msg3->exec()==QMessageBox::Ok)
            {delete f_msg3;}
    }
}

//Чтение состояния клавиши CapsLock при запуске приложения
void MainWindow::state_capslock()
{
    if ((GetKeyState(VK_CAPITAL) & 0x0001)!=0)
    {
        label_state_capslock->setText(tr("Нажата клавиша Caps Lock"));
    }
    else
    {
        label_state_capslock->setText(tr("                        "));
    }
}

//Слот нажатия клавиши CapsLock
void MainWindow:: push_capslock()
{
    state_capslock();
}

//Проверка блокировки приложения
void MainWindow::check_blocked()
{
    if (start_read==true)
    {
        blocked_app=setting->value("setting_value").toInt();     //Проверка заблокировано ли приложение
        if (blocked_app==255)
        {
            dialog_message(3, tr("Приложение заблокировано"), tr("Исчерпано количество попыток входа"));   //вызов диалогового окна с ошибкой
            this->close();
            exit(1);
        }
        else
        {
            read_main_settings();	//Если приложение не заблокировано, то читаем все настройки
        }
    }
    start_read=false;
}

//Чтение основных настроек приложения: блокировки прил., состояния CapsLock, видимость виджетов пров. кода, старт таймера и т.д.
void MainWindow::read_main_settings()
{
      state_capslock();                                                 //Чтение состояния клавиши CapsLock
      //Чтение параметра привязки email к приложению и самого email
        if (file1->exists())                                            //Если файл существует в данной директории, то открываем на чтение
            {
            file1->open(QIODevice::ReadOnly);
            QDataStream* stream = new QDataStream(file1);
            stream->setVersion(QDataStream::Qt_4_9);
            check_mail=read_settings(stream);                           //чтение параметра привязки почты
            if (check_mail==1)
            {mail_autoriz=read_data(stream);}             //чтение адреса привязанной почты
            output_pas=read_data(stream);                   //чтение мастер-пароля
            file1->close();
            delete stream;
            }
        else
            {
                check_mail=0;
                output_pas = (tr("12345"));    //Если файла нет в текущей директории, записываем стандатный пароль
            }

        //Инициализация видимости виджетов проверки одноразового кода и первый запуск таймера
        if (check_mail==1)      //Если почта привязана
        {
            QString text_request_mail;
            text_request_mail.clear();
            text_request_mail=tr("Вам был отправлен код подтверждения на электронную почту:");
            text_request_mail+=tr("\r\n");
            text_request_mail+=mail_autoriz;
            label_email_confrmation->setText(text_request_mail);
            groupBox2->setVisible(true);
            limiting_attempts_resend();     //ограничение количества попыток по одноразовому коду
            this->resize(500, 330);
        }
        else
        {
            groupBox2->setVisible(false);
            this->resize(500, 180);
        }
}

//Чтение настройки check_mail
int MainWindow::read_settings(QDataStream* arg_stream)
{
    int int_data=0;
    double double_data=0;
    *arg_stream >> (double_data);                              //чтение настройки, привязана ли почта
    return int_data=static_cast<int>(double_data);             //параметр привязки почты
}

//Чтение данных из файла
QString MainWindow::read_data(QDataStream* arg_stream)
{
    QString str;
    double temp_arr[300];                                               //промежуточный массив double
    for (int i=0; i<300; i++)                                           //Первоначальная инициализация всего массива
    {temp_arr[i]=0;}
    int size_str=My_Encrytion::read_data(arg_stream, temp_arr);           //чтение
    My_Encrytion::reverse_shuffle_data(temp_arr, size_str, 8);            //вызов функции обратного перемешивания
    str=My_Encrytion::decrypt(temp_arr, str, size_str);                   //расшифровка инвертированной математической функцией
    return str;
}

//Слот перезапуска таймера
void MainWindow::slot_restart_Timer()
{
    Sleep(1500);    //ожидание результата проверки интернет-соединения
    if (no_internet_connection==false)
    {
        counter_attempts_autoriz++;
        dialog_message(2, tr("Предупреждение"), (tr("Осталось ")+QString::number(max_quantity_attempts-counter_attempts_autoriz)+tr(" попыток авторизации")));
    }
    no_internet_connection=false;
    //отправка сообщения и перезапуск таймера
    limiting_attempts_resend();
}

void MainWindow::limiting_attempts_resend()
{
    if (counter_attempts_autoriz>=max_quantity_attempts)
    {
        dialog_message(3, tr("Ошибка"), tr("Исчерпано кол-во попыток авторизации. <br> Приложение заблокировано"));   //вызов диалогового окна с ошибкой
        blocked_app=255;                               //блокировка повторного открытия приложения
        setting->setValue("setting_value", blocked_app);
        this->close();
        exit(1);
    }
    else
    {
        timer1->start(1000);                                                            //старт отсчёта, мс
        resending_message->setVisible(false);                                           //видимость кнопки
        countdown_timer->setText(tr("Отправить код повторно можно через: 01:30"));
        countdown_timer->setVisible(true);                                              //видимость текущего времени отсчёта
        generating_code_message();
        emit call_send_message("verification:"+mail_autoriz+" code_mes:"+code_mes);
    }
}

void MainWindow::generating_code_message()
{
    code_mes.clear();
    for (int i=0; i<6; i++)
    {
        std::uniform_int_distribution<int>distribution(0,9);
        gen_elem[i] = distribution(*QRandomGenerator::global());
        code_mes+=QString::number(gen_elem[i]);
    }
}


void MainWindow:: slotTimeout()
{
    if (var_timer_of_attempts>0)
    {
        current_time.clear();
        var_timer_of_attempts--;
        minutes=var_timer_of_attempts/60;
        secondes=var_timer_of_attempts-minutes*60;
        current_time=tr("Отправить код повторно можно через: ");
        if (minutes>9)
            {current_time+=QString::number(minutes);}
        else
            {current_time+="0"+QString::number(minutes);}
        current_time+=tr(":");
        if (secondes>9)
            {current_time+=QString::number(secondes);}
        else
            {current_time+="0"+QString::number(secondes);}
        countdown_timer->setText(current_time);
        countdown_timer->setVisible(true);              //видимость текущего времени отсчёта
    }
    else
    {
        timer1->stop();
        var_timer_of_attempts=max_time_delay;       //Инициализация перед следующим запуском таймера
        countdown_timer->setVisible(false);         //видимость текущего времени отсчёта
        resending_message->setVisible(true);        //видимость кнопки
    }

}

void MainWindow::check_password()
{
    //при стирании пароля задать стандартный
    if (output_pas.isEmpty())
    {output_pas = (tr("12345"));}
    input_pas=line_enter_pas->text();

    //Проверка введённого пароля
    if (input_pas!=output_pas)
    {
        verification_pas=false;
    }
    if (input_pas==output_pas)
    {
        verification_pas=true;
    }
    //Проверка кода из сообщения по электронной почте
    if (check_mail!=1)
    {
        verification_code_from_message=true;    //почта не привязана, проверка кода не требуется
    }
    if (check_mail==1)                          //Если почта привязана, проверяем введённый код
    {
        enter_code_mes=line_enter_code->text();
        if (enter_code_mes!=code_mes)
        {
            verification_code_from_message=false;
        }
        if (enter_code_mes==code_mes)
        {
            verification_code_from_message=true;
        }
    }
    //Результат аутентификации
    if (verification_pas==true && verification_code_from_message==true)
    {
        window_app->show();
        window_app->readSettings();
        window_app->set_backgound_image();
        this->close();
        dialog_message(1, tr("Информация"), tr("Вход в приложение выполнен!"));
    }
    else
    {
        counter_attempts_autoriz++;   //Счётчик кол-ва попыток
        //Если кол-во попыток исчерпано
        if (counter_attempts_autoriz>=max_quantity_attempts)
        {
            dialog_message(3, tr("Ошибка"), tr("Исчерпано кол-во попыток авторизации <br> приложение заблокировано"));   //вызов диалогового окна с ошибкой
            blocked_app=255;                               //блокировка повторного открытия приложения
            setting->setValue("setting_value", blocked_app);
            this->close();
            exit(1);
        }
        else
        {
            dialog_message(2, tr("Предупреждение"), (tr("Данные введены неверно. <br> Осталось ")+QString::number(max_quantity_attempts-counter_attempts_autoriz)+tr(" попыток авторизации")));
        }
    }
}


void MainWindow::slot_check_size_line()
{
    QString str=tr("");
    if ((line_enter_pas->hasFocus())==true)
    {
        str=line_enter_pas->text();
        str.truncate(300);
        line_enter_pas->setText(str);
    }
    if ((line_enter_code->hasFocus())==true)
    {
        str=line_enter_code->text();
        str.truncate(300);
        line_enter_code->setText(str);
    }
}


//Результат проверки наличия интернет соединения
void MainWindow::result_connection(QString result)
{
    timer1->stop();
    var_timer_of_attempts=max_time_delay;       //Инициализация перед следующим запуском таймера
    countdown_timer->setVisible(false);         //видимость текущего времени отсчёта
    resending_message->setVisible(true);        //видимость кнопки
    counter_attempts_autoriz++;
    no_internet_connection=true;

    if (counter_attempts_autoriz>=max_quantity_attempts)
    {
        dialog_message(3, tr("Ошибка"), tr("Исчерпано кол-во попыток авторизации <br> приложение заблокировано"));   //вызов диалогового окна с ошибкой
        blocked_app=255;                               //блокировка повторного открытия приложения
        setting->setValue("setting_value", blocked_app);
        this->close();
        exit(1);
    }
    else
    {
        dialog_message(3, tr("Ошибка"), (result+tr("Осталось ")+QString::number(max_quantity_attempts-counter_attempts_autoriz)+tr(" попыток авторизации")));
    }
}


void MainWindow::close_window()
{
    this->close();
}

