#ifndef WINDOW_SETTING_H
#define WINDOW_SETTING_H

#include <QMainWindow>
#include <QtWidgets>
#include <QString>
#include <QBoxLayout>
#include <QGroupBox>
#include <QDataStream>
#include <QThread>
#include "my_encryption.h"
#include "network_connection.h"


class Window_setting : public QMainWindow, My_Encrytion
{
    Q_OBJECT

public:
    Window_setting();
    ~Window_setting() override;

    void write_global_setting(QDataStream*);               //Внешний метод класса - public
    void read_global_setting(QDataStream*);                //Внешний метод класса - public
    void otherwise_global_setting();                       //Внешний метод класса - public
    void read_setting_mail();                              //при открытии настроек проверка установки настройки email

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void call_request_passw(QString);   //Передача команды на отправку запроса пароля в базе данных
    void call_test_message(QString);    //Отправка тестового письма на почту
    void save_main_password();
    void show_main_password();

private slots:

    void save1();                              //сохранение настроек приложения
    void show1();                              //метод, отправляющий сигнал на чтение сохранённого мастер-пароля приложения, для отображения в окне
    void cancel1();                            //закрытие окна настроек
    void difficult_of_passw();                 //вычисление сложности нового пароля
    void gen_master_passw();                   //генерация мастер-пароля
    void visible_check();                      //Метод добавляющий аутентификацию по коду с электронной почты
    void slot_request_pas();                   //слот запроса пароля в БД
    void result_connection(QString);           //сообщение наличия/отсутствия соединения с интернетом
    void response_from_db(QString);            //ответ от базы данных паролей
    void result_confirmation_email(QString);   //результат подтверждения адреса email
    void slot_check_size_line();               //проверка длины вводимого текста

private:

    QWidget* Setting_window;            //виджет окна настроек
    Network_connection* network_connection1;
    QThread* thread1;

    //Виджеты текущего сохранённого мастер-пароля
    QLabel* label_saved_passw;
    QLineEdit* enter_old_pas;
    QString old_global_pas;
    QPushButton* show_current_pas;      //кнопка обновления окна для показа текущих сохранённых настроек

    //Виджеты для ввода нового мастер-пароля
    QLineEdit* enter_new_pas;
    QString new_global_pas;

    //Для генерации пароля
    QLabel* label_generate_pasw;
    QPushButton* generate_pas;
    const int len_string=15;            //Длина генерируемого пароля
    int array_position[15];             //массив с позициями элементов
    int gen_elem_array_code[15];        //массив куда помещаются элементы пароля
    int gen_symbol[15];                 //генерируемый элемент

    //Unicode-коды символов
    int code_latin_symbol [52] = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};
    int code_digits [10] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};
    int code_spec_symbol [32] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 58, 59, 60, 61, 62, 63, 64, 91, 92, 93, 94, 95, 96, 123, 124, 125, 126};

    QPushButton* save_pas1;             //кнопка сохранения настроек
    QPushButton* cancel_setting;        //кнопка закрытия окна

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
    int set_mail_authoriz;              //1 или 0 для проверки видимости привязки почты
    bool permission_to_save;            //Разрешение на сохранение настроек приложения


    QGroupBox* groupBox_new_pas;
    QVBoxLayout* vertical_new_pas;
    QHBoxLayout* horizontal_button_sett;    //компонока кнопок управления окном настроек
    QHBoxLayout* horizontal_widg_gen;       //Компоновка виджетов генерации пароля
    QHBoxLayout* horizontal_checkbox;       //Компоновка виджетов с чекбокосом
    QHBoxLayout* horizontal_widget_mail;    //Компоновка виджетов ввода адреса эл почты
    QVBoxLayout* vertical_qroup;            //Компоновка виджетов запроса пароля в БД
    QHBoxLayout* horizontal_group;
    QGroupBox* group_check_DB;

    //Виджеты проверки нового пароля в базе данных паролей
    QLabel* label_check_pas_DB;
    QPushButton* button_request_DB;
    QLabel* label_result_DB;
    QLabel* wait_widget;        //Виджет ожидания проверки корректности email

    void write_data(QDataStream*, QString);        //Внутренний метод класса - private
    QString read_data(QDataStream*, QString);      //Внутренний метод класса - private
    QString password_generator();                  //метод для генерации пароля
    bool repeating_sequence(QString);              //проверка на повторяющиеся подряд последовательности
    void check_address_mail();                     //проверка корректности введённого адреса email

};


#endif // WINDOW_SETTING_H


