#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    vector<string> commandType = {"set", "get", "del", "set", "get", "del"};
    string key[] = {"key1", "key2", "key3", "key4", "key5", "key6", "key7", "key8", "key9", "key10"};
    string value[] = {"value1", "value2", "value3", "value4", "value5", "value6", "value7", "value8", "value9", "value10"};
    string command;
    string keyName;
    string keyValue;
    ofstream output;
    cout << "Enter the type of workload you want to generate" << endl;
    cout << "Read Heavy: 1" << endl;
    cout << "Write Heavy: 2" << endl;
    cout << "Balanced: 3" << endl;

    int workloadType;
    cin >> workloadType;

    cout << "Enter the number of commands you want to generate" << endl;
    int numCommands;
    cin >> numCommands;

    if (workloadType == 1)
    {
        commandType[0] = "get";
        commandType[1] = "get";
        commandType[2] = "get";
        commandType[3] = "get";
        commandType[4] = "set";
        commandType[5] = "del";
        output.open("readHeavy_" + to_string(numCommands) + ".txt", ios::out);
    }
    else if (workloadType == 2)
    {
        commandType[0] = "set";
        commandType[1] = "set";
        commandType[2] = "set";
        commandType[3] = "set";
        commandType[4] = "get";
        commandType[5] = "del";
        output.open("writeHeavy_" + to_string(numCommands) + ".txt", ios::out);
    }
    else
    {
        output.open("balanced_" + to_string(numCommands) + ".txt", ios::out);
    }

    for (int i = 0; i < numCommands; i++)
    {
        command = commandType[rand() % commandType.size()];
        keyName = key[rand() % 10];
        keyValue = value[rand() % 10];
        if (!output.is_open())
        {
            cerr << "Unable to open file" << endl;
            break;
        }
        if (command == "set")
        {
            output << command << " " << keyName << " " << keyValue << endl;
        }
        else
        {
            output << command << " " << keyName << endl;
        }
    }
    return 0;
}