#include "client_window.h"



Client_window::Client_window(QWidget *parent): QMainWindow(parent)
{
    tabs_counter=0;
    MyGlobalWindow = new QTabWidget(this);                 //Главный виджет
    setCentralWidget(MyGlobalWindow);                      //QMainWindow становится владельцем указателя виджета и удаляет его в соответствующее время
    MyGlobalWindow->setAttribute(Qt::WA_DeleteOnClose);    //очистка при закрытии окна

    setAcceptDrops(true);                               //разрешение сбрасываний на виджет файлов
    counterBackgroundsSaves=0;                          //кол-во вкладок с изменённым фоном
    file_for_paths_bg.setFileName("data_img.bin");      //файл куда сохраняются пути к изображениям

    //Угловой виджет поиска по вкладкам
    search_widget= new QWidget(this);
    search_widget->setObjectName("search_widget");  //присваиваем имя объекту класса QWidget, для обращения к нему в QSS
    search_line = new QLineEdit(this);
    search_line->setPlaceholderText(tr("Поиск по имени ресурса"));
    search_line->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    search_line->installEventFilter(this);
    key_search = new QShortcut(this);
    key_search->setKey(Qt::Key_Return);
    search_button = new QPushButton(this);
    search_button->setText(tr("Поиск"));
    search_button->setFixedWidth(50);
    connect(search_button, SIGNAL(clicked()), this, SLOT(search_tabs()));
    layoutCornerWidget = new QHBoxLayout();
    layoutCornerWidget->setContentsMargins(1, 1, 5, 5);        //Рамки компоновки элементов
    layoutCornerWidget->addWidget(search_line);
    layoutCornerWidget->setAlignment(search_line, Qt::AlignRight);
    layoutCornerWidget->setAlignment(search_line, Qt::AlignVCenter);
    layoutCornerWidget->setSpacing(5);                         //Расстояние между полем ввода и кнопкой
    layoutCornerWidget->addWidget(search_button);
    layoutCornerWidget->setAlignment(search_button, Qt::AlignRight);
    layoutCornerWidget->setAlignment(search_button, Qt::AlignVCenter);
    search_widget->setLayout(layoutCornerWidget);
    MyGlobalWindow->setCornerWidget(search_widget, Qt::TopRightCorner);

    //Прогресс бар
    wait_widget.setAlignment(Qt::AlignCenter);
    wait_widget.setWindowFlags(Qt::WindowStaysOnTopHint);          //поверх окон
    wait_widget.setWindowFlags(Qt::FramelessWindowHint);           //отключаем обрамление окна виджета
    wait_widget.setAttribute(Qt::WA_TranslucentBackground, true);  //Прозрачность
    wait_widget.resize(150, 150);
    movie = new QMovie(&wait_widget);
    movie->setFileName(":/img/loading.gif");
    movie->setScaledSize(wait_widget.size());
    wait_widget.setMovie(movie);
    //--------------------------------------------------------

    //Добавляем 1-ую вкладку
    tab_widget[0] = new Tab_template(this);
    MyGlobalWindow->addTab(tab_widget[0], QString("Вкладка %1").arg(QString::number(1)));   //QTabWidget становится владельцем tab_widget[0]
    connect(tab_widget[0], &Tab_template::signal_save_tab, this, &Client_window::writeData_null);

    //Используем последнюю вкладку как кнопку добавления новых вкладок
    widgetButtonAddTab = new QWidget(this);
    MyGlobalWindow->addTab(widgetButtonAddTab, QString(""));    //QTabWidget становится владельцем widgetButtonAddTab
    button_addTab = new QToolButton(this);
    button_addTab->setText(" +");
    MyGlobalWindow->tabBar()->setTabButton(1, QTabBar::RightSide, button_addTab);
    connect(button_addTab, SIGNAL(clicked()), this, SLOT(addTab()));
    MyGlobalWindow->setTabEnabled(1, false);   //блокировка вкладки с кнопкой добавления

    //Кастомная кнопка закрытия вкладок
    closeBtn.load(":/img/closeButton.png");
    button_delTab[0] = new QToolButton(this);
    MyGlobalWindow->tabBar()->setTabButton(0, QTabBar::RightSide, button_delTab[0]);
    button_delTab[0]->setIcon(QIcon(closeBtn));
    button_delTab[0]->setIconSize(QSize(20, 20));
    connect(button_delTab[0], SIGNAL(clicked()), this, SLOT(closeTab()));
    button_delTab[0]->setVisible(false);
    //-----------------------------------------------------------------------

    //Окно настроек приложения
    setting_window = new Window_setting();
    setting_window->setWindowModality(Qt::ApplicationModal);

    //Действия меню и статус-бар
    createActions();
    createMenus();
    QString message = tr("Приложение успешно открылось. Откройте справку для получения информации");
    statusBar()->showMessage(message);

    settings = new QSettings("My_Corp", "Spreadsheet");
    setWindowTitle(tr("Password Manager"));
    setWindowIcon(QIcon(":/img/ico4017.png")); //Установка иконки приложения отображаемой в справке о программе и в панели задач
    setMinimumSize(500, 600);
    resize(900, 700);

    //стандартный фон вкладок
    pixmap_default = QPixmap(":/img/image_forest_75proc.jpg").scaled(this->size(), Qt::IgnoreAspectRatio);
    //параметры шрифта
    QFont font1("Times", 14, QFont::Normal);
    MyGlobalWindow->setFont(font1);
}

