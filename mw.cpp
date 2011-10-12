#include "mw.h"
#include "ui_mw.h"
#include "montecarlo.h"
#include "task4.h"
#include <QDebug>
MW::MW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MW)
{
    ui->setupUi(this);
    connect(ui->bt1, SIGNAL(clicked()),
            this, SLOT(bt1()));
    connect(ui->bt2, SIGNAL(clicked()),
            this, SLOT(bt2()));
    connect(ui->bt3, SIGNAL(clicked()),
            this, SLOT(bt3()));
    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(bt4()));
}

MW::~MW()
{
}

void MW::bt1()
{
    MonteCarlo form(this);
    form.calculate_pi();
}

void MW::bt2()
{
    MonteCarlo form(this);
    form.calculate_integral();
}

void MW::bt3()
{
    MonteCarlo form(this);
    form.plot();
}

void MW::bt4()
{
    Task4 form(this);
    form.exec();
}
