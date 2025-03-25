#pragma once

#include <bits/stdc++.h>
#include "../Models/Command.h"
#include "../Services/SetService.h"
#include "../Services/GetService.h"
#include "../Services/DelService.h"
#include "../Cache/Cache.h"

using namespace std;

/**
 * @brief The APIGateway class acts as an intermediary between clients and the BlinkDB storage system.
 *
 * It processes incoming commands, interacts with the cache, and calls the appropriate services
 * for handling `set`, `get`, and `del` operations.
 */
class APIGateway
{
private:
    /**
     * @brief L1 Cache for storing frequently accessed key-value pairs.
     */
    Cache L1Cache;

    /**
     * @brief Reference to the BlinkDB database.
     */
    BlinkDB &blinkDB;

    /**
     * @brief Service for handling `set` operations.
     */
    SetService setService;

    /**
     * @brief Service for handling `get` operations.
     */
    GetService getService;

    /**
     * @brief Service for handling `del` operations.
     */
    DelService delService;

public:
    /**
     * @brief Constructs the APIGateway and initializes services.
     *
     * @param blinkDB Reference to the BlinkDB database.
     */
    APIGateway(BlinkDB &blinkDB) : blinkDB(blinkDB), delService(blinkDB), getService(blinkDB), setService(blinkDB)
    {
        L1Cache = Cache();
    }

    /**
     * @brief Executes a given command by routing it to the appropriate service.
     *
     * - `set`: Stores the key-value pair in cache and database.
     * - `get`: Retrieves the value from cache or database.
     * - `del`: Deletes the key from cache and database.
     *
     * @param command The command to be executed.
     * @return string Response indicating success or failure.
     */
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
