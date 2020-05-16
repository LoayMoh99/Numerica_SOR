#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setStyleSheet("background-color:white;");
    setAutoFillBackground( true );
}

MainWindow::~MainWindow()
{
    delete ui;
}

