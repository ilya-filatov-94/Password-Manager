#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "windows.h"
#include "datamanagement.h"
#include "network_connection.h"
#include "client_window.h"

#include <QString>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFont>
#include <QGroupBox>
#include <QBoxLayout>
#include <QShortcut>
#include <QTimer>
#include <QRandomGenerator64>
#include <QThread>
#include <QMovie>


class MainWindow : public QMainWindow, protected DataManagement
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
    void requestReadData();

private:

    QWidget* authorization_widget1;             //Виджет окна авторизации в приложении
    Network_connection* network_connection;     //Указатель на объект, работающий с http, dns запросами и с smtp-сервером
    QThread* thread;
    Client_window* window_app;

    QLineEdit* line_enter_pas;          //Поле ввода пароля
    QPushButton* visiblePassw;
    QLabel* label_state_capslock;       //Надпись состояния клавиши CapsLock
    QShortcut* key_capslock;            //Горячая клавиша CapsLock
    QLabel* label_email_confrmation;    //Надпись запроса ввода кода подтверждения из email
    QLineEdit* line_enter_code;         //Поле ввода кода подтверждения
    QLabel* countdown_timer;            //Отображение времени обратного отсчёта таймера
    QPushButton* resending_message;     //Кнопка повторной отправки сообщения
    QPushButton* ok;                    //кнопка запуска метода проверки пароля и кода для входа в приложение
    QShortcut* key_enter;               //Горячая клавиша enter
    QPushButton* cancel;                //отмена, закрытие приложения
    QTimer* timer1;                     //таймер обратного отсчёта
    QGroupBox *groupBox1;               //группирование виджетов ввода пароля
    QGroupBox *groupBox2;               //группирование виджетов ввода кода подтверждения

    QVBoxLayout* vertical0;
    QVBoxLayout* vertical01;
    QVBoxLayout* vertical02;
    QHBoxLayout* horizontal0;
    QHBoxLayout* horizontal1;

    int counter_attempts_autoriz;               //счётчик кол-ва попыток авторизации
    const int max_quantity_attempts=3;          //максимальное кол-во попыток авторизации
    const int max_time_delay=90;                //максимальное время задержки повторной отправки сообщения, сек
    int var_timer_of_attempts;                  //таймер попыток повторной отправки сообщения, сек
    QString current_time;                       //Текущее время обратного отсчёта
    int minutes;                                //минуты текущего времени обратн отсчёта
    int secondes;                               //секунды текущего времени обратн отсчёта
    int check_mail;                             //наличие привязки почты к приложению 1 - привязана, 0 - не привязана
    QString output_pas;
    QString mail_autoriz;
    QString code_mes;
    bool no_internet_connection;

    bool checkUSBDrive();
    bool GetSerialNumberUSB(LPCWSTR sDriveName, std::string &strSerialNumber);
    QStringList listFiles;
    void searhPathFile(QDir, QStringList&);
    QSettings* setting;
    QSqlDatabase db;

    //Виджет загрузки
    QLabel wait_widget;
    QMovie* movie;

    void createProgressBar();
    void showProgressBar(bool);
    void dialog_message(QMessageBox::Icon, QString, QString);
    void state_capslock();                            //чтение состояния клавиши capslock при запуске приложения
    void limiting_attempts_resend();                  //метод огр кол-ва попыток повторн. отправки сообщения
    void limiting_attempts_enter_app();               //метод огр кол-ва попыток входа в приложение
    void generating_code_message();                   //генерация одноразового кода проверки
    void check_blocked();                             //проверка заблокировано приложение или нет
    void read_main_settings();                        //Чтение основных настроек приложения


private slots:

    void slot_requestReadData();
    void push_capslock();       //повторное нажатие клавиши CapsLock
    void slotTimeout();         //слот вызываемый таймером по таймауту времени отсчёта (разблокирует повторную отправку сообщения)
    void slot_restart_Timer();  //Перезапуск таймера при нажатии на кнопку
    void check_password();      //метод проверки пароля и кода для входа в приложение
    void changeVisiblePas();    //видимость вводимого пароля
    void close_window();        //Закрытие приложения

    void slot_check_size_line();        //проверка длины вводимого текста
    void result_connection(QString);    //сообщение наличия/отсутствия соединения с интернетом
    void getDataForSmtp();

signals:

    void call_network_action(QString, QString, QString);
    void send_data_smtp(QString, QString);
};

#endif // MAINWINDOW_H

