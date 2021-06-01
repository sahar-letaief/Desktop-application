#include "alarmearduino.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include <QSqlQuery>
#include "alarmeui.h"

AlarmeArduino::AlarmeArduino()
{

}

int AlarmeArduino::connect_arduino(){

    foreach(const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
        if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()){
            if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier() == arduino_uno_producy_id){
                arduino_is_available = true;
                arduino_port_name=serial_port_info.portName();
            }
        }
    }
    qDebug()<< "arduino port name is :" << arduino_port_name;
    if(arduino_is_available){
        serial.setPortName(arduino_port_name);
        if(serial.open(QSerialPort::ReadWrite)) {
           serial.setBaudRate(QSerialPort::Baud9600); // débit:9608 bits/s
           serial.setDataBits(QSerialPort::Data8); //Longueur des données: 8 bits,
           serial.setParity(QSerialPort::NoParity) ;
           serial.setStopBits(QSerialPort::OneStop);
           serial.setFlowControl(QSerialPort::NoFlowControl);

           return 0;
        }
    }
    return 1;
}

int AlarmeArduino::close_arduino(){
    if(serial.isOpen()){
        serial.close();
        return 0;
    }
    return 1;
}

void AlarmeArduino::read_from_arduino(){
    qDebug() << "i am here";
    QStringList buffer_split = serialBuffer.split(",");
    if(buffer_split.length() < 3){
        serialData = serial.readAll();
        qDebug() << serialData;
        if(serialData=="2")
        {
            AlarmeUI a;
            a.init(&serial);
        }
        if(serialData!="2")
        {
            QString DATABASE;
            QSqlQuery qry("select nombre_lits from chambre where numero='"+serialData+"'");
            while(qry.next()){
                DATABASE=qry.value(0).toString();
            }
            QByteArray lits=DATABASE.toLocal8Bit();
            qDebug() << lits;
            AlarmeArduino::write_to_arduino(lits);
        }
        serialData.clear();
    }
}

int AlarmeArduino::write_to_arduino(QByteArray d){
    if(serial.isWritable()){
        serial.write(d);
        return 1;
    }
    else{
        qDebug("Couldn't write to serial!");
        return 0;
    }
}
