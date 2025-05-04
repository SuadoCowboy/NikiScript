#include "Token.h"

#include "PrintCallback.h"
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

void ns::insertReferencesInToken(Context* pCtx, Token& token) {
	size_t offset = 0;
	for (auto& reference : token.references) {
		if (pCtx->consoleVariables.count(reference.second) != 0) { // console variable
			token.value = token.value.insert(offset+reference.first, pCtx->consoleVariables[reference.second]);
			offset += pCtx->consoleVariables[reference.second].size();

		} else if (pCtx->programVariables.count(reference.second) != 0) { // program variable
			ProgramVariable& var = pCtx->programVariables[reference.second];
			std::string value = var.get(pCtx, &var);

			token.value = token.value.insert(offset+reference.first, value);
			offset += value.size();

		} else {
			void* pOriginalPrintCallbackData = pPrintCallbackData;
			PrintCallback originalPrintCallback = printCallback;

			std::string printOutput;
			setPrintCallback(&printOutput, printAppendToStringEchoOnly);

			uint8_t originalOrigin = pCtx->origin;

			pCtx->origin |= OriginType::REFERENCE;
			parseInsideAnotherScript(pCtx, reference.second.c_str());
			setPrintCallback(pOriginalPrintCallbackData, originalPrintCallback);

			pCtx->origin = originalOrigin;

			for (size_t i = 0; i < printOutput.size(); ++i) {
				if (printOutput[i] == '\n') {
					if (i == printOutput.size()-1) {
						printOutput.pop_back();
						break;
					} else
						printOutput[i] = ' ';
				}
			}

			token.value = token.value.insert(offset+reference.first, printOutput);
			offset += printOutput.size();
		}
		// else {
		// 	token.value = token.value.insert(offset+reference.first, formatString("{}{}{}{}", NS_REFERENCE, NS_REFERENCE_OPEN, reference.second, NS_REFERENCE_CLOSE));
		// 	offset += reference.second.size()+3;
		// }
	}
}