#include "mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    counter_attempts_autoriz=0;                             
    var_timer_of_attempts=max_time_delay;                   
    authorization_widget1 = new QWidget(this);              
    authorization_widget1->setObjectName("check_window");   
    window_app = new Client_window();                      
    no_internet_connection=false;

    line_enter_pas = new QLineEdit(this);
    line_enter_pas->setEchoMode(QLineEdit::Password);
    connect(line_enter_pas, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   

    visiblePassw = new QPushButton(this);
    visiblePassw->setIcon(QIcon(":/img/closed_lock.png"));
    visiblePassw->setIconSize(QSize(26,26));
    visiblePassw->setFixedHeight(26);
    visiblePassw->setFixedWidth(26);
    visiblePassw->setToolTip(tr("Изменение видимости пароля"));
    connect(visiblePassw, &QPushButton::clicked, this, &MainWindow::changeVisiblePas);

    horizontal0 = new QHBoxLayout();
    horizontal0->setSpacing(5);
    horizontal0->addWidget(line_enter_pas);
    horizontal0->addWidget(visiblePassw);

    label_state_capslock = new QLabel(tr("                        "), this);
    key_capslock = new QShortcut(this);
    key_capslock->setKey(Qt::Key_CapsLock);
    connect(key_capslock, SIGNAL(activated()), this, SLOT(push_capslock()));
    QFont font("Times", 8, QFont:: Bold);
    label_state_capslock->setFont(font);

    label_email_confrmation = new QLabel(this);                             
    line_enter_code = new QLineEdit(this);                                 
    connect(line_enter_code, SIGNAL(textChanged(const QString&)), this, SLOT(slot_check_size_line()));   
    timer1 = new QTimer(this);                                          
    connect(timer1, SIGNAL(timeout()), this, SLOT(slotTimeout()));     

    countdown_timer = new QLabel(tr("                        "), this);
    countdown_timer->setMargin(6);
    resending_message = new QPushButton(tr("Отправить код"), this);
    resending_message->setFixedWidth(195);
    connect(resending_message, SIGNAL(clicked()), this, SLOT(slot_restart_Timer()));
    resending_message->setVisible(false);             

    ok = new QPushButton(tr("Ok"), this);
    connect(ok, SIGNAL(clicked()), this, SLOT(slot_requestReadData()));
    key_enter = new QShortcut(this);
    key_enter->setKey(Qt::Key_Return);
    connect(key_enter, SIGNAL(activated()), this, SLOT(slot_requestReadData()));

    cancel = new QPushButton(tr("Cancel"), this);
    connect(cancel, SIGNAL(clicked()), this, SLOT(close_window()));

    groupBox1 = new QGroupBox(this);
    groupBox1->setTitle(tr(" Введите пароль:"));        
    //Компоновка виджетов ввода мастер-пароля
    vertical0 = new QVBoxLayout();    
    vertical0->setMargin(15);                       
    vertical0->setSpacing(5);            
    vertical0->addStretch(1);
    vertical0->addLayout(horizontal0);
    vertical0->addWidget(label_state_capslock);
    vertical0->setAlignment(label_state_capslock, Qt::AlignCenter);
    groupBox1->setLayout(vertical0);    

    //компоновка виджетов ввода одноразового кода
    groupBox2 = new QGroupBox(this);
    groupBox2->setTitle(tr(" Введите код подтверждения:"));   
    vertical01 = new QVBoxLayout;          
    vertical01->setMargin(15);                          
    vertical01->setSpacing(10);                      
    vertical01->addStretch(1);
    vertical01->addWidget(label_email_confrmation);
    vertical01->addWidget(line_enter_code);
    vertical01->addWidget(countdown_timer);
    countdown_timer->setAlignment(Qt::AlignCenter);
    vertical01->addStretch(5);
    vertical01->addWidget(resending_message);
    vertical01->setAlignment(resending_message, Qt::AlignCenter);
    groupBox2->setLayout(vertical01);        
    groupBox2->setVisible(false);            

    //Компоновка кнопок
    horizontal1 = new QHBoxLayout();
    horizontal1->setMargin(5);    
    horizontal1->setSpacing(10);  
    horizontal1->addStretch(1);
    horizontal1->addWidget(ok);
    horizontal1->addWidget(cancel);
    horizontal1->addStretch(1);

    //Размещение компоновок на на виджете окна
    vertical02 = new QVBoxLayout();        
    vertical02->setMargin(5);                
    vertical02->setSpacing(5);             
    vertical02->addStretch(1);
    vertical02->addWidget(groupBox1);
    vertical02->addWidget(groupBox2);
    vertical02->addLayout(horizontal1);
    vertical02->addStretch(1);

    setting = new QSettings("My_Corp", "Spreadsheet");
    db = QSqlDatabase::addDatabase("QSQLITE");

    network_connection = new Network_connection();
    thread = new QThread(this);
    thread->start();
    network_connection->moveToThread(thread);
    connect(thread, &QThread::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    //Запуск проверки интернет-соединения
    connect(this, &MainWindow::call_network_action, network_connection, &Network_connection::check_http_connection, Qt::QueuedConnection);

    //Результат проверки интернет соединения
    connect(network_connection, &Network_connection::failure_internet_connection, this, &MainWindow::result_connection, Qt::QueuedConnection);

    connect(network_connection, &Network_connection::signal_smtp_data, this, &MainWindow::getDataForSmtp, Qt::QueuedConnection);
    connect(this, &MainWindow::send_data_smtp, network_connection, &Network_connection::getDataSmtp, Qt::QueuedConnection);

    createProgressBar();

    setWindowTitle(tr("Password Manager"));
    authorization_widget1->setLayout(vertical02);
    setCentralWidget(authorization_widget1);
    setMinimumSize(500, 180);
    setMaximumSize(947, 509);
    resize(500, 180);
}

MainWindow::~MainWindow()
{
    delete setting;
    delete window_app;

    thread->quit();
    thread->wait();
    thread->deleteLater();
    network_connection->deleteLater();
    delete network_connection;
    delete thread;

    delete horizontal0;
    delete vertical0;
    delete groupBox1;
    delete vertical01;
    delete groupBox2;
    delete horizontal1;
    delete vertical02;
    delete authorization_widget1;
}

void MainWindow::createProgressBar()
{
    wait_widget.setAlignment(Qt::AlignCenter);
    wait_widget.setWindowFlags(Qt::WindowStaysOnTopHint);          //поверх окон
    wait_widget.setWindowModality(Qt::ApplicationModal);           //модальность
    wait_widget.setWindowFlags(Qt::FramelessWindowHint);           //отключаем обрамление окна виджета
    wait_widget.setAttribute(Qt::WA_TranslucentBackground, true);  //Прозрачность
    wait_widget.resize(150, 150);
    movie = new QMovie(&wait_widget);
    movie->setFileName(":/img/loading.gif");
    movie->setScaledSize(wait_widget.size());
    wait_widget.setMovie(movie);
}

void MainWindow::showProgressBar(bool visible)
{
    if (visible) {
        wait_widget.show();
        movie->start();
    }
    if (!visible) {
        movie->stop();
        wait_widget.setVisible(false);
    }
}

//Метод для вывода 3-ёх разных типов диалоговых сообщений
void MainWindow::dialog_message(QMessageBox::Icon icon, QString str1, QString str2)
{
    QMessageBox* f_msg = new QMessageBox(icon,str1,str2);
    if (f_msg->exec()==QMessageBox::Ok)
    {delete f_msg;}
}

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

void MainWindow:: push_capslock()
{
    state_capslock();
}

void MainWindow::slot_requestReadData()
{
    requestReadData();
}

void MainWindow::requestReadData()
{
    showProgressBar(true);
    if (setting->value("status").toInt()!=255)
    {
        if (checkUSBDrive())
        {
            disconnect(ok, SIGNAL(clicked()), this, SLOT(slot_requestReadData()));
            disconnect(key_enter, SIGNAL(activated()), this, SLOT(slot_requestReadData()));
            connect(ok, SIGNAL(clicked()), this, SLOT(check_password()));
            connect(key_enter, SIGNAL(activated()), this, SLOT(check_password()));
            if(db.open())
            {
                check_blocked();
            }
        }
        else
        {
            counter_attempts_autoriz++;
            if (counter_attempts_autoriz>=max_quantity_attempts)
            {
                dialog_message(QMessageBox::Critical, tr("Ошибка"), tr("Исчерпано кол-во попыток авторизации <br> приложение заблокировано")); 
                setting->setValue("status", 255);
                QByteArray status = "255";
                if (db.open())
                {
                    QSqlQuery query;
                    writeDataSQL_settingTable(db, query, 3, "app_lock_status", status);  
                }
                this->close();
                exit(1);
            }
            else
            {
                showProgressBar(false);
                dialog_message(QMessageBox::Critical, tr("Ошибка!"), (tr("Вставьте USB-ключ! <br> Осталось ")+QString::number(max_quantity_attempts-counter_attempts_autoriz)+tr(" попыток авторизации")));
            }
        }
    }
    else {
        dialog_message(QMessageBox::Critical, tr("Приложение заблокировано"), tr("Исчерпано количество попыток входа"));   
        this->close();
        exit(1);
    }
}

void MainWindow::check_blocked()
{
    int blocked_app=255;
    if(db.open())
    {
        QSqlQuery query;
        QString str = QString::fromLocal8Bit(readDataSQL_settingTable(db, query, "app_lock_status"));
        blocked_app = str.toInt();
        if (blocked_app==255 or setting->value("status").toInt()==255)
        {
            dialog_message(QMessageBox::Critical, tr("Приложение заблокировано"), tr("Исчерпано количество попыток входа"));   
            this->close();
            exit(1);
        }
        else {
            read_main_settings();	
            if (!line_enter_pas->text().isEmpty())
                check_password();
        }
    }
}

void MainWindow::read_main_settings()
{
    state_capslock();
    if(db.open())
    {
        QSqlQuery query;
        QByteArray key, iv, text;
        key = readDataSQL_settingTable(db, query, "main_key");
        iv = readDataSQL_settingTable(db, query, "main_vector");
        text = readDataSQL_settingTable(db, query, "master_pas");
        output_pas = decryptAES(text, key, iv);

        mail_autoriz = QString::fromLocal8Bit(readDataSQL_settingTable(db, query, "availability_email"));
        check_mail = mail_autoriz.toInt();

        key = readDataSQL_settingTable(db, query, "key_mail");
        iv = readDataSQL_settingTable(db, query, "vector_mail");
        text = readDataSQL_settingTable(db, query, "address_email");
        mail_autoriz = decryptAES(text, key, iv);
        showProgressBar(false);
    }
    else
    {
        check_mail=0;
        output_pas="";
        dialog_message(QMessageBox::Warning, tr("Ошибка!"), (tr("Вставьте USB-ключ!")));
    }
    if (check_mail==1)
    {
        QString text_request_mail;
        text_request_mail.clear();
        text_request_mail=tr("Вам был отправлен код подтверждения на электронную почту:");
        text_request_mail+=tr("\r\n");
        text_request_mail+=mail_autoriz;
        label_email_confrmation->setText(text_request_mail);
        groupBox2->setVisible(true);
        limiting_attempts_resend();
        this->resize(500, 330);
    }
    else
    {
        groupBox2->setVisible(false);
        this->resize(500, 180);
    }
}

void MainWindow::slot_restart_Timer()
{
    Sleep(1500);    //ожидание результата проверки интернет-соединения
    if (no_internet_connection==false)
    {
        counter_attempts_autoriz++;
        dialog_message(QMessageBox::Warning, tr("Предупреждение"), (tr("Осталось ")+QString::number(max_quantity_attempts-counter_attempts_autoriz)+tr(" попыток авторизации")));
    }
    no_internet_connection=false;
    limiting_attempts_resend();
}

void MainWindow::limiting_attempts_resend()
{
    if (counter_attempts_autoriz>=max_quantity_attempts)
    {
        dialog_message(QMessageBox::Critical, tr("Ошибка"), tr("Исчерпано кол-во попыток авторизации. <br> Приложение заблокировано"));    
        setting->setValue("status", 255);
        QByteArray status = "255";
        if (db.open())
        {
            QSqlQuery query;
            writeDataSQL_settingTable(db, query, 3, "app_lock_status", status);
        }
        this->close();
        exit(1);
    }
    else
    {
        timer1->start(1000);                                                           
        resending_message->setVisible(false);                                          
        countdown_timer->setText(tr("Отправить код повторно можно через: 01:30"));
        countdown_timer->setVisible(true);                                              
        generating_code_message();
        emit call_network_action("verification", mail_autoriz, code_mes);
    }
}

void MainWindow::generating_code_message()
{
    int gen_elem[6];
    code_mes.clear();
    for (int i=0; i<6; i++)
    {
        std::uniform_int_distribution<int>distribution(0,9);
        gen_elem[i] = distribution(*QRandomGenerator::global());
        code_mes+=QString::number(gen_elem[i]);
    }
}

void MainWindow::slotTimeout()
{
    if (var_timer_of_attempts>0)
    {
        current_time.clear();
        var_timer_of_attempts--;
        minutes=var_timer_of_attempts/60;
        secondes=var_timer_of_attempts-minutes*60;
        current_time=tr("Отправить код повторно можно через: ");
        if (minutes<10) {current_time+="0";}
        current_time+=QString::number(minutes)+tr(":");
        if (secondes<10) {current_time+="0";}
        current_time+=QString::number(secondes);
        countdown_timer->setText(current_time);
        countdown_timer->setVisible(true);        
    }
    else
    {
        timer1->stop();
        var_timer_of_attempts=max_time_delay;      
        countdown_timer->setVisible(false);     
        resending_message->setVisible(true);   
    }
}

void MainWindow::check_password()
{
    if (!output_pas.isEmpty()){
    bool verification_pas=false;              
    bool verification_code_from_message=false;     
    if (output_pas == line_enter_pas->text())     
    {
        verification_pas=true;
    }

    if (check_mail!=1)
    {
        verification_code_from_message=true;           
    }

    if (check_mail==1 && code_mes == line_enter_code->text()) 
    {
        verification_code_from_message=true;
    }

    if (verification_pas==true && verification_code_from_message==true)
    {
        wait_widget.setVisible(true);
        movie->start();
        if (movie->state()==QMovie::Running) {
            window_app->readSettings();
        }
        showProgressBar(false);
        window_app->show();
        this->close();
    }
    else
    {
        counter_attempts_autoriz++;   //Счётчик кол-ва попыток
        if (counter_attempts_autoriz>=max_quantity_attempts)
        {
            dialog_message(QMessageBox::Critical, tr("Ошибка"), tr("Исчерпано кол-во попыток авторизации <br> приложение заблокировано"));   
            setting->setValue("status", 255);
            QByteArray status = "255";
            if (db.open())
            {
                QSqlQuery query;
                writeDataSQL_settingTable(db, query, 3, "app_lock_status", status);
            }
            this->close();
            exit(1);
        }
        else
        {
            dialog_message(QMessageBox::Warning, tr("Предупреждение"), (tr("Данные введены неверно. <br> Осталось ")+QString::number(max_quantity_attempts-counter_attempts_autoriz)+tr(" попыток авторизации")));
        }
    }
    }
}

void MainWindow::changeVisiblePas()
{
    if (QLineEdit::Password == line_enter_pas->echoMode())
    {
        visiblePassw->setIcon(QIcon(":/img/open_lock.png"));
        visiblePassw->setIconSize(QSize(26,26));
        line_enter_pas->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        visiblePassw->setIcon(QIcon(":/img/closed_lock.png"));
        visiblePassw->setIconSize(QSize(26,26));
        line_enter_pas->setEchoMode(QLineEdit::Password);
    }
}

void MainWindow::slot_check_size_line()
{
    QString str=tr("");
    if ((line_enter_pas->hasFocus())==true)
    {
        str=line_enter_pas->text();
        //Ячейки SQL таблицы ограничены 300 символами
        //(кодируется unicode - по 2 байта каждый - исключаем специфические вроде смайлов и иероглифов)
        //В алгоритме AES-256 режим CBC длина блока 16 байт
        //Ограничимся 18-ю блоками - 288 символов
        str.truncate(288);
        line_enter_pas->setText(str);
    }
    if ((line_enter_code->hasFocus())==true)
    {
        str=line_enter_code->text();
        str.truncate(288);
        line_enter_code->setText(str);
    }
}

void MainWindow::result_connection(QString result)
{
    timer1->stop();
    var_timer_of_attempts=max_time_delay;      
    countdown_timer->setVisible(false);         
    resending_message->setVisible(true);    
    counter_attempts_autoriz++;
    no_internet_connection=true;

    if (counter_attempts_autoriz>=max_quantity_attempts)
    {
        dialog_message(QMessageBox::Critical, tr("Ошибка"), tr("Исчерпано кол-во попыток авторизации <br> приложение заблокировано"));
        setting->setValue("status", 255);
        QByteArray status = "255";
        if (db.open())
        {
            QSqlQuery query;
            writeDataSQL_settingTable(db, query, 3, "app_lock_status", status);
        }
        this->close();
        exit(1);
    }
    else
    {
        dialog_message(QMessageBox::Critical, tr("Ошибка"), (result+tr("Осталось ")+QString::number(max_quantity_attempts-counter_attempts_autoriz)+tr(" попыток авторизации")));
    }
}

void MainWindow::getDataForSmtp()
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
}

