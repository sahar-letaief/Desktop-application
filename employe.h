#ifndef EMPLOYE_H
#define EMPLOYE_H


#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "QMessageBox"

class Employe
{
    QString username,lname,mname,fname,email;
    int id;

public:
    Employe();
    Employe(QString,QString,QString,QString,QString,int);

    QString getUsername(){return username;}
    QString getLname(){return lname;}
    QString getMname(){return mname;}
    QString getFname(){return fname;}
    QString getEmail(){return email;}
    int getID(){return id;}


    void setUsername(QString U){U=username;}
    void setLname(QString L){L=lname;}
    void setMname(QString M){M=mname;}
    void setFname(QString F){F=fname;}
    void setEmail(QString E){E=email;}
    void setID(int id){this->id=id;}


    bool modifyValues(QString username, QString mname , QString fname , QString email , QString id, QString oldid );
    bool addValuesToDB(QString username, QString mname,QString lname, QString fname ,QString email ,QString id);
    QSqlQueryModel * afficher();
    ;
    bool ajouter();
    bool supprimer(int);
    void afficher(QString username, QString mname,QString lname, QString fname ,QString email ,int id);
    QSqlQueryModel * recherche(QString a);
    QSqlQueryModel * afficherOrderedList();


};

#endif // EMPLOYE_H