Client_window:: ~Client_window()
{
    delete settings;
    settings=nullptr;
    delete setting_window;

    delete layoutCornerWidget;
    delete MyGlobalWindow;
}

void Client_window::dialog_message()
{
    QMessageBox* msg = new QMessageBox(QMessageBox::Critical, tr("Ошибка!"), (tr("Вставьте USB-ключ!")));
    if (msg->exec()==QMessageBox::Ok)
    {delete msg;}
}

void Client_window::show_progressBar(bool visible)
{
    if (visible) {
        double height = MyGlobalWindow->height();
        double width = MyGlobalWindow->width();
        int x = static_cast<int>(width / 2.0);
        int y = static_cast<int>(height / 2.0);
        wait_widget.setGeometry(x-90, y-70, 150, 150);
        wait_widget.show();
        movie->start();
    }
    if (!visible) {
        movie->stop();
        wait_widget.close();
    }
}

#ifndef QT_NO_CONTEXTMENU
void Client_window::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void Client_window::save()
{
    writeAllData();
}

void Client_window::redo()
{
    show_progressBar(true);
    setting_window->show();
    setting_window->read_settings();
    show_progressBar(false);
}

void Client_window::addTab()
{
    MyGlobalWindow->setTabEnabled(tabs_counter+1, true);   //разблокировка вкладки с кнопкой добавления
    tabs_counter++;
    if (tabs_counter<max_tabs-1)
    {
        tab_widget[tabs_counter] = new Tab_template(this);
        MyGlobalWindow->addTab(tab_widget[tabs_counter], QString("Вкладка %1").arg(QString::number(tabs_counter+1)));
        if (tabs_counter>0)
        {
            connect(tab_widget[tabs_counter], &Tab_template::signal_save_tab, this, &Client_window::writeData);
        }

        MyGlobalWindow->setCurrentWidget(tab_widget[tabs_counter]);
        //кнопка добавления вкладок
        MyGlobalWindow->tabBar()->moveTab(MyGlobalWindow->indexOf(widgetButtonAddTab),tabs_counter+1);    //Перемещаем кнопку добавления вкладки вправо от вкладок
        MyGlobalWindow->setTabEnabled(tabs_counter+1, false);                                         //блокировка вкладки с кнопкой добавления

        //Кнопка закрытия на добавленной вкладке
        button_delTab[tabs_counter] = new QToolButton(this);
        MyGlobalWindow->tabBar()->setTabButton(tabs_counter, QTabBar::RightSide, button_delTab[tabs_counter]);
        button_delTab[tabs_counter]->setIcon(QIcon(closeBtn));
        button_delTab[tabs_counter]->setIconSize(QSize(20, 20));
        connect(button_delTab[tabs_counter], SIGNAL(clicked()), this, SLOT(closeTab()));
        button_delTab[tabs_counter-1]->setVisible(false);

        QPalette pal;
        set_default_bg_image(pal, tabs_counter);
    }
    else
    {
        QMessageBox* msg1 = new QMessageBox(QMessageBox::Warning,
                                        tr("Внимание!"),
                                        tr("Больше добавлять вкладки нельзя!"));
        if (msg1->exec()==QMessageBox::Ok)
        {delete msg1;}
        tabs_counter--;
    }
}

