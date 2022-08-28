#ifndef TAB_TEMPLATE_H
#define TAB_TEMPLATE_H

#include "datamanagement.h"

#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QTimer>

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QPixmap>          //для создания объекта изображения
#include <QImage>
#include <QShortcut>
#include <QKeySequence>
#include <QToolTip>


class Tab_template : public QMainWindow, protected DataManagement
{
    Q_OBJECT

public:
    explicit Tab_template(QWidget *parent=nullptr);
    ~Tab_template() override;

    void read_data_tab(QSqlDatabase&, int);                    //метод чтения данных
    void write_data_tab(QSqlDatabase&, int);                   //метод записи данных
    //void delete_data_tab(QSqlDatabase&, int);                //метод удаления данных
    bool get_flag_change_tab();                                //Проверка для Client_window были ли изменения данных на вкладках
    void save_tab_change(QSqlDatabase&, int);                  //сохраняет данные вкладок у которых флаг изменения true

    QString get_name_resource1();
    QString get_name_resource2();
    void select_resource1();
    void select_resource2();

private:

    QWidget* widget;
    bool dataChanged;                              //флаг изменения данных на вкладке
    void difficult_of_passw(QLineEdit*);
    bool repeating_sequence(QString);
    void setVisiblePas();
    QString password_generator(int);
    const int length_pas=10;

    //Размеры и компонока виджетов вкладки
    int id_template_widget;
    QWidget* templateWidget[6];
    int fixedHeightTemplateWidget;
    int fixedWidthNameResource;
    int fixSizeLabel;
    int fixSizeButton;
    int id_horizontal_layout;
    QHBoxLayout* horizontalWidget[6];

    //Виджеты названия ресурса
    QLabel* name_label1;
    QLineEdit* name_resourse1;
    QPushButton* btnCopyLogin1;

    //Виджеты логина
    QLabel* login_label1;
    QLineEdit* login_line1;
    QPushButton* btnCopyPassw1;

    //Виджеты пароля
    QLabel* passw_label1;
    QLineEdit* passw_line1;
    QPushButton* gen_pas_button1;
    QPushButton* visiblePassw1;

    //Виджеты названия ресурса
    QLabel* name_label2;
    QLineEdit* name_resourse2;
    QPushButton* btnCopyLogin2;

    //Виджеты логина
    QLabel* login_label2;
    QLineEdit* login_line2;
    QPushButton* btnCopyPassw2;

    //Виджеты пароля
    QLabel* passw_label2;
    QLineEdit* passw_line2;
    QPushButton* gen_pas_button2;
    QPushButton* visiblePassw2;

    //Кнопка сохранения данных на вкладке
    QPushButton* save_change_page;
    QVBoxLayout* verticalWidget;

    //Для копирования в буфер обмена
    QMimeData mime;
    QTimer timer1_clipboard;
    QTimer timer2_clipboard;
    QTimer timer3_clipboard;
    QTimer timer4_clipboard;

private slots:

    void save_data_tab();
    void slot_check_size_line();                    //проверка длины вводимого текста
    void changeVisiblePas1();
    void changeVisiblePas2();
    void generate_passw1();
    void generate_passw2();

    void copy_login1();
    void copy_passw1();
    void copy_login2();
    void copy_passw2();
    void timeout1_clipboard();
    void timeout2_clipboard();
    void timeout3_clipboard();
    void timeout4_clipboard();

signals:

    void signal_save_tab();

};

#endif // TAB_TEMPLATE_H

