#include "CommandHandler.h"

#include "SweatContext.h"

sci::Command* sci::CommandHandler::get(const std::string_view& name) {
    return commands.count(name) == 0? nullptr : &commands[name];
}

void sci::CommandHandler::call(SweatContext& ctx, Command& command) {
    ctx.pCommand = &command;
    command.callback(ctx);
}