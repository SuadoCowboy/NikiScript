#include "Variable.h"

#include "PrintCallback.h"

sci::Variable::Variable() {}

sci::Variable::Variable(const parseStringToVariable& parseString, const variableToString& toString,
    const std::string& name, void* pVariable)
 : pVariable(pVariable), name(name), parseString(parseString), toString(toString) {}