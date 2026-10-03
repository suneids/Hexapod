#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/icon.png"));
    MainWindow w;
    return a.exec();
}
