#pragma once

#include <string>
#include <unordered_map>

namespace sci {
    struct ProgramVariable;

    typedef std::string_view(*GetProgramVariableValue)(const ProgramVariable& var);
    typedef void(*SetProgramVariableValue)(ProgramVariable& var, const std::string& str);

    struct ProgramVariable {
        std::string_view name;
        void* pValue = nullptr;

        GetProgramVariableValue get = nullptr;
        SetProgramVariableValue set = nullptr;

        ProgramVariable();
        ProgramVariable(const std::string_view& name, void* pVariable, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
    };

    typedef std::unordered_map<std::string, ProgramVariable> ProgramVariables;

    std::string getString(const ProgramVariable& var);
    void setString(ProgramVariable& var, const std::string& str);
}