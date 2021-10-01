#ifndef TAB_TEMPLATE_H
#define TAB_TEMPLATE_H

#include <QMainWindow>
#include <QtWidgets>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QSettings>
#include <QRandomGenerator64>
#include <QBoxLayout>
#include <QDataStream>
#include "my_encryption.h"

#include <QPixmap>
#include <QPalette>
#include <QImage>
#include <QBrush>

class Tab_template : public QMainWindow, My_Encrytion

{
    Q_OBJECT

public:
    Tab_template();
    int counter1;           //счётчик изменения видимости пароля 1
    int counter2;           //счётчик изменения видимости пароля 1
    ~Tab_template() override;

protected:
      void resizeEvent(QResizeEvent*) override;

public slots:

    void write_regedit(QSettings*, int, int, int);  //Запись данных в реестр
    void read_regedit(QSettings*, int);             //Чтение данных их реестра
    void delete_regedit(QSettings*, int);           //Удаление данных в реестре
    void write_data_tab(QDataStream*);              //Внешний метод класса - public
    void read_data_tab(QDataStream*);               //Внешний метод класса - public
    void otherwise_data_tab();
    QString get_name_resource1();
    QString get_name_resource2();
    void select_resource1();
    void select_resource2();

private slots:

    void visible_passw1();
    void visible_passw2();
    void write_change_tab();
    void generate_passw1();
    void generate_passw2();
    void slot_check_size_line();        //проверка длины вводимого текста

signals:
    void save_page();
private:
   void visible_password_regedit();
   void write_data(QDataStream*, QString);        //Внутренний метод класса - private
   QString read_data(QDataStream*, QString);      //Внутренний метод класса - private
   QString password_generator();                  //метод для генерации пароля

   //Компоновка виджетов
   int id_template_widget;
   QWidget* templateWidget[6];
   int fixedHeightTemplateWidget;
   int fixedWidthNameResource;
   int id_horizontal_layout;
   QHBoxLayout* horizontalWidget[6];
   QVBoxLayout* verticalWidget;

   int fixWidthName;
   int fixSizeLabel;
   int fixSizeButton;

   QWidget* widget;
   QLabel* label_name_data1;
   QLineEdit* name_data_entry1;
   QLabel* label_login1;
   QLineEdit* username_entry1;
   QLabel* label_pas1;
   QLineEdit* password_entry1;
   QPushButton* gen_pas1;
   QPushButton* visible_pas1;

   QLabel* label_name_data2;
   QLineEdit* name_data_entry2;
   QLabel* label_login2;
   QLineEdit* username_entry2;
   QLabel* label_pas2;
   QLineEdit* password_entry2;
   QPushButton* gen_pas2;
   QPushButton* visible_pas2;
   QPushButton* save_change_page;

   QString name_data_1;
   QString username1;
   QString password1;

   QString name_data_2;
   QString username2;
   QString password2;

   //Для генерации пароля
   const int len_string=10;         //Длина пароля. При изменении длины генерируемого пароля поменять коэффициенты в методе, влияющие на кол-во символов, цифр в пароле
   int array_position[10];          //массив с позициями элементов
   int gen_elem_array_code[10];     //массив куда помещаются элементы пароля
   int gen_symbol[10];              //генерируемый элемент

   //Unicode коды
   int code_latin_symbol [52] = {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};
   int code_digits [10] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};
   int code_spec_symbol [32] = {33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 58, 59, 60, 61, 62, 63, 64, 91, 92, 93, 94, 95, 96, 123, 124, 125, 126};

};

#endif // TAB_TEMPLATE_H
