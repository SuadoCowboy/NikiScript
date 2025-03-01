#pragma once

#include "SweatContext.h"

namespace sci {
    /**
     * @brief is called in parse function when EOS or END is reached
     * @note ctx.pCommand should point to a valid Command object
     * @param ctx 
     * @see sci::parse
     */
    void handleCommandCall(SweatContext& ctx);

    /**
     * @brief Handles commands and variables as well as their arguments
     * 
     * @param ctx 
     */
    void parse(SweatContext& ctx);
}