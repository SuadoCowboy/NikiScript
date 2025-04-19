#include "Token.h"

#include "Context.h"
#include "Parser.h"

ns::Token::Token() {}
ns::Token::Token(TokenType type) : type(type), value("") {}
ns::Token::Token(TokenType type, const char *value) : type(type), value(value) {}

uint8_t operator|(ns::TokenType l, ns::TokenType r) {
	return static_cast<uint8_t>(l)|static_cast<uint8_t>(r);
}

uint8_t operator|(uint8_t l, ns::TokenType r) {
	return l|static_cast<uint8_t>(r);
}

uint8_t operator|(ns::TokenType l, uint8_t r) {
	return static_cast<uint8_t>(l)|r;
}

uint8_t operator&(uint8_t l, ns::TokenType r) {
	return l&static_cast<uint8_t>(r);
}

uint8_t operator&(ns::TokenType l, uint8_t r) {
	return static_cast<uint8_t>(l)&r;
}

static void printAppendToString(void* pBuffer, ns::PrintLevel, const char* string) {
	static_cast<std::string*>(pBuffer)->append(string);
}

void ns::insertReferencesInToken(Context& ctx, Token& token) {
	size_t offset = 0;
	for (auto& reference : token.references) {
		if (ctx.consoleVariables.count(reference.second) != 0) { // console variable
			token.value = token.value.insert(offset+reference.first, ctx.consoleVariables[reference.second]);
			offset += ctx.consoleVariables[reference.second].size();

		} else if (ctx.programVariables.count(reference.second) != 0) { // program variable
			ProgramVariable& var = ctx.programVariables[reference.second];
			std::string value = var.get(ctx, &var);

			token.value = token.value.insert(offset+reference.first, value);
			offset += value.size();

		} else {
			Arguments originalArgs = ctx.args;
			Lexer *pOriginalLexer = ctx.pLexer;
			Command *pOriginalCommand = ctx.pCommand;
			std::string originalFilePath = ctx.filePath;
			uint8_t originalOrigin = ctx.origin;

			Lexer tempLexer{reference.second};
			ctx.pLexer = &tempLexer;

			clearStatementData(ctx);

			void* pOriginalPrintCallbackData = pPrintCallbackData;
			PrintCallback originalPrintCallback = printCallback;

			std::string printOutput = "";
			setPrintCallback(&printOutput, printAppendToString);

			ctx.origin |= ns::OriginType::REFERENCE;
			parse(ctx);

			setPrintCallback(pOriginalPrintCallbackData, originalPrintCallback);

			ctx.pLexer = pOriginalLexer;
			ctx.pCommand = pOriginalCommand;
			ctx.args = originalArgs;
			ctx.filePath = originalFilePath;
			ctx.origin = originalOrigin;

			while (printOutput.size() != 0 && printOutput[printOutput.size()-1] == '\n')
				printOutput.erase(printOutput.begin()+(printOutput.size()-1));

			token.value = token.value.insert(offset+reference.first, printOutput);
			offset += printOutput.size();

		}
		// else {
		// 	token.value = token.value.insert(offset+reference.first, formatString("{}{}{}{}", NIKISCRIPT_REFERENCE, NIKISCRIPT_REFERENCE_OPEN, reference.second, NIKISCRIPT_REFERENCE_CLOSE));
		// 	offset += reference.second.size()+3;
		// }
	}
}