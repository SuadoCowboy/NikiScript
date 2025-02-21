#pragma once

#include <stdint.h>
#include <string>

#include "Utils.h"

namespace sci {
    enum PrintLevel : uint8_t {
        DEFAULT = 0, // text that is not involved by user interaction
        ECHO, // any text that came from a command that is not an error
        WARNING,
        // _ERROR needs a underscore because when including windows.h, it identifies as a macro
        _ERROR, // anything that went wrong
    };

    std::string printLevelToString(PrintLevel level);

    typedef void(*PrintCallback)(void* pData, PrintLevel level, const std::string& message);

    extern PrintCallback printCallback;
    extern void* pPrintCallbackData;

    template<typename... Args>
    static void printf(const PrintLevel& level, const std::string& format, Args ... args) {
        print(level, formatString(format, args...));
    }

    extern PrintCallback printCallback;
    extern void* pPrintCallbackData;

    void setPrintCallback(void* pData, PrintCallback callback);
    void print(const PrintLevel& level, const std::string& str);
    void printUnknownCommand(const std::string& command);
}