#include <bits/stdc++.h>

using namespace std;

class Utils
{
public:
    vector<string> split(string s, char delimter)
    {
        vector<string> tokens;
        stringstream ss(s);
        string word;

        while (getline(ss, word, delimter))
        {
            tokens.push_back(word);
        }
        return tokens;
    }

    string convertToLowercase(string s)
    {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    void terminate()
    {
        // TODO: Write Termination Logic Here
    }
};