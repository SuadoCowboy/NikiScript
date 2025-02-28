#pragma once

#include <vector>
#include <string_view>

namespace sci {
    struct SweatContext;

    typedef void(*CommandCallback)(SweatContext& ctx);

    struct Command {
        std::string_view name;
        unsigned char minArgs = 0, maxArgs = 0;
        CommandCallback callback = nullptr;
        std::string_view usage;
        std::vector<std::string_view> argsDescriptions;
        //void* pData = nullptr;

        Command();
        Command(const std::string_view& name, unsigned char minArgs, unsigned char maxArgs, CommandCallback callback, const std::string_view& usage, const std::vector<std::string_view>& argsDescriptions/*, void* pData=nullptr*/);
    };
}