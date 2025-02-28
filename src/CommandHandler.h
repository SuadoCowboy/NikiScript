#pragma once

#include <unordered_map>
#include <string_view>

#include "Command.h"

namespace sci {
    struct SweatContext;

    struct CommandHandler {
        std::unordered_map<std::string_view, Command> commands;

        Command* get(const std::string_view& name);

        static void call(SweatContext& ctx, Command& command);
    };
}