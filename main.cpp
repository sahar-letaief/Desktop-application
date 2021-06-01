#include "menu.h"
#include "connectdb.h"
#include <QApplication>
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QInputDialog>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectDB db;
    db.Connect();
    //traduction en langue anglaise
    QStringList langues;
    QTranslator t;
    langues<<"Anglais"<<"Français";
    QString choix=QInputDialog::getItem(NULL,"CHOISIR","Langue",langues);
    if(choix=="Anglais")
    {
        t.load(":/pics/pics/english.qm");
    }
    if(choix!="Français")
    {
        a.installTranslator(&t);
    }
    Menu w;
    w.show();
    return a.exec();
}


