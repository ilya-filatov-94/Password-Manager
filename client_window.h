#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include "window_setting.h"
#include "tab_template.h"

#include <QMessageBox>
#include <QMainWindow>
#include <QtWidgets>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QShortcut>
#include <QBoxLayout>
#include <QDragEnterEvent>  //для реализации перетаскивания
#include <QMimeData>        //для перетаскивания Mime-файлов
#include <QPalette>         //для заполнения фона создаём объект палитры
#include <QBrush>           //для заполнения фона класс кисти (тип заполнения)
#include <QPixmap>          //для создания объекта изображения
#include <QDir>             //Для определения текущей директории файла
#include <QVector>          //для вектора строк с путями к файлам


class QAction;
class QActionGroup;
class QLabel;
class QMenu;

const int max_tabs=31;   //Максимальное количество вкладок+1 (30 шт)

class Client_window : public QMainWindow
{
    Q_OBJECT

public:

    Client_window(QWidget *parent = nullptr);
    ~Client_window() override;

    void set_backgound_image();     //установка фона вкладок при открытии приложения

protected:

    #ifndef QT_NO_CONTEXTMENU
        void contextMenuEvent(QContextMenuEvent* event) override;
    #endif // QT_NO_CONTEXTMENU

    void closeEvent(QCloseEvent* event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

    //Обработка событий нажатия, перестакивания и отпускания для установки изображения для фона
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

public slots:

    void readSettings();

private slots:

    //Функции для действий от кнопок подменю
    void save();
    void redo();
    void add_my_tab();
    void delete_my_tab();
    void about();

    void writeSettings();
    void write_all();       //Инкрементирование счётчика по нажатию кнопки сохранить всё. По результату счёта вызывается write_all_data при закрытии приложения
    void write_all_data();
    void show_glob_pas();
    void search_tabs();

private:

    void createActions();
    void createMenus();

    int tabs_counter;
    //Кнопки меню
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;
    //Кнопки подменю с действиями
    QAction* saveAct;
    QAction* exitAct;
    QAction* redoAct;
    QAction* adding_tabs;       //Добавление вкладок
    QAction* delete_tabs;       //Удаление вкладок
    QAction* aboutAct;

    QTabWidget* MyGlobalWindow;
    Tab_template* tab_widget[max_tabs];  //массив вкладок
    Window_setting* setting_window;

    QFile* file1;
    QSettings* settings;
    int counter_records;
    QLineEdit* search_line;
    QPushButton* search_button;
    QWidget* search_widget;
    QShortcut* key_return;
    QHBoxLayout* layoutCornerWidget;

    //Для перетаскивания
    QFile* file_for_paths;      //текстовый файл с путями для изображений вкладок
    int counter_change_palette; //счётчик изменений фона

};


#endif // CLIENT_WINDOW_H
