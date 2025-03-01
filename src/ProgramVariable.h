#pragma once

#include <string>
#include <unordered_map>

namespace sci {
    struct ProgramVariable;

    typedef std::string(*GetProgramVariableValue)(const ProgramVariable& var);
    typedef void(*SetProgramVariableValue)(ProgramVariable& var, const std::string& str);

    struct ProgramVariable {
        void* pValue = nullptr;

        GetProgramVariableValue get = nullptr;
        SetProgramVariableValue set = nullptr;

        ProgramVariable();
        ProgramVariable(void* pVariable, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
    };

    typedef std::unordered_map<std::string, ProgramVariable> ProgramVariables;

    std::string getString(const ProgramVariable& var);
    void setString(ProgramVariable& var, const std::string& str);
}