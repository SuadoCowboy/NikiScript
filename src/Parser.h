#pragma once

#include "SweatContext.h"

#ifndef SWEATCI_LOOP_VARIABLE
#define SWEATCI_LOOP_VARIABLE '!'
#endif

#ifndef SWEATCI_TOGGLE_ON_VARIABLE
#define SWEATCI_TOGGLE_ON_VARIABLE '+'
#endif

#ifndef SWEATCI_TOGGLE_OFF_VARIABLE
#define SWEATCI_TOGGLE_OFF_VARIABLE '-'
#endif

namespace sci {
	/**
	 * @brief Clear anything related to the current lexer statement
	 * @note It does not go to the next EOS
	 */
	void clearStatementData(SweatContext& ctx);

	/**
	 * @brief Called in parse function when EOS or END is reached
	 * @note Sets ctx.pCommand to nullptr
	 * @note ctx.pCommand should point to a valid Command object
	 * @param ctx
	 * @see sci::parse
	 */
	void handleCommandCall(SweatContext& ctx);

	/**
	 * @brief Handles references in the string and checks if the parameter matches the argument type
	 * @note Possible argument types: s = string, i = integer, d = decimal, v = variable
	 */
	void handleArgumentToken(SweatContext& ctx);
	
	extern uint64_t maxConsoleVariableCalls; ///< 0 = unlimited

	/**
	 * @brief Called in parse function when an ConsoleVariable is passed as a IDENTIFIER
	 * @note I find this algorithm very interesting and fun(it creates a bunch of lexers for each variable call instead of recursive calling)
	 * @see sci::maxRecursiveCalls
	 * @param ctx
	 */
	void handleConsoleVariableCall(SweatContext& ctx);

	/**
	 * @brief if a loop variable is active, its script is ran here
	 * @param ctx
	 */
	void updateLoopVariables(sci::SweatContext& ctx);

	/**
	 * @brief Parses and interpret scripts: handles commands and variables as well as their arguments
	 * @param ctx
	 */
	void parse(SweatContext& ctx);
}