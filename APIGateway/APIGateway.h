#pragma once

#include <bits/stdc++.h>
#include "../Models/Command.h"
#include "../Services/SetService.h"
#include "../Services/GetService.h"
#include "../Services/DelService.h"
#include "../Cache/Cache.h"

using namespace std;

class APIGateway
{
private:
    Cache L1Cache;
    BlinkDB &blinkDB;
    SetService setService;
    GetService getService;
    DelService delService;

public:
    APIGateway(BlinkDB &blinkDB) : blinkDB(blinkDB), delService(blinkDB), getService(blinkDB), setService(blinkDB)
    {
        L1Cache = Cache();
    }

    string executeCommand(Command command)
    {
        if (command.getCommand() == "set")
        {
            if (L1Cache.getSize() >= 100000000)
            {
                L1Cache.clear();
            }
            L1Cache.set(command.getKey(), command.getValue());
            setService.set(command.getKey(), command.getValue());
            return "Set Success";
        }
        else if (command.getCommand() == "get")
        {
            string cacheCheckResult = L1Cache.get(command.getKey());
            if (cacheCheckResult == "-1")
            {
                return getService.get(command.getKey());
            }
            else
            {
                return cacheCheckResult;
            }
        }
        else if (command.getCommand() == "del")
        {
            L1Cache.del(command.getKey());
            delService.del(command.getKey());
            return "Deletion Success";
        }
        return "Invalid Command";
    }
};