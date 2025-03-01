#include "SweatCI.h"

#include <sstream>

#include "Utils.h"

void sci::help_command(sci::SweatContext& ctx) {
    if (ctx.arguments.arguments.size() == 0) {
        std::stringstream oss;
        for (auto& command : ctx.commands.commands)
            oss << command.second.getUsage() << '\n';

        sci::print(sci::PrintLevel::ECHO, oss.str());
    } else {
        std::string& commandName = ctx.arguments.getString();
        trim(commandName);

        Command* pCommand = ctx.commands.get(commandName);

        if (pCommand == nullptr) {
            sci::printf(sci::PrintLevel::ERROR, "Command \"{}\" not found\n", commandName);
            return;
        }

        pCommand->printAsDataTree();
    }
}

void sci::echo_command(sci::SweatContext& ctx) {
    sci::printf(sci::PrintLevel::ECHO, "{}\n", ctx.arguments.getString());
}

void sci::var_command(sci::SweatContext& ctx) {
    std::string& name = ctx.arguments.getString();

    for (auto& c : name) {
        if (isspace(c)) {
            sci::print(PrintLevel::ERROR, "Variable name can not contain whitespace\n");
            return;
        }
    }

    if (name.empty()) {
        sci::print(PrintLevel::ERROR, "Variable name can not be empty\n");
        return;
    }

    if (ctx.programVariables.count(name) != 0) {
        sci::print(PrintLevel::ERROR, "A program variable already uses this name and therefore can not be replaced\n");
        return;
    }

    if (ctx.arguments.arguments.size() == 1)
        ctx.consoleVariables[name] = "";
    else
        ctx.consoleVariables[name] = ctx.arguments.getString();
}

void sci::registerCommands(sci::SweatContext& ctx) {
    ctx.commands.add(Command("echo", 1, 1, echo_command, "prints the passed message to console", {"s[message]", "content to print to console"}));
    ctx.commands.add(Command("help", 0,1, help_command, "prints a list of commands with their usages or the usage of a specified command", {"s[command?]", "command to see usage"}));
    ctx.commands.add(Command("var", 1,2, var_command, "creates a variable", {"s[name]", "variable name", "s[value?]", "if value is not specified, variable becomes an empty string"}));
}