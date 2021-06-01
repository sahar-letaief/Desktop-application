#include "reunion.h"
#include "ui_reunion.h"
#include <QTcpSocket>
#include <QTextStream>

Reunion::Reunion(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Reunion)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);

    connect(mSocket, &QTcpSocket::readyRead, [&]() {
        QTextStream T(mSocket);
        auto text = T.readAll();
        ui->textEdit->append(text);
        ui->textEdit->setReadOnly(true);
    });

    mSocket->connectToHost("localhost",3333);
}

Reunion::~Reunion()
{
    delete ui;
}


void Reunion::on_send_2_clicked()
{
    QTextStream T(mSocket);
    T << nickname << ": " << ui->message_2->text();
    mSocket->flush();
    ui->message_2->clear();
}



void Reunion::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}
