#pragma once
#include <bits/stdc++.h>
#include "../Database/BlinkDB.h"

using namespace std;

class GetService
{
    BlinkDB &blinkDB;

public:
    GetService(BlinkDB &blinkDB) : blinkDB(blinkDB) {}

    string get(string key)
    {
        return blinkDB.get(key);
    }
};