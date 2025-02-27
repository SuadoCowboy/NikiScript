#include "Variable.h"

sci::Variable::Variable() {}

sci::Variable::Variable(const std::string& name, VariableType type, void* pVariable)
 : type(type), pVariable(pVariable), name(name) {}