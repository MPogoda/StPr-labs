#include "mw.h"
#include "ui_mw.h"
#include <QDebug>

MW::MW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MW),
    monteCarlo(new MonteCarlo(this))
{
    ui->setupUi(this);
    connect(ui->bt1, SIGNAL(clicked()),
            monteCarlo, SLOT(prepare_for_pi()));
    connect(ui->bt1, SIGNAL(clicked()),
            monteCarlo, SLOT(show()));
    connect(ui->bt2, SIGNAL(clicked()),
            monteCarlo, SLOT(prepare_for_integrate()));
    connect(ui->bt2, SIGNAL(clicked()),
            monteCarlo, SLOT(show()));
    connect(ui->bt3, SIGNAL(clicked()),
            monteCarlo, SLOT(prepare_for_plot()));
    connect(ui->bt3, SIGNAL(clicked()),
            monteCarlo, SLOT(show()));
}

MW::~MW()
{
    delete monteCarlo;
    delete ui;
}
