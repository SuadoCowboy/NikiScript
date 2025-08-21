#include "CommandHandler.h"

#include "Context.h"

ns::Command* ns::CommandHandler::get(const std::string& name) {
	return commands.count(name) == 0? nullptr : &commands[name];
}

bool ns::CommandHandler::add(const Command& command) {
	if (commands.count(command.name) != 0)
		return false;

	commands[command.name] = command;

	return true;
}

void ns::CommandHandler::remove(CommandContext* pCtx, const std::string& name) {
	if (commands.count(name) == 0)
		return;

	for (size_t i = 0; i < pCtx->pCtx->toggleCommandsRunning.size(); ++i) {
		if (name == pCtx->pCtx->toggleCommandsRunning[i]->name) {
			pCtx->pCtx->toggleCommandsRunning.erase(pCtx->pCtx->toggleCommandsRunning.begin()+i);
			break;
		}
	}

	commands.erase(name);
}