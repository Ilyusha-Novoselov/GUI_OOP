#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <functional>
#include <QDebug>

#include "qcustomplot.h"
#include "MathPart/emperical_distribution.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawing_a_graph(std::function<double(double x)> func, double EPS, Qt::GlobalColor color);

    void drawing_a_points(QVector<double>& X, QVector<double>& Y, QColor color);

    void clear_point_graphs();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_pushButton_clicked();

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

    void on_horizontalSlider_6_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QVector<QCPGraph*> Point_graphs;
};
#endif // MAINWINDOW_H
