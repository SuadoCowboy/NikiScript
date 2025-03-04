#include "ProgramVariable.h"

#include "PrintCallback.h"
#include "SweatContext.h"

sci::ProgramVariable::ProgramVariable() {}

sci::ProgramVariable::ProgramVariable(void* pVariable, const GetProgramVariableValue& get, const SetProgramVariableValue& set)
 : pValue(pVariable), get(get), set(set) {}

std::string sci::getString(ProgramVariable* pVar) {
	return *static_cast<std::string*>(pVar->pValue);
}

void sci::setString(ProgramVariable* pVar, const std::string& str) {
	*static_cast<std::string*>(pVar->pValue) = str;
}