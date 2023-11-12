#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setInteraction(QCP::iRangeZoom, true);
    ui->widget->setInteraction(QCP::iRangeDrag, true);

    ui->spinBox->setRange(0,1000000);

    QSlider *slider_v = ui->horizontalSlider;
    QSlider *slider_mu = ui->horizontalSlider_2;
    QSlider *slider_lambda = ui->horizontalSlider_3;
    slider_v->setMinimum(6);
    slider_v->setMaximum(100);
    slider_v->setSingleStep(0);
    slider_v->setPageStep(10);
    slider_v->setValue(40);

    slider_mu->setMinimum(-100);
    slider_mu->setMaximum(100);
    slider_mu->setSingleStep(0);
    slider_mu->setPageStep(10);

    slider_lambda->setMinimum(1);
    slider_lambda->setMaximum(100);
    slider_lambda->setSingleStep(0);
    slider_lambda->setPageStep(10);
    slider_lambda->setValue(10);


    slider_v = ui->horizontalSlider_4;
    slider_mu = ui->horizontalSlider_5;
    slider_lambda = ui->horizontalSlider_6;
    slider_v->setMinimum(6);
    slider_v->setMaximum(100);
    slider_v->setSingleStep(0);
    slider_v->setPageStep(10);
    slider_v->setValue(40);

    slider_mu->setMinimum(-100);
    slider_mu->setMaximum(100);
    slider_mu->setSingleStep(0);
    slider_mu->setPageStep(10);

    slider_lambda->setMinimum(1);
    slider_lambda->setMaximum(100);
    slider_lambda->setSingleStep(0);
    slider_lambda->setPageStep(10);
    slider_lambda->setValue(10);

    ui->label->setText("v = " + QString::number(ui->horizontalSlider->value() / 10.0));
    ui->label_2->setText("mu = " + QString::number(ui->horizontalSlider_2->value() / 10.0));
    ui->label_3->setText("lambda = " + QString::number(ui->horizontalSlider_3->value() / 10.0));
    ui->label_4->setText("v = " + QString::number(ui->horizontalSlider_4->value() / 10.0));
    ui->label_5->setText("mu = " + QString::number(ui->horizontalSlider_5->value() / 10.0));
    ui->label_6->setText("lambda = " + QString::number(ui->horizontalSlider_6->value() / 10.0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawing_a_graph(std::function<double(double x)> func, double EPS, Qt::GlobalColor color)
{
    QVector<double> X, Y;
    double h = 0.0001;
    for(double x = -20; x <= 20 + h; x += h)
    {
        double y = func(x);
        if(y < EPS)
            continue;
        X.push_back(x);
        Y.push_back(y);
    }
    auto graph = ui->widget->addGraph();
    graph->addData(X, Y);
    graph->rescaleAxes();
    QPen pen;
    pen.setColor(color);
    graph->setPen(pen);
}

void MainWindow::drawing_a_points(QVector<double> &X, QVector<double> &Y, QColor color)
{
    QCPScatterStyle scatterStyle;
    scatterStyle.setShape(QCPScatterStyle::ssCircle);
    scatterStyle.setSize(2.5);
    scatterStyle.setBrush(color);
    scatterStyle.setPen(QPen(color));
    QCPGraph *graph = ui->widget->addGraph();
    graph->addData(X,Y);
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(scatterStyle);
    Point_graphs.push_back(graph);
}

void MainWindow::clear_point_graphs()
{
    for(auto graph: Point_graphs)
    {
        ui->widget->removeGraph(graph);
        delete graph;
    }
    Point_graphs.clear();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label->setText("v = " + QString::number(value / 10.0));
}


void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_2->setText("mu = " + QString::number(value / 10.0));
}


void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_3->setText("lambda = " + QString::number(value / 10.0));
}


