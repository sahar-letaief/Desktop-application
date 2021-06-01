#ifndef AMBULANCE_H
#define AMBULANCE_H

#include<QString>
#include"QSqlQueryModel"
#include"QSqlQuery"


class ambulance
{ public:
   ambulance();
       ambulance(int,QString,QString,QString);
       int get_immatricule();
          QString get_etat();
          QString get_ambulancier();
          QString get_materiel_medical();

          bool ajouter();
          bool Supprimer_ambulance(int);
          bool Modifier_ambulance(int immatricule,QString etat,QString ambulancier ,QString materiel_medical);
          QSqlQueryModel * Afficher_ambulance();
          QSqlQueryModel *  TRIER_ambulance();
          QSqlQueryModel *CHERCHERa(const QString &aux);

   private :
          int immatricule;
          QString etat,ambulancier,materiel_medical;

};

#endif // AMBULANCE_H
