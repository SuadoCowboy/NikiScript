#include "CommandHandlerWrapper.h"

#include "BaseWrapper.h"

#include "Context.h"

ns_CommandHandler* ns_CommandHandlerNew() {
	return new ns_CommandHandler();
}

void ns_CommandHandlerDelete(ns_CommandHandler *pCommandHandler) {
	delete pCommandHandler;
}

ns_Command* ns_CommandHandlerGet(ns_CommandHandler *pCommandHandler, const char *name) {
	auto it = pCommandHandler->commands.find(name);
	if (it == pCommandHandler->commands.end())
		return nullptr;
	else
		return &it->second;
}

ns_Command* ns_CommandHandlerAdd(ns_CommandHandler *pCommandHandler, const ns_Command *pCommand) {
	if (pCommandHandler->add(*pCommand))
		return &pCommandHandler->commands[pCommand->name];
	else
		return nullptr;
}

void ns_CommandHandlerErase(ns_CommandHandler *pCommandHandler, const char *name) {
	pCommandHandler->commands.erase(name);
}

void ns_CommandHandlerClear(ns_CommandHandler *pCommandHandler) {
	pCommandHandler->commands.clear();
}

const char** ns_CommandHandlerAllocKeys(ns_CommandHandler *pCommandHandler) {
	return ns::allocUnorderedMapStringKeysToCharArray(pCommandHandler->commands);
}

uint64_t ns_CommandHandlerSize(ns_CommandHandler *pCommandHandler) {
	return pCommandHandler->commands.size();
}