#include "Context.h"

std::string& ns::Arguments::getString() {
	return arguments[offset++];
}

float ns::Arguments::getFloat() {
	return std::stof(arguments[offset++]);
}

double ns::Arguments::getDouble() {
	return std::stod(arguments[offset++]);
}

void ns::Arguments::clear() {
	arguments.clear();
	offset = 0;
}

ns::Context ns::copyContext(const Context& source) {
	Context copy{source};

	for (size_t i = 0; i < copy.loopVariablesRunning.size(); ++i)
		copy.loopVariablesRunning[i] = &*copy.consoleVariables.find(copy.loopVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleVariablesRunning.size(); ++i)
		copy.toggleVariablesRunning[i] = &*copy.consoleVariables.find(copy.toggleVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleCommandsRunning.size(); ++i)
		copy.toggleCommandsRunning[i] = &copy.commands.commands.find(copy.toggleCommandsRunning[i]->name)->second;

	return copy;
}