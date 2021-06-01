#include "menu.h"
#include "chambre.h"
#include "service.h"
#include <QFile>
#include <QFileDialog>
#include <QPrintDialog>
#include <QTextDocument>
#include <QSqlRecord>
#include <QPixmap>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPixmap>
#include <QMessageBox>
#include "ambulance.h"
#include "mission.h"
#include <QPrinter>
#include <QWidget>
#include <QPrintDialog>
#include <QtPrintSupport>
#include <QtMultimedia/QMediaPlayer>
#include <QDesktopServices>
#include <QUrl>
#include "smtp.h"
#include <QDebug>
#include "arduinow.h"
#define CARACTERES_ETRANGERS "~{}[]()|-`'^ç@_]\"°01234567890+=£$*µ/§!?,.&#;><"


Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);

    QPixmap med_icon(":/pics/pics/med.png");
    ui->icon_med->setPixmap(med_icon.scaled(100,100,Qt::KeepAspectRatio));
    ui->icon_med_2->setPixmap(med_icon.scaled(100,100,Qt::KeepAspectRatio));

    QPixmap equi_icon(":/pics/pics/equipement.png");
    ui->icon_equi->setPixmap(equi_icon.scaled(100,100,Qt::KeepAspectRatio));
    ui->icon_equi_2->setPixmap(equi_icon.scaled(100,100,Qt::KeepAspectRatio));

    QPixmap listmedlogo(":/pics/pics/infrastructure.png");
    ui->listmedlogo->setPixmap(listmedlogo.scaled(400,400,Qt::KeepAspectRatio));
    ui->listmedlogo->setPixmap(listmedlogo.scaled(400,400,Qt::KeepAspectRatio));

    QPixmap listeqlogo(":/pics/pics/infrastructure.png");
    ui->listeqlogo->setPixmap(listmedlogo.scaled(400,400,Qt::KeepAspectRatio));
    ui->listeqlogo->setPixmap(listmedlogo.scaled(400,400,Qt::KeepAspectRatio));

    ui->stackedWidget->setCurrentIndex(0);
    player->setMedia(QUrl("qrc:/pics/pics/offres.mp3"));
    player->setVolume(150);



    initialiserDali();
    covid();
    statistique_med();

    arduinoInit();

    stat_sahar();
    ui->tableView_services->setModel(ser.afficher_ListeService());
    ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
    ui->tabambulance->setModel(tmpambulance.Afficher_ambulance());
     ui->tabmission->setModel(tmpmission.afficher_mission());
    //arduino walid
//     int rett=Z.connect_arduinow();
//     switch (rett)
//     {
//     case (0):qDebug()<<"arduino is available and connected to :"<<Z.getarduinow_port_name();
//         break;
//     case (1):qDebug()<<"arduino is available but not connected to :"<<Z.getarduinow_port_name();
//         break;
//     case (-1):qDebug()<<"arduino is not available";

//     }
//     QObject::connect(Z.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));

}

Menu::~Menu()
{
    db.closeDB();
    delete ui;
}

void Menu::covid(){
    QNetworkAccessManager *man= new QNetworkAccessManager(this);
    connect(man,&QNetworkAccessManager::finished,this,[&](QNetworkReply* reply){
        QJsonObject jsonObject= QJsonDocument::fromJson(reply->readAll()).object();
        qDebug()<< jsonObject["cases"].toDouble();
        ui->cas_covid->setText(QString::number(jsonObject["cases"].toDouble()));
        ui->recovered_covid->setText(QString::number(jsonObject["recovered"].toDouble()));
        ui->Active_covid->setText(QString::number(jsonObject["active"].toDouble()));
    });
    const QUrl url=QUrl("https://corona.lmao.ninja/v2/countries/tunisia?yesterday&strict&query%20");
    QNetworkRequest request(url);
    man->get(request);
}

void Menu::myfunction()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString(" hh ::  mm  ::  ss");
    if ((time.second() % 2 ==0)){
        time_text[3] = ' ';
        time_text[8] = ' ';
    }
    ui->label_date_time->setText(time_text);
}


void Menu::arduinoInit(){
    int ret = A.connect_arduino();
    switch(ret){
    case(0): qDebug() << "arduino is available and connected to :" << A.getarduino_port_name();
        break;
    case(1): qDebug() << "arduino is available and not connected to :" << A.getarduino_port_name();
        break;
    case(-1): qDebug() << "arduino is not available" ;
    }
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));
}


void Menu::update_label(){
    A.read_from_arduino();
}

void Menu::on_gestionPatientMG_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//DALI

//usefull functions

void Menu::initialiserDali(){
    refreshDBPatient();
    refreshDBRdv();
    initialiserTout();
    initialiserPatient();
    initialiserRDV();
    setWindowTitle("Hoptimal");
    advancedRDV_bool=false;
    ui->advanced_rdv->setVisible(false);
    advancedPatient_bool=false;
    ui->advanced_2->setVisible(false);
}

void Menu::advancedRDV(){
    R.updateMinuscule();
    QString column=triRDV();
    QSqlQuery qry;
    ui->listRendezVous->setModel(R.search(qry,column,ui->advanced_cinrdv->text(),ui->advanced_nomrdv->text(),ui->advanced_prenomrdv->text(),ui->advanced_emailrdv->text()));
    qDebug("List RDV refreshed.");

}


void Menu::advancedPatient(){
    P.updateMinuscule();
    QString column=triPatient();
    QSqlQuery qry;
    ui->listPatient->setModel(P.search(qry,column,ui->advanced_cin->text(),ui->advanced_nom->text(),ui->advanced_prenom->text(),ui->advanced_chambre->text()));
    qDebug("List patient refreshed.");

}

void Menu::initialiserErrorsRDV(){
    ui->nom_error_rdv->setText("");
    ui->prenom_error_rdv->setText("");
    ui->prenomPatientRendezVous->setMaxLength(20);
    ui->nomPatientRendezVous->setMaxLength(20);
    ui->idRendezVous->setMaxLength(20);
}

void Menu::initialiserErrorsPatient(){
    ui->Nom_error->setText("");
    ui->Prenom_error->setText("");
    ui->Cin_error->setText("");
    ui->numChambre_error->setText("");
    ui->prenomPatient->setMaxLength(20);
    ui->nomPatient->setMaxLength(20);
    ui->numChambrePatient->setMaxLength(20);
    ui->cinPatient->setMaxLength(8);
}

QString Menu::triRDV()
{
    if(ui->triRDV->currentText() == "Id de la RDV"){
        ui->rechercheRendezVous->setPlaceholderText("Rechercher par l'ID de la rendez vous");
        return "id";
    }
    else if(ui->triRDV->currentText() == "Nom du patient"){
        ui->rechercheRendezVous->setPlaceholderText("Rechercher par le nom du patient");
        return "nomPatient";
    }
    else if(ui->triRDV->currentText() == "Email du patient"){
        ui->rechercheRendezVous->setPlaceholderText("Rechercher par l'email du patient");
        return "email";
    }
    else{
        ui->rechercheRendezVous->setPlaceholderText("Rechercher par le prenom du patient");
        return "prenomPatient";
    }
}

void Menu::refreshDBRdv()
{
    R.updateMinuscule();
    QString arg1=ui->rechercheRendezVous->text();
    QSqlQuery qry;
    QString column=triRDV();
    if(arg1!=""){
        ui->listRendezVous->setModel(R.search(qry,column,arg1));
        qDebug("List RDV refreshed.");
    }
    else{
        ui->listRendezVous->setModel(R.search(qry,column));
        qDebug("list RDV refreshed.");
    }
}

QString Menu::triPatient()
{
if (ui->triPatient->currentText()=="Numero de la chambre"){
    ui->recherchePatient->setPlaceholderText("Rechercher par le numero de la chambre");
    return "numChambre";
    }
    else{
        ui->recherchePatient->setPlaceholderText("Rechercher par "+ui->triPatient->currentText().toLower());
        return ui->triPatient->currentText();
    }
}

void Menu::refreshDBPatient()
{
    P.updateMinuscule();
    QString arg1=ui->recherchePatient->text();
    QString column=triPatient();
    QSqlQuery qry;
    if(arg1!=""){
        ui->listPatient->setModel(P.search(qry,column,arg1));
        qDebug("List patient refreshed.");
    }
    else{ 
        ui->listPatient->setModel(P.search(qry,column));
        qDebug("List patient refreshed.");
    }
}

void Menu::initialiserTout()
{
    //validators

    QRegExp letters("[A-Z a-z]{0,20}");
    QRegExpValidator *validatorL = new QRegExpValidator(letters, this);
    ui->nomPatient->setValidator(validatorL);
    ui->prenomPatient->setValidator(validatorL);
    ui->prenomPatientRendezVous->setValidator(validatorL);
    ui->nomPatientRendezVous->setValidator(validatorL);

    QRegExp numbers("[0-9]{0,8}");
    QRegExpValidator *validatorN = new QRegExpValidator(numbers, this);
    ui->cinPatient->setValidator(validatorN);

    ui->rechercheRendezVous->setPlaceholderText("Rechercher par l'ID de la rendez vous");
    ui->recherchePatient->setPlaceholderText("Rechercher par cin");
    ui->advanced_cin->setPlaceholderText("Rechercher par cin");
    ui->advanced_nom->setPlaceholderText("Rechercher par nom");
    ui->advanced_prenom->setPlaceholderText("Rechercher par prenom");
    ui->advanced_chambre->setPlaceholderText("Rechercher par numero de la chambre");
    ui->advanced_cinrdv->setPlaceholderText("Rechercher par l'ID de la rendez vous");
    ui->advanced_nomrdv->setPlaceholderText("Rechercher par le nom du patient");
    ui->advanced_prenomrdv->setPlaceholderText("Rechercher par le prenom du patient");
    ui->advanced_emailrdv->setPlaceholderText("Rechercher par l'email du patient");

    QIcon icon,icon1,iconMail,iconPdf,advancedIcon;
    QPixmap qpm,qpm1,qpm2,qpm3,advanced;
    QPixmap qpmDelete,qpmMail;

    if(advanced.load(":/pics/pics/advanced.png"))
    {
        advancedIcon.addPixmap(advanced);
        ui->advanced_button->setIcon(advancedIcon);
        ui->advancedButton_rdv->setIcon(advancedIcon);
    }
    if(qpm3.load(":/pics/pics/pdf_icon1.png"))
    {
        iconPdf.addPixmap(qpm3);
        ui->pdf_icon->setIcon(iconPdf);
    }
    if(qpmMail.load(":/pics/pics/email_closed.png"))
    {
        iconMail.addPixmap(qpmMail);
        ui->email_sending->setIcon(iconMail);
    }
    if(qpmDelete.load(":/pics/pics/trash.png"))
    {
        icon1.addPixmap(qpmDelete);
        ui->DeleteButton->setIcon(icon1);
        ui->DeleteButton_2->setIcon(icon1);
    }

    if(qpm.load(":/pics/pics/Patient.png"))
    {
        icon.addPixmap(qpm);
        ui->Patient_Button->setIcon(icon);
        ui->Patient_Button_2->setIcon(icon);
    }
    if(qpm1.load(":/pics/pics/Home.png"))
    {
        icon.addPixmap(qpm1);
        ui->Home_Button->setIcon(icon);
        ui->Home_Button_2->setIcon(icon);
    }
    if(qpm2.load(":/pics/pics/RendezVous.png"))
    {
        icon.addPixmap(qpm2);
        ui->RendezVous_Button->setIcon(icon);
        ui->RendezVous_Button_2->setIcon(icon);
    }

    QMovie *movie = new QMovie(":/pics/pics/logo_gif.gif");
    ui->Logo_gif->setMovie(movie);
    movie->setScaledSize(ui->Logo_gif->size());
    movie->start();
}

void Menu::smallLineAnimationPatient()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->line_smallPatient, "geometry");
    animation->setDuration(500);
    animation->setStartValue(QRect(ui->line_smallPatient->geometry().x()+(ui->line_smallPatient->geometry().width()/2),ui->line_smallPatient->geometry().y(),10,3));
    animation->setEndValue(ui->line_smallPatient->geometry());
    animation->start();
}

void Menu::smallLineAnimationRendezVous()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->line_smallRendezVous, "geometry");
    animation->setDuration(500);
    animation->setStartValue(QRect(ui->line_smallRendezVous->geometry().x()+(ui->line_smallRendezVous->geometry().width()/2),ui->line_smallRendezVous->geometry().y(),10,3));
    animation->setEndValue(ui->line_smallRendezVous->geometry());
    animation->start();
}

void Menu::bigLineAnimationPatient()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->line_bigPatient, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(-500,ui->line_bigPatient->geometry().y(),500,3));
    animation->setEndValue(ui->line_bigPatient->geometry());
    QEasingCurve curve;
    curve.setType(QEasingCurve::OutBounce);
    animation->setEasingCurve(curve);
    animation->start();
}

