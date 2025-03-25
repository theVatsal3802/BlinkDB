#pragma once
#include <bits/stdc++.h>
#include "../Database/BlinkDB.h"

using namespace std;

/**
 * @brief Service class for retrieving values from BlinkDB.
 *
 * This class provides an interface for fetching values associated with a given key.
 */
class GetService
{
private:
    /**
     * @brief Reference to the BlinkDB instance.
     */
    BlinkDB &blinkDB;

public:
    /**
     * @brief Constructs a GetService instance.
     *
     * @param blinkDB Reference to the BlinkDB database instance.
     */
    GetService(BlinkDB &blinkDB) : blinkDB(blinkDB) {}

    /**
     * @brief Retrieves the value associated with a given key.
     *
     * @param key The key whose value is to be retrieved.
     * @return The value corresponding to the key, or an appropriate error message if the key does not exist.
     */
    string get(string key)
    {
        return blinkDB.get(key);
    }
};
