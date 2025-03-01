#pragma once

#include <stdint.h>
#include <string>
#include <unordered_map>

namespace sci {
    typedef std::string(*variableToString)();
    typedef void(*parseStringToVariable)(const std::string& str);

    struct Variable {
        void* pVariable = nullptr;
        std::string name = nullptr;

        parseStringToVariable parseString = nullptr;
        variableToString toString = nullptr;

        Variable();
        Variable(const parseStringToVariable& parseString, const variableToString& toString,
            const std::string& name, void* pVariable);
    };

    typedef std::unordered_map<std::string, Variable> Variables;
}