void Menu::bigLineAnimationRendezVous()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->line_bigRendezVous, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(-500,ui->line_bigRendezVous->geometry().y(),500,3));
    animation->setEndValue(ui->line_bigRendezVous->geometry());
    QEasingCurve curve;
    curve.setType(QEasingCurve::OutBounce);
    animation->setEasingCurve(curve);
    animation->start();
}

void Menu::initialiserPatient()
{
    initialiserErrorsPatient();
    ui->ajouterPatient->setText("Ajouter");
    setWindowTitle("Hoptimal - Liste des patients");
    ui->pdf_icon->setVisible(false);
    ui->DeleteButton->setVisible(false);
    ui->cinPatient->setText("");
    ui->nomPatient->setText("");
    ui->prenomPatient->setText("");
    ui->DateNaissancePatient->setDate(QDate::fromString("20000821", "yyyyMMdd"));
    ui->numChambrePatient->setText("");
    qDebug("init patient");
    if(rank==1){
        ui->NewPatient_button->setVisible(false);
    }
    else{
        ui->NewPatient_button->setVisible(true);
        ui->ajouterPatient->setVisible(true);
        ui->DeleteButton->setVisible(false);
        ui->cinPatient->setReadOnly(false);
        ui->nomPatient->setReadOnly(false);
        ui->prenomPatient->setReadOnly(false);
        ui->DateNaissancePatient->setReadOnly(false);
        ui->numChambrePatient->setReadOnly(false);
    }
}

void Menu::afficherPatient()
{
    setWindowTitle("Hoptimal - Modifier un patient");
    ui->pdf_icon->setVisible(true);
    ui->DeleteButton->setVisible(true);
    ui->ajouterPatient->setVisible(true);
    ui->cinPatient->setText(P.getCIN());
    ui->nomPatient->setText(P.getNom());
    ui->prenomPatient->setText(P.getPrenom());
    ui->DateNaissancePatient->setDate(P.getDateNaissance());
    ui->numChambrePatient->setText(P.getNumChambre());

    if(this->rank==-1){
        ui->ajouterPatient->setVisible(true);
        ui->DeleteButton->setVisible(true);
        ui->ajouterPatient->setText("Modifier");
        ui->cinPatient->setReadOnly(false);
        ui->nomPatient->setReadOnly(false);
        ui->prenomPatient->setReadOnly(false);
        ui->DateNaissancePatient->setReadOnly(false);
        ui->numChambrePatient->setReadOnly(false);
    }
    else if(this->rank==1){
        ui->ajouterPatient->setVisible(false);
        ui->DeleteButton->setVisible(false);
        ui->cinPatient->setReadOnly(true);
        ui->nomPatient->setReadOnly(true);
        ui->prenomPatient->setReadOnly(true);
        ui->DateNaissancePatient->setReadOnly(true);
        ui->numChambrePatient->setReadOnly(true);
    }
}

void Menu::initialiserRDV(){
    initialiserErrorsRDV();
    ui->ajouterRendezVous->setText("Ajouter");
    ui->email_sending->setVisible(false);
    ui->QrButton->setVisible(false);
    ui->DeleteButton_2->setVisible(false);
    setWindowTitle("Hoptimal - Liste des rendez vous");
    ui->idRendezVous->setText("");
    ui->emailPatientRendezVous->setText("");
    ui->nomPatientRendezVous->setText("");
    ui->prenomPatientRendezVous->setText("");
    ui->dateHeureRendezVous->setDateTime(QDateTime::currentDateTime());
    qDebug("init RDV");
    if(rank==1)
        ui->newRDV_button->setVisible(false);
    else{
        ui->newRDV_button->setVisible(true);
        ui->ajouterRendezVous->setVisible(true);
        ui->emailPatientRendezVous->setReadOnly(false);
        ui->nomPatientRendezVous->setReadOnly(false);
        ui->prenomPatientRendezVous->setReadOnly(false);
        ui->dateHeureRendezVous->setReadOnly(false);
    }
}

void Menu::afficherRDV(){

    setWindowTitle("Hoptimal - Modifier un rendez vous");
    ui->email_sending->setVisible(true);
    ui->QrButton->setVisible(true);
    ui->DeleteButton_2->setVisible(true);
    ui->ajouterRendezVous->setVisible(true);
    ui->idRendezVous->setText(R.getId());
    ui->emailPatientRendezVous->setText(R.getEmail());
    ui->nomPatientRendezVous->setText(R.getNom());
    ui->prenomPatientRendezVous->setText(R.getPrenom());
    ui->dateHeureRendezVous->setDateTime(R.getDateTime());
    if(rank==-1){
        ui->ajouterRendezVous->setText("Modifier");
        ui->ajouterRendezVous->setVisible(true);
        ui->DeleteButton_2->setVisible(true);
        ui->emailPatientRendezVous->setReadOnly(false);
        ui->nomPatientRendezVous->setReadOnly(false);
        ui->prenomPatientRendezVous->setReadOnly(false);
        ui->dateHeureRendezVous->setReadOnly(false);
    }
    else if (rank==1){
        ui->ajouterRendezVous->setVisible(false);
        ui->DeleteButton_2->setVisible(false);
        ui->emailPatientRendezVous->setReadOnly(true);
        ui->nomPatientRendezVous->setReadOnly(true);
        ui->prenomPatientRendezVous->setReadOnly(true);
        ui->dateHeureRendezVous->setReadOnly(true);
    }
}
//slots

void Menu::on_menu_listeRendezVous_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    smallLineAnimationRendezVous();
    bigLineAnimationRendezVous();
}

void Menu::on_menu_listePatients_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    smallLineAnimationPatient();
    bigLineAnimationPatient();
}

void Menu::on_rechercheRendezVous_textChanged()
{
    refreshDBRdv();
}

void Menu::on_recherchePatient_textChanged()
{
    refreshDBPatient();
}

void Menu::on_Home_Button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_Home_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_RendezVous_Button_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    smallLineAnimationRendezVous();
    bigLineAnimationRendezVous();
}

void Menu::on_Patient_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    smallLineAnimationPatient();
    bigLineAnimationPatient();
}



void Menu::on_listPatient_doubleClicked(const QModelIndex &index)
{
    int row=index.row();
    int occurence=0;
    while(index.data().toString()==index.sibling(--row,index.column()).data().toString())
         occurence++;

    ui->recherchePatient->isEnabled() ? P.afficher(triPatient(),index.data().toString(),occurence) : P.afficher(triPatient(),occurence,ui->advanced_cin->text(),ui->advanced_nom->text(),ui->advanced_prenom->text(),ui->advanced_chambre->text());
    afficherPatient();
    ui->stackedWidget->setCurrentIndex(5);

}

void Menu::on_listRendezVous_doubleClicked(const QModelIndex &index)
{
    int occurence=0;
    int row=index.row();

    while(index.data().toString()==index.sibling(--row,index.column()).data().toString())
        occurence++;

    ui->recherchePatient->isEnabled() ? R.afficher(triRDV(),index.data().toString(),occurence) : R.afficher(triRDV(),occurence,ui->advanced_cinrdv->text(),ui->advanced_nomrdv->text(),ui->advanced_prenomrdv->text(),ui->advanced_emailrdv->text());



    afficherRDV();
    ui->stackedWidget->setCurrentIndex(6);
}

void Menu::on_triPatient_currentTextChanged()
{
    if(!advancedPatient_bool){
    ui->recherchePatient->setText("");
    refreshDBPatient();
    }
    else
        advancedPatient();
}

void Menu::on_triRDV_currentTextChanged()
{
    if(!advancedRDV_bool){
    ui->rechercheRendezVous->setText("");
    refreshDBRdv();
    }
    else
       advancedRDV();

}

//patient slots

void Menu::on_ajouterPatient_clicked()
{
    //affecting inputs into variables to test on them
    QString nom=ui->nomPatient->text();
    QString prenom=ui->prenomPatient->text();
    QString CIN=ui->cinPatient->text();
    QString numChambre=ui->numChambrePatient->text();
    QDate dateNaissance=ui->DateNaissancePatient->date();

    //empty fields verifications

    if(CIN.size()==0 || nom.size()==0 || prenom.size()==0 || numChambre.size()==0){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Impossible de laisser des champs vides.");
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else{

        //number check and testing
        QIntValidator v(0,1316134911,this);
        int pos=0;
        if(CIN=="00000000"){
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Le CIN ne peut pas être 00000000.");
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        else{
            if(!v.validate(CIN,pos)){
                //if u have letters in the cin
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText("Vous ne pouvez pas avoir des lettres dans le CIN.");
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
            }
            else{
                //cin length verification
                if(CIN.size()==8){
                    if(P.chercherCin(CIN,ui->ajouterPatient->text())==false){
                        //affecting variables to class if everything went fine

                        if(ui->ajouterPatient->text()=="Ajouter"){
                            if(P.addValuesToDB(nom,prenom,dateNaissance,CIN,numChambre)){
                                ui->stackedWidget->setCurrentIndex(3);
                                refreshDBPatient();
                                initialiserPatient();
                            }
                        }
                        else{
                            if(P.modifyValues(nom,prenom,dateNaissance,CIN,numChambre,P.getCIN())){
                                ui->stackedWidget->setCurrentIndex(3);
                                refreshDBPatient();
                                initialiserPatient();
                            }
                        }
                    }
                    else{
                        QMessageBox msgBox;
                        msgBox.setIcon(QMessageBox::Critical);
                        msgBox.setText("Le CIN doit être unique.");
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        msgBox.exec();
                    }
                }
                else{
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Critical);
                    msgBox.setText("La longueur du CIN doit être de 8.");
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                }
            }
        }
    }
}

void Menu::on_Annuler_clicked()
{
    initialiserPatient();
    ui->stackedWidget->setCurrentIndex(3);

}

void Menu::on_DeleteButton_pressed()
{
    QIcon icon;
    QPixmap qpm;

    if(qpm.load(":/pics/pics/trashOpen.png"))
    {
        icon.addPixmap(qpm);
        ui->DeleteButton->setIcon(icon);
    }
    QSound::play(":/pics/pics/trashmp3.wav");
}

void Menu::on_DeleteButton_released()
{
    QIcon icon;
    QPixmap qpm;

    if(qpm.load(":/pics/pics/trash.png"))
    {
        icon.addPixmap(qpm);
        ui->DeleteButton->setIcon(icon);
    }
}

void Menu::on_DeleteButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("Etes-vous sûr de vouloir supprimer ce patient ?");
    msgBox.setInformativeText("Vous ne pouvez plus récuperer ses informations.");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec()== QMessageBox::Yes){
        P.Delete();
        ui->stackedWidget->setCurrentIndex(3);
        refreshDBPatient();
    }
}


//RDV slots

