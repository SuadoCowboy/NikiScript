#pragma once

namespace sci {
    struct CommandContext {
        void* pData = nullptr;
        Command* pCommand = nullptr;
    };
}