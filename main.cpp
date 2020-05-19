#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("dr amani elhob");
    w.setWindowIcon(QIcon(":i.png"));
    w.show();
    return a.exec();
}
