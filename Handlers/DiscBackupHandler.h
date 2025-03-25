#pragma once
#include <bits/stdc++.h>
#include <mutex>
#include <filesystem>
#include "../Utils/Utils.h"

using namespace std;

/**
 * @brief The DiscBackupHandler class handles disk-based backups for BlinkDB.
 *
 * This class manages periodic backups of the database by writing data to files,
 * sorting and distributing data across multiple backup files, and retrieving
 * data from disk when necessary.
 */
class DiscBackupHandler
{
private:
    /**
     * @brief Array of backup file names (0-9).
     *
     * Used for categorizing backup data based on key prefixes.
     */
    string backupFiles[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    /**
     * @brief Utility functions for string operations.
     */
    Utils utils;

    /**
     * @brief Mutex for thread-safe file operations.
     */
    mutex backupMutex;

    /**
     * @brief Writes database contents to a temporary backup buffer file.
     *
     * @param map The database map to be backed up.
     * @return true if the backup was successful, false otherwise.
     */
    bool backupFromMapToBufferFile(const unordered_map<string, string> &map)
    {
        lock_guard<mutex> lock(backupMutex);
        ofstream file("./backups/backup.txt", ios::app);
        if (!file.is_open())
        {
            return false;
        }
        for (const auto &entry : map)
        {
            file << entry.first << " " << entry.second << endl;
        }
        return true;
    }

    /**
     * @brief Transfers buffered backup data to categorized disk files.
     *
     * Reads from `backup.txt`, sorts the entries, and distributes them into
     * files named `0.txt` to `9.txt` based on key prefixes.
     *
     * @return true if backup to disk was successful, false otherwise.
     */
    bool backupFromBufferFileToDisc()
    {
        lock_guard<mutex> lock(backupMutex);
        ifstream backupBuffer("./backups/backup.txt");
        if (!backupBuffer.is_open())
        {
            return false;
        }

        vector<string> lines;
        string line;
        while (getline(backupBuffer, line))
        {
            lines.push_back(line);
        }
        backupBuffer.close();

        sort(lines.begin(), lines.end());

        for (int i = 0; i < 10; i++)
        {
            ofstream file("./backups/" + backupFiles[i] + ".txt", ios::out);
            if (!file.is_open())
            {
                return false;
            }
            for (const string &entry : lines)
            {
                if (utils.startsWith(entry, backupFiles[i]))
                {
                    file << entry << endl;
                }
            }
        }

        // Clear buffer file
        ofstream clearBuffer("./backups/backup.txt", ios::out);
        return true;
    }

public:
    /**
     * @brief Constructs a DiscBackupHandler instance.
     *
     * Ensures the backup directory exists before performing any operations.
     */
    DiscBackupHandler()
    {
        std::filesystem::create_directories("./backups");
    }

    /**
     * @brief Initiates a backup of the given database map to a temporary file.
     *
     * @param map The database contents to be backed up.
     * @return true if backup was successful, false otherwise.
     */
    bool backup(const unordered_map<string, string> &map)
    {
        return backupFromMapToBufferFile(map);
    }

    /**
     * @brief Commits the buffered backup to permanent disk storage.
     *
     * Transfers data from the temporary buffer file (`backup.txt`) to categorized
     * backup files (`0.txt` to `9.txt`).
     *
     * @return true if the commit was successful, false otherwise.
     */
    bool commitBackup()
    {
        return backupFromBufferFileToDisc();
    }

    /**
     * @brief Deletes all backup files, effectively clearing the backup storage.
     *
     * @return true always.
     */
    bool terminate()
    {
        lock_guard<mutex> lock(backupMutex);
        std::filesystem::remove_all("./backups");
        return true;
    }

    /**
     * @brief Checks if a key exists in the disk backup and retrieves its value.
     *
     * Searches for the key in the appropriate backup file (`0.txt` to `9.txt`)
     * based on the first character of the key.
     *
     * @param key The key to search for.
     * @return The value associated with the key if found, "-1" if not found, "-2" if an error occurs.
     */
    string checkBackupForKey(const string &key)
    {
        lock_guard<mutex> lock(backupMutex);
        string filename = key.substr(0, 1) + ".txt";
        ifstream backupBuffer("./backups/" + filename);
        if (!backupBuffer.is_open())
        {
            return "-2";
        }

        string line, foundValue = "-1";
        while (getline(backupBuffer, line))
        {
            if (utils.startsWith(line, key))
            {
                size_t pos = line.find(" ");
                if (pos != string::npos)
                {
                    foundValue = line.substr(pos + 1);
                }
                break;
            }
        }
        return foundValue;
    }
};
