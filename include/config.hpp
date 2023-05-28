#include "modloader/shared/modloader.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t
{
    bool SwapChannels = false;
    bool UseMonoAudio = false;
    int DSPBufferSize = 512;
    float VolumeBoost = 1.0f;
    float Pan = 0.0f;
};

extern Config_t Settings;

bool LoadConfig();

void SetupConfig();
