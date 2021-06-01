#ifndef MISSION_H
#define MISSION_H

#include"QSqlQueryModel"
#include"QSqlQuery"

#include <QObject>

class mission
{
    int id;
    QString destination,heure;
public:
    mission();
    mission(int,QString,QString);

    int getid();
    QString getdestination();
    QString getheure();
    bool Modifier_mission(int id,QString destination,QString heure );
    bool ajouter();
    QSqlQueryModel * afficher_mission();
    bool supprimer_mission(int);
    QSqlQueryModel *CHERCHERm(const QString &aux);
    QSqlQueryModel *  TRIERm();

};

#endif // MISSION_H
