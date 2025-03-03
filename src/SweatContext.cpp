#include "SweatContext.h"

#include <algorithm>

std::string& sci::Arguments::getString() {
	return arguments[offset++];
}

float sci::Arguments::getFloat() {
	return std::stof(arguments[offset++]);
}

double sci::Arguments::getDouble() {
	return std::stod(arguments[offset++]);
}

void sci::Arguments::clear() {
	arguments.clear();
	offset = 0;
}

sci::SweatContext sci::copySweatContext(const SweatContext& source) {
	SweatContext copy{source};

	for (size_t i = 0; i < copy.loopVariablesRunning.size(); ++i)
		copy.loopVariablesRunning[i] = &*copy.consoleVariables.find(copy.loopVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleVariablesRunning.size(); ++i)
		copy.toggleVariablesRunning[i] = &*copy.consoleVariables.find(copy.toggleVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleCommandsRunning.size(); ++i)
		copy.toggleCommandsRunning[i] = &copy.commands.commands.find(copy.toggleCommandsRunning[i]->name)->second;

	return copy;
}