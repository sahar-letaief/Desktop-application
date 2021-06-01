#ifndef PATIENT_H
#define PATIENT_H

#include <QDialog>
#include <QString>
#include <QDate>
#include <QVector>
#include "connectdb.h"
#include <QSqlQueryModel>
#include <QListWidget>
#include <QIntValidator>
#include <QMessageBox>
#include <QSound>
#include <QPrinter>
#include <QPainter>




class Patient
{
private:
    QString nom,prenom,CIN,numChambre;
    QDate dateNaissance;

public:

    void setNom(QString nom);

    void setPrenom(QString prenom);

    void setCIN(QString cin){this->CIN=cin;};

    QString getCIN()const{return CIN;};

    QString getNom(){return nom;};

    QString getPrenom(){return prenom;};

    QString getNumChambre(){return numChambre;};

    QDate getDateNaissance(){return dateNaissance;};

    bool addValuesToDB(QString nom, QString prenom, QDate dateNaissance,QString Cin,QString numChambre);

    void afficher(QString,QString,int);

    void afficher(QString tri,int occurence,QString cin1,QString nom1,QString prenom1,QString numChambre1);

    bool chercherCin(QString Cin,QString operation);

    bool modifyValues(QString nom, QString prenom, QDate dateNaissance,QString Cin,QString numChambre,QString oldCin);

    void Delete();

    void printPDF();

    QSqlQueryModel* search(QSqlQuery qry,QString column,QString cin,QString nom,QString prenom,QString numChambre);

    QSqlQueryModel* search(QSqlQuery qry,QString column);

    QSqlQueryModel* search(QSqlQuery qry,QString column,QString text);

    void updateMinuscule();
};

#endif // PATIENT_H
