#ifndef TASK4_H
#define TASK4_H

#include <QDialog>
#include <qwt_plot.h>
#include <qwt_plot_histogram.h>

namespace Ui {
    class Task4;
}

class Task4 : public QDialog
{
    Q_OBJECT

public:
    explicit Task4(QWidget *parent = 0);
    ~Task4();

private:
    Ui::Task4 *ui;
    QwtPlot *plot;
    QwtPlotHistogram *hist;
private slots:
    void process();
};

#endif // TASK4_H
