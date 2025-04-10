#pragma once

#include "DLLExport.h"
#include "CommandHandler.h"

#include "CommandWrapper.h"

typedef ns::CommandHandler ns_CommandHandler;

extern "C" {
	NIKIAPI	ns_CommandHandler* ns_CommandHandlerNew();
	NIKIAPI void ns_CommandHandlerDelete(ns_CommandHandler *pCommandHandler);

	NIKIAPI ns_Command* ns_CommandHandlerGet(ns_CommandHandler *pCommandHandler, const char *name);
	NIKIAPI ns_Command* ns_CommandHandlerAdd(ns_CommandHandler *pCommandHandler, const ns_Command *pCommand);

	NIKIAPI void ns_CommandHandlerErase(ns_CommandHandler *pCommandHandler, const char *name);
	NIKIAPI void ns_CommandHandlerClear(ns_CommandHandler *pCommandHandler);

	NIKIAPI const char** ns_CommandHandlerAllocKeys(ns_CommandHandler *pCommandHandler);

	NIKIAPI uint64_t ns_CommandHandlerSize(ns_CommandHandler *pCommandHandler);
}