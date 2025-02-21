#pragma once

#include <stdint.h>
#include <unordered_map>
#include <string>

namespace sci {
    struct Variable;
    typedef std::unordered_map<uint64_t, Variable*> References;

    /// @brief should not replace std::string but it should be used on commands arguments
    struct SweatString {
        std::string format;
        References references;

        /// @note character='$' -> 'a\\$bcde' = false
        /// @note 'a${b}cde' = 1
        /// @return 0 = char; 1 = variable; 2 = end reached
        uint8_t getCharOrVariable(uint64_t& offset, Variable*& pVariableOut, char*& charOut);
    };
}