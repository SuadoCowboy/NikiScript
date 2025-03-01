#include "SweatCI.h"

void sci::help_command(sci::SweatContext& ctx) {
    if (ctx.arguments.arguments.size() == 0)
        for (auto& command : ctx.commands.commands)
            command.second.printUsage();
    
    else {
        const std::string& commandName = ctx.arguments.getString();
        Command* pCommand = ctx.commands.get(commandName);

        if (pCommand == nullptr) {
            sci::printf(sci::PrintLevel::ERROR, "Command \"{}\" not found\n", commandName);
            return;
        }

        pCommand->printUsage();
    }
}

void sci::echo_command(sci::SweatContext& ctx) {
    sci::printf(sci::PrintLevel::ECHO, "{}\n", ctx.arguments.getString());
}

void sci::registerCommands(sci::SweatContext& ctx) {
    ctx.commands.add(Command("echo", 1, 1, echo_command, "prints the passed message to console", {"s[message]", "content to print to console"}));
    ctx.commands.add(Command("help", 0,1, help_command, "prints a list of commands with their usages or the usage of a specified command", {"s[command?]", "command to see usage"}));
}