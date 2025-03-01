#include "Command.h"

#include <sstream>

#include "PrintCallback.h"

sci::Command::Command() {}

sci::Command::Command(const std::string_view& name, unsigned char minArgs, unsigned char maxArgs,
    CommandCallback callback, const std::string_view& description, const std::vector<std::string_view>& argsDescriptions)
  : name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), description(description), argsDescriptions(argsDescriptions) {}

void sci::Command::printDescription() {
    std::stringstream oss;
    bool isName = true;

    for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
        if (isName)
            oss << argsDescriptions[i] << ": ";
        else
            oss << argsDescriptions[i] << ';';

        isName = !isName;
    }

    sci::printf(PrintLevel::ECHO, "Description: {}\nArguments: {}\n", description, oss.str());
}

void sci::Command::printUsage() {
    std::stringstream usage;

    usage << "Usage: " << name << ' ';
    bool isName = true;
    for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
        if (isName) {
            usage << argsDescriptions[i] << ' ';
        }

        isName = !isName;
    }
    usage << '\n';

    sci::print(PrintLevel::ECHO, usage.str());
}

void sci::Command::printAsDataTree() {
    std::stringstream descriptions;
    std::stringstream usage;
    bool isName = true;

    usage << "- Usage: " << name << ' ';
    descriptions << "- Description: " << description << "\n- Arguments:\n";
    for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
        if (isName) {
            descriptions << "- - " << argsDescriptions[i] << ": ";
            usage << argsDescriptions[i] << ' ';

        } else
            descriptions << argsDescriptions[i] << '\n';

        isName = !isName;
    }
    usage << '\n' << descriptions.str();

    sci::print(PrintLevel::ECHO, usage.str());
}