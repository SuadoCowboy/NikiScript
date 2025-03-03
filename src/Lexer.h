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

#ifndef SWEATCI_LOOP_VARIABLE
#define SWEATCI_LOOP_VARIABLE '!'
#endif

#ifndef SWEATCI_TOGGLE_ON_VARIABLE
#define SWEATCI_TOGGLE_ON_VARIABLE '+'
#endif

#ifndef SWEATCI_TOGGLE_OFF_VARIABLE
#define SWEATCI_TOGGLE_OFF_VARIABLE '-'
#endif

namespace sci {
	struct Lexer {
		std::string input;
		uint64_t position = 0;
		/**
		 * @brief This variable is where advance stores the token
		 * @see Lexer::advance
		 */
		Token token = {TokenType::NONE};

		Lexer();
		Lexer(const std::string& input);

		/**
		 * @brief Gets the next token in the input
		 * @note Position is set to the **next** token position in the end of this function
		 * @see Lexer::token
		 * @see Lexer::setTokenValue
		 * @see Lexer::setTokenType
		 */
		void advance();

		/**
		 * @brief Advances tokens until it reaches one of the flags
		 * @param flags Bitwise TokenType
		 * @see Lexer::advance
		 */
		void advanceUntil(uint8_t flags);

		/**
		 * @brief Gets token value by checking where a whitespace is found
		 *
		 * @return Next input's initial position. Either whitespace, EOS or END
		 * @see Lexer::setTokenType
		 */
		uint64_t setTokenValue();

		/**
		 * @brief Identifies token type by checking the previous token type
		 * @see Lexer::setTokenValue
		 */
		void setTokenType();

		/**
		 * @brief resets members
		 */
		void clear();
	};
}