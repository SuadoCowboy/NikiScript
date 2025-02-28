#pragma once

#include <vector>
#include <stdint.h>
#include <string>

#include "Command.h"
#include "CommandHandler.h"
#include "PrintCallback.h"

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

    struct SweatContext {
        //void* pData = nullptr;
        Command* pCommand = nullptr;
        Arguments arguments;

        CommandHandler commands;
    };
}

template<typename T>
bool sci::Arguments::getInteger(SweatContext& ctx, T& output) {
    const std::string& str = arguments[offset++];

    try {
        output = (uint8_t)std::stoi(str);
        return true;
    } catch (...) {
        sci::printf(sci::_ERROR, "\"{}\" is not a valid integer. Argument #{}: {}\n", str, offset-1, ctx.pCommand->maxArgs == 1? ctx.pCommand->argsDescriptions[0] : ctx.pCommand->argsDescriptions[offset-1]);
        return false;
    }
}