#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H
#include <QString>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QDateEdit>
#include <iostream>
#include <QString>
#include <QDebug>
#include <QtDebug>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QModelIndex>
#include <QListWidgetItem>


class equipement
{
private:
    QString nom,etat_fonctionnement,dispo;
public:
    equipement();
    equipement(QString,QString,QString);

    QString get_nom(){return nom;}
    QString get_etat_fonc(){return etat_fonctionnement;}
    QString get_dispo(){return dispo;}

    bool ajouter_equipement();
    bool supprimer_equipement(QString);
    bool modifier_equipement(QString);
    int clear_liste_eq();
    QSqlQuery afficher_eq_double_clicked(QString );
    int chercher_nom_exist(QString);
    int chercher_nomEquipement_a_modifier(QString );
    QSqlQuery *afficherEquipementListe();
    QSqlQuery *trier_liste_equi();
};

#endif // EQUIPEMENT_H
