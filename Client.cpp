#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    cout << "Connecting to BlinkDB server..." << endl;
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connectionStatus = connect(clientSocket, (sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectionStatus == -1)
    {
        cerr << "Connection to BlinkDB failed." << endl;
        return -2;
    }
    cout << "Connected to BlinkDB server." << endl;

    while (true)
    {
        cout << "User > ";
        string input;
        getline(cin, input);
        if (input == "" || input == "\n")
        {
            cout << "Invalid Command\n";
            continue;
        }
        send(clientSocket, input.c_str(), input.size() + 1, 0);
        if (input == "exit")
        {
            close(clientSocket);
            cout << "Exiting BlinkDB..." << endl;
            break;
        }
        char response[512];
        memset(response, 0, 512);
        recv(clientSocket, response, sizeof(response), 0);
        cout << "Server > " << response << endl;
    }
}