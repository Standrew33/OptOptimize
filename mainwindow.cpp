#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->RChart->xAxis->setLabel("Time (min)");
    ui->RChart->yAxis->setLabel("Passenger (person/min)");
    ui->RChart->xAxis->setRange(0, 1080);
    ui->RChart->yAxis->setRange(0, 65);
    ui->RChart->legend->setVisible(true);
    ui->RChart->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignRight|Qt::AlignTop);

    ui->RChart2->xAxis->setLabel("Iteration");
    ui->RChart2->yAxis->setLabel("Fitness value");
    ui->RChart2->xAxis->setRange(1, 100);
    ui->RChart2->yAxis->setRange(300000, 900000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->RPBar->setValue(0);
    ui->RPBar->setMaximum(ui->SSize->value());

    Population object;
    ui->SSLabel->setText("Инициализация...");
    repaint();
    object.RSExcel();
    int RBus = qrand() % 8 + 5;

    ui->SSLabel->setText("Рассчет...");
    repaint();
    QVector<double> x1(100), y1(100);
    for (int i = 0; i <= ui->SIter->value() - 1; i++)
    {
        object.RGeneric(ui->SSize->value());
        if (i > 0)
        {
            int Num = 0;
            do
            {
                /*if (Num = 10)
                {
                    object.Mutation(0.05 * ui->SSize->value() * 10);
                    Num = 0;
                }*/
                object.RFitness(RBus);
                object.RFindMax();
                y1[i] = object.RFindMin() * 10000;
                Num += 1;
            }while (y1[i - 1] < y1[i]);
        }
        else
        {
            object.RFitness(RBus);
            object.RFindMax();
            y1[i] = object.RFindMin() * 10000;
        }

        object.Selection();
        object.Crossing(ui->SProbCross->value() * ui->SSize->value());
        object.Mutation(ui->SProbMut->value() * ui->SSize->value() * 10);
        ui->RPBar->setValue(i + 1);
        x1[i] = i + 1;
        qApp->processEvents();
    }
     ui->SSLabel->setText("");

     ui->RChart->clearGraphs();
     ui->RChart2->clearGraphs();
     y1.reserve(100);
     for (int j = 0; j < 6; j++)
     {
         QVector<double> x(1080), y(1080);
         for (int i = 0; i < 1080; i++)
         {
              y[i] = object.RListRate[j][i];
              x[i] = i + 1;
         }
         ui->RChart->addGraph();
         ui->RChart->graph(j)->setData(x, y);
     }
     ui->RChart->graph(1)->setPen(QPen(QColor(Qt::darkYellow)));
     ui->RChart->graph(2)->setPen(QPen(QColor(Qt::red)));
     ui->RChart->graph(3)->setPen(QPen(QColor(Qt::darkRed)));
     ui->RChart->graph(4)->setPen(QPen(QColor(Qt::darkGreen)));
     ui->RChart->graph(5)->setPen(QPen(QColor(Qt::green)));

     ui->RChart->graph(0)->setName("St 1");
     ui->RChart->graph(1)->setName("St 2");
     ui->RChart->graph(2)->setName("St 3");
     ui->RChart->graph(3)->setName("St 4");
     ui->RChart->graph(4)->setName("St 5");
     ui->RChart->graph(5)->setName("St 6");

     ui->RChart2->addGraph();
     ui->RChart2->graph(0)->setData(x1, y1);

     ui->RChart->replot();
     ui->RChart2->replot();
}
