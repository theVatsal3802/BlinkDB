#pragma once
#include <bits/stdc++.h>

using namespace std;

class Cache
{
private:
    unordered_map<string, string> cache;

public:
    void set(string key, string value)
    {
        cache[key] = value;
    }

    string get(string key)
    {
        if (cache.find(key) == cache.end())
        {
            return "-1";
        }
        return cache[key];
    }

    void del(string key)
    {
        cache.erase(key);
    }

    size_t getSize()
    {
        return cache.size();
    }

    void clear()
    {
        cache.clear();
    }
};