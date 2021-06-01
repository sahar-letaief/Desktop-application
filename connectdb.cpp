#include "connectdb.h"
#include "connection.h"

void ConnectDB::Connect()
{
    db = QSqlDatabase::addDatabase("QODBC");
   db.setDatabaseName("secour");
   db.setUserName("secour");//inserer nom de l'utilisateur
   db.setPassword("esprit20");//inserer mot de passe de cet utilisateur

    if (db.open()){
        qDebug("DataBase connected.");
    }
    else
        qDebug("DataBase failed.");
}

Connection::Connection()
{

}
