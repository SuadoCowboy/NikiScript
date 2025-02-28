#include "Lexer.h"

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
    Example 1: echo "Hello! How are you ${name}?" -> {COMMAND, STRING, REFERENCE, STRING} -> {COMMAND, STRING}
    
    Example 2: echo ${name} -> {COMMAND, REFERENCE} -> {COMMAND, STRING}

    Example 3: incrementvar ${myNumber} 0 10 1 -> {COMMAND, REFERENCE, NUMBER, NUMBER, NUMBER} -> {COMMAND, REFERENCE, NUMBER, NUMBER, NUMBER}

    All depends on what the command itself is asking for. If a REFENRECE conversion is not possible, print usage of the command
    WE ARE ONLY CHECKING REFERENCES AND NOT ALL TYPES
    
    TODO: split code into functions
    */

    while (position < input.size() && input[position] == ' ')
        ++position;

    if (position >= input.size()) {
        token.type = TokenType::END;
        token.value = "";
        return token;
    }

    uint64_t nextTokenPosition = setTokenValue();
    setTokenType(nextTokenPosition);

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
    
    bool allowWhiteSpace = false;
    while (nextTokenPosition < input.size() && (position == nextTokenPosition || ((input[nextTokenPosition] != ' ' || allowWhiteSpace) && input[nextTokenPosition] != SWEATCI_STATEMENT_SEPARATOR))) {
        if (input[nextTokenPosition] == '"' && input[nextTokenPosition-1] != '\\') {
            if (allowWhiteSpace) {
                break;
            } else {
                allowWhiteSpace = true;
                ++position;
            }
        }

        ++nextTokenPosition;
    }

    token.value = input.substr(position, nextTokenPosition-position);
    if (allowWhiteSpace)
        ++nextTokenPosition;

    return nextTokenPosition;
}

void sci::Lexer::setTokenType(uint64_t nextTokenPosition) {
    if (token.type == TokenType::NONE || ((TokenType::EOS|TokenType::END) & token.type)) { // if the lexer just started
        token.type = TokenType::COMMAND;

    } else if (!token.value.empty() && token.value[0] == SWEATCI_STATEMENT_SEPARATOR) {
        token.type = TokenType::EOS;

    } else if ((TokenType::COMMAND|TokenType::REFERENCE|TokenType::STRING|TokenType::NUMBER) & token.type) {
        if (position < nextTokenPosition
         && input[position] == SWEATCI_REFERENCE
         && input[position+1] == SWEATCI_REFERENCE_OPEN
         && input[input.find(' ', position)] == SWEATCI_REFERENCE_CLOSE) {
            token.type = TokenType::REFERENCE;

        } else {
            try {
                std::stoi(token.value);
                token.type = TokenType::NUMBER;
            } catch (...) {
                token.type = TokenType::STRING;
            }
        }
    }
}