void Menu::on_ajouterRendezVous_clicked()
{
    //empty fields verification
    if(ui->emailPatientRendezVous->text().size()==0 || ui->nomPatientRendezVous->text().size()==0 || ui->prenomPatientRendezVous->text().size()==0){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Impossible de laisser des champs vides.");
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    else{
        //email verification
        if(!R.isEmailAddress(ui->emailPatientRendezVous->text())){
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Email invalide.");
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        else{
            //affecting input to my class

            if(ui->ajouterRendezVous->text()=="Ajouter"){
                if(R.addValuesToDB(ui->nomPatientRendezVous->text(),ui->prenomPatientRendezVous->text(),ui->idRendezVous->text(),ui->emailPatientRendezVous->text(),ui->dateHeureRendezVous->dateTime()))
                {
                    ui->stackedWidget->setCurrentIndex(4);
                    refreshDBRdv();
                    initialiserRDV();
                }
            }
            else{
                if(R.modifyValues(ui->nomPatientRendezVous->text(),ui->prenomPatientRendezVous->text(),ui->idRendezVous->text(),ui->emailPatientRendezVous->text(),ui->dateHeureRendezVous->dateTime()))
                {
                    ui->stackedWidget->setCurrentIndex(4);
                    refreshDBRdv();
                    initialiserRDV();
                }
            }

        }
    }
}

void Menu::on_annuler_clicked()
{
    initialiserRDV();
    ui->stackedWidget->setCurrentIndex(4);
}

void Menu::on_DeleteButton_2_pressed()
{
    QIcon icon;
    QPixmap qpm;

    if(qpm.load(":/pics/pics/trashOpen.png"))
    {
        icon.addPixmap(qpm);
        ui->DeleteButton_2->setIcon(icon);
    }
    QSound::play(":/pics/pics/trashmp3.wav");
}

void Menu::on_DeleteButton_2_released()
{
    QIcon icon;
    QPixmap qpm;

    if(qpm.load(":/pics/pics/trash.png"))
    {
        icon.addPixmap(qpm);
        ui->DeleteButton_2->setIcon(icon);
    }
}

void Menu::on_DeleteButton_2_clicked()
{

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("Etes-vous sûr de vouloir supprimer ce rendez vous ?");
    msgBox.setInformativeText("Vous ne pouvez plus récuperer ses informations.");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec()== QMessageBox::Yes){
        R.Delete();
        ui->stackedWidget->setCurrentIndex(4);
        refreshDBRdv();
    }
}

void Menu::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1==1 || arg1==2 || arg1==3 || arg1==4){
        initialiserRDV();
        initialiserPatient();
        setWindowTitle("Hoptimal");
    }
    if(arg1 == 11 && this->loggedIn)
    {
        if(QFile::exists(this->picDir+"/"+this->username))
        {
            ui->rpLabel_2->setText("<img src=\"file:///"+this->picDir+"/"+this->username+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
        }
    }

    if(this->loggedIn)
    {
        if(QFile::exists(this->picDir+"/"+this->username))
        {
            ui->loggedPic->setText("<img src=\"file:///"+this->picDir+"/"+this->username+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
        }

        QSqlQuery fetcher;
        fetcher.prepare("SELECT * FROM sys_users WHERE username = (:un)");
        fetcher.bindValue(":un", this->username);
        fetcher.exec();

        int idFname = fetcher.record().indexOf("fname");
        int idMname = fetcher.record().indexOf("mname");
        int idLname = fetcher.record().indexOf("lname");
        int idRank = fetcher.record().indexOf("rank");
        int idEmail = fetcher.record().indexOf("email");

        QString fullname, email, rank;

        while (fetcher.next())
        {
            fullname = fetcher.value(idFname).toString();
            fullname += " " + fetcher.value(idMname).toString();
            fullname += " " + fetcher.value(idLname).toString();
            rank = fetcher.value(idRank).toString();
            email = fetcher.value(idEmail).toString();
        }
        if(rank == "-1")
        {
            ui->adminButton->setVisible(true);
        }
        ui->nameLabel->setText(fullname);
        ui->rankLabel->setText(rank);
        ui->emailLabel_2->setText(email);
    }

    if(arg1 == 12 && this->loggedIn)
    {
        ui->stackedWidget->setCurrentIndex(12);
    }
}

void Menu::on_email_sending_clicked()
{
    R.send_email(R.getEmail(),R.getDateTime().date().toString("dd/MM/yyyy"),R.getDateTime().time().toString("hh:mm"),R.getPrenom());
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Un email a été envoyé.");
    msgBox.setDefaultButton(QMessageBox::Ok);
    if(msgBox.exec()==QMessageBox::Ok)
        ui->stackedWidget->setCurrentIndex(4);
}

void Menu::on_email_sending_pressed()
{
    QIcon icon;
    QPixmap qpm;

    if(qpm.load(":/pics/pics/email_open.png"))
    {
        icon.addPixmap(qpm);
        ui->email_sending->setIcon(icon);
    }
}

void Menu::on_email_sending_released()
{
    QIcon icon;
    QPixmap qpm;

    if(qpm.load(":/pics/pics/email_closed.png"))
    {
        icon.addPixmap(qpm);
        ui->email_sending->setIcon(icon);
    }
}

void Menu::on_pdf_icon_clicked()
{
    P.printPDF();
    ui->stackedWidget->setCurrentIndex(3);
}

void Menu::on_NewPatient_button_clicked()
{
    initialiserPatient();
    ui->stackedWidget->setCurrentIndex(5);
}

void Menu::on_newRDV_button_clicked()
{
    initialiserRDV();
    ui->stackedWidget->setCurrentIndex(6);
}

void Menu::on_nomPatient_textChanged(const QString &arg1)
{
    if(arg1==""){
        ui->Nom_error->setText("Ce champ ne peut pas rester vide!");
    }
    else
        ui->Nom_error->setText("");
}

void Menu::on_prenomPatient_textChanged(const QString &arg1)
{
    if(arg1==""){
        ui->Prenom_error->setText("Ce champ ne peut pas rester vide!");
    }
    else
        ui->Prenom_error->setText("");
}

void Menu::on_numChambrePatient_textChanged(const QString &arg1)
{
    if(arg1==""){
        ui->numChambre_error->setText("Ce champ ne peut pas rester vide!");
    }
    else
        ui->numChambre_error->setText("");
}

void Menu::on_cinPatient_textChanged(const QString &arg1)
{
    if(arg1=="")
            ui->Cin_error->setText("Ce champ ne peut pas rester vide!");
    else if(arg1.length()!=8){
        ui->Cin_error->setText("Le CIN doit être composé de 8 chiffres!");
    }
    else
        ui->Cin_error->setText("");
}

void Menu::on_advanced_cin_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedPatient();
}

void Menu::on_advanced_nom_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedPatient();
}

void Menu::on_advanced_prenom_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedPatient();
}

void Menu::on_advanced_chambre_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedPatient();
}



void Menu::on_advanced_button_clicked()
{
    advancedPatient_bool=!advancedPatient_bool;
    if(advancedPatient_bool){
        ui->advanced_2->setVisible(true);
        ui->recherchePatient->setEnabled(false);
        ui->recherchePatient->setText("");
        ui->recherchePatient->setStyleSheet("background-color:rgba(0, 33, 52, 60);");
    }
    else{
        ui->advanced_2->setVisible(false);
        ui->recherchePatient->setEnabled(true);
        ui->advanced_cin->setText("");
        ui->advanced_nom->setText("");
        ui->advanced_prenom->setText("");
        ui->advanced_chambre->setText("");
        ui->recherchePatient->setStyleSheet("background-color:rgba(0, 33, 52, 100);");
    }
}

void Menu::on_advanced_cinrdv_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedRDV();
}

void Menu::on_advanced_nomrdv_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedRDV();
}

void Menu::on_advanced_prenomrdv_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedRDV();
}

void Menu::on_advanced_emailrdv_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    advancedRDV();
}

void Menu::on_advancedButton_rdv_clicked()
{
    advancedRDV_bool=!advancedRDV_bool;
    if(advancedRDV_bool){
        ui->advanced_rdv->setVisible(true);
        ui->rechercheRendezVous->setEnabled(false);
        ui->rechercheRendezVous->setText("");
        ui->rechercheRendezVous->setStyleSheet("background-color:rgba(0, 33, 52, 60);");
    }
    else{
        ui->advanced_rdv->setVisible(false);
        ui->rechercheRendezVous->setEnabled(true);
        ui->advanced_cinrdv->setText("");
        ui->advanced_nomrdv->setText("");
        ui->advanced_emailrdv->setText("");
        ui->advanced_prenomrdv->setText("");
        ui->rechercheRendezVous->setStyleSheet("background-color:rgba(0, 33, 52, 100);");
    }
}


void Menu::on_nomPatientRendezVous_textChanged(const QString &arg1)
{
    if(arg1==""){
        ui->nom_error_rdv->setText("Ce champ ne peut pas rester vide!");
    }
    else
        ui->nom_error_rdv->setText("");
}

void Menu::on_prenomPatientRendezVous_textChanged(const QString &arg1)
{
    if(arg1==""){
        ui->prenom_error_rdv->setText("Ce champ ne peut pas rester vide!");
    }
    else
        ui->prenom_error_rdv->setText("");
}


void Menu::on_ReunionMG_clicked()
{
    R1.nickname=this->username;
    R1.exec();
}

void Menu::on_regButton_clicked()
{
    ui->uBox->setText(ui->usernameBox->text());
    ui->pBox->setText(ui->passwordBox->text());
    ui->stackedWidget->setCurrentIndex(7);
}

void Menu::on_backButton_clicked()
{
    ui->loginLabel->setText("");
    ui->stackedWidget->setCurrentIndex(0);
}

void Menu::on_completeRegButton_clicked()
{
    bool halt = false;

    if(ui->uBox->text() == "")
    {
        ui->uBox->setPlaceholderText("Username EMPTY!");
        halt = true;
    }

    if(ui->pBox->text() == "")
    {
        ui->pBox->setPlaceholderText("Password EMPTY!");
        halt = true;
    }

    if(ui->eBox->text() == "")
    {
        ui->eBox->setPlaceholderText("E-mail EMPTY!");
        halt = true;
    }

    if(ui->fBox->text() == "")
    {
        ui->fBox->setPlaceholderText("First Name EMPTY!");
        halt = true;
    }

    if(ui->mBox->text() == "")
    {
        ui->mBox->setPlaceholderText("Middle Name (optional)");
        halt = false;
    }

    if(ui->lBox->text() == "")
    {
        ui->lBox->setPlaceholderText("Last Name EMPTY!");
        halt = true;
    }

    QSqlQuery cQuery;
    cQuery.prepare("SELECT username FROM sys_users WHERE username = (:un)");
    cQuery.bindValue(":un", ui->uBox->text());

    if(cQuery.exec())
    {
        if(cQuery.next())
        {
            ui->uBox->setText("");
            ui->uBox->setPlaceholderText("Choose a different Username!");
            halt = true;
        }
    }

    QSqlQuery cQuery2;
    cQuery2.prepare("SELECT email FROM sys_users WHERE email = (:em)");
    cQuery2.bindValue(":em", ui->eBox->text());

    if(cQuery2.exec())
    {
        if(cQuery2.next())
        {
            ui->eBox->setText("");
            ui->eBox->setPlaceholderText("Use another E-mail!");
            halt = true;
        }
    }


    if(halt)
    {
        ui->regLabel->setText("Please correct your mistakes.");
    }
    else
    {
        if (this->picName != "")
        {
            QString to = this->picDir+"/"+ui->uBox->text();

            if (QFile::exists(to))
            {
                QFile::remove(to);
            }

            QFile::copy(this->picName, to);
            this->picName = "";
        }

        ui->regLabel->setText("");
        QSqlQuery iQuery;
        iQuery.prepare("INSERT INTO sys_users(username, passwd, fname, mname, lname, email)"\
                       "VALUES(:un, :pw, :fn, :mn, :ln, :em)");
        iQuery.bindValue(":un", ui->uBox->text());
        iQuery.bindValue(":pw", ui->pBox->text());
        iQuery.bindValue(":fn", ui->fBox->text());
        iQuery.bindValue(":mn", ui->mBox->text());
        iQuery.bindValue(":ln", ui->lBox->text());
        iQuery.bindValue(":em", ui->eBox->text());

        if(iQuery.exec())
        {
            ui->uBox->setText("");
            ui->pBox->setText("");
            ui->eBox->setText("");
            ui->fBox->setText("");
            ui->mBox->setText("");
            ui->lBox->setText("");
            ui->rpLabel->setText("<img src=\":user.png\" />");
            ui->loginLabel->setText("Registration Successful! You can now login.");
            ui->stackedWidget->setCurrentIndex(0);
        }

    }
}

void Menu::on_uplButton_clicked()
{
    this->picName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->rpLabel->setText("<img src=\"file:///"+this->picName+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
}

void Menu::on_loginButton_clicked()
{



    this->loggedIn = loginPage(ui->usernameBox->text(), ui->passwordBox->text());

    if(this->loggedIn)
    {
        this->username = ui->usernameBox->text();
        this->password = ui->passwordBox->text();

        ui->loginLabel->setText("");
        ui->stackedWidget->setCurrentIndex(1);
        this->rank=ui->rankLabel->text().toInt();
        volume=true;
        player->play();

    }
    else
    {
        ui->loginLabel->setText("Login failed: Invalid credentials!");

    }
}
bool Menu::loginPage(QString u, QString p)
{
   ui->adminButton->setVisible(false);

    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT username FROM sys_users WHERE username = (:un) AND passwd = (:pw)");
    checkQuery.bindValue(":un", u);
    checkQuery.bindValue(":pw", p);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }

    return exists;
}


void Menu::on_gestionPersonnelMG_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void Menu::on_ajouteremploye_2_clicked()
{
    bool halt = false;
    Q_UNUSED(halt);
    if(ui->lineEdit_username->text() == "")
    {
        ui->lineEdit_username->setPlaceholderText("Username EMPTY!");
        halt = true;
    }

    if(ui->lineEdit_email->text() == "")
    {
        ui->lineEdit_email->setPlaceholderText("Email EMPTY!");
        halt = true;
    }

    if(ui->lineEdit_lname->text() == "")
    {
        ui->lineEdit_lname->setPlaceholderText("Lname (optional)");
        halt = false;
    }

    if(ui->lineEdit_mname->text() == "")
    {
        ui->lineEdit_mname->setPlaceholderText("Mname EMPTY!");
        halt = true;
    }

    if(ui->lineEdit_fname->text() == "")
    {
        ui->lineEdit_fname->setPlaceholderText("Fname EMPTY !");
        halt = false;
    }

    if(ui->lineEdit_ID->text() == "")
    {
        ui->lineEdit_ID->setPlaceholderText("ID EMPTY!");
        halt = true;
    }

    QSqlQuery query;
   QString username=ui->lineEdit_username->text();
   QString mname=ui->lineEdit_mname->text();
   QString lname=ui->lineEdit_lname->text();
   QString fname=ui->lineEdit_fname->text();
   QString email=ui->lineEdit_email->text();
   int id=ui->lineEdit_ID->text().toInt();

       Employe E(username,lname,mname,fname,email,id);
       bool test =E.ajouter();
       Q_UNUSED(test);
       if(!query.exec()){
           QMessageBox::information(nullptr, QObject::tr("effectue."),
                       QObject::tr(" Effectué."), QMessageBox::Ok);
           ui->tabemploye->setModel(Etmp.afficher());
           ui->listView->setModel(E.afficher());


       }
       else{
           qDebug("Values added to DB");

       }
}


