#pragma once

#include <string>

#include "Context.h"

namespace ns {
	void help(Context& ctx, const std::string& name);

	/**
	 * @brief shows command usage
	 * @param ctx
	 * @note s[command?]
	 */
	void help_command(Context& ctx);

	/**
	 * @brief prints all the arguments passed
	 * @param ctx
	 * @note s[message]
	 */
	void echo_command(Context& ctx);

	/**
	 * @brief creates a variable
	 * @param ctx
	 * @param name
	 * @param value
	 * @return true on success
	 * @note if origin contains OriginType::REFERENCE then it will not print errors
	 */
	bool var(Context& ctx, const std::string& name, const std::string& value);

	/**
	 * @brief creates a variable
	 * @param ctx
	 * @note s[name] s[value]
	 * @see ns::var
	 */
	void var_command(Context& ctx);

	void delvar(Context& ctx, const std::string& name);

	/**
	 * @brief deletes a variable
	 * @param ctx
	 * @note v[consoleVariable]
	 * @see ns::delvar
	 */
	void delvar_command(Context& ctx);

	void incrementvar(Context& ctx, const std::string& name, float min, float max, float delta);

	/**
	 * @brief increments a variable value
	 * @param ctx
	 * @note v[variable] d[min] d[max] d[delta?]
	 * @see ns::incrementvar
	 */
	void incrementvar_command(Context& ctx);

	/**
	 * @brief toggles a variable value between option1 and option2
	 * @param ctx
	 * @param variable
	 * @param varName
	 * @param option1
	 * @param option2
	 */
	void toggle(Context& ctx, const std::string& varName, const std::string& option1, const std::string& option2);

	/**
	 * @brief toggles a variable value between option1 and option2
	 * @param ctx
	 * @note v[variable] s[option1] s[option2]
	 * @see ns::toggle
	 */
	void toggle_command(Context& ctx);

	/**
	 * @brief parses a file as nikiscript
	 * @param ctx
	 * @note s[filePath]
	 * @see ns::parseFile
	 */
	void exec_command(Context& ctx);

	/**
	 * @brief adds default commands such as echo
	 * @param ctx
	 * @see ns::echo_command
	 */
	void registerCommands(Context& ctx);

	/**
	 * @brief creates a variable and stores it in ns::Context::programVariables
	 * @param ctx
	 * @param name
	 * @param description
	 * @param pVar
	 * @param get
	 * @param set
	 */
	void registerVariable(Context& ctx, const std::string& name, const std::string& description, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
}