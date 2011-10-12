#ifndef MW_H
#define MW_H

#include <QMainWindow>
#include <QWidget>

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
private slots:
    void bt1();
    void bt2();
    void bt3();
    void bt4();
};

#endif // MW_H