void Client_window::closeTab()
{
    if (tabs_counter<1)
         {
            QMessageBox* msg1 = new QMessageBox(QMessageBox::Warning,
                               tr("Внимание!"),
                               tr("Первая вкладка не может быть закрыта!"));
            if (msg1->exec()==QMessageBox::Ok)
            {delete msg1;}
         }
    else
         {
            disconnect(button_delTab[tabs_counter], SIGNAL(clicked()), this, SLOT(closeTab()));
            button_delTab[tabs_counter-1]->setVisible(true);
            //deleteDataTab(tabs_counter);
            MyGlobalWindow->removeTab(tabs_counter);
            delete tab_widget[tabs_counter];
            tab_widget[tabs_counter]=nullptr;
            MyGlobalWindow->setCurrentWidget(tab_widget[tabs_counter-1]);
            tabs_counter--;
            settings->setValue("number_tabs", tabs_counter);
            if (tabs_counter==0)
                button_delTab[tabs_counter]->setVisible(false);
    }
}

void Client_window::about()
{
    QMessageBox msg;
    QSpacerItem* horizontalSpacer = new QSpacerItem(900, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QPixmap pixIcon(":/img/ico4017.png");
    msg.setIconPixmap(pixIcon.scaled(100,100));
    msg.setText(tr("Программа <b>Password Manager</b> позволяет хранить логины и пароли от аккаунтов различных сервисов,<br>"
                   "форумов, соц. сетей и приложений. Данные сохраняются в зашифрованном виде, в базе данных,<br>"
                   "расположенной на USB-носителе (USB-ключе). Используется алгоритм шифрования данных AES256<br>"
                   "Присутствует возможность добавления и удаления вкладок с данными (максимум 30 вкладок).<br>"
                   "В правом верхнем углу доступен виджет поиска по вкладкам по имени ресурса.<br>"
                   "Если искомый ресурс найден, то приложение выполняет переход на вкладку с найденным ресурсом<br>"
                   "и выделяет его название.<br>"
                   "При использовании кнопок копирования логина и пароля данные находятся в буфере обмена<br>"
                   "в течение 5 сек. Время хранения ограничено из соображений безопасности.<br>"
                   "В журнал буфера обмена данные не попадают (Windows 10).<br>"
                   "Присутствует возможность генерировать пароли автоматически. По умолчанию, такие пароли содержат<br>"
                   "символы, цифры, спец. символы, что увеличивает их стойкость. Длина генерируемых паролей<br>"
                   "для ресурсов - 10 символов, для генерируемого мастер-пароля приложения - 15 символов.<br>"
                   "Также присутствует возможность проверки мастер-пароля в базе данных утекших в сеть паролей.<br>"
                   "Используется база данных авторитетного сервиса <a href='https://haveibeenpwned.com/Passwords/'>Have I Been Pwned</a><br>"
                   "При вводе нового мастер-пароля автоматически определяется уровень его стойкости.<br>"
                   "Рекомендуется генерировать автоматически мастер-пароль, либо использовать уникальный, стойкий пароль.<br>"
                   "Присутствует возможность привязать адрес электронной почты к приложению, чтобы при запуске<br>"
                   "приложения получать на почту одноразовый код подтверждения, позволяющий добавить<br>"
                   "ещё один уровень защиты приложения (двухфакторная аутентификация - 2FA)<br>"
                   "Данная функция также позволяет мгновенно получить информацию о запуске приложения<br>"
                   "без вашего ведома (в сообщении содержится информация о дате и времени запуска, а также<br>"
                   "об операционной системе, в которой производится запуск приложения).<br>"
                   "Максимальное количество попыток входа в приложение - 3 (ввод пароля и/или кода подтверждения).<br>"
                   "При превышении кол-ва попыток приложение блокируется. Для разблокировки приложения<br>"
                   "обратитесь к разработчику.<br>"
                   "В качестве фона каждой отдельной вкладки может использоваться любое изображение формата .JPG<br>"
                   "Для изменения фона вкладки, перетащите желаемое изображение мышкой на пространство окна<br>"
                   "приложения. Не рекомендуется использовать изображения имеющие большой размер файла (более 1 Мб),<br>"
                   "Так как это может замедлять запуск приложения (увеличение времени загрузки фона вкладок)."
                   " <br>                                                "
                   " <br>                                                "
                   " <br>                                                 "
                   "Контакты для связи с разработчиком приложения: <br>"
                   "e-mail:   ilya-filatov-1994@mail.ru             "));
    QGridLayout* layout = new QGridLayout;
    layout = static_cast<QGridLayout*>(msg.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    if (msg.exec()==QMessageBox::Ok)
    {
        delete layout;
        delete horizontalSpacer;
    }
}

void Client_window::createActions()
{
    saveAct = new QAction(tr("&Сохранить все вкладки"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить все данные вкладок"));
    connect(saveAct, &QAction::triggered, this, &Client_window::save);

    exitAct = new QAction(tr("&Закрыть приложение"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Выйти из приложения"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    redoAct = new QAction(tr("&Настройки пароля приложения"), this);
    redoAct->setShortcut(tr("Ctrl+X"));
    redoAct->setStatusTip(tr("Переход к настройкам приложения"));
    connect(redoAct, &QAction::triggered, this, &Client_window::redo);

    adding_tabs = new QAction(tr("&Добавить вкладку"), this);
    adding_tabs->setShortcut(tr("Ctrl+A"));
    adding_tabs->setStatusTip(tr("Добавление новой вкладки"));
    connect(adding_tabs, &QAction::triggered, this, &Client_window::addTab);

    delete_tabs = new QAction(tr("&Удалить последнюю вкладку"), this);
    delete_tabs->setShortcut(tr("Ctrl+D"));
    delete_tabs->setStatusTip(tr("Удаление последней по счёту вкладки"));
    connect(delete_tabs, &QAction::triggered, this, &Client_window::closeTab);

    aboutAct = new QAction(tr("&О программе"), this);
    aboutAct->setShortcut(tr("Ctrl+O"));
    aboutAct->setStatusTip(tr("Показать информацию о приложении"));
    connect(aboutAct, &QAction::triggered, this, &Client_window::about);
}

void Client_window::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Настройки"));
    editMenu->addAction(redoAct);
    editMenu->addAction(adding_tabs);
    editMenu->addAction(delete_tabs);

    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpMenu->addAction(aboutAct);
}

void Client_window::readSettings()
{
    db = QSqlDatabase::database();
    if(db.open())
    {
        readData();
    }
    else {
        dialog_message();
    }
    QApplication::processEvents(QEventLoop::AllEvents);
}

void Client_window::readData()
{
    QPalette pal;
    tabs_counter=settings->value("number_tabs").toInt();

    tab_widget[0]->read_data_tab(db, 0);
    set_default_bg_image(pal, 0);
    for (int i=1; i<=tabs_counter; i++)
    {
        tab_widget[i] = new Tab_template(this);
        MyGlobalWindow->addTab(tab_widget[i], QString("Вкладка %1").arg(QString::number(i+1)));
        connect(tab_widget[i], &Tab_template::signal_save_tab, this, &Client_window::writeData);
        tab_widget[i]->read_data_tab(db, i);
        set_default_bg_image(pal, i);

        MyGlobalWindow->tabBar()->moveTab(i,i+1);

        //Кнопка закрытия вкладок
        button_delTab[i] = new QToolButton(this);
        MyGlobalWindow->tabBar()->setTabButton(i, QTabBar::RightSide, button_delTab[i]);
        button_delTab[i]->setIcon(QIcon(closeBtn));
        button_delTab[i]->setIconSize(QSize(20, 20));
        connect(button_delTab[i], SIGNAL(clicked()), this, SLOT(closeTab()));
        button_delTab[i]->setVisible(false);
        if (i==tabs_counter)
            button_delTab[i]->setVisible(true);

        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    MyGlobalWindow->setCurrentWidget(tab_widget[tabs_counter]);
    MyGlobalWindow->setTabEnabled(tabs_counter+1, false);   //блокировка вкладки с кнопкой добавления
}

void Client_window::writeData_null()
{
    show_progressBar(true);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    int current_tab = MyGlobalWindow->currentIndex();
    if (current_tab>settings->value("number_tabs").toInt()) {
        settings->setValue("number_tabs", current_tab);
    }
    if(db.open())
    {
        tab_widget[0]->write_data_tab(db, 0);
    }
    else {
        dialog_message();
    }
    show_progressBar(false);
    QApplication::processEvents(QEventLoop::AllEvents);
}

void Client_window::writeData()
{
    show_progressBar(true);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    int current_tab = MyGlobalWindow->currentIndex();
    if (current_tab>settings->value("number_tabs").toInt()) {
        settings->setValue("number_tabs", current_tab);
    }
    if(db.open())
    {
        tab_widget[current_tab]->write_data_tab(db, current_tab);
    }
    else {
        dialog_message();
    }
    show_progressBar(false);
    QApplication::processEvents(QEventLoop::AllEvents);
}

void Client_window::writeAllData()
{
    show_progressBar(true);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    settings->setValue("number_tabs", tabs_counter);     //Сохранение в реестре количества открытых вкладок
    QApplication::processEvents(QEventLoop::AllEvents);
    if(db.open())
    {
        for (int i=0; i<=tabs_counter; i++)
        {
            tab_widget[i]->write_data_tab(db, i);
        }
    }
    else {
        dialog_message();
    }
    show_progressBar(false);
}

void Client_window::writeChangedData()      //запись данных только тех вкладок, на которых произошли изменения
{
    show_progressBar(true);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    int current_tab = MyGlobalWindow->currentIndex();
    if (current_tab>settings->value("number_tabs").toInt()) {
        settings->setValue("number_tabs", current_tab);
    }
    if(db.open())
    {
        for (int i=0; i<=tabs_counter; i++)
            tab_widget[i]->save_tab_change(db, i);
    }
    else {
        dialog_message();
    }
    QApplication::processEvents(QEventLoop::AllEvents);
    show_progressBar(false);
}

//Проверка на сохранение перед выходом из приложения
void Client_window::closeEvent(QCloseEvent* event)
{
    bool change_data=false;
    for (int i=0; i<=tabs_counter; i++)
    {
        if (tab_widget[i]->get_flag_change_tab()) {
            change_data=true;
            break;
        }
    }
    if (change_data) {
        QMessageBox* close_message = new QMessageBox(QMessageBox::Information,
                                    tr("Сообщение"),
                                    tr("Данные вкладок были изменены. Сохранить изменения?"),
                                    QMessageBox::Yes | QMessageBox::No);
        if (close_message->exec()==QMessageBox::Yes) {
            writeChangedData();
            delete close_message;
            event->accept();
        }
        else {
            delete close_message;
            event->accept();
        }
    }
}

//поиск по вкладкам по имени ресурса
void Client_window::search_tabs()
{
    bool value_found=false;
    for (int i=0; i<(tabs_counter+1); i++)
    {
        if (tab_widget[i]->get_name_resource1().contains(search_line->text(), Qt::CaseInsensitive))
        {
            MyGlobalWindow->setCurrentIndex(i);
            tab_widget[i]->select_resource1();
            value_found=true;
            break;
        }
        if (tab_widget[i]->get_name_resource2().contains(search_line->text(), Qt::CaseInsensitive))
        {
            MyGlobalWindow->setCurrentIndex(i);
            tab_widget[i]->select_resource2();
            value_found=true;
            break;
        }
    }
    if (value_found==false)
    {
        QMessageBox* info_msg = new QMessageBox(QMessageBox::Information,
                                  tr("Сообщение"),
                                  tr("Искомый ресурс не найден"));
        if (info_msg->exec()==QMessageBox::Ok)
        {delete info_msg;}
    }
}

bool Client_window::eventFilter(QObject *obj, QEvent *event)   //Фильтр событий
{
    if (obj == search_line)
    {
        if (event->type() == QEvent::FocusIn)
                    {
                       search_line->setToolTip(tr("Введите имя ресурса для поиска по вкладкам"));
                       connect(key_search, SIGNAL(activated()), this, SLOT(search_tabs()));
                       return false;
                    }
        else if (event->type() == QEvent::FocusOut)
                   {
                       disconnect(key_search, SIGNAL(activated()), this, SLOT(search_tabs()));
                       return false;
                   }
        else return false;
    }
    else return QObject::eventFilter(obj, event);
}

void Client_window::set_bg_image(QPalette& pal, Qt::AspectRatioMode mode)
{
    counterBackgroundsSaves = settings->value("numberChangeBack").toInt();
    if (counterBackgroundsSaves>0) {
        if (file_for_paths_bg.exists()) {
            QVector<QString> vector_index;
            QVector<QString> vector_string;
            readAllPathImages(vector_index, vector_string);
            QString path;
            int index;
            for (int i = 0; i < counterBackgroundsSaves; i++) {
                index = vector_index[i].toInt();
                path = vector_string[i];
                if (index <= tabs_counter) {
                    QPixmap pixmap = QPixmap(path).scaled(this->size(), mode);
                    pal.setBrush(tab_widget[index]->backgroundRole(),QBrush(pixmap));
                    tab_widget[index]->setPalette(pal);
                    tab_widget[index]->setAutoFillBackground(true);
                }
            }
        }
    }
}

void Client_window::set_default_bg_image(QPalette& pal, int index)
{
    pal.setBrush(tab_widget[index]->backgroundRole(),QBrush(pixmap_default));
    tab_widget[index]->setPalette(pal);
    tab_widget[index]->setAutoFillBackground(true);
}

void Client_window::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    pixmap_default = QPixmap(":/img/image_forest_75proc.jpg").scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette pal;
    for (int i=0; i<=tabs_counter; i++) {
        set_default_bg_image(pal, i);
    }
    set_bg_image(pal, Qt::KeepAspectRatioByExpanding);
}

//вызывается каждый раз, когда перетаскиваемые объекты покидают границу окна виджета
void Client_window::dragLeaveEvent(QDragLeaveEvent* event)
{
   event->accept();
}
//вызывается каждый раз во время перетаскивания объекта
void Client_window::dragMoveEvent(QDragMoveEvent* event)
{
   event->accept();
}
//вызывается каждый раз, когда перетаскиваемые объекты пересекают границу окна виджета (входят)
void Client_window::dragEnterEvent(QDragEnterEvent* event)
{
    event->accept();
}

//вызывается при сбрасывании перетаскиваемых объектов в пределах окна виджета
void Client_window::dropEvent(QDropEvent* event)
{
   QString filePath=event->mimeData()->urls()[0].toLocalFile();
   //Проверяем расширение файла
   QFileInfo info_file(filePath);

   if ((info_file.suffix())!="jpg") {
       QMessageBox* warning_msg = new QMessageBox(QMessageBox::Warning,
                                       tr("Предупреждение"),
                                       tr("Используйте для изменения фона изоражения с расширением .JPG"));
       warning_msg->setWindowFlags(Qt::WindowStaysOnTopHint);  //поверх окон
       if (warning_msg->exec()==QMessageBox::Ok)
       {delete warning_msg;}
   }
   else
   {
        //Если файл формата .jpg то устанавливаем фон
        QPixmap image = QPixmap(filePath).scaled(this->size(), Qt::KeepAspectRatioByExpanding);
        QPalette palette;
        palette.setBrush(tab_widget[MyGlobalWindow->currentIndex()]->backgroundRole(),QBrush(image));
        tab_widget[MyGlobalWindow->currentIndex()]->setPalette(palette);              //Устанавливаем в виджете объект палитры
        tab_widget[MyGlobalWindow->currentIndex()]->setAutoFillBackground(true);      //Переводим свойство заполнения фона в true

        //Копирование файла изображения
        QFileInfo info(filePath);                   //полная инфа о файле
        QString fileName=info.fileName();           //имя копируемого файла
        QString copyPath=QDir::currentPath();       //Текущая директория приложения куда будет копироваться файл
        copyPath+="/"+fileName;                     //абсолютный путь куда копируется файл
        if (!QFile::exists(copyPath))               //если такого файла нет в директории куда копируем,
            QFile::copy(filePath, copyPath);        //выполняем копирование

        //Сохранение пути к файлу изображения
        int currentIndexTab = MyGlobalWindow->currentIndex();
        QString currentIndex = QString::number(currentIndexTab);
        QVector<QString> vector_index;
        QVector<QString> vector_string;
        counterBackgroundsSaves = settings->value("numberChangeBack").toInt();

        //Файл уже существует, читаем все индексы и пути
        if (file_for_paths_bg.exists()) {
            readAllPathImages(vector_index, vector_string);
            if (vector_index.contains(currentIndex)) {
                int indexVector = vector_index.indexOf(currentIndex);
                vector_string[indexVector].replace(vector_string[indexVector], filePath);
                writeAllPathImages(vector_index, vector_string);
            }
            else  {
                writeOnePathImage(currentIndex, filePath);
                vector_index << currentIndex;
                vector_string << filePath;
                counterBackgroundsSaves = vector_index.size();
                settings->setValue("numberChangeBack", counterBackgroundsSaves);
            }
        }
        if (!file_for_paths_bg.exists()) {
            writeOnePathImage(currentIndex, filePath);
            counterBackgroundsSaves = 1;
            settings->setValue("numberChangeBack", counterBackgroundsSaves);
        }
   }
}

void Client_window::readAllPathImages(QVector<QString>& vector_index, QVector<QString>& vector_string)
{
    file_for_paths_bg.open(QIODevice::ReadOnly);
    QDataStream* stream = new QDataStream(&file_for_paths_bg);
    stream->setVersion(QDataStream::Qt_4_9);
    QString index, str;
    for (int i = 0; i < counterBackgroundsSaves; i++) {
        *stream >> index;
        *stream >> str;
        vector_index << index;
        vector_string << str.replace("\r\n", "");
    }
    file_for_paths_bg.close();
    delete stream;
}

void Client_window::writeAllPathImages(QVector<QString>& vector_index, QVector<QString>& vector_string)
{
    file_for_paths_bg.remove();
    file_for_paths_bg.open(QIODevice::WriteOnly);
    QDataStream* stream = new QDataStream(&file_for_paths_bg);
    stream->setVersion(QDataStream::Qt_4_9);
    for (int i = 0; i < counterBackgroundsSaves; i++) {
        *stream << vector_index.at(i);
        *stream << ("\r\n"+vector_string.at(i)+"\r\n");
    }
    file_for_paths_bg.close();
    delete stream;
}

void Client_window::writeOnePathImage(QString& index, QString& pathFile)
{
    file_for_paths_bg.open(QIODevice::Append);
    QDataStream* stream = new QDataStream(&file_for_paths_bg);
    stream->setVersion(QDataStream::Qt_4_9);
    *stream << index;
    *stream << ("\r\n"+pathFile+"\r\n");
    file_for_paths_bg.close();
    delete stream;
}
