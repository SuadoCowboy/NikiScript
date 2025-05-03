#include "NikiScript.h"

#include <sstream>

#include "Utils.h"
#include "Parser.h"
#include "Lexer.h"

void ns::help(Context& ctx, const std::string& name) {
	if (name.empty()) {
		std::stringstream oss{};
		for (auto& command : ctx.commands.commands)
			oss << command.second.name << ' ' << command.second.getArgumentsNames() << '\n';

		ns::print(ns::ECHO, oss.str().c_str());

	} else {
		Command* pCommand = ctx.commands.get(name);
		if (pCommand == nullptr) {
			ns::printf(ns::ERROR, "Command \"{}\" not found\n", name);
			return;
		}

		pCommand->printAsDataTree();
	}
}

void ns::help_command(Context& ctx) {
	help(ctx, ctx.args.arguments.size() > 0? ctx.args.getString(0) : "");
}

void ns::echo_command(Context& ctx) {
	ns::printf(ns::ECHO, "{}\n", ctx.args.getString(0));
}

bool ns::var(Context& ctx, const std::string& name, const std::string& value) {
	if (name.empty()) {
		ns::print(PrintLevel::ERROR, "Variable name can not be empty\n");
		return false;
	}

	if (isdigit(name[0])) {
		ns::print(PrintLevel::ERROR, "Variable name can not contain digit in the beginning\n");
		return  false;
	}

	for (size_t i = 0; i < name.size(); ++i) {
		if (isspace(name[i])) {
			ns::print(PrintLevel::ERROR, "Variable name can not contain whitespace\n");
			return  false;
		}

		switch (name[i]) {
		case NIKISCRIPT_LOOP_VARIABLE:
			if (i == 0 && name.size() > 1)
				break;
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKISCRIPT_LOOP_VARIABLE);
			return false;

		case NIKISCRIPT_TOGGLE_ON:
			if (i == 0 && name.size() > 1)
				break;
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKISCRIPT_TOGGLE_ON);
			return false;

		case NIKISCRIPT_TOGGLE_OFF:
			if (i == 0 && name.size() > 1)
				break;
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKISCRIPT_TOGGLE_OFF);
			return false;

		case NIKISCRIPT_REFERENCE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_REFERENCE);
			return false;

		case NIKISCRIPT_REFERENCE_OPEN:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_REFERENCE_OPEN);
			return false;

		case NIKISCRIPT_REFERENCE_CLOSE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_REFERENCE_CLOSE);
			return false;

		case NIKISCRIPT_ARGUMENTS_SEPARATOR:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_ARGUMENTS_SEPARATOR);
			return false;

		case NIKISCRIPT_ARGUMENTS_CLOSE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_ARGUMENTS_CLOSE);
			return false;

		case NIKISCRIPT_ARGUMENTS_OPEN:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_ARGUMENTS_OPEN);
			return false;

		case NIKISCRIPT_STATEMENT_SEPARATOR:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_STATEMENT_SEPARATOR);
			return false;

		case NIKISCRIPT_COMMENT_LINE:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_COMMENT_LINE);
			return false;

		case NIKISCRIPT_COMMENT_LINES:
			ns::printf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKISCRIPT_COMMENT_LINES);
			return false;
		}
	}

	if (ctx.programVariables.count(name) != 0) {
		ns::print(PrintLevel::ERROR, "A program variable already uses this name and therefore can not be replaced\n");
		return false;
	}

	if (ctx.commands.commands.count(name) != 0) {
		ns::print(PrintLevel::ERROR, "A command already uses this name and therefore can not be replaced\n");
		return false;
	}

	ctx.consoleVariables[name] = value;
	return true;
}

void ns::var_command(Context& ctx) {
	bool result = var(ctx, ctx.args.getString(0), ctx.args.arguments.size() > 1? ctx.args.getString(1) : "");
	if (ctx.origin & OriginType::REFERENCE)
		ns::printf(ns::ECHO, "{}\n", result);
}

void ns::delvar(Context& ctx, const std::string& name) {
	if (ctx.consoleVariables.count(name) == 0) {
		ns::printf(PrintLevel::ERROR, "Expected console variable\n");
		return;
	}

	for (size_t i = 0; i < ctx.loopVariablesRunning.size(); ++i) {
		if (ctx.loopVariablesRunning[i]->first == name) {
			ctx.loopVariablesRunning.erase(ctx.loopVariablesRunning.begin()+i);
			break;
		}
	}

	if (name.size() > 1 && name[0] == '+') {
		std::string toggleVarName = name.substr(1);

		for (size_t i = 0; i < ctx.toggleVariablesRunning.size(); ++i) {
			if (ctx.toggleVariablesRunning[i]->first == toggleVarName) {
				ctx.toggleVariablesRunning.erase(ctx.toggleVariablesRunning.begin()+i);
				break;
			}
		}
	}

	ctx.consoleVariables.erase(name);
}

void ns::delvar_command(Context& ctx) {
	delvar(ctx, ctx.args.getString(0));
}

