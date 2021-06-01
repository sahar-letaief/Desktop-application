#ifndef SERVICE_H
#define SERVICE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class service
{
private:
    QString nom,staff1,staff2,staff3,nom_equipement;
   // QString equip;

public:
    service();
    service(QString,QString,QString,QString,QString);

    void set_nom_service(QString nv_nom){nv_nom=nom;}
    void set_staff1(QString nv_staff1){nv_staff1=staff1;}
    void set_staff2(QString nv_staff2){nv_staff2=staff2;}
    void set_staff3(QString nv_staff3){nv_staff3=staff3;}
    void set_equipement(QString nv_equipement){nv_equipement=nom_equipement;}
    //void set_equip(QString nv_equip){nv_equip=equip;}

    QString get_nom_service(){return nom;}
    QString get_staff1(){return staff1;}
    QString get_staff2(){return staff2;}
    QString get_staff3(){return staff3;}
    QString get_equipement(){return  nom_equipement;}
    //QString get_equip(){return  equip;}

    bool ajouter_service();
    bool supprimer_service(QString);
    QSqlQueryModel* afficher_ListeService();
    bool modifier_service(QString);
    QSqlQuery* afficher_service(QString val);

    QSqlQueryModel* rechercher_service(QString nom);
    QSqlQueryModel * trier_service();

    bool controle_saisie_serviceAjout(service s);
    void printPDF_service();
     QSqlQueryModel* afficher_Staff();



};

#endif // SERVICE_H
