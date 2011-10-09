#include "montecarlo.h"
#include "ui_montecarlo.h"
#include <qmath.h>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QDebug>


MonteCarlo::MonteCarlo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonteCarlo)
{
    ui->setupUi(this);
    plot1 = new QwtPlot(this);
    ui->verticalLayout_8->addWidget(plot1);
    connect(ui->a, SIGNAL(valueChanged(double)),
            this, SLOT(a_changed(double)));
    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)),
            this, SLOT(aslide_changed(int)));
    connect(ui->b, SIGNAL(valueChanged(double)),
            this, SLOT(b_changed(double)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
            this, SLOT(bslide_changed(int)));
    connect(ui->k, SIGNAL(valueChanged(double)),
            this, SLOT(k_changed(double)));
    connect(ui->horizontalSlider_4, SIGNAL(valueChanged(int)),
            this, SLOT(kslide_changed(int)));
    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(process()));
    connect(ui->pushButton_3, SIGNAL(clicked()),
            this, SLOT(process_plot()));
}

MonteCarlo::~MonteCarlo()
{
    delete ui;
    delete plot1;
}

void MonteCarlo::a_changed(const double &value)
{
    ui->horizontalSlider_2->setValue(value);
}

void MonteCarlo::aslide_changed(const int &value)
{
    ui->a->setValue(value);
}

void MonteCarlo::b_changed(const double &value)
{
    ui->horizontalSlider->setValue(value);
}

void MonteCarlo::bslide_changed(const int &value)
{
    ui->b->setValue(value);
}

void MonteCarlo::k_changed(const double &value)
{
    ui->horizontalSlider_4->setValue(value);
}

void MonteCarlo::kslide_changed(const int &value)
{
    ui->k->setValue(value);
}

void MonteCarlo::prepare_for_pi()
{
    ui->pi_lbl->show();
    ui->int_groupBox->hide();
    ui->a->setValue(0);
    ui->b->setValue(1);
    ui->k->setValue(1);
    ui->textBrowser->clear();
    ui->textBrowser->show();
    ui->pushButton->show();
    ui->pushButton_3->hide();
    ui->groupBox_5->show();
    plot1->hide();
}

void MonteCarlo::prepare_for_integrate()
{
    ui->pi_lbl->hide();
    ui->int_groupBox->show();
    ui->textBrowser->clear();
    ui->textBrowser->show();
    ui->pushButton_3->hide();
    ui->pushButton->show();
    ui->groupBox_5->show();
    plot1->hide();
}

void MonteCarlo::prepare_for_plot()
{
    ui->pi_lbl->hide();
    ui->int_groupBox->hide();
    ui->textBrowser->hide();
    ui->pushButton->hide();
    ui->pushButton_3->show();
    ui->groupBox_5->hide();
    plot1->show();
}

uint montecarlo(qreal (*f)(qreal), quint8 k, uint number,
                 const qreal& minx, const qreal& dx,
                 const qreal& miny, const qreal& dy )
{
    uint captured = 0;
    qreal x, y;
    for (; number; --number)
    {
        x = minx + dx * (static_cast<double>(qrand()) / RAND_MAX);
        y = miny + dy * (static_cast<double>(qrand()) / RAND_MAX);
        if (y <= (k * f(x)))
            ++captured;
    }
    return captured;
}

qreal circle(qreal x)
{
    return (qSqrt(1 - x*x));
}

qreal g(qreal x)
{
    return (x * x);
}

qreal G(qreal x)
{
    return x * x * x / 3.0;
}

void MonteCarlo::process()
{
    bool calculate_pi = ui->pi_lbl->isVisible();

    qreal (*f)(qreal);
    qreal (*F)(qreal);
    if (calculate_pi)
    {
        f = &circle;
        F = &qAtan;
    }
    else
    {
        F = &G;
        f = &g;
    }

    // k * x^2
    const quint8 k = ui->k->value();
    // left edge of area
    const qreal  minx = qMin(ui->a->value(), ui->b->value());
    // width of area
    const qreal  dx   = qMax(ui->a->value(), ui->b->value()) - minx;
    // bottom edge of area
    const qreal  miny = (dx < qMax(minx, minx + dx)) ? k * f(qMin(qAbs(minx), qAbs(minx + dx))) : 0;
    // height of area
    const qreal  dy = k * qMax(f(minx), f(minx + dx)) - miny;
    // number of experiments
    const quint8 nexp = ui->nexp->value();
    // number of points to be thrown
    const uint   npoints = ui->npoints->value();
    // if we're calculating PI, we have to multiply all result by 4
    const quint8 zz = (calculate_pi) ? 4 : 1;
    // total area where we'll throw points
    const qreal  area = dx * dy;
    // real answer (cheat (: )
    const qreal  real_answer = zz * k * (F(minx + dx) - F(minx));
    // theoretical error
    const qreal  error = qSqrt(real_answer * (zz * area - real_answer) / npoints);

    // used for mean values in number of experiments
    qreal sum_answer = 0.0;
    qreal sum_error  = 0.0;

    qsrand(QDateTime::currentDateTime().toTime_t());

    for (quint8 j = 1; j <= nexp; ++j)
    {
        uint captured = montecarlo(f, k, npoints, minx, dx, miny, dy);

        qreal answer = static_cast<double>(zz * captured) * area / npoints;
        qreal emp_error = qAbs(real_answer - answer);
        sum_answer += answer;
        sum_error  += emp_error;

        ui->textBrowser->append(QString::fromUtf8(
            "Опыт № %1. Полученное значение: %2, отклонение: %3")
                            .arg(j)
                            .arg(answer, 0, 'f', 3)
                            .arg(emp_error, 0, 'f', 3));
    }

    sum_error /= nexp;
    sum_answer /= nexp;

    ui->textBrowser->append(QString::fromUtf8(
        "Среднее значение: %1, среднее отклонение: %2, ожидаемое теоретическое отклонение: %4")
                        .arg(sum_answer, 0, 'f', 3)
                        .arg(sum_error, 0, 'f', 3)
                        .arg(error, 0, 'f', 3));

}

void MonteCarlo::process_plot()
{
    plot1->setAutoReplot();
    const uint nmax = 300000;
    const uint nmin = 50000;
    const uint nexp = ui->nexp->value();

    QVector<qreal> th, em, se;
    const qreal real_answer   = qAtan(1);
    const qreal error_prepare = qSqrt(real_answer * (1 - real_answer));
    uint captured;
    qreal error;

    qsrand(QDateTime::currentDateTime().toTime_t());

    for (uint i = nmin; i <= nmax; i += 1000)
    {
        error  = 0.0;
        for (uint j = nexp; j; --j)
        {
            captured = montecarlo(&circle, 1, i,0, 1, 0, 1);
            error += qAbs(real_answer - (static_cast<double>(captured) / i));
        }
        error /= nexp;
        th << error_prepare / qSqrt(i);
        em << error;
        se << i;
    }

    QwtPlotCurve *curve1 = new QwtPlotCurve("theory");
    QwtPlotCurve *curve2 = new QwtPlotCurve("reality");
    curve2->attach(plot1);
    curve1->attach(plot1);
    curve2->setPen(QPen(Qt::cyan));
    curve1->setPen(QPen(Qt::red));
    curve1->setSamples(se, th);
    curve2->setSamples(se, em);
}
