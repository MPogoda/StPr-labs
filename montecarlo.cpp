#include "montecarlo.h"
#include "ui_montecarlo.h"
#include <qmath.h>
#include <QString>
#include <QDateTime>
#include <QDebug>

MonteCarlo::MonteCarlo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonteCarlo)
{
    ui->setupUi(this);
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
}

MonteCarlo::~MonteCarlo()
{
    delete ui;
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
    ui->int_frame->hide();
    ui->a->setValue(0);
    ui->b->setValue(1);
    ui->k->setValue(1);
    ui->textBrowser->clear();
}

void MonteCarlo::prepare_for_integrate()
{
    ui->pi_lbl->hide();
    ui->int_frame->show();
    ui->textBrowser->clear();
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
    const qreal  real_answer = zz * k * F(minx + dx) - F(minx);
    // theoretical error
    const qreal  error = zz * qSqrt(real_answer * (area - real_answer) / npoints);

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
