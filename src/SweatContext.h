#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "Command.h"
#include "CommandHandler.h"
#include "PrintCallback.h"
#include "ProgramVariable.h"
#include "Lexer.h"

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
	struct SweatContext;

	struct Arguments {
		std::vector<std::string> arguments;
		uint8_t offset = 0;

		std::string& getString();

		template<typename T>
		T getInteger();
		template<typename T>
		T getUnsignedInteger();

		float getFloat();
		double getDouble();

		/**
		 * @brief clears arguments vector and sets offset to 0
		 */
		void clear();

#ifdef SWEATCI_ARGUMENTS_EXTRA
		SWEATCI_ARGUMENTS_EXTRA
#endif
	};

	// TODO: it's not safe to store those pointers below...
	// If it ever happen for SweatContext to be cloned, the references will
	// still be pointing to the old references. maybe make a copy function
	// for SweatContext so that this won't happen.

	typedef std::unordered_map<std::string, std::string> ConsoleVariables;
	// TODO: when deleting a variable, check if it's in here and then delete from this vector
	typedef std::vector<ConsoleVariables::pointer> LoopVariablesRunning;
	// TODO: when deleting a variable, check if it's in here and then delete from this vector
	typedef std::vector<ConsoleVariables::pointer> ToggleVariablesRunning;
	typedef std::vector<Command*> ToggleCommandsRunning;

	struct SweatContext {
		Lexer* pLexer = nullptr;

		Command* pCommand = nullptr;
		void* pData = nullptr; ///< command data

		Arguments arguments;

		ConsoleVariables consoleVariables;
		ProgramVariables programVariables;

		CommandHandler commands;

		LoopVariablesRunning loopVariablesRunning;
		ToggleVariablesRunning toggleVariablesRunning;
		ToggleCommandsRunning toggleCommandsRunning;
	};

	/**
	 * @brief Do not copy SweatContext without calling this function.
	 * LoopVariablesRunning and ToggleVariablesRunning stores pointers
	 * pointed to ConsoleVariables as well as toggleCommandsRunning whose
	 * pointers are from CommandHandler. That's why this function exists:
	 * It updates all those pointers.
	 * @param source object to copy content from
	 */
	SweatContext copySweatContext(const SweatContext& source);
}

template<typename T>
T sci::Arguments::getInteger() {
	return std::stoll(arguments[offset++]);
}

template <typename T>
T sci::Arguments::getUnsignedInteger() {
	return std::stoull(arguments[offset++]);
}