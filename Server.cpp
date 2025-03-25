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

/**
 * @brief Represents the main database instance.
 */
BlinkDB blinkDB;

/**
 * @brief Stores the command to be executed.
 */
Command command;

/**
 * @brief Handles API requests for executing database commands.
 */
APIGateway apiGateway(blinkDB);

/**
 * @brief Manages disk backup operations.
 */
DiscBackupHandler discBackupHandler;

/**
 * @brief Utility class for helper functions.
 */
Utils utils;

/**
 * @brief Tracks the number of active client connections.
 */
atomic<int> activeConnections(0);

/**
 * @brief Mutex to synchronize access to the database.
 */
mutex dbMutex;

/**
 * @brief Handles termination signals (e.g., Ctrl+C).
 *
 * Terminates ongoing operations, deletes backups, and shuts down the server.
 *
 * @param signal The received signal code.
 */
void signalHandler(int signal)
{
    cout << "Exiting BlinkDB: Deleting Backups..." << endl;
    discBackupHandler.terminate();
    cout << "Exiting BlinkDB: Deleting Backups... Done" << endl;
    cout << "Exiting BlinkDB: Closing server..." << endl;
    cout << "Exited" << endl;
    exit(signal);
}

/**
 * @brief Handles an individual client connection.
 *
 * Receives commands from the client, processes them, executes the necessary database operations,
 * and sends the response back to the client.
 *
 * @param clientSocket The socket file descriptor for the client connection.
 */
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

/**
 * @brief Entry point of the BlinkDB server.
 *
 * Initializes the server, sets up a signal handler for termination, starts listening for client connections,
 * and creates separate threads for each connected client.
 *
 * @return int Exit status code.
 */
int main()
{
    cout << "Initializing BlinkDB server..." << endl;

    // Register signal handler for graceful termination
    signal(SIGINT, signalHandler);

    // Create a server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    // Configure server address settings
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the port
    int bindStatus = bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindStatus == -1)
    {
        cerr << "Binding to port 5000 failed." << endl;
        return -2;
    }

    // Accept the maximum number of connections from the user
    cout << "Enter maximum number of connections: ";
    int maxConnections;
    cin >> maxConnections;

    // Start listening for client connections
    int listenStatus = listen(serverSocket, maxConnections);
    if (listenStatus == -1)
    {
        cerr << "Listening on port 5000 failed." << endl;
        return -3;
    }

    cout << "Initialized BlinkDB server." << endl;
    cout << "Listening on port 5000..." << endl;
    cout << "Press Ctrl+C to exit." << endl;

    // Accept and handle client connections in separate threads
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
