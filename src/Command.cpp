#include "Command.h"

#include <sstream>

#include "PrintCallback.h"

#ifdef NDEBUG
#define NIKISCRIPT_COMMAND_ASSERT(name, expression)
#else
#include <iostream>
#define NIKISCRIPT_COMMAND_ASSERT(name, expression) \
	if (!(expression)){std::cout << "Assertion failed on command \"" << name << "\": " # expression "\n"; exit(1);}
#endif

ns::Command::Command() {}

ns::Command::Command(const std::string& name, uint8_t minArgs, uint8_t maxArgs,
	CommandCallback callback, const std::string& description, const std::vector<std::string>& argsDescriptions)
	: name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), description(description), argsDescriptions(argsDescriptions) {
#ifndef NDEBUG
	NIKISCRIPT_COMMAND_ASSERT(name, !name.empty())
	NIKISCRIPT_COMMAND_ASSERT(name, minArgs <= maxArgs)
	NIKISCRIPT_COMMAND_ASSERT(name, argsDescriptions.size() % 2 == 0)
	NIKISCRIPT_COMMAND_ASSERT(name, argsDescriptions.size() / 2 == maxArgs);

	bool isName = true;
	for (uint8_t i = 0; i < argsDescriptions.size(); ++i) {
		if (isName) {
			const std::string& arg = argsDescriptions[i];

			NIKISCRIPT_COMMAND_ASSERT(name, arg.size() > 3);
			NIKISCRIPT_COMMAND_ASSERT(name, arg[0] == 's' || arg[0] == 'i' || arg[0] == 'd' || arg[0] == 'v');
			NIKISCRIPT_COMMAND_ASSERT(name, arg[1] == '[' && arg[arg.size()-1] == ']');
			if (i >= minArgs*2)
			NIKISCRIPT_COMMAND_ASSERT(name, arg[arg.size()-2] == '?');
		}

		isName = !isName;
	}

	NIKISCRIPT_COMMAND_ASSERT(name, callback != nullptr);
#endif
}

std::string ns::Command::getArgumentsNames() {
	if (argsDescriptions.size() == 0)
		return "";

	std::stringstream oss{};
	bool isName = true;

	for (size_t i = 0; i < argsDescriptions.size(); ++i) {
		if (isName)
			oss << argsDescriptions[i] << ' ';

		isName = !isName;
	}

	std::string out = oss.str();
	out.erase(out.size()-1);

	return out;
}

void ns::Command::printAsDataTree() {
	std::stringstream descriptions{};
	std::stringstream usage{};
	bool isName = true;

	usage << "- Usage: " << name << ' ';
	descriptions << "- Description: " << description << "\n- Arguments:\n";
	for (size_t i = 0; i < argsDescriptions.size(); ++i) {
		if (isName) {
			descriptions << "- - " << argsDescriptions[i] << ": ";
			usage << argsDescriptions[i] << ' ';

		} else
			descriptions << argsDescriptions[i] << '\n';

		isName = !isName;
	}
	usage << '\n' << descriptions.str();

	ns::print(PrintLevel::ECHO, usage.str().c_str());
}