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
    name_label1 = new QLabel(this);
    name_label1->setText(tr("Имя ресурса"));
    name_label1->setFixedWidth(fixSizeLabel);
    name_resourse1 = new QLineEdit(this);
    name_resourse1->setPlaceholderText(tr("Введите название ресурса"));
    connect(name_resourse1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyLogin1 = new QPushButton(this);
    btnCopyLogin1->setText(tr("Копировать логин"));
    btnCopyLogin1->setFixedWidth(fixSizeButton);
    connect(btnCopyLogin1, SIGNAL(clicked()), this, SLOT(copy_login1()));
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
    login_label1 = new QLabel(this);
    login_label1->setText(tr("Логин"));
    login_label1->setFixedWidth(fixSizeLabel);
    login_line1 = new QLineEdit(this);
    login_line1->setPlaceholderText(tr("Введите логин"));
    connect(login_line1, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyPassw1 = new QPushButton(this);
    btnCopyPassw1->setText(tr("Копировать пароль"));
    btnCopyPassw1->setFixedWidth(fixSizeButton);
    connect(btnCopyPassw1, SIGNAL(clicked()), this, SLOT(copy_passw1()));
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
    passw_label1 = new QLabel(this);
    passw_label1->setText(tr("Пароль"));
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
    gen_pas_button1 = new QPushButton(this);
    gen_pas_button1->setText(tr("Генерировать пароль"));
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
    name_label2 = new QLabel(this);
    name_label2->setText(tr("Имя ресурса"));
    name_label2->setFixedWidth(fixSizeLabel);
    name_resourse2 = new QLineEdit(this);
    name_resourse2->setPlaceholderText(tr("Введите название ресурса"));
    connect(name_resourse2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyLogin2 = new QPushButton(this);
    btnCopyLogin2->setText(tr("Копировать логин"));
    btnCopyLogin2->setFixedWidth(fixSizeButton);
    connect(btnCopyLogin2, SIGNAL(clicked()), this, SLOT(copy_login2()));
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
    login_label2 = new QLabel(this);
    login_label2->setText(tr("Логин"));
    login_label2->setFixedWidth(fixSizeLabel);
    login_line2 = new QLineEdit(this);
    login_line2->setPlaceholderText(tr("Введите логин"));
    connect(login_line2, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   //проверка на длину вводимого текста
    btnCopyPassw2 = new QPushButton(this);
    btnCopyPassw2->setText(tr("Копировать пароль"));
    btnCopyPassw2->setFixedWidth(fixSizeButton);
    connect(btnCopyPassw2, SIGNAL(clicked()), this, SLOT(copy_passw2()));
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
    passw_label2 = new QLabel(this);
    passw_label2->setText(tr("Пароль"));
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
    gen_pas_button2 = new QPushButton(this);
    gen_pas_button2->setText(tr("Генерировать пароль"));
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
    save_change_page = new QPushButton(this);
    save_change_page->setText(tr("Сохранить изменения"));
    connect(save_change_page, &QPushButton::clicked, this, &Tab_template::save_data_tab);

    save_change_page->setFixedWidth(fixSizeButton);
    verticalWidget->addWidget(save_change_page);
    verticalWidget->setAlignment(save_change_page, Qt::AlignRight);
    verticalWidget->addStretch(1);

    widget->setLayout(verticalWidget);

    QFont font3("Times", 14, QFont::Normal);       //параметры шрифта для наименований вкладок
    widget->setFont(font3);

    //Таймеры буфера обмена
    connect(&timer1_clipboard, &QTimer::timeout, this, &Tab_template::timeout1_clipboard);
    connect(&timer2_clipboard, &QTimer::timeout, this, &Tab_template::timeout2_clipboard);
    connect(&timer3_clipboard, &QTimer::timeout, this, &Tab_template::timeout3_clipboard);
    connect(&timer4_clipboard, &QTimer::timeout, this, &Tab_template::timeout4_clipboard);

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
        str.truncate(300);
        name_resourse1->setText(str);
    }

    if ((login_line1->hasFocus())==true)
    {
        str=login_line1->text();
        str.truncate(300);
        login_line1->setText(str);
    }

    if ((passw_line1->hasFocus())==true)
    {
        str=passw_line1->text();
        str.truncate(300);
        passw_line1->setText(str);
    }

    if ((name_resourse2->hasFocus())==true)
    {
        str=name_resourse2->text();
        str.truncate(300);
        name_resourse2->setText(str);
    }

    if ((login_line2->hasFocus())==true)
    {
        str=login_line2->text();
        str.truncate(300);
        login_line2->setText(str);
    }

    if ((passw_line2->hasFocus())==true)
    {
        str=passw_line2->text();
        str.truncate(300);
        passw_line2->setText(str);
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

/*
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
    if (passw_line1->text().isEmpty() or QLineEdit::Password == passw_line1->echoMode())
        passw_line1->setEchoMode(QLineEdit::Password);

    passw_line1->setText(password_generator());
}

void Tab_template::generate_passw2()
{
    if (passw_line2->text().isEmpty() or QLineEdit::Password == passw_line2->echoMode())
        passw_line2->setEchoMode(QLineEdit::Password);

    passw_line2->setText(password_generator());
}

void Tab_template::copy_login1()
{
    timer1_clipboard.stop(); timer2_clipboard.stop(); timer3_clipboard.stop(); timer4_clipboard.stop();
    if (QApplication::clipboard())
    {
        mime.setText(login_line1->text());
        mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);

        timer1_clipboard.setInterval(5000);
        timer1_clipboard.start();
    }
}

void Tab_template::copy_passw1()
{
    timer1_clipboard.stop(); timer2_clipboard.stop(); timer3_clipboard.stop(); timer4_clipboard.stop();
    if (QApplication::clipboard())
    {
        mime.setText(passw_line1->text());
        mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);

        timer2_clipboard.setInterval(5000);
        timer2_clipboard.start();
    }
}

void Tab_template::copy_login2()
{
    timer1_clipboard.stop(); timer2_clipboard.stop(); timer3_clipboard.stop(); timer4_clipboard.stop();
    if (QApplication::clipboard())
    {
        mime.setText(login_line2->text());
        mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);

        timer3_clipboard.setInterval(5000);
        timer3_clipboard.start();
    }
}

void Tab_template::copy_passw2()
{
    timer1_clipboard.stop(); timer2_clipboard.stop(); timer3_clipboard.stop(); timer4_clipboard.stop();
    if (QApplication::clipboard())
    {
        mime.setText(passw_line2->text());
        mime.setData("ExcludeClipboardContentFromMonitorProcessing", QByteArrayLiteral("1"));
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);

        timer4_clipboard.setInterval(5000);
        timer4_clipboard.start();
    }
}

void Tab_template::timeout1_clipboard()
{
    QString inputStr = login_line1->text();
    if (inputStr == QApplication::clipboard()->text(QClipboard::Clipboard) || inputStr == QApplication::clipboard()->text(QClipboard::Selection))
    {
        mime.setText("");
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
    }
    timer1_clipboard.stop();
}

void Tab_template::timeout2_clipboard()
{
    QString inputStr = passw_line1->text();
    if (inputStr == QApplication::clipboard()->text(QClipboard::Clipboard) || inputStr == QApplication::clipboard()->text(QClipboard::Selection))
    {
        mime.setText("");
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
    }
    timer2_clipboard.stop();
}

void Tab_template::timeout3_clipboard()
{
    QString inputStr = login_line2->text();
    if (inputStr == QApplication::clipboard()->text(QClipboard::Clipboard) || inputStr == QApplication::clipboard()->text(QClipboard::Selection))
    {
        mime.setText("");
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
    }
    timer3_clipboard.stop();
}

void Tab_template::timeout4_clipboard()
{
    QString inputStr = passw_line2->text();
    if (inputStr == QApplication::clipboard()->text(QClipboard::Clipboard) || inputStr == QApplication::clipboard()->text(QClipboard::Selection))
    {
        mime.setText("");
        QApplication::clipboard()->setMimeData(&mime, QClipboard::Clipboard);
    }
    timer4_clipboard.stop();
}

QString Tab_template::password_generator()
{
    QString generated_password;       //сгенерированный пароль
    int counter_position=0;           //текущая позиция элемента
    bool duplicate=false;             //если генерируемая позиция повторяется, генерация производится снова
    int max_limit_symbol=0;           //максимальное количество символов и цифр в генераторе случайных чисел
    int number_symbol=0;              //количество символов
    int number_digits=0;              //количество цифр

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
    //Вычисление количества символов, цифр и спец. символов в пароле (коэффициенты для 10-значного пароля)
    max_limit_symbol=static_cast<int>(0.5*len_string);
    std::uniform_int_distribution<int>distribution(3,max_limit_symbol);
    number_symbol = distribution(*QRandomGenerator::global());
    number_digits = distribution(*QRandomGenerator::global());
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

/*
void Tab_template::data_filling(int id)
{
    int current_index = id*2+1;

    name_resourse1->setText(tr("ресурс")+QString::number(current_index));
    login_line1->setText(tr("логин")+QString::number(current_index));
    passw_line1->setText(tr("пароль")+QString::number(current_index));

    current_index = id*2+2;

    name_resourse2->setText(tr("ресурс")+QString::number(current_index));
    login_line2->setText(tr("логин")+QString::number(current_index));
    passw_line2->setText(tr("пароль")+QString::number(current_index));

}
*/
