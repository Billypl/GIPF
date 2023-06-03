#pragma once
#include <string>
#include <vector>
#include <sstream>
#define WHITESPACE " \n\r\t\f\v"

class Helper
{

public:

    static std::string ltrimString(const std::string& s)
    {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    static std::string rtrimString(const std::string& s)
    {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    static std::string trimString(const std::string& s) {
        return rtrimString(ltrimString(s));
    }

    static std::vector<std::string> explodeString(std::string s, char del)
    {
        std::stringstream ss(s);
        std::vector<std::string> result;
        std::string word;
        while (!ss.eof()) 
        {
            getline(ss, word, del);
            result.push_back(word);
        }
        return result;
    }

};