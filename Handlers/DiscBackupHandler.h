#pragma once
#include <bits/stdc++.h>
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
     * @brief Writes database contents to a temporary backup buffer file.
     *
     * @param map The database map to be backed up.
     * @return true if the backup was successful, false otherwise.
     */
    bool backupFromMapToBufferFile(unordered_map<string, string> map)
    {
        ofstream file;
        file.open("./backups/backup.txt", ios::app);
        if (!file.is_open())
        {
            return false;
        }
        for (auto it = map.begin(); it != map.end(); it++)
        {
            file << it->first << " " << it->second << endl;
        }
        file.close();
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
        ifstream backupBuffer;
        backupBuffer.open("./backups/backup.txt", ios::in);
        if (!backupBuffer.is_open())
        {
            return false;
        }

        string line;
        vector<string> lines;
        while (getline(backupBuffer, line))
        {
            lines.push_back(line);
        }
        backupBuffer.close();
        sort(lines.begin(), lines.end());

        for (int i = 0; i < 10; i++)
        {
            ofstream file;
            file.open("./backups/" + backupFiles[i] + ".txt", ios::out);
            if (!file.is_open())
            {
                return false;
            }
            for (int j = 0; j < lines.size(); j++)
            {
                if (utils.startsWith(lines[i], backupFiles[i]))
                {
                    file << lines[j] << endl;
                }
            }
            file.close();
        }

        // Clear the buffer file after committing backup
        ofstream clearBuffer;
        clearBuffer.open("./backups/backup.txt", ios::out);
        clearBuffer.close();
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
        system("mkdir -p ./backups");
        utils = Utils();
    }

    /**
     * @brief Initiates a backup of the given database map to a temporary file.
     *
     * @param map The database contents to be backed up.
     * @return true if backup was successful, false otherwise.
     */
    bool backup(unordered_map<string, string> map)
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
        system("rm -rf ./backups");
        return true;
    }

    /**
     * @brief Checks if a key exists in the disk backup and retrieves its value.
     *
     * Searches for the key in the appropriate backup file (`0.txt` to `9.txt`)
     * based on the first character of the key.
     *
     * @param key The key to search for.
     * @return string The value associated with the key if found, "-1" if not found, "-2" if an error occurs.
     */
    string checkBackupForKey(string key)
    {
        string filename = key.substr(0, 1) + ".txt";
        ifstream backupBuffer;
        backupBuffer.open("./backups/" + filename, ios::in);
        if (!backupBuffer.is_open())
        {
            return "-2";
        }

        string line;
        while (getline(backupBuffer, line))
        {
            if (utils.startsWith(line, key))
            {
                return line;
            }
        }
        return "-1";
    }
};
