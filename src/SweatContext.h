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
	typedef std::vector<ConsoleVariables::pointer> LoopVariablesRunning;
	typedef std::vector<ConsoleVariables::pointer> ToggleVariablesRunning; ///< This is unecessary to be a pointer but I like the idea of using only 8 bytes instead of the same bytes as the var name
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