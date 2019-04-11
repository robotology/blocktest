#include "mainwindow.h"
#include <QApplication>
#include <QSplitter>
#include <QStandardItemModel>
#include <qtreeview.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
