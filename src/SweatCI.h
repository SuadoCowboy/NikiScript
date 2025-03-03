#pragma once

#include "SweatContext.h"

namespace sci {
	/**
	 * @brief shows command usage
	 * @param command s[command?]
	 */
	void help_command(SweatContext& ctx);

	/**
	 * @brief prints all the arguments passed
	 * @param message s[message]
	 */
	void echo_command(SweatContext& ctx);

	void var_command(SweatContext& ctx);
	void delvar_command(SweatContext& ctx);

    void math_command(SweatContext& ctx);

	void toggle_command(SweatContext& ctx);

	/**
	 * @brief adds default commands such as echo
	 * @param ctx 
	 * @see echo_command
	 */
	void registerCommands(SweatContext& ctx);

	void registerVariable(SweatContext& ctx, const std::string& name, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
}