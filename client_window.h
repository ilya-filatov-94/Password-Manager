#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include "window_setting.h"
#include "tab_template.h"
#include "windows.h"

#include <QApplication>
#include <QEventLoop>

#include <QMainWindow>
#include <QToolButton>
#include <QTabBar>
#include <QStatusBar>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QMessageBox>
#include <QStyle>
#include <QString>
#include <QPalette>         //для заполнения фона создаём объект палитры
#include <QBrush>           //для заполнения фона класс кисти (тип заполнения)
#include <QPixmap>          //для создания объекта изображения
#include <QShortcut>
#include <QKeySequence>
#include <QMovie>

#include <QFile>
#include <QSettings>
#include <QDataStream>
#include <QDragEnterEvent>  //для реализации перетаскивания
#include <QMimeData>        //для перетаскивания Mime-файлов
#include <QDir>             //Для определения текущей директории файла изображения
#include <QVector>          //для вектора строк с путями к файлам
#include <QStringList>
#include <QtSql>
#include <QSqlDatabase>

//взаимодействие с меню
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>



const int max_tabs=31;   //Максимальное количество вкладок+1 (30 шт)

class Client_window : public QMainWindow
{
    Q_OBJECT

public:

    explicit Client_window(QWidget *parent = nullptr);
    ~Client_window() override;

    void readSettings();

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

    void resizeEvent(QResizeEvent*) override;

private slots:

    //Функции для действий от кнопок подменю
    void save();
    void redo();
    void addTab();
    void closeTab();
    void about();

    void search_tabs();
    void writeData();          //Запись данных в БД активной вкладки
    void writeData_null();     //для записи БД 0 вкладки
    void writeAllData();       //запись данных всех вкладок

private:

    void readData();                                                 //чтение данных приложения при открытии
    void writeChangedData();                                         //запись данных тех вкладок на которых происходили изменения
    //void deleteDataTab(int&);
    void dialog_message();
    void show_progressBar(bool);

    void createActions();
    void createMenus();

    void set_bg_image(QPalette&, Qt::AspectRatioMode);               //установка фона вкладок при открытии приложения
    void set_default_bg_image(QPalette&, int);                       //установка стандартногот фона вкладки при добавлении новой
    void readAllPathImages(QVector<QString>&, QVector<QString>&);
    void writeAllPathImages(QVector<QString>&, QVector<QString>&);
    void writeOnePathImage(QString&, QString&);

    QTabWidget* MyGlobalWindow;
    Tab_template* tab_widget[max_tabs];  //массив шаблонов вкладок (интерфейсы вкладок одинаковы)
    QPixmap pixmap_default;              //изображение вкладок
    Window_setting* setting_window;      //окно настроек приложения
    QSettings* settings;

    QWidget* widgetButtonAddTab;
    QToolButton* button_addTab;
    QToolButton* button_delTab[max_tabs];
    QPixmap closeBtn;
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

    //Угловой виджет поиска
    QLineEdit* search_line;
    QPushButton* search_button;
    QWidget* search_widget;
    QShortcut* key_search;
    QHBoxLayout* layoutCornerWidget;

    QSqlDatabase db;

    //Для перетаскивания
    QFile file_for_paths_bg;      //текстовый файл с путями для изображений вкладок
    int counterBackgroundsSaves;        //счётчик изменений фона

    //Виджет загрузки
    QLabel wait_widget;
    QMovie* movie;
};


#endif // CLIENT_WINDOW_H
