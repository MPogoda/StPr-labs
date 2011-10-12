
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
    void process();
    void process_plot();
};
#endif // MONTECARLO_H
