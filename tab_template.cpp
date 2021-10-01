#include "tab_template.h"



Tab_template::Tab_template()
{
    counter1=0;
    counter2=0;
    widget= new QWidget(this);

    fixedHeightTemplateWidget = 40;     //Высота шаблонного виджета
    fixedWidthNameResource = 400;       //длина поля ввода имени ресурса
    fixWidthName=507;                   //Длина шаблонного виджета поля ввода имени ресурса fixedWidthNameResource+107
    fixSizeLabel=90;                    //длина Qlabel
    fixSizeButton=195;                  //длина Qpushbutton


    //-----Шаблон виджета с вводом имени ресурса-----1
    id_template_widget=0;                                                   //ID шаблонного виджета
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");   //Установим одно и тоже имя объекту чтобы применить QSS ко всем шаблонам
    id_horizontal_layout=0;                                                 //ID горизонтального layout для шаблонного виджета
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    label_name_data1 = new QLabel(this);
    label_name_data1->setText(tr("Имя ресурса"));
    label_name_data1->setFixedWidth(fixSizeLabel);
    name_data_entry1 = new QLineEdit(this);
    name_data_entry1->setPlaceholderText(tr("Введите название ресурса"));
    connect(name_data_entry1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    name_data_entry1->setFixedWidth(fixedWidthNameResource);
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(label_name_data1);
    horizontalWidget[id_horizontal_layout]->addWidget(name_data_entry1);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    templateWidget[id_template_widget]->setFixedWidth(fixWidthName);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);


    //-------------Шаблон виджета с вводом логина------1
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    label_login1 = new QLabel(this);
    label_login1->setText(tr("Логин"));
    label_login1->setFixedWidth(fixSizeLabel);
    username_entry1 = new QLineEdit(this);
    username_entry1->setPlaceholderText(tr("Введите логин"));
    connect(username_entry1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    gen_pas1 = new QPushButton(this);
    gen_pas1->setText(tr("Генерировать пароль"));
    gen_pas1->setFixedWidth(fixSizeButton);
    connect(gen_pas1, SIGNAL(clicked()), this, SLOT(generate_passw1()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(label_login1);
    horizontalWidget[id_horizontal_layout]->addWidget(username_entry1);
    horizontalWidget[id_horizontal_layout]->addWidget(gen_pas1);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //--------------------------------------------------------------------------


    //-----Шаблон виджета с вводом пароля ресурса-----1
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    label_pas1 = new QLabel(this);
    label_pas1->setText(tr("Пароль"));
    label_pas1->setFixedWidth(fixSizeLabel);
    password_entry1 = new QLineEdit(this);
    password_entry1->setPlaceholderText(tr("Введите пароль"));
    connect(password_entry1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    visible_pas1 = new QPushButton(this);
    visible_pas1->setText(tr("Скрыть/показать пароль"));
    visible_pas1->setFixedWidth(fixSizeButton);
    connect(visible_pas1, SIGNAL(clicked()), this, SLOT(visible_passw1()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(label_pas1);
    horizontalWidget[id_horizontal_layout]->addWidget(password_entry1);
    horizontalWidget[id_horizontal_layout]->addWidget(visible_pas1);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //--------------------------------------------------------------------------


    //-----Шаблон виджета с вводом имени ресурса-----2
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    label_name_data2 = new QLabel(this);
    label_name_data2->setText(tr("Имя ресурса"));
    label_name_data2->setFixedWidth(fixSizeLabel);
    name_data_entry2 = new QLineEdit(this);
    name_data_entry2->setFixedWidth(fixedWidthNameResource);
    name_data_entry2->setPlaceholderText(tr("Введите название ресурса"));
    connect(name_data_entry2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(label_name_data2);
    horizontalWidget[id_horizontal_layout]->addWidget(name_data_entry2);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    templateWidget[id_template_widget]->setFixedWidth(fixWidthName);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);


    //-------------Шаблон виджета с вводом логина------2
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    label_login2 = new QLabel(this);
    label_login2->setText(tr("Логин"));
    label_login2->setFixedWidth(fixSizeLabel);
    username_entry2 = new QLineEdit(this);
    username_entry2->setPlaceholderText(tr("Введите логин"));
    connect(username_entry2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    gen_pas2 = new QPushButton(this);
    gen_pas2->setText(tr("Генерировать пароль"));
    gen_pas2->setFixedWidth(fixSizeButton);
    connect(gen_pas2, SIGNAL(clicked()), this, SLOT(generate_passw2()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(label_login2);
    horizontalWidget[id_horizontal_layout]->addWidget(username_entry2);
    horizontalWidget[id_horizontal_layout]->addWidget(gen_pas2);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //--------------------------------------------------------------------------


    //-----Шаблон виджета с вводом пароля ресурса-----2
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    label_pas2 = new QLabel(this);
    label_pas2->setText(tr("Пароль"));
    label_pas2->setFixedWidth(fixSizeLabel);
    password_entry2 = new QLineEdit(this);
    password_entry2->setPlaceholderText(tr("Введите пароль"));
    connect(password_entry2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    visible_pas2 = new QPushButton(this);
    visible_pas2->setText(tr("Скрыть/показать пароль"));
    visible_pas2->setFixedWidth(fixSizeButton);
    connect(visible_pas2, SIGNAL(clicked()), this, SLOT(visible_passw2()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(label_pas2);
    horizontalWidget[id_horizontal_layout]->addWidget(password_entry2);
    horizontalWidget[id_horizontal_layout]->addWidget(visible_pas2);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //--------------------------------------------------------------------------

    //Компоновка всех виджетов
    verticalWidget = new QVBoxLayout();
    verticalWidget->setMargin(10);                       //Толщина рамки
    verticalWidget->setSpacing(20);                      //Расстояние между виджетами
    verticalWidget->addStretch(1);
    for (int i=0; i<(id_template_widget+1); i++)
    {
        verticalWidget->addWidget(templateWidget[i]);
        if (i==2)
        verticalWidget->addStretch(2);
    }
    verticalWidget->addStretch(2);
    save_change_page = new QPushButton(this);
    save_change_page->setText(tr("Сохранить изменения"));
    connect(save_change_page, SIGNAL(clicked()), this, SLOT(write_change_tab()));
    save_change_page->setFixedWidth(fixSizeButton);
    verticalWidget->addWidget(save_change_page);
    verticalWidget->setAlignment(save_change_page, Qt::AlignRight);
    verticalWidget->addStretch(1);

    widget->setLayout(verticalWidget);
    setCentralWidget(widget);
    setMinimumSize(500, 450);
    resize(900, 700);
}

Tab_template:: ~Tab_template()
{
}

void Tab_template::visible_passw1()
{
    counter1++;
    if (counter1 %2 == 0)
        {password_entry1->setEchoMode(QLineEdit::Normal);}
    else
        {password_entry1->setEchoMode(QLineEdit::Password);}
}

void Tab_template::visible_passw2()
{
    counter2++;
    if (counter2 %2 == 0)
        {password_entry2->setEchoMode(QLineEdit::Normal);}
    else
        {password_entry2->setEchoMode(QLineEdit::Password);}
}

void Tab_template::visible_password_regedit()
{
    if (password1==tr("Введите пароль"))
        password_entry1->setEchoMode(QLineEdit::Normal);
    else if (counter1 %2 != 0)
        password_entry1->setEchoMode(QLineEdit::Password);
    else
        password_entry1->setEchoMode(QLineEdit::Normal);

    if (password2==tr("Введите пароль"))
        password_entry2->setEchoMode(QLineEdit::Normal);
    else if (counter2 %2 != 0)
        password_entry2->setEchoMode(QLineEdit::Password);
    else
        password_entry2->setEchoMode(QLineEdit::Normal);
}

void Tab_template::write_regedit(QSettings* set, int counter1, int counter2, int counter)
{
     set->setValue(QString("visible_label%1").arg(QString::number(counter)), counter1);         //visible_label1 со значением counter1
     set->setValue(QString("visible_label%1").arg(QString::number(counter+100)), counter2);     //visible_label101 со значением counter2
}

void Tab_template::read_regedit(QSettings* set, int counter)
{
     counter1=set->value(QString("visible_label%1").arg(QString::number(counter))).toInt();
     counter2=set->value(QString("visible_label%1").arg(QString::number(counter+100))).toInt();
     visible_password_regedit();
}

void Tab_template::delete_regedit(QSettings* set, int counter)
{
    set->remove(QString("visible_label%1").arg(QString::number(counter)));
    set->remove(QString("visible_label%1").arg(QString::number(counter+100)));
}

void Tab_template::write_change_tab()
{
    emit save_page();
}

void Tab_template::write_data_tab(QDataStream* arg_stream)
{
    name_data_1=name_data_entry1->text();
    write_data(arg_stream, name_data_1);

    username1=username_entry1->text();
    write_data(arg_stream, username1);

    password1=password_entry1->text();
    write_data(arg_stream, password1);

    name_data_2=name_data_entry2->text();
    write_data(arg_stream, name_data_2);

    username2=username_entry2->text();
    write_data(arg_stream, username2);

    password2=password_entry2->text();
    write_data(arg_stream, password2);

}

void Tab_template::read_data_tab(QDataStream* arg_stream)
{
    name_data_1=read_data(arg_stream, name_data_1);
    name_data_entry1->setText(name_data_1);
    if (name_data_1.isEmpty())
        name_data_entry1->setPlaceholderText(tr("Введите название ресурса"));

    username1=read_data(arg_stream, username1);
    username_entry1->setText(username1);
    if (username1.isEmpty())
        username_entry1->setPlaceholderText(tr("Введите логин"));

    password1=read_data(arg_stream, password1);
    password_entry1->setText(password1);
    if (password1.isEmpty())
        password_entry1->setPlaceholderText(tr("Введите пароль"));

    name_data_2=read_data(arg_stream, name_data_2);
    name_data_entry2->setText(name_data_2);
    if (name_data_2.isEmpty())
        name_data_entry2->setPlaceholderText(tr("Введите название ресурса"));

    username2=read_data(arg_stream, username2);
    username_entry2->setText(username2);
    if (username2.isEmpty())
        username_entry2->setPlaceholderText(tr("Введите логин"));

    password2=read_data(arg_stream, password2);
    password_entry2->setText(password2);
    if (password2.isEmpty())
        password_entry2->setPlaceholderText(tr("Введите пароль"));
}

void Tab_template::write_data(QDataStream* arg_stream, QString str)
{
    double temp_arr[300];                                               //промежуточный массив double
    for (int i=0; i<300; i++)                                           //Первоначальная инициализация всего массива
    {temp_arr[i]=0;}
    My_Encrytion::encrypt(str, temp_arr);                               //Шифрование математической функцией
    My_Encrytion::shuffle_data(temp_arr, str.size(), 8);                //вызов функции перемешивания
    My_Encrytion::write_data(arg_stream, temp_arr, str.size());         //запись
}

QString Tab_template::read_data(QDataStream* arg_stream, QString str)
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

void Tab_template::otherwise_data_tab()
{
    name_data_entry1->setPlaceholderText(tr("Введите название ресурса"));
    username_entry1->setPlaceholderText(tr("Введите логин"));
    password_entry1->setPlaceholderText(tr("Введите пароль"));

    name_data_entry2->setPlaceholderText(tr("Введите название ресурса"));
    username_entry2->setPlaceholderText(tr("Введите логин"));
    password_entry2->setPlaceholderText(tr("Введите пароль"));
}

//Ограничение длины вводимых строк до 300 символов
void Tab_template::slot_check_size_line()
{
    QString str=tr(""); //инициализация
    if ((name_data_entry1->hasFocus())==true)
    {
        str=name_data_entry1->text();
        str.truncate(300);
        name_data_entry1->setText(str);
    }

    if ((username_entry1->hasFocus())==true)
    {
        str=username_entry1->text();
        str.truncate(300);
        username_entry1->setText(str);
    }

    if ((password_entry1->hasFocus())==true)
    {
        str=password_entry1->text();
        str.truncate(300);
        password_entry1->setText(str);
    }

    if ((name_data_entry2->hasFocus())==true)
    {
        str=name_data_entry2->text();
        str.truncate(300);
        name_data_entry2->setText(str);
    }

    if ((username_entry2->hasFocus())==true)
    {
        str=username_entry2->text();
        str.truncate(300);
        username_entry2->setText(str);
    }

    if ((password_entry2->hasFocus())==true)
    {
        str=password_entry2->text();
        str.truncate(300);
        password_entry2->setText(str);
    }
}


void Tab_template::generate_passw1()
{
    password_entry1->setText(password_generator());
}

void Tab_template::generate_passw2()
{
    password_entry2->setText(password_generator());
}

QString Tab_template::password_generator()
{
    QString generated_password;       //сгенерированный пароль
    int counter_position=0;           //текущая позиция элемента
    bool duplicate=false;             //если генерируемая позиция повторяется, генерация производится снова
    int max_limit_symbol=0;           //максимальное количество символов и цифр в генераторе случайных чисел
    int number_symbol=0;              //количество символов
    int number_digits=0;              //количество цифр
    //int number_spec_symbol=0;         //количество спец. символов

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
            counter_position=counter_position+1;
        }
    }

    //Вычисление количества символов, цифр и спец. символов в пароле (коэффициенты для 10-значного пароля)
    max_limit_symbol=static_cast<int>(0.5*len_string);
    std::uniform_int_distribution<int>distribution(3,max_limit_symbol);
    number_symbol = distribution(*QRandomGenerator::global());

    number_digits = distribution(*QRandomGenerator::global());

    //number_spec_symbol=len_string-number_symbol-number_digits;

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

QString Tab_template::get_name_resource1()
{
    QString str=name_data_entry1->text();
    return str;
}
QString Tab_template::get_name_resource2()
{
    QString str=name_data_entry2->text();
    return str;
}
void Tab_template::select_resource1()
{
    name_data_entry1->setFocus();
    name_data_entry1->selectAll();
}
void Tab_template::select_resource2()
{
    name_data_entry2->setFocus();
    name_data_entry2->selectAll();
}

void Tab_template::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    int newSizeWidget=static_cast<int>(0.56*(widget->width()));
    name_data_entry1->setFixedWidth(newSizeWidget-107);
    templateWidget[0]->setFixedWidth(newSizeWidget);
    name_data_entry2->setFixedWidth(newSizeWidget-107);
    templateWidget[3]->setFixedWidth(newSizeWidget);
}
