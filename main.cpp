#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/resources/main-icon.png"));

    MainWidget w;
    w.show();
    return a.exec();
}
