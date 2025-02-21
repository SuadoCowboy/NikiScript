#pragma once

#include <stdint.h>
#include <string>

#include "Token.h"

namespace sci {
    class Lexer {
    public:
        std::string input;
        uint64_t position = 0;
        Token firstToken = NONE;

        Lexer(const std::string& input);

        Token advance();

        /// @brief when firstToken.type is IDENTIFIER
        /// @return STRING or REFERENCE
        Token getArgument();
        /// @return STRING
        Token getQuotedString();
        /// @brief get all characters until a whitespace or end of input is reached
        /// @note when quotes are not placed this function is called
        /// @return STRING
        Token getString();
        Token getIdentifier();

        /// @brief only sets the first token if firstToken.type is NONE 
        void setFirstToken(const Token& token);
    };
}