#include "SerialPort.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

SerialPort::SerialPort(const std::string& portName, int baudRate)
    : portName(portName), baudRate(baudRate), serialPort(-1) {
}

SerialPort::~SerialPort() {
    closePort();
}

bool SerialPort::openPort() {
    serialPort = open(portName.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (serialPort < 0) {
        std::cerr << "Error " << errno << " opening " << portName << ": " << strerror(errno) << std::endl;
        return false;
    }
    return configurePort();
}

void SerialPort::closePort() {
    if (serialPort >= 0) {
        close(serialPort);
        serialPort = -1;
    }
}

bool SerialPort::configurePort() {
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serialPort, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
        return false;
    }

    cfsetospeed(&tty, baudRate);
    cfsetispeed(&tty, baudRate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0; // no signaling chars, no echo, no canonical processing
    tty.c_oflag = 0; // no remapping, no delays
    tty.c_cc[VMIN] = 1; // read blocks
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool SerialPort::sendData(const std::string& data) {
    if (serialPort < 0) {
        std::cerr << "Serial port not open" << std::endl;
        return false;
    }

    int n_written = write(serialPort, data.c_str(), data.length());
    if (n_written < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}
