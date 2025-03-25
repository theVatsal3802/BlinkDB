#pragma once
#include <bits/stdc++.h>
#include "../Utils/Utils.h"

using namespace std;

class DiscBackupHandler
{
private:
    string backupFiles[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    Utils utils;

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
        ofstream clearBuffer;
        clearBuffer.open("./backups/backup.txt", ios::out);
        clearBuffer.close();
        return true;
    }

public:
    DiscBackupHandler()
    {
        system("mkdir -p ./backups");
        utils = Utils();
    }

    bool backup(unordered_map<string, string> map)
    {
        return backupFromMapToBufferFile(map);
    }

    bool commitBackup()
    {
        return backupFromBufferFileToDisc();
    }

    bool terminate()
    {
        system("rm -rf ./backups");
        return true;
    }

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