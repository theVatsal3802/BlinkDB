#include <bits/stdc++.h>
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
 * @brief Read-Eval-Print Loop (REPL) for processing user commands.
 *
 * This function continuously prompts the user for input, parses the command,
 * executes it via the API Gateway, and prints the response.
 */
void REPL()
{
    while (true)
    {
        cout << "User > ";
        string input;
        getline(cin, input);
        vector<string> result = utils.splitCommand(input);

        // Hash the key if present
        if (result.size() > 1)
        {
            result[1] = utils.hashKey(result[1]);
        }

        // Process the command based on its type
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

        // Execute the command and retrieve the response
        string apiResponse;
        {
            lock_guard<mutex> lock(dbMutex);
            apiResponse = apiGateway.executeCommand(command);
        }

        // Construct and print the response
        if (apiResponse == "-1")
        {
            Response response(404, "Not Found", {"Data", "Key not found"});
            cout << "Response: " << response.to_string() << endl;
        }
        else if (apiResponse == "-2")
        {
            Response response(500, "Internal Server Error", {"Data", "Error occurred"});
            cout << "Response: " << response.to_string() << endl;
        }
        else
        {
            Response response(200, "Success", {"Data", apiResponse});
            cout << "Response: " << response.to_string() << endl;
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
int main()
{
    cout << "Initializing BlinkDB server..." << endl;

    // Register signal handler for graceful termination
    signal(SIGINT, signalHandler);

    // Start the Read-Eval-Print Loop
    REPL();

    // Cleanup before exiting
    discBackupHandler.terminate();
    cout << "Exiting BlinkDB: Closing server..." << endl;
    cout << "Exited" << endl;

    return 0;
}
