#ifndef ARDUINOW_H
#define ARDUINOW_H

#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>


class arduinow
{

public:
    arduinow();
    int connect_arduinow();
    int close_arduinow();
    int write_to_arduinow(QByteArray);
    QByteArray read_from_arduinow();
    QSerialPort* getserial();
    QString getarduinow_port_name();

private:
    QSerialPort * serial;
    static const quint16 arduino_uno_vendor_id=9025;
    static const quint16 arduino_uno_producy_id=67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray datta;
};

#endif // ARDUINOW_H
