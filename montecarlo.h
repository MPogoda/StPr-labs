
#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <QDialog>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace Ui {
    class MonteCarlo;
}

class MonteCarlo : public QDialog
{
    Q_OBJECT

public:
    explicit MonteCarlo(QWidget *parent = 0);
    ~MonteCarlo();
    void calculate_pi();
    void calculate_integral();
    void plot();
private:
    Ui::MonteCarlo  *ui;
    QwtPlot         *plot1;
    QwtPlotCurve    *curve1;
    QwtPlotCurve    *curve2;
private slots:
    void bslide_changed(const int& value);
    void aslide_changed(const int& value);
    void a_changed(const double& value);
    void b_changed(const double& value);
    void kslide_changed(const int& value);
    void k_changed(const double& value);
    void process();
    void process_plot();
};
#endif // MONTECARLO_H
