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

    //for validation
    std::vector<QLineEdit *> array;
    array.resize(16);
    array ={ui->x1,ui->y1,ui->z1,ui->x2,ui->y2,ui->z2,ui->x3,ui->y3,ui->z3,ui->b1,ui->b2,ui->b3,ui->xi,ui->yi,ui->zi,ui->w};
    for(int i=0;i<16;i++)
        array[i]->setValidator( new QDoubleValidator(-100, 100, 2, this) );
    ui->i->setValidator( new QDoubleValidator(-100, 100, 2 ,this) );

    //setStyleSheet("background-color:white;");

    setAutoFillBackground( true );
    ui->errorTB->setTextColor(QColor( "red" ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_calcBtn_clicked(){
    readDataAndCalc();
}

void MainWindow::readDataAndCalc() {
    std::vector<QLineEdit *> array;
    array.resize(12);

    //QLineEdit** array = {nullptr};

    array ={ui->x1,ui->y1,ui->z1,ui->x2,ui->y2,ui->z2,ui->x3,ui->y3,ui->z3};//,ui->b1,ui->b2,ui->b3};
/////////////////////////////////////////create 2d matrix here
        double** arr = new double* [3];
        double* b = new double[3];
        for (int i = 0; i < 3; i++)
            arr[i] = new double[3];

        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                arr[i][j]=array[3*i+j]->text().toDouble();

        b[0] = ui->b1->text().toDouble();
        b[1] = ui->b2->text().toDouble();
        b[2] = ui->b3->text().toDouble();
        //////////////////////////////////////////////////////////////////////////////////
        double w;
        int it;



        //Reading all text edits..
        int msg=ui->SolvingMethodCB->currentIndex();
        int sw=ui->StopCriteriaCB->currentIndex();//elements of combo box 0:# iteratio 1: stopping error
        switch(msg){

        case 0:{ // sor
        switch(sw){
            case 0:{ //# of itr
                 if(ui->i->text().isEmpty()){// if there is no value in # iter text box
                        ui->errorTB->setText("Please insert the no. of iterations to complete the solution!!");
                          return;
                 }
                 else{
                     it = ui->i->text().toInt();
                     it++;
                  }
                 if(ui->w->text().isEmpty()){
                    ui->errorTB->setText("Please insert the w to complete the solution!!");
                    return;
                  }
                 else w = ui->w->text().toDouble();
                 reArrangeArray(arr, b, 3);
                 calculateSor(arr, b, it, w);
                       break;
              }

              case 1:{      double err=0 ;//stopping error(i don't need here # of iter it will stop when error<= given error

                        if(ui->i->text().isEmpty()){// if there is no value in # iter text box
                            ui->errorTB->setText("Please insert the stopping value to complete the solution!!");
                              return;
                     }
                     else err = ui->i->text().toDouble();

                    if(ui->w->text().isEmpty()){
                    ui->errorTB->setText("Please insert the w to complete the solution!!");
                       return;
                     }
                     else w = ui->w->text().toDouble();
             reArrangeArray(arr, b, 3);
             calculateSorStpError(arr, b,err,w);
             break;

               }
            }
        break;
    }
    case 1:{//gauess seidal..
    switch(sw){
        case 0:{ //# of itr
                if(ui->i->text().isEmpty()){// if there is no value in # iter text box
                    ui->errorTB->setText("Please insert the no. of iterations to complete the solution!!");
                      return;
                 }
                 else{
                    it = ui->i->text().toInt();
                    it++;
                }
                 ui->errorTB->setText("");
                 reArrangeArray(arr, b, 3);
                 calculateGaussSeidel(arr,b,it);
                       break;  }

        case 1:{ double err=0 ;//stopping error(i don't need here # of iter it will stop when error<= given error

          if(ui->i->text().isEmpty()){// if there is no value in # iter text box
                ui->errorTB->setText("Please insert the stopping value to complete the solution!!");
                  return;
             }
          else err = ui->i->text().toDouble();

           ui->errorTB->setText("");
            reArrangeArray(arr, b, 3);
            calculateGaussSeidalStpErr(arr,b,err);
            break;
         }
        }
    break;
      }
    }
}

  //////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::calculateSorStpError(double** a, double* b,double e ,double w){
    variable x, y, z;
    y.column = 1; z.column = 2;
    double** res = new double* [50];int lp=1;
    for (int i = 0; i < 50; i++)
        res[i] = new double[3];

    res[0][0]= ui->xi->text().toDouble();
    res[0][1]= ui->yi->text().toDouble();
    res[0][2]= ui->zi->text().toDouble();
double error=0;
do{
   // for (int i = 1; i < n; i++) {

        res[lp][0] = w * (-(a[0][1] * res[y.row][y.column] + a[0][2] * res[z.row][z.column]) + b[0]) / a[0][0] + (1 - w) * res[lp - 1][0];
        x++;
        res[lp][1] = w * (-(a[1][0] * res[x.row][x.column] + a[1][2] * res[z.row][z.column]) + b[1]) / a[1][1] + (1 - w) * res[lp- 1][1];
        y++;
        res[lp][2] = w * (-(a[2][1] * res[y.row][y.column] + a[2][0] * res[x.row][x.column]) + b[2]) / a[2][2] + (1 - w) * res[lp - 1][2];
        z++;
      error=calculateEroor(res,lp+1);
    //}
lp++;
}while(error > e);
    ui->solution->append("SOR sol. :");
    ui->solution->append("i :    x    y    z");
    ui->solution->append("-------------------------------");
   for (int i = 0; i < lp; i++)
    {
       ui->solution->append("");
       ui->solution->insertPlainText(QString::number(i) + " :   ");
       for (int j = 0; j < 3; j++)
            ui->solution->insertPlainText(QString::number(res[i][j]) + "  ");
    }
    ui->solution->append("");
   // double error = calculateEroor(res, n);
    ui->relError->setText(QString::number(error));
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::calculateGaussSeidalStpErr(double** a, double* b,double e ){
    variable x, y, z;
    y.column = 1; z.column = 2;
    double** res = new double* [50];int lp=1;
    for (int i = 0; i < 50; i++)
        res[i] = new double[3];

    res[0][0]= ui->xi->text().toDouble();
    res[0][1]= ui->yi->text().toDouble();
    res[0][2]= ui->zi->text().toDouble();
double error=0;
do{
   // for (int i = 1; i < n; i++) {

        res[lp][0] =  (-(a[0][1] * res[y.row][y.column] + a[0][2] * res[z.row][z.column]) + b[0]) / a[0][0] ;
        x++;
        res[lp][1] =  (-(a[1][0] * res[x.row][x.column] + a[1][2] * res[z.row][z.column]) + b[1]) / a[1][1] ;
        y++;
        res[lp][2] =  (-(a[2][1] * res[y.row][y.column] + a[2][0] * res[x.row][x.column]) + b[2]) / a[2][2] ;
        z++;
      error=calculateEroor(res,lp+1);
    //}
lp++;
}while(error > e);
    ui->solution->append("GaussSeidal. :");
    ui->solution->append("i :    x    y    z");
    ui->solution->append("-------------------------------");
   for (int i = 0; i < lp; i++)
    {
       ui->solution->append("");
       ui->solution->insertPlainText(QString::number(i) + " :   ");
       for (int j = 0; j < 3; j++)
            ui->solution->insertPlainText(QString::number(res[i][j]) + "  ");
    }
    ui->solution->append("");
   // double error = calculateEroor(res, n);
    ui->relError->setText(QString::number(error));
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::calculateSor(double** a, double* b, int n, double w){
    variable x, y, z;
    y.column = 1; z.column = 2;
    double** res = new double* [n];
    for (int i = 0; i < n; i++)
        res[i] = new double[3];

    res[0][0]= ui->xi->text().toDouble();
    res[0][1]= ui->yi->text().toDouble();
    res[0][2]= ui->zi->text().toDouble();

    for (int i = 1; i < n; i++) {

        res[i][0] = w * (-(a[0][1] * res[y.row][y.column] + a[0][2] * res[z.row][z.column]) + b[0]) / a[0][0] + (1 - w) * res[i - 1][0];
        x++;
        res[i][1] = w * (-(a[1][0] * res[x.row][x.column] + a[1][2] * res[z.row][z.column]) + b[1]) / a[1][1] + (1 - w) * res[i - 1][1];
        y++;
        res[i][2] = w * (-(a[2][1] * res[y.row][y.column] + a[2][0] * res[x.row][x.column]) + b[2]) / a[2][2] + (1 - w) * res[i - 1][2];
        z++;

    }
    ui->solution->append("SOR sol. :");
    ui->solution->append("i :    x    y    z");
    ui->solution->append("-------------------------------");
   for (int i = 0; i < n; i++)
    {
       ui->solution->append("");
       ui->solution->insertPlainText(QString::number(i) + " :   ");
       for (int j = 0; j < 3; j++)
            ui->solution->insertPlainText(QString::number(res[i][j]) + "  ");
    }
    ui->solution->append("");
    double error = calculateEroor(res, n);
    ui->relError->setText(QString::number(error));
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
double MainWindow::calculateEroor(double** a, int n) {
    double s = 0;
    for (int i = 0; i < 3; i++)
        s = max(((a[n - 1][i] - a[n - 2][i]) / a[n - 1][i]) * 100, s);

    return s;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::calculateGaussSeidel(double **a, double *b, int n)
{
    variable x, y, z;
    y.column = 1;
    z.column = 2;
    double **res = new double *[n];
    for (int i = 0; i < n; i++)
        res[i] = new double[3];
    res[0][0] = ui->xi->text().toDouble();
    res[0][1] = ui->yi->text().toDouble();
    res[0][2] = ui->zi->text().toDouble();

    for (int i = 1; i < n; i++)
    {

        res[i][0] = (-(a[0][1] * res[y.row][y.column] + a[0][2] * res[z.row][z.column]) + b[0]) / a[0][0];
        x++;
        res[i][1] = (-(a[1][0] * res[x.row][x.column] + a[1][2] * res[z.row][z.column]) + b[1]) / a[1][1];
        y++;
        res[i][2] = (-(a[2][1] * res[y.row][y.column] + a[2][0] * res[x.row][x.column]) + b[2]) / a[2][2];
        z++;
    }
    ui->solution->append("Gauss Seidal sol. :");
    ui->solution->append("i :    x    y    z");
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
    ui->solution->append("");
    double error = calculateEroor(res, n);
    ui->relError->setText(QString::number(error));
    return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
double *& MainWindow::chooseAndSwap(double **&numb, int y, int &o)
{
    int prev, next;
    if (y == 0)
    {
        prev = 2, next = 1;
    }
    else if (y == 2)
    {
        next = 0, prev = 1;
    }
    else

    {
        prev = y - 1, next = y + 1;
    }
    if (numb[prev][y] <= numb[prev][prev] + numb[prev][next])
    {
        o = next;
        return numb[next];
    }
    else
    {
        o = prev;
        return numb[prev];
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::reArrangeArray(double **&a, double *&b, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
    tani:
        sum = 0;
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            else
                sum += a[i][j];
        }
        if (a[i][i] <= sum)
        {
            int r;
            double *d = chooseAndSwap(a, i, r);
            a[r] = a[i];
            a[i] = d;
            swap(b[i], b[r]);
            i++;
            if (i < !n)
                return;
            goto tani;
        }
    }
    return;
}


