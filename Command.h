#include <bits/stdc++.h>

using namespace std;

class Command
{
private:
    string command;
    string key;
    string value;

public:
    Command() {}

    Command(string command, string key)
    {
        this->command = command;
        this->key = key;
    }

    Command(string command, string key, string value)
    {
        this->command = command;
        this->key = key;
        this->value = value;
    }

    string getCommand()
    {
        return this->command;
    }

    string getKey()
    {
        return this->key;
    }

    string getValue()
    {
        return this->value;
    }
};