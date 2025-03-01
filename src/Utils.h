#pragma once

#include <stdint.h>
#include <string>
#include <sstream>

namespace sci {
    /// @warning this function is not meant to be used outside this header
    template<typename ...>
    static void _formatStringValue(const std::string& format, std::stringstream& buf) {
        buf << format;
    }

    /// @warning this function is not meant to be used outside this header
    template<typename T, typename ... Args>
    static void _formatStringValue(const std::string& format, std::stringstream& buf, T value, Args& ... args) {
        uint64_t idx = format.find("{}");
        if (idx == std::string::npos) {
            buf << format;
            return;
        }

        buf << format.substr(0, idx) << value;
        
        _formatStringValue(format.substr(idx+2), buf, args...);
    }

    template<typename ... Args>
    std::string formatString(const std::string& format, Args ... args) {
        std::stringstream buf;
        _formatStringValue(format, buf, args...);
        return buf.str();
    }

    template<typename T>
    std::string numberToString(T value) {
        std::string str = std::to_string(value);
        std::stringstream out;

        for (uint64_t i = 0; i < str.size(); ++i) {
            if (str[i] != '.') {
                out << str[i];
                continue;
            }
            
            while (str.back() == '0') {
                str.pop_back();
                
                if (str.back() == '.') {
                    str.pop_back();
                    break;
                }
            }
        }

        return out.str();
    }

    /**
     * @brief trim from start (in place)
     * 
     * @param s string to trim the end
     */
    void ltrim(std::string &s);
    
    /**
     * @brief trim from end (in place)
     * 
     * @param s string to trim the end
     */
    void rtrim(std::string &s);
    
    /**
     * @brief trim from start and end
     * 
     * @param s string to trim
     * @see sci::trim
     * @see sci::rtrim
     */
    void trim(std::string& s);
}