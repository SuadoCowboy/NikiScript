#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>

#include <NikiScript.h>
#include <PrintCallback.h>
#include <Context.h>
#include <Token.h>
#include <Lexer.h>
#include <Parser.h>

void nikiScriptPrintCallback(void*, ns::PrintLevel level, const std::string& msg) {
	std::cout << ns::printLevelToString(level) << ": " << msg;
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
	std::stringstream out;
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

bool running = false;

static void quit_command(ns::Context&) {
	running = false;
}

int main(int, char**) {
	ns::setPrintCallback(nullptr, nikiScriptPrintCallback);

	ns::Context ctx;
	ns::registerCommands(ctx);

	ns::Lexer lexer;
	ctx.pLexer = &lexer;

	ctx.commands.add(ns::Command("quit", 0, 1, quit_command, "stops the main loop from running", {"s[?]", ""}));

	// var (hello, var (b, Hello there!); echo \${b})
	// var = IDENTIFIER
	// ( = open();

	// read until , or )
	// , => return hello = ARGUMENT

	// ( => ignore because it's already inside one

	// read until , or )

	// , => ignore because we're ignoring the previous (

	// read until , or )

	// ) => ignore because this is together with the previous (
	// ) => var (b, Hello there!) = ARGUMENT

	running = true;
	while (running) {
		std::string input;

		std::cout << "> ";
		std::getline(std::cin, input);

		lexer.input = input;
		ns::parse(ctx);
		lexer.clear();

		ns::updateLoopVariables(ctx);
	}
}