void MainWindow::close_window()
{
    this->close();
}

bool MainWindow::GetSerialNumberUSB(LPCWSTR sDriveName, std::string &strSerialNumber) {
    // handle to physical drive
    HANDLE hVolume = CreateFileW(
        sDriveName,                            // lpFileName
        GENERIC_READ,                          // dwDesiredAccess
        FILE_SHARE_READ,   //|| FILE_SHARE_WRITE    // dwShareMode
        nullptr,                                  // lpSecurityAttributes
        OPEN_EXISTING,                         // dwCreationDisposition
        FILE_ATTRIBUTE_NORMAL,                 // dwFlagsAndAttributes
        nullptr                                   // hTemplateFile
    );
    if (hVolume == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    STORAGE_PROPERTY_QUERY PropertyQuery;
    ZeroMemory(&PropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
    PropertyQuery.PropertyId = StorageDeviceProperty;
    PropertyQuery.QueryType = PropertyStandardQuery;
    // first call to DeviceIocontrol to get the size of the output
    STORAGE_DEVICE_DESCRIPTOR  DeviceDescriptor = { };
    DWORD nBytesDeviceDescriptor = 0;
    if ( !DeviceIoControl(
        hVolume,                              // hDevice
        IOCTL_STORAGE_QUERY_PROPERTY,         // dwIoControlCode
        &PropertyQuery,                       // lpInBuffer
        sizeof(STORAGE_PROPERTY_QUERY),       // nInBufferSize
        &DeviceDescriptor,                    // lpOutBuffer
        sizeof(STORAGE_DESCRIPTOR_HEADER),    // nOutBufferSize
        &nBytesDeviceDescriptor,              // lpBytesReturned
        nullptr                               // lpOverlapped
    )) {
        CloseHandle(hVolume);
        return FALSE;
    }
    // allocate the output
    const DWORD dwOutBufferSize = DeviceDescriptor.Size;
    char* pOutBuffer = new char[dwOutBufferSize];
    ZeroMemory(pOutBuffer, dwOutBufferSize);
    STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(pOutBuffer);
    if (!DeviceIoControl(
        hVolume,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &PropertyQuery,
        sizeof(PropertyQuery),
        pDeviceDescriptor,
        dwOutBufferSize,
        &nBytesDeviceDescriptor,
        nullptr
    )) {
        delete[] pOutBuffer;
        CloseHandle(hVolume);
        return FALSE;
    }
    const DWORD nSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
    if (nSerialNumberOffset == 0) {
        delete[] pOutBuffer;
        CloseHandle(hVolume);
        return FALSE;
    }
    strSerialNumber = static_cast<std::string>(pOutBuffer + nSerialNumberOffset);
    delete[] pOutBuffer;
    CloseHandle(hVolume);
    return (strSerialNumber.empty()) ? FALSE : TRUE;
}

void MainWindow::searhPathFile(QDir dir, QStringList& files)
{
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    QStringList listFiles = dir.entryList((tr("*.db")).split(" "), QDir::Files);
    if (!listFiles.isEmpty())
        files+=listFiles;

    QStringList listDir = dir.entryList(QDir::Dirs);
    foreach (QString subdir, listDir)
    {
        if (subdir == "." || subdir == "..")
        {
            continue;
        }
        searhPathFile(QDir(dir.absoluteFilePath(subdir)), files);
    }
}

bool MainWindow::checkUSBDrive()
{
    WCHAR myDrives[105];
    WCHAR volumeName[MAX_PATH];
    WCHAR fileSystemName[MAX_PATH];
    DWORD serialNumber, maxComponentLen, fileSystemFlags;
    UINT driveType;
    QString nameDisk;
    QString serialNumberString;
    QString rootPath;
    QString readSerialNumber;
    QString readNameDevice;
    if (GetLogicalDriveStringsW(ARRAYSIZE(myDrives)-1, myDrives))
    {
        for (LPWSTR drive = myDrives; *drive != 0; drive += 4)
        {
            driveType = GetDriveTypeW(drive);
            if (driveType==2)
            {
                QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
                nameDisk = "\\\\.\\";
                nameDisk += QString::fromWCharArray(drive, ARRAYSIZE(drive)-1);
                nameDisk +=":";
                std::string s;
                std::wstring mWstring = nameDisk.toStdWString();
                GetSerialNumberUSB(mWstring.c_str(), s);
                serialNumberString=QString::fromStdString(s);

                if (GetVolumeInformationW(drive, volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
                {
                    nameDisk=QString::fromWCharArray(volumeName, 15);
                }
                rootPath=QString::fromWCharArray(drive, ARRAYSIZE(drive)-1)+":/";
                listFiles.clear();
                searhPathFile(QDir(rootPath), listFiles);
                QApplication::processEvents(QEventLoop::AllEvents);
                foreach (QString file, listFiles)
                {
                    if (!file.isEmpty())    {
                        db.setDatabaseName(rootPath+file);
                        if(!db.open())
                        {
                            return false;
                        }
                        else
                        {
                            QSqlQuery query;
                            readNameDevice = QString::fromLocal8Bit(readDataSQL_settingTable(db, query, "name_device"));
                            readSerialNumber = QString::fromLocal8Bit(readDataSQL_settingTable(db, query, "serial_number"));
                        }
                        if (nameDisk==readNameDevice && serialNumberString==readSerialNumber)
                        {
                            return true;
                        }
                        else {
                            db.close();
                            db.removeDatabase(rootPath+file); }
                        }
                }
            }
            else {
                continue;
            }
        }
    }
    else {
        return false;
    }
    return false;
}
