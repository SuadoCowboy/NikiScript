#pragma once

#include "DLLExport.h"
#include "Command.h"

typedef ns::CommandCallback ns_CommandCallback;
typedef ns::Command ns_Command;

extern "C" {
	NIKIAPI ns_Command* ns_CommandNew(const char* name, uint8_t minArgs, uint8_t maxArgs, ns_CommandCallback callback, const char* description, const char* argsDescriptions[]);
	NIKIAPI void ns_CommandDelete(ns_Command* pCommand);

	NIKIAPI const char* ns_CommandGetArgumentsNames(ns_Command* pCommand);

	NIKIAPI void ns_CommandPrintAsDataTree(ns_Command* pCommand);

	NIKIAPI void ns_CommandSetName(ns_Command* pCommand, const char* name);
	NIKIAPI const char* ns_CommandGetName(ns_Command* pCommand);

	NIKIAPI void ns_CommandSetMinArgs(ns_Command* pCommand, uint8_t minArgs);
	NIKIAPI uint8_t ns_CommandGetMinArgs(ns_Command* pCommand);

	NIKIAPI void ns_CommandSetMaxArgs(ns_Command* pCommand, uint8_t maxArgs);
	NIKIAPI uint8_t ns_CommandGetMaxArgs(ns_Command* pCommand);

	NIKIAPI void ns_CommandSetCallback(ns_Command* pCommand, ns_CommandCallback callback);
	NIKIAPI ns_CommandCallback ns_CommandGetCallback(ns_Command* pCommand);
	
	NIKIAPI void ns_CommandSetDescription(ns_Command* pCommand, const char* description);
	NIKIAPI const char* ns_CommandGetDescription(ns_Command* pCommand);

	NIKIAPI void ns_CommandSetArgsDescriptions(ns_Command* pCommand, const char* argsDescriptions[]);
	NIKIAPI const char* ns_CommandGetArgDescription(ns_Command* pCommand, uint16_t index);

	NIKIAPI uint16_t ns_CommandGetArgsDescriptionsSize(ns_Command* pCommand);
}