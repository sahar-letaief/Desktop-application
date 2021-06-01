#include "medicament.h"
#include "menu.h"

medicament::medicament()
{
    nom="";
    description="";
    DF="";
    DLC="";
    prix="";
    quantite="";
}

medicament::medicament(QString nom,QString description,QString DF,QString DLC,QString prix,QString quantite)
{
    this->nom=nom;
    this->description=description;
    this->DF=DF;
    this->DLC=DLC;
    this->prix=prix;
    this->quantite=quantite;

}

QString medicament::getNom(){return nom;}
QString medicament::getDescription(){return description;}
QString  medicament::getDF(){return DF;}
QString  medicament::getDLC(){return DLC;}
QString medicament::getPrix(){return prix;}
QString medicament::getQuantite(){return quantite;}

void medicament::setNom (QString nom){this->nom=nom;}
void medicament::setDescription (QString description){this->description=description;}
void medicament::setDF (QString  DF){this->DF=DF;}
void medicament::setDLC (QString  DLC){this->DLC=DLC;}
void medicament::setPrix (QString prix){this->prix=prix;}
void medicament::setQuantite (QString quantite){this->quantite=quantite;}

bool medicament::ajouter_medicament()
{
    QSqlQuery query;
    query.prepare("INSERT INTO MEDICAMENTS (NAME,DESCRIPTION,DF,DLC,PRIX,QUANTITE)" " VALUES(:name, :description, :df, :dlc,:prix, :quantite)");
    query.bindValue(":name",nom);
    query.bindValue(":description",description);
    query.bindValue(":df",DF);
    query.bindValue(":dlc",DLC);
    query.bindValue(":prix",prix);
    query.bindValue(":quantite",quantite);
    return query.exec();


}

bool medicament::supprimer_medicaments(QString nom)
{
    QSqlQuery query;
    query.prepare("DELETE FROM MEDICAMENTS WHERE NAME= :nom");
    query.bindValue(":nom",nom);
    return query.exec();
}


bool medicament::modifier_medicament(QString nom_init)
{
    QSqlQuery qry;
    qry.prepare("UPDATE MEDICAMENTS SET NAME=:nom , DESCRIPTION=:desc , DF=:df , DLC = :dlc , PRIX= :prix, QUANTITE= :quantite WHERE NAME=:name");
    qry.bindValue(":nom",nom);
    qry.bindValue(":desc",description);
    qry.bindValue(":df",DF);
    qry.bindValue(":dlc",DLC);
    qry.bindValue(":prix",prix);
    qry.bindValue(":quantite",quantite);
    qry.bindValue(":name",nom_init);

    return qry.exec();
}


QSqlQuery medicament::afficher_med_double_clicked(QString nom)
{
    QSqlQuery* qry=new QSqlQuery();
    qry->prepare("SELECT * FROM MEDICAMENTS WHERE NAME = :nom");
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



QSqlQuery* medicament::afficherMedicamentList()
{
    QSqlQuery* qry=new QSqlQuery();
    qry->exec("SELECT * FROM MEDICAMENTS");
    return qry;
}



int medicament::clear_liste_med()
{
    QSqlQuery *qry=new QSqlQuery();
    int rowCount = 0;
    if (qry->exec("SELECT * FROM MEDICAMENTS"))
    {

        while(qry->next())
        {
            rowCount++;
        }

    }

    return rowCount;
}

int medicament::chercher_nom_exist(QString nom)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM MEDICAMENTS WHERE NAME= :nom");
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

int medicament::chercher_nomMedicament_a_modifier(QString nom_init)
{
    QSqlQuery qry;

    qry.prepare("SELECT * FROM MEDICAMENTS WHERE NAME = :nom");
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



QSqlQuery* medicament::trier_liste_med()
{
    QSqlQuery* qry= new QSqlQuery() ;

    qry->exec("SELECT * FROM MEDICAMENTS ORDER BY NAME ASC");

    return qry;
}

QSqlQuery* medicament::trierQuantite_liste_med()
{
    QSqlQuery* qry= new QSqlQuery() ;

    qry->exec("SELECT * FROM MEDICAMENTS ORDER BY QUANTITE DESC");

    return qry;
}

QSqlQuery* medicament::trierPrix_liste_med()
{
    QSqlQuery* qry= new QSqlQuery() ;

    qry->exec("SELECT * FROM MEDICAMENTS ORDER BY PRIX DESC");

    return qry;
}
