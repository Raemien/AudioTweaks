#include "Types/InvertAudioFilter.hpp"
#include "UI/AdvancedAudioView.hpp"
#include "UI/DefaultAudioMenu.hpp"
#include "config.hpp"
#include "main.hpp"

#include "questui/shared/CustomTypes/Components/Settings/SliderSetting.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Vector2.hpp"

#include "UnityEngine/AudioSource.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/AnimatedSwitchView.hpp"
#include "HMUI/AnimatedSwitchView_ColorBlock.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"

#include "GlobalNamespace/SettingsFlowCoordinator.hpp"
#include "GlobalNamespace/SettingsNavigationController.hpp"

void ModifyAudioMenu(UnityEngine::Transform* container)
{
    auto& modcfg = getConfig().config;
    bool cfg_mono = modcfg["UseMonoAudio"].GetBool();
    bool cfg_swap = modcfg["SwapChannels"].GetBool();
    bool cfg_pan = modcfg["Pan"].GetFloat();

    UnityEngine::Transform* vol = container->Find(StringW("Volume"));
    vol->set_localScale(UnityEngine::Vector3(0.88f, 0.88f, 1.0f));
    UnityEngine::Transform* ambvol = container->Find(StringW("AmbientVolumeScale"));
    ambvol->set_localScale(UnityEngine::Vector3(0.88f, 0.88f, 1.0f));
    UnityEngine::Transform* latency = container->Find(StringW("OverrideAudioLatency"));
    latency->set_localScale(UnityEngine::Vector3(0.88f, 0.88f, 1.0f));
    UnityEngine::Transform* setupcont = container->Find(StringW("Setup"));
    setupcont->set_localScale(UnityEngine::Vector3(0.88f, 0.88f, 1.0f));
    setupcont->Find(StringW("InfoText"))->GetComponent<TMPro::TextMeshProUGUI*>()->set_fontSize(3.1f);
    setupcont->GetComponent<UnityEngine::UI::VerticalLayoutGroup*>()->set_spacing(2.0f);

    auto* audiosrc = setupcont->GetComponentInChildren<UnityEngine::AudioSource*>();
    audiosrc->set_bypassEffects(false);
    static auto asrc_bypassListenerFX = reinterpret_cast<function_ptr_t<void, UnityEngine::AudioSource*, bool>>(il2cpp_functions::resolve_icall("UnityEngine.AudioSource::set_bypassListenerEffects"));
    asrc_bypassListenerFX(audiosrc, false);

    UnityEngine::Transform* sep_1 = container->Find(StringW("HorizontalSeparator"));
    UnityEngine::Transform* sep_2 = container->Find(StringW("HorizontalSeparator (3)"));
    sep_1->get_gameObject()->SetActive(false);
    sep_2->get_gameObject()->SetActive(false);

    latency->SetAsLastSibling();
    setupcont->SetAsLastSibling();

    QuestUI::BeatSaberUI::CreateUIButton(container, "Show Advanced Settings", []()
    {
        auto* activeFC = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::SettingsFlowCoordinator*>()[0];
        HMUI::ViewController* audioVC = UnityEngine::Resources::FindObjectsOfTypeAll<AudioTweaks::AdvancedAudioView*>()[0];
        if (!audioVC) audioVC = QuestUI::BeatSaberUI::CreateViewController<AudioTweaks::AdvancedAudioView*>();
        activeFC->ReplaceViewController(audioVC);
    });
}