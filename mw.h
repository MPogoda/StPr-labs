#ifndef MW_H
#define MW_H

#include <QMainWindow>
#include <QWidget>
#include "montecarlo.h"

namespace Ui {
    class MW;
}

class MW : public QMainWindow
{
    Q_OBJECT

public:
    explicit MW(QWidget *parent = 0);
    ~MW();

private:
    Ui::MW *ui;
    MonteCarlo *monteCarlo;
};

#endif // MW_H
