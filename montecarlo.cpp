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

void MonteCarlo::prepare_for_pi()
{
    ui->pi_lbl->show();
    ui->int_frame->hide();
    ui->a->setValue(0);
    ui->b->setValue(1);
}

void MonteCarlo::prepare_for_integrate()
{
    ui->pi_lbl->hide();
    ui->int_frame->show();
}

qreal circle(const qreal &x)
{
    return (qSqrt(1 - x*x));
}

qreal g(const qreal &x)
{
    return (3*x*x);
}

qreal G(qreal x)
{
    return x * x * x;
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
    const qreal miny = (qAbs(minx - maxx) < qMax(minx, maxx)) ? f(qMin(qAbs(minx), qAbs(maxx))) : 0;
    const qreal maxy = qMax(f(minx), f(maxx));
    qreal x, y;
    const uint npoints = ui->npoints->value();
    uint captured = 0;

    qsrand(QDateTime::currentDateTime().toTime_t());

    for (uint i = npoints; i; --i)
    {
        x = minx + (maxx - minx) * (static_cast<double>(qrand()) / RAND_MAX);
        y = miny + (maxy - miny) * (static_cast<double>(qrand()) / RAND_MAX);
        if (y <= f(x))
            ++captured;
    }

    const qreal area = (maxx - minx) * (maxy - miny);
    qreal answer = static_cast<double>(captured) * area / npoints;
    const qreal real_answer = F(maxx) - F(minx);
    qreal emp_error = qAbs(real_answer - answer);
    qreal error = qSqrt(real_answer * (area - real_answer) / npoints);
    if (calculate_pi)
    {
        answer      *= 4.0;
        emp_error   *= 4.0;
        error       *= 4.0;
    }

    ui->textBrowser->append(QString::fromUtf8("Полученное значение: %1, отклонение: %2, теоретическое отклонение: %3")
                            .arg(answer, 0, 'f', 3)
                            .arg(emp_error, 0, 'f', 3)
                            .arg(error, 0, 'f', 3));
}
