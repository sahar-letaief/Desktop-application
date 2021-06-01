#include "service.h"
#include "connectdb.h"
#include "qpainter.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPrinter>
#include<QPrintDialog>
#include <QPdfWriter>
#include <QMessageBox>
service::service()
{

}
service::service(QString nom,QString staff1,QString staff2,QString staff3,QString nom_equipement)
{
    this->nom=nom;
    this->staff1=staff1;
    this->staff2=staff2;
    this->staff3=staff3;
    this->nom_equipement=nom_equipement;
}
 bool service::ajouter_service()
 {
     QSqlQuery query;
     query.prepare("INSERT INTO SERVICES(nom,staff1,staff2,staff3,nom_equipement)"
                   "VALUES(:nom,:staff1,:staff2,:staff3,:nom_equipement)");
     query.bindValue(":nom",nom);
     query.bindValue(":staff1",staff1);
     query.bindValue(":staff2",staff2);
     query.bindValue(":staff3",staff3);
     query.bindValue(":nom_equipement",nom_equipement);
     return query.exec();
 }

 bool service::supprimer_service(QString nom_supp)
 {
     QSqlQuery query;
     query.prepare("DELETE FROM SERVICES WHERE NOM=:nom");
     query.bindValue(":nom",nom_supp);
     return  query.exec();
 }
 QSqlQueryModel* service::afficher_ListeService()
 {
     void statistiques();
     QSqlQueryModel *model=new QSqlQueryModel();
     model->setQuery("SELECT * FROM SERVICES");
     model->setHeaderData(0,Qt::Horizontal,QObject::tr("Nom"));
     model->setHeaderData(1,Qt::Horizontal,QObject::tr("Staff1"));
     model->setHeaderData(2,Qt::Horizontal,QObject::tr("Staff2"));
     model->setHeaderData(3,Qt::Horizontal,QObject::tr("Staff3"));
     model->setHeaderData(4,Qt::Horizontal,QObject::tr("Nom equipement"));


     return model;

 }
 bool service::modifier_service(QString old_name)
 {
     QSqlQuery query;
     query.prepare("UPDATE SERVICES SET NOM=:nom,STAFF1=:staff1,STAFF2=:staff2,STAFF3=:staff3,NOM_EQUIPEMENT=:nom_equipement WHERE NOM='"+old_name+"'");
     query.bindValue(":nom",nom);
     query.bindValue(":staff1",staff1);
     query.bindValue(":staff2",staff2);
     query.bindValue(":staff3",staff3);
     query.bindValue(":nom_equipement",nom_equipement);
     ////////////////////////
     return query.exec();

 }

 QSqlQuery* service::afficher_service(QString val)
 {
     QSqlQuery* query=new QSqlQuery();
     query->prepare("SELECT * FROM SERVICES WHERE (NOM='"+val+"') OR (STAFF1='"+val+"') OR (STAFF2='"+val+"')OR (STAFF3='"+val+"')");
     return  query;
 }

 QSqlQueryModel * service::rechercher_service(QString nom )
 {
     QSqlQueryModel * model= new QSqlQueryModel();

             model->setQuery("SELECT * FROM SERVICES WHERE( NOM =  '"+nom+"' OR STAFF1='"+nom+"'OR STAFF2='"+nom+"'OR STAFF3='"+nom+"' OR NOM_EQUIPEMENT='"+nom+"')");
             model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
             model->setHeaderData(1, Qt::Horizontal, QObject::tr("Staff1"));
             model->setHeaderData(2, Qt::Horizontal, QObject::tr("Staff2"));
             model->setHeaderData(3, Qt::Horizontal, QObject::tr("Staff3"));
             model->setHeaderData(4, Qt::Horizontal, QObject::tr("Nom equipement"));


         return model ;
 }
 QSqlQueryModel * service::trier_service()
 {
     QSqlQueryModel * model= new QSqlQueryModel();

     model->setQuery("SELECT * FROM SERVICES ORDER BY NOM ;");

     model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
     model->setHeaderData(1, Qt::Horizontal, QObject::tr("Staff1"));
     model->setHeaderData(2, Qt::Horizontal, QObject::tr("Staff2"));
     model->setHeaderData(3, Qt::Horizontal, QObject::tr("Staff3"));
     model->setHeaderData(4, Qt::Horizontal, QObject::tr("Nom equipement"));

     return model;
 }
 bool service::controle_saisie_serviceAjout(service s)
 {
      if((s.get_staff1()==s.get_staff2())||(s.get_staff1()==s.get_staff3())||(get_staff2()==get_staff3()))
       return true;
     else
         return false;
 }
 void service::printPDF_service()
 {

     QPdfWriter pdf("C:/temp/service.pdf");
     QPainter painter(&pdf);
     QFont font=painter.font();
     QMessageBox msgBox;
     QImage image(":/pics/pics/pdfSahar.png");
     painter.drawImage(-20,-30,image);
        font.setPointSize(font.pointSize() * 2);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(300,800 , "Nom: ");
        painter.drawText(300,1600, "Staff1: ");
        painter.drawText(300, 2400, "Staff2: ");
        painter.drawText(300, 3200, "Staff3: ");
        painter.drawText(300,4000,"Nom equipement: ");
        painter.setPen(Qt::darkGray);
        painter.drawText(1300, 800, this->nom);
        painter.drawText(1300, 1600, this->staff1);
        painter.drawText(1300, 2400, this->staff2);
        painter.drawText(1300, 3200, this->staff3);
        painter.drawText(2700,4000,this->nom_equipement);
        painter.end();
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("A pdf has been created.");
        msgBox.exec();


 }


