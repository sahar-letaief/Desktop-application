#ifndef CHAMBRE_H
#define CHAMBRE_H
#include <QString>
#include <QSpinBox>
#include <QComboBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>


class chambre
{
private:
    QString emplacement,numero,nom_surveillant, nombre_lits,type;

public:
    chambre();
    chambre(QString,QString,QString,QString,QString);

    void set_emplacement(QString nv_emplacement){nv_emplacement=emplacement;}
    void set_numero(QString nv_num){nv_num=numero;}
    void set_nom_surveillant(QString nv_nom_surveillant){nv_nom_surveillant=nom_surveillant;}
    void set_nombre_lits(QString nv_nombre_lits){nv_nombre_lits=nombre_lits;}
    void set_type(QString nv_type){nv_type=type;}

    QString get_emplacement(){return emplacement;}
    QString get_numero(){return numero;}
    QString get_nom_surveillant(){return nom_surveillant;}
    QString get_nombre_lits(){return  nombre_lits;}
    QString get_type(){return  type;}

    bool ajouter_chambre();
    bool supprimer_chambre(QString num_supp);
    QSqlQueryModel* afficher_ListeChambre();
    QSqlQuery* afficher_chambre(QString val);
    bool modifier_chambre(QString new_type,QString emplacement,QString type,QString numero,QString nombre_lits,QString nom_surveillant);

    QSqlQueryModel * rechercher_chambre(QString numero );
    QSqlQueryModel * trier_chambre();



    bool controle_saisieChambre(chambre cham);





};

#endif // CHAMBRE_H
