#pragma once

#include "CommandContext.h"

namespace sci {
    typedef void(*CommandCallback)(CommandContext& ctx);

    struct Command {
        const char* name = nullptr;
        unsigned char minArgs = 0, maxArgs = 0;
        CommandCallback callback = nullptr;
        char* usage = nullptr;
        char** argsDescriptions = nullptr;
        void* pData = nullptr;

        Command();
        Command(const char* name, unsigned char minArgs, unsigned char maxArgs, CommandCallback callback, char* usage, char** argsDescriptions, void* pData=nullptr);
    
        void call(CommandContext& ctx);
    };
}