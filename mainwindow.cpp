#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Лабораторная работа №6");

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

    qreal L0up = 20*log10(2*pi * d_up*1000/lamda_up);
    ui->label_52->setText("l0uр = -"+QString::number(L0up)+" дБ");

    qreal lamda_dw = 3*pow(10, 8)/(ui->spinBox_2->value()*pow(10, 9));

    qreal L0dw = 20*log10(2*pi * d_dw*1000/lamda_dw);
    ui->label_54->setText("l0dw = -"+QString::number(L0dw)+" дБ");

    qreal Ldop = (Lad + Lpd + Lrd);
    ui->label_53->setText("Lдоп = -"+QString::number(Ldop)+ " дБ");

    qreal Tsh = (ui->spinBox_20->value()-1.0)*T0;
    qreal n = ui->spinBox_22->value()/100.0;
    qreal Tsumm = ui->spinBox_21->value() + (T0*((1.0-n)/n)) + Tsh/n;

    qreal Gprd = 20*log10(10*g*pow(ui->spinBox_3->value(), 2)/lamda_up);
    ui->label_55->setText("Gпрд = "+QString::number(Gprd) + " дБ");

    qreal Gprm = 20*log10(10*g*pow(ui->spinBox_10->value(), 2)/lamda_dw);
    ui->label_58->setText("Gпрм = "+QString::number(Gprm) + " дБ");

    int t =  20*log10(k*Tsumm*ui->spinBox_11->value()*pow(10, 6));
    qreal Rpdr = (L0up+Ldop-t+6+ui->doubleSpinBox_7->value())/(Gprd*ui->spinBox_7->value()/100.0);
    ui->label_60->setText("Pпрд = " + QString::number(Rpdr) + " Вт");

    qreal Rprm = (L0dw+Ldop-t+6+ui->doubleSpinBox_7->value())/(Gprm*ui->spinBox_14->value()/100.0);
    ui->label_61->setText("Pпрд = " + QString::number(Rprm) + " Вт");

    QPen pen;
    pen.setColor(Qt::red);
    pen.setCapStyle(Qt::RoundCap);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);


    QLineSeries *line = new QLineSeries();
    line->append(0, -150);
    line->append(31, -150);
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

    qreal last = 0;
    last  = Rpdr * ui->spinBox_7->value()/100;// Lафт
    series_up->append(6, Rpdr * ui->spinBox_7->value()/100);
    last += Gprd;
    series_up->append(7, last);
    last = last - (L0up + Ldop);
    series_up->append(10, last);
    last += ui->spinBox_17->value();
    series_up->append(11, last);
    last -= ui->spinBox_17->value()*(1-ui->spinBox_22->value()/100.0);
    series_up->append(14, last);
    last += 90; //усиление ретрансялтора
    series_up->append(17, last);
    last -= ui->spinBox_18->value()*(1-ui->spinBox_22->value()/100.0);
    series_up->append(20, last);
    last += ui->spinBox_18->value();
    series_up->append(21, last);
    last -= (L0dw + Ldop);
    series_up->append(24, last);
    last += Gprm;
    series_up->append(25, last);
    last -= Gprm * (1-(ui->spinBox_14->value()/100.0));
    series_up->append(28, last);
    last += Rprm;
    series_up->append(31, last);

    QChart *up = new QChart();
    up->setTitle("График уровней сигнала спутниковой линии связи");
    up->addSeries(line);
    up->addSeries(series_up);
    up->createDefaultAxes();

    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(15);
    axisX->setRange(0, 31);
    axisX->setLabelsVisible(false);
    up->setAxisX(axisX, series_up);
    up->setAxisX(axisX, line);


    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-200, 200);
    axisY->setTickCount(17);
    up->setAxisY(axisY, series_up);
    up->setAxisY(axisY, line);



    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("График");
    QVBoxLayout *layout = new QVBoxLayout();
    QChartView *view;
    view = new QChartView(up);
    view->chart()->setAnimationOptions(QChart::AllAnimations);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    layout->addWidget(view);
    dlg->setLayout(layout);
    dlg->resize(1000, 500);
    dlg->show();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0) {
        ui->doubleSpinBox->setValue(0);
        ui->doubleSpinBox_2->setValue(0);
        ui->doubleSpinBox_3->setValue(0);
        ui->doubleSpinBox_4->setValue(0);
        ui->doubleSpinBox_5->setValue(0);
        ui->doubleSpinBox_6->setValue(0);
        ui->spinBox->setValue(1);
        ui->spinBox_2->setValue(1);
        ui->doubleSpinBox_7->setValue(1);
        ui->spinBox_7->setValue(1);
        ui->spinBox_14->setValue(1);
        ui->spinBox_11->setValue(1);
        ui->spinBox_3->setValue(1);
        ui->spinBox_10->setValue(1);
        ui->spinBox_20->setValue(1);
        ui->spinBox_21->setValue(1);
        ui->spinBox_22->setValue(1);
        return;
    }
    if(index == 1) {
        ui->doubleSpinBox->setValue(51.0);
        ui->doubleSpinBox_2->setValue(45.0);
        ui->doubleSpinBox_3->setValue(79.0);
        ui->doubleSpinBox_4->setValue(43.0);
        ui->doubleSpinBox_5->setValue(79.0);
        ui->doubleSpinBox_6->setValue(45.0);
        ui->spinBox->setValue(12);
        ui->spinBox_2->setValue(13);
        ui->doubleSpinBox_7->setValue(15.5);
        ui->spinBox_7->setValue(92);
        ui->spinBox_14->setValue(92);
        ui->spinBox_17->setValue(36);
        ui->spinBox_18->setValue(31);
        ui->spinBox_11->setValue(38);
        ui->spinBox_3->setValue(12);
        ui->spinBox_10->setValue(14);
        ui->spinBox_20->setValue(7);
        ui->spinBox_21->setValue(47);
        ui->spinBox_22->setValue(89);
        return;
    }
    if(index == 2) {
        ui->doubleSpinBox->setValue(50.0);
        ui->doubleSpinBox_2->setValue(46.0);
        ui->doubleSpinBox_3->setValue(75.0);
        ui->doubleSpinBox_4->setValue(46.0);
        ui->doubleSpinBox_5->setValue(75.0);
        ui->doubleSpinBox_6->setValue(44.0);
        ui->spinBox->setValue(14);
        ui->spinBox_2->setValue(11);
        ui->doubleSpinBox_7->setValue(17.1);
        ui->spinBox_7->setValue(91);
        ui->spinBox_14->setValue(87);
        ui->spinBox_17->setValue(34);
        ui->spinBox_18->setValue(32);
        ui->spinBox_11->setValue(35);
        ui->spinBox_3->setValue(9);
        ui->spinBox_10->setValue(5);
        ui->spinBox_20->setValue(6);
        ui->spinBox_21->setValue(53);
        ui->spinBox_22->setValue(91);
        return;
    }
    if(index == 3) {
        ui->doubleSpinBox->setValue(53.0);
        ui->doubleSpinBox_2->setValue(48.0);
        ui->doubleSpinBox_3->setValue(77.0);
        ui->doubleSpinBox_4->setValue(41.0);
        ui->doubleSpinBox_5->setValue(77.0);
        ui->doubleSpinBox_6->setValue(48.0);
        ui->spinBox->setValue(15);
        ui->spinBox_2->setValue(13);
        ui->doubleSpinBox_7->setValue(15.00);
        ui->spinBox_7->setValue(94);
        ui->spinBox_14->setValue(84);
        ui->spinBox_17->setValue(32);
        ui->spinBox_18->setValue(35);
        ui->spinBox_11->setValue(37);
        ui->spinBox_3->setValue(11);
        ui->spinBox_10->setValue(6);
        ui->spinBox_20->setValue(8);
        ui->spinBox_21->setValue(49);
        ui->spinBox_22->setValue(95);
        return;
    }
}
