#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBox->setValue(12);
    ui->spinBox_2->setValue(13);
    ui->doubleSpinBox_7->setValue(15.5);
    ui->spinBox_7->setValue(92);
    ui->spinBox_14->setValue(92);
    ui->spinBox_4->setValue(34);
    ui->spinBox_11->setValue(38);
    ui->spinBox_3->setValue(12);
    ui->spinBox_10->setValue(14);
    ui->spinBox_20->setValue(7);
    ui->spinBox_21->setValue(47);
    ui->spinBox_22->setValue(89);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qreal cos_ksi_up = cos(ui->doubleSpinBox_2->value()) * cos(ui->doubleSpinBox->value() - ui->doubleSpinBox_5->value());
    qreal d_up = 42644 * sqrt(1.0-0.2954*cos_ksi_up);
    ui->label_50->setText("d вверх = "+QString::number(d_up) + " км");

    qreal cos_ksi_dw = cos(ui->doubleSpinBox_4->value()) * cos(ui->doubleSpinBox->value() - ui->doubleSpinBox_3->value());
    qreal d_dw = 42644 * sqrt(1.0-0.2954*cos_ksi_dw);
    ui->label_51->setText("d вниз = "+QString::number(d_dw) + " км");

    qreal lamda_up = 3*pow(10, 8)/(ui->spinBox->value()*pow(10, 9));

    qreal L0p = 20*log10(16*pow(pi, 2)*pow(d_up, 2)/pow(lamda_up, 2));
    ui->label_52->setText("l0р = -"+QString::number(L0p)+" дБ");
    qreal lamda_dw = 3*pow(10, 8)/(ui->spinBox_2->value()*pow(10, 9));

    qreal Ldop = (Lad + Lpd + Lrd);
    ui->label_53->setText("Lдоп = -"+QString::number(Ldop)+ " дБ");

    qreal Tsh = (ui->spinBox_20->value()-1.0)*T0;
    qreal n = ui->spinBox_22->value()/100.0;
    qreal Tsumm = ui->spinBox_21->value() + (T0*((1.0-n)/n)) + Tsh/n;
    ui->label_54->setText("Ts = "+QString::number(Tsumm) + " K");

    qreal Gprd = 20*log10(10*g*pow(ui->spinBox_3->value(), 2)/lamda_up);
    ui->label_55->setText("Gпрд = "+QString::number(Gprd) + " дБ");

    qreal Gprm = 20*log10(10*g*pow(ui->spinBox_10->value(), 2)/lamda_dw);
    ui->label_58->setText("Gпрм = "+QString::number(Gprm) + " дБ");

    qreal Rpdr = (L0p+Ldop+k*Tsumm*ui->spinBox_11->value()*pow(10, 6)+6+ui->doubleSpinBox_7->value()+70)/(Gprd*ui->spinBox_7->value()/100.0);
    Rpdr = Rpdr*10;
    ui->label_60->setText("Pпрд = " + QString::number(Rpdr) + " Вт");

    qreal last = 0;
    QPen pen;
    pen.setColor(Qt::red);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);


    QLineSeries *line = new QLineSeries();
    line->append(0, -100);
    line->append(14, -100);
    line->setPen(pen);
    line->setName("Граница");

    pen.setColor(Qt::darkGray);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(3);
    QLineSeries *series_up = new QLineSeries();
    series_up->setPen(pen);
    series_up->setName("Уровень сигнала");
    series_up->append(0, 0);
    series_up->append(3, Rpdr);
    last  = Rpdr * ui->spinBox_7->value()/100;// Lафт
    series_up->append(6, Rpdr * ui->spinBox_7->value()/100);
    last += Gprd;
    series_up->append(7, last);
    last = last - (L0p + Ldop);
    series_up->append(10, last);
    last += ui->spinBox_17->value();
    series_up->append(11, last);
    last -= ui->spinBox_17->value()*(1-ui->spinBox_22->value()/100.0);
    series_up->append(14, last);

    QChart *up = new QChart();
    up->setTitle("Диаграмма уровней мощности сигнала вверх");
    up->addSeries(line);
    up->addSeries(series_up);
    up->createDefaultAxes();
    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(15);
    axisX->setRange(0, 14);
    axisX->setLabelsVisible(false);
    up->setAxisX(axisX);
    up->axisY()->setRange(-200, 200);







    QLineSeries *series_dw = new QLineSeries();
    //series_dw->setColor(Qt::blue);
    series_dw->setName("Уровень сигнала");

    QChart *dw = new QChart();
    dw->setTitle("Диаграмма уровней мощности сигнала вниз");
    //dw->addSeries(line);
    dw->addSeries(series_dw);
    dw->createDefaultAxes();
    dw->axisX()->setRange(0, 14);
    dw->axisY()->setRange(-200, 200);


    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("График");
    QVBoxLayout *layout = new QVBoxLayout();
    QChartView *view;
    view = new QChartView(up);
    view->chart()->setAnimationOptions(QChart::AllAnimations);
    layout->addWidget(view);
    //view = new QChartView(dw);
    //layout->addWidget(view);
    dlg->setLayout(layout);
    dlg->resize(1000, 500);
    dlg->exec();
}
