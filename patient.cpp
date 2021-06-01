#include "patient.h"

bool Patient::modifyValues(QString nom, QString prenom, QDate dateNaissance,QString newCin,QString numChambre,QString oldCin){
   QSqlQuery qry;
   qry.prepare("UPDATE patient "
               "SET Cin = :newCin, Nom= :nom, Prenom=:prenom, "
               "dateNaissance = :dateNaissance, " //TO_DATE('"+dateNaissance.toString("yyyy.MM.dd")+" 00:00:00', 'YYYY.MM.DD HH24:MI:SS')
               "numChambre= :numChambre "
               "WHERE Cin = :oldCin ;");
    qry.addBindValue(newCin);
    qry.addBindValue(nom);
    qry.addBindValue(prenom);
    qry.addBindValue(dateNaissance);
    qry.addBindValue(numChambre);
    qry.addBindValue(oldCin);

    if(!qry.exec()){
        QMessageBox::critical(nullptr, QObject::tr("Cette chambre n'existe pas."),
                    QObject::tr("Veuillez changer le numero de la chambre."), QMessageBox::Ok);
        return false;
    }
    else{
        qDebug("Values added to DB");
        return true;
    }
}

bool Patient::addValuesToDB(QString nom, QString prenom, QDate dateNaissance,QString Cin,QString numChambre){
    QSqlQuery qry;
    qry.prepare("INSERT INTO patient("
                "Cin,"
                "Nom,"
                "Prenom,"
                "dateNaissance,"
                "numChambre)"
                "VALUES (?,?,?,?,?);");
    qry.addBindValue(Cin);
    qry.addBindValue(nom);
    qry.addBindValue(prenom);
    qry.addBindValue(dateNaissance);
    qry.addBindValue(numChambre);


    if(!qry.exec()){
        QMessageBox::critical(nullptr, QObject::tr("Cette chambre n'existe pas."),
                    QObject::tr("Veuillez changer le numero de la chambre."), QMessageBox::Ok);
        return false;
    }
    else{
        qDebug("Values added to DB");
        return true;
    }
}

void Patient::setNom(QString nom){
    this->nom=nom;
}

void Patient::setPrenom(QString prenom){
    this->prenom=prenom;
}

void Patient::Delete(){
    QSqlQuery qry;
    qry.prepare("DELETE FROM patient WHERE CIN='"+this->CIN+"';");
    qry.exec();
}

void Patient::printPDF(){
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("C:/temp/fichePatient.pdf");
    QPainter painter;
    QImage image(":/pics/pics/pdf_image.jpg");
    if (! painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("On n'a pas réussi à créer le pdf.");
        msgBox.exec();
    }
    else{
        QFont font = painter.font();
        font.setPointSize(font.pointSize() * 2);
        painter.setFont(font);
        painter.setPen(Qt::cyan);
        painter.drawImage(-20,-40,image);
        painter.drawText(330, 20, "Nom: ");
        painter.drawText(330, 65, "Prenom: ");
        painter.drawText(330, 110, "CIN: ");
        painter.drawText(330, 155, "Numero de la chambre: ");
        painter.drawText(330, 200, "Date de naissance: ");
        painter.setPen(Qt::white);
        painter.drawText(410, 20, this->nom);
        painter.drawText(450, 65, this->prenom);
        painter.drawText(400, 110, this->CIN);
        painter.drawText(630, 155, this->numChambre);
        painter.drawText(580, 200, this->dateNaissance.toString("dd/MM/yyyy"));
        painter.end();
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Le pdf a été créé.");
        msgBox.exec();
    }
}

void Patient::afficher(QString tri,QString data,int occurence){
    QString nom,prenom,numChambre,cin;
    QDate dateNaissance;
    QSqlQuery query("SELECT * FROM patient WHERE "+tri+"='"+data+"' ORDER BY "+tri);


    for(int i=0;i<=occurence;i++)
        query.next();

    cin =query.value(0).toString();
    nom = query.value(1).toString();
    prenom = query.value(2).toString();
    dateNaissance = query.value(3).toDate();
    numChambre=query.value(4).toString();

    this->CIN=cin;
    this->nom=nom;
    this->prenom=prenom;
    this->dateNaissance=dateNaissance;
    this->numChambre=numChambre;

}

void Patient::afficher(QString tri,int occurence,QString cin1,QString nom1,QString prenom1,QString numChambre1){

    QString nom,prenom,numChambre,cin;
    QDate dateNaissance;
    QSqlQuery query("SELECT * FROM PATIENT "
                    "WHERE CIN LIKE '%"+cin1+"%'"
                    " AND NOM LIKE '%"+nom1+"%' AND PRENOM LIKE '%"+prenom1+"%'"
                    " AND NUMCHAMBRE LIKE '%"+numChambre1+"%' ORDER BY "+tri);


    for(int i=0;i<=occurence;i++)
        query.next();

    cin =query.value(0).toString();
    nom = query.value(1).toString();
    prenom = query.value(2).toString();
    dateNaissance = query.value(3).toDate();
    numChambre=query.value(4).toString();

    this->CIN=cin;
    this->nom=nom;
    this->prenom=prenom;
    this->dateNaissance=dateNaissance;
    this->numChambre=numChambre;

}



QSqlQueryModel* Patient::search(QSqlQuery qry,QString column,QString cin,QString nom,QString prenom,QString numChambre){
    QSqlQueryModel *modal=new QSqlQueryModel();
    qry.prepare("SELECT "+column+" FROM PATIENT "
                "WHERE CIN LIKE '%"+cin+"%'"
                " AND NOM LIKE '%"+nom+"%' AND PRENOM LIKE '%"+prenom+"%'"
                " AND NUMCHAMBRE LIKE '%"+numChambre+"%' ORDER BY "+column);
    qry.exec();
    modal->setQuery(qry);
    return modal;
}

QSqlQueryModel* Patient::search(QSqlQuery qry,QString column){
    QSqlQueryModel *modal=new QSqlQueryModel();
    qry.prepare("SELECT "+column+" FROM patient ORDER BY "+column);
    qry.exec();
    modal->setQuery(qry);
    return modal;
}


QSqlQueryModel* Patient::search(QSqlQuery qry, QString column, QString text){
    QSqlQueryModel *modal=new QSqlQueryModel();
    qry.prepare("SELECT "+column+" FROM patient "
                 "WHERE "+column+" LIKE '%"+text+"%' ORDER BY "+column);
    qry.exec();
    modal->setQuery(qry);
    return modal;
}

void Patient::updateMinuscule(){
    QSqlQuery qery;
    qery.exec("UPDATE PATIENT SET Nom=LOWER(Nom),Prenom=LOWER(Prenom);");
}



bool Patient::chercherCin(QString Cin,QString operation){
    QSqlQuery qry("SELECT CIN FROM PATIENT;");
    int occurence=0;
    while(qry.next()){
        if(qry.value(0).toString()==Cin)
            occurence++;
    }
    if(operation=="Modifier" && occurence==1 && Cin==this->CIN)
        return false;
    else if(operation=="Ajouter" && occurence==0)
        return false;
    if(operation=="Modifier" && occurence==0 && Cin!=this->CIN)
        return false;
    else return true;
}
