#include <bits/stdc++.h>
#include "Utils.h"
#include "DBEngine.h"

using namespace std;

int main()
{
    Utils utils = Utils();
    DBEngine dbengine = DBEngine();
    while (true)
    {
        cout << "User > ";
        string userInput;
        getline(cin, userInput);
        vector<string> params = utils.split(userInput, 32);
        Command command;
        params[0] = utils.convertToLowercase(params[0]);
        switch (params.size())
        {
        case 1:
            if (params[0] == "exit")
            {
                utils.terminate();
                // To be removed later.
                cout << "Called Terminate" << endl;
                exit(0);
            }
            else
            {
                cout << "Invalid Command\n";
            }
            break;

        case 2:
            command = Command(params[0], params[1]);
            break;

        case 3:
            command = Command(params[0], params[1], params[2]);
            break;

        default:
            cout << "Invalid Command\n";
            break;
        }
        dbengine.handleCommand(command);
    }

    return 0;
}