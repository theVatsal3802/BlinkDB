#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <atomic>
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
 * @brief Mutex to synchronize access to the database.
 */
mutex dbMutex;

/**
 * @brief Handles termination signals (e.g., Ctrl+C).
 *
 * This function ensures a graceful shutdown of BlinkDB by cleaning up backups
 * and closing the server safely before exiting the program.
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
    exit(0);
}

/**
 * @brief Parses and executes a given command.
 *
 * This function tokenizes the input command, determines its validity,
 * executes the appropriate API request, and returns the response.
 *
 * @param input The command entered by the user.
 * @param mode Execution mode (interactive or batch file execution).
 * @return int Returns -1 for exit, 0 for invalid command, otherwise continues execution.
 */
int executeCommand(string input, string mode)
{
    vector<string> result = utils.splitCommand(input);

    // Process the command based on its type
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
        return -1;
    }
    else
    {
        cout << "Invalid command" << endl;
        return 0;
    }

    // Execute the command and retrieve the response
    string apiResponse;
    {
        lock_guard<mutex> lock(dbMutex); // Ensures thread safety while accessing the database
        apiResponse = apiGateway.executeCommand(command);
    }

    // Construct and print the response
    Response response;
    if (apiResponse == "-1" || apiResponse == "-2")
    {
        response = Response(404, "Not Found", {"Data", "Key not found"});
    }
    else
    {
        response = Response(200, "Success", {"Data", apiResponse});
    }

    // Print response based on mode
    if (mode == "1" && command.getCommand() == "GET")
    {
        cout << response.getValue() << endl;
    }
    else if (mode == "0")
    {
        cout << "Response: " << response.to_string() << endl;
    }
    return 0;
}

/**
 * @brief Read-Eval-Print Loop (REPL) for processing user commands.
 *
 * This function continuously prompts the user for input, parses the command,
 * executes it via the API Gateway, and prints the response. It supports both
 * interactive and batch (file-based) execution modes.
 *
 * @param mode Execution mode ("0" for interactive, "1" for batch processing).
 * @param filename Optional filename for batch execution.
 */
void REPL(string mode, string filename = "")
{
    if (mode == "1")
    {
        chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
        ifstream testFile(filename);
        string line;
        while (getline(testFile, line))
        {
            executeCommand(line, mode);
        }
        chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Time taken to execute all commands: " << elapsed.count() << "s" << endl;
    }
    else if (mode == "0")
    {
        while (true)
        {
            cout << "User > ";
            string input;
            getline(cin, input);
            int result = executeCommand(input, mode);
            if (result == -1)
            {
                break;
            }
        }
    }
}

/**
 * @brief Entry point of the BlinkDB server.
 *
 * This function initializes the server, sets up a signal handler for termination,
 * starts the REPL loop, and gracefully shuts down the system.
 *
 * @return int Exit status code.
 */
int main(int argc, char *argv[])
{
    cout << "Initializing BlinkDB server..." << endl;

    // Register signal handler for graceful termination
    signal(SIGINT, signalHandler);
    if (argc < 2)
    {
        cout << "Enter 0 for interactive mode and 1 for file mode in command line and a filename for file mode." << endl;
        discBackupHandler.terminate();
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }
    string mode = string(argv[1]);
    string filename = string(argv[2] != NULL ? argv[2] : "");
    if (mode != "0" && mode != "1")
    {
        cout << "Enter 0 for interactive mode and 1 for file mode" << endl;
        discBackupHandler.terminate();
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }

    if (mode == "1" && filename == "")
    {
        cout << "Please provide a filename for the test file" << endl;
        discBackupHandler.terminate();
        cout << "Exiting BlinkDB: Closing server..." << endl;
        cout << "Exited" << endl;
        return 0;
    }
    // Start the Read-Eval-Print Loop
    REPL(mode, filename);

    // Cleanup before exiting
    discBackupHandler.terminate();
    cout << "Exiting BlinkDB: Closing server..." << endl;
    cout << "Exited" << endl;

    return 0;
}
