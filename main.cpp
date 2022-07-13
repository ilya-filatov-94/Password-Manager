#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QLatin1String>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile StyleSheetFile(":/img/my_Styles2.qss");
    if (StyleSheetFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString styleSheet=QLatin1String(StyleSheetFile.readAll());
        a.setStyleSheet(styleSheet);
        StyleSheetFile.close();
    }
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.show();
    w.requestReadData();

    return a.exec();
}