void MainWindow::on_pushButton_clicked()
{
   const int N = ui->spinBox->value();
    double EPS_theory = 0.01;
    double EPS_emp = 1e-10;
   QVector<double> X, Y;
   ui->widget->clearGraphs();

   if(ui->radioButton->isChecked())
   {
        statistics::Parameters par{ui->horizontalSlider->value() / 10.0, ui->horizontalSlider_2->value() / 10.0, ui->horizontalSlider_3->value() / 10.0};
        statistics::MainDistribution obj(par);
        drawing_a_graph([&obj](double x) {return obj.get_density_distribution(x);}, EPS_theory, Qt::cyan);
        if(ui->checkBox->isChecked())
        {
            statistics::EmpericalDistribution emperical(N, obj);
            auto stdX = emperical.get_X();
            X =  QVector<double>(stdX.begin(), stdX.end());
            for(auto x: X)
                Y.push_back(emperical.get_density_distribution(x));
            drawing_a_graph([&emperical](double x){return emperical.get_density_distribution(x);}, EPS_emp, Qt::green);
            ui->widget->graph(ui->widget->graphCount() - 2)->rescaleAxes(true);
            if(ui->checkBox_2->isChecked())
            {
                statistics::EmpericalDistribution emperical_new(N, emperical);
                drawing_a_graph([&emperical_new](double x){return emperical_new.get_density_distribution(x);}, EPS_emp, Qt::red);
                ui->widget->graph(ui->widget->graphCount() - 2)->rescaleAxes(true);
                ui->widget->graph(ui->widget->graphCount() - 3)->rescaleAxes(true);
            }
        }
        drawing_a_points(X, Y, Qt::darkGreen);
        Y.clear();
        if(X.empty())
            for(int i = 0; i < N; i++)
                X.push_back(obj.generate_random_variable());
        for(auto x: X)
            Y.push_back(obj.get_density_distribution(x));
        drawing_a_points(X, Y, Qt::blue);
        ui->widget->replot();
   }

   if(ui->radioButton_2->isChecked())
   {
        statistics::Parameters par_1{ui->horizontalSlider->value() / 10.0, ui->horizontalSlider_2->value() / 10.0, ui->horizontalSlider_3->value() / 10.0};
        statistics::Parameters par_2{ui->horizontalSlider_4->value() / 10.0, ui->horizontalSlider_5->value() / 10.0, ui->horizontalSlider_6->value() / 10.0};
        statistics::MainDistribution main_1(par_1);
        statistics::MainDistribution main_2(par_2);
        statistics::MixtureDistribution mixture(main_1, main_2, 0.5);
        drawing_a_graph([&mixture](double x) {return mixture.get_density_distribution(x);}, EPS_theory, Qt::cyan);
        if(ui->checkBox->isChecked())
        {
            statistics::EmpericalDistribution emperical(N, mixture);
            auto stdX = emperical.get_X();
            X =  QVector<double>(stdX.begin(), stdX.end());
            for(auto x: X)
                Y.push_back(emperical.get_density_distribution(x));
            drawing_a_graph([&emperical](double x){return emperical.get_density_distribution(x);}, EPS_emp, Qt::green);
            ui->widget->graph(ui->widget->graphCount() - 2)->rescaleAxes(true);
            if(ui->checkBox_2->isChecked())
            {
                statistics::EmpericalDistribution emperical_new(N, emperical);
                drawing_a_graph([&emperical_new](double x){return emperical_new.get_density_distribution(x);}, EPS_emp, Qt::red);
                ui->widget->graph(ui->widget->graphCount() - 2)->rescaleAxes(true);
                ui->widget->graph(ui->widget->graphCount() - 3)->rescaleAxes(true);
            }
        }
        drawing_a_points(X, Y, Qt::darkGreen);
        Y.clear();
        if(X.empty())
            for(int i = 0; i < N; i++)
                X.push_back(mixture.generate_random_variable());
        for(auto x: X)
            Y.push_back(mixture.get_density_distribution(x));
        drawing_a_points(X, Y, Qt::blue);
        ui->widget->replot();
   }

}


void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->label_4->setText("v = " + QString::number(value / 10.0));
}


void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    ui->label_5->setText("mu = " + QString::number(value / 10.0));
}


void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    ui->label_6->setText("lambda = " + QString::number(value / 10.0));
}
