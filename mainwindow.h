#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT //just by2ol ll compiler en Q objects will be used here..

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void readDataAndCalc();
    void calculateSor(double** a, double* b, int n, double w) ;
    double calculateEroor(double** a, int n);
    double*& chooseAndSwap(double**&numb,int y,int &o);
    void reArrangeArray(double**&a,double *&b, int n);
    void calculateGaussSeidel(double** a, double* b, int n);
    void on_calcBtn_clicked();
    void calculateSorStpError(double** a, double* b,double e ,double w);
    void calculateGaussSeidalStpErr(double** a, double* b,double e );
};
#endif // MAINWINDOW_H
