#pragma once

#include <bits/stdc++.h>
#include "../Models/Command.h"
#include "../Services/SetService.h"
#include "../Services/GetService.h"
#include "../Services/DelService.h"
#include "../Cache/Cache.h"

using namespace std;

/**
 * @class APIGateway
 * @brief Acts as an intermediary between clients and the BlinkDB storage system.
 *
 * The APIGateway processes incoming commands, interacts with the cache, and
 * routes requests to the appropriate services for handling `set`, `get`, and `del` operations.
 */
class APIGateway
{
private:
    /**
     * @brief L1 cache for storing frequently accessed key-value pairs.
     *
     * The cache is used to improve performance by avoiding unnecessary database queries.
     */
    Cache L1Cache;

    /**
     * @brief Reference to the BlinkDB database.
     *
     * This allows direct interaction with the main database when necessary.
     */
    BlinkDB &blinkDB;

    /**
     * @brief Service for handling `set` operations.
     *
     * This service is responsible for storing key-value pairs in the database.
     */
    SetService setService;

    /**
     * @brief Service for handling `get` operations.
     *
     * This service retrieves values associated with keys from the database.
     */
    GetService getService;

    /**
     * @brief Service for handling `del` operations.
     *
     * This service deletes key-value pairs from the database.
     */
    DelService delService;

public:
    /**
     * @brief Constructs the APIGateway and initializes services.
     *
     * This constructor initializes the cache and services required for handling
     * database interactions.
     *
     * @param blinkDB Reference to the BlinkDB database instance.
     */
    APIGateway(BlinkDB &blinkDB) : blinkDB(blinkDB), setService(blinkDB), getService(blinkDB), delService(blinkDB)
    {
        L1Cache = Cache();
    }

    /**
     * @brief Executes a given command by routing it to the appropriate service.
     *
     * This function determines the command type and delegates processing to
     * the corresponding service (`set`, `get`, `del`).
     *
     * - `set`: Stores the key-value pair in both cache and database.
     * - `get`: Retrieves the value from the cache or falls back to the database.
     * - `del`: Removes the key from both cache and database.
     *
     * @param command The command object containing operation type, key, and optional value.
     * @return std::string Response message indicating success or failure.
     */
    string executeCommand(Command command)
    {
        if (command.getCommand() == "SET")
        {
            // Clear cache if size exceeds limit
            if (L1Cache.getSize() >= 100000000)
            {
                L1Cache.clear();
            }
            // Store in cache and database
            L1Cache.set(command.getKey(), command.getValue());
            setService.set(command.getKey(), command.getValue());
            return "Set Success";
        }
        else if (command.getCommand() == "GET")
        {
            // Check cache first
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
        else if (command.getCommand() == "DEL")
        {
            // Remove from cache and database
            L1Cache.del(command.getKey());
            delService.del(command.getKey());
            return "Deletion Success";
        }
        return "Invalid Command";
    }
};
