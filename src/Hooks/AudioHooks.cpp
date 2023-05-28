#include "Helpers/HookInstaller.hpp"
#include "Types/InvertAudioFilter.hpp"
#include "config.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/AudioListener.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"

#include "GlobalNamespace/MainCamera.hpp"
#include "GlobalNamespace/AudioListenerController.hpp"
#include "GlobalNamespace/AudioManagerSO.hpp"

// To prevent desync freezes in-game, audio configuration should only be applied during scene transitions.
MAKE_HOOK_MATCH(SceneManager_SceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene prevScene, UnityEngine::SceneManagement::Scene nextScene)
{
    SceneManager_SceneChanged(prevScene, nextScene);
    if (nextScene.get_name() != "ShaderWarmup") return;
    AudioTweaks::InvertAudioFilter::ApplyAudioSettings();
}

MAKE_HOOK_MATCH(MainCamera_Awake, &GlobalNamespace::MainCamera::Awake, void, GlobalNamespace::MainCamera* instance)
{
    MainCamera_Awake(instance);
    auto* listener = instance->GetComponent<UnityEngine::AudioListener*>();
    auto* filter = instance->get_gameObject()->AddComponent<AudioTweaks::InvertAudioFilter*>();
    filter->listener = listener;
}

MAKE_HOOK_MATCH(AudioListenerController_Awake, &GlobalNamespace::AudioListenerController::Awake, void, GlobalNamespace::AudioListenerController* instance)
{
    AudioListenerController_Awake(instance);
    auto* listener = instance->GetComponent<UnityEngine::AudioListener*>();
    auto* filter = instance->get_gameObject()->AddComponent<AudioTweaks::InvertAudioFilter*>();
    filter->listener = listener;
}

MAKE_HOOK_MATCH(AudioManagerSO_SetMainVolume, &GlobalNamespace::AudioManagerSO::set_mainVolume, void, GlobalNamespace::AudioManagerSO* instance, float value)
{
    float mul_vol = getConfig().config["Volume"].GetFloat();
    value *= std::clamp(mul_vol, 0.01f, 2.00f);
    AudioManagerSO_SetMainVolume(instance, value);
}

void AudioTweaks::Hooks::Install_AudioHooks()
{
    INSTALL_HOOK(getLogger(), MainCamera_Awake);
    INSTALL_HOOK(getLogger(), AudioListenerController_Awake);
    INSTALL_HOOK(getLogger(), AudioManagerSO_SetMainVolume);
    INSTALL_HOOK(getLogger(), SceneManager_SceneChanged);
    getLogger().info("[AudioTweaks] Installed Audio hooks");
}