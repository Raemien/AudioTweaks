#include "UI/AdvancedAudioView.hpp"
#include "UI/SecretCreditsView.hpp"
#include "config.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UnityEngine/Color.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "TMPro/TextAlignmentOptions.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"

#include "HMUI/AnimatedSwitchView_ColorBlock.hpp"
#include "HMUI/AnimatedSwitchView.hpp"
#include "HMUI/SimpleTextDropdown.hpp"
#include "HMUI/FlowCoordinator.hpp"

using namespace AudioTweaks;
DEFINE_TYPE(AudioTweaks, AdvancedAudioView);

AdvancedAudioView* AudioView;

void UpdateStereoToggle(bool newval)
{
    if (AudioView->swapToggle != nullptr) {
        StringW txt_on = newval ? "R" : "L";
        StringW txt_off = newval ? "L" : "R";
        auto* switchView = AudioView->swapToggle->GetComponent<HMUI::AnimatedSwitchView*>();
        switchView->offText->set_text(txt_off);
        switchView->onText->set_text(txt_on);

        auto col_base = switchView->onColors->knobColor + UnityEngine::Color(0.0f, 0.0f, 0.0f, 0.5f);
        auto col_off = newval ? UnityEngine::Color(1.0f, 1.0f, 1.0f, col_base.a * 2) - col_base : col_base;
        auto col_on = newval ? col_base : UnityEngine::Color(1.0f, 1.0f, 1.0f, col_base.a * 2) - col_base;
        switchView->offText->set_color(col_off);
        switchView->onText->set_color(col_on);
        switchView->offText->set_alpha(0.9f);
        switchView->onText->set_alpha(0.9f);

        switchView->originalKnobWidth = 0.0f;
        switchView->switchAnimationSmooth = 0.0f;
        switchView->switchAmount = 0.35f;
        switchView->offText->set_alpha(0.9f);
        switchView->onText->set_alpha(0.9f);
    }
}


void OnChangeBufferSize(StringW newval_str)
{
    int newval = std::stoi(to_utf8(newval_str));
    getConfig().config["DSPBufferSize"].SetInt(newval);
    getConfig().Write();
}

void OnSwapStereo(bool newval)
{
    UpdateStereoToggle(newval);
    getConfig().config["SwapChannels"].SetBool(newval);
    getConfig().Write();
}

void OnCredits()
{
    auto* activeFC = UnityEngine::GameObject::FindObjectOfType<GlobalNamespace::SettingsFlowCoordinator*>();

    if (activeFC != nullptr) {
        HMUI::ViewController* creditsVC = UnityEngine::Resources::FindObjectsOfTypeAll<AudioTweaks::SecretCreditsView*>()[0];
        if (!creditsVC) creditsVC = QuestUI::BeatSaberUI::CreateViewController<AudioTweaks::SecretCreditsView*>();
        activeFC->ReplaceViewController(creditsVC);
    }
}


void OnToggleMono(bool newval)
{
    getConfig().config["UseMonoAudio"].SetBool(newval);
    getConfig().Write();
    auto* channelgroup = AudioView->channelContainer;
    if (channelgroup != nullptr) channelgroup->get_gameObject()->SetActive(!newval);
}

void OnChangePan(float newval)
{
    getConfig().config["Pan"].SetFloat(newval);
    getConfig().Write();
}

void AdvancedAudioView::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    AudioView = this;
    if (firstActivation && addedToHierarchy)
    {
        auto& modcfg = getConfig().config;
        int cfg_dspsize = modcfg["DSPBufferSize"].GetInt();
        bool cfg_mono = modcfg["UseMonoAudio"].GetBool();
        bool cfg_swap = modcfg["SwapChannels"].GetBool();
        float cfg_vol = modcfg["VolumeBoost"].GetFloat();
        float cfg_pan = modcfg["Pan"].GetFloat();

        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        container->set_childForceExpandHeight(true);
        container->set_childControlHeight(false);
        
        UnityEngine::UI::VerticalLayoutGroup* maincontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        maincontainer->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 0, 0));
        maincontainer->set_childAlignment(UnityEngine::TextAnchor::MiddleLeft);
        maincontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);
        maincontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(2);
        maincontainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(110);

        QuestUI::BeatSaberUI::CreateText(maincontainer->get_rectTransform(), "Main", false)->set_fontSize(7);
        
        // I've capped the volume modifier at 150%. If this is ever raised, a volume warning modal popup should be used.
        auto* volslider = QuestUI::BeatSaberUI::CreateSliderSetting(maincontainer->get_rectTransform(), "Volume Boost", 0.1f, 1.0f, 0.5f, 1.5f, nullptr); 
        QuestUI::BeatSaberUI::AddHoverHint(volslider->get_gameObject(), "Overrides audio output volume beyond the game's limits. Can introduce clipping.");

        std::vector<StringW> bufferLengths {"128", "256", "512", "1024"};
        auto* bufferdrop = QuestUI::BeatSaberUI::CreateDropdown(maincontainer->get_transform(), u"Buffer Size", to_utf16(std::to_string(cfg_dspsize)), bufferLengths, OnChangeBufferSize);
        QuestUI::BeatSaberUI::AddHoverHint(bufferdrop->get_gameObject(), "Default is 512. Higher values increase bluetooth stability at the cost of audio latency.");


        UnityEngine::UI::VerticalLayoutGroup* subContainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(maincontainer->get_rectTransform());
        subContainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(110);
        subContainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredHeight(33);
        subContainer->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        subContainer->set_childForceExpandHeight(false);
        subContainer->set_childControlHeight(false);
        QuestUI::BeatSaberUI::CreateText(subContainer->get_rectTransform(), "Channels", false)->set_fontSize(7);
        this->monoToggle = QuestUI::BeatSaberUI::CreateToggle(subContainer->get_rectTransform(), "Force Mono", cfg_mono, OnToggleMono);

        this->channelContainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(subContainer->get_rectTransform());
        channelContainer->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        channelContainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(110);
        channelContainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);
        channelContainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(2);

        swapToggle = QuestUI::BeatSaberUI::CreateToggle(channelContainer->get_transform(), "Channel Output", cfg_swap, OnSwapStereo);

        this->panSlider = QuestUI::BeatSaberUI::CreateSliderSetting(channelContainer->get_transform(), "Channel Pan", 0.1, cfg_pan, -1.0f, 1.0f, 0.05f, OnChangePan);
    
        UpdateStereoToggle(cfg_swap);
        channelContainer->get_gameObject()->SetActive(!cfg_mono);

        std::string footertxt = GlobalNamespace::OVRPlugin::get_headphonesPresent() ? "Headphones detected!" : "   \n   ";
        auto* headphinfo = QuestUI::BeatSaberUI::CreateClickableText(maincontainer->get_rectTransform(), StringW(footertxt), true, OnCredits);
        headphinfo->set_fontSize(2.3f);
    }
}

void AdvancedAudioView::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}