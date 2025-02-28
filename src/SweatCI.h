#pragma once

#include "SweatContext.h"

namespace sci {
    /**
     * @brief prints all the arguments passed to the console
     * @param message
     */
    void echo_command(sci::SweatContext& ctx);

    /**
     * @brief adds default commands such as echo
     * @param ctx 
     * @see echo_command
     */
    void registerCommands(sci::SweatContext& ctx);
}