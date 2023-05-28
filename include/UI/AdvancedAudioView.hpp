#include "questui/shared/CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "custom-types/shared/macros.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(AudioTweaks, AdvancedAudioView, HMUI::ViewController,

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, channelContainer);

    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, monoToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, swapToggle);
    DECLARE_INSTANCE_FIELD(QuestUI::SliderSetting*, panSlider);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool systemScreenDisabling);
)