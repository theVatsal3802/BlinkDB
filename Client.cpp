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
#include <sstream>

using namespace std;

/**
 * @brief Converts a string into RESP2 protocol format.
 * @param data The command string to be sent to the server.
 * @return The RESP2 formatted string.
 */
string toRESP2(const string &data)
{
    return "$" + to_string(data.size()) + "\r\n" + data + "\r\n";
}

/**
 * @brief Extracts the actual response from a RESP2 formatted string.
 * @param resp The RESP2 formatted response string received from the server.
 * @return The extracted response string.
 */
string fromRESP2(const string &resp)
{
    if (resp.empty() || resp[0] != '$')
        return "";

    istringstream stream(resp);
    string lengthLine, data;

    getline(stream, lengthLine); // Read first line ($length)
    getline(stream, data);       // Read actual string

    return data;
}

/**
 * @brief Splits the command string into individual words.
 * @param command The command string to be split.
 * @return A vector of strings containing the individual words.
 */
vector<string> splitCommand(string command)
{
    vector<string> result;
    string word = "";
    int count = 2;
    for (auto x : command)
    {
        if (x == '\0')
        {
            break;
        }

        if (x == ' ' && count > 0)
        {
            result.push_back(word);
            word = "";
            count--;
        }
        else
        {
            word = word + x;
        }
    }
    result.push_back(word);
    return result;
}

/**
 * @brief Main function to establish a connection with the BlinkDB server and send commands.
 * @return int Returns 0 on successful execution, -1 if socket creation fails, and -2 if the connection fails.
 */
int main(int argc, char *argv[])
{
    cout << "Connecting to BlinkDB server..." << endl;

    // Check if sufficient arguments are provided
    if (argc < 2)
    {
        cout << "Enter 0 for interactive mode and 1 for file mode in command line and a filename for file mode." << endl;
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }

    // Create a TCP socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    // Define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);                   // Port number
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    // Attempt to connect to the BlinkDB server
    int connectionStatus = connect(clientSocket, (sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectionStatus == -1)
    {
        cerr << "Connection to BlinkDB failed." << endl;
        return -2;
    }
    cout << "Connected to BlinkDB server." << endl;

    string mode = string(argv[1]);
    string filename = string(argv[2] != NULL ? argv[2] : "");

    // Validate mode input
    if (mode != "0" && mode != "1")
    {
        cout << "Enter 0 for interactive mode and 1 for file mode" << endl;
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }

    // Validate filename in file mode
    if (mode == "1" && filename == "")
    {
        cout << "Please provide a filename for the test file" << endl;
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }

    // File mode execution
    if (mode == "1")
    {
        chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
        ifstream testFile(filename);
        string line;
        while (getline(testFile, line))
        {
            // Convert command to RESP2 format and send it
            string resp = toRESP2(line);
            send(clientSocket, resp.substr(0, resp.size() - 1).c_str(), resp.size(), 0);

            // Receive response from server
            char response[512];
            memset(response, 0, sizeof(response));
            recv(clientSocket, response, sizeof(response), 0);

            // Process and display response
            string responseStr = fromRESP2(response);
            vector<string> responseWords = splitCommand(line);
            if (responseWords[0] == "get")
            {
                cout << "Server > " << responseStr << endl;
            }
        }
        chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Time taken to execute all commands: " << elapsed.count() << "s" << endl;
    }
    // Interactive mode execution
    else if (mode == "0")
    {
        while (true)
        {
            cout << "User > ";
            string input;
            getline(cin, input);

            // Validate user input
            if (input.empty())
            {
                cout << "Invalid Command\n";
                continue;
            }

            // Send command to server
            string resp = toRESP2(input);
            send(clientSocket, resp.substr(0, resp.size() - 1).c_str(), resp.size(), 0);

            // Handle exit command
            if (input == "exit")
            {
                close(clientSocket);
                cout << "Exiting BlinkDB..." << endl;
                break;
            }

            // Receive and display server response
            char response[512];
            memset(response, 0, sizeof(response));
            recv(clientSocket, response, sizeof(response), 0);
            string responseStr = fromRESP2(response);
            cout << "Server > " << responseStr << endl;
        }
    }

    return 0;
}