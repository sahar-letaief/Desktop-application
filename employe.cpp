#include "employe.h"
#include "QMessageBox"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QPrinterInfo>
#include "connectdb.h"


Employe::Employe()
{

}

bool modifyValues(QString username, QString mname, QString lname ,QString fname ,QString email,QString newid, QString id, QString oldid)
{

   QSqlQuery query;
   query.prepare("UPDATE emp "
               "SET  id = :newID, Lname= :lname, Fname=:fname, Mname=:mname , Email=:email , WHERE id = :oldid ;");
   query.addBindValue(username);
    query.addBindValue(id);
    query.addBindValue(lname);
    query.addBindValue(fname);
    query.addBindValue(mname);
    query.addBindValue(email);
    query.addBindValue(oldid);


    if(!query.exec()){
        QMessageBox::critical(nullptr, QObject::tr("ID already exists."),
                    QObject::tr("Please change the ID."), QMessageBox::Ok);
        return false;
    }
    else{
        qDebug("Values added to DB");
        return true;
    }
}

bool addValuesToDB(QString username, QString mname, QString fname , QString lname , QString email ,QString id)
{
   QSqlQuery query;
       query.prepare("UPDATE emp "
                   "SET Username = :username, Lname= :lname, Fname=:fname, Mname=:mname , Email=:email , ID=:id");
        query.addBindValue(username);
        query.addBindValue(lname);
        query.addBindValue(fname);
        query.addBindValue(mname);
        query.addBindValue(email);
        query.addBindValue(id);


        if(!query.exec()){
            QMessageBox::critical(nullptr, QObject::tr("ID already exists."),
                        QObject::tr("Please change the ID."), QMessageBox::Ok);
            return false;
        }
        else{
            qDebug("Values added to DB");
            return true;
        }
    }


Employe::Employe(QString username,QString lname ,QString mname,QString fname,QString email,int id)
{
   this->username=username;
   this->lname=lname;
   this->mname=mname;
   this->fname=fname;
   this->email=email;
   this->id=id;
}
bool Employe::ajouter()
{
    QSqlQuery query;
    QString res= QString::number(id);
    query.prepare("INSERT INTO EMP("
                "username,"
                "mname,"
                "lname,"
                "fname,"
                "email,"
                "id)"
                "VALUES (?,?,?,?,?,?);");
    query.addBindValue(username);
    query.addBindValue(mname);
    query.addBindValue(lname);
    query.addBindValue(fname);
    query.addBindValue(email);
    query.addBindValue(id);


    if(!query.exec()){
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                    QObject::tr("Employeur ajouter"), QMessageBox::Ok);
        return false;
    }
    else{
        qDebug("Values added to DB");
        return true;
    }
}

QSqlQueryModel * Employe::afficher()
{

QSqlQueryModel * model=new QSqlQueryModel();
model->setQuery("select * from emp");
model->setHeaderData(0,Qt::Horizontal,QObject::tr("username"));
model->setHeaderData(1,Qt::Horizontal,QObject::tr("mname"));
model->setHeaderData(2,Qt::Horizontal,QObject::tr("fname"));
model->setHeaderData(3,Qt::Horizontal,QObject::tr("lname"));
model->setHeaderData(4,Qt::Horizontal,QObject::tr("email"));
model->setHeaderData(5,Qt::Horizontal,QObject::tr("id"));
return model;

}
bool Employe::supprimer(int id)
{
 QSqlQuery query;
 QString res=QString::number(id);
 query.prepare("DELETE From emp where ID= :id");
 query.bindValue(":id",res);

 return query.exec();
}
void Employe::afficher(QString username, QString mname,QString lname, QString fname ,QString email ,int id){
    this->username=username;
    this->mname=mname;
    this->fname=fname;
    this->email=email;
    this->id=id;
    this->lname=lname;
}
QSqlQueryModel* Employe::recherche(QString a)
{



        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("select ID from emp where ID='"+a+"' ");

        return model;


}
QSqlQueryModel * Employe::afficherOrderedList()
{
QSqlQueryModel * model=new QSqlQueryModel();
QSqlQuery qry ;
qry.prepare(" SELECT ID FROM EMP ORDER BY ID DESC");
qry.exec();

model->setQuery(qry);



return model;
}
