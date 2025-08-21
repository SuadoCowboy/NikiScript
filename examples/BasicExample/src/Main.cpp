#include <iostream>
#include <string>
#include <stdint.h>

#include <NikiScript/NikiScript.h>
#include <NikiScript/Parser.h>

const char* printLevelToCStr(ns::PrintLevel level) {
	switch (level) {
	case ns::PrintLevel::ECHO:
		return "ECHO";
	case ns::PrintLevel::WARNING:
		return "WARNING";
	case ns::PrintLevel::ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

void nikiScriptPrintCallback(void*, ns::PrintLevel level, const char* msg) {
	std::cout << printLevelToCStr(level) << ": " << msg;
}


static void test_command(ns::CommandContext* pCtx, void*) {
	// we copy the context so that we can run stuff without changing the main context
	ns::Context contextCopy = ns::deepCopyContext(pCtx->pCtx);

	ns::CommandContext ctx{};
	ctx.pCtx = &contextCopy;

	ctx.lexer = pCtx->args.getString(0);

	ns::parse(&ctx);
}

bool running = false;
static void quit_command(ns::CommandContext*, void*) {
	running = false;
}

bool isCrazy = false;
static void crazy_command(ns::CommandContext* pCtx, void*) {
	if (pCtx->args.arguments.size() == 0) {
		ns::printf(ns::PrintLevel::ECHO, "{}\n", isCrazy);
		return;
	}

	isCrazy = pCtx->args.getSigned<uint32_t>(0) > 0;
}


void init(ns::Context& ctx) {
	ns::registerCommands(&ctx);

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

	char pName[16] = "nameless tee";
	ns::registerVariable(&ctx, "name", "", pName, ns::getCharArray, ns::setCharArray<16>);
}

int main(int, char**) {
	ns::setPrintCallback(nullptr, nikiScriptPrintCallback);

	ns::Context ctx;
	char _cfgDirectory[] = "data/cfg/";
	ctx.cfgDirectory = _cfgDirectory;
	init(ctx);

	std::string input;

	running = true;
	while (running) {
		input.clear();

		std::cout << "> ";
		std::getline(std::cin, input);

		ns::CommandContext commandCtx;
		commandCtx.pCtx = &ctx;
		commandCtx.lexer = input;
		ns::parse(&commandCtx);

		ns::updateLoopVariables(&ctx);
	}
}