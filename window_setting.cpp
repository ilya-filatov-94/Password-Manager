#include "window_setting.h"
#include <QDesktopServices>



Window_setting::Window_setting()

{
    Setting_window = new QWidget(this);
    Setting_window->setObjectName("Setting_window");
    set_mail_authoriz=0;                        //инициализация настройки аутентификации по почте
    permission_to_save=false;                   //инициализация

    //Виджеты показа текущего пароля
    label_saved_passw = new QLabel(this);
    label_saved_passw->setText(tr("Текущий мастер-пароль:"));
    enter_old_pas = new QLineEdit(this);
    enter_old_pas->setStyleSheet("background-color:rgb(255,255,255);");
    enter_old_pas->setFixedWidth(600);

    //Виджеты ввода нового пароля
    enter_new_pas = new QLineEdit(this);
    enter_new_pas->setStyleSheet("background-color:rgb(255,255,255);");
    connect(enter_new_pas, SIGNAL(textChanged(const QString &)), this, SLOT(difficult_of_passw()));
    enter_new_pas->installEventFilter(this);

    //Кнопки управления окном
    save_pas1 = new QPushButton(this);
    save_pas1->setText(tr("Сохранить настройки"));
    connect(save_pas1, SIGNAL(clicked()), this, SLOT(save1()));
    show_current_pas = new QPushButton(this);
    show_current_pas->setText(tr("Обновить окно"));
    connect(show_current_pas, SIGNAL(clicked()), this, SLOT(show1()));
    cancel_setting = new QPushButton(this);
    cancel_setting->setText(tr("Отмена"));
    connect(cancel_setting, SIGNAL(clicked()), this, SLOT(cancel1()));

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
    //label_check_pas_DB->openExternalLinks();
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

    //Компоновка виджетов подключения аутентификации по email
    check_box_for_email = new QCheckBox(this);
    connect(check_box_for_email,SIGNAL(toggled(bool)), this, SLOT(visible_check()));
    label_checkbox = new QLabel(this);
    label_checkbox->setText(tr("Аутентификация по электронной почте"));
    label_email = new QLabel(this);
    enter_address_mail = new QLineEdit(this);
    enter_address_mail->setStyleSheet("background-color:rgb(255,255,255);");
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
    vertical_new_pas->addWidget(enter_new_pas);
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
    horizontal_button_sett->addWidget(save_pas1);
    horizontal_button_sett->addWidget(show_current_pas);
    horizontal_button_sett->addWidget(cancel_setting);

    //Компоновка всех элементов
    QVBoxLayout* vertical1 = new QVBoxLayout;
    vertical1->setMargin(10);                       //Толщина рамки
    vertical1->setSpacing(15);                     //Расстояние между виджетами
    vertical1->addWidget(label_saved_passw);
    vertical1->addWidget(enter_old_pas);
    vertical1->setAlignment(enter_old_pas, Qt::AlignCenter);
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
    connect(this, &Window_setting::call_request_passw, network_connection1, &Network_connection::check_http_connection);
    connect(this, SIGNAL(destroyed()), thread1, SLOT(terminate()));
    thread1->start();
    network_connection1->moveToThread(thread1);
    //Результат проверки интернет соединения
    connect(network_connection1, &Network_connection::failure_internet_connection, this, &Window_setting::result_connection);

    //Результат ответ из базы данных паролей
    connect(network_connection1, &Network_connection::response_mes_db, this, &Window_setting::response_from_db);

    //Тестовое письмо на почту
    connect(this, &Window_setting::call_test_message, network_connection1, &Network_connection::check_http_connection);
    connect(network_connection1, &Network_connection::result_send_message, this, &Window_setting::result_confirmation_email);
    setWindowTitle(tr("Настройки приложения"));
    setMinimumSize(650, 485);
    setMaximumSize(650, 500);
    resize(650, 500);
}

Window_setting:: ~Window_setting()
{
    delete  wait_widget;
}

void Window_setting::save1()
{
    if (set_mail_authoriz==1 && permission_to_save==false)
    {
        check_address_mail();       //Проверка корректности введённого email

    }
    else
        permission_to_save=true;

    if (permission_to_save)
    {
        permission_to_save=false;
        emit save_main_password();
        QMessageBox* info_msg = new QMessageBox(QMessageBox::Information,
                                  tr("Информация!"),
                                  tr("Данные успешно сохранены!"));
        if (info_msg->exec()==QMessageBox::Ok)
        {delete info_msg;}
    }
}

