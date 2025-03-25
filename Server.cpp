#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <atomic>
#include <csignal>
#include "./APIGateway/APIGateway.h"
#include "./Models/Response.h"

using namespace std;

BlinkDB blinkDB;
Command command;
APIGateway apiGateway(blinkDB);
DiscBackupHandler discBackupHandler;
Utils utils;

atomic<int> activeConnections(0);
mutex dbMutex;

void signalHandler(int signal)
{
    cout << "Exiting BlinkDB: Deleting Backups..." << endl;
    discBackupHandler.terminate();
    cout << "Exiting BlinkDB: Deleting Backups... Done" << endl;
    cout << "Exiting BlinkDB: Closing server..." << endl;
    cout << "Exited" << endl;
    exit(signal);
}

void handleClient(int clientSocket)
{
    activeConnections++;
    cout << "New client connected. Active clients: " << activeConnections << endl;

    char buffer[512];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            cout << "Client disconnected. Active clients: " << --activeConnections << endl;
            break;
        }

        vector<string> result = utils.splitCommand(buffer);

        if (result.size() > 1)
        {
            result[1] = utils.hashKey(result[1]);
        }

        if (result.size() == 3 && result[0] == "set")
        {
            command = Command(result[0], result[1], result[2]);
        }
        else if (result.size() == 2 && result[0] == "get")
        {
            command = Command(result[0], result[1]);
        }
        else if (result.size() == 2 && result[0] == "del")
        {
            command = Command(result[0], result[1]);
        }
        else if (result[0] == "exit")
        {
            break;
        }
        else
        {
            cout << "Invalid command" << endl;
            continue;
        }

        string apiResponse;
        {
            lock_guard<mutex> lock(dbMutex);
            apiResponse = apiGateway.executeCommand(command);
        }

        Response response(200, "Success", {"Data", apiResponse});
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, response.to_string().c_str());
        send(clientSocket, buffer, strlen(buffer), 0);
    }

    close(clientSocket);
}

int main()
{
    cout << "Initializing BlinkDB server..." << endl;
    signal(SIGINT, signalHandler);
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int bindStatus = bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindStatus == -1)
    {
        cerr << "Binding to port 5000 failed." << endl;
        return -2;
    }

    cout << "Enter maximum number of connections: ";
    int maxConnections;
    cin >> maxConnections;

    int listenStatus = listen(serverSocket, maxConnections);
    if (listenStatus == -1)
    {
        cerr << "Listening on port 5000 failed." << endl;
        return -3;
    }

    cout << "Initialized BlinkDB server." << endl;
    cout << "Listening on port 5000..." << endl;
    cout << "Press Ctrl+C to exit." << endl;

    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressSize);
        if (clientSocket == -1)
        {
            cerr << "Accepting connection failed." << endl;
            continue;
        }

        thread(handleClient, clientSocket).detach();
    }
    close(serverSocket);
    return 0;
}
