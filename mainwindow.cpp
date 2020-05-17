#include "mainwindow.h"
#include "Variable.h"
#include "ui_mainwindow.h"
#include <algorithm>
using namespace std;

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


void MainWindow::on_calcBtn_clicked(){
    readDataAndCalc();
}

void MainWindow::readDataAndCalc(){
    try {
        /////////////////////////////////////////create 2d matrix here
        double** arr = new double* [3];
        double* b = new double[3];
        for (int i = 0; i < 3; i++)
            arr[i] = new double[3];

        arr[0][0] = ui->x1->toPlainText().toDouble();
        arr[0][1] = ui->y1->toPlainText().toDouble();
        arr[0][2] = ui->z1->toPlainText().toDouble();
        arr[1][0] = ui->x2->toPlainText().toDouble();
        arr[1][1] = ui->y2->toPlainText().toDouble();
        arr[1][2] = ui->z2->toPlainText().toDouble();
        arr[2][0] = ui->x3->toPlainText().toDouble();
        arr[2][1] = ui->y3->toPlainText().toDouble();
        arr[2][2] = ui->z3->toPlainText().toDouble();


        b[0] = ui->b1->toPlainText().toDouble();
        b[1] = ui->b2->toPlainText().toDouble();
        b[2] = ui->b3->toPlainText().toDouble();
        //////////////////////////////////////////////////////////////////////////////////
        double w;
        int it;
        //Reading all text edits..
        if(ui->StopCriteriaCB->currentIndex()==0)//# of iter
         it = ui->i->toPlainText().toInt();
        else it=3;
        it++;
        if(ui->SolvingMethodCB->currentIndex()==0)//sor
          w = ui->w->toPlainText().toDouble();
        else w=1;

        calculateSor(arr, b, it, w);
    }
     catch(...) {
        ui->errorTB->setText("Error! You wrote an String not a number!!");
    }
}

void MainWindow::calculateSor(double** a, double* b, int n, double w){
    variable x, y, z;
    y.column = 1; z.column = 2;
    double** res = new double* [n];
    for (int i = 0; i < n; i++)
        res[i] = new double[3];

    res[0][0]= ui->xi->toPlainText().toDouble();
    res[0][1]= ui->yi->toPlainText().toDouble();
    res[0][2]= ui->zi->toPlainText().toDouble();

    for (int i = 1; i < n; i++) {

        res[i][0] = w * (-(a[0][1] * res[y.row][y.column] + a[0][2] * res[z.row][z.column]) + b[0]) / a[0][0] + (1 - w) * res[i - 1][0];
        x++;
        res[i][1] = w * (-(a[1][0] * res[x.row][x.column] + a[1][2] * res[z.row][z.column]) + b[1]) / a[1][1] + (1 - w) * res[i - 1][1];
        y++;
        res[i][2] = w * (-(a[2][1] * res[y.row][y.column] + a[2][0] * res[x.row][x.column]) + b[2]) / a[2][2] + (1 - w) * res[i - 1][2];
        z++;

    }
    ui->solution->insertPlainText("i :    x    y    z");
    ui->solution->append("-------------------------------");
   for (int i = 0; i < n; i++)
    {
       ui->solution->append("");
       //cout << i << ")   ";
       ui->solution->insertPlainText(QString::number(i) + " :   ");
       for (int j = 0; j < 3; j++)
            //cout << res[i][j] << "  ";
            ui->solution->insertPlainText(QString::number(res[i][j]) + "  ");
       // cout << endl;
    }

    double error = calculateEroor(res, n);
    ui->relError->setText(QString::number(error));
    return;
}

double MainWindow::calculateEroor(double** a, int n) {
    double s = 0;
    for (int i = 0; i < 3; i++)
        s = max(((a[n - 1][i] - a[n - 2][i]) / a[n - 1][i]) * 100, s);
        //s=((a[n - 1][i] - a[n - 2][i]) / a[n - 1][i]) * 100;

    return s;

};
