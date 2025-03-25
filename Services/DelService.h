#pragma once
#include <bits/stdc++.h>
#include "../Database/BlinkDB.h"

using namespace std;

class DelService
{
private:
    BlinkDB &blinkDB;

public:
    DelService(BlinkDB &blinkDB) : blinkDB(blinkDB) {}

    void del(string key)
    {
        blinkDB.del(key);
    }
};