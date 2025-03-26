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
    string toRESP2(const string &command)
    {
        istringstream stream(command);
        vector<string> tokens;
        string word;

        while (stream >> word)
        {
            tokens.push_back(word);
        }

        string result = "*" + to_string(tokens.size()) + "\r\n";
        for (const auto &token : tokens)
        {
            result += "$" + to_string(token.size()) + "\r\n" + token + "\r\n";
        }

        return result;
    }

    /**
     * @brief Parses a RESP2-formatted string back to a normal string.
     *
     * @param resp The RESP2-formatted string.
     * @return The extracted string data.
     */
    vector<string> fromRESP2(const string &resp)
    {
        vector<string> result;
        istringstream stream(resp);
        string line;

        getline(stream, line, '\r');
        if (line[0] != '*')
            return {}; // Must start with '*'

        int numArgs = stoi(line.substr(1)); // Number of arguments
        stream.ignore(1);                   // Ignore '\n'

        for (int i = 0; i < numArgs; i++)
        {
            getline(stream, line, '\r');
            if (line[0] != '$')
                return {}; // Must start with '$'

            int len = stoi(line.substr(1)); // Get length of argument
            stream.ignore(1);               // Ignore '\n'

            string arg(len, ' ');
            stream.read(&arg[0], len); // Read the argument
            result.push_back(arg);

            stream.ignore(2); // Ignore '\r\n'
        }

        return result;
    }
};
