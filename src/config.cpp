#include "modloader/shared/modloader.hpp"
#include "config.hpp"
#include "main.hpp"

#include "UnityEngine/Time.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/OVRPlugin_Media.hpp"

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

struct Config_t Settings = Config_t();

void SetupConfig()
{
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    auto& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("SwapChannels", Settings.SwapChannels, allocator);
    getConfig().config.AddMember("UseMonoAudio", Settings.UseMonoAudio, allocator);
    getConfig().config.AddMember("DSPBufferSize", Settings.DSPBufferSize, allocator);
    getConfig().config.AddMember("VolumeBoost", Settings.VolumeBoost, allocator);
    getConfig().config.AddMember("Pan", Settings.Pan, allocator);
    getConfig().Write();
}

bool LoadConfig()
{
    getConfig().Load();
    if(!getConfig().config.HasMember("SwapChannels") || !getConfig().config["SwapChannels"].IsBool()) return false;
    if(!getConfig().config.HasMember("UseMonoAudio") || !getConfig().config["UseMonoAudio"].IsBool()) return false;
    if(!getConfig().config.HasMember("DSPBufferSize") || !getConfig().config["DSPBufferSize"].IsInt()) return false;
    if(!getConfig().config.HasMember("VolumeBoost") || !getConfig().config["VolumeBoost"].IsFloat()) return false;
    if(!getConfig().config.HasMember("Pan") || !getConfig().config["Pan"].IsFloat()) return false;
    return true;
}
