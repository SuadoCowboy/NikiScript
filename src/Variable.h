#pragma once

#include <stdint.h>
#include <string_view>
#include <string>
#include <unordered_map>

namespace sci {
    struct Variable;

    typedef std::string_view(*GetVariable)(const Variable& var);
    typedef void(*SetVariable)(Variable& var, const std::string& str);

    struct Variable {
        std::string_view name;
        void* pVariable = nullptr;

        GetVariable get = nullptr;
        SetVariable set = nullptr;

        Variable();
        Variable(const std::string_view& name, void* pVariable, const GetVariable& get, const SetVariable& set);
    };

    typedef std::unordered_map<std::string, Variable> Variables;

    std::string getString(const Variable& var);
    void setString(Variable& var, const std::string& str);
}
