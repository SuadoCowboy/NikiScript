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
        bool getInteger(SweatContext& ctx, T& output);

        bool getFloat(SweatContext& ctx, float& output);
        bool getDouble(SweatContext& ctx, double& output);

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
bool sci::Arguments::getInteger(SweatContext& ctx, T& output) {
    const std::string& str = arguments[offset++];

    try {
        output = (T)std::stoi(str);
        return true;
    } catch (...) {
        sci::printf(sci::PrintLevel::ERROR, "\"{}\" is not a valid integer. Argument #{}: {}\n", str, offset-1, ctx.pCommand->maxArgs == 1? ctx.pCommand->argsDescriptions[0] : ctx.pCommand->argsDescriptions[offset-1]);
        return false;
    }
}