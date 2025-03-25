#pragma once
#include <string>

using namespace std;

class Command
{
private:
    string command;
    string key;
    string value;

public:
    Command()
    {
        command = "";
        key = "";
        value = "";
    }

    Command(string command, string key, string value)
    {
        this->command = command;
        this->key = key;
        this->value = value;
    }

    Command(string command, string key)
    {
        this->command = command;
        this->key = key;
        this->value = "";
    }

    string getCommand()
    {
        return command;
    }

    string getKey()
    {
        return key;
    }

    string getValue()
    {
        return value;
    }
};