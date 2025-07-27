#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "DLL.h"

namespace ns {
	typedef uint8_t TokenFlag;
	enum TokenType : TokenFlag {
		NONE = 0,
		IDENTIFIER = 1, ///< If it's the first token after an EOS, END or NONE. Should be either a variable or command
		ARGUMENT = 2, ///< If lexer could not convert to integer and an identifier was already caught up
		EOS = 4, ///< End Of Statement
		END = 8 ///< End of input data
	};

	struct NS_API Token {
		TokenType type = TokenType::NONE;
		std::string value;
		std::vector<std::pair<size_t, std::string>> references{}; ///< References identified in Token::value. **pair.first** = index where should insert the reference but _does not count with previous inserted references_

		Token();
		Token(TokenType type);
		Token(TokenType type, const char *value);
	};

	struct Context;

	/**
	 * @brief inserts all references in the value
	 */
	NS_API void insertReferencesInToken(Context* pCtx, Token& token);
}

NS_API ns::TokenFlag operator|(ns::TokenType l, ns::TokenType r);
NS_API ns::TokenFlag operator|(ns::TokenFlag l, ns::TokenType r);
NS_API ns::TokenFlag operator|(ns::TokenType l, ns::TokenFlag r);
NS_API ns::TokenFlag operator&(ns::TokenFlag l, ns::TokenType r);
NS_API ns::TokenFlag operator&(ns::TokenType l, ns::TokenFlag r);