#pragma once

#include <vector>
#include <stdint.h>

#include "SweatString.h"

namespace sci {
    struct Arguments {
        std::vector<SweatString> arguments;
        uint8_t offset = 0;

        SweatString& getString();

        int8_t getInt8(CommandContext& ctx);
        uint8_t getUInt8(CommandContext& ctx);

        int16_t getInt16(CommandContext& ctx);
        uint16_t getUInt16(CommandContext& ctx);

        int32_t getInt32(CommandContext& ctx);
        uint32_t getUInt32(CommandContext& ctx);

        int64_t getInt64(CommandContext& ctx);
        uint64_t getUInt64(CommandContext& ctx);

        float getFloat(CommandContext& ctx);
        double getDouble(CommandContext& ctx);

#ifdef SWEATCI_ARGUMENTS_EXTRA
        SWEATCI_ARGUMENTS_EXTRA
#endif
    };

    struct CommandContext {
        void* pData = nullptr;
        Command* pCommand = nullptr;
        Arguments arguments;
    };
}