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

void sci::ProgramVariable::callback(SweatContext& ctx) {
    if (ctx.pData == nullptr)
        return;

    ProgramVariable* pVar = static_cast<ProgramVariable*>(ctx.pData);

    if (ctx.arguments.arguments.size() == 1)
        pVar->set(pVar, ctx.arguments.getString());
    else
        sci::printf(PrintLevel::ECHO, "Value: {}\n", pVar->get(pVar));
}