void Menu::on_pushButton_4_clicked()
{
    ui->tabemploye->setModel(E.afficherOrderedList());
}

void Menu::on_checkBox_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
}

void Menu::on_checkBox_2_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(2);
}

void Menu::on_pushButton_clicked()
{
    close();
}

void Menu::on_Print_clicked()
{

    QString strStream;
    QTextStream out(&strStream);
    QPrinter printer;
    QPainter painter;


    const int rowCount = ui->tabemploye->model()->rowCount();
    const int columnCount = ui->tabemploye->model()->columnCount();

    out <<  "<html>\n"
        "<head>\n"
        "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        <<  QString("<title>%1</title>\n").arg("GestionEmploye")
        <<  "</head>\n"
        "<body bgcolor=#cae9ff link=#5000A0>\n"
        "<table border=1 cellspacing=0 cellpadding=2>\n";



    // headers
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (ui->tabemploye->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(ui->tabemploye->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->tabemploye->isColumnHidden(column)) {
                QString data =ui->tabemploye->model()->data(ui->tabemploye->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
        "</body>\n"
        "</html>\n";



    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);




    QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }

    delete document;
}

void Menu::on_recherche_clicked()
{
    QString a=ui->lineEdit_ID_2->text();
       if(a!="")
       { ui->listView->setModel(E.recherche(a));


}
}

void Menu::on_commandLinkButton_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(1);
}

void Menu::on_addemploye_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void Menu::on_reportemploye_clicked()
{
    ui->tabemploye->setModel(Etmp.afficher());
    ui->stackedWidget_2->setCurrentIndex(1);
}

void Menu::on_employeelist_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void Menu::on_menu_listePatients_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void Menu::on_uplButton_2_clicked()
{
    this->picName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->label->setText("<img src=\"file:///"+this->picName+"\" alt=\"Image read error!\" height=\"128\" width=\"128\" />");
}

void Menu::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_LogoutMG_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Menu::on_editButton_clicked()
{
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM sys_users WHERE username = (:un) AND passwd = (:pw)");
    fetcher.bindValue(":un", this->username);
    fetcher.bindValue(":pw", this->password);
    fetcher.exec();

    int idUsername = fetcher.record().indexOf("username");
    int idPasswd = fetcher.record().indexOf("passwd");
    int idEmail = fetcher.record().indexOf("email");
    int idFname = fetcher.record().indexOf("fname");
    int idMname = fetcher.record().indexOf("mname");
    int idLname = fetcher.record().indexOf("lname");

    while (fetcher.next())
    {
        ui->uBox_2->setText(fetcher.value(idUsername).toString());
        ui->pBox_2->setText(fetcher.value(idPasswd).toString());
        ui->eBox_2->setText(fetcher.value(idEmail).toString());
        ui->fBox_2->setText(fetcher.value(idFname).toString());
        ui->mBox_2->setText(fetcher.value(idMname).toString());
        ui->lBox_2->setText(fetcher.value(idLname).toString());
    }

    ui->stackedWidget->setCurrentIndex(11);
}

void Menu::on_menu_listeRendezVous_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void Menu::on_logoutButton_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                       "Login System", "Are you sure you want to logout?",
                                       QMessageBox::Yes|QMessageBox::No).exec())
    {
        this->loggedIn = false;
        ui->passwordBox->setText("");
        ui->loginLabel->setText("You signed out!");
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void Menu::on_delButton_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                       "Login System", "Are you sure you want to delete your account?",
                                       QMessageBox::Yes|QMessageBox::No).exec())
    {
        QString to = this->picDir+"/"+this->username;

        if (QFile::exists(to))
        {
            QFile::remove(to);
        }

        QSqlQuery dQuery;
        dQuery.prepare("DELETE FROM sys_users WHERE username = (:un)");
        dQuery.bindValue(":un", this->username);

        if(dQuery.exec())
        {
            ui->usernameBox->setText("");
            ui->passwordBox->setText("");
            ui->loginLabel->setText("Account deleted!");
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
}

void Menu::on_backButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void Menu::on_editedButton_clicked()
{
    bool halt = false;

    if(ui->uBox_2->text() == "")
    {
        ui->uBox_2->setPlaceholderText("Username EMPTY!");
        halt = true;
    }

    if(ui->pBox_2->text() == "")
    {
        ui->pBox_2->setPlaceholderText("Password EMPTY!");
        halt = true;
    }

    if(ui->eBox_2->text() == "")
    {
        ui->eBox_2->setPlaceholderText("E-mail EMPTY!");
        halt = true;
    }

    if(ui->fBox_2->text() == "")
    {
        ui->fBox_2->setPlaceholderText("First Name EMPTY!");
        halt = true;
    }

    if(ui->mBox_2->text() == "")
    {
        ui->mBox_2->setPlaceholderText("Middle Name (optional)");
        halt = false;
    }

    if(ui->lBox_2->text() == "")
    {
        ui->lBox_2->setPlaceholderText("Last Name EMPTY!");
        halt = true;
    }

    QSqlQuery cQuery;
    cQuery.prepare("SELECT username FROM sys_users WHERE username = (:un)");
    cQuery.bindValue(":un", ui->uBox->text());

    if(cQuery.exec())
    {
        if(cQuery.next() && ui->uBox_2->text() != cQuery.value(0).toString())
        {
            ui->uBox_2->setText("");
            ui->uBox_2->setPlaceholderText("Choose a different Username!");
            halt = true;
        }
    }

    QSqlQuery cQuery2;
    cQuery2.prepare("SELECT email FROM sys_users WHERE email = (:em)");
    cQuery2.bindValue(":em", ui->eBox_2->text());

    if(cQuery2.exec())
    {
        if(cQuery2.next() && ui->eBox_2->text() != cQuery2.value(0).toString())
        {
            ui->eBox_2->setText("");
            ui->eBox_2->setPlaceholderText("Use another E-mail!");
            halt = true;
        }
    }


    if(halt)
    {
        ui->regLabel_2->setText("Please correct your mistakes.");
    }
    else
    {
        if (this->picName != "")
        {
            QString to = this->picDir+"/"+ui->uBox_2->text();

            if (QFile::exists(to))
            {
                QFile::remove(to);
            }

            QFile::copy(this->picName, to);
            this->picName = "";
        }

        ui->regLabel_2->setText("");
        QSqlQuery iQuery;
        iQuery.prepare("UPDATE sys_users SET username=(:un), passwd=(:pw), fname=(:fn), mname=(:mn), lname=(:ln), email=(:em) WHERE username=(:uno)");
        iQuery.bindValue(":un", ui->uBox_2->text());
        iQuery.bindValue(":pw", ui->pBox_2->text());
        iQuery.bindValue(":fn", ui->fBox_2->text());
        iQuery.bindValue(":mn", ui->mBox_2->text());
        iQuery.bindValue(":ln", ui->lBox_2->text());
        iQuery.bindValue(":em", ui->eBox_2->text());
        iQuery.bindValue(":uno", ui->uBox_2->text());

        if(iQuery.exec())
        {
            ui->stackedWidget->setCurrentIndex(0);
        }

    }
}

void Menu::on_userBrowse_clicked()
{
    ui->stackedWidget_5->setCurrentIndex(0);
}

void Menu::on_adminBrowse_clicked()
{
     ui->stackedWidget_5->setCurrentIndex(1);
}

void Menu::on_pageButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void Menu::on_delUButton_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                           "Login System", "Are you sure you want to erase all accounts?",
                                           QMessageBox::Yes|QMessageBox::No).exec())
    {
        QSqlQuery dQuery;
        dQuery.prepare("DELETE FROM sys_users WHERE rank != 0 AND rank != -1");

        if(dQuery.exec())
        {
            ui->adminLabel->setText("Query executed!");
        }
    }
}

void Menu::on_delAButton_clicked()
{
    if(QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                               "Login System", "Are you sure you want to erase all administrators?"\
                                               "\n(This won't erase regular users and you)",
                                               QMessageBox::Yes|QMessageBox::No).exec())
        {
            QSqlQuery dQuery;
            dQuery.prepare("DELETE FROM sys_users WHERE rank != 1 AND username != \"" + this->username + "\"");

            if(dQuery.exec())
            {
                ui->adminLabel->setText("Query executed!");
            }
        }
}

void Menu::on_backButton_5_clicked()
{
    this->tblMdl->revertAll();
    this->tblMdl->database().rollback();
}

void Menu::on_editedButton_2_clicked()
{
    if(this->tblMdl->submitAll())
    {
        this->tblMdl->database().commit();
        ui->adminLabel->setText("Saved to database!");
    }
    else
    {
        this->tblMdl->database().rollback();
    }
}

void Menu::on_stackedWidget_5_currentChanged(int arg1)
{
    if(arg1 == 0 && this->loggedIn)
    {
        ui->headLabel->setText("USERS");
        this->tblMdl = new QSqlTableModel;
        this->tblMdl->setTable("sys_users");
        this->tblMdl->setFilter("rank != -1 AND rank != 0");
        this->tblMdl->select();
        ui->tableView->setModel(this->tblMdl);
        this->tblMdl->database().transaction();
    }

    if(arg1 == 1 && this->loggedIn)
    {
        ui->headLabel->setText("ADMINS");
        this->tblMdl = new QSqlTableModel;
        this->tblMdl->setTable("sys_users");
        this->tblMdl->setFilter("rank == -1 OR rank == 0");
        this->tblMdl->select();
        ui->tableView_2->setModel(this->tblMdl);
        this->tblMdl->database().transaction();
    }
}



void Menu::on_adminButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

void Menu::on_pushButton_5_clicked()
{
    ui->tabemploye->setModel(Etmp.afficher());
}

void Menu::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_checkBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
  ui->stackedWidget_3->setCurrentIndex(1);
}

void Menu::on_checkBox_2_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->stackedWidget_3->setCurrentIndex(2);
}

void Menu::on_Delete_clicked()
{
    QSqlQuery querry;
    querry.prepare("DELETE FROM emp where ID=:id");
    querry.bindValue(":id",ui->listView->currentIndex().data().toInt());

    if (querry.exec())
    {
        QString a=ui->lineEdit_ID_2->text();
           if(a!="")
            ui->listView->setModel(E.recherche(a));
    }


}

//////////////////////////////////////////////integration hejer Gestion Infrastructure

void Menu::smallLineAnimationEquipement(){
    QPropertyAnimation *animation = new QPropertyAnimation(ui->line_equi, "geometry");
    animation->setDuration(500);
    animation->setStartValue(QRect(ui->line_equi->geometry().x()+(ui->line_equi->geometry().width()/2),40,10,3));
    animation->setEndValue(ui->line_equi->geometry());
    animation->start();
}
void Menu::smallLineAnimationMedicament(){
    QPropertyAnimation *animation = new QPropertyAnimation(ui->line_med_3, "geometry");
    animation->setDuration(500);
    animation->setStartValue(QRect(ui->line_med_3->geometry().x()+(ui->line_med_3->geometry().width()/2),40,10,3));
    animation->setEndValue(ui->line_med_3->geometry());
    animation->start();
}

void Menu::bigLineAnimationMedicament()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->bigline_med_3, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(-500,ui->bigline_med_3->geometry().y(),500,3));
    animation->setEndValue(ui->bigline_med_3->geometry());
    QEasingCurve curve;
    curve.setType(QEasingCurve::OutBounce);
    animation->setEasingCurve(curve);
    animation->start();
}

void Menu::bigLineAnimationEquipement()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->bigline_eq, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(-500,ui->bigline_eq->geometry().y(),500,3));
    animation->setEndValue(ui->bigline_eq->geometry());
    QEasingCurve curve;
    curve.setType(QEasingCurve::OutBounce);
    animation->setEasingCurve(curve);
    animation->start();
}

void Menu::on_gestionInfrastructrueMG_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
}

