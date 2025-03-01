#include "SweatCI.h"

#include <sstream>

#include "Utils.h"

void sci::help_command(SweatContext& ctx) {
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

void sci::echo_command(SweatContext& ctx) {
	sci::printf(sci::PrintLevel::ECHO, "{}\n", ctx.arguments.getString());
}

void sci::var_command(SweatContext& ctx) {
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

void sci::delvar_command(SweatContext& ctx) {
	// TODO: when runningFrom is implemented: make this function unable to be called in aliases? Why?
	const std::string& varName = ctx.arguments.getString();

	if (ctx.consoleVariables.count(varName) == 0) {
		sci::printf(PrintLevel::ERROR, "Expected console variable\n");
		return;
	}

	ctx.consoleVariables.erase(varName);
}

void sci::math_command(sci::SweatContext&) {
	// TODO: create math expression parser
	// TODO: +,-, *,^, /,%
}

void sci::registerCommands(sci::SweatContext& ctx) {
	ctx.commands.add(Command("_program_variable_callback", 0,1, ProgramVariable::callback, "gets/sets a variable", {"s[value?]", "new variable value"}));

	ctx.commands.add(Command("echo", 1, 1, echo_command, "prints the passed message to console", {"s[message]", "content to print to console"}));
	ctx.commands.add(Command("help", 0,1, help_command, "prints a list of commands with their usages or the usage of a specified command", {"s[command?]", "command to see usage"}));
	ctx.commands.add(Command("var", 1,2, var_command, "creates a variable", {"s[name]", "variable name", "s[value?]", "if value is not specified, variable becomes an empty string"}));
	ctx.commands.add(Command("delvar", 1,1, delvar_command, "deletes a variable", {"v[variable]", "variable to delete"}));
	ctx.commands.add(Command("math", 1,2, math_command, "parses math expressions", {
		"s[expression]", "expression",
		"v[output?]", "variable to store expression result. If blank, prints to result to console"
	}));

	// registerCommand("help", 0, 1, help, "<command> - shows the usage of the command specified");
	// registerCommand("commands", 0, 0, commands, "- shows a list of commands with their usages");
	// registerCommand("echo", 1, 1, echo, "<message> - echoes a message to the console");
	// registerCommand("incrementvar", 4, 4, incrementvar, "<var|cvar> <minValue> <maxValue> <delta> - increments the value of a variable", pVariables);
	// registerCommand("exec", 1, 1, exec, "- executes a .cfg file that contains SweatCI script", pVariables);
	// registerCommand("toggle", 3, 3, toggle, "<var|cvar> <option1> <option2> - toggles value between option1 and option2", pVariables);
}

void sci::registerVariable(sci::SweatContext& ctx, const std::string& name, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set) {
	ctx.programVariables[name] = ProgramVariable(pVar, get, set);
}