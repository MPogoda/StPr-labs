#include "task4.h"
#include "ui_task4.h"
#include <QDateTime>

static const double chi[][25] = {
    {
        0.0002, 0.0201, 0.1148,  0.2971,  0.5543,
        0.8721, 1.2390, 1.6465,  2.0879,  2.5582,
        3.0535, 3.5706, 4.1069,  4.6604,  5.2293,
        5.8122, 6.4078, 7.0149,  7.6327,  8.2604,
        8.8972, 9.5425, 10.1957, 10.8564, 11.5240
    },
    {
        0.0010,  0.0506,  0.2158,  0.4844,  0.8312,
        1.2373,  1.6899,  2.1797,  2.7004,  3.2470,
        3.8157,  4.4038,  5.0088,  5.6287,  6.2621,
        6.9077,  7.5642,  8.2307,  8.9065,  9.5908,
        10.2829, 10.9823, 11.6886, 12.4012, 13.1197
    },
    {
        0.0039, 0.1026, 0.3518, 0.7107, 1.1455,
        1.6354, 2.1676, 2.7326, 3.3251, 3.9403,
        4.5748, 5.2260, 5.8919, 6.5706, 7.2609,
        7.9616, 8.6718, 9.3905, 10.1170, 10.8508,
        11.5913, 12.3380, 13.0905, 13.8484, 14.6114
    },
    {
        0.0158, 0.2107, 0.5844, 1.0636, 1.6103,
        2.2041, 2.8331, 3.4895,  4.1682, 4.8652,
        5.5778, 6.3038, 7.0415, 7.7895, 8.5468,
        9.3122, 10.0852, 10.8649, 11.6509, 12.4426,
        13.2396, 14.0415, 14.8480, 15.6587, 16.4734
    },
    {
        0.0642, 0.4463, 1.0052, 1.6488, 2.3425,
        3.0701, 3.8223, 4.5936, 5.3801, 6.1791,
        6.9887, 7.8073, 8.6339, 9.4673, 10.3070,
        11.1521, 12.0023, 12.8570, 13.7158, 14.5784,
        15.4446, 16.3140, 17.1865, 18.0618, 18.9398
    },
    {
        2.1485, 2.7133, 3.4237, 4.1947, 4.9999,
        5.8276, 6.6713, 7.5274, 8.3933, 9.2672,
        10.1479, 11.0343, 11.9257, 12.8215, 13.7212,
        14.6243, 15.5307, 16.4399, 17.3517, 24.2659,
        25.1823, 26.1007, 37.0211, 39.9432, 40.8670
    }
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
                            .arg(chi[5][dx-4])
                            .arg(xi_emp));
    if (chi[5][dx-4] > xi_emp)
        ui->textBrowser->append(QString::fromUtf8(
            "Нет основание опровергать гипотезу о равномерности распределения"));
    else
        ui->textBrowser->append(QString::fromUtf8(
            "Гипотеза не верна"));
}
