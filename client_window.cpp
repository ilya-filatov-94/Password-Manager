#include "client_window.h"


Client_window::Client_window(QWidget *parent): QMainWindow(parent)
{
    file1 = new QFile("data1.bin");
    settings = new QSettings("My_Corp", "Spreadsheet");
    tabs_counter=0;
    counter_records=0;
    setting_window = new Window_setting();
    setting_window->setWindowModality(Qt::ApplicationModal);    //Модальное окно
    MyGlobalWindow = new QTabWidget(this);
    MyGlobalWindow->setMovable(true);                           //перемещение вкладок

    //-------Для перетаскиваний изображений для фона и сохранений фона вкладок------------
    setAcceptDrops(true);                               //разрешение сбрасываний на виджет файлов
    counter_change_palette=0;                           //кол-во вкладок с изменённым фоном
    file_for_paths = new QFile("data_img.bin");         //файл куда сохраняются пути к изображениям
    //--------------------------

    //Угловой виджет поиска по вкладкам
    search_widget= new QWidget(this);
    search_widget->setObjectName("search_widget");  //явно называем объект класса QWidget, для обращения к его имени в QSS
    search_line = new QLineEdit(this);
    search_line->setPlaceholderText(tr("Поиск по имени ресурса"));
    search_line->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    search_line->installEventFilter(this);
    key_return = new QShortcut(this);
    key_return->setKey(Qt::Key_Return);
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

    //1-ая вкладка
    tab_widget[0] = new Tab_template;
    MyGlobalWindow->addTab(tab_widget[0], QString("Вкладка %1").arg(QString::number(1)));

    QFont font1("Times", 14, QFont::Normal);       //параметры шрифта
    MyGlobalWindow->setFont(font1);
    setCentralWidget(MyGlobalWindow);        //Устанавливаем центральным виджетом MainWindow виджет MyGlobalWindow, главным становится MyGlobalWindow

    connect(setting_window, &Window_setting::save_main_password, this, &Client_window::writeSettings);
    connect(setting_window, &Window_setting::show_main_password, this, &Client_window::show_glob_pas);
    createActions();
    createMenus();
    QString message = tr("Приложение успешно открылось. Откройте справку для получения информации");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Password Manager"));
    setWindowIcon(QIcon(":/img/ico4017.png")); //Установка иконки приложения отображаемой в справке о программе
    setMinimumSize(500, 525);
    resize(900, 700);

}

Client_window:: ~Client_window()
{
    delete setting_window;
    for (int i=0; i<=tabs_counter; i++)
        delete tab_widget[i];
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
    write_all();
}

void Client_window::redo()
{
    setting_window->show();
    show_glob_pas();
    setting_window->read_setting_mail();
}

void Client_window::add_my_tab()
{
    tabs_counter++;
    if (tabs_counter<max_tabs-1)
         {
            if (tabs_counter==0)
            {
                tabs_counter=1;
            }
            tab_widget[tabs_counter] = new Tab_template;
            MyGlobalWindow->addTab(tab_widget[tabs_counter], QString("Вкладка %1").arg(QString::number(tabs_counter+1)));
            tab_widget[tabs_counter]->otherwise_data_tab();
            connect(tab_widget[tabs_counter], &Tab_template::save_page, this, &Client_window::writeSettings);
            set_backgound_image();
            MyGlobalWindow->setCurrentWidget(tab_widget[tabs_counter]);
         }
    else if (tabs_counter>=max_tabs-1)
         {
             QMessageBox* msg1 = new QMessageBox(QMessageBox::Warning,
                                    tr("Внимание!"),
                                    tr("Больше добавлять вкладки нельзя!"));
             if (msg1->exec()==QMessageBox::Ok)
             {delete msg1;}
             tabs_counter=max_tabs-2;
         }
}

void Client_window::delete_my_tab()
{
    if (tabs_counter<1)
         {
            QMessageBox* msg1 = new QMessageBox(QMessageBox::Warning,
                               tr("Внимание!"),
                               tr("Больше удалять вкладки нельзя!"));
            if (msg1->exec()==QMessageBox::Ok)
            {delete msg1;}

         }
    else
         {
            MyGlobalWindow->removeTab(tabs_counter);
            tab_widget[tabs_counter]->delete_regedit(settings, tabs_counter);
            delete tab_widget[tabs_counter];
            tab_widget[tabs_counter]=nullptr;
            tabs_counter--;
         }
}

