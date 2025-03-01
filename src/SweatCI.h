#pragma once

#include "SweatContext.h"

namespace sci {
	/**
	 * @brief shows command usage
	 * @param command s[command?]
	 */
	void help_command(sci::SweatContext& ctx);

	/**
	 * @brief prints all the arguments passed
	 * @param message s[message]
	 */
	void echo_command(sci::SweatContext& ctx);

	void var_command(sci::SweatContext& ctx);
	void delvar_command(sci::SweatContext& ctx);

    void math_command(sci::SweatContext& ctx);

	/**
	 * @brief adds default commands such as echo
	 * @param ctx 
	 * @see echo_command
	 */
	void registerCommands(sci::SweatContext& ctx);

	void registerVariable(sci::SweatContext& ctx, const std::string& name, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
}