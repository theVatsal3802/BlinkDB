#pragma once
#include <bits/stdc++.h>

using namespace std;

/**
 * @brief The Cache class provides an in-memory key-value store.
 *
 * This class is used to temporarily store frequently accessed data
 * for faster retrieval, reducing database queries.
 */
class Cache
{
private:
    /**
     * @brief Internal unordered map for storing key-value pairs.
     */
    unordered_map<string, string> cache;

public:
    /**
     * @brief Stores a key-value pair in the cache.
     *
     * @param key The key to store.
     * @param value The corresponding value.
     */
    void set(string key, string value)
    {
        cache[key] = value;
    }

    /**
     * @brief Retrieves a value associated with the given key.
     *
     * @param key The key to search for.
     * @return string The corresponding value, or "-1" if the key is not found.
     */
    string get(string key)
    {
        if (cache.find(key) == cache.end())
        {
            return "-1";
        }
        return cache[key];
    }

    /**
     * @brief Deletes a key-value pair from the cache.
     *
     * @param key The key to delete.
     */
    void del(string key)
    {
        cache.erase(key);
    }

    /**
     * @brief Gets the current size of the cache.
     *
     * @return size_t The number of key-value pairs stored.
     */
    size_t getSize()
    {
        return cache.size();
    }

    /**
     * @brief Clears all key-value pairs from the cache.
     */
    void clear()
    {
        cache.clear();
    }
};
