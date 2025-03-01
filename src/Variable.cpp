#include "Variable.h"

#include "PrintCallback.h"

sci::Variable::Variable() {}

sci::Variable::Variable(const std::string_view& name, void* pVariable, const GetVariable& get, const SetVariable& set)
 : name(name), pVariable(pVariable), get(get), set(set) {}

std::string sci::getString(const Variable& var) {
    return *static_cast<std::string*>(var.pVariable);
}

void sci::setString(Variable& var, const std::string& str) {
    *static_cast<std::string*>(var.pVariable) = str;
}