#include "Helpers/HookInstaller.hpp"
#include "UI/DefaultAudioMenu.hpp"
#include "config.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"

#include "GlobalNamespace/SettingsNavigationController.hpp"

MAKE_HOOK_MATCH(SettingsNavController_DidActivate, &GlobalNamespace::SettingsNavigationController::DidActivate, void, GlobalNamespace::SettingsNavigationController* instance, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    SettingsNavController_DidActivate(instance, firstActivation, addedToHierarchy, screenSystemEnabling);

    // Find container's transform
    UnityEngine::Transform* audiocontainer = instance->get_transform()->Find(StringW("AudioSettings"));
    if (audiocontainer == nullptr) audiocontainer = instance->get_transform()->Find(StringW("Container/AudioSettings"));

    // Modifiy the base-game audio menu
    if (firstActivation && addedToHierarchy) ModifyAudioMenu(audiocontainer);
}

void AudioTweaks::Hooks::Install_UIHooks()
{
    INSTALL_HOOK(getLogger(), SettingsNavController_DidActivate);
    getLogger().info("[AudioTweaks] Installed UI hooks");
}