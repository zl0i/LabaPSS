#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <qmath.h>
#include <cmath>
#include <QtCharts/QChartGlobal>
#include <QtCharts>
#include <QLineSeries>
#include <QChartView>

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

#define pi 3.14
#define Lad 0.8
#define Lpd 0.9
#define Lrd 0.2
#define T0 290.0
#define g 0.7
#define k 1.38*pow(10, -23)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
