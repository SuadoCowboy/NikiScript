#pragma once
#include <stdint.h>
#include <string>

namespace sci {
    enum class TokenType : uint8_t {
        NONE = 0,
        COMMAND = 1,
        REFERENCE = 2, // '$' example: var "my var" "hello there"; echo "${my var}, how are you?". Yes it can use whitespace, but it's suggested not to
        STRING = 4,
        NUMBER = 8,
        EOS = 16, // End Of Statement
        END = 32
    };

    /// @brief either keep the original char array alive so that value won't have issues, or allocate to value
    struct Token {
        TokenType type = TokenType::NONE;
        std::string value;

        Token();
        Token(TokenType type);
        Token(TokenType type, const std::string& value);
    };
}

uint8_t operator|(const sci::TokenType& l, const sci::TokenType& r);
uint8_t operator|(uint8_t l, const sci::TokenType& r);
uint8_t operator|(const sci::TokenType& l, uint8_t r);
uint8_t operator&(uint8_t l, const sci::TokenType& r);
uint8_t operator&(const sci::TokenType& l, uint8_t r);