void Menu::on_pushButton_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_list_med_clicked()
{
    if(rank==1)
    {
        ui->ajouter_med->setVisible(false);
        ui->effacer_medicament->setVisible(false);

    }
    ui->stackedWidget->setCurrentIndex(14);
    smallLineAnimationMedicament();
    bigLineAnimationMedicament();
    afficher_medicament();

}

void Menu::on_list_eq_clicked()
{
    if(rank==1)
    {
        ui->ajouter_equi->setVisible(false);
        ui->effacer_equipement->setVisible(false);

    }
    ui->stackedWidget->setCurrentIndex(15);
    smallLineAnimationEquipement();
    bigLineAnimationEquipement();
     afficher_equipement();
}

void Menu::on_Home_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

}

void Menu::on_Equipement_clicked()
{
    if(rank==1)
    {
        ui->ajouter_equi->setVisible(false);
        ui->effacer_equipement->setVisible(false);

    }
     ui->stackedWidget->setCurrentIndex(15);
     smallLineAnimationEquipement();
     bigLineAnimationEquipement();
      afficher_equipement();
}

void Menu::on_Home_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);

}

void Menu::on_Medicament_5_clicked()
{
    if(rank==1)
    {
        ui->ajouter_med->setVisible(false);
        ui->effacer_medicament->setVisible(false);

    }
     ui->stackedWidget->setCurrentIndex(14);
     smallLineAnimationMedicament();
     bigLineAnimationMedicament();
     afficher_medicament();
}



void Menu::on_ajouter_med_clicked()
{
    ui->stackedWidget->setCurrentIndex(16);

}



void Menu::on_ajouter_equi_clicked()
{
    ui->stackedWidget->setCurrentIndex(17);

}

void Menu::on_pushButton_annuler_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
    ui->nom_med->setText("");
    ui->prix->setText("");
    ui->quantite->setText("");
    ui->description->setText("");
    QString df_1="01-01-2015",dlc_1="01-01-2020";
    ui->dateEdit_DF->date().toString(df_1);
    ui->dateEdit_DLC->date().toString(dlc_1);

}

void Menu::on_pushButton_annuler_clicked()
{
    ui->stackedWidget->setCurrentIndex(15);
    ui->nom_equi->setText("");
    ui->checkBox_bon->setChecked(false);
    ui->checkBox_mauvais->setChecked(false);
    ui->checkBox_dispo->setChecked(false);
    ui->checkBox_nondispo->setChecked(false);
}

void Menu::on_annuler_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
}

void Menu::on_pushButton_anuuler_clicked()
{
    ui->stackedWidget->setCurrentIndex(15);
}

void Menu::on_retour_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);

}

/////affichage liste des médicaments
void Menu::afficher_medicament()
{
    if (rank==1)
   {
              ui->ModifierMedicament->setVisible(false);


    }
    medicament m;
    int rowCount = m.clear_liste_med();

    for (int i =rowCount ; i>=0 ; i--)
    {
        QListWidgetItem *name = new QListWidgetItem;
        name = ui->listWidget_MED->takeItem(i);
        ui->listWidget_MED->removeItemWidget(name);
    }

    QSqlQuery* qry=m.afficherMedicamentList();


        rowCount = 0;
        while(qry->next())
        {

            QListWidgetItem *Name = new QListWidgetItem;


            Name->setText(qry->value(0).toString());
            ui->listWidget_MED->insertItem(rowCount,Name);
            rowCount++;



        }

}

//////affichage liste des équipements
void Menu::afficher_equipement()
{

            if(rank==1)
            {
                ui->ModifierEquipement->setVisible(false);

            }
    equipement e;
    int rowCount = e.clear_liste_eq();



        for (int i =rowCount ; i>=0 ; i--)
        {
            QListWidgetItem *name = new QListWidgetItem;
            name = ui->listWidget_EQUI->takeItem(i);
            ui->listWidget_EQUI->removeItemWidget(name);
        }

        QSqlQuery* qry=e.afficherEquipementListe();



        rowCount = 0;
        while(qry->next())
        {

            QListWidgetItem *Name = new QListWidgetItem;


            Name->setText(qry->value(0).toString());
            ui->listWidget_EQUI->insertItem(rowCount,Name);
            rowCount++;



        }


}

///statistique
void Menu::statistique_med()
{
    QPieSeries *series = new QPieSeries();
    QSqlQuery qry("SELECT * FROM MEDICAMENTS");

        while(qry.next())
        {
            series->append(qry.value(0).toString(),qry.value(5).toInt());
        }



    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Quantitees Medicaments");
    QChartView *chartview = new QChartView(chart);
    chartview->setParent(ui->horizontalFrame);

}

