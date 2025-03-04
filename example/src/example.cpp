#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>

struct Color {
	uint8_t r=0,g=0,b=0,a=255;
};

#define NIKISCRIPT_ARGUMENTS_EXTRA bool getColor(Context& ctx, Color& output);

#include <NikiScript.h>
#include <PrintCallback.h>
#include <Context.h>
#include <Token.h>
#include <Lexer.h>
#include <Parser.h>

bool parseStringToColor(const std::string& str, Color& buf, bool printError) {
	Color color = {0,0,0,255};
	constexpr const char* errorFormat = "expected \"RRR,GGG,BBB\" or \"RRR,GGG,BBB,AAA\" but received \"{}\"\n";

	try {
		size_t secondCommaIdx;
		{
			size_t firstCommaIdx = str.find(",");
			if (firstCommaIdx == std::string::npos) {
				if (printError)
					ns::printf(ns::PrintLevel::ERROR, errorFormat, str);

				return false;
			}

			color.r = std::stoi(str.substr(0, firstCommaIdx));

			secondCommaIdx = str.find(",", firstCommaIdx+1);
			if (secondCommaIdx == std::string::npos) {
				if (printError)
					ns::printf(ns::PrintLevel::ERROR, errorFormat, str);

				return false;
			}

			color.g = std::stoi(str.substr(firstCommaIdx+1, secondCommaIdx-firstCommaIdx-1));
		}

		size_t thirdCommaIdx = str.find(",", secondCommaIdx+1);

		color.b = std::stoi(str.substr(secondCommaIdx+1, thirdCommaIdx-secondCommaIdx-1));
		if (thirdCommaIdx != std::string::npos)
			color.a = std::stoi(str.substr(thirdCommaIdx+1));

		buf = color;
		return true;

	} catch (...) {
		if (printError)
			ns::printf(ns::PrintLevel::ERROR, errorFormat, str);

		return false;
	}
}

bool ns::Arguments::getColor(Context& ctx, Color& output) {
	if (!parseStringToColor(arguments[offset++], output, false)) {
		ns::printf(ns::PrintLevel::ERROR, "\"{}\" is not a valid color. Argument #{}: {}\n", arguments[offset-1], offset-1, ctx.pCommand->argsDescriptions[offset-1]);
		return false;
	}

	return true;
}

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

static void plus_test_command(ns::Context& ctx) {
	if (ctx.arguments.arguments.size() != 0)
		ns::printf(ns::PrintLevel::ECHO, "+test called! {}\n", ctx.arguments.getString());
	else
		ns::print(ns::PrintLevel::ECHO, "+test called!\n");
}

static void minus_test_command(ns::Context&) {
	ns::print(ns::PrintLevel::ECHO, "-test called!\n");
}

static std::string colorToString(const Color& color) {
	return ns::formatString("({}, {}, {}, {})",
		static_cast<uint16_t>(color.r),
		static_cast<uint16_t>(color.g),
		static_cast<uint16_t>(color.b),
		static_cast<uint16_t>(color.a));
}

static std::string getColor(ns::ProgramVariable* pVar) {
	return colorToString(*static_cast<Color*>(pVar->pValue));
}

static void setColor(ns::ProgramVariable* pVar, const std::string& str) {
	parseStringToColor(str, *static_cast<Color*>(pVar->pValue), true);
}

static void print_color_command(ns::Context& ctx) {
	Color c;
	if (!ctx.arguments.getColor(ctx, c))
		return;

	ns::print(ns::PrintLevel::ECHO, colorToString(c)+'\n');
}

int main(int, char**) {
	ns::setPrintCallback(nullptr, nikiScriptPrintCallback);

	ns::Context ctx;
	ns::registerCommands(ctx);

	std::string penes = "Penes";
	ns::registerVariable(ctx, "penes", &penes, ns::getString, ns::setString);
	Color preto = {0,0,0,255};
	ns::registerVariable(ctx, "preto", &preto, getColor, setColor);

	ns::Lexer lexer;
	ctx.pLexer = &lexer;

	ctx.commands.add(ns::Command("quit", 0, 1, quit_command, "stops the main loop from running", {"s[?]", ""}));

	ctx.commands.add(ns::Command("+test", 0,1, plus_test_command, "", {"s[?]", ""}));
	ctx.commands.add(ns::Command("-test", 0,0, minus_test_command, "", {}));

	ctx.commands.add(ns::Command("print_color", 1,1, print_color_command, "prints color in rgb format", {"s[color]", "color to be printed"}));

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