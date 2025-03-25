#pragma once
#include <bits/stdc++.h>
#include "../Handlers/DiscBackupHandler.h"

using namespace std;

/**
 * @class BlinkDB
 * @brief Implements an in-memory key-value database with periodic disk backups.
 *
 * This class provides functionality to store, retrieve, and delete key-value pairs efficiently.
 * It also manages background backups to disk for data persistence.
 */
class BlinkDB
{
private:
    /**
     * @brief The main in-memory key-value store.
     */
    unordered_map<string, string> database;

    /**
     * @brief A temporary buffer used during backup operations.
     */
    unordered_map<string, string> buffer;

    /**
     * @brief Handles disk backup operations.
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
     * @brief Flag to control the backup worker thread's execution.
     */
    bool stopBackup = false;

    /**
     * @brief Background worker function that performs periodic backups.
     *
     * The function runs in a separate thread and triggers a backup every 5 seconds
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
     * @brief Executes the backup process.
     *
     * This function writes the database contents to disk and finalizes the backup.
     *
     * @return true if the backup was successful.
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
     * Initializes the in-memory database and starts the background backup thread.
     */
    BlinkDB()
    {
        DiscBackupHandler discBackupHandler = DiscBackupHandler();
        database = unordered_map<string, string>();
        backupThread = thread(&BlinkDB::backupWorker, this);
        buffer = unordered_map<string, string>();
    }

    /**
     * @brief Destructor that ensures cleanup of resources.
     *
     * Stops the background backup worker thread before shutting down.
     */
    ~BlinkDB()
    {
        stopBackup = true;
        if (backupThread.joinable())
            backupThread.join();
    }

    /**
     * @brief Stores a key-value pair in the database.
     *
     * If the key already exists, its value is updated.
     *
     * @param key The key to store.
     * @param value The associated value.
     */
    void set(string key, string value)
    {
        database[key] = value;
    }

    /**
     * @brief Retrieves the value associated with a key.
     *
     * First, it checks the in-memory database. If not found, it attempts to retrieve
     * the value from the disk backup.
     *
     * @param key The key to look up.
     * @return string The corresponding value if found, otherwise an empty string.
     */
    string get(string key)
    {
        if (database.find(key) != database.end())
        {
            return database[key];
        }
        return discBackupHandler.checkBackupForKey(key);
    }

    /**
     * @brief Deletes a key-value pair from the database.
     *
     * If the key exists, it is removed from the in-memory store.
     *
     * @param key The key to be deleted.
     */
    void del(string key)
    {
        database.erase(key);
    }
};
