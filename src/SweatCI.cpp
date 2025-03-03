#include "SweatCI.h"

#include <sstream>

#include "Utils.h"

void sci::help_command(SweatContext& ctx) {
	if (ctx.arguments.arguments.size() == 0) {
		std::stringstream oss;
		for (auto& command : ctx.commands.commands)
			oss << command.second.name << ' ' << command.second.getArgumentsNames() << '\n';

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

	if (name.empty()) {
		sci::print(PrintLevel::ERROR, "Variable name can not be empty\n");
		return;
	}

	for (auto& c : name) {
		if (isspace(c)) {
			sci::print(PrintLevel::ERROR, "Variable name can not contain whitespace\n");
			return;
		}
	}

	if (ctx.programVariables.count(name) != 0) {
		sci::print(PrintLevel::ERROR, "A program variable already uses this name and therefore can not be replaced\n");
		return;
	}

	if (ctx.commands.commands.count(name) != 0) {
		sci::print(PrintLevel::ERROR, "A command already uses this name and therefore can not be replaced\n");
		return;
	}

	if (ctx.arguments.arguments.size() == 1)
		ctx.consoleVariables[name] = "";
	else
		ctx.consoleVariables[name] = ctx.arguments.getString();
}

void sci::delvar_command(SweatContext& ctx) {
	// TODO: when runningFrom is implemented: make this function unable to be called in variables? Why?
	const std::string& varName = ctx.arguments.getString();

	if (ctx.consoleVariables.count(varName) == 0) {
		sci::printf(PrintLevel::ERROR, "Expected console variable\n");
		return;
	}

	for (size_t i = 0; i < ctx.loopVariablesRunning.size(); ++i) {
		if (ctx.loopVariablesRunning[i]->first == varName) {
			ctx.loopVariablesRunning.erase(ctx.loopVariablesRunning.begin()+i);
			break;
		}
	}

	if (varName.size() > 1 && varName[0] == '+') {
		std::string toggleVarName = varName.substr(1);

		for (size_t i = 0; i < ctx.toggleVariablesRunning.size(); ++i) {
			if (ctx.toggleVariablesRunning[i]->first == toggleVarName) {
				ctx.toggleVariablesRunning.erase(ctx.toggleVariablesRunning.begin()+i);
				break;
			}
		}
	}

	ctx.consoleVariables.erase(varName);
}

void sci::math_command(SweatContext&) {
	// TODO: create math expression parser
	// TODO: +,-, *,^, /,%
	return;
}

void sci::toggle_command(sci::SweatContext& ctx) {
	const std::string& varName = ctx.arguments.getString();
	const std::string& option1 = ctx.arguments.getString();
	const std::string& option2 = ctx.arguments.getString();
	ctx.arguments.clear();

	if (ctx.consoleVariables.count(varName) != 0) {
		std::string& varValue = ctx.consoleVariables[varName];

		if (varValue == option1)
			varValue = option2;
		else
			varValue = option1;

		return;
	}

	sci::ProgramVariable& var = ctx.programVariables[varName];
	std::string varValue = var.get(&var);

	if (varValue == option1)
		var.set(&var, option2);
	else
		var.set(&var, option1);
}

void sci::registerCommands(sci::SweatContext& ctx) {
	ctx.commands.add(Command("_program_variable_callback", 0,1, ProgramVariable::callback, "gets/sets a variable", {"s[value?]", "new variable value"}));

	ctx.commands.add(Command("echo", 1, 1, echo_command, "prints the passed message to console", {"s[message]", "content to print to console"}));
	ctx.commands.add(Command("help", 0,1, help_command, "prints a list of commands with their usages or the usage of the specified command", {"s[command?]", "command to see usage"}));
	ctx.commands.add(Command("var", 1,2, var_command, "creates a variable", {"s[name]", "variable name", "s[value?]", "if value is not specified, variable becomes an empty string"}));
	ctx.commands.add(Command("delvar", 1,1, delvar_command, "deletes a variable", {"v[variable]", "variable to delete"}));
	// ctx.commands.add(Command("math", 1,2, math_command, "parses math expressions", {
	// 	"s[expression]", "expression",
	// 	"v[output?]", "variable to store expression result. If blank, prints to result to console"
	// }));
	ctx.commands.add(Command("toggle", 3,3, toggle_command, "toggles value between option1 and option2", {"v[variable]", "variable to modify value", "s[option1]", "option to set variable in case variable value is option2", "s[option2]", "option to set variable in case variable value is option1"}));
	// registerCommand("exec", 1, 1, exec, "- executes a .cfg file that contains SweatCI script", pVariables);
}

void sci::registerVariable(sci::SweatContext& ctx, const std::string& name, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set) {
	ctx.programVariables[name] = ProgramVariable(pVar, get, set);
}