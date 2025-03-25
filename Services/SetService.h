#pragma once
#include "../Database/BlinkDB.h"

using namespace std;

/**
 * @brief Service class for setting key-value pairs in BlinkDB.
 *
 * This class provides an interface for storing key-value pairs in the BlinkDB database.
 */
class SetService
{
private:
    /**
     * @brief Reference to the BlinkDB instance.
     */
    BlinkDB &blinkDB;

public:
    /**
     * @brief Constructs a SetService instance.
     *
     * @param blinkDB Reference to the BlinkDB database instance.
     */
    SetService(BlinkDB &blinkDB) : blinkDB(blinkDB) {}

    /**
     * @brief Stores a key-value pair in BlinkDB.
     *
     * @param key The key to store.
     * @param value The value associated with the key.
     */
    void set(string key, string value)
    {
        blinkDB.set(key, value);
    }
};
