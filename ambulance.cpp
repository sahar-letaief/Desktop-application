#include "ambulance.h"
#include<QString>
#include "menu.h"
#include "QSqlQuery"
#include<QMessageBox>
#include <QObject>
#include"QSqlQueryModel"

ambulance::ambulance()
{
immatricule=0;
etat="";
ambulancier="";
materiel_medical="";
}

ambulance::ambulance(int immatricule,QString etat,QString ambulancier ,QString materiel_medical)
{


    this->immatricule=immatricule;
    this->etat=etat;
    this->ambulancier=ambulancier;
    this->materiel_medical=materiel_medical;

}

int ambulance::get_immatricule()
{
    return immatricule;
}

QString ambulance::get_etat()
{
    return etat;
}

QString ambulance::get_ambulancier()
{
    return ambulancier;
}
QString ambulance::get_materiel_medical()
{
    return materiel_medical;
}

//----------------------AJOUTER---------------------
bool ambulance::ajouter()
{
    QSqlQuery qry;
   QString  res=QString::number(immatricule);
    qry.prepare("INSERT INTO ambulance(IMMATRICULE,ETAT,AMBULANCIER,MATERIEL_MEDICAL)""VALUES(:immatricule,:etat,:ambulancier,:materiel_medical)");
    qry.bindValue(":immatricule",immatricule);
    qry.bindValue(":etat",etat);
    qry.bindValue(":ambulancier",ambulancier);
    qry.bindValue(":materiel_medical",materiel_medical);



    return qry.exec();

}

//------------------AFFICHER----------------------------
QSqlQueryModel * ambulance::Afficher_ambulance()
{
QSqlQueryModel* model= new QSqlQueryModel();
model->setQuery("select * from ambulance");
model->setHeaderData(0,Qt::Vertical,QObject::tr("immatricule"));
model->setHeaderData(1,Qt::Vertical,QObject::tr("etat"));
model->setHeaderData(2,Qt::Vertical,QObject::tr("ambulancier"));
model->setHeaderData(3,Qt::Vertical,QObject::tr("materiel_medical"));

return model;
}

//********************************************SUPPRIMER***********************
bool ambulance::Supprimer_ambulance(int immatricule)
{
    QSqlQuery qry;
    QString res=QString::number(immatricule);
    qry.prepare("delete from ambulance where IMMATRICULE = :immatricule");
    qry.bindValue(":immatricule",res);
    return qry.exec();
}


//***********************************************recherche******************************
QSqlQueryModel * ambulance::CHERCHERa(const QString &aux)
{
    QSqlQueryModel * model = new QSqlQueryModel();

    model->setQuery("select * from ambulance where ((immatricule ) LIKE '%"+aux+"%')");
    model->setHeaderData(0,Qt::Vertical,QObject::tr("immatricule"));
    model->setHeaderData(1,Qt::Vertical,QObject::tr("etat"));
    model->setHeaderData(2,Qt::Vertical,QObject::tr("ambulancier"));
    model->setHeaderData(3,Qt::Vertical,QObject::tr("materiel_medical"));
    return model;
}

//********************************************************************************
QSqlQueryModel * ambulance::TRIER_ambulance()
{
    QSqlQueryModel *model=new QSqlQueryModel();
    model->setQuery("select * from ambulance order by etat desc");
    model->setHeaderData(0,Qt::Vertical,QObject::tr("immatricule"));
    model->setHeaderData(1,Qt::Vertical,QObject::tr("etat"));
    model->setHeaderData(2,Qt::Vertical,QObject::tr("ambulancier"));
    model->setHeaderData(3,Qt::Vertical,QObject::tr("materiel_medical"));

    return model ;

}


//------------------------MODIFIER--------------------

bool ambulance::Modifier_ambulance(int immatricule,QString etat,QString ambulancier ,QString materiel_medical)
{
    QSqlQuery qry;

    qry.prepare("update AMBULANCE set IMMATRICULE=:immatricule,ETAT=:etat,AMBULANCIER=:ambulancier,MATERIEL_MEDICAL=:materiel_medical where IMMATRICULE=:immatricule");

    qry.bindValue(":immatricule",immatricule);
    qry.bindValue(":etat",etat);
    qry.bindValue(":ambulancier",ambulancier);
    qry.bindValue(":materiel_medical",materiel_medical);


    return qry.exec();
}

// --------update
