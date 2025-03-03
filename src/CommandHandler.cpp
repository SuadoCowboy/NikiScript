#include "CommandHandler.h"

#include "SweatContext.h"

sci::Command* sci::CommandHandler::get(const std::string_view& name) {
	return commands.count(name) == 0? nullptr : &commands[name];
}

bool sci::CommandHandler::add(const Command& command) {
	if (commands.count(command.name) != 0)
		return false;

	commands[command.name] = command;

	return true;
}

void sci::CommandHandler::remove(const std::string_view& name, SweatContext& ctx) {
	for (size_t i = 0; i < ctx.toggleCommandsRunning.size(); ++i) {
		if (name == ctx.toggleCommandsRunning[i]->name) {
			ctx.toggleCommandsRunning.erase(ctx.toggleCommandsRunning.begin()+i);
			break;
		}
	}

	commands.erase(name);
}