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

int serverSocket;
BlinkDB blinkDB;
Command command;
APIGateway apiGateway(blinkDB);
DiscBackupHandler discBackupHandler;
Utils utils;
atomic<int> activeConnections(0);
mutex dbMutex;
mutex sendMutex;

void closeServer()
{
    close(serverSocket);
}

void signalHandler(int signal)
{
    cout << "Exiting BlinkDB: Deleting Backups..." << endl;
    discBackupHandler.terminate();
    cout << "Exiting BlinkDB: Deleting Backups... Done" << endl;
    cout << "Exiting BlinkDB: Closing server..." << endl;
    cout << "Exited" << endl;
    closeServer();
    exit(signal);
}

void handleClient(int clientSocket, string mode)
{
    {
        lock_guard<mutex> lock(sendMutex);
        activeConnections++;
    }

    char buffer[512];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            {
                lock_guard<mutex> lock(sendMutex);
                --activeConnections;
            }
            break;
        }

        vector<string> result = utils.fromRESP2(buffer);
        if (result.empty())
            continue;

        if (result[0] == "CONFIG")
            result.erase(result.begin());

        string apiResponse;
        Response response;

        Command command; // Fix: Use local variable instead of global command

        if (result.size() == 3 && result[0] == "SET")
        {
            command = Command(result[0], result[1], result[2]);
        }
        else if (result.size() == 2 && result[0] == "GET")
        {
            command = Command(result[0], result[1]);
        }
        else if (result.size() == 2 && result[0] == "DEL")
        {
            command = Command(result[0], result[1]);
        }
        else if (result[0] == "EXIT")
        {
            break;
        }
        else if (result[0] == "PING")
        {
            response = Response(200, "Success", {"Data", "PONG"});
        }
        else
        {
            response = Response(400, "Bad Request", {"Data", "Invalid command"});
        }

        if (result[0] != "PING")
        {
            {
                lock_guard<mutex> lock(dbMutex);
                apiResponse = apiGateway.executeCommand(command);
            }
            if (mode == "0")
            {
                apiResponse = "+OK\r\n";
                memset(buffer, 0, sizeof(buffer));

                // Fix: Use strncpy to avoid buffer overflow
                strncpy(buffer, apiResponse.c_str(), sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';

                {
                    lock_guard<mutex> lock(sendMutex);
                    send(clientSocket, buffer, strlen(buffer), 0);
                }
            }
            else
            {
                if (apiResponse == "-1" || apiResponse == "-2")
                {
                    response = Response(404, "Not Found", {"Data", "Key not found"});
                }
                else
                {
                    response = Response(200, "Success", {"Data", apiResponse});
                }
                memset(buffer, 0, sizeof(buffer));
                string temp = utils.toRESP2(response.to_string());
                temp = temp.substr(0, temp.size() - 1);

                // Fix: Use strncpy
                strncpy(buffer, temp.c_str(), sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';

                {
                    lock_guard<mutex> lock(sendMutex);
                    send(clientSocket, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    close(clientSocket);
}

int main(int argc, char *argv[])
{
    cout << "Initializing BlinkDB server..." << endl;
    signal(SIGINT, signalHandler);

    if (argc < 2)
    {
        cout << "Enter 1 for Client-server mode and 0 for redis-benchmark mode" << endl;
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        discBackupHandler.terminate();
        return 0;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    int listenStatus = listen(serverSocket, 100000);
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

        thread(handleClient, clientSocket, argv[1]).detach();
    }

    close(serverSocket);
    return 0;
}
