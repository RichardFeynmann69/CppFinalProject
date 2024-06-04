#include "SocketClient.h"
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

int main() {
        menu obj;
    int x;
    obj.menu_head("App Menu");
    obj.add("Send a Frame to localhost");
    obj.add("Send a Frame to static IP");
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