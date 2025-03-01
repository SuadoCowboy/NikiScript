#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <locale>

void sci::ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void sci::rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void sci::trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}