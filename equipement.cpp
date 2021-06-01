#include "equipement.h"



equipement::equipement()
{
    nom ="";
    etat_fonctionnement = "";
    dispo = "";
}

equipement::equipement(QString n,QString etat_f,QString dispo)
{
    this->nom = n;
    this->etat_fonctionnement = etat_f;
    this->dispo=dispo;
}



bool equipement::ajouter_equipement()
{
    QSqlQuery query;
    query.prepare("INSERT INTO EQUIPEMENT (NOM,DISPO,ETAT_FONC)" " VALUES(:name, :dispo, :etat)");
    query.bindValue(":name",nom);
    query.bindValue(":dispo",dispo);
    query.bindValue(":etat",etat_fonctionnement);
    return query.exec();


}


bool equipement::supprimer_equipement(QString nom)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPEMENT WHERE NOM= :nom");
    query.bindValue(":nom",nom);
    return query.exec();
}


bool equipement::modifier_equipement(QString nom_init)
{
    QSqlQuery qry;
    qry.prepare("UPDATE EQUIPEMENT SET NOM=:nom , DISPO=:disp , ETAT_FONC=:etat  WHERE NOM=:name");
    qry.bindValue(":nom",nom);
    qry.bindValue(":disp",dispo);
    qry.bindValue(":etat",etat_fonctionnement);
    qry.bindValue(":name",nom_init);
    return qry.exec();
}

int equipement::clear_liste_eq()
{
    QSqlQuery *qry=new QSqlQuery();
    int rowCount = 0;
    if (qry->exec("SELECT * FROM EQUIPEMENT"))
    {

        while(qry->next())
        {
            rowCount++;
        }

    }

    return rowCount;
}

QSqlQuery equipement::afficher_eq_double_clicked(QString nom)
{
    QSqlQuery* qry=new QSqlQuery();
    qry->prepare("SELECT * FROM EQUIPEMENT WHERE NOM = :nom");
    qry->bindValue(":nom",nom);

    if (qry->exec())
    {
        while (qry->next())
        {
            return *qry;
        }
    }
return *qry;

}

QSqlQuery* equipement::afficherEquipementListe()
{
    QSqlQuery* qry = new QSqlQuery();

    qry->exec("SELECT * FROM EQUIPEMENT");

    return qry;

}



int equipement::chercher_nom_exist(QString nom)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EQUIPEMENT WHERE NOM= :nom");
    query.bindValue(":nom",nom);
    int count(0);
    if(query.exec())
    {

        while(query.next())
        {
            count++;
        }
    }
    return count;

}


int equipement::chercher_nomEquipement_a_modifier(QString nom_init)
{
    QSqlQuery qry;

    qry.prepare("SELECT * FROM EQUIPEMENT WHERE NOM = :nom");
    qry.bindValue(":nom",nom_init);
    qDebug() << nom_init << endl;
   int count(0);
    if ( qry.exec() )
    {

        while (qry.next())
        {
            count++;

        }
}
    return count;
}

QSqlQuery* equipement::trier_liste_equi()
{
    QSqlQuery* qry= new QSqlQuery() ;

    qry->exec("SELECT * FROM EQUIPEMENT ORDER BY NOM ASC");

    return qry;
}
