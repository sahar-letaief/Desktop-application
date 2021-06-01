#include "arduinow.h"
#include <QDebug>
arduinow::arduinow()
{
    datta="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;

}

QString arduinow::getarduinow_port_name()
{return arduino_port_name;}
QSerialPort* arduinow::getserial()
{
    return serial;
}

int arduinow::connect_arduinow()
{
    foreach (const QSerialPortInfo &serial_port_info,QSerialPortInfo::availablePorts()){
        if(serial_port_info.hasVendorIdentifier() &&serial_port_info.hasProductIdentifier()){
            if(serial_port_info.vendorIdentifier() ==arduino_uno_vendor_id && serial_port_info.productIdentifier()
                    == arduino_uno_producy_id) {
                arduino_is_available = true;
                arduino_port_name=serial_port_info.portName();

           }}}
    qDebug() <<"arduino_port_name is :" << arduino_port_name;
    if(arduino_is_available){
        serial->setPortName(arduino_port_name);
        if(serial->open(QSerialPort::ReadWrite)){
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;
        }
        return 1;
    }
    return -1;
}

int arduinow::close_arduinow()
{
    if(serial->isOpen()){
        serial->close();
        return 0;
    }
    return 1;
}

int arduinow::write_to_arduinow(QByteArray d)
{
    if(serial->isWritable()){
        serial->write(d);
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}

QByteArray arduinow::read_from_arduinow()
{
    if(serial->isReadable()){
        datta=serial->readAll();
        return datta;
    }
}