void ns::incrementvar(Context& ctx, const std::string& name, float min, float max, float delta) {
	if (min > max) {
		ns::printf(ns::ERROR, "max({}) should be higher than min({})\n", max, min);
		return;
	}

	float value = 0.0;
	if (ctx.consoleVariables.count(name) != 0) {
		try {
			value = std::stof(ctx.consoleVariables[name]);
		} catch (...) {
			ns::printf(ns::ERROR, "\"{}\" is not a number\n", ctx.consoleVariables[name]);
			return;
		}
	} else {
		try {
			value = std::stof(ctx.programVariables[name].get(ctx, &ctx.programVariables[name]));
		} catch (...) {
			ns::printf(ns::ERROR, "\"{}\" is not a number\n", ctx.consoleVariables[name]);
			return;
		}
	}

	value += delta;
	if (value > max)
		value = min;

	if (value < min)
		value = min;

	if (ctx.consoleVariables.count(name) != 0) {
		ctx.consoleVariables[name] = std::to_string(value);
	} else
		ctx.programVariables[name].set(ctx, &ctx.programVariables[name], std::to_string(value));
}

void ns::incrementvar_command(Context& ctx) {
	incrementvar(ctx, ctx.args.getString(0), ctx.args.getFloat(1), ctx.args.getFloat(2), ctx.args.arguments.size() > 3? ctx.args.getFloat(3) : 1.0f);
}

void ns::toggle(Context& ctx, const std::string& varName, const std::string& option1, const std::string& option2) {
	if (ctx.consoleVariables.count(varName) != 0) { // Console Variable
		std::string& varValue = ctx.consoleVariables[varName];

		if (varValue == option1)
			varValue = option2;
		else
			varValue = option1;

	} else if (ctx.programVariables.count(varName) != 0) { // Program Variable
		ns::ProgramVariable& var = ctx.programVariables[varName];
		std::string varValue = var.get(ctx, &var);

		if (varValue == option1)
			var.set(ctx, &var, option2);
		else
			var.set(ctx, &var, option1);

	} else if (ns::Command *pCommand = ctx.commands.get(varName)) { // Command
		void* pOriginalPrintCallbackData = pPrintCallbackData;
		PrintCallback originalPrintCallback = printCallback;

		std::string varValue;
		setPrintCallback(&varValue, printAppendToStringEchoOnly);
		
		parseInsideAnotherScript(ctx, pCommand->name.c_str());
		setPrintCallback(pOriginalPrintCallbackData, originalPrintCallback);

		if (!varValue.empty() && varValue[varValue.size()] == '\n')
			varValue.pop_back();

		if (varValue == option1)
			varValue = option2;
		else
			varValue = option1;

		// This could be unsecure depending on what nikiscript is being used, because if you set one of the options with something like "); destroy_humanity\\\\", it will call destroy_humanity!
		varValue = formatString("{}({})", pCommand->name, varValue.c_str());
		parseInsideAnotherScript(ctx, varValue.c_str());
	} else
		ns::print(PrintLevel::ERROR, "toggle command expected a variable or command\n");
}

void ns::toggle_command(ns::Context& ctx) {
	ns::toggle(ctx, ctx.args.getString(0), ctx.args.getString(1), ctx.args.getString(2));
}

void ns::exec_command(Context& ctx) {
	parseFile(ctx, ctx.args.getString(0).c_str(), true);
}

void ns::registerCommands(ns::Context& ctx) {
	nsRegisterCommand(ctx, "echo", 1, 1, echo_command, "prints the passed message to console", "s[message]", "content to print to console");
	nsRegisterCommand(ctx, "help", 0,1, help_command, "prints a list of commands with their usages or the usage of the specified command", "s[command?]", "command to see usage");
	nsRegisterCommand(ctx, "var", 1,2, var_command, "creates a variable", "s[name]", "variable name", "s[value?]", "if value is not specified, variable becomes an empty string");
	nsRegisterCommand(ctx, "delvar", 1,1, delvar_command, "deletes a variable", "v[variable]", "variable to delete");
	nsRegisterCommand(ctx, "toggle", 3,3, toggle_command, "toggles value between option1 and option2", "s[variableOrCommand]", "variable or command to modify its value", "s[option1]", "option to set variable in case variable value is option2", "s[option2]", "option to set variable in case variable value is option1");
	nsRegisterCommand(ctx, "exec", 1,1, exec_command, "parses a script file", "s[filePath]", "path to the file");
	nsRegisterCommand(ctx, "incrementvar", 3,4, incrementvar_command, "do value + delta, when value > max: value = min", "v[variable]", "variable to modify value", "d[min]", "minimum value possible", "d[max]", "maximum possible value", "d[delta?]", "to increase value with -> value + delta");
}

void ns::registerVariable(ns::Context& ctx, const std::string& name, const std::string& description, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set) {
	ctx.programVariables[name] = ProgramVariable(pVar, description, get, set);
}