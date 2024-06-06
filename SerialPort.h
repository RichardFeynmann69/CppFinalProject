#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <vector>
#include <termios.h>

class SerialPort {
public:
    SerialPort(const std::string& portName, int baudRate);
    ~SerialPort();
    
    bool openPort();
    void closePort();
    bool sendData(const std::string& data);

private:
    std::string portName;
    int baudRate;
    int serialPort;
    struct termios tty;

    bool configurePort();
};

#endif // SERIALPORT_H
