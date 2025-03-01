#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace sci {
    enum class TokenType : uint8_t {
        NONE = 0,
        IDENTIFIER = 1,
        STRING = 4,
        NUMBER = 8,
        EOS = 16, // End Of Statement
        END = 32
    };

    /// @brief either keep the original char array alive so that value won't have issues, or allocate to value
    struct Token {
        TokenType type = TokenType::NONE;
        std::string value;
        std::unordered_map<uint64_t, std::string> references;

        Token();
        Token(TokenType type);
        Token(TokenType type, const std::string& value);
    };
}

uint8_t operator|(sci::TokenType l, sci::TokenType r);
uint8_t operator|(uint8_t l, sci::TokenType r);
uint8_t operator|(sci::TokenType l, uint8_t r);
uint8_t operator&(uint8_t l, sci::TokenType r);
uint8_t operator&(sci::TokenType l, uint8_t r);