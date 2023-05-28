#include "config.hpp"
#include "Helpers/HookInstaller.hpp"
#include "UI/AudioFlowCoordinator.hpp"
using namespace AudioTweaks;

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "custom-types/shared/register.hpp"
#include "questui/shared/QuestUI.hpp"

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

extern "C" void setup(ModInfo& info) {
    info.id = "AudioTweaks";
    info.version = "0.1.0";
    modInfo = info;
    getConfig().Load();
}

extern "C" void load() {
    if (!LoadConfig()) SetupConfig();
    il2cpp_functions::Init();
    QuestUI::Init();
    
    AudioTweaks::Hooks::Install_AudioHooks();
    AudioTweaks::Hooks::Install_UIHooks();

    custom_types::Register::AutoRegister();
}
