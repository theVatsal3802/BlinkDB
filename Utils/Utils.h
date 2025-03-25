#pragma once
#include <boost/container_hash/hash.hpp>
#include <bits/stdc++.h>

using namespace std;

class Utils
{
public:
    string hashKey(string key)
    {
        hash<string> hash_fn;
        size_t hash = hash_fn(key);
        return to_string(hash);
    }

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

    bool startsWith(string str, string prefix)
    {
        return str.rfind(prefix, 0) == 0;
    }
};