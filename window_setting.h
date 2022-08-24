#ifndef WINDOW_SETTING_H
#define WINDOW_SETTING_H

#include "datamanagement.h"
#include "network_connection.h"

#include <QApplication>
#include <QEventLoop>

#include <QMainWindow>
#include <QMovie>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QString>
#include <QBoxLayout>
#include <QGroupBox>
#include <QThread>



class Window_setting : public QMainWindow, protected DataManagement
{
    Q_OBJECT

public:
    explicit Window_setting(QWidget *parent=nullptr);
    ~Window_setting() override;

    void read_settings();

protected:

    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:

    void save();                               //сохранение настроек приложения
    void cancel();                             //закрытие окна настроек
    void difficult_of_passw();                 //вычисление сложности нового пароля
    void gen_master_passw();                   //генерация мастер-пароля
    void visible_checkbox();                   //Смена состояния чекбокса
    void slot_request_pas();                   //слот запроса пароля в БД
    void result_connection(QString);           //сообщение наличия/отсутствия соединения с интернетом
    void response_from_db(QString);            //ответ от базы данных паролей
    void result_confirmation_email(QString);   //результат подтверждения адреса email
    void slot_check_size_line();               //проверка длины вводимого текста
    void getDataForSmtp();
    void changeVisibleOld_pas();
    void changeVisibleNew_pas();

signals:

    void call_network_action(QString, QString, QString);
    void send_data_smtp(QString, QString);

private:

    void save_password();
    void read_setting_mail(int&, QString&);
    void save_mail_status();
    void save_mail();
    QString password_generator(int);                  //метод для генерации пароля
    bool repeating_sequence(QString);              //проверка на повторяющиеся подряд последовательности
    void check_address_mail(QString);              //проверка корректности введённого адреса email
    void dialog_message();

    QWidget* Setting_window;            //виджет окна настроек
    Network_connection* network_connection1;
    QThread* thread1;

    //Виджеты текущего сохранённого мастер-пароля
    QLabel* label_saved_passw;
    QLineEdit* enter_old_pas;
    QPushButton* visibleOldPas;

    //Виджеты для ввода нового мастер-пароля
    QLineEdit* enter_new_pas;
    QPushButton* visibleNewPas;

    //Для генерации пароля
    QLabel* label_generate_pasw;
    QPushButton* generate_pas;
    const int length_pas=15;             //Длина генерируемого пароля

    QPushButton* save_settings;         //кнопка сохранения настроек
    QPushButton* cancel_settings;        //кнопка закрытия окна

    //Определение сложности пароля
    QLabel* label_difficult_passw;      //Текущая сложость пароля
    //Массив самых простых паролей
    QString array_easy_pas[33]={"hello", "0123456789", "9876543210", "1234", "4567", "6789", "9876", "4321", "привет", "qwerty", "пароль", "йцукен", "asdfgh", "password", "ytrewq", "zxcvb", "1q2w3e", "dragon", "monkey", "qazwsx", "iloveyou", "pass", "default", "admin", "guest", "911", "314159", "271828", "122358", "Mypassword", "smoke", "sun", "mypas"};

    //добавление аутентификации по эл. почте
    QCheckBox* check_box_for_email;     //Checkbox для установки/снятия настройки аутентификация по email
    QLabel* label_checkbox;
    QLabel* label_email;
    QLineEdit* enter_address_mail;
    QString address_mail_string;
    bool change_status_mail;

    QGroupBox* groupBox_new_pas;
    QVBoxLayout* vertical_new_pas;
    QHBoxLayout* horizontal_button_sett;    //компонока кнопок управления окном настроек
    QHBoxLayout* horizontal_widg_gen;       //Компоновка виджетов генерации пароля
    QHBoxLayout* horizontal_checkbox;       //Компоновка виджетов с чекбокосом
    QHBoxLayout* horizontal_widget_mail;    //Компоновка виджетов ввода адреса эл почты
    QVBoxLayout* vertical_qroup;            //Компоновка виджетов запроса пароля в БД
    QHBoxLayout* horizontal_group;
    QGroupBox* group_check_DB;
    QVBoxLayout* vertical1;
    QHBoxLayout* horizontal_old_pas;
    QHBoxLayout* horizontal_new_pas;

    //Виджеты проверки нового пароля в базе данных паролей
    QLabel* label_check_pas_DB;
    QPushButton* button_request_DB;
    QLabel* label_result_DB;
    QLabel* wait_widget;        //Виджет ожидания проверки корректности email

    QSqlDatabase db;

    //Виджет загрузки
    QLabel progress_widget;
    QMovie* movie;
};


#endif // WINDOW_SETTING_H


