#pragma once

#include "SweatContext.h"

namespace sci {
    /**
     * @brief clear anything related to the current lexer statement
     * @note it does not go to the next EOS
     */
    void clearStatementData(SweatContext& ctx);

    /**
     * @brief called in parse function when EOS or END is reached
     * @note sets ctx.pCommand to nullptr
     * @note ctx.pCommand should point to a valid Command object
     * @param ctx 
     * @see sci::parse
     */
    void handleCommandCall(SweatContext& ctx);

    /**
     * @brief handles references in the string and checks if the parameter matches the type
     * 
     */
    void handleArgumentToken(SweatContext& ctx);
    
    extern uint64_t maxConsoleVariableCalls; ///< 0 = unlimited

    /**
     * @brief called in parse function when an ConsoleVariable is passed as a IDENTIFIER
     * @note I find this algorithm very interesting and fun(it creates a bunch of lexers for each variable call instead of recursive calling)
     * @see sci::maxRecursiveCalls
     * @param ctx 
     */
    void handleConsoleVariableCall(SweatContext& ctx);

    /**
     * @brief Parses and interpret scripts: handles commands and variables as well as their arguments
     * 
     * @param ctx 
     */
    void parse(SweatContext& ctx);
}