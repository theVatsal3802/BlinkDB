#include <bits/stdc++.h>

using namespace std;

class Data
{
private:
    string key;
    string value;
    long long int lastAccessTime;

public:
    Data(string key, string value)
    {
        this->key = key;
        this->value = value;
        this->lastAccessTime = 0;
    }

    bool updateLastAccessTime(long long int currentTime)
    {
        this->lastAccessTime = currentTime;
        return true;
    }
};