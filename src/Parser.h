#pragma once

#include "Context.h"

#ifndef NIKISCRIPT_LOOP_VARIABLE
#define NIKISCRIPT_LOOP_VARIABLE '!'
#endif

#ifndef NIKISCRIPT_TOGGLE_ON
#define NIKISCRIPT_TOGGLE_ON '+'
#endif

#ifndef NIKISCRIPT_TOGGLE_OFF
#define NIKISCRIPT_TOGGLE_OFF '-'
#endif

namespace ns {
	/**
	 * @brief Clear anything related to the current lexer statement
	 * @note It does not go to the next EOS
	 */
	void clearStatementData(Context& ctx);

	/**
	 * @brief Called in parse function when EOS or END is reached
	 * @note Sets ctx.pCommand to nullptr
	 * @note ctx.pCommand should point to a valid Command object
	 * @param ctx
	 * @param pProgramVar if not null: sets/gets program variable
	 * @see ns::parse
	 */
	void handleCommandCall(Context& ctx, ProgramVariable*& pProgramVar);

	/**
	 * @brief Handles references in the string and checks if the parameter matches the argument type
	 * @note Possible argument types: s = string, i = integer, d = decimal, v = variable
	 */
	void handleArgumentToken(Context& ctx);
	
	extern uint64_t maxConsoleVariableCalls; ///< 0 = unlimited

	/**
	 * @brief Called in parse function when an ConsoleVariable is passed as a IDENTIFIER
	 * @note I find this algorithm very interesting and fun(it creates a bunch of lexers for each variable call instead of recursive calling)
	 * @see ns::maxRecursiveCalls
	 * @param ctx
	 */
	void handleConsoleVariableCall(Context& ctx, ProgramVariable*& pProgramVar);

	/**
	 * @brief if a loop variable is active, its script is ran here
	 * @param ctx
	 */
	void updateLoopVariables(ns::Context& ctx);

	/**
	 * @brief Parses and interpret scripts: handles commands and variables as well as their arguments
	 * @param ctx
	 */
	void parse(Context& ctx);
}