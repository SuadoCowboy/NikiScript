#pragma once

#include <string>

#include "Context.h"

namespace ns {
	void help(Context* pCtx, const std::string& name);

	/**
	 * @brief shows command usage
	 * @param pCtx
	 * @note s[command?]
	 */
	void help_command(Context* pCtx);

	/**
	 * @brief prints all the arguments passed
	 * @param pCtx
	 * @note s[message]
	 */
	void echo_command(Context* pCtx);

	/**
	 * @brief creates a variable
	 * @param pCtx
	 * @param name
	 * @param value
	 * @return true on success
	 * @note if origin contains OriginType::REFERENCE then it will not print errors
	 */
	bool var(Context* pCtx, const std::string& name, const std::string& value);

	/**
	 * @brief creates a variable
	 * @param pCtx
	 * @note s[name] s[value]
	 * @see ns::var
	 */
	void var_command(Context* pCtx);

	void delvar(Context* pCtx, const std::string& name);

	/**
	 * @brief deletes a variable
	 * @param pCtx
	 * @note v[consoleVariable]
	 * @see ns::delvar
	 */
	void delvar_command(Context* pCtx);

	void incrementvar(Context* pCtx, const std::string& name, float min, float max, float delta);

	/**
	 * @brief increments a variable value
	 * @param pCtx
	 * @note v[variable] d[min] d[max] d[delta?]
	 * @see ns::incrementvar
	 */
	void incrementvar_command(Context* pCtx);

	/**
	 * @brief toggles a variable value between option1 and option2
	 * @param pCtx
	 * @param variable
	 * @param varName
	 * @param option1
	 * @param option2
	 */
	void toggle(Context* pCtx, const std::string& varName, const std::string& option1, const std::string& option2);

	/**
	 * @brief toggles a variable value between option1 and option2
	 * @param pCtx
	 * @note v[variable] s[option1] s[option2]
	 * @see ns::toggle
	 */
	void toggle_command(Context* pCtx);

	/**
	 * @brief parses a file as nikiscript
	 * @param pCtx
	 * @note s[filePath]
	 * @see ns::parseFile
	 */
	void exec_command(Context* pCtx);

	/**
	 * @brief adds default commands such as echo
	 * @param pCtx
	 * @see ns::echo_command
	 */
	void registerCommands(Context* pCtx);

	/**
	 * @brief creates a variable and stores it in ns::Context::programVariables
	 * @param pCtx
	 * @param name
	 * @param description
	 * @param pVar
	 * @param get
	 * @param set
	 */
	void registerVariable(Context* pCtx, const std::string& name, const std::string& description, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
}