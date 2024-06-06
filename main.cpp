#include "SocketClient.h"
#include "SerialPort.h"
#include "mmaker.h"
#include <iostream>

int sendFrame() {
    std::string ip_address = "127.0.0.1";  // Use the appropriate IP address
    int port = 5001;                      // Use the appropriate port
    int row = 0;                           // You can change this as needed
    std::string text = "LOL!";    // You can change this as needed

    SocketClient client(ip_address, port);
    if (!client.connectToServer()) {
        return 1;
    }

    std::vector<unsigned char> frame = createFrame(row, text);

    std::cout << "Frame to send: ";
    for (auto byte : frame) {
        printf("%02X ", byte);
    }
    std::cout << std::endl;

    if (client.sendFrame(frame)) {
        std::cout << "Frame sent successfully." << std::endl;
    }

    client.closeConnection();
    return 0;
}

int sendSerial(){
    SerialPort serial("/dev/ttyUSB0", B9600); // Adjust the port and baud rate as needed

    if (!serial.openPort()) {
        std::cerr << "Failed to open port" << std::endl;
        return 1;
    }

    std::string data = "Siema Arduino!";
    if (!serial.sendData(data)) {
        std::cerr << "Failed to send data" << std::endl;
    }

    serial.closePort();
    return 0;
}

int main() {
        menu obj;
    int x;
    obj.menu_head("App Menu");
    obj.add("Send a Frame to Network Device");
    obj.add("Send a Frame to Serial Port Device");
    obj.add("Exit");
    while(true)
    {
        x=obj.display();
        switch(x)
        {
            case 1:
                sendFrame();
                break;
            case 2:
                sendSerial();
                break;
            case 3:
                cout << "BAY BAY\n";
                exit(0);
            default:
                cout << x << endl;
                break;
        }
        getch();
    }
    return 0;
}