void Menu::on_Statistique_med_clicked()
{
    statistique_med();
    ui->stackedWidget->setCurrentIndex(20);


}
//////ajouter médicament
void Menu::on_pushButton_ajouter_clicked()
{
    QString df,dlc;
    QSqlQuery query;



    df = ui->dateEdit_DF->date().toString("dd-MM-yyyy");
    dlc = ui->dateEdit_DLC->date().toString("dd-MM-yyyy");

    medicament m(ui->nom_med->text(),ui->description->text(),df,dlc,ui->prix->text(),ui->quantite->text());
    int prix= m.getPrix().toInt();
    int quantite= m.getQuantite().toInt();

    if ((m.getNom()=="")||(m.getDescription()=="")||(m.getPrix()=="")||(m.getQuantite()==""))
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"),
                              QObject::tr("Informations manquantes.\n"
                                          "Click Cancel to try again."), QMessageBox::Cancel);
    }
    else if(prix==0)
    {
        QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                 QObject::tr("il faut un prix .\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else if(quantite==0)
    {
        QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                 QObject::tr("il faut un entier .\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }

    else
    {



            if(m.chercher_nom_exist(m.getNom())==0)
            {
                bool test=m.ajouter_medicament();
                if(test)
                {
                    qDebug ()<< "ajout medicament";
                }

            }
            else
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("medicament deja existant.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }


    }
    ui->stackedWidget->setCurrentIndex(14);
    afficher_medicament();

    ui->nom_med->setText("");
    ui->prix->setText("");
    ui->quantite->setText("");
    ui->description->setText("");
    QString df_1="01-01-2015",dlc_1="01-01-2020";
    ui->dateEdit_DF->date().toString(df_1);
    ui->dateEdit_DLC->date().toString(dlc_1);
}


void Menu::on_effacer_medicament_clicked()
{
    QString nom = ui->listWidget_MED->currentItem()->text();
    medicament m;
    bool test = m.supprimer_medicaments(nom);
    if(test)
    {
        qDebug ()<< "sup medicament reussite" ;
        afficher_medicament();
    }

}

void Menu::on_effacer_equipement_clicked()
{
    QString nom = ui->listWidget_EQUI->currentItem()->text();
    equipement e;
    bool test = e.supprimer_equipement(nom);
    if(test)
    {
        qDebug ()<< "sup equipement reussite";
        afficher_equipement();
    }
}


///////chercher medicament
void Menu::on_chercher_med_textChanged(const QString &arg1)
{
    QString searchText = ui->chercher_med->text();
    int listWidgetSize = ui->listWidget_MED->count();

    for (int k1 = 0; k1 < listWidgetSize; ++k1)
    {
        if (ui->listWidget_MED->item(k1)->text().contains(arg1))
        {
            ui->listWidget_MED->item(k1)->setHidden(false);
        }
        else
        {
            ui->listWidget_MED->item(k1)->setHidden(true);
        }
    }

}

///////afficher un médicament
void Menu::on_listWidget_MED_itemDoubleClicked(QListWidgetItem *item)
{
            if(rank==1)
            {
                ui->ModifierMedicament->setEnabled(false);
                ui->lineEdit_nomMed->setEnabled(false);
                ui->lineEdit_description->setEnabled(false);
                ui->dateEdit_DF2->setEnabled(false);
                ui->dateEdit_4->setEnabled(false);
                ui->lineEdit_quantite->setEnabled(false);
                ui->lineEdit_prix->setEnabled(false);

            }
    medicament m;
        ui->stackedWidget->setCurrentIndex(18);
        QString nom = item->text();
        QSqlQuery qry=m.afficher_med_double_clicked(nom);

        QString df = qry.value(2).toString();
        QString dlc = qry.value(3).toString();
        QDate df_date = QDate::fromString(df,"dd-MM-yyyy");
        QDate dlc_date = QDate::fromString(dlc,"dd-MM-yyyy");


        ui->dateEdit_DF2->setDate(df_date);
        ui->dateEdit_4->setDate(dlc_date);
        QString nom_m,des,prix,quantite;
        nom_m= qry.value(0).toString();
        ui->lineEdit_nomMed->setText(nom_m);
        ui->lineEdit_description->setText(qry.value(1).toString());
        ui->lineEdit_prix->setText(qry.value(4).toString());
        ui->lineEdit_quantite->setText(qry.value(5).toString());

}


void Menu::on_ModifierMedicament_clicked()
{
    medicament m;
    QString nom_init = ui->listWidget_MED->currentItem()->text();
    int count=m.chercher_nomMedicament_a_modifier(nom_init);
        if (count == 1)
        {
            QString df= ui->dateEdit_DF2->date().toString("dd-MM-yyyy") ;
            QString dlc= ui->dateEdit_4->date().toString("dd-MM-yyyy") ;
            QString nom,desc,prix,quantite;
            nom= ui->lineEdit_nomMed->text();
            desc = ui->lineEdit_description->text();
            prix = ui->lineEdit_prix->text();
            quantite = ui->lineEdit_quantite->text();
            medicament m(nom,desc,df,dlc,prix,quantite);

            if ( nom == "" || desc == "" || prix == ""|| quantite == "")
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("il y a une information vide.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }
            else
            {
                bool test=m.modifier_medicament(nom_init);

                if ( !test)
                {
                    QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                             QObject::tr("erreur a la modification.\n"
                                                         "Click Cancel to exit."), QMessageBox::Cancel);
                }

                ui->stackedWidget->setCurrentIndex(14);


            }

        }
        else
        {
            QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                     QObject::tr("name not found.\n"
                                                 "Click Cancel to exit."), QMessageBox::Cancel);
        }
    afficher_medicament();
}

//////ajouter un equipement
void Menu::on_ajout_equi_2_clicked()
{
    QString etat="",dispo="",name="";

    name= ui->nom_equi->text();

    if ( (!ui->checkBox_dispo->isChecked()) && (!ui->checkBox_nondispo->isChecked()) )
    {
        QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                 QObject::tr("choisir option de disponibilites.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else if ( ui->checkBox_dispo->isChecked() && ui->checkBox_nondispo->isChecked() )
    {
        QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                 QObject::tr("dispo et non dispo ne peuvent pas etre cocher en meme temps.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else if ( ui->checkBox_dispo->isChecked() )
    {
        dispo = "1";
    }
    else  if ( ui->checkBox_nondispo->isChecked() )
    {
        dispo = "0";

    }
    if ( (!ui->checkBox_bon->isChecked()) && (!ui->checkBox_mauvais->isChecked()) )
    {
        QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                 QObject::tr("choisir un etat d'equipement.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else if ( ui->checkBox_bon->isChecked() && ui->checkBox_mauvais->isChecked() )
    {
        QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                 QObject::tr("bon et mauvais ne peuvent pas etre cocher en meme temps.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else if ( ui->checkBox_bon->isChecked() )
    {
        etat = "1";
    }
    else if ( ui->checkBox_mauvais->isChecked() )
    {
        etat = "0";
    }


    if ( name == "")
    {
        QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                 QObject::tr("nom manquant.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }

    if (( etat == "0" || etat == "1" )&& (dispo == "0" || dispo == "1") && ( name != ""))
    {
        qDebug() << etat << name << dispo;
        equipement e(name,etat,dispo);


            if(e.chercher_nom_exist(e.get_nom())==0)
            {
                bool test=e.ajouter_equipement();
                if(test)
                {
                    qDebug ()<< "ajout equipement";
                }
                ui->stackedWidget->setCurrentIndex(2);

            }
            else
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("equipement deja existant.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }
        }
        afficher_equipement();
    ui->stackedWidget->setCurrentIndex(15);
    ui->nom_equi->setText("");
    ui->checkBox_bon->setChecked(false);
    ui->checkBox_mauvais->setChecked(false);
    ui->checkBox_dispo->setChecked(false);
    ui->checkBox_nondispo->setChecked(false);
}

/////chercher equi
void Menu::on_chercher_equi_textChanged(const QString &arg1)
{
    QString searchText = ui->chercher_equi->text();
    int listWidgetSize = ui->listWidget_EQUI->count();

    for (int k1 = 0; k1 < listWidgetSize; ++k1)
    {
        if (ui->listWidget_EQUI->item(k1)->text().contains(arg1))
        {
            ui->listWidget_EQUI->item(k1)->setHidden(false);
        }
        else
        {
            ui->listWidget_EQUI->item(k1)->setHidden(true);
        }
    }
}

//////afficher un equipement

void Menu::on_listWidget_EQUI_itemDoubleClicked(QListWidgetItem *item)
{

            if(rank==1)
            {
                ui->ModifierEquipement->setVisible(false);
                if(rank==1)
                {
                    ui->lineEdit_nomEqui->setEnabled(false);
                    ui->dispo->setEnabled(false);
                    ui->non_dispo->setEnabled(false);
                    ui->bon->setEnabled(false);
                    ui->mauvais->setEnabled(false);

                }

            }

         equipement e;
        ui->stackedWidget->setCurrentIndex(19);
        QString nom = item->text();
        QSqlQuery qry=e.afficher_eq_double_clicked(nom);

                QString nom_e,etat,dispo;
                nom_e= qry.value(0).toString();
                ui->lineEdit_nomEqui->setText(nom_e);

                dispo = qry.value(1).toString();

                if ( dispo == "0")
                {
                    ui->non_dispo->setChecked(true);
                }
                else
                {
                    ui->dispo->setChecked(true);
                }

                etat = qry.value(2).toString();

                if ( etat == "0")
                {
                    ui->mauvais->setChecked(true);
                }
                else
                {
                    ui->bon->setChecked(true);
                }
}


void Menu::on_ModifierEquipement_clicked()
{
    equipement e;
    QString nom_init = ui->listWidget_EQUI->currentItem()->text();
    int count = e.chercher_nomEquipement_a_modifier(nom_init);
        if (count == 1)
        {
            QString nom="",dispo="",etat="";
            nom= ui->lineEdit_nomEqui->text();

            if ( (!ui->dispo->isChecked()) && (!ui->non_dispo->isChecked()) )
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("choisir option de disponibilites.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }
            else if ( ui->dispo->isChecked() && ui->non_dispo->isChecked() )
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("dispo et non dispo ne peuvent pas etre cocher en meme temps.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }
            else if ( ui->dispo->isChecked() )
            {
                dispo = "1";
            }
            else  if ( ui->non_dispo->isChecked() )
            {
                dispo = "0";
            }


            if ( (!ui->bon->isChecked()) && (!ui->mauvais->isChecked()) )
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("choisir un etat d'equipement.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }
            else if ( ui->bon->isChecked() && ui->mauvais->isChecked() )
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("bon et mauvais ne peuvent pas etre cocher en meme temps.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }
            else if ( ui->bon->isChecked() )
            {
                etat = "1";
            }
            else if ( ui->mauvais->isChecked() )
            {
                etat = "0";
            }

            if ( nom == "" || etat == "" || dispo == "")
            {
                QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                         QObject::tr("nom or disponibilite ou etat est vide.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);
            }
            else
            {
                equipement eq(nom,etat,dispo);
                bool test = eq.modifier_equipement(nom_init);

                if(!test)
                {
                    QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                             QObject::tr("erreur modifier l'equipement.\n"
                                                         "Click Cancel to exit."), QMessageBox::Cancel);
                }

                ui->stackedWidget->setCurrentIndex(15);
            }

        }
        else
        {
            QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                     QObject::tr("nom non trouve.\n"
                                                 "Click Cancel to exit."), QMessageBox::Cancel);
        }

   /* ui->lineEdit_nomEqui->setText("");
    ui->bon->setChecked(false);
    ui->mauvais->setChecked(false);
    ui->dispo->setChecked(false);
    ui->non_dispo->setChecked(false);*/
    afficher_equipement();
}


void Menu::on_trier_med_clicked()
{
    medicament m;
        int rowCount = m.clear_liste_med();

        for (int i =rowCount ; i>=0 ; i--)
        {
            QListWidgetItem *name = new QListWidgetItem;
            name = ui->listWidget_MED->takeItem(i);
            ui->listWidget_MED->removeItemWidget(name);
        }

        QSqlQuery* qry = m.trier_liste_med();
        rowCount = 0;
        while(qry->next())
        {

            QListWidgetItem *Name = new QListWidgetItem;


            Name->setText(qry->value(0).toString());
            ui->listWidget_MED->insertItem(rowCount,Name);
            rowCount++;



        }
}


void Menu::on_trier_eq_clicked()
{
    equipement e;
    int rowCount = e.clear_liste_eq();

        for (int i =rowCount ; i>=0 ; i--)
        {
            QListWidgetItem *name = new QListWidgetItem;
            name = ui->listWidget_EQUI->takeItem(i);
            ui->listWidget_EQUI->removeItemWidget(name);
        }

        QSqlQuery* qry=e.trier_liste_equi();
        rowCount = 0;
        while(qry->next())
        {

            QListWidgetItem *Name = new QListWidgetItem;


            Name->setText(qry->value(0).toString());
            ui->listWidget_EQUI->insertItem(rowCount,Name);
            rowCount++;



        }
}


void Menu::on_imprimer_clicked()
{
    QPrinter printer;
       printer.setPrinterName("my_printer_machine");
        QPrintDialog dialog(&printer,this);
        dialog.setWindowTitle("Print Document");

        if ( dialog.exec()== QDialog::Rejected)
        {
            QMessageBox msg;
            msg.setText("ERROR");
            msg.show();
        };
     QPainter painter(this);
     painter.begin(&printer);

    QString nom,dispo,etat;
     nom = ui->lineEdit_nomEqui->text();
     if ( (!ui->dispo->isChecked()) && (!ui->non_dispo->isChecked()) )
     {
         QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                  QObject::tr("choisir option de disponibilites.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
     }
     else if ( ui->dispo->isChecked() && ui->non_dispo->isChecked() )
     {
         QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                  QObject::tr("dispo et non dispo ne peuvent pas etre cocher en meme temps.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
     }
     else if ( ui->dispo->isChecked() )
     {
         dispo = "disponible";
     }
     else  if ( ui->non_dispo->isChecked() )
     {
         dispo = "non disponible";
     }


     if ( (!ui->bon->isChecked()) && (!ui->mauvais->isChecked()) )
     {
         QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                  QObject::tr("choisir un etat d'equipement.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
     }
     else if ( ui->bon->isChecked() && ui->mauvais->isChecked() )
     {
         QMessageBox::information(nullptr, QObject::tr("ERROR"),
                                  QObject::tr("bon et mauvais ne peuvent pas etre cocher en meme temps.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
     }
     else if ( ui->bon->isChecked() )
     {
         etat = "bon";
     }
     else if ( ui->mauvais->isChecked() )
     {
         etat = "mauvais";
     }
     QFont font = painter.font();
     font.setPointSize(font.pointSize() * 2);
     painter.setFont(font);
    QImage image(":/pics/pics/pdf_image.jpg");
    painter.setPen(Qt::cyan);
     painter.drawImage(-20,-40,image);
    painter.drawText(300,90,"Equipement : ");

    painter.setPen(Qt::darkGray);

    painter.drawText(230,160,"Nom : ");
    painter.drawText(230,185,"Etat de fonctionnement : ");
    painter.drawText(230,210,"Disponibilité : ");

    painter.setPen(Qt::white);

    painter.drawText(320,160,nom);
    painter.drawText(530,185,etat);
    painter.drawText(400,210,dispo);



        painter.end();

      //  QTextEdit y;

       // y.print(&printer);

}
void Menu::animation_chambre()
{
    QPixmap pic_service(":/pics/pics/chambre.png");
    QPropertyAnimation *animation1;
    int w2=ui->animation_chambre->width();
    int h2=ui->animation_chambre->height();
    ui->animation_chambre->setPixmap(pic_service.scaled(h2,w2,Qt::KeepAspectRatio));
    animation1 = new QPropertyAnimation(ui->animation_chambre, "geometry");
                    animation1->setDuration(3000);
                    animation1->setStartValue(ui->animation_chambre->geometry());
                    animation1->setEndValue(QRect(720,550,71,61 ));
                    animation1->start();
}
void Menu::animation_service()
{
    QPixmap pic_service(":/pics/pics/service.png");
    QPropertyAnimation *animation1;
    int w2=ui->animation_service->width();
    int h2=ui->animation_service->height();
    ui->animation_service->setPixmap(pic_service.scaled(h2,w2,Qt::KeepAspectRatio));
    animation1 = new QPropertyAnimation(ui->animation_service, "geometry");
                    animation1->setDuration(3000);
                    animation1->setStartValue(ui->animation_service->geometry());
                    animation1->setEndValue(QRect(720,550,71,61 ));
                    animation1->start();
}
void Menu::on_pushButton_menuChambres_clicked()
{
    if(rank==1)
    {
        ui->ajouter_chambre->setVisible(false);
        ui->supprimer_chambre->setVisible(false);
    }
    ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
    ui->stackedWidget->setCurrentIndex(26);
    animation_chambre();
}
void Menu::on_pushButton_menuServices_clicked()
{
    if(rank==1)
    {
        ui->ajouter_service->setVisible(false);
        ui->supprimer_service->setVisible(false);
    }
    ui->tableView_services->setModel(ser.afficher_ListeService());
    ui->stackedWidget->setCurrentIndex(22);
    animation_service();
}
void Menu::on_ajouter_service_clicked()
{
    ui->stackedWidget->setCurrentIndex(23);
    ui->comboBox_AjoutEquip->clear();
    ui->comboBox_AjoutStaff1->clear();
    ui->comboBox_AjoutStaff2->clear();
    ui->comboBox_AjoutStaff3->clear();
    ui->lineEdit_ajoutNomService->clear();
    QSqlQuery qry,qry2;
    qry.prepare("select USERNAME from emp");
    qry.exec();
    while(qry.next()){
    ui->comboBox_AjoutStaff1->addItem(qry.value(0).toString());
    ui->comboBox_AjoutStaff2->addItem(qry.value(0).toString());
    ui->comboBox_AjoutStaff3->addItem(qry.value(0).toString());
    }
    qry2.prepare("SELECT NOM FROM EQUIPEMENT");
    qry2.exec();
    while(qry2.next())
    {
        ui->comboBox_AjoutEquip->addItem(qry2.value(0).toString());
    }



}

void Menu::on_supprimer_service_clicked()
{
    QString id =ui->tableView_services->model()->index(ui->tableView_services->currentIndex().row(),0).data().toString();
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Supprimer", "Etes vous sur de supprimer ce service?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          bool test=ser.supprimer_service(id);
          if(test)
          {
    ui->tableView_services->setModel(ser.afficher_ListeService());
    QMessageBox::information(nullptr,"Suppression","Service supprimé");

          }
      }
}
void Menu::on_ajouter_chambre_clicked()
{
    ui->stackedWidget->setCurrentIndex(27);
    ui->comboBox_AjoutEmplacement->clear();
    ui->comboBox_AjoutSurveillant->clear();
    ui->lineEdit_AjoutNumero->clear();
    ui->spinBox_AjoutLits->clear();
    ui->spinBox_AjoutLits->setValue(0);
    QSqlQuery qry1,qry2;
    qry1.prepare("select nom from services");
    qry1.exec();
    while(qry1.next())
    {
        ui->comboBox_AjoutEmplacement->addItem(qry1.value(0).toString());
    }
    qry2.prepare("select USERNAME from emp");
    qry2.exec();
    while(qry2.next())
    {
        ui->comboBox_AjoutSurveillant->addItem(qry2.value(0).toString());
    }

}
void Menu::on_supprimer_chambre_clicked()
{
    QString id =ui->tableView_chambres->model()->index(ui->tableView_chambres->currentIndex().row(),2).data().toString();
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Supprimer", "Etes vous sur de supprimer cette chambre?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          bool test=cham.supprimer_chambre(id);
          if(test)
          {
    ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
    QMessageBox::information(nullptr,"Suppression","Chambre supprimé");}
      }
}
void Menu::on_pushButton_Services_clicked()
{
    if(rank==1)
    {
        ui->ajouter_service->setVisible(false);
        ui->supprimer_service->setVisible(false);
    }
    ui->tableView_services->setModel(ser.afficher_ListeService());
    ui->stackedWidget->setCurrentIndex(22);

    animation_service();
}
void Menu::on_pushButton_Chambres_clicked()
{
    if(rank==1)
    {
        ui->ajouter_chambre->setVisible(false);
        ui->supprimer_chambre->setVisible(false);
    }
    ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
    ui->stackedWidget->setCurrentIndex(26);

    //animation
   animation_chambre();

}
void Menu::on_pushButton_petitMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void Menu::on_pushButton_services_clicked()
{
    if(rank==1)
    {
        ui->ajouter_service->setVisible(false);
        ui->supprimer_service->setVisible(false);
    }
    ui->tableView_services->setModel(ser.afficher_ListeService());
    ui->stackedWidget->setCurrentIndex(22);

    //animation
   animation_service();

}
void Menu::on_pushButton_Chambre_clicked()
{
    if(rank==1)
    {
        ui->ajouter_chambre->setVisible(false);
        ui->supprimer_chambre->setVisible(false);
    }
    ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
    ui->stackedWidget->setCurrentIndex(26);

    //animation
   animation_chambre();

}
void Menu::on_pushButton_petitmenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}
void Menu::on_pushButton_AnnulerAjoutService_clicked()
{
    ui->stackedWidget->setCurrentIndex(22);
}

void Menu::on_pushButton_AnnulerModifService_clicked()
{
    ui->stackedWidget->setCurrentIndex(25);
}
void Menu::on_pushButton_AnnulerAjoutChambre_clicked()
{
    ui->stackedWidget->setCurrentIndex(26);
}

// controle de saisie nom du service
void Menu::on_lineEdit_ajoutNomService_textEdited(const QString &arg1)
{
    QString texte=arg1;
           QString caracteresEtrangers(CARACTERES_ETRANGERS);
           for (int i=0; i<texte.size(); i++)
               foreach (const QChar &y, caracteresEtrangers)
                   if(texte.at(i)==y)
                       texte[i]=' ';

           ui->lineEdit_ajoutNomService->setText(texte);
}

void Menu::on_pushButton_AjouterService_clicked()
{

   QString nom=ui->lineEdit_ajoutNomService->text();
   QString staff1=ui->comboBox_AjoutStaff1->currentText();
   QString staff2=ui->comboBox_AjoutStaff2->currentText();
   QString staff3=ui->comboBox_AjoutStaff3->currentText();
   QString nom_equipement=ui->comboBox_AjoutEquip->currentText();
   service s(nom,staff1,staff2,staff3,nom_equipement);
   bool controle=s.controle_saisie_serviceAjout(s);
   if(s.get_nom_service()=="")
   {
       QMessageBox::critical(nullptr,QObject::tr("attention"),
                                        QObject::tr("Inserer un nom du service.\n Taper CANCEL pour les remplir"),
                                        QMessageBox::Cancel);
   }
   else if(controle)
   {
       QMessageBox::critical(nullptr,QObject::tr("attention"),
                                        QObject::tr("Les noms des staffs ne doivent pas etre repetés.\n Taper CANCEL pour les remplir"),
                                        QMessageBox::Cancel);
   }
   else
   {
       bool test=s.ajouter_service();

    if(test)
    {
        ui->tableView_services->setModel(ser.afficher_ListeService());
        QMessageBox::information(nullptr,QObject::tr("Success"),
                                         QObject::tr("Ajout effectué.\n Taper OK pour continuer"),
                                         QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(nullptr,QObject::tr("Echec"),
                                         QObject::tr("Ajout non effectué.\n Taper CANCEL pour sortir"),
                                         QMessageBox::Cancel);

    }

    ui->stackedWidget->setCurrentIndex(22);

   }

}






void Menu::on_pushButton_AjoutChambre_clicked()
{

   QString emplacement=ui->comboBox_AjoutEmplacement->currentText();
   QString type=ui->comboBox_AjoutType->currentText();
   QString numero=ui->lineEdit_AjoutNumero->text();
   QString nom_surveillant=ui->comboBox_AjoutSurveillant->currentText();
   QString nombre_lits=ui->spinBox_AjoutLits->text();
   chambre ch(emplacement,type,numero,nombre_lits,nom_surveillant);

   bool controle=cham.controle_saisieChambre(ch);
   if(controle)
   {
       QMessageBox::critical(nullptr,QObject::tr("attention"),
                                        QObject::tr("Champs non remplis.\n Taper CANCEL pour les remplir"),
                                        QMessageBox::Cancel);
   }
   if(ch.get_nombre_lits()=="0")
   {
       QMessageBox::critical(nullptr,QObject::tr("attention"),
                                        QObject::tr("Nombre de lits ne doit pas etre égal à 0.\n Taper CANCEL pour changer "),
                                        QMessageBox::Cancel);
   }
   else
   {
   bool test=ch.ajouter_chambre();
   if(test)
   {
       ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
       QMessageBox::information(nullptr,QObject::tr("Success"),
                                        QObject::tr("Ajout chambre effectué.\n Taper OK pour continuer"),
                                        QMessageBox::Ok);

   }
   else
   {
       QMessageBox::critical(nullptr,QObject::tr("Echec"),
                                        QObject::tr("Ajout chambre non effectué.\n Taper CANCEL pour sortir"),
                                        QMessageBox::Cancel);
   }
   ui->stackedWidget->setCurrentIndex(26);
   }

}






void Menu::on_tableView_services_activated(const QModelIndex &index)
{
    QString val=ui->tableView_services->model()->data(index).toString();
    aux_ser=val;
    QSqlQuery* query=ser.afficher_service(val);
    if(rank==1)
    {
        ui->pushButton_modifierServiceAff->setVisible(false);
    }
    if(query->exec())
    {
        while(query->next())
        {
            ui->lineEdit_AffNomService->setText(query->value(0).toString());
            ui->lineEdit_AffStaff1->setText(query->value(1).toString());
            ui->lineEdit_AffStaff2->setText(query->value(2).toString());
            ui->lineEdit_AffStaff3->setText(query->value(3).toString());
            ui->lineEdit_AffEquipement->setText(query->value(4).toString());
        }

        ui->stackedWidget->setCurrentIndex(25);
    }
    else
        QMessageBox::critical(nullptr,QObject::tr("echec"),
                              QObject::tr("Affichage du service non effectué.\n Taper CANCEL pour quitter"),
                              QMessageBox::Cancel  );
    delete query;
}



void Menu::on_tableView_chambres_activated(const QModelIndex &index)
{
    QString val=ui->tableView_chambres->model()->data(index).toString();
    aux_cham=val;
    QSqlQuery* query=cham.afficher_chambre(val);
    if(query->exec())
    {
        while(query->next())
        {
           ui->lineEdit_AffEmplacement->setText(query->value(0).toString());
           ui->lineEdit_AffType->setText(query->value(1).toString());
           ui->lineEdit_AffNumero->setText(query->value(2).toString());
           ui->lineEdit_AffNombreLits->setText(query->value(3).toString());
           ui->lineEdit_AffSurveillant->setText(query->value(4).toString());

        }
        ui->stackedWidget->setCurrentIndex(28);
        if(rank==1)
        {
            ui->pushButton_ModifChambre->setVisible(false);
        }
    }
    else

            QMessageBox::critical(nullptr,QObject::tr("echec"),
                                  QObject::tr("Affichage de la chambre non effectué.\n Taper CANCEL pour quitter"),
                                  QMessageBox::Cancel  );
    delete query;

}

void Menu::on_pushButton_RetourAffChambre_clicked()
{
    ui->stackedWidget->setCurrentIndex(26);


}

void Menu::on_pushButton_RetourAffService_clicked()
{
    ui->stackedWidget->setCurrentIndex(22);

}

void Menu::on_pushButton_modifierServiceAff_clicked()
{
    QSqlQuery query,query1,query2;
    query.prepare("SELECT * FROM SERVICES WHERE NOM='"+aux_ser+"'");
    query1.prepare("select USERNAME from emp");
    query1.exec();
    query2.prepare("SELECT NOM FROM EQUIPEMENT");
    query2.exec();
    if (query.exec())
    {
        while(query.next())
        {
            ui->lineEdit_ModifNomService->setText(query.value(0).toString());
        }
        while(query1.next())
        {
    ui->comboBox_ModifStaf1->addItem(query1.value(0).toString());
    ui->comboBox_ModifStaf2->addItem(query1.value(0).toString());
    ui->comboBox_ModifStaf3->addItem(query1.value(0).toString());
        }
        while(query2.next())
        {
     ui->comboBox_ModifEquip->addItem(query2.value(0).toString());
        }

    }
    ui->stackedWidget->setCurrentIndex(24);

}

void Menu::on_pushButton_triServices_clicked()
{
    ui->tableView_services->setModel(ser.trier_service());
}


void Menu::on_pushButton_modifService_clicked()
{


    QString nom=ui->lineEdit_ModifNomService->text();
    QString staff1=ui->comboBox_ModifStaf1->currentText();
    QString staff2=ui->comboBox_ModifStaf2->currentText();
    QString staff3=ui->comboBox_ModifStaf3->currentText();
    QString nom_equipement=ui->comboBox_ModifEquip->currentText();

    service s(nom,staff1,staff2,staff3,nom_equipement);
    bool test=s.modifier_service(aux_ser);
    if(test)
    {
        ui->tableView_services->setModel(ser.afficher_ListeService());
        QMessageBox::information(nullptr,QObject::tr("Success"),
                                         QObject::tr("Mise à jour effectuée.\n Taper OK pour continuer"),
                                         QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(nullptr,QObject::tr("echec"),
                                         QObject::tr("Mise à jour non effectuée.\n Taper CANCEL pour quitter"),
                                         QMessageBox::Cancel);
    }
    ui->stackedWidget->setCurrentIndex(22);
}

void Menu::on_pushButton_ModifChambre_clicked()
{
    QSqlQuery query,query1,query2;
    query.prepare("SELECT * FROM CHAMBRE WHERE EMPLACEMENT='"+aux_cham+"'");
    query1.prepare("select nom from services");
    query1.exec();
    query2.prepare("select USERNAME from emp ");
    query2.exec();
    if (query.exec())
    {
        while(query.next())
        {
            ui->lineEdit_ModifNumero->setText(query.value(2).toString());
            ui->lineEdit_ModifNombreLits->setText(query.value(3).toString());
        }
        while(query1.next())
        {
            ui->comboBox_ModifEmplacement->addItem(query1.value(0).toString());
        }
        while(query2.next())
        {
            ui->comboBox_ModifSurveillant->addItem(query2.value(0).toString());
        }
    }
    ui->stackedWidget->setCurrentIndex(29);

}



void Menu::on_pushButton_annulerModifChambre_clicked()
{
    ui->stackedWidget->setCurrentIndex(28);

}


void Menu::on_pushButton_ModifierChambre_clicked()
{
    QString emplacement=ui->comboBox_ModifEmplacement->currentText();
    QString type=ui->comboBox_ModifType->currentText();
    QString new_numero=ui->lineEdit_ModifNumero->text();
    QString nombre_lits=ui->lineEdit_ModifNombreLits->text();
    QString nom_surveillant=ui->comboBox_ModifSurveillant->currentText();
    chambre cham;
    qDebug()<< nombre_lits;
    bool test=cham.modifier_chambre(aux_cham,emplacement,type,new_numero,nombre_lits,nom_surveillant);
    if(test)
    {

        QMessageBox::information(nullptr,QObject::tr("success"),
                                 QObject::tr("Mise à jour effectuée\n.Taper OK pour continuer"),
                                 QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(nullptr,QObject::tr("echec"),
                                 QObject::tr("Mise à jour non effectuée\n.Taper CANCEL pour sortir"),
                                 QMessageBox::Cancel);
    }
    ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
    ui->stackedWidget->setCurrentIndex(26);
}

void Menu::on_pushButton_imprimerService_clicked()
{
    QString nom=ui->lineEdit_AffNomService->text();
    QString staff1=ui->lineEdit_AffStaff1->text();
    QString staff2=ui->lineEdit_AffStaff2->text();
    QString staff3=ui->lineEdit_AffStaff3->text();
    QString equipement=ui->lineEdit_AffEquipement->text();
    service s(nom,staff1,staff2,staff3,equipement);


    s.printPDF_service();
    ui->stackedWidget->setCurrentIndex(22);
}



void Menu::on_pushButton_rechercherChambre_clicked()
{
    QString rech=ui->lineEdit_rechercherChambre->text();
    if(rech=="")
        ui->tableView_chambres->setModel(cham.afficher_ListeChambre());
    else
    ui->tableView_chambres->setModel(cham.rechercher_chambre(rech));

}
void Menu::on_pushButton_triChambre_clicked()
{
    ui->tableView_chambres->setModel(cham.trier_chambre());
}

void Menu::on_pushButton_rechercherService_clicked()
{
    QString rech=ui->lineEdit_rechercheServices->text();
    if(rech=="")
        ui->tableView_services->setModel(ser.afficher_ListeService());
    else
    ui->tableView_services->setModel(ser.rechercher_service(rech));

}

void Menu::stat_sahar()
{
    QPieSeries *series = new QPieSeries();
       QSqlQuery qry("SELECT * FROM CHAMBRE");
           QMap<QString,int >qq;
           QVector <QString> names;
           while(qry.next())
           {
               if(qq[qry.value(0).toString()]==0)
                   names.push_back(qry.value(0).toString());
               qq[qry.value(0).toString()]+=qry.value(3).toInt();

           }
           for(int i=0;i< names.size();i++)
               series->append(names[i],qq[names[i]]);

       QChart *chart = new QChart();
       chart->addSeries(series);
       chart->setTitle("Nombre de lits dans les services");

       QChartView *chartview = new QChartView(chart);
       chartview->setParent(ui->stat_chambre);

}
void Menu::on_pushButton_StatService_clicked()
{
    stat_sahar();
    ui->stackedWidget->setCurrentIndex(30);

}

void Menu::on_pushButton_backstat_clicked()
{
    ui->stackedWidget->setCurrentIndex(26);
}


void Menu::on_gestionOffresMG_2_clicked()
{

    ui->stackedWidget->setCurrentIndex(21);

}

void Menu::on_pushButton_retourGestionoffres_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

//////////////////////integration walid



void Menu::on_pushButtonaj_clicked()
{
    ui->stackedWidget->setCurrentIndex(34);
}

void Menu::on_pushButton_12aj_clicked()
{
    ui->stackedWidget->setCurrentIndex(35);
}

void Menu::on_pushButton_14AN_clicked()
{
    ui->stackedWidget->setCurrentIndex(32);
}

void Menu::on_pushButton_16AN_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void Menu::on_pushButton_13OK_clicked()
{


    int immatricule= ui->lineEdit_3imm->text().toInt();
    QString etat= ui->lineEdit_6et->text();
    QString ambulancier= ui->lineEdit_4am->text();
    QString materiel_medical= ui->lineEdit_5mm->text();



        ambulance a (immatricule,etat,ambulancier,materiel_medical);

        if(immatricule<10000){ QMessageBox::information(nullptr,QObject::tr("OK"),QObject::tr(" Erreur immatricule necessite 5 chiffres.\n"
                                                                                        "click cancel to exit."),QMessageBox::Cancel);}
               else{  bool test=a.ajouter();

        if (test)

    {
        ui->tabambulance->setModel(tmpambulance.Afficher_ambulance());

        QMessageBox::information(nullptr,QObject::tr("ajouter une ambulance"),QObject::tr("ambulance ajouté.\n"
                                                                                       "click cancel to exit."),QMessageBox::Cancel);

   }
}}



void Menu::on_pushButton_15oK_clicked()
{
    int id=ui->lineEdit_9ID->text().toInt();
    QString destination=ui->lineEdit_7DES->text();
    QString heure=ui->lineEdit_8HE->text();

    mission m(id,destination,heure);
    if(id<100000){ QMessageBox::information(nullptr,QObject::tr("OK"),QObject::tr(" Erreur ID necessite 6 chiffres.\n"
                                                                                    "click cancel to exit."),QMessageBox::Cancel);}
   else{ bool test=m.ajouter();

    if (test)
    {
        ui->tabmission->setModel(tmpmission.afficher_mission());

        QMessageBox::information(nullptr,QObject::tr("ajouter une mission"),QObject::tr("mission ajouté.\n"
                                                                                       "click cancel to exit."),QMessageBox::Cancel);

    }
}}

void Menu::on_pushButton_18supp_clicked()
{
    int immatricule=ui->lineEdit_10_im->text().toInt();
    bool test=tmpambulance.Supprimer_ambulance(immatricule);
    if(test)
    {
        ui->tabambulance->setModel(tmpambulance.Afficher_ambulance());
        QMessageBox::information(nullptr,QObject::tr("supprimer une ambulance"),QObject::tr("ambulance supprimé.\n"
                                                                                       "click cancel to exit."),QMessageBox::Cancel);
    }
}

void Menu::on_pushButton_20supp_clicked()
{
    int id=ui->lineEdit_10_id->text().toInt();
    bool test=tmpmission.supprimer_mission(id);
    if(test)
    {
        ui->tabmission->setModel(tmpmission.afficher_mission());
        QMessageBox::information(nullptr,QObject::tr("supprimer une mission"),QObject::tr("mission supprimé.\n"
                                                                                       "click cancel to exit."),QMessageBox::Cancel);
    }
}

void Menu::on_pushButton_9ok_clicked()
{
    QString text;

     text=ui->lineEditSRCH->text();

     if(text == "")

     {
         ui->tabambulance->setModel(tmpambulance.Afficher_ambulance());

     }

     else

     {
         ui->tabambulance->setModel(tmpambulance.CHERCHERa(text));

     }
}

void Menu::on_pushButton_11ok_clicked()
{
    QString text;

     text=ui->lineEdit_2SRCH->text();

     if(text == "")

     {

         ui->tabmission->setModel(tmpmission.afficher_mission());

     }

     else

     {

         ui->tabmission->setModel(tmpmission.CHERCHERm(text));

     }
}

void Menu::on_pushButton_21tr_clicked()
{
     ui->tabambulance->setModel(tmpambulance.TRIER_ambulance());
}

void Menu::on_pushButton_22tr_clicked()
{
    ui->tabmission->setModel(tmpmission.TRIERm());
}




void Menu::on_pushButton_print_clicked()
{

    QPrinter printer;
        printer.setPrinterName("diserter printer name");
        QPrintDialog dialog(&printer,this);
        if(dialog.exec()==QDialog::Rejected)return;
        ui->tabambulance->render(&printer);

}



void Menu::on_pushButton_17mod_clicked()
{
    QSqlQuery query;
            int immatricule= ui->lineEdit_10_im->text().toInt();
            QString etat= ui->lineEdit_6et->text();
            QString ambulancier= ui->lineEdit_4am->text();
            QString materiel_medical= ui->lineEdit_5mm->text();



                ambulance a(immatricule,etat,ambulancier,materiel_medical);


        bool test=a.Modifier_ambulance(immatricule,etat,ambulancier,materiel_medical);

            if(test)
            {ui->tabambulance->setModel(a.Afficher_ambulance());
                QMessageBox::information(nullptr, QObject::tr("modifier ambulance"),
                                         QObject::tr("ambulance modifié.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);

            }
            else
                       {
                           QMessageBox ::critical(nullptr,QObject::tr("modifier ambulance"),
                                                             QObject::tr("Erreur.\n"
                                                                 "click cancel to exit"),QMessageBox::Cancel);

                       }
}



void Menu::on_pushButton_19mod_clicked()
{
    QSqlQuery query;
            int id= ui->lineEdit_10_id->text().toInt();
            QString destination= ui->lineEdit_7DES->text();
            QString heure= ui->lineEdit_8HE->text();



                mission m(id,destination,heure);


        bool test=m.Modifier_mission(id,destination,heure);

            if(test)
            {ui->tabmission->setModel(m.afficher_mission());
                QMessageBox::information(nullptr, QObject::tr("modifier mission"),
                                         QObject::tr("mission modifié.\n"
                                                     "Click Cancel to exit."), QMessageBox::Cancel);

            }
            else
                       {
                           QMessageBox ::critical(nullptr,QObject::tr("modifier mission"),
                                                             QObject::tr("Erreur.\n"
                                                                 "click cancel to exit"),QMessageBox::Cancel);

                       }
}


void Menu::on_gestionSecoursMG_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(31);
}

void Menu::on_pushButtonGDA_clicked()
{
     ui->stackedWidget->setCurrentIndex(32);
}

void Menu::on_pushButtonGDM_clicked()
{
     ui->stackedWidget->setCurrentIndex(33);
}

void Menu::on_pushButton_9_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_pushButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(33);
}

void Menu::on_pushButton_18_clicked()
{
     ui->stackedWidget->setCurrentIndex(32);
}

void Menu::on_trier_med_Prix_clicked()
{
    medicament m;


        int rowCount = m.clear_liste_med();

        for (int i =rowCount ; i>=0 ; i--)
        {
            QListWidgetItem *name = new QListWidgetItem;
            name = ui->listWidget_MED->takeItem(i);
            ui->listWidget_MED->removeItemWidget(name);
        }

        QSqlQuery* qry = m.trierPrix_liste_med();
        rowCount = 0;
        while(qry->next())
        {

            QListWidgetItem *Name = new QListWidgetItem;


            Name->setText(qry->value(0).toString());
            ui->listWidget_MED->insertItem(rowCount,Name);
            rowCount++;



        }
}

void Menu::on_trier_med_Quantite_clicked()
{
    medicament m;


        int rowCount = m.clear_liste_med();

        for (int i =rowCount ; i>=0 ; i--)
        {
            QListWidgetItem *name = new QListWidgetItem;
            name = ui->listWidget_MED->takeItem(i);
            ui->listWidget_MED->removeItemWidget(name);
        }

        QSqlQuery* qry = m.trierQuantite_liste_med();
        rowCount = 0;
        while(qry->next())
        {

            QListWidgetItem *Name = new QListWidgetItem;


            Name->setText(qry->value(0).toString());
            ui->listWidget_MED->insertItem(rowCount,Name);
            rowCount++;



        }
}

void Menu::on_pushButton_menu_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::downloadFinished(QNetworkReply* reply){
    QPixmap pix;
    pix.loadFromData(reply->readAll());
    ui->QR_code->setPixmap(pix.scaled(300,300,Qt::KeepAspectRatio));
    QR=pix;
    ui->stackedWidget->setCurrentIndex(36);
}

void Menu::on_QrButton_clicked()
{
    QString myURL="http://api.qrserver.com/v1/create-qr-code/?data=id: "+R.getId()+"%0ADate et heure: "+R.getDateTime().toString("dd/MM/yyyy hh:mm")+"%0ANom et prenom: "+R.getNom()+" "+R.getPrenom();
    ui->id_QR->setText(R.getId());
    ui->dateTime_QR->setText(R.getDateTime().toString("dd/MM/yyyy hh:mm"));
    ui->nomPrenom_QR->setText(R.getNom()+" "+R.getPrenom());
    QNetworkAccessManager *man= new QNetworkAccessManager(this);
    connect(man,&QNetworkAccessManager::finished,this,&Menu::downloadFinished);
    const QUrl url=QUrl(myURL);
    QNetworkRequest request(url);
    man->get(request);
}

void Menu::on_pushButton_menu_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void Menu::on_pushButton_menu_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Menu::on_downlaod_buttonqrcode_clicked()
{
    QImage originalPixmap=QR.toImage();
    if(originalPixmap.save("C:\\temp\\QRcode.png"))
        QMessageBox::information(this, "Téléchargement", "Téléchargement terminé!",
                                            QMessageBox::Ok);
    else
        QMessageBox::warning(this, "Téléchargement", "Erreur lors du téléchargement!",
                                            QMessageBox::Ok);
}

void Menu::on_pushButton_7_clicked()
{
    QString link="https://jassersmida01.wixsite.com/website?fbclid=IwAR2TKusxepF0BGbRS_7nxbSVycy134rpw42D-w3fLrxJJP7PDWSwoGBQVdY";
    QDesktopServices::openUrl(QUrl(link));
}

void Menu::on_pushButton_8_clicked()
{
    QString link="https://www.wix.com/dashboard/a95991b6-881a-4e85-9567-77a6cb92ac02/inbox/626b0626-611d-3ca8-a396-b796127e8a6e?referralInfo=BackToSiteLink";
    QDesktopServices::openUrl(QUrl(link));
}
void Menu::sendMail()
{
    Smtp* smtp = new Smtp("mohamedjasser.bensmida@esprit.tn", "191JMT1723", "smtp.gmail.com", 465);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));


    smtp->sendMail("mohamedjasser.bensmida@esprit.tn", ui->lineEdit_email->text() , "hoptimal","you are welcome");
}

void Menu::mailSent(QString status)
{
    if(status == "Message sent")
        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );
}
void Menu::on_pushButton_2_clicked()
{
    sendMail();
}



void Menu::on_pushButton_12ard_clicked()
{
    Z.write_to_arduinow("1");
}

void Menu::on_pushButton_13ard_clicked()
{
    Z.write_to_arduinow("0");
}

void Menu::on_pushButton_12menuu_clicked()
{
    ui->stackedWidget->setCurrentIndex(31);
}

void Menu::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(37);
}

void Menu::update_labelw()
{
   datta=Z.read_from_arduinow();
   if(datta=="1")
       ui->label_655->setText("ON");
   else if(datta=="0")
       ui->label_655->setText("OFF");
}

void Menu::on_volume_button_clicked()
{
    volume=!volume;
    if(volume){
        QIcon volumeON;
        QPixmap advanced;

        if(advanced.load(":/pics/pics/volumeON.png"))
        {
            volumeON.addPixmap(advanced);
            ui->volume_button->setIcon(advanced);
        }
        //ghne
        player->setMedia(QUrl("qrc:/pics/pics/offres.mp3"));
        player->setVolume(150);
        player->play();

    }
    else if(!volume){
        QIcon volumeON;
        QPixmap advanced;

        if(advanced.load(":/pics/pics/volumeOFF.png"))
        {
            volumeON.addPixmap(advanced);
            ui->volume_button->setIcon(advanced);
        }
        //stop ghne
        player->stop();

    }
}
