#ifndef REUNION_H
#define REUNION_H

#include <QDialog>

namespace Ui {
class Reunion;
}

class QTcpSocket;

class Reunion : public QDialog
{
    Q_OBJECT

    private slots:
        void on_send_2_clicked();
        void on_pushButton_2_clicked();


    public:
        explicit Reunion(QWidget *parent = nullptr);
        ~Reunion();
        QString nickname;

    private:
        Ui::Reunion *ui;
        QTcpSocket *mSocket;

};

#endif // REUNION_H
