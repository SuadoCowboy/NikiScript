#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace sci {
    enum class TokenType : uint8_t {
        NONE = 0,
        IDENTIFIER = 1, ///< If it's the first token after an EOS, END or NONE. Should be either a variable or command
        STRING = 4, ///< If lexer could not convert to integer and an identifier was already caught up
        NUMBER = 8, ///< If lexer was able to convert to integer
        EOS = 16, ///< End Of Statement
        END = 32 ///< End of input data
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

    struct SweatContext;

    /**
     * @brief inserts all references in the value
     */
    void expandStringToken(SweatContext& ctx, Token& token);
}

uint8_t operator|(sci::TokenType l, sci::TokenType r);
uint8_t operator|(uint8_t l, sci::TokenType r);
uint8_t operator|(sci::TokenType l, uint8_t r);
uint8_t operator&(uint8_t l, sci::TokenType r);
uint8_t operator&(sci::TokenType l, uint8_t r);