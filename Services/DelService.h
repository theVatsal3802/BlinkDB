#pragma once
#include <bits/stdc++.h>
#include "../Database/BlinkDB.h"

using namespace std;

/**
 * @brief Service class for handling key deletion in BlinkDB.
 *
 * This class provides an interface for deleting keys from the BlinkDB database.
 */
class DelService
{
private:
    /**
     * @brief Reference to the BlinkDB instance.
     */
    BlinkDB &blinkDB;

public:
    /**
     * @brief Constructs a DelService instance.
     *
     * @param blinkDB Reference to the BlinkDB database instance.
     */
    DelService(BlinkDB &blinkDB) : blinkDB(blinkDB) {}

    /**
     * @brief Deletes a key from BlinkDB.
     *
     * @param key The key to be deleted.
     */
    void del(string key)
    {
        blinkDB.del(key);
    }
};
