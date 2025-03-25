#pragma once
#include <boost/container_hash/hash.hpp>
#include <bits/stdc++.h>

using namespace std;

/**
 * @brief Utility class providing helper functions for hashing, string manipulation, and pattern matching.
 */
class Utils
{
public:
    /**
     * @brief Generates a hash value for a given key.
     *
     * @param key The input string to hash.
     * @return A string representing the hashed value.
     */
    string hashKey(string key)
    {
        hash<string> hash_fn;
        size_t hash = hash_fn(key);
        return to_string(hash);
    }

    /**
     * @brief Splits a command string into separate parts based on spaces.
     *
     * @param command The input command string.
     * @return A vector containing the split components of the command.
     */
    vector<string> splitCommand(string command)
    {
        vector<string> result;
        string word = "";
        int count = 2;
        for (auto x : command)
        {
            if (x == '\0')
            {
                break;
            }

            if (x == ' ' && count > 0)
            {
                result.push_back(word);
                word = "";
                count--;
            }
            else
            {
                word = word + x;
            }
        }
        result.push_back(word);
        return result;
    }

    /**
     * @brief Checks if a given string starts with a specified prefix.
     *
     * @param str The main string to check.
     * @param prefix The prefix to compare.
     * @return true if the string starts with the prefix, false otherwise.
     */
    bool startsWith(string str, string prefix)
    {
        return str.rfind(prefix, 0) == 0;
    }
};
