#pragma once
#include <string>

using namespace std;

/**
 * @brief Represents a user command in BlinkDB.
 *
 * This class encapsulates a database command with a command type, key, and optional value.
 */
class Command
{
private:
    /**
     * @brief The command type (e.g., "set", "get", "del").
     */
    string command;

    /**
     * @brief The key associated with the command.
     */
    string key;

    /**
     * @brief The value associated with the command (used for "set" operations).
     */
    string value;

public:
    /**
     * @brief Default constructor initializing empty command, key, and value.
     */
    Command()
    {
        command = "";
        key = "";
        value = "";
    }

    /**
     * @brief Constructs a Command with a specified command type, key, and value.
     *
     * @param command The command type (e.g., "set").
     * @param key The key to be used in the operation.
     * @param value The value to be set (only relevant for "set" commands).
     */
    Command(string command, string key, string value)
    {
        this->command = command;
        this->key = key;
        this->value = value;
    }

    /**
     * @brief Constructs a Command with a command type and key (for "get" and "del" commands).
     *
     * @param command The command type (e.g., "get", "del").
     * @param key The key associated with the operation.
     */
    Command(string command, string key)
    {
        this->command = command;
        this->key = key;
        this->value = "";
    }

    /**
     * @brief Retrieves the command type.
     *
     * @return The command type as a string.
     */
    string getCommand()
    {
        return command;
    }

    /**
     * @brief Retrieves the key associated with the command.
     *
     * @return The key as a string.
     */
    string getKey()
    {
        return key;
    }

    /**
     * @brief Retrieves the value associated with the command (only relevant for "set").
     *
     * @return The value as a string.
     */
    string getValue()
    {
        return value;
    }
};
