#pragma once
#include <bits/stdc++.h>

using namespace std;

class Response
{
    int statusCode;
    string message;
    pair<string, string> data;

public:
    Response()
    {
        statusCode = 0;
        message = "";
        data = pair<string, string>("Data", "");
    }

    Response(int statusCode, string message, pair<string, string> data)
    {
        this->statusCode = statusCode;
        this->message = message;
        this->data = data;
    }

    Response(int statusCode, string key, string value, string message)
    {
        this->statusCode = statusCode;
        this->message = message;
        this->data = pair<string, string>(key, value);
    }

    string to_string()
    {
        return "Status Code: " + std::to_string(statusCode) + ", Message: " + message + ", Data: " + data.second;
    }
};