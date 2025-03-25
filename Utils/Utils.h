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
     * @brief Hashes a string using Boost's hash function.
     *
     * @param key The input string to hash.
     * @return The hashed string value.
     */
    string hash(const string &key)
    {
        boost::hash<string> hash_fn;
        size_t hash = hash_fn(key);
        return to_string(hash);
    }

    /**
     * @brief Splits a command string into at most three parts (command, key, and value).
     *
     * @param command The input command string.
     * @return A vector containing the split components (command, key, and optionally value).
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
    bool startsWith(const string &str, const string &prefix)
    {
        return str.rfind(prefix, 0) == 0;
    }

    /**
     * @brief Converts a string to Redis Serialization Protocol (RESP2) format.
     *
     * @param data The input string.
     * @return The RESP2 formatted string.
     */
    string toRESP2(const string &data)
    {
        return "$" + to_string(data.size()) + "\r\n" + data + "\r\n";
    }

    /**
     * @brief Parses a RESP2-formatted string back to a normal string.
     *
     * @param resp The RESP2-formatted string.
     * @return The extracted string data.
     */
    string fromRESP2(const string &resp)
    {
        if (resp.empty() || resp[0] != '$')
            return "";

        istringstream stream(resp);
        string lengthLine, data;

        getline(stream, lengthLine); // Read first line ($length)
        getline(stream, data);       // Read actual string

        // Trim trailing \r if present
        if (!lengthLine.empty() && lengthLine.back() == '\r')
        {
            lengthLine.pop_back();
        }
        if (!data.empty() && data.back() == '\r')
        {
            data.pop_back();
        }

        return data;
    }
};
