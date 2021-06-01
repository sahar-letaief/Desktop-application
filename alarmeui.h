#ifndef ALARMEUI_H
#define ALARMEUI_H

#include <QDialog>
#include "alarmearduino.h"

namespace Ui {
class AlarmeUI;
}

class AlarmeUI : public QDialog
{
    Q_OBJECT

public:
    explicit AlarmeUI(QWidget *parent = nullptr);
    ~AlarmeUI();
    void init(QSerialPort*);

private slots:
    void on_pushButton_clicked();

private:
    Ui::AlarmeUI *ui;
    AlarmeArduino A;
    QSerialPort* serial;
};

#endif // ALARMEUI_H
