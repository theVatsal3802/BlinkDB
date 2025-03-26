#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    vector<string> commandType = {"SET", "GET", "DEL", "SET", "GET", "DEL"};
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
        commandType[0] = "GET";
        commandType[1] = "GET";
        commandType[2] = "GET";
        commandType[3] = "GET";
        commandType[4] = "SET";
        commandType[5] = "DEL";
        output.open("readHeavy_" + to_string(numCommands) + ".txt", ios::out);
    }
    else if (workloadType == 2)
    {
        commandType[0] = "SET";
        commandType[1] = "SET";
        commandType[2] = "SET";
        commandType[3] = "SET";
        commandType[4] = "GET";
        commandType[5] = "DEL";
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
        if (command == "SET")
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