void Window_setting::cancel1()
{
    this ->close();
}

void Window_setting::show1()
{
    emit show_main_password();
}

void Window_setting::write_global_setting(QDataStream* arg_stream)
{
    double setting_mail_authoriz=0;
    setting_mail_authoriz=static_cast<double>(set_mail_authoriz);
    *arg_stream << (setting_mail_authoriz);
    address_mail_string = enter_address_mail->text();       //чтение введённого адреса почты
    if (!address_mail_string.isEmpty() or set_mail_authoriz==1)
    {
        write_data(arg_stream, address_mail_string);
    }
    old_global_pas = enter_old_pas->text();       //Чтение старого пароля
    if (old_global_pas!=new_global_pas and !new_global_pas.isEmpty())
    {
        new_global_pas=enter_new_pas->text();
    }
    else if (!old_global_pas.isEmpty() and new_global_pas.isEmpty())
    {
        new_global_pas=old_global_pas;
    }
    else if (old_global_pas.isEmpty())
    {
        old_global_pas=tr("12345");
        new_global_pas=old_global_pas;
    }
    write_data(arg_stream, new_global_pas);
}

void Window_setting::read_global_setting(QDataStream* arg_stream)
{
    double setting_mail_authoriz=0;
    *arg_stream >> (setting_mail_authoriz);
    set_mail_authoriz=static_cast<int>(setting_mail_authoriz);
    if (!address_mail_string.isEmpty() or set_mail_authoriz==1)
    {
    address_mail_string=read_data(arg_stream, address_mail_string);
    enter_address_mail->setText(address_mail_string);
    }
    old_global_pas=read_data(arg_stream, old_global_pas);
    enter_old_pas->setText(old_global_pas);                             //Запись как текущего пароля
    enter_old_pas->setReadOnly(true);                                   //Блокировка ввода

}

void Window_setting::otherwise_global_setting()
{
    enter_address_mail->clear();
    old_global_pas = (tr("12345"));                                     //Если файла нет в текущей директории, записываем стандартный пароль
    enter_old_pas->setText(old_global_pas);
    enter_new_pas->setText(old_global_pas);
    enter_old_pas->setReadOnly(true);
}

QString Window_setting::read_data(QDataStream* arg_stream, QString str)
{
    str.clear();
    double temp_arr[300];                                               //промежуточный массив double
    for (int i=0; i<300; i++)                                           //Первоначальная инициализация всего массива
    {temp_arr[i]=0;}
    int size_str=My_Encrytion::read_data(arg_stream, temp_arr);         //чтение
    My_Encrytion::reverse_shuffle_data(temp_arr, size_str, 8);          //вызов функции обратного перемешивания
    str=My_Encrytion::decrypt(temp_arr, str, size_str);                 //расшифровка инвертированной математической функцией
    return str;
}

void Window_setting::write_data(QDataStream* arg_stream, QString str)
{
    double temp_arr[300];                                               //промежуточный массив double
    for (int i=0; i<300; i++)                                           //Первоначальная инициализация всего массива
    {temp_arr[i]=0;}
    My_Encrytion::encrypt(str, temp_arr);                               //Шифрование математической функцией
    My_Encrytion::shuffle_data(temp_arr, str.size(), 8);                //вызов функции перемешивания
    My_Encrytion::write_data(arg_stream, temp_arr, str.size());         //запись
}

bool Window_setting::eventFilter(QObject *obj, QEvent *event)   //Фильтр событий

