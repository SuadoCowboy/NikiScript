#pragma once

#include <unordered_map>
#include <string_view>

#include "Command.h"

namespace sci {
    struct SweatContext;

    struct CommandHandler {
        std::unordered_map<std::string_view, Command> commands;

        Command* get(const std::string_view& name);
        /**
         * @brief adds command to commands unordered_map
         * 
         * @param command name of the will be mapped in the commands variable
         * @return true if command name is okay
         * @return false if command with this name already exists
         */
        bool add(const Command& command);

        static void call(SweatContext& ctx, Command& command);
    };
}