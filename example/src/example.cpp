#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>

#define NS_CFG_DIRECTORY "data/cfg/"

#define NS_IMPLEMENTATION
#include <NikiScript/NikiScript.h>
#include <NikiScript/PrintCallback.h>
#include <NikiScript/Context.h>
#include <NikiScript/Token.h>
#include <NikiScript/Lexer.h>
#include <NikiScript/Parser.h>

void nikiScriptPrintCallback(void*, ns::PrintLevel level, const char* msg) {
	std::cout << ns::levelToString(level) << ": " << msg;
}

std::string tokenTypeToString(const ns::TokenType& type) {
	switch (type) {
	case ns::TokenType::NONE:
		return "NONE";
	case ns::TokenType::IDENTIFIER:
		return "IDENTIFIER";
	case ns::TokenType::ARGUMENT:
		return "ARGUMENT";
	case ns::TokenType::EOS:
		return "EOS";
	case ns::TokenType::END:
		return "END";
	default:
		return "UNKNOWN";
	}
}

std::string tokenToString(const ns::Token& token) {
	std::stringstream out{};
	out << '(' << tokenTypeToString(token.type) << ", \"" << token.value << "\", REFS: {";

	std::string formatted = token.value;
	if (token.references.empty())
		return out.str()+"})";

	else for (auto& ref : token.references) {
		out << '(' << ref.first << ", " << ref.second << "), ";
		formatted.insert(ref.first, ref.second);
	}

	std::string outString = out.str();
	outString.replace(outString.begin()+outString.size()-2, outString.begin()+outString.size()-1, "}) -> ");

	return outString + formatted;
}


static void test_command(ns::Context* pCtx) {
	ns::Context copy = ns::deepCopyContext(pCtx);
	ns::Lexer lexer{pCtx->args.getString(0)};
	copy.pLexer = &lexer;
	copy.args.arguments.clear();

	ns::parse(&copy);
}

bool running = false;
static void quit_command(ns::Context*) {
	running = false;
}

bool isCrazy = false;
static void crazy_command(ns::Context* pCtx) {
	if (pCtx->args.arguments.size() == 0) {
		ns::printf(ns::PrintLevel::ECHO, "{}\n", isCrazy);
		return;
	}

	isCrazy = pCtx->args.getSigned<uint32_t>(0) > 0;
}

int main(int, char**) {
	ns::setPrintCallback(nullptr, nikiScriptPrintCallback);

	ns::Context ctx;
	ns::registerCommands(&ctx);

	ns::Lexer lexer;
	ctx.pLexer = &lexer;

	nsRegisterCommand(&ctx, "quit", 0,1, quit_command, "stops the main loop from running", "s[?]", "");
	nsRegisterCommand(&ctx, "test", 1,1, test_command, "runs script", "s[script]", "parses to nikiscript");
	nsRegisterCommand(&ctx, "crazy", 0,1, crazy_command, "", "i[isCrazy?]", "");

	// decimal numbers
	float floatNumber = 0;
	ns::registerVariable(&ctx, "float", "", &floatNumber, ns::getNumber<float>, ns::setFloat);

	double doubleNumber = 0;
	ns::registerVariable(&ctx, "double", "", &doubleNumber, ns::getNumber<double>, ns::setDouble);

	long double lDoubleNumber = 0;
	ns::registerVariable(&ctx, "ldouble", "", &lDoubleNumber, ns::getNumber<long double>, ns::setLongDouble);

	// signed numbers
	int8_t int8 = 0;
	ns::registerVariable(&ctx, "int8", "", &int8, ns::getNumber<int8_t>, ns::setChar);

	int16_t int16 = 0;
	ns::registerVariable(&ctx, "int16", "", &int16, ns::getNumber<int16_t>, ns::setShort);

	int32_t int32 = 0;
	ns::registerVariable(&ctx, "int32", "", &int32, ns::getNumber<int32_t>, ns::setInteger);

	int64_t int64 = 0;
	ns::registerVariable(&ctx, "int64", "", &int64, ns::getNumber<int64_t>, ns::setLong);

	// unsigned numbers
	uint8_t uint8 = 0;
	ns::registerVariable(&ctx, "uint8", "", &uint8, ns::getNumber<uint8_t>, ns::setUnsigned<uint8_t>);

	uint16_t uint16 = 0;
	ns::registerVariable(&ctx, "uint16", "", &uint16, ns::getNumber<uint16_t>, ns::setUnsigned<uint16_t>);

	uint32_t uint32 = 0;
	ns::registerVariable(&ctx, "uint32", "", &uint32, ns::getNumber<uint32_t>, ns::setUnsigned<uint32_t>);

	unsigned long long uint64 = 0;
	ns::registerVariable(&ctx, "uint64", "", &uint64, ns::getNumber<unsigned long long>, ns::setUnsignedLongLong);

	running = true;
	while (running) {
		std::string input;

		std::cout << "> ";
		std::getline(std::cin, input);

		lexer.input = input;
		ns::parse(&ctx);
		lexer.clear();

		ns::updateLoopVariables(&ctx);
	}
}