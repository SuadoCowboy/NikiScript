#include "ProgramVariableWrapper.h"

#include "BaseWrapper.h"

ns_ProgramVariable* ns_ProgramVariableNew(void *pValue, const char *description, ns_GetProgramVariableValue get, ns_SetProgramVariableValue set) {
	return new ns_ProgramVariable(pValue, description, get, set);
}

void ns_ProgramVariableDelete(ns_ProgramVariable *pVar) {
	delete pVar;
}


void* ns_ProgramVariableGetValue(ns_ProgramVariable *pVar) {
	return pVar->pValue;
}

void ns_ProgramVariableSetValue(ns_ProgramVariable *pVar, void *pValue) {
	pVar->pValue = pValue;
}


const char* ns_ProgramVariableGetDescription(ns_ProgramVariable *pVar) {
	return pVar->description.c_str();
}

void ns_ProgramVariableSetDescription(ns_ProgramVariable *pVar, const char *description) {
	pVar->description = description;
}

ns_GetProgramVariableValue ns_ProgramVariableGetGetter(ns_ProgramVariable* pVar) {
	return pVar->get;
}

void ns_ProgramVariableSetGetter(ns_ProgramVariable* pVar, ns_GetProgramVariableValue get) {
	pVar->get = get;
}


ns_SetProgramVariableValue ns_ProgramVariableGetSetter(ns_ProgramVariable* pVar) {
	return pVar->set;
}

void ns_ProgramVariableSetSetter(ns_ProgramVariable* pVar, ns_SetProgramVariableValue set) {
	pVar->set = set;
}



ns_ProgramVariables* ns_ProgramVariablesNew() {
	return new ns_ProgramVariables();
}

void ns_ProgramVariablesDelete(ns_ProgramVariables *pVars) {
	delete pVars;
}

ns_ProgramVariable* ns_ProgramVariablesGet(ns_ProgramVariables *pVars, const char *name) {
	auto it = pVars->find(name);

	if (it == pVars->end())
		return nullptr;
	else
		return &it->second;
}

ns_ProgramVariable* ns_ProgramVariablesAdd(ns_ProgramVariables *pVars, const char *name, const ns_ProgramVariable *pVar) {
	(*pVars)[name] = *pVar;
	return &(*pVars)[name];
}

void ns_ProgramVariablesErase(ns_ProgramVariables *pVars, const char *name) {
	pVars->erase(name);
}

void ns_ProgramVariablesClear(ns_ProgramVariables *pVars) {
	pVars->clear();
}

const char** ns_ProgramVariablesAllocKeys(ns_ProgramVariables *pVars) {
	return ns::allocUnorderedMapStringKeysToCharArray(*pVars);
}

uint64_t ns_ProgramVariablesSize(ns_ProgramVariables *pVars) {
	return pVars->size();
}