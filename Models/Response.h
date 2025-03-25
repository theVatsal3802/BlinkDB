#pragma once
#include <bits/stdc++.h>

using namespace std;

/**
 * @brief Represents an API response in BlinkDB.
 *
 * This class encapsulates the response status, message, and optional key-value data.
 */
class Response
{
private:
    /**
     * @brief The HTTP-like status code for the response (e.g., 200 for success, 404 for not found).
     */
    int statusCode;

    /**
     * @brief A descriptive message about the response.
     */
    string message;

    /**
     * @brief The key-value pair representing data (used for responses containing retrieved data).
     */
    pair<string, string> data;

public:
    /**
     * @brief Default constructor initializing an empty response.
     */
    Response()
    {
        statusCode = 0;
        message = "";
        data = pair<string, string>("Data", "");
    }

    /**
     * @brief Constructs a Response with a status code, message, and data.
     *
     * @param statusCode The status code of the response.
     * @param message The response message.
     * @param data The key-value pair representing data.
     */
    Response(int statusCode, string message, pair<string, string> data)
    {
        this->statusCode = statusCode;
        this->message = message;
        this->data = data;
    }

    /**
     * @brief Constructs a Response with a status code, key, value, and message.
     *
     * @param statusCode The status code of the response.
     * @param key The key associated with the data.
     * @param value The value associated with the key.
     * @param message The response message.
     */
    Response(int statusCode, string key, string value, string message)
    {
        this->statusCode = statusCode;
        this->message = message;
        this->data = pair<string, string>(key, value);
    }

    /**
     * @brief Converts the response to a string representation.
     *
     * @return A formatted string containing the status code, message, and data.
     */
    string to_string()
    {
        return "Status Code: " + std::to_string(statusCode) + ", Message: " + message + ", Data: " + data.second;
    }
};
