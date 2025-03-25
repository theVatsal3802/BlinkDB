#pragma once
#include <bits/stdc++.h>

using namespace std;

/**
 * @class Cache
 * @brief Provides an in-memory key-value store.
 *
 * The Cache class is designed to store frequently accessed key-value pairs
 * to improve performance by reducing direct database queries.
 */
class Cache
{
private:
    /**
     * @brief Internal storage for key-value pairs.
     *
     * An unordered map is used for fast lookup and storage.
     */
    unordered_map<string, string> cache;

public:
    /**
     * @brief Stores a key-value pair in the cache.
     *
     * This function inserts a new key-value pair into the cache.
     * If the key already exists, its value is updated.
     *
     * @param key The key to be stored.
     * @param value The corresponding value.
     */
    void set(string key, string value)
    {
        cache[key] = value;
    }

    /**
     * @brief Retrieves a value associated with the given key.
     *
     * This function looks up a key in the cache and returns the associated value.
     * If the key is not found, it returns "-1".
     *
     * @param key The key to search for.
     * @return std::string The corresponding value if found, otherwise "-1".
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
     * If the key exists in the cache, it is removed.
     *
     * @param key The key to be deleted.
     */
    void del(string key)
    {
        cache.erase(key);
    }

    /**
     * @brief Gets the current size of the cache.
     *
     * This function returns the number of key-value pairs currently stored in the cache.
     *
     * @return size_t The total number of stored key-value pairs.
     */
    size_t getSize()
    {
        return cache.size();
    }

    /**
     * @brief Clears all key-value pairs from the cache.
     *
     * This function removes all stored entries, effectively resetting the cache.
     */
    void clear()
    {
        cache.clear();
    }
};
