#include <bits/stdc++.h>
#include <atomic>
#include "./APIGateway/APIGateway.h"
#include "./Models/Response.h"

using namespace std;

BlinkDB blinkDB;
Command command;
APIGateway apiGateway(blinkDB);
DiscBackupHandler discBackupHandler;
Utils utils;

mutex dbMutex;

void signalHandler(int signal)
{
    cout << "Exiting BlinkDB: Deleting Backups..." << endl;
    discBackupHandler.terminate();
    cout << "Exiting BlinkDB: Deleting Backups... Done" << endl;
    cout << "Exiting BlinkDB: Closing server..." << endl;
    cout << "Exited" << endl;
    exit(0);
}

void REPL()
{
    while (true)
    {
        cout << "User > ";
        string input;
        getline(cin, input);
        vector<string> result = utils.splitCommand(input);

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
        cout << "Response: " << response.to_string() << endl;
    }
}

int main()
{
    cout << "Initializing BlinkDB server..." << endl;
    signal(SIGINT, signalHandler);
    REPL();
    discBackupHandler.terminate();
    cout << "Exiting BlinkDB: Closing server..." << endl;
    cout << "Exited" << endl;
    return 0;
}
