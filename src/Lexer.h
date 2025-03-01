#pragma once

#include <stdint.h>
#include <string>

#include "Token.h"

#ifndef SWEATCI_STATEMENT_SEPARATOR
#define SWEATCI_STATEMENT_SEPARATOR ';'
#endif

#ifndef SWEATCI_REFERENCE
#define SWEATCI_REFERENCE '$'
#endif

#ifndef SWEATCI_REFERENCE_OPEN
#define SWEATCI_REFERENCE_OPEN '{'
#endif

#ifndef SWEATCI_REFERENCE_CLOSE
#define SWEATCI_REFERENCE_CLOSE '}'
#endif

namespace sci {
    class Lexer {
    public:
        std::string input;
        uint64_t position = 0;
        /**
         * @brief this variable is where advance stores the token
         * @see Lexer::advance
         */
        Token token = {TokenType::NONE};

        Lexer(const std::string& input);

        /**
         * @brief stores the token in the token member
         * @see Lexer::token
         */
        void advance();

        /**
         * @brief advances tokens until it reaches one of the flags
         * @param flags TokenType
         * @see Lexer::advance
         */
        void advanceUntil(uint8_t flags);
    
        /**
         * @brief Sets the value of the token
         * 
         * @return uint64_t next input initial position(either whitespace, EOS or END)
         * @see Lexer::setTokenType
         */
        uint64_t setTokenValue();

        /**
         * @brief Checks if the last token type was any of those and search which type current token should be
         * @see Lexer::setTokenValue
         */
        void setTokenType();
    };
}