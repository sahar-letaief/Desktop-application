#ifndef ALARMEARDUINO_H
#define ALARMEARDUINO_H

#include <QByteArray>
#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class AlarmeArduino
{
public:
    AlarmeArduino();
    int connect_arduino();
    int close_arduino();
    int write_to_arduino(QByteArray);
    void read_from_arduino();
    QSerialPort* getserial(){return &serial;};
    QString getarduino_port_name(){return arduino_port_name;};
private:
    QSerialPort serial;
    static const quint16 arduino_uno_vendor_id=9025;
    static const quint16 arduino_uno_producy_id=67;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
    QByteArray serialData;
    QString serialBuffer;
};

#endif // ALARMEARDUINO_H
