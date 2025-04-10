#include "CommandWrapper.h"

#include <vector>
#include <string>

ns_Command* ns_CommandNew(const char *name, uint8_t minArgs, uint8_t maxArgs, ns_CommandCallback callback, const char *description, const char **_argsDescriptions) {
	std::vector<std::string> argsDescriptions{};
	for (uint16_t i = 0; i < static_cast<uint16_t>(maxArgs)*2; ++i)
		argsDescriptions.push_back(_argsDescriptions[i]);

	return new ns::Command(name, minArgs, maxArgs, callback, description, argsDescriptions);
}

void ns_CommandDelete(ns_Command *pCommand) {
	delete pCommand;
}

const char* ns_CommandGetArgumentsNames(ns_Command *pCommand) {
	return pCommand->getArgumentsNames().c_str();
}

void ns_CommandPrintAsDataTree(ns_Command *pCommand) {
	pCommand->printAsDataTree();
}


void ns_CommandSetName(ns_Command *pCommand, const char *name) {
	pCommand->name = name;
}

const char* ns_CommandGetName(ns_Command *pCommand) {
	return pCommand->name.c_str();
}


void ns_CommandSetMinArgs(ns_Command *pCommand, uint8_t minArgs) {
	pCommand->minArgs = minArgs;
}

uint8_t ns_CommandGetMinArgs(ns_Command *pCommand) {
	return pCommand->minArgs;
}


void ns_CommandSetMaxArgs(ns_Command *pCommand, uint8_t maxArgs) {
	pCommand->maxArgs = maxArgs;
}

uint8_t ns_CommandGetMaxArgs(ns_Command *pCommand) {
	return pCommand->maxArgs;
}


void ns_CommandSetCallback(ns_Command *pCommand, ns_CommandCallback callback) {
	pCommand->callback = callback;
}

ns_CommandCallback ns_CommandGetCallback(ns_Command* pCommand) {
	return pCommand->callback;
}


void ns_CommandSetDescription(ns_Command *pCommand, const char *description) {
	pCommand->description = description;
}

const char* ns_CommandGetDescription(ns_Command* pCommand) {
	return pCommand->description.c_str();
}


void ns_CommandSetArgsDescriptions(ns_Command *pCommand, const char *argsDescriptions[]) {
	for (uint16_t i = 0; i < static_cast<uint16_t>(pCommand->maxArgs)*2; ++i)
		pCommand->argsDescriptions[i] = argsDescriptions[i];
}

uint16_t ns_CommandGetArgsDescriptionsSize(ns_Command *pCommand) {
	return pCommand->argsDescriptions.size();
}

const char* ns_CommandGetArgDescription(ns_Command *pCommand, uint16_t index) {
	return pCommand->argsDescriptions[index].c_str();
}