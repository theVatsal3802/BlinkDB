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
 * @brief Converts a string to Redis Serialization Protocol (RESP2) format.
 *
 * @param data The input string.
 * @return The RESP2 formatted string.
 */
string toRESP2(const string &command)
{
    istringstream stream(command);
    vector<string> tokens;
    string word;

    while (stream >> word)
    {
        tokens.push_back(word);
    }

    string result = "*" + to_string(tokens.size()) + "\r\n";
    for (const auto &token : tokens)
    {
        result += "$" + to_string(token.size()) + "\r\n" + token + "\r\n";
    }

    return result;
}

/**
 * @brief Parses a RESP2-formatted string back to a normal string.
 *
 * @param resp The RESP2-formatted string.
 * @return The extracted string data.
 */
vector<string> fromRESP2(const string &resp)
{
    vector<string> result;
    istringstream stream(resp);
    string line;

    getline(stream, line, '\r');
    if (line[0] != '*')
        return {}; // Must start with '*'

    int numArgs = stoi(line.substr(1)); // Number of arguments
    stream.ignore(1);                   // Ignore '\n'

    for (int i = 0; i < numArgs; i++)
    {
        getline(stream, line, '\r');
        if (line[0] != '$')
            return {}; // Must start with '$'

        int len = stoi(line.substr(1)); // Get length of argument
        stream.ignore(1);               // Ignore '\n'

        string arg(len, ' ');
        stream.read(&arg[0], len); // Read the argument
        result.push_back(arg);

        stream.ignore(2); // Ignore '\r\n'
    }

    return result;
}

/**
 * @brief Splits a command string into at most three parts (command, key, and value).
 *
 * @param command The input command string.
 * @return A vector containing the split components (command, key, and optionally value).
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
        cout << "Executing commands from " << filename << "... Please wait..." << endl;
        while (getline(testFile, line))
        {
            // Convert command to RESP2 format and send it
            struct timeval timeout;
            timeout.tv_sec = 5; // Set timeout to 5 seconds
            timeout.tv_usec = 0;

            setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            string resp = toRESP2(line);
            send(clientSocket, resp.substr(0, resp.size() - 1).c_str(), resp.size(), 0);

            // Receive response from server
            char response[512];
            memset(response, 0, sizeof(response));
            recv(clientSocket, response, sizeof(response), 0);
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
            struct timeval timeout;
            timeout.tv_sec = 5; // Set timeout to 5 seconds
            timeout.tv_usec = 0;

            setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
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
            vector<string> responseStr = fromRESP2(response);
            cout << "Server > ";
            for (auto word : responseStr)
            {
                cout << word << " ";
            }
            cout << endl;
        }
    }

    return 0;
}