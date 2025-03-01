#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "Command.h"
#include "CommandHandler.h"
#include "PrintCallback.h"
#include "ProgramVariable.h"
#include "Lexer.h"

namespace sci {
    struct SweatContext;

    struct Arguments {
        std::vector<std::string> arguments;
        uint8_t offset = 0;

        std::string& getString();

        template<typename T>
        T getInteger();
        template<typename T>
        T getUnsignedInteger();

        float getFloat();
        double getDouble();

#ifdef SWEATCI_ARGUMENTS_EXTRA
        SWEATCI_ARGUMENTS_EXTRA
#endif
    };

    typedef std::unordered_map<std::string, std::string> ConsoleVariables;

    struct SweatContext {
        Lexer* pLexer = nullptr;

        Command* pCommand = nullptr;
        void* pData = nullptr; ///< command data

        Arguments arguments;

        ConsoleVariables consoleVariables;
        ProgramVariables programVariables;

        CommandHandler commands;
    };
}

template<typename T>
T sci::Arguments::getInteger() {
    return std::stoll(arguments[offset++]);
}

template <typename T>
T sci::Arguments::getUnsignedInteger() {
    return std::stoull(arguments[offset++]);
}