void Client_window::about()
{
    //QMessageBox::about(this, tr("О программе"), tr("Программа Password Manager....."));
    QMessageBox msg;
    QSpacerItem* horizontalSpacer = new QSpacerItem(900, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QPixmap pixIcon(":/img/ico4017.png");
    msg.setIconPixmap(pixIcon.scaled(100,100));
    msg.setText(tr("Программа <b>Password Manager</b> позволяет хранить логины и пароли от аккаунтов различных сервисов,<br>"
                   "форумов, соц. сетей и приложений. Данные сохраняются в зашифрованном виде.<br>"
                   "Присутствует возможность добавления и удаления вкладок с данными (максимум 30 вкладок).<br>"
                   "В качестве фона каждой отдельной вкладки может использоваться любое изображение формата .PNG<br>"
                   "Для смены картинки на фоне, перетащите желаемое изображение мышкой на пространство окна<br>"
                   "приложения. Также в верхнем угловом виджете доступен поиск по вкладкам по имени ресурса.<br>"
                   "Если искомый ресурс найден, то приложение выполняет переход на вкладку с данным ресурсом <br>"
                   "и выделяет его название. Также возможно сохранение видимости паролей. <br>"
                   "Пароли можно генерировать автоматически. По умолчанию, такие пароли содержат символы,<br>"
                   "цифры, спец. символы. Длина генерируемых паролей для ресурсов - 10 символов.<br>"
                   "Длина генерируемого мастер-пароля приложения - 15 символов.<br>"
                   "Также присутствует возможность проверки мастер-пароля в базе данных утекших в сеть паролей <br>"
                   "авторитетного сервиса <a href='https://haveibeenpwned.com/'>Have I Been Pwned</a><br>"
                   "При вводе нового мастер-пароля автоматически определяется уровень его надёжности.<br>"
                   "Присутствует возможность привязать адрес электронной почты к приложению, чтобы при запуске<br>"
                   "приложения получать на привязанную почту одноразовый код подтверждения, позволяющий добавить<br>"
                   "ещё один уровень защиты приложения. Данная функция также позволяет мгновенно получить информацию<br>"
                   "о запуске приложения без вашего ведома (в сообщении содержится информация о дате и времени запуска,<br>"
                   "а также об операционной системе, в которой производится запуск приложения.<br>"
                   "Максимальное количество попыток входа в приложение - 3 (ввод пароля и/или кода подтверждения).<br>"
                   "При превышении кол-ва попыток приложение блокируется. Для разблокировки приложения <br>"
                   "обратитесь к разработчику."
                   " <br>                                                "
                   " <br>                                                "
                   " <br>                                                 "
                   "Контакты для связи с разработчиком приложения: <br>"
                   "e-mail:   ilya-filatov-1994@mail.ru             "));
    //QGridLayout* layout = static_cast<QGridLayout*>(msg.layout());
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

    saveAct = new QAction(tr("&Сохранить всё"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить все данные приложения"));
    connect(saveAct, &QAction::triggered, this, &Client_window::save);
    connect(saveAct, &QAction::triggered, this, &Client_window::writeSettings);

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
    connect(adding_tabs, &QAction::triggered, this, &Client_window::add_my_tab);

    delete_tabs = new QAction(tr("&Удалить последнюю вкладку"), this);
    delete_tabs->setShortcut(tr("Ctrl+D"));
    delete_tabs->setStatusTip(tr("Удаление последней по счёту вкладки"));
    connect(delete_tabs, &QAction::triggered, this, &Client_window::delete_my_tab);

    aboutAct = new QAction(tr("&О программе"), this);
    aboutAct->setShortcut(tr("Ctrl+O"));
    aboutAct->setStatusTip(tr("Показать информацию о приложении"));
    connect(aboutAct, &QAction::triggered, this, &Client_window::about);

}

void Client_window::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Файл")); //функция menuBar() создаёт и возвращает пустую строку меню
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Настройки"));
    editMenu->addAction(redoAct);
    editMenu->addAction(adding_tabs);
    editMenu->addAction(delete_tabs);

    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpMenu->addAction(aboutAct);

}

void Client_window::write_all_data()        //Сохранение всех данных (количество вкладок, видимость паролей, значение главного пароля, данные вкладок
{
    settings->setValue("number_tabs", tabs_counter);     //Сохранение в реестре количества открытых вкладок
    for (int i=0; i<=tabs_counter; i++)
    {
        tab_widget[i]->write_regedit(settings, tab_widget[i]->counter1,tab_widget[i]->counter2, i);
    }

    file1->open(QIODevice::WriteOnly | QIODevice::Append);      //Открытие файла на запись или дозапись
    file1->reset();
    QDataStream* stream = new QDataStream(file1);
    stream->setVersion(QDataStream::Qt_4_9);

    setting_window->write_global_setting(stream);                   //Запись главного пароля приложения

    for (int i=0; i<=tabs_counter; i++)
    {
        tab_widget[i]->write_data_tab(stream);                  //Запись данных остальных вкладок
    }
    file1->close();                                            //Закрытие файла
    delete stream;
}

void Client_window::writeSettings()         //Сохранение только значения главного пароля и данных вкладок
{
    file1->remove();
    file1->open(QIODevice::WriteOnly | QIODevice::Append);      //Открытие файла на запись или дозапись
    file1->reset();
    QDataStream* stream = new QDataStream(file1);
    stream->setVersion(QDataStream::Qt_4_9);

    setting_window->write_global_setting(stream);                   //Запись главного пароля приложения

    for (int i=0; i<=tabs_counter; i++)
    {
        tab_widget[i]->write_data_tab(stream);                  //Запись данных остальных вкладок
    }
    counter_records++;
    file1->close();                                            //Закрытие файла
    delete stream;
}

void Client_window::show_glob_pas()
{
    if (file1->exists())
        {
            file1->open(QIODevice::ReadOnly);
            QDataStream* stream = new QDataStream(file1);
            stream->setVersion(QDataStream::Qt_4_9);
            setting_window->read_global_setting(stream);
            file1->close();
            delete stream;
        }
    else
        {
            setting_window->otherwise_global_setting();       //Если файл не существует, ставится пароль по умолчанию
        }
}

void Client_window::readSettings()
{
    search_line->setPlaceholderText(tr("Поиск по имени ресурса"));
    tabs_counter=settings->value("number_tabs").toInt();     //Чтение из реестра количества сохранённых вкладок
    for (int i=0; i<=tabs_counter; i++)
        {
        if (i>0)
            {
                tab_widget[i] = new Tab_template;
                MyGlobalWindow->addTab(tab_widget[i], QString("Вкладка %1").arg(QString::number(i+1)));
            }
        tab_widget[i]->read_regedit(settings, i);
        connect(tab_widget[i], &Tab_template::save_page, this, &Client_window::writeSettings);
        }

    if (file1->exists())                                            //Если файл существует в данной директории, то открываем на чтение
        {                         
         file1->open(QIODevice::ReadOnly);
           QDataStream* stream = new QDataStream(file1);
           stream->setVersion(QDataStream::Qt_4_9);

           setting_window->read_global_setting(stream);         //Чтение главного пароля приложения

           for (int i=0; i<=tabs_counter; i++)
               {
                tab_widget[i]->read_data_tab(stream);      //Данные вкладок
               }

           file1->close();                                  //Закрытие файла
           delete stream;
        }
    else
        {
        setting_window->otherwise_global_setting();       //Если файл не существует, ставится пароль по умолчанию

        for (int i=0; i<=tabs_counter; i++)
            {
             tab_widget[i]->otherwise_data_tab();      //Данные вкладок
            }
        }
}

//Проверка на сохранение перед выходом из приложения
void Client_window::closeEvent(QCloseEvent* event)
{
    //Указываем переменную, которая будет хранить результат ответа
    QMessageBox* close_message = new QMessageBox(QMessageBox::Information,
                                    tr("Сообщение"),
                                    tr("Сохранить настройки приложения перед закрытием?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (close_message->exec()==QMessageBox::Yes)
         {write_all_data();
          delete close_message;
          event->accept();}
    else {//event->ignore();
          if (counter_records>0)
            {write_all_data();}
             delete close_message;
             event->accept();}
}

void Client_window::write_all()
{
    counter_records++;
}

//поиск по вкладкам по имени ресурса
void Client_window::search_tabs()
{
    const int sizeSearchArray=max_tabs*2;       //на каждой вкладке 2 ресурса
    QString search_string[sizeSearchArray];     //массив с именами всех ресурсов
    QString required_data=search_line->text();  //искомый ресурс
    bool value_found=false;                     //маркер результата поиска
    for (int i=0; i<(tabs_counter+1); i++)
    {
        search_string[2*i]=tab_widget[i]->get_name_resource1();
        search_string[2*i+1]=tab_widget[i]->get_name_resource2();
    }
    for (int i=0; i<(tabs_counter*2+2); i++)
    {
        if (search_string[i].contains(required_data, Qt::CaseInsensitive) and i %2 == 0)
        {
            MyGlobalWindow->setCurrentIndex(i/2);
            tab_widget[i/2]->select_resource1();
            value_found=true;
            break;
        }
        if (search_string[i].contains(required_data, Qt::CaseInsensitive) and i %2 != 0)
        {
            MyGlobalWindow->setCurrentIndex(i/2);
            tab_widget[i/2]->select_resource2();
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
                       connect(key_return, SIGNAL(activated()), this, SLOT(search_tabs()));
                       return false;
                    }
        else if (event->type() == QEvent::FocusOut)
                   {
                       disconnect(key_return, SIGNAL(activated()), this, SLOT(search_tabs()));
                       return false;
                   }
        else return false;
    }
    else return QObject::eventFilter(obj, event);
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
   if ((info_file.suffix())!="png")
   {
       QMessageBox* warning_msg = new QMessageBox(QMessageBox::Warning,
                                       tr("Предупреждение"),
                                       tr("Используйте для изменения фона изоражения с расширением .PNG"));
       warning_msg->setWindowFlags(Qt::WindowStaysOnTopHint);  //поверх окон
       if (warning_msg->exec()==QMessageBox::Ok)
       {delete warning_msg;}
   }
   else //Если файл формата .png то устанавливаем фон
   {
        QPixmap image(filePath);
        QPalette palette;
        palette.setBrush(tab_widget[MyGlobalWindow->currentIndex()]->backgroundRole(),QBrush(QPixmap(filePath)));
        tab_widget[MyGlobalWindow->currentIndex()]->setPalette(palette);              //Устанавливаем в виджете объект палитры
        tab_widget[MyGlobalWindow->currentIndex()]->setAutoFillBackground(true);      //Переводим свойство заполнения фона в true

        //Копирование файла и сохранение пути к нему
        QFileInfo info(filePath);                   //полная инфа о файле
        QString fileName=info.fileName();           //имя копируемого файла
        QString copyPath=QDir::currentPath();       //Текущая директория приложения куда будет копироваться файл
        copyPath+="/"+fileName;                     //абсолютный путь куда копируется файл
        if (!QFile::exists(copyPath))               //если такого файла нет в директории куда копируем,
            QFile::copy(filePath, copyPath);        //выполняем копирование

   //Индекс текущей вкладки
   int CurIndex=MyGlobalWindow->currentIndex();
   bool counter_repeating=false;            //Флаг повтора индекса
   //Запись индекса и пути
   if (file_for_paths->exists())            //"Файл уже существует, читаем все индексы и пути \r\n";
   {
       int index_array[max_tabs];
       QVector<QString> vector_string;
       int index_vector=0;
       QString tempPath;
       //Открытие файла на чтение
       file_for_paths->open(QIODevice::ReadOnly);
       QDataStream* stream1 = new QDataStream(file_for_paths);
       stream1->setVersion(QDataStream::Qt_4_9);
       counter_change_palette=settings->value("numberChangeBack").toInt();
       for (int i=0; i<counter_change_palette; i++)
       {
           *stream1 >> index_array[i];
           *stream1 >> tempPath;
           vector_string+=tempPath;
           vector_string[i].replace("\r\n", "");
           if (index_array[i]==CurIndex)
           {
               counter_repeating=true;
               index_vector=i;
               //"Повторяющийся элемент! " << CurIndex << "\r\n";
           }
       }
       file_for_paths->close();
       delete stream1;
       //Если текущий индекс совпадает с уже записанным ранее, переписываем путь этого индекса, пути других индексов пишем как обычно
       if (counter_repeating)
       {
           //qDebug() << "Переписываем путь в векторе \r\n";
           vector_string[index_vector].replace(tempPath, copyPath);
           tempPath.replace("\r\n", "");                    //чтобы получить валидный путь, удаляем лишние символы
           QDir dir;
           dir.remove(tempPath);                            //Удаляем файл который раньше использовался как фон из текущей директории
           //qDebug() << copyPath << "\r\n";
           file_for_paths->remove();
           file_for_paths->open(QIODevice::WriteOnly | QIODevice::Append);
           QDataStream* stream2 = new QDataStream(file_for_paths);
           stream2->setVersion(QDataStream::Qt_4_9);
           //qDebug() << "переписываем актуальные пути в файле \r\n";
           for (int i=0; i<counter_change_palette; i++)
           {
               if (index_array[i]==CurIndex)
               {
                   *stream2 << CurIndex;
                   *stream2 << ("\r\n"+copyPath+"\r\n");
               }
               else
               {
                   *stream2 << index_array[i];
                   *stream2 << ("\r\n"+vector_string.at(i)+"\r\n");
               }
           }
           file_for_paths->close();
           delete stream2;
           settings->setValue("numberChangeBack", counter_change_palette);
           //qDebug() << "Пути переписаны";
       }
   }
   //Если файла не существовало, Либо индекс не повторяется - обычная запись
  if (!file_for_paths->exists() or !counter_repeating)
  {
       file_for_paths->open(QIODevice::WriteOnly | QIODevice::Append);
       QDataStream* stream3 = new QDataStream(file_for_paths);
       stream3->setVersion(QDataStream::Qt_4_9);
       *stream3 << CurIndex;
       *stream3 << ("\r\n"+copyPath+"\r\n");   //Запись пути в файл
       file_for_paths->close();
       delete stream3;
       counter_change_palette++;
       settings->setValue("numberChangeBack", counter_change_palette);
   }
   }
   //1) читаем все индексы и пути
   //2) ищем индекс, совпадающий с currentIndex (на текущей вкладке уже меняли фон)
   //3) по индексу, совпадающему с currentIndex переписываем значение пути (новая картинка будет использоваться)
   //4) сбрасываем записи файла и записываем ранее прочтённые индексы и актуальные пути к ним. counter_change_palette НЕинкрементируем, пишем в реестр
   //5) если нет индекса, совпадающего с currentIndex, то НЕ выполняем пункт 3 и 4. Просто пишем currentIndex и путь в файл. Инкрементируем counter_change_palette и пишем в реестр
}

void Client_window::set_backgound_image()
{
    //Установка стандартного фона
    for (int i=0; i<=tabs_counter; i++)
    {
        QPalette pal;                               //объект палитры
        pal.setBrush(tab_widget[i]->backgroundRole(),QBrush(QPixmap(":/img/image_forest.png"))); //Устанавливаем в фон изображение из ресурсов
        tab_widget[i]->setPalette(pal);                                                            //Устанавливаем в виджете объект палитры
        tab_widget[i]->setAutoFillBackground(true);
    }
    //Если фон был изменён
    counter_change_palette=settings->value("numberChangeBack").toInt();
    if (file_for_paths->exists())
    {
     int index[max_tabs];
     QVector<QString> vector_string;
     QString path;
      //Чтение сохранённых настроек
      file_for_paths->open(QIODevice::ReadOnly);
      QDataStream* stream = new QDataStream(file_for_paths);
      stream->setVersion(QDataStream::Qt_4_9);
      for (int i=0; i<counter_change_palette; i++)
      {
        *stream >> index[i];
        *stream >> path;
        vector_string+=path;
        vector_string[i].replace("\r\n", "");
      }
       file_for_paths->close();
       //установка фона из сохранённых путей
       for (int i=0; i<counter_change_palette; i++)
       {
           if (counter_change_palette<=max_tabs)
          {
            path.clear();
            path=vector_string.at(i);
            QPalette pal;                                                                    //Создаём объект палитры
            pal.setBrush(tab_widget[index[i]]->backgroundRole(),QBrush(QPixmap(path)));      //Устанавливаем в фон изображение по указанному пути
            tab_widget[index[i]]->setPalette(pal);                                           //Устанавливаем в виджете объект палитры
            tab_widget[index[i]]->setAutoFillBackground(true);                               //переводим свойство заполнения фона в true
           }
        }
    }
    else
        settings->remove("numberChangeBack");
}
