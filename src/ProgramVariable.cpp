#include "ProgramVariable.h"

#include "PrintCallback.h"

sci::ProgramVariable::ProgramVariable() {}

sci::ProgramVariable::ProgramVariable(void* pVariable, const GetProgramVariableValue& get, const SetProgramVariableValue& set)
 : pValue(pVariable), get(get), set(set) {}

std::string sci::getString(const ProgramVariable& var) {
    return *static_cast<std::string*>(var.pValue);
}

void sci::setString(ProgramVariable& var, const std::string& str) {
    *static_cast<std::string*>(var.pValue) = str;
}