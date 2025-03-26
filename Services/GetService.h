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
    explicit GetService(BlinkDB &blinkDB) : blinkDB(blinkDB) {}

    /**
     * @brief Retrieves the value associated with a given key.
     *
     * @param key The key whose value is to be retrieved.
     * @return The value corresponding to the key, or "Key not found" if it does not exist.
     */
    string get(const string &key)
    {
        try
        {
            string value = blinkDB.get(key);
            return value.empty() ? "Key not found" : value;
        }
        catch (const exception &e)
        {
            cerr << "Error retrieving key '" << key << "': " << e.what() << endl;
            return "Error retrieving value";
        }
    }
};
