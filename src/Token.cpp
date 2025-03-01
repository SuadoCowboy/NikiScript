#include "Token.h"

#include "SweatContext.h"

sci::Token::Token() {}
sci::Token::Token(TokenType type) : type(type), value("") {}
sci::Token::Token(TokenType type, const std::string& value) : type(type), value(value) {}

uint8_t operator|(sci::TokenType l, sci::TokenType r) {
	return static_cast<uint8_t>(l)|static_cast<uint8_t>(r);
}

uint8_t operator|(uint8_t l, sci::TokenType r) {
	return l|static_cast<uint8_t>(r);
}

uint8_t operator|(sci::TokenType l, uint8_t r) {
	return static_cast<uint8_t>(l)|r;
}

uint8_t operator&(uint8_t l, sci::TokenType r) {
	return l&static_cast<uint8_t>(r);
}

uint8_t operator&(sci::TokenType l, uint8_t r) {
	return static_cast<uint8_t>(l)&r;
}

void sci::insertReferencesInToken(SweatContext& ctx, Token& token) {
	size_t offset = 0;
	for (auto& reference : token.references) {
		if (ctx.consoleVariables.count(reference.second) != 0) { // console variable
			token.value = token.value.insert(offset+reference.first, ctx.consoleVariables[reference.second]);
			offset += ctx.consoleVariables[reference.second].size();

		} else if (ctx.programVariables.count(reference.second) != 0) { // program variable
			ProgramVariable& var = ctx.programVariables[reference.second];
			std::string value = var.get(&var);

			token.value = token.value.insert(offset+reference.first, value);
			offset += value.size();
		
		} else {
			token.value = token.value.insert(reference.first, formatString("{}{}{}{}", SWEATCI_REFERENCE, SWEATCI_REFERENCE_OPEN, reference.second, SWEATCI_REFERENCE_CLOSE));
			offset += reference.second.size()+3;
		}
	}
}