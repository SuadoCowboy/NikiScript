#include "Lexer.h"

#include <sstream>

#include "PrintCallback.h"

sci::Lexer::Lexer() {}
sci::Lexer::Lexer(const std::string& input) : input(input) {}

void sci::Lexer::advance() {
	token.references.clear();
	while (position < input.size() && input[position] == ' ')
		++position;

	if (position >= input.size()) {
		token.type = TokenType::END;
		token.value = "";
		return;
	}

	uint64_t nextTokenPosition = setTokenValue();
	setTokenType();

	/*
	echo "This is a quoted string"
	echo This is not a quoted string
	"what happens" if I do this? // I belive the output would be: Unknown Command: "what
	*/

	position = nextTokenPosition;
}

void sci::Lexer::advanceUntil(uint8_t flags) {
	flags |= static_cast<uint8_t>(TokenType::END);

	advance();
	while (!(flags & token.type))
		advance();
}

uint64_t sci::Lexer::setTokenValue() {
	uint64_t nextTokenPosition = position;
	std::stringstream result;

	/*
	1 = allow white space and SWEATCI_STATEMENT_SEPARATOR
	2 = escape quotes allowed
	*/
	unsigned char flags = 0;

	while (nextTokenPosition < input.size() && (position == nextTokenPosition || ((input[nextTokenPosition] != ' ' && (input[nextTokenPosition] != SWEATCI_STATEMENT_SEPARATOR || (flags & 2))) || (flags & 1)))) {
		if (flags & 2) {
			flags &= ~2;
			result << input[nextTokenPosition++];
			continue;
		}

		if (input[nextTokenPosition] == '\\') {
			flags |= 2;
			++nextTokenPosition;
			continue;
		
		} else if (input[nextTokenPosition] == SWEATCI_REFERENCE && nextTokenPosition+1 < input.size() && input[nextTokenPosition+1] == SWEATCI_REFERENCE_OPEN) {
			std::stringstream referenceStream;

			uint64_t tempIndex = nextTokenPosition+2;
			
			bool foundCloseReference = false;
			for (; tempIndex < input.size() && input[tempIndex] != ' '; ++tempIndex) {
				if (input[tempIndex] == SWEATCI_REFERENCE_CLOSE) {
					++tempIndex;
					foundCloseReference = true;
					break;
				}

				referenceStream << input[tempIndex];
			}

			if (foundCloseReference) {
				token.references[result.str().size()] = referenceStream.str();
				nextTokenPosition = tempIndex;
				continue;
			}
		}
		
		if (input[nextTokenPosition] == '"') {
			++nextTokenPosition;
			
			if (flags & 1) {
				flags &= ~1;
				break;
			} else {
				flags |= 1;
				continue;
			}
		}

		result << input[nextTokenPosition++];
	}

	token.value = result.str();

	return nextTokenPosition;
}

void sci::Lexer::setTokenType() {
	if (token.type == TokenType::NONE || ((TokenType::EOS|TokenType::END) & token.type)) { // if the lexer just started
		token.type = TokenType::IDENTIFIER;

	} else if (!token.value.empty() && token.value[0] == SWEATCI_STATEMENT_SEPARATOR) {
		token.type = TokenType::EOS;

	} else if ((TokenType::IDENTIFIER|TokenType::ARGUMENT) & token.type)
		token.type = TokenType::ARGUMENT;
}