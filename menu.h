#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include "patient.h"
#include "rendezvous.h"
#include <QList>
#include <QListWidget>
#include <QMessageBox>
#include <QSound>
#include <windows.h>
#include "ui_menu.h"
#include <QVector>
#include <QPropertyAnimation>
#include <QMovie>
#include <QLayout>
#include <QSqlQueryModel>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include "reunion.h"
#include "employe.h"
#include <QTimer>
#include <QSqlTableModel>
#include "medicament.h"
#include "equipement.h"
#include "alarmearduino.h"
#include "service.h"
#include "chambre.h"
#include <QMainWindow>
#include "ambulance.h"
#include "mission.h"
#include <QPrinter>
#include <QPainter>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QMediaPlayer>
#include <QInputDialog>
#include <QTranslator>
#include "arduinow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Menu; }
QT_END_NAMESPACE

class Menu : public QMainWindow
{
    Q_OBJECT
    int id ;

private:
    bool volume;
    QPixmap QR;
    Reunion R1;
    Patient P;
    RendezVous R;
    ConnectDB db;
    bool advancedPatient_bool;
    bool advancedRDV_bool;
    QByteArray data;
    AlarmeArduino A;
    service ser;
    chambre cham;
    QString aux_ser,aux_cham;

    void arduinoInit();
    void covid();

public:
    Menu(QWidget *parent = nullptr);
    ~Menu();
    QString picName;
    QString picDir = QCoreApplication::applicationDirPath()+"/../../login/users/avatar";
    bool loggedIn;
    bool loginPage(QString u, QString p);
    QSqlTableModel* tblMdl;

    void initialiserDali();

    void initialiserErrorsRDV();

    void initialiserErrorsPatient();

    void initialiserTout();

    void smallLineAnimationPatient();

    void smallLineAnimationRendezVous();

    void bigLineAnimationPatient();

    void bigLineAnimationRendezVous();

    void refreshDBPatient();

    void refreshDBRdv();

    QString triPatient();

    QString triRDV();

    void initialiserPatient();

    void afficherPatient();

    void initialiserRDV();

    void afficherRDV();

    void advancedPatient();

    void advancedRDV();


    void smallLineAnimationEquipement();
    void smallLineAnimationMedicament();
    void bigLineAnimationMedicament();
    void bigLineAnimationEquipement();
    void update_labelw();

private slots:
    void sendMail();

    void mailSent(QString);
    void downloadFinished(QNetworkReply*);

    void myfunction();

    void on_gestionPatientMG_2_clicked();

    void on_menu_listeRendezVous_clicked();

    void on_menu_listePatients_clicked();

    void on_rechercheRendezVous_textChanged();

    void on_recherchePatient_textChanged();

    void on_Home_Button_2_clicked();

    void on_Home_Button_clicked();

    void on_RendezVous_Button_2_clicked();

    void on_Patient_Button_clicked();

    void on_listPatient_doubleClicked(const QModelIndex &index);

    void on_listRendezVous_doubleClicked(const QModelIndex &index);

    void on_triPatient_currentTextChanged();

    void on_triRDV_currentTextChanged();

    void on_ajouterPatient_clicked();

    void on_Annuler_clicked();

    void on_DeleteButton_pressed();

    void on_DeleteButton_released();

    void on_DeleteButton_clicked();

    void on_ajouterRendezVous_clicked();

    void on_annuler_clicked();

    void on_DeleteButton_2_pressed();

    void on_DeleteButton_2_released();

    void on_DeleteButton_2_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_email_sending_clicked();

    void on_email_sending_pressed();

    void on_email_sending_released();

    void on_pdf_icon_clicked();

    void on_NewPatient_button_clicked();

    void on_newRDV_button_clicked();

    void on_nomPatient_textChanged(const QString &arg1);

    void on_prenomPatient_textChanged(const QString &arg1);

    void on_numChambrePatient_textChanged(const QString &arg1);

    void on_cinPatient_textChanged(const QString &arg1);

    void on_advanced_cin_textChanged(const QString &arg1);

    void on_advanced_nom_textChanged(const QString &arg1);

    void on_advanced_prenom_textChanged(const QString &arg1);

    void on_advanced_chambre_textChanged(const QString &arg1);

    void on_advanced_button_clicked();

    void on_advanced_cinrdv_textChanged(const QString &arg1);

    void on_advanced_nomrdv_textChanged(const QString &arg1);

    void on_advanced_prenomrdv_textChanged(const QString &arg1);

    void on_advanced_emailrdv_textChanged(const QString &arg1);

    void on_advancedButton_rdv_clicked();

    void on_nomPatientRendezVous_textChanged(const QString &arg1);

    void on_prenomPatientRendezVous_textChanged(const QString &arg1);

    void update_label();

    void on_ReunionMG_clicked();

    void on_regButton_clicked();

    void on_backButton_clicked();

    void on_completeRegButton_clicked();

    void on_uplButton_clicked();

    void on_loginButton_clicked();

    void on_gestionPersonnelMG_2_clicked();

    void on_ajouteremploye_2_clicked();

    void on_pushButton_4_clicked();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_clicked();

    void on_Print_clicked();

    void on_recherche_clicked();

    void on_commandLinkButton_clicked();

    void on_addemploye_clicked();

    void on_reportemploye_clicked();

    void on_employeelist_clicked();

    void on_menu_listePatients_2_clicked();

    void on_uplButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_LogoutMG_clicked();


    void on_editButton_clicked();

    void on_menu_listeRendezVous_2_clicked();

    void on_logoutButton_clicked();

    void on_delButton_clicked();

    void on_backButton_2_clicked();