{
    if (event->type() == QEvent::FocusIn)   //Если выбранный объект в фокусе (т.е. если например QLineEdit выбран для ввода)
                {
                    enter_new_pas->setToolTip(tr("Введите новый пароль длиной не менее 12 символов и сохраните его. \r\n Используйте символы, русские и английские буквы верхнего и нижних регистров, числа"));
                    new_global_pas=enter_new_pas->text();
                    difficult_of_passw();
                    return false;
                }
    else if (event->type() == QEvent::FocusOut)    //Если объект не в фокусе (выбран другой объект в этом виджете)
                {
                    new_global_pas=enter_new_pas->text();
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
        //Сюда вставим проверку пароля на повторяющиеся символы или цифры и будем выводить сообщение об этом
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
    QString passw=enter_new_pas->text();
    emit  call_request_passw("request_dataBase:"+passw);
}


void Window_setting::result_connection(QString str)
{
    int position=0;
    for (int i=0; i<str.size(); i++)
        position=str.indexOf("\n");
    str.remove(position-1, str.size());
    wait_widget->close();
    QMessageBox* error_msg1 = new QMessageBox(QMessageBox::Critical,"Ошибка!",str+"!");
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
        permission_to_save=true;
        wait_widget->close();
        save1();
    }
    else
    {
        permission_to_save=false;
        QMessageBox* critical_msg = new QMessageBox(QMessageBox::Critical,
                                  tr("Ошибка!"),
                                  tr("Некорректный адрес email! <br>Сохранение данных приложения не выполнено!"));
        if (critical_msg->exec()==QMessageBox::Ok)
        {delete critical_msg;}
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

void Window_setting:: visible_check()
{
    if (check_box_for_email->isChecked())   //and already_checked==true
    {
        label_email->setVisible(true);
        label_email->setText(tr("Введите адрес электронной почты:"));
        enter_address_mail->setVisible(true);
        set_mail_authoriz=1;
    }
    else
    {
        label_email->clear();
        label_email->setFixedHeight(24);
        label_email->setText("                      ");
        enter_address_mail->setVisible(false);
        set_mail_authoriz=0;
    }
}

void Window_setting:: read_setting_mail()
{
    if  (set_mail_authoriz==0)
    {
        label_email->clear();
        label_email->setFixedHeight(24);
        label_email->setText("                      ");
        enter_address_mail->setVisible(false);
        check_box_for_email->setChecked(false);
    }
    else if (set_mail_authoriz==1)
    {
        label_email->setVisible(true);
        label_email->setText(tr("Введите адрес электронной почты:"));
        enter_address_mail->setVisible(true);
        check_box_for_email->setChecked(true);
    }
}

void Window_setting::check_address_mail()
{
    bool false_address=false;
    int position_symbol=0;
    QString local_part;
    QString domain_part;
    QString str=enter_address_mail->text();
    if (str.isEmpty())
    {
        QMessageBox* critical_msg = new QMessageBox(QMessageBox::Critical,
                                  tr("Внимание!"),
                                  tr("Введите адрес электронной почты!"));
        if (critical_msg->exec()==QMessageBox::Ok)
        {delete critical_msg;}
        permission_to_save=false;
    }
    else
    {
        /*Согласно RFC 3696 (раздел 3) - информация об ограничениях адреса email
         * Современные адреса email состоят из "local part", отделённых от
         * "domain part" (полного доменного имени) знаком at ("@")*/

        //не должно быть >1 символа @
        int counter_at=0;
        for (int i=0; i<str.size(); i++)
        {
            if (str[i]=="@")
                counter_at++;
        }
        if (counter_at>1)
            false_address=true;

        position_symbol=str.indexOf("@", Qt::CaseInsensitive);
        if (position_symbol==-1)
        {
            false_address=true;
        }
       else
        {
            local_part=str;
            local_part.remove(position_symbol,str.size());
            domain_part=str;
            domain_part.remove(0,position_symbol+1);
        }
        /*Если перед @ нет хотя бы 2-ух символов, то такой адрес email могут
        * распознать не все почтовые серверы, даже если такой адрес действительно существует
        * Поэтому выполним проверку на длину "local part"  *
        * также local part не должно превышать 255 символов*/
        if (local_part.size()<=2 or local_part.size()>=255)
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
        if (domain_part.size()<=4 or domain_part.size()>=64)
            false_address=true;

        if (false_address)
        {
            QMessageBox* critical_msg = new QMessageBox(QMessageBox::Critical,
                                      tr("Ошибка"),
                                      tr("Адрес электронной почты введён некорректно!"));
            if (critical_msg->exec()==QMessageBox::Ok)
            {delete critical_msg;}
            permission_to_save=false;
        }
        else
        {
            enter_address_mail->setText(local_part+"@"+domain_part);
            str=local_part+"@"+domain_part;
            wait_widget->show();
            emit call_test_message("mail:"+str);

        }
    }
}

