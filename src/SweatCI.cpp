#include "SweatCI.h"

void sci::echo_command(sci::SweatContext& ctx) {
    sci::print(sci::PrintLevel::ECHO, ctx.arguments.getString());
}

void sci::registerCommands(sci::SweatContext& ctx) {
    ctx.commands.add(Command("echo", 1, 1, echo_command, "prints the passed message to console", {"s[message]", "content to print to console"}));
}