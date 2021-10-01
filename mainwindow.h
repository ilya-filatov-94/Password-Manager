#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "windows.h"               //для определения состояния клавиши Caps Lock
#include "my_encryption.h"         //чтение/запись/шифрование данных
#include "network_connection.h"
#include "client_window.h"
#include "window_setting.h"

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QSettings>
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


class MainWindow : public QMainWindow, My_Encrytion
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
    void check_blocked();

private:

    int blocked_app;                            //переменная блокировки приложения
    bool start_read;                            //разрешает запустить публичн. метод проверки блокировки приложения 1 раз
    int counter_attempts_autoriz;               //счётчик кол-ва попыток авторизации
    const int max_quantity_attempts=3;          //максимальное кол-во попыток авторизации
    const int max_time_delay=90;                //максимальное время задержки повторной отправки сообщения, сек
    int var_timer_of_attempts;                  //таймер попыток повторной отправки сообщения, сек
    QString current_time;                       //Текущее время обратного отсчёта
    int minutes;                                //минуты текущего времени обратн отсчёта
    int secondes;                               //секунды текущего времени обратн отсчёта
    int check_mail;                             //наличие привязки почты к приложению 1 - привязана, 0 - не привязана
    QString input_pas;                          //Вводимый пользователем мастер-пароль
    QString output_pas;                         //Сохранённый в приложении мастер-пароль
    QString mail_autoriz;                       //адрес привязанной почты
    bool verification_pas;                      //результат проверки мастер-пароля
    bool verification_code_from_message;        //результат проверки кода из сообщения
    int gen_elem[6];                            //генерируемый массив чисел кода проверки
    QString code_mes;                           //одноразовый код проверки
    QString enter_code_mes;                     //Вводимый пользователем одноразовый код проверки
    bool no_internet_connection;                //Отсутствие соединенеия с интернетом

    QLineEdit* line_enter_pas;          //Поле ввода пароля
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
    QSettings* setting;                 //для чтения из реестра
    QGroupBox *groupBox1;               //группирование виджетов ввода пароля
    QGroupBox *groupBox2;               //группирование виджетов ввода кода подтверждения


    QFile* file1;                       //Файл в котором сохраняются данные
    QWidget* authorization_widget1;     //Виджет окна
    Network_connection* network_connection;
    QThread* thread;
    Client_window* window_app;              //Указатель на объект главного окна приложения

    void dialog_message(int, QString, QString);       //вызов диалогового сообщения: 1 - инфо, 2 - предупреждение, 3 - ошибка
    void state_capslock();                            //чтение состояния клавиши capslock при запуске приложения

    void limiting_attempts_resend();                  //метод огр кол-ва попыток повторн. отправки сообщения
    void limiting_attempts_enter_app();               //метод огр кол-ва попыток входа в приложение
    void generating_code_message();                   //генерация одноразового кода проверки

    /*Методы чтения сохранённых данных приложения*/
    void read_main_settings();                        //Чтение основных настроек приложения
    int read_settings(QDataStream*);                  //чтение настройки check_mail
    QString read_data(QDataStream*);                  //чтение Данных

private slots:

    void push_capslock();       //повторное нажатие клавиши CapsLock
    void slotTimeout();         //слот вызываемый таймером по таймауту времени отсчёта
    void slot_restart_Timer();  //Перезапуск таймера при нажатии на кнопку
    void check_password();      //метод проверки пароля и кода для входа в приложение
    void close_window();        //Закрытие приложения

    void slot_check_size_line();        //проверка длины вводимого текста
    void result_connection(QString);    //сообщение наличия/отсутствия соединения с интернетом

signals:

    void call_send_message(QString);

};

#endif // MAINWINDOW_H

