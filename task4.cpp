#include "task4.h"
#include "ui_task4.h"
#include <QDateTime>

static const double chi[] = {
    3.8415, 5.9915, 7.8147, 9.4877, 11.0705,
    12.5916, 14.0671, 15.5073, 16.9190, 18.3070,
    19.6751, 21.0261, 22.3620, 23.6848, 24.9958,
    26.2962, 27.5871, 28.8693, 30.1435, 31.4104,
    32.6706, 33.9244, 35.1725, 36.4150, 37.6525
};


Task4::Task4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task4),
    plot(new QwtPlot(this)),
    hist(new QwtPlotHistogram("Hits"))
{
    ui->setupUi(this);
    hist->attach(plot);
    ui->verticalLayout_2->addWidget(plot);

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(process()));
}

Task4::~Task4()
{
    delete hist;
    delete plot;
    delete ui;
}

void Task4::process()
{
    const uint nexp = ui->nexp->value();
    const quint8 dx = qFloor(3.32 * log10(nexp)) + 1;
    QVector<qreal> experiments;

    srand(QDateTime::currentDateTime().toTime_t());

    for (uint i = nexp; i; --i)
        experiments << 1.0 * rand() / RAND_MAX;

    qSort(experiments);

    uint j = 0;
    uint captured[dx];
    memset(captured, 0, dx * sizeof(uint));
    foreach (const qreal& v, experiments)
    {
        if (v >= (j + 1.0)/dx)
            ++j;
        ++captured[j];
    }

    QVector<QwtIntervalSample> samples;
    for (j = 0; j < dx; ++j)
    {
        ui->textBrowser->append(
            QString::fromUtf8("Интервал [%2/%1; %3/%1]: %4 значений")
                           .arg(dx)
                           .arg(j)
                           .arg(j+1)
                           .arg(captured[j]));
        samples << QwtIntervalSample(captured[j], 1.0 * j / dx, (j+1.0)/dx);
    }
    hist->setSamples(samples);
    plot->replot();

    qreal xi_emp = 0.0;
    qreal expected = 1.0 * nexp / dx;
    for (uint j = 0; j < dx; ++j)
        xi_emp += (captured[j] - expected) * (captured[j] - expected) / expected;
    ui->textBrowser->append(QString::fromUtf8(
        "Хи теоретическое: %1. Хи эмперическое: %2.")
                            .arg(chi[dx-4])
                            .arg(xi_emp));
    if (chi[dx-4] > xi_emp)
        ui->textBrowser->append(QString::fromUtf8(
            "Нет основание опровергать гипотезу о равномерности распределения"));
    else
        ui->textBrowser->append(QString::fromUtf8(
            "Гипотеза не верна"));
}
