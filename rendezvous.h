#ifndef RENDEZVOUS_H
#define RENDEZVOUS_H

#include <QDate>
#include <QDateTime>
#include <QString>
#include <QDialog>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSound>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <windows.h>


class RendezVous {
private:
    QString id,nomPatient,prenomPatient,email;
    QDateTime dateTime;

public:
    bool isEmailAddress(QString strEmailAddr);

    QString getId(){return id;};

    QString getEmail(){return email;};

    QString getPrenom(){return prenomPatient;};

    QString getNom(){return nomPatient;};

    QDateTime getDateTime(){return dateTime;};

    void setID(QString id){this->id=id;};

    bool addValuesToDB(QString nomPatient, QString prenomPatient,QString id,QString email,QDateTime dateTime);

    bool modifyValues(QString nomPatient, QString prenomPatient,QString id,QString email,QDateTime dateTime);

    void send_email(QString email,QString date,QString heure,QString prenom);

    void afficher(QString,QString,int);

    void afficher(QString tri,int occurence,QString id1,QString nomPatient1,QString prenomPatient1,QString email1);

    void Delete();

    QSqlQueryModel* search(QSqlQuery qry,QString column,QString id,QString nomPatient,QString prenomPatient,QString email);

    QSqlQueryModel* search(QSqlQuery qry,QString column);

    QSqlQueryModel* search(QSqlQuery qry,QString column,QString arg1);

    void updateMinuscule();

};

#endif // RENDEZVOUS_H