    void on_editedButton_clicked();

    void on_userBrowse_clicked();

    void on_adminBrowse_clicked();

    void on_pageButton_clicked();

    void on_delUButton_clicked();

    void on_delAButton_clicked();

    void on_backButton_5_clicked();

    void on_editedButton_2_clicked();

    void on_stackedWidget_5_currentChanged(int arg1);

    void on_adminButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_Delete_clicked();




    void on_gestionInfrastructrueMG_2_clicked();

    void on_pushButton_menu_clicked();

    void on_list_med_clicked();

    void on_list_eq_clicked();

    void on_Home_2_clicked();

    void on_Equipement_clicked();

    void on_Home_3_clicked();

    void on_Medicament_5_clicked();

    void on_ajouter_med_clicked();

    void on_Statistique_med_clicked();

    void on_ajouter_equi_clicked();

    void on_pushButton_annuler_2_clicked();

    void on_pushButton_annuler_clicked();

    void on_annuler_2_clicked();

    void on_pushButton_anuuler_clicked();

    void on_retour_clicked();

    void afficher_medicament();

    void statistique_med();

    void afficher_equipement();

    void on_pushButton_ajouter_clicked();

    void on_effacer_medicament_clicked();

    void on_effacer_equipement_clicked();

    void on_chercher_med_textChanged(const QString &arg1);

    void on_listWidget_MED_itemDoubleClicked(QListWidgetItem *item);

    void on_ModifierMedicament_clicked();

    void on_ajout_equi_2_clicked();

    void on_chercher_equi_textChanged(const QString &arg1);

    void on_listWidget_EQUI_itemDoubleClicked(QListWidgetItem *item);

    void on_ModifierEquipement_clicked();

    void on_trier_med_clicked();

    void on_trier_eq_clicked();

    void on_imprimer_clicked();

    void on_pushButton_menuChambres_clicked();
    void on_pushButton_menuServices_clicked();
    void on_ajouter_service_clicked();
    void on_supprimer_service_clicked();
    void on_ajouter_chambre_clicked();
    void on_supprimer_chambre_clicked();
    void on_pushButton_Services_clicked();
    void on_pushButton_Chambres_clicked();
    void on_pushButton_petitMenu_clicked();
    void on_pushButton_services_clicked();
    void on_pushButton_Chambre_clicked();
    void on_pushButton_petitmenu_clicked();
    void on_pushButton_AnnulerAjoutService_clicked();
    void on_pushButton_AnnulerModifService_clicked();
    void on_pushButton_AnnulerAjoutChambre_clicked();
    void on_lineEdit_ajoutNomService_textEdited(const QString &arg1);
    void on_pushButton_AjouterService_clicked();
    void on_pushButton_AjoutChambre_clicked();
    void on_tableView_services_activated(const QModelIndex &index);
    void on_tableView_chambres_activated(const QModelIndex &index);
    void on_pushButton_RetourAffChambre_clicked();
    void on_pushButton_RetourAffService_clicked();
    void on_pushButton_modifierServiceAff_clicked();
    void on_pushButton_triServices_clicked();
    void on_pushButton_modifService_clicked();
    void on_pushButton_ModifChambre_clicked();
    void on_pushButton_annulerModifChambre_clicked();
    void on_pushButton_ModifierChambre_clicked();
    void on_pushButton_imprimerService_clicked();
    void on_pushButton_rechercherChambre_clicked();
    void on_pushButton_triChambre_clicked();
    void on_pushButton_rechercherService_clicked();
    void stat_sahar();
    void on_pushButton_StatService_clicked();
    void on_pushButton_backstat_clicked();




    void on_gestionOffresMG_2_clicked();

    void on_pushButton_retourGestionoffres_clicked();



    void on_pushButtonaj_clicked();

    void on_pushButton_12aj_clicked();

    void on_pushButton_14AN_clicked();

    void on_pushButton_16AN_clicked();

    void on_pushButton_13OK_clicked();





    void on_pushButton_15oK_clicked();

    void on_pushButton_18supp_clicked();

    void on_pushButton_20supp_clicked();

    void on_pushButton_9ok_clicked();


    void on_pushButton_11ok_clicked();

    void on_pushButton_21tr_clicked();

    void on_pushButton_22tr_clicked();



    void on_pushButton_print_clicked();

    void on_pushButton_19mod_clicked();

    void on_pushButton_17mod_clicked();



    void on_pushButtonGDA_clicked();

    void on_pushButtonGDM_clicked();





    void on_gestionSecoursMG_2_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_18_clicked();

    void on_trier_med_Prix_clicked();

    void on_trier_med_Quantite_clicked();

    void on_pushButton_menu_2_clicked();

    void on_QrButton_clicked();

    void on_pushButton_menu_4_clicked();

    void on_pushButton_menu_3_clicked();

    void on_downlaod_buttonqrcode_clicked();

    void animation_service();
    void animation_chambre();
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_2_clicked();

   void on_pushButton_12_clicked();

   void on_pushButton_12ard_clicked();

   void on_pushButton_13ard_clicked();

   void on_pushButton_12menuu_clicked();

   void on_volume_button_clicked();

private:
    Ui::Menu *ui;
    QString username;
    QString password;
    int rank; //if -1 == admin else if 1 == employé
    QString fname , lname , mname , email ;
    QTimer *timer;
    Employe Etmp;
    Employe E;
    QList<Employe*> mesEmploye;
     ambulance tmpambulance;
     mission tmpmission;
     QMediaPlayer *player = new QMediaPlayer();
     QTranslator t;
     arduinow Z;
     QByteArray datta;



};
#endif // MENU_H
