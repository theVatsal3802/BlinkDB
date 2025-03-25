#pragma once
#include <bits/stdc++.h>
#include "../Handlers/DiscBackupHandler.h"

using namespace std;

/**
 * @brief The BlinkDB class implements an in-memory key-value database.
 *
 * This class manages data storage, retrieval, and deletion while also
 * handling periodic disk backups.
 */
class BlinkDB
{
private:
    /**
     * @brief The main in-memory database storing key-value pairs.
     */
    unordered_map<string, string> database;

    /**
     * @brief A temporary buffer for backup operations.
     */
    unordered_map<string, string> buffer;

    /**
     * @brief Handles backup operations to disk.
     */
    DiscBackupHandler discBackupHandler;

    /**
     * @brief Mutex to ensure thread-safe access to the database.
     */
    mutex dbMutex;

    /**
     * @brief Mutex to ensure thread-safe access to the backup buffer.
     */
    mutex bufferMutex;

    /**
     * @brief Thread responsible for periodic backup operations.
     */
    thread backupThread;

    /**
     * @brief Flag to indicate when to stop the backup worker thread.
     */
    bool stopBackup = false;

    /**
     * @brief Background worker function that handles periodic backups.
     *
     * Runs in a separate thread and backs up the database every 5 seconds
     * when the database size exceeds 100 million entries.
     */
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

    /**
     * @brief Performs the actual backup operation.
     *
     * Saves the database contents to disk and commits the backup.
     *
     * @return true if the backup is successful.
     */
    bool performBackup()
    {
        discBackupHandler.backup(database);
        discBackupHandler.commitBackup();
        return true;
    }

public:
    /**
     * @brief Constructs a BlinkDB instance.
     *
     * Initializes the in-memory database and starts the backup worker thread.
     */
    BlinkDB()
    {
        DiscBackupHandler discBackupHandler = DiscBackupHandler();
        database = unordered_map<string, string>();
        backupThread = thread(&BlinkDB::backupWorker, this);
        buffer = unordered_map<string, string>();
    }

    /**
     * @brief Destructor to clean up resources.
     *
     * Stops the backup worker thread before shutting down.
     */
    ~BlinkDB()
    {
        stopBackup = true;
        if (backupThread.joinable())
            backupThread.join();
    }

    /**
     * @brief Inserts or updates a key-value pair in the database.
     *
     * @param key The key to insert or update.
     * @param value The value to associate with the key.
     */
    void set(string key, string value)
    {
        database[key] = value;
    }

    /**
     * @brief Retrieves the value associated with a given key.
     *
     * If the key is not found in memory, it checks the disk backup.
     *
     * @param key The key to retrieve.
     * @return string The value associated with the key, or an empty string if not found.
     */
    string get(string key)
    {
        if (database.find(key) != database.end())
        {
            return database[key];
        }
        string result = discBackupHandler.checkBackupForKey(key);
        return result;
    }

    /**
     * @brief Deletes a key-value pair from the database.
     *
     * @param key The key to delete.
     */
    void del(string key)
    {
        database.erase(key);
    }
};
