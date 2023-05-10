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
#include <QPixmap>         
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

    void read_data_tab(QSqlDatabase&, int);           
    void write_data_tab(QSqlDatabase&, int);           
    //void delete_data_tab(QSqlDatabase&, int);      
    bool get_flag_change_tab();                  
    void save_tab_change(QSqlDatabase&, int);         

    QString get_name_resource1();
    QString get_name_resource2();
    void select_resource1();
    void select_resource2();

private:

    QWidget* widget;
    bool dataChanged;                       
    void difficult_of_passw(QLineEdit*);
    bool repeating_sequence(QString);
    void setVisiblePas();
    QString password_generator(int);
    const int length_pas=10;

    int id_template_widget;
    QWidget* templateWidget[6];
    int fixedHeightTemplateWidget;
    int fixedWidthNameResource;
    int fixSizeLabel;
    int fixSizeButton;
    int id_horizontal_layout;
    QHBoxLayout* horizontalWidget[6];

    QLabel* name_label1;
    QLineEdit* name_resourse1;
    QPushButton* btnCopyLogin1;

    QLabel* login_label1;
    QLineEdit* login_line1;
    QPushButton* btnCopyPassw1;

    QLabel* passw_label1;
    QLineEdit* passw_line1;
    QPushButton* gen_pas_button1;
    QPushButton* visiblePassw1;

    QLabel* name_label2;
    QLineEdit* name_resourse2;
    QPushButton* btnCopyLogin2;

    QLabel* login_label2;
    QLineEdit* login_line2;
    QPushButton* btnCopyPassw2;

    QLabel* passw_label2;
    QLineEdit* passw_line2;
    QPushButton* gen_pas_button2;
    QPushButton* visiblePassw2;

    QPushButton* save_change_page;
    QVBoxLayout* verticalWidget;
    QMimeData mime;
    QTimer timer_clipboard;

private slots:

    void save_data_tab();
    void slot_check_size_line();       
    void changeVisiblePas1();
    void changeVisiblePas2();
    void generate_passw1();
    void generate_passw2();
    void timeout_clipboard();
    void abstractCopyData();

signals:

    void signal_save_tab();

};

#endif // TAB_TEMPLATE_H

