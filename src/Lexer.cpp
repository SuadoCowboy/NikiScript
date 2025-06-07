#include "Lexer.h"

#include <sstream>

#include "Utils.h"

ns::Lexer::Lexer() {}
ns::Lexer::Lexer(const std::string& input) : input(input) {}

void ns::Lexer::advance() {
	token.references.clear();
	while (position < input.size() && isSpaceNotNewline(input[position]))
		++position;

	if (position >= input.size()) {
		token.type = TokenType::END;
		token.value = "";
		return;
	}

	if (input[position] == '\n')
		lineIndex++;

	setTokenType(setTokenValue());
}

void ns::Lexer::advanceUntil(uint8_t flags) {
	flags |= static_cast<uint8_t>(TokenType::END);

	advance();
	while (!(flags & token.type))
		advance();
}

bool ns::Lexer::setTokenValue() {
	if (input[position] == NS_STATEMENT_SEPARATOR || input[position] == '\n') {
		token.value = NS_STATEMENT_SEPARATOR;
		return position+1;
	}

	size_t nextTokenPosition = position;
	token.value.clear();

	/*
	1 = allow white space and NS_STATEMENT_SEPARATOR
	2 = escape next char if is a known char like NS_STATEMENT_SEPARATOR
	4 = skipping all until NS_COMMENT_LINES+NS_COMMENT_LINE is found
	8 = token value is NS_STATEMENT_SEPARATOR and should its type should be string
	*/
	uint8_t flags = openArguments == 0? 0 : 1;

	while (nextTokenPosition < input.size() && (position == nextTokenPosition || ((!isSpaceNotNewline(input[nextTokenPosition]) && (input[nextTokenPosition] != NS_STATEMENT_SEPARATOR || (flags & 2))) || (flags & 1)))) {
		if (flags & 2) {
			flags &= ~2;
			switch (input[nextTokenPosition]) {
			case NS_ARGUMENTS_CLOSE:
			case NS_ARGUMENTS_OPEN:
			case NS_ARGUMENTS_QUOTE:
			case NS_ARGUMENTS_SEPARATOR:
			case NS_COMMENT_LINE:
			case NS_COMMENT_LINES:
			case NS_ESCAPE_NEXT_CHAR:
			case NS_REFERENCE:
			case NS_REFERENCE_CLOSE:
			case NS_REFERENCE_OPEN:
			case NS_STATEMENT_SEPARATOR:
				break;
			default:
				token.value += input[nextTokenPosition-1];
				break;
			}
			token.value += input[nextTokenPosition++];
			continue;
		}

		if (flags & 4) {
			if (input[nextTokenPosition] == '\n')
				++lineIndex;

			if (input[nextTokenPosition] == NS_COMMENT_LINE && input[nextTokenPosition-1] == NS_COMMENT_LINES)
				flags &= ~5;

			++nextTokenPosition;
			continue;
		}

		if (input[nextTokenPosition] == '\n')
			break;

		if (nextTokenPosition+1 < input.size() && input[nextTokenPosition] == NS_COMMENT_LINE) {
			if (input[nextTokenPosition+1] == NS_COMMENT_LINE) {
				size_t i = nextTokenPosition;
				for (; i < input.size(); ++i) {
					if (input[i] == '\n')
						break;
				}

				nextTokenPosition = i;
				break;
			}

			if (input[nextTokenPosition+1] == NS_COMMENT_LINES) {
				flags |= 5; // 4 | 1
				nextTokenPosition += 3;
				continue;
			}
		}

		if (input[nextTokenPosition] == NS_ARGUMENTS_OPEN) {
			if (token.type == TokenType::NONE || ((TokenType::EOS|TokenType::END) & token.type))
				break;

			// only accept this kind of arguments tokenization if begins exactly after the identifier
			else if (token.type == TokenType::IDENTIFIER && openArguments == 0) {
				++openArguments;
				flags |= 1;
				++nextTokenPosition;

			} else {
				// if the input is something like: "help a (b)" we can just ignore all of them.
				// we keep track of how many arguments are open because we need to know if the arguments separator will be used or not by the first open argument, which is the only one we care.
				if (openArguments != 0)
					++openArguments;
				token.value += input[nextTokenPosition++];
			}

			continue;

		} else if (input[nextTokenPosition] == NS_ARGUMENTS_SEPARATOR && openArguments == 1) {
			if (token.value.size() == 1 && token.value[0] == NS_STATEMENT_SEPARATOR)
				flags |= 8;
			++nextTokenPosition;
			break;

		} else if (input[nextTokenPosition] == NS_ARGUMENTS_CLOSE && openArguments != 0) {
			--openArguments;
			if (openArguments == 0) {
				if (token.value.size() == 1 && token.value[0] == NS_STATEMENT_SEPARATOR)
					flags |= 8;
				++nextTokenPosition;
				break;
			}
		}

		if (input[nextTokenPosition] == NS_ESCAPE_NEXT_CHAR) {
			flags |= 2;
			++nextTokenPosition;
			continue;

		} else if (input[nextTokenPosition] == NS_REFERENCE && nextTokenPosition+1 < input.size() && input[nextTokenPosition+1] == NS_REFERENCE_OPEN) {
			std::stringstream referenceStream;

			size_t tempIndex = nextTokenPosition+2;

			size_t referenceCount = 0; ///< how many references were found inside of it
			bool foundCloseReference = false;
			for (; tempIndex < input.size() && input[tempIndex] != '\n'; ++tempIndex) {
				if (input[tempIndex] == NS_REFERENCE && tempIndex+1 < input.size() && input[tempIndex+1] == NS_REFERENCE_OPEN)
					++referenceCount;
				else if (input[tempIndex] == NS_REFERENCE_CLOSE) {
					if (referenceCount == 0) {
						++tempIndex;
						foundCloseReference = true;
						break;
					} else
						--referenceCount;
				}

				referenceStream << input[tempIndex];
			}

			if (foundCloseReference) {
				token.references.emplace_back(token.value.size(), referenceStream.str());
				nextTokenPosition = tempIndex;
				continue;
			}
		
		} else if (input[nextTokenPosition] == NS_ARGUMENTS_QUOTE && openArguments == 0) {
			++nextTokenPosition;
			
			if (flags & 1) {
				if (token.value.size() == 1 && token.value[0] == NS_STATEMENT_SEPARATOR)
					flags |= 8;
				flags &= ~1;
				break;
			} else {
				flags |= 1;
				continue;
			}
		}

		token.value += input[nextTokenPosition++];
	}

	position = nextTokenPosition;
	return (flags & 8);
}

void ns::Lexer::setTokenType(bool skipStatementSeparator) {
	if (!skipStatementSeparator && token.value.size() == 1 && token.value[0] == NS_STATEMENT_SEPARATOR) {
		token.type = TokenType::EOS;

	} else if (token.type == TokenType::NONE || ((TokenType::EOS|TokenType::END) & token.type)) { // if the lexer just started and is not EOS
		token.type = TokenType::IDENTIFIER;

	} else if ((TokenType::IDENTIFIER|TokenType::ARGUMENT) & token.type)
		token.type = TokenType::ARGUMENT;
}

void ns::Lexer::clear() {
	input.clear();
	position = 0;
	token = {TokenType::NONE};
	openArguments = 0;
	lineIndex = 0;
}