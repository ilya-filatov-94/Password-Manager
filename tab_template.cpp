#include "tab_template.h"


Tab_template::Tab_template(QWidget *parent) : QMainWindow(parent)
{
    widget= new QWidget(this);

    fixedHeightTemplateWidget = 40;     //Высота шаблонного виджета
    fixedWidthNameResource = 400;       //длина поля ввода имени ресурса
    fixSizeLabel=90;                    //длина Qlabel
    fixSizeButton=195;                  //длина Qpushbutton

    //-----Шаблон виджета с вводом имени ресурса-----1
    id_template_widget=0;                                                   //ID шаблонного виджета
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");   //Установим одно и тоже имя объекту чтобы применить QSS ко всем шаблонам
    id_horizontal_layout=0;                                                 //ID горизонтального layout для шаблонного виджета
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    name_label1 = new QLabel(tr("Имя ресурса"), this);
    name_label1->setFixedWidth(fixSizeLabel);
    name_resourse1 = new QLineEdit(this);
    name_resourse1->setPlaceholderText(tr("Введите название ресурса"));
    connect(name_resourse1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyLogin1 = new QPushButton(tr("Копировать логин"), this);
    btnCopyLogin1->setFixedWidth(fixSizeButton);
    connect(btnCopyLogin1, SIGNAL(clicked()), this, SLOT(abstractCopyData()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(name_label1);
    horizontalWidget[id_horizontal_layout]->addWidget(name_resourse1);
    horizontalWidget[id_horizontal_layout]->addWidget(btnCopyLogin1);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);

    //-------------Шаблон виджета с вводом логина------1
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    login_label1 = new QLabel(tr("Логин"), this);
    login_label1->setFixedWidth(fixSizeLabel);
    login_line1 = new QLineEdit(this);
    login_line1->setPlaceholderText(tr("Введите логин"));
    connect(login_line1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyPassw1 = new QPushButton(tr("Копировать пароль"), this);
    btnCopyPassw1->setFixedWidth(fixSizeButton);
    connect(btnCopyPassw1, SIGNAL(clicked()), this, SLOT(abstractCopyData()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(login_label1);
    horizontalWidget[id_horizontal_layout]->addWidget(login_line1);
    horizontalWidget[id_horizontal_layout]->addWidget(btnCopyPassw1);/********/
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //--------------------------------------------------------------------------

    //-----Шаблон виджета с вводом пароля ресурса-----1
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    passw_label1 = new QLabel(tr("Пароль"), this);
    passw_label1->setFixedWidth(fixSizeLabel);
    passw_line1 = new QLineEdit(this);
    passw_line1->setPlaceholderText(tr("Введите пароль"));
    connect(passw_line1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    visiblePassw1 = new QPushButton(this);
    visiblePassw1->setIcon(QIcon(":/img/closed_lock.png"));
    visiblePassw1->setIconSize(QSize(26,26));
    visiblePassw1->setFixedHeight(26);
    visiblePassw1->setFixedWidth(26);
    visiblePassw1->setToolTip(tr("Изменение видимости пароля"));
    connect(visiblePassw1, &QPushButton::clicked, this, &Tab_template::changeVisiblePas1);
    gen_pas_button1 = new QPushButton(tr("Генерировать пароль"), this);
    gen_pas_button1->setFixedWidth(fixSizeButton);
    connect(gen_pas_button1, SIGNAL(clicked()), this, SLOT(generate_passw1()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(passw_label1);
    horizontalWidget[id_horizontal_layout]->addWidget(passw_line1);
    horizontalWidget[id_horizontal_layout]->addWidget(visiblePassw1);
    horizontalWidget[id_horizontal_layout]->addWidget(gen_pas_button1);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //--------------------------------------------------------------------------

    //-----Шаблон виджета с вводом имени ресурса-----2
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    name_label2 = new QLabel(tr("Имя ресурса"), this);
    name_label2->setFixedWidth(fixSizeLabel);
    name_resourse2 = new QLineEdit(this);
    name_resourse2->setPlaceholderText(tr("Введите название ресурса"));
    connect(name_resourse2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyLogin2 = new QPushButton(tr("Копировать логин"), this);
    btnCopyLogin2->setFixedWidth(fixSizeButton);
    connect(btnCopyLogin2, SIGNAL(clicked()), this, SLOT(abstractCopyData()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(name_label2);
    horizontalWidget[id_horizontal_layout]->addWidget(name_resourse2);
    horizontalWidget[id_horizontal_layout]->addWidget(btnCopyLogin2);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);

    //-------------Шаблон виджета с вводом логина------2
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    login_label2 = new QLabel(tr("Логин"), this);
    login_label2->setFixedWidth(fixSizeLabel);
    login_line2 = new QLineEdit(this);
    login_line2->setPlaceholderText(tr("Введите логин"));
    connect(login_line2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyPassw2 = new QPushButton(tr("Копировать пароль"), this);
    btnCopyPassw2->setFixedWidth(fixSizeButton);
    connect(btnCopyPassw2, SIGNAL(clicked()), this, SLOT(abstractCopyData()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(login_label2);
    horizontalWidget[id_horizontal_layout]->addWidget(login_line2);
    horizontalWidget[id_horizontal_layout]->addWidget(btnCopyPassw2);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //--------------------------------------------------------------------------

    //-----Шаблон виджета с вводом пароля ресурса-----2
    id_template_widget+=1;
    templateWidget[id_template_widget] = new QWidget(this);
    templateWidget[id_template_widget]->setObjectName("templateWidget1");
    id_horizontal_layout+=1;
    horizontalWidget[id_horizontal_layout] = new QHBoxLayout();
    passw_label2 = new QLabel(tr("Пароль"), this);
    passw_label2->setFixedWidth(fixSizeLabel);
    passw_line2 = new QLineEdit(this);
    passw_line2->setPlaceholderText(tr("Введите пароль"));
    connect(passw_line2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    visiblePassw2 = new QPushButton(this);
    visiblePassw2->setIcon(QIcon(":/img/closed_lock.png"));
    visiblePassw2->setIconSize(QSize(26,26));
    visiblePassw2->setFixedHeight(26);
    visiblePassw2->setFixedWidth(26);
    visiblePassw2->setToolTip(tr("Изменение видимости пароля"));
    connect(visiblePassw2, &QPushButton::clicked, this, &Tab_template::changeVisiblePas2);
    gen_pas_button2 = new QPushButton(tr("Генерировать пароль"), this);
    gen_pas_button2->setFixedWidth(fixSizeButton);
    connect(gen_pas_button2, SIGNAL(clicked()), this, SLOT(generate_passw2()));
    horizontalWidget[id_horizontal_layout]->setMargin(5);
    horizontalWidget[id_horizontal_layout]->setSpacing(5);
    horizontalWidget[id_horizontal_layout]->addWidget(passw_label2);
    horizontalWidget[id_horizontal_layout]->addWidget(passw_line2);
    horizontalWidget[id_horizontal_layout]->addWidget(visiblePassw2);
    horizontalWidget[id_horizontal_layout]->addWidget(gen_pas_button2);
    templateWidget[id_template_widget]->setLayout(horizontalWidget[id_horizontal_layout]);
    templateWidget[id_template_widget]->setFixedHeight(fixedHeightTemplateWidget);
    //----------------------------------------------------------------------------------------

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
    save_change_page = new QPushButton(tr("Сохранить изменения"), this);
    connect(save_change_page, &QPushButton::clicked, this, &Tab_template::save_data_tab);

    save_change_page->setFixedWidth(fixSizeButton);
    verticalWidget->addWidget(save_change_page);
    verticalWidget->setAlignment(save_change_page, Qt::AlignRight);
    verticalWidget->addStretch(1);

    widget->setLayout(verticalWidget);

    QFont font3("Times", 14, QFont::Normal);       //параметры шрифта для наименований вкладок
    widget->setFont(font3);
    connect(&timer_clipboard, &QTimer::timeout, this, &Tab_template::timeout_clipboard);

    setCentralWidget(widget);
    setMinimumSize(500, 525);
    resize(900, 700);
}

Tab_template:: ~Tab_template()
{
    for (int i=0; i<6; i++)
    {delete horizontalWidget[i];}
    delete verticalWidget;
    delete widget;
}

void Tab_template::slot_check_size_line()
{
    QString str=tr("");
    if ((name_resourse1->hasFocus())==true)
    {
        str=name_resourse1->text();
        //Ячейки SQL таблицы ограничены 300 символами
        //(кодируется unicode - по 2 байта каждый - исключаем специфические вроде смайлов и иероглифов)
        //В алгоритме AES-256 режим CBC длина блока 16 байт
        //Ограничимся 18-ю блоками - 288 символов. Если будет 19 блоков - 304 символа - выход за пределы SQL таблицы
        str.truncate(288);
        name_resourse1->setText(str);
    }

    if ((login_line1->hasFocus())==true)
    {
        str=login_line1->text();
        str.truncate(288);
        login_line1->setText(str);
    }

    if ((passw_line1->hasFocus())==true)
    {
        str=passw_line1->text();
        str.truncate(288);
        passw_line1->setText(str);
        difficult_of_passw(passw_line1);
    }

    if ((name_resourse2->hasFocus())==true)
    {
        str=name_resourse2->text();
        str.truncate(288);
        name_resourse2->setText(str);
    }

    if ((login_line2->hasFocus())==true)
    {
        str=login_line2->text();
        str.truncate(288);
        login_line2->setText(str);
    }

    if ((passw_line2->hasFocus())==true)
    {
        str=passw_line2->text();
        str.truncate(288);
        passw_line2->setText(str);
        difficult_of_passw(passw_line2);
    }
    dataChanged = true;
}

void Tab_template::save_data_tab()
{
    emit signal_save_tab();
}

void Tab_template::read_data_tab(QSqlDatabase& db, int index_tab)
{
    QSqlQuery query;
    QString name, login, pas;
    if (readDataSQL(db, query, (index_tab*2+1), name, login, pas))
    {
        name_resourse1->setText(name);
        login_line1->setText(login);
        passw_line1->setText(pas);
    }

    if (readDataSQL(db, query, (index_tab*2+2), name, login, pas))
    {
        name_resourse2->setText(name);
        login_line2->setText(login);
        passw_line2->setText(pas);
    }
    setVisiblePas();
    dataChanged = false;    //инициализация флага после чтения данных вкладок
}

void Tab_template::write_data_tab(QSqlDatabase& db, int index_tab)
{
    QSqlQuery query;
    QString name, login, pas;
    name = name_resourse1->text();
    login = login_line1->text();
    pas = passw_line1->text();

    writeDataSQL(db, query, (index_tab*2+1), name, login, pas);

    name = name_resourse2->text();
    login = login_line2->text();
    pas = passw_line2->text();

    writeDataSQL(db, query, (index_tab*2+2), name, login, pas);
    dataChanged = false;    //изменения вкладки сохранены
}

/* Удаление и после создание строки SQL таблицы замедляет удаление вкладок и сохранение данных
Оптимальнее не удалять, а перезаписывать созданные строки SQL таблицы
void Tab_template::delete_data_tab(QSqlDatabase& db, int index_tab)
{
    QSqlQuery query;
    if (deleteDataSQL(db, query, (index_tab*2+1)))
    {
        qDebug() << "Успешно удалены данные ресурса вкладки " <<index_tab+1;
    }
    if (deleteDataSQL(db, query, (index_tab*2+2)))
    {
        qDebug() << "Успешно удалены данные ресурса вкладки " <<index_tab+1;
    }
}
*/

void Tab_template::save_tab_change(QSqlDatabase& db, int id)
{
    if (dataChanged==true)
    {
        write_data_tab(db, id);
    }
}

bool Tab_template::get_flag_change_tab()
{
    return dataChanged;
}

void Tab_template::setVisiblePas()
{
    if (!passw_line1->text().isEmpty())
    {
        visiblePassw1->setIcon(QIcon(":/img/closed_lock.png"));
        visiblePassw1->setIconSize(QSize(26,26));
        passw_line1->setEchoMode(QLineEdit::Password);
    }

    if (!passw_line2->text().isEmpty())
    {
        visiblePassw2->setIcon(QIcon(":/img/closed_lock.png"));
        visiblePassw2->setIconSize(QSize(26,26));
        passw_line2->setEchoMode(QLineEdit::Password);
    }
}

void Tab_template::changeVisiblePas1()
{
    if (QLineEdit::Password == passw_line1->echoMode())
    {
        visiblePassw1->setIcon(QIcon(":/img/open_lock.png"));
        visiblePassw1->setIconSize(QSize(26,26));
        passw_line1->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        visiblePassw1->setIcon(QIcon(":/img/closed_lock.png"));
        visiblePassw1->setIconSize(QSize(26,26));
        passw_line1->setEchoMode(QLineEdit::Password);
    }
}

void Tab_template::changeVisiblePas2()
{
    if (QLineEdit::Password == passw_line2->echoMode())
    {
        visiblePassw2->setIcon(QIcon(":/img/open_lock.png"));
        visiblePassw2->setIconSize(QSize(26,26));
        passw_line2->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        visiblePassw2->setIcon(QIcon(":/img/closed_lock.png"));
        visiblePassw2->setIconSize(QSize(26,26));
        passw_line2->setEchoMode(QLineEdit::Password);
    }
}

QString Tab_template::get_name_resource1()
{
    return name_resourse1->text();
}

QString Tab_template::get_name_resource2()
{
    return name_resourse2->text();
}

void Tab_template::select_resource1()
{
    name_resourse1->setFocus();
    name_resourse1->selectAll();
}
void Tab_template::select_resource2()
{
    name_resourse2->setFocus();
    name_resourse2->selectAll();
}

void Tab_template::generate_passw1()
{
    if (QLineEdit::Password == passw_line1->echoMode())
        passw_line1->setEchoMode(QLineEdit::Password);

    passw_line1->setText(password_generator(length_pas));
}

void Tab_template::generate_passw2()
{
    if (QLineEdit::Password == passw_line2->echoMode())
        passw_line2->setEchoMode(QLineEdit::Password);

    passw_line2->setText(password_generator(length_pas));
}

void Tab_template::difficult_of_passw(QLineEdit* line)
{
    QString enter_pas=line->text();                         //введённый пароль
    QPoint tooltipPos = line->mapToGlobal(QPoint(0,10));    //расположение всплывающей подсказки
    QFont font2("Times", 7, QFont::Normal);                 //шрифт всплывающей подсказки
    //Массив самых простых паролей
    const int sizeArraySimplePasswords = 33;
    QString array_easy_pas[sizeArraySimplePasswords]={"hello", "0123456789", "9876543210", "1234", "4567", "6789", "9876", "4321", "привет", "qwerty", "пароль", "йцукен", "asdfgh", "password", "ytrewq", "zxcvb", "1q2w3e", "dragon", "monkey", "qazwsx", "iloveyou", "pass", "default", "admin", "guest", "911", "314159", "271828", "122358", "Mypassword", "smoke", "sun", "mypas"};

    bool simplePas = false;
    int counter_letters=0;                      //количество символов в пароле
    int counter_digits=0;                       //количество цифр в пароле
    int counter_spec_symbol=0;                  //количество спец символов в пароле

    //Определение сложности вводимого мастер-пароля
    if (enter_pas.size()>1)
    {
        for (int i=0; i<(sizeArraySimplePasswords-1); i++)
        {
            if (enter_pas.contains(array_easy_pas[i], Qt::CaseInsensitive)) //Найдено часто используемое слово в пароле
            {
                simplePas = true;
                QToolTip::showText(tooltipPos, "<img src=':/img/bad_pas3.png'>", this);
                break;
            }
        }
        //подсчёт количества символов, цифр и спец символов в введённой строке
        for (int i=0; i<enter_pas.size(); i++)
        {
            if (enter_pas[i].isLetter())          //Если в введённом пароле найдена буква
                counter_letters++;
            else if (enter_pas[i].isDigit())	  //Если в введённом пароле найдена цифра
                counter_digits++;
            //Подсчёт кол-ва спец символов, исключаем пробелы (20) и табуляции (9)
            else if (!enter_pas[i].isLetter() and !enter_pas[i].isDigit() and enter_pas[i]!=QChar(20) and enter_pas[i]!=QChar(9))
                counter_spec_symbol++;
        }
        //Определение сложности пароля
        if (counter_letters>3 && counter_digits>3 && counter_spec_symbol>1 && !simplePas)
        {
            QToolTip::showText(tooltipPos, "<img src=':/img/nice_pas2.png'>", this);
        }
        else if (counter_letters>2 && counter_digits>2 && enter_pas.size()>7 && !simplePas)
        {
            QToolTip::showText(tooltipPos, "<img src=':/img/good_pas2.png'>", this);
        }
        else if (enter_pas.size()<8 && !simplePas)
        {
            QToolTip::showText(tooltipPos, "<img src=':/img/bad_pas2.png'>", line);
        }
        //проверка пароля на повторяющиеся подряд элементы (от 3-ёх подряд)
        if (repeating_sequence(enter_pas))
        {
            QToolTip::showText(tooltipPos, "Введённый пароль содержит повтояющиеся подряд элементы", this);
        }
    }
}

//Проверка на повторяющиеся подряд элементы
bool Tab_template::repeating_sequence(QString enter_string)
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
    if (counter_repeating>=2)   //считаем повторяющейся последовательностью - идущие 3 подряд символа и больше
        return true;
    else
        return false;
}

QString Tab_template::password_generator(int length_pas) {

    QString generated_password;             //строка сгенерированного пароля
    const int numberOfSymbolCodes = 52;     //кол-во латинских символов в unicode таблице
    const int numberOfDigitCodes = 10;      //количество цифр
    const int numberOfSpecSymbolCodes = 32; //кол-во спец символов в unicode таблице

    //Нужно соблюсти определённое количество символов, опр. количество цифр и опр. кол-во спец. символов, поэтому 3 отдельных массива Unicode
    int code_latin_symbol [numberOfSymbolCodes] = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};
    int code_digits [numberOfDigitCodes] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};
    int code_spec_symbol [numberOfSpecSymbolCodes] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 58, 59, 60, 61, 62, 63, 64, 91, 92, 93, 94, 95, 96, 123, 124, 125, 126};

    //Задаём примерное соотношение символов и цифр в пароле
    std::uniform_int_distribution<int>distribution1(static_cast<int>(length_pas * 0.4), static_cast<int>(length_pas * 0.5));
    int number_symbols = distribution1(*QRandomGenerator::global());
    std::uniform_int_distribution<int>distribution2(static_cast<int>(length_pas * 0.3), static_cast<int>(length_pas * 0.4));
    int number_digits = distribution2(*QRandomGenerator::global());

    //Генерация символов в пароле
    for (int i=0; i<number_symbols; i++)
    {
        std::uniform_int_distribution<int>distribution(0, numberOfSymbolCodes-1);
        generated_password[i] = QChar(code_latin_symbol[distribution(*QRandomGenerator::global())]);
    }

    //Генерация цифр в пароле
    for (int i=number_symbols; i<(number_symbols+number_digits); i++)
    {
        std::uniform_int_distribution<int>distribution(0, numberOfDigitCodes-1);
        generated_password[i] = QChar(code_digits[distribution(*QRandomGenerator::global())]);
    }

    //Генерация спец. символов в пароле
    for (int i=(number_symbols+number_digits); i<length_pas; i++)
    {
        std::uniform_int_distribution<int>distribution(0, numberOfSpecSymbolCodes-1);
        generated_password[i] = QChar(code_spec_symbol[distribution(*QRandomGenerator::global())]);
    }

    //Перемешивание элементов пароля
    for (int i = length_pas-1; i>0; i--)
    {
        std::uniform_int_distribution<int>distribution1(0, i);
        int rand = distribution1(*QRandomGenerator::global());
        if (i != rand)
        {
            QChar charElem = generated_password[i];
            generated_password[i] = generated_password[rand];
            generated_password[rand] = charElem;
        }
    }
    return generated_password;
}

void Tab_template::abstractCopyData()
{
    timer_clipboard.stop();
    QObject* object = QObject::sender();
    if (object == btnCopyLogin1)
    {
        if (QApplication::clipboard())
        {
            mime.setText(login_line1->text());
            mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
            QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
            timer_clipboard.setInterval(5000);
            timer_clipboard.start();
        }
    }

    if (object == btnCopyPassw1)
    {
        if (QApplication::clipboard())
        {
            mime.setText(passw_line1->text());
            mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
            QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
            timer_clipboard.setInterval(5000);
            timer_clipboard.start();
        }
    }

    if (object == btnCopyLogin2)
    {
        if (QApplication::clipboard())
        {
            mime.setText(login_line2->text());
            mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
            QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
            timer_clipboard.setInterval(5000);
            timer_clipboard.start();
        }
    }

    if (object == btnCopyPassw2)
    {
        if (QApplication::clipboard())
        {
            mime.setText(passw_line2->text());
            mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
            QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
            timer_clipboard.setInterval(5000);
            timer_clipboard.start();
        }
    }
}

void Tab_template::timeout_clipboard()
{
    mime.setText("");
    QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
    timer_clipboard.stop();
}
