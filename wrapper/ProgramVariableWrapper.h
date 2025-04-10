#pragma once

#include <stdint.h>

#include "DLLExport.h"
#include "ProgramVariable.h"

typedef ns::GetProgramVariableValue ns_GetProgramVariableValue;
typedef ns::SetProgramVariableValue ns_SetProgramVariableValue;
typedef ns::ProgramVariable ns_ProgramVariable;
typedef ns::ProgramVariables ns_ProgramVariables;

extern "C" {
	NIKIAPI ns_ProgramVariable* ns_ProgramVariableNew(void* pValue, const char* description, ns_GetProgramVariableValue get, ns_SetProgramVariableValue set);
	NIKIAPI void ns_ProgramVariableDelete(ns_ProgramVariable* pVar);

	NIKIAPI void* ns_ProgramVariableGetValue(ns_ProgramVariable* pVar);
	NIKIAPI void ns_ProgramVariableSetValue(ns_ProgramVariable* pVar, void* pValue);

	NIKIAPI const char* ns_ProgramVariableGetDescription(ns_ProgramVariable* pVar);
	NIKIAPI void ns_ProgramVariableSetDescription(ns_ProgramVariable* pVar, const char* description);

	NIKIAPI ns_GetProgramVariableValue ns_ProgramVariableGetGetter(ns_ProgramVariable* pVar);
	NIKIAPI void ns_ProgramVariableSetGetter(ns_ProgramVariable* pVar, ns_GetProgramVariableValue get);

	NIKIAPI ns_SetProgramVariableValue ns_ProgramVariableGetSetter(ns_ProgramVariable* pVar);
	NIKIAPI void ns_ProgramVariableSetSetter(ns_ProgramVariable* pVar, ns_SetProgramVariableValue set);

	NIKIAPI ns_ProgramVariables* ns_ProgramVariablesNew();
	NIKIAPI void ns_ProgramVariablesDelete(ns_ProgramVariables* pVars);

	NIKIAPI ns_ProgramVariable* ns_ProgramVariablesGet(ns_ProgramVariables *pVars, const char *name);
	NIKIAPI ns_ProgramVariable* ns_ProgramVariablesAdd(ns_ProgramVariables *pVars, const char *name, const ns_ProgramVariable *pVar);

	NIKIAPI void ns_ProgramVariablesErase(ns_ProgramVariables *pVars, const char *name);
	NIKIAPI void ns_ProgramVariablesClear(ns_ProgramVariables *pVars);

	NIKIAPI const char** ns_ProgramVariablesAllocKeys(ns_ProgramVariables *pVars);

	NIKIAPI uint64_t ns_ProgramVariablesSize(ns_ProgramVariables *pVars);
}