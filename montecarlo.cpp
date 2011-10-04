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
    ui->horizontalSlider_2->setValue(static_cast<int>(value));
}

void MonteCarlo::aslide_changed(const int &value)
{
    ui->a->setValue(static_cast<double>(value));
}

void MonteCarlo::b_changed(const double &value)
{
    ui->horizontalSlider->setValue(static_cast<int>(value));
}

void MonteCarlo::bslide_changed(const int &value)
{
    ui->b->setValue(static_cast<double>(value));
}

void MonteCarlo::k_changed(const double &value)
{
    ui->horizontalSlider_4->setValue(static_cast<int>(value));
}

void MonteCarlo::kslide_changed(const int &value)
{
    ui->k->setValue(static_cast<double>(value));
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

qreal circle(const qreal &x)
{
    return (qSqrt(1 - x*x));
}

qreal g(const qreal &x)
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
    qreal (*f)(const qreal&);
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

    const qreal &minx = ui->a->value();
    const qreal &maxx = ui->b->value();
    const qreal &k = ui->k->value();
    const qreal miny = (qAbs(minx - maxx) < qMax(minx, maxx)) ? k * f(qMin(qAbs(minx), qAbs(maxx))) : 0;
    const qreal maxy = k * qMax(f(minx), f(maxx));
    const quint8 nexp = ui->nexp->value();
    const quint8 zz = (calculate_pi) ? 4 : 1;
    const qreal area = (maxx - minx) * (maxy - miny);
    const qreal real_answer = zz * k * F(maxx) - F(minx);
    const uint npoints = ui->npoints->value();
    const qreal error = zz * qSqrt(real_answer * (area - real_answer) / npoints);


    qreal x, y;
    qreal sum = 0.0;

    qsrand(QDateTime::currentDateTime().toTime_t());

    for (quint8 j = 1; j <= nexp; ++j)
    {
        uint captured = 0;
        for (uint i = npoints; i; --i)
        {
            x = minx + (maxx - minx) * (static_cast<double>(qrand()) / RAND_MAX);
            y = miny + (maxy - miny) * (static_cast<double>(qrand()) / RAND_MAX);
            if (y <= (k * f(x)))
                ++captured;
        }

        qreal answer = static_cast<double>(zz * captured) * area / npoints;
        qreal emp_error = qAbs(real_answer - answer);
        sum += answer;

        ui->textBrowser->append(QString::fromUtf8(
            "Опыт № %1. Полученное значение: %2, отклонение: %3")
                            .arg(j)
                            .arg(answer, 0, 'f', 3)
                            .arg(emp_error, 0, 'f', 3));
    }

    sum /= nexp;

    ui->textBrowser->append(QString::fromUtf8(
        "Среднее значение: %1, среднее отклонение: %2, ожидаемое теоретическое отклонение: %4")
                        .arg(sum, 0, 'f', 3)
                        .arg(qAbs(real_answer - sum), 0, 'f', 3)
                        .arg(error, 0, 'f', 3));
}
