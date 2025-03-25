/**
 * @file BlinkDBClient.cpp
 * @brief Client program to interact with the BlinkDB server using TCP sockets.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <chrono>

using namespace std;

/**
 * @brief Main function to establish a connection with the BlinkDB server and send commands.
 * @return int Returns 0 on successful execution, -1 if socket creation fails, and -2 if the connection fails.
 */
int main(int argc, char *argv[])
{
    cout << "Connecting to BlinkDB server..." << endl;

    if (argc < 2)
    {
        cout << "Enter 0 for interactive mode and 1 for file mode in command line and a filename for file mode." << endl;
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }
    /// Create a TCP socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    /// Define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);                   // Port number
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    /// Attempt to connect to the BlinkDB server
    int connectionStatus = connect(clientSocket, (sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectionStatus == -1)
    {
        cerr << "Connection to BlinkDB failed." << endl;
        return -2;
    }
    cout << "Connected to BlinkDB server." << endl;

    int mode = int(argv[1]);
    string filename = string(argv[2] != NULL ? argv[2] : "");

    if (mode != 0 || mode != 1)
    {
        cout << "Enter 0 for interactive mode and 1 for file mode" << endl;
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }

    if (mode == 1 && filename == "")
    {
        cout << "Please provide a filename for the test file" << endl;
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }

    if (mode == 1)
    {
        auto start = chrono::high_resolution_clock::now();
        ifstream testFile(filename);
        string line;
        while (getline(testFile, line))
        {
            send(clientSocket, line.c_str(), line.size() + 1, 0);
            char response[512];
            memset(response, 0, sizeof(response));
            recv(clientSocket, response, sizeof(response), 0);
            cout << "Server > " << response << endl;
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Time taken to execute all commands: " << elapsed.count() << "s" << endl;
    }
    else if (mode == 0)
    {
        while (true)
        {
            cout << "User > ";
            string input;
            getline(cin, input);

            /// Validate user input
            if (input.empty())
            {
                cout << "Invalid Command\n";
                continue;
            }

            /// Send command to server
            send(clientSocket, input.c_str(), input.size() + 1, 0);

            /// Handle exit command
            if (input == "exit")
            {
                close(clientSocket);
                cout << "Exiting BlinkDB..." << endl;
                break;
            }

            /// Receive and display server response
            char response[512];
            memset(response, 0, sizeof(response));
            recv(clientSocket, response, sizeof(response), 0);
            cout << "Server > " << response << endl;
        }
    }

    return 0;
}
