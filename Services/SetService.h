#pragma once
#include "../Database/BlinkDB.h"

using namespace std;

class SetService
{
    BlinkDB &blinkDB;

public:
    SetService(BlinkDB &blinkDB) : blinkDB(blinkDB) {}

    void set(string key, string value)
    {
        blinkDB.set(key, value);
    }
};