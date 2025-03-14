#include <bits/stdc++.h>
#include "Command.h"
#include "Data.h"

using namespace std;

class DBEngine
{
private:
    vector<Data> data;
    int MAX_SIZE = INT_MAX;
    long long int currentTime;

    bool flushToDisc()
    {
        // Write Flushing Logic Here
        // If success, return true else false
        return true;
    }

    // Commands here assume that they have the correct format for the typr of command being handles.
    // Eg: Set Command has the format "set key value" and here it is assumed to be true.
    bool handleSet(Command command)
    {
        return true;
    }

    string handleGet(Command command)
    {
        return "";
    }

    bool handleDel(Command command)
    {
        return true;
    }

public:
    DBEngine()
    {
        this->currentTime = 0;
    }

    string handleCommand(Command command)
    {
        if (command.getCommand() == "set")
        {
            bool result = handleSet(command);
            if (!result)
            {
                throw runtime_error("Failed to Set Data");
            }
            return "Success";
        }
        else if (command.getCommand() == "get")
        {
            string data = handleGet(command);
            if (data != "")
            {
                return data;
            }
            throw runtime_error("Failed to get data.");
        }
        else if (command.getCommand() == "del")
        {
            bool result = handleDel(command);
            if (!result)
            {
                throw runtime_error("Failed to delete data.");
            }
            return "Success";
        }
        else
        {
            throw runtime_error("Invalid Command.");
        }
        return "";
    }
};