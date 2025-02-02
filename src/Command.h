#pragma once

#include "Context.h"

namespace sci {
    typedef void(*CommandCallback)(CommandContext& ctx);

    struct Command {
        const char* name = nullptr;
        unsigned char minArgs = 0, maxArgs = 0;
        CommandCallback callback = nullptr;
        const char* description = nullptr;
        void* pData = nullptr;

        Command();
        Command(const char* name, unsigned char minArgs, unsigned char maxArgs, CommandCallback callback, const char* description, void* pData=nullptr);
    
        void call(CommandContext& ctx);
    };
}