#include "Lexer.h"

#include <sstream>

#include "PrintCallback.h"

sci::Lexer::Lexer(const std::string& input) : input(input) {}

sci::Token sci::Lexer::advance() {
    /*
    BASICS:
    Let's say a string "a b c; d" goes into the lexer.
    "a" is passed as a command with "b" "c" as parameters.
    ";" is identified as a EOS(End Of Statement) and therefore
    identifies "d" as a command with no arguments.

    NOTE: The lexer only gives information to the parser.
    WHO DECIDES HOW THE ARGUMENTS ARE PASSED IS THE PARSER
    Example 1: echo "Hello! How are you ${name}?" -> {IDENTIFIER, STRING, REFERENCE, STRING} -> {IDENTIFIER, STRING}
    
    Example 2: echo ${name} -> {IDENTIFIER, REFERENCE} -> {IDENTIFIER, STRING}

    Example 3: incrementvar ${myNumber} 0 10 1 -> {IDENTIFIER, REFERENCE, NUMBER, NUMBER, NUMBER} -> {IDENTIFIER, REFERENCE, NUMBER, NUMBER, NUMBER}

    All depends on what the command itself is asking for. If a REFENRECE conversion is not possible, print usage of the command
    WE ARE ONLY CHECKING REFERENCES AND NOT ALL TYPES
    
    TODO: split code into functions
    */

    token.references.clear();
    while (position < input.size() && input[position] == ' ')
        ++position;

    if (position >= input.size()) {
        token.type = TokenType::END;
        token.value = "";
        return token;
    }

    uint64_t nextTokenPosition = setTokenValue();
    setTokenType();

    /*
    echo "This is a quoted string"
    echo This is not a quoted string
    "what happens" if I do this? // I belive the output would be: Unknown Command: "what
    */

    position = nextTokenPosition;
    return token;
}

uint64_t sci::Lexer::setTokenValue() {
    uint64_t nextTokenPosition = position;
    std::stringstream result;

    /*
    1 = allow white space
    2 = escape quotes allowed
    */
    unsigned char flags = 0;

    while (nextTokenPosition < input.size() && (position == nextTokenPosition || ((input[nextTokenPosition] != ' ' || (flags & 1)) && input[nextTokenPosition] != SWEATCI_STATEMENT_SEPARATOR))) {
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

            nextTokenPosition += 2;
            for (; nextTokenPosition < input.size() && input[nextTokenPosition] != ' '; ++nextTokenPosition) {
                if (input[nextTokenPosition] == '}') {
                    ++nextTokenPosition;
                    break;
                }

                referenceStream << input[nextTokenPosition];
            }

            token.references[result.str().size()] = referenceStream.str();
            continue;
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

    } else if ((TokenType::IDENTIFIER|TokenType::STRING|TokenType::NUMBER) & token.type) {
        try {
            std::stoi(token.value);
            token.type = TokenType::NUMBER;
        } catch (...) {
            token.type = TokenType::STRING;
        }
    }
}