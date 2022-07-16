#include "window_setting.h"


Window_setting::Window_setting(QWidget *parent) : QMainWindow(parent)
{
    Setting_window = new QWidget(this);
    Setting_window->setObjectName("Setting_window");

    //Виджеты показа текущего пароля
    label_saved_passw = new QLabel(this);
    label_saved_passw->setText(tr("Текущий мастер-пароль:"));
    enter_old_pas = new QLineEdit(this);
    enter_old_pas->setFixedWidth(550);

    //Копка видимости текущего пароля
    visibleOldPas = new QPushButton(this);
    visibleOldPas->setIcon(QIcon(":/img/closed_lock.png"));
    visibleOldPas->setIconSize(QSize(26,26));
    visibleOldPas->setFixedHeight(26);
    visibleOldPas->setFixedWidth(26);
    visibleOldPas->setToolTip(tr("Изменение видимости пароля"));
    connect(visibleOldPas, &QPushButton::clicked, this, &Window_setting::changeVisibleOld_pas);

    //горизонт компоновка текущего пароля
    horizontal_old_pas = new QHBoxLayout();
    horizontal_old_pas->setSpacing(5);
    horizontal_old_pas->setMargin(15);
    horizontal_old_pas->addWidget(enter_old_pas);
    horizontal_old_pas->setAlignment(enter_old_pas, Qt::AlignLeft);
    horizontal_old_pas->addWidget(visibleOldPas);
    horizontal_old_pas->setAlignment(visibleOldPas, Qt::AlignLeft);

    //Виджеты ввода нового пароля
    enter_new_pas = new QLineEdit(this);
    connect(enter_new_pas, SIGNAL(textChanged(const QString &)), this, SLOT(difficult_of_passw()));
    enter_new_pas->installEventFilter(this);

    //Копка видимости нового пароля
    visibleNewPas = new QPushButton(this);
    visibleNewPas->setIcon(QIcon(":/img/closed_lock.png"));
    visibleNewPas->setIconSize(QSize(26,26));
    visibleNewPas->setFixedHeight(26);
    visibleNewPas->setFixedWidth(26);
    visibleNewPas->setToolTip(tr("Изменение видимости пароля"));
    connect(visibleNewPas, &QPushButton::clicked, this, &Window_setting::changeVisibleNew_pas);

    //horizontal_new_pas
    //вертик компоновка текущего пароля
    horizontal_new_pas = new QHBoxLayout();
    horizontal_new_pas->setSpacing(5);
    horizontal_new_pas->addWidget(enter_new_pas);
    horizontal_new_pas->addWidget(visibleNewPas);
    horizontal_old_pas->setAlignment(visibleNewPas, Qt::AlignTop);

    //Кнопки управления окном
    save_settings = new QPushButton(this);
    save_settings->setText(tr("Сохранить настройки"));
    save_settings->setFixedWidth(200);
    connect(save_settings, SIGNAL(clicked()), this, SLOT(save()));
    cancel_settings = new QPushButton(this);
    cancel_settings->setText(tr("Отмена"));
    cancel_settings->setFixedWidth(200);
    connect(cancel_settings, SIGNAL(clicked()), this, SLOT(cancel()));

    //Сложность пароля текущая
    label_difficult_passw = new QLabel(this);
    label_difficult_passw->setFixedWidth(271);
    label_difficult_passw->setFixedHeight(24);
    label_difficult_passw->setScaledContents(false);

    //Виджеты генерации пароля
    label_generate_pasw = new QLabel(this);
    label_generate_pasw->setText(tr("Для автоматического создания мастер-пароля нажмите кнопку"));
    generate_pas = new QPushButton(this);
    generate_pas->setText(tr("Генерировать пароль"));
    connect(generate_pas, SIGNAL(clicked()), this, SLOT(gen_master_passw()));

    //-----Компоновка виджетов запроса пароля в БД------------
    group_check_DB = new QGroupBox();
    QFont font1("Times", 9, QFont::Normal);
    group_check_DB->setTitle(tr("Проверить новый пароль в базе данных:"));
    label_check_pas_DB = new QLabel(this);
    label_check_pas_DB->setTextInteractionFlags(Qt ::LinksAccessibleByMouse);
    label_check_pas_DB->setOpenExternalLinks(true);
    label_check_pas_DB->setText(tr("Приложение использует базу данных авторитетного сервиса "
                                    "<a href='https://haveibeenpwned.com/'>Have I Been Pwned</a>,"
                                    "<br>который накапливает информацию об утекших в сеть паролях"));
    button_request_DB = new QPushButton(this);
    button_request_DB->setText(tr("Проверить пароль"));
    button_request_DB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(button_request_DB, SIGNAL(clicked()), this, SLOT(slot_request_pas()));
    label_result_DB = new QLabel(this);
    label_result_DB->setText(tr("Результат запроса пароля в БД:"));
    horizontal_group = new QHBoxLayout();
    horizontal_group->setSpacing(10);
    horizontal_group->addWidget(button_request_DB);
    horizontal_group->addWidget(label_result_DB);
    vertical_qroup = new QVBoxLayout();
    vertical_qroup->setMargin(10);
    vertical_qroup->setSpacing(5);
    vertical_qroup->addWidget(label_check_pas_DB);
    vertical_qroup->addLayout(horizontal_group);
    group_check_DB->setLayout(vertical_qroup);
    group_check_DB->setFont(font1);
    group_check_DB->setFixedHeight(120);

    //Компоновка виджетов подключения аутентификации по email
    check_box_for_email = new QCheckBox(this);
    change_status_mail=false;
    connect(check_box_for_email,SIGNAL(toggled(bool)), this, SLOT(visible_checkbox()));
    label_checkbox = new QLabel(this);
    label_checkbox->setText(tr("Аутентификация по электронной почте"));
    label_email = new QLabel(this);
    enter_address_mail = new QLineEdit(this);
    connect(enter_address_mail, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста

    //Окно паузы чтобы программа успела проверить корректность email
    wait_widget = new QLabel();
    wait_widget->setText(tr("Пожалуйста, подождите, пока программа завершит <br>проверку введённого вами адреса email <br><br><br>По указанному адресу вам было отправлено информирующее <br>письмо, пожалуйста не отвечайте на него"));
    wait_widget->setAlignment(Qt::AlignCenter);
    wait_widget->setStyleSheet("background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffffff, stop: 0.5 #e0e5e8, stop: 0.51 #d2dadc, stop: 1 #c0cecf);"
                               "border: 1px solid darkgray;");
    wait_widget->setWindowFlags(Qt::WindowStaysOnTopHint);  //поверх окон
    wait_widget->setWindowModality(Qt::ApplicationModal);   //модальность
    wait_widget->setWindowFlags(Qt::FramelessWindowHint);   //отключаем обрамление окна виджета
    wait_widget->resize(400, 170);

    //Компоновка виджетов ввода нового пароля
    groupBox_new_pas = new QGroupBox(this);
    groupBox_new_pas->setTitle(tr("Введите новый мастер-пароль:"));        //группа с рамкой
    vertical_new_pas = new QVBoxLayout();    //вертикальное размещение 1
    vertical_new_pas->setMargin(10);                    //Толщина рамки
    vertical_new_pas->setSpacing(5);                    //Расстояние между виджетами
    vertical_new_pas->addLayout(horizontal_new_pas);
    vertical_new_pas->addWidget(label_difficult_passw);
    vertical_new_pas->setAlignment(label_difficult_passw, Qt::AlignLeft);
    groupBox_new_pas->setLayout(vertical_new_pas);        //группа с рамкой
    groupBox_new_pas->setFont(font1);

    //Компоновка виджетов генерации пароля
    horizontal_widg_gen = new QHBoxLayout;
    horizontal_widg_gen->setMargin(5);                       //Толщина рамки
    horizontal_widg_gen->setSpacing(10);                     //Расстояние между виджетами
    horizontal_widg_gen->addWidget(label_generate_pasw);
    horizontal_widg_gen->addWidget(generate_pas);

    //Компоновка виджетов настройки аутентификации по email
    horizontal_checkbox = new QHBoxLayout;
    horizontal_checkbox->addWidget(label_checkbox);
    horizontal_checkbox->setSpacing(5);
    horizontal_checkbox->addWidget(check_box_for_email, Qt::AlignLeft);
    horizontal_widget_mail = new QHBoxLayout;
    horizontal_widget_mail->addWidget(label_email);
    horizontal_widget_mail->setSpacing(5);
    horizontal_widget_mail->addWidget(enter_address_mail, Qt::AlignLeft);

    //Компоновка кнопок внизу окна
    horizontal_button_sett = new QHBoxLayout;
    horizontal_button_sett->setMargin(5);
    horizontal_button_sett->setSpacing(10);
    horizontal_button_sett->addWidget(save_settings, Qt::AlignHCenter);
    horizontal_button_sett->addWidget(cancel_settings, Qt::AlignHCenter);

    //Компоновка всех элементов
    vertical1 = new QVBoxLayout;
    vertical1->setMargin(20);                       //Толщина рамки
    vertical1->setSpacing(10);                     //Расстояние между виджетами
    vertical1->addWidget(label_saved_passw);
    vertical1->addLayout(horizontal_old_pas);
    vertical1->addWidget(groupBox_new_pas);
    vertical1->addLayout(horizontal_widg_gen);
    vertical1->addWidget(group_check_DB);
    vertical1->addLayout(horizontal_checkbox);
    vertical1->addLayout(horizontal_widget_mail);
    vertical1->addLayout(horizontal_button_sett);
    Setting_window->setLayout(vertical1);
    setCentralWidget(Setting_window);

    network_connection1 = new Network_connection();        //объект передаваемый в поток не должен иметь родителя
    thread1 = new QThread(this);
    thread1->start();
    network_connection1->moveToThread(thread1);
    connect(thread1, &QThread::finished, thread1, &QThread::quit);
    connect(thread1, &QThread::finished, thread1, &QThread::deleteLater);

    //Запуск проверки интернет-соединения
    connect(this, &Window_setting::call_network_action, network_connection1, &Network_connection::check_http_connection, Qt::QueuedConnection);

    //Результат проверки интернет соединения
    connect(network_connection1, &Network_connection::failure_internet_connection, this, &Window_setting::result_connection, Qt::QueuedConnection);

    //Результат проверки базы данных паролей
    connect(network_connection1, &Network_connection::response_mes_db, this, &Window_setting::response_from_db, Qt::QueuedConnection);

    //Тестовое письмо на почту
    connect(network_connection1, &Network_connection::result_send_message, this, &Window_setting::result_confirmation_email, Qt::QueuedConnection);

    //Запрос данных для smtp-сервера
    connect(network_connection1, &Network_connection::signal_smtp_data, this, &Window_setting::getDataForSmtp, Qt::QueuedConnection);
    connect(this, &Window_setting::send_data_smtp, network_connection1, &Network_connection::getDataSmtp, Qt::QueuedConnection);

    //Прогресс бар
    progress_widget.setAlignment(Qt::AlignCenter);
    progress_widget.setWindowFlags(Qt::WindowStaysOnTopHint);          //поверх окон
    progress_widget.setWindowModality(Qt::ApplicationModal);           //модальность
    progress_widget.setWindowFlags(Qt::FramelessWindowHint);           //отключаем обрамление окна виджета
    progress_widget.setAttribute(Qt::WA_TranslucentBackground, true);  //Прозрачность
    progress_widget.resize(150, 150);
    movie = new QMovie(&progress_widget);
    movie->setFileName(":/img/loading.gif");
    movie->setScaledSize(progress_widget.size());
    progress_widget.setMovie(movie);
    //--------------------------------------------------------

    setWindowTitle(tr("Настройки приложения"));
    setMinimumSize(650, 525);
    setMaximumSize(650, 600);
    resize(650, 525);
}

Window_setting:: ~Window_setting()
{    
    delete  wait_widget;

    thread1->quit();
    thread1->wait();
    thread1->deleteLater();
    network_connection1->deleteLater();
    delete network_connection1;
    delete thread1;

    delete horizontal_old_pas;
    delete horizontal_new_pas;
    delete horizontal_group;
    delete vertical_qroup;
    delete group_check_DB;
    delete vertical_new_pas;
    delete groupBox_new_pas;
    delete horizontal_widg_gen;
    delete horizontal_checkbox;
    delete horizontal_button_sett;
    delete vertical1;
    delete Setting_window;
}

void Window_setting::read_settings()
{
    enter_new_pas->setText(tr(""));
    QString old_global_pas;
    db = QSqlDatabase::database();
    int check_mail = 0;
    if(db.open())
    {
        QSqlQuery query;
        QByteArray key, iv, text;
        key = readDataSQL_settingTable(db, query, "main_key");
        iv = readDataSQL_settingTable(db, query, "main_vector");
        text = readDataSQL_settingTable(db, query, "master_pas");
        old_global_pas = decryptAES(text, key, iv);
        enter_old_pas->setText(old_global_pas);
        enter_old_pas->setEchoMode(QLineEdit::Password);
        enter_old_pas->setReadOnly(true);
        enter_new_pas->setEchoMode(QLineEdit::Password);

        address_mail_string = QString::fromLocal8Bit(readDataSQL_settingTable(db, query, "availability_email"));
        check_mail = address_mail_string.toInt();
        if (check_mail == 1)
        {
            key = readDataSQL_settingTable(db, query, "key_mail");
            iv = readDataSQL_settingTable(db, query, "vector_mail");
            text = readDataSQL_settingTable(db, query, "address_email");
            address_mail_string = decryptAES(text, key, iv);
        }
        read_setting_mail(check_mail, address_mail_string);
    }
    else
    {
        enter_old_pas->setText(tr("12345"));
        enter_old_pas->setReadOnly(true);
        dialog_message();
    }
}

void Window_setting::save_password()
{
    QString old_global_pas = enter_old_pas->text();       //Чтение старого пароля
    QString new_global_pas = enter_new_pas->text();       //чтение нового пароля
    QByteArray key, vector, master_pas;
    generateKey(key, vector);

    if (db.open()) {
        QSqlQuery query;
        if (!new_global_pas.isEmpty() && old_global_pas.isEmpty()) {
            master_pas = encryptAES(new_global_pas, key, vector);
            writeDataSQL_settingTable(db, query, 4, "main_key", key);
            writeDataSQL_settingTable(db, query, 5, "main_vector", vector);
            writeDataSQL_settingTable(db, query, 6, "master_pas", master_pas);
            enter_old_pas->setText(new_global_pas);
            enter_old_pas->setReadOnly(true);
        }

        if (new_global_pas.isEmpty() && old_global_pas.isEmpty()) {
            new_global_pas = "12345";
            master_pas = encryptAES(new_global_pas, key, vector);
            writeDataSQL_settingTable(db, query, 4, "main_key", key);
            writeDataSQL_settingTable(db, query, 5, "main_vector", vector);
            writeDataSQL_settingTable(db, query, 6, "master_pas", master_pas);
            enter_old_pas->setText(new_global_pas);
            enter_old_pas->setReadOnly(true);
        }

        if (!new_global_pas.isEmpty() && !old_global_pas.isEmpty()) {
            if (new_global_pas!=old_global_pas) {
                master_pas = encryptAES(new_global_pas, key, vector);
                writeDataSQL_settingTable(db, query, 4, "main_key", key);
                writeDataSQL_settingTable(db, query, 5, "main_vector", vector);
                writeDataSQL_settingTable(db, query, 6, "master_pas", master_pas);
                enter_old_pas->setText(new_global_pas);
                enter_old_pas->setReadOnly(true);
            }
        }
    }
    else {
        dialog_message();
    }
}

void Window_setting::save_mail_status()
{
    if (change_status_mail==true)
    {
        QByteArray check_mail= "0";
        QByteArray address = "";
        if (db.open())
        {
            QSqlQuery query;
            writeDataSQL_settingTable(db, query, 7, "availability_email", check_mail);
            writeDataSQL_settingTable(db, query, 10, "address_email", address);
            enter_address_mail->setText(address);
            change_status_mail = false;
        }
        else
        {
            dialog_message();
        }
    }
}

void Window_setting::save_mail()
{
    QString address = enter_address_mail->text();
    QByteArray key, vector, address_mail, check_mail;
    generateKey(key, vector);
    address_mail = encryptAES(address, key, vector);
    if (db.open())
    {
        QSqlQuery query;
        check_mail = "1";
        writeDataSQL_settingTable(db, query, 7, "availability_email", check_mail);
        writeDataSQL_settingTable(db, query, 8, "key_mail", key);
        writeDataSQL_settingTable(db, query, 9, "vector_mail", vector);
        writeDataSQL_settingTable(db, query, 10, "address_email", address_mail);
        address_mail_string = address;
        change_status_mail = false;
    }
    else
    {
        dialog_message();
    }
}

void Window_setting::save()
{
    progress_widget.show();
    movie->start();
    QApplication::processEvents(QEventLoop::AllEvents);
    if (check_box_for_email->isChecked())
    {
        if (address_mail_string != enter_address_mail->text()) {
            check_address_mail(enter_address_mail->text());
        }
        if (address_mail_string == enter_address_mail->text()) {
            save_password();
        }
    }
    else
    {
        save_mail_status();
        save_password();
    }
    movie->stop();
    progress_widget.close();
}

void Window_setting::cancel()
{
    this ->close();
}

bool Window_setting::eventFilter(QObject *obj, QEvent *event)   //Фильтр событий
{
    if (event->type() == QEvent::FocusIn)   //Если выбранный объект в фокусе (т.е. если например QLineEdit выбран для ввода)
                {
                    enter_new_pas->setToolTip(tr("Введите новый пароль длиной не менее 12 символов и сохраните его. \r\n Используйте символы, русские и английские буквы верхнего и нижних регистров, числа"));
                    difficult_of_passw();
                    return false;
                }
    else if (event->type() == QEvent::FocusOut)    //Если объект не в фокусе (выбран другой объект в этом виджете)
                {
                    label_difficult_passw->setText("<img src=':/img/emptiness2.png'>");
                    return QObject::eventFilter(obj, event);
                }
    else return QObject::eventFilter(obj, event);
}

void Window_setting::difficult_of_passw()
{
    QString enter_pas=enter_new_pas->text();    //введённый пароль
    QChar array_enter_pas[300];                 //массив символов пароля
    int difficult_of_pas=0;                     //Вычисляемая сложность пароля
    int counter_letters=0;                      //количество символов в пароле
    int counter_digits=0;                       //количество цифр в пароле
    int counter_spec_symbol=0;                  //количество спец символов в пароле

    //Ограничение длины вводимого пароля
    if (enter_pas.size()>300)
    {
        enter_pas.truncate(300);
        enter_new_pas->setText(enter_pas);
    }
    //Определение сложности вводимого мастер-пароля
    if (enter_pas.size()>1)
    {
        for (int i=0; i<32; i++)
        {
            if (enter_pas.contains(array_easy_pas[i], Qt::CaseInsensitive) or enter_pas.size()<7)
            //Найден простой, либо слишком короткий пароль
            {
                difficult_of_pas=1;
                label_difficult_passw->clear();
                label_difficult_passw->setText("<img src=':/img/bad_pas2.png'>");
                break;
            }
        }
        //подсчёт количества символов, цифр и спец символов в введённой строке
        for (int i=0; i<enter_pas.size(); i++)
        {
            array_enter_pas[i]=enter_pas[i];
            if (array_enter_pas[i].isLetter())          //Если в введённом пароле найдена буква
                counter_letters++;
            else if (array_enter_pas[i].isDigit())		//Если в введённом пароле найдена цифра
                counter_digits++;
        }
        //Подсчёт кол-ва спец символов, исключаем пробелы (20) и табуляции (9)
        for (int i=0; i<enter_pas.size(); i++)
        {
            if (!array_enter_pas[i].isLetter() and !array_enter_pas[i].isDigit() and array_enter_pas[i]!=QChar(20) and array_enter_pas[i]!=QChar(9))
                counter_spec_symbol++;
        }
        //Определение сложности пароля
        if (counter_letters>3 and counter_digits>3 and counter_spec_symbol>1 and enter_pas.size()>8 and difficult_of_pas!=1)
        {
            difficult_of_pas=3;
            label_difficult_passw->clear();
            label_difficult_passw->setText("<img src=':/img/nice_pas2.png'>");
        }
        else if (counter_letters>2 and counter_digits>2 and enter_pas.size()>6 and difficult_of_pas!=1)
        {
            difficult_of_pas=2;
            label_difficult_passw->clear();
            label_difficult_passw->setText("<img src=':/img/good_pas2.png'>");
        }
        else
        {
            label_difficult_passw->clear();
            label_difficult_passw->setText("<img src=':/img/bad_pas2.png'>");
        }
        //проверка пароля на повторяющиеся символы или цифры и будем выводить сообщение об этом
        if (repeating_sequence(enter_pas))
        {
            label_difficult_passw->clear();
            label_difficult_passw->setFixedWidth(500);
            label_difficult_passw->setText("Введённый пароль содержит повтояющиеся подряд элементы");
        }
        else
            label_difficult_passw->setFixedWidth(271);
    }
    else if (enter_pas.isEmpty())
    {
        label_difficult_passw->clear();
        label_difficult_passw->setText("<img src=':/img/emptiness2.png'>");
    }
}
//Проверка на повторяющиеся подряд элементы
bool Window_setting::repeating_sequence(QString enter_string)
{
    int counter_repeating=0;                //счётчик повторений элементов
    for (int i=0; i<enter_string.size(); i++)
    {
        if (i>0)
        {
            if (enter_string[i]==enter_string[i-1])
                counter_repeating++;
            else
            {
                if (counter_repeating==1)
                counter_repeating=0;
            }
        }
    }
    if (counter_repeating>=2)
        return true;
    else
        return false;
}

void Window_setting::gen_master_passw()
{
    if (QLineEdit::Password == enter_new_pas->echoMode())
        enter_new_pas->setEchoMode(QLineEdit::Password);

    enter_new_pas->setText(password_generator());
}

QString Window_setting::password_generator()
{
    QString generated_password;   //сгенерированный пароль
    int counter_position=0;       //текущая позиция элемента
    bool duplicate=false;         //если генерируемая позиция повторяется, генерация производится снова
    int max_limit_symbol=0;       //максимальное количество символов и цифр в генераторе случайных чисел
    int number_symbol=0;          //количество символов
    int number_digits=0;          //количество цифр
    int number_spec_symbol=0;     //количество спец. символов

    //Определение позиции элементов
    counter_position=0;
    while (counter_position!=len_string)
    {
        std::uniform_int_distribution<int>distribution(0,len_string-1);
        array_position[counter_position] = distribution(*QRandomGenerator::global());
        duplicate=false;
        for (int i=0; i<(len_string-1); i++)
        {
            if (array_position[i]==array_position[counter_position] and i!=counter_position)
            {
                duplicate=true;
            }
        }
        if (duplicate==true)
        {
            std::uniform_int_distribution<int>distribution(0,len_string-1);
            array_position[counter_position] = distribution(*QRandomGenerator::global());
        }
        else
        {
            counter_position+=1;
        }
    }

    //Вычисление количества символов, цифр и спец. символов в пароле
    max_limit_symbol=static_cast<int>(0.5*len_string);
    std::uniform_int_distribution<int>distribution(5,max_limit_symbol);
    number_symbol = distribution(*QRandomGenerator::global())+1;

    number_digits = distribution(*QRandomGenerator::global());

    number_spec_symbol=len_string-number_symbol-number_digits;
    while (number_spec_symbol<2)
    {
        number_spec_symbol+=1;
        number_symbol-=1;
    }

    //Генерация символов в пароле
    for (int i=0; i<number_symbol; i++)
    {
       std::uniform_int_distribution<int>distribution(0,51);
       gen_elem_array_code[array_position[i]] = distribution(*QRandomGenerator::global());
       gen_symbol[array_position[i]]=code_latin_symbol[gen_elem_array_code[array_position[i]]];
    }
    //Генерация цифр в пароле
    for (int i=number_symbol; i<number_digits+number_symbol; i++)
    {
        std::uniform_int_distribution<int>distribution(0,9);
        gen_elem_array_code[array_position[i]] = distribution(*QRandomGenerator::global());
        gen_symbol[array_position[i]]=code_digits[gen_elem_array_code[array_position[i]]];
    }
    //Генерация спец. символов в пароле
    for (int i=number_digits+number_symbol; i<len_string; i++)
    {
        std::uniform_int_distribution<int>distribution(0,31);
        gen_elem_array_code[array_position[i]] = distribution(*QRandomGenerator::global());
        gen_symbol[array_position[i]]=code_spec_symbol[gen_elem_array_code[array_position[i]]];
    }
    //Сборка пароля
    for (int i=0; i<len_string; i++)
    {
        generated_password[i]=QChar(gen_symbol[i]);
    }
    return generated_password;
}

void Window_setting::slot_request_pas()
{
    emit call_network_action("request_db", enter_new_pas->text(), "");
}

void Window_setting::result_connection(QString str)
{
    wait_widget->close();
    QMessageBox* error_msg1 = new QMessageBox(QMessageBox::Critical,"Ошибка!",str);
    if (error_msg1->exec()==QMessageBox::Ok)
    {delete error_msg1;}
}

void Window_setting::response_from_db(QString str)
{
    label_result_DB->setText(str);
}

void Window_setting::result_confirmation_email(QString str)
{
    if (str==tr("Письмо успешно отправлено!"))
    {
        wait_widget->close();
        save_mail();
        save_password();
    }
    else
    {
        QMessageBox* critical_msg = new QMessageBox(QMessageBox::Critical,
                                  tr("Ошибка!"),
                                  tr("Некорректный адрес email! <br>Сохранение данных приложения не выполнено!"));
        if (critical_msg->exec()==QMessageBox::Ok)
        {delete critical_msg;}
        wait_widget->close();
    }
}

void Window_setting::slot_check_size_line()
{
    QString str=tr("");
    if ((enter_address_mail->hasFocus())==true)
    {
        str=enter_address_mail->text();
        str.truncate(300);
        enter_address_mail->setText(str);
    }
}

void Window_setting:: visible_checkbox()
{
    if (check_box_for_email->isChecked())   //and already_checked==true
    {
        label_email->setVisible(true);
        label_email->setText(tr("Введите адрес электронной почты:"));
        enter_address_mail->setVisible(true);
        change_status_mail = true;
    }
    else
    {
        label_email->clear();
        label_email->setFixedHeight(24);
        label_email->setText("                      ");
        enter_address_mail->setVisible(false);
        change_status_mail = true;
    }
}

void Window_setting:: read_setting_mail(int& setting_mail, QString& address_mail)
{
    if (setting_mail==1)
    {
        label_email->setVisible(true);
        label_email->setText(tr("Введите адрес электронной почты:"));
        enter_address_mail->setVisible(true);
        enter_address_mail->setText(address_mail);
        check_box_for_email->setChecked(true);
    }
    else
    {
        label_email->clear();
        label_email->setFixedHeight(24);
        label_email->setText("                      ");
        enter_address_mail->setVisible(false);
        check_box_for_email->setChecked(false);
    }
}

void Window_setting::check_address_mail(QString address)
{
    bool false_address=false;
    int position_symbol=0;
    QString local_part;
    QString domain_part;
    if (address.isEmpty())
    {
        QMessageBox* critical_msg = new QMessageBox(QMessageBox::Critical,
                                  tr("Внимание!"),
                                  tr("Введите адрес электронной почты!"));
        if (critical_msg->exec()==QMessageBox::Ok)
        {delete critical_msg;}
    }
    else
    {
        /*Согласно RFC 3696 (раздел 3) - информация об ограничениях адреса email
         * Современные адреса email состоят из "local part", отделённых от
         * "domain part" (полного доменного имени) знаком at ("@")*/

        //не должно быть >1 символа @
        int counter_at=0;
        for (int i=0; i<address.size(); i++)
        {
            if (address[i]=="@")
                counter_at++;
        }
        if (counter_at>1)
            false_address=true;

        position_symbol=address.indexOf("@", Qt::CaseInsensitive);
        if (position_symbol==-1)
        {
            false_address=true;
        }
       else
        {
            local_part=address;
            local_part.remove(position_symbol, address.size());
            domain_part=address;
            domain_part.remove(0,position_symbol+1);
        }
        /*Если перед @ нет хотя бы 3-ух символов, то такой адрес email могут
        * распознать не все почтовые серверы, даже если такой адрес действительно существует
        * Поэтому выполним проверку на длину "local part"  *
        * также local part не должно превышать 255 символов*/
        if (local_part.size()<3 or local_part.size()>=255)
            false_address=true;

        //убираем пробелы и табуляции в конце "domain part"
        QString temp=domain_part;
        int size_temp=temp.size();
        while((temp[size_temp-1]==" ") or (temp[size_temp-1]==QChar(9)))
        {
            temp.remove((temp.size()-1), 1);
            size_temp--;
        }
        domain_part=temp;
        //"domain part" не должна быть длиннее 64 символов. Установим ещё что не короче 5, т.к. не все почтовые сервисы примут
        if (domain_part.size()<4 or domain_part.size()>=64)
            false_address=true;

        if (false_address)
        {
            QMessageBox* critical_msg = new QMessageBox(QMessageBox::Critical,
                                      tr("Ошибка"),
                                      tr("Адрес электронной почты введён некорректно!"));
            if (critical_msg->exec()==QMessageBox::Ok)
            {delete critical_msg;}
        }
        else
        {
            enter_address_mail->setText(local_part+"@"+domain_part);
            address=local_part+"@"+domain_part;
            wait_widget->show();
            emit call_network_action("test_message", address, "");
        }
    }
}

void Window_setting::getDataForSmtp()
{
    if (db.open())
    {
        QSqlQuery query;
        QByteArray ByteLogin, BytePassw, key_smtp, iv_smtp;
        QString login, passw;
        key_smtp = readDataSQL_settingTable(db, query, "key_smtp");
        iv_smtp = readDataSQL_settingTable(db, query, "vector_smtp");
        ByteLogin = readDataSQL_settingTable(db, query, "login_smtp");
        BytePassw = readDataSQL_settingTable(db, query, "smtp_pas");
        login = decryptAES(ByteLogin, key_smtp, iv_smtp);
        passw = decryptAES(BytePassw, key_smtp, iv_smtp);
        passw.replace(" ", "");
        emit send_data_smtp(login, passw);
    }
    else
    {
        dialog_message();
    }
}

void Window_setting::dialog_message()
{
    QMessageBox* msg = new QMessageBox(QMessageBox::Critical, tr("Ошибка!"), (tr("Вставьте USB-ключ!")));
    if (msg->exec()==QMessageBox::Ok)
    {delete msg;}
}

void Window_setting::changeVisibleOld_pas()
{
    if (QLineEdit::Password == enter_old_pas->echoMode())
    {
        visibleOldPas->setIcon(QIcon(":/img/open_lock.png"));
        visibleOldPas->setIconSize(QSize(26,26));
        enter_old_pas->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        visibleOldPas->setIcon(QIcon(":/img/closed_lock.png"));
        visibleOldPas->setIconSize(QSize(26,26));
        enter_old_pas->setEchoMode(QLineEdit::Password);
    }
}

void Window_setting::changeVisibleNew_pas()
{
    if (QLineEdit::Password == enter_new_pas->echoMode())
    {
        visibleNewPas->setIcon(QIcon(":/img/open_lock.png"));
        visibleNewPas->setIconSize(QSize(26,26));
        enter_new_pas->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        visibleNewPas->setIcon(QIcon(":/img/closed_lock.png"));
        visibleNewPas->setIconSize(QSize(26,26));
        enter_new_pas->setEchoMode(QLineEdit::Password);
    }
}

void Window_setting::closeEvent(QCloseEvent* event)
{
    //видимость паролей
    visibleOldPas->setIcon(QIcon(":/img/closed_lock.png"));
    visibleOldPas->setIconSize(QSize(26,26));
    enter_old_pas->setEchoMode(QLineEdit::Password);

    visibleNewPas->setIcon(QIcon(":/img/closed_lock.png"));
    visibleNewPas->setIconSize(QSize(26,26));
    enter_new_pas->setEchoMode(QLineEdit::Password);

    event->accept();
}
