#pragma once
#include <bits/stdc++.h>
#include "../Handlers/DiscBackupHandler.h"

using namespace std;

class BlinkDB
{
private:
    unordered_map<string, string> database;
    unordered_map<string, string> buffer;
    DiscBackupHandler discBackupHandler;
    mutex dbMutex, bufferMutex;
    thread backupThread;
    bool stopBackup = false;

    void backupWorker()
    {
        while (!stopBackup)
        {
            this_thread::sleep_for(chrono::seconds(5));
            if (database.size() > 100000000)
            {
                dbMutex.lock();
                buffer = database;
                dbMutex.unlock();

                bufferMutex.lock();
                performBackup();
                buffer.clear();
                bufferMutex.unlock();
            }
        }
    }

    bool performBackup()
    {
        discBackupHandler.backup(database);
        discBackupHandler.commitBackup();
        return true;
    }

public:
    BlinkDB()
    {
        DiscBackupHandler discBackupHandler = DiscBackupHandler();
        database = unordered_map<string, string>();
        backupThread = thread(&BlinkDB::backupWorker, this);
        buffer = unordered_map<string, string>();
    }

    ~BlinkDB()
    {
        stopBackup = true;
        if (backupThread.joinable())
            backupThread.join();
    }

    void set(string key, string value)
    {
        database[key] = value;
    }

    string get(string key)
    {
        if (database.find(key) != database.end())
        {
            return database[key];
        }
        string result = discBackupHandler.checkBackupForKey(key);
        return result;
    }

    void del(string key)
    {
        database.erase(key);
    }
};