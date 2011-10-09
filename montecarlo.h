#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <QDialog>
#include <QGraphicsScene>
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
public slots:
    void prepare_for_pi();
    void prepare_for_integrate();
    void prepare_for_plot();
private:
    Ui::MonteCarlo *ui;
    QwtPlot *plot1;
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

qreal circle(const qreal& x);
qreal g(const qreal& x);
#endif // MONTECARLO_H
