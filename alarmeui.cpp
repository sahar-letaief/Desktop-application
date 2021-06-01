#include "alarmeui.h"
#include "ui_alarmeui.h"

AlarmeUI::AlarmeUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlarmeUI)
{
    ui->setupUi(this);
}

AlarmeUI::~AlarmeUI()
{
    delete ui;
}

void AlarmeUI::on_pushButton_clicked()
{
    serial->write("0");
    close();
}

void AlarmeUI::init(QSerialPort* a){
    serial=a;
    exec();
}
