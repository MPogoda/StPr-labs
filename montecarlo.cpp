#include "montecarlo.h"
#include "ui_montecarlo.h"
#include <qmath.h>
#include <QString>
#include <QDateTime>

MonteCarlo::MonteCarlo(QWidget *parent) :
    QDialog(parent),
    plot1(NULL),
    ui(new Ui::MonteCarlo)
{
    ui->setupUi(this);
}

MonteCarlo::~MonteCarlo()
{
    delete ui;
    if (plot1 != NULL)
    {
        delete curve1;
        delete curve2;
        delete plot1;
    }
}

void MonteCarlo::calculate_pi()
{
    ui->int_groupBox->hide();
    ui->k->setValue(1.0);

    ui->pushButton_3->hide();
    ui->npoints_min_gb->hide();

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(process()));
    exec();
}

void MonteCarlo::calculate_integral()
{
    ui->pi_lbl->hide();
    ui->pushButton_3->hide();
    ui->npoints_min_gb->hide();

    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(process()));

    exec();
}

void MonteCarlo::plot()
{
    ui->pi_lbl->hide();
    ui->int_groupBox->hide();
    ui->textBrowser->hide();
    ui->pushButton->hide();
    ui->npoints->setMaximum(250000);

    plot1   = new QwtPlot(this);
    curve1  = new QwtPlotCurve("Theory");
    curve2  = new QwtPlotCurve("Real");
    curve1->attach(plot1);
    curve2->attach(plot1);
    curve1->setPen(QPen(Qt::red));
    curve2->setPen(QPen(Qt::blue));
    ui->verticalLayout_8->addWidget(plot1);

    connect(ui->pushButton_3, SIGNAL(clicked()),
            this, SLOT(process_plot()));

    exec();
}

quint32 montecarlo(bool (*f)(const qreal&, const qreal&),
                   const qreal& k, quint32 number,
                   const qreal& minx, const qreal& dx,
                   const qreal& miny, const qreal& dy )
{
    quint32 captured(0);
    qreal x, y;
    for (; number; --number)
    {
        x = minx + dx * rand() / RAND_MAX;
        y = (miny + dy * rand() / RAND_MAX) / k;
        if (f(x, y))
            ++captured;
    }
    return captured;
}

bool circle(const qreal& x, const qreal& y)
{
    return ((x*x + y*y) <= 1.0);
}

qreal Circle(const qreal& x)
{
    return (x) ? qAtan(x) : 0;
}

qreal sqr(const qreal& x)
{
    return x*x;
}

bool g(const qreal& x, const qreal& y)
{
    return (y < sqr(x));
}

qreal G(const qreal& x)
{
    return x * x * x / 3.0;
}

bool t(const qreal& _, const qreal& y)
{
    return (y < 0.5);
}

void MonteCarlo::process()
{
    bool calculate_pi = ui->pi_lbl->isVisible();
    bool (*f)(const qreal&, const qreal&);
    qreal (*F)(const qreal&);
    if (calculate_pi)
    {
        f = &circle;
        F = &Circle;
    }
    else
    {
        F = &G;
        f = &g;
    }

    // k * x^2
    const qreal     &k(         ui->k->value()                                  );
    // left edge of area
    const qreal     &minx(      ui->a->value()                                  );
    // width of area
    const qreal     dx(         ui->b->value() - minx                           );
    // bottom edge of area
    const qreal     miny(       ((minx < 0.0) && (dx > -minx)) ?
                                (k * sqr(qMin(qAbs(minx), qAbs(minx + dx)))) :
                                0.0                                             );
    // height of area
    const qreal     dy(         k * qMax(sqr(minx), sqr(minx + dx)) - miny      );
    // number of experiments
    const quint8    nexp(       ui->nexp->value()                               );
    // number of points to be thrown
    const quint32   npoints(    ui->npoints->value()                            );
    // if we're calculating PI, we have to multiply all result by 4
    const quint8    zz(         (calculate_pi) ? 4 : 1                          );
    // total area where we'll throw points
    const qreal     area(       dx * dy                                         );
    // real answer (cheat (: )
    const qreal     real_answer(zz * k * (F(minx + dx) - F(minx))               );
    // theoretical error
    const qreal     error(      qSqrt(real_answer *
                                      (zz * area - real_answer) / npoints)      );

    // used for mean values in number of experiments
    qreal sum_answer(   0.0);
    qreal sum_error(    0.0 );
    qreal answer, emp_error;
    uint captured;

    srand(QDateTime::currentDateTime().toTime_t());

    for (quint8 j(1); j <= nexp; ++j)
    {
        captured    = montecarlo(f, k, npoints, minx, dx, miny, dy);

        answer      = zz * captured * area / npoints;
        emp_error   = qAbs(real_answer - answer);
        sum_answer += answer;
        sum_error  += emp_error;

        ui->textBrowser->append(QString::fromUtf8(
            "Опыт № %1. Полученное значение: %2, отклонение: %3")
                            .arg(j)
                            .arg(answer, 0, 'f', 4)
                            .arg(emp_error, 0, 'f', 4));
    }

    sum_error   /= nexp;
    sum_answer  /= nexp;

    ui->textBrowser->append(QString::fromUtf8(
        "Среднее значение: %1, среднее отклонение: %2, ожидаемое теоретическое отклонение: %3")
                        .arg(sum_answer, 0, 'f', 4)
                        .arg(sum_error, 0, 'f', 4)
                        .arg(error, 0, 'f', 4));

}

void MonteCarlo::process_plot()
{
    const quint32 nmax( ui->npoints->value()    );
    const quint32 nmin( ui->npoints_min->value());
    const quint8  nexp( ui->nexp->value()       );
    const quint32 step( (nmax - nmin) / 200     );

    static double th[201], em[201], se[201];
    const qreal real_answer(0.5);
    const qreal error_prepare(qSqrt(real_answer * (1.0 - real_answer)));
    quint32 captured;
    qreal error;

    srand(QDateTime::currentDateTime().toTime_t());

    quint8 z(0);

    for (quint32 i = nmin; i <= nmax; i += step, ++z)
    {
        error  = 0.0;
        for (quint8 j = nexp; j; --j)
        {
            captured = montecarlo(&t, 1, i, 0, 1, 0, 1);
            error += qAbs(real_answer - 1.0 * captured / i);
        }
        error /= nexp;
        th[z] = error_prepare / qSqrt(i);
        em[z] = error;
        se[z] = i;
    }

    curve1->setSamples(se, th, 201);
    curve2->setSamples(se, em, 201);
    plot1->replot();
}
