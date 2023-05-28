#include "UI/SecretCreditsView.hpp"
#include "config.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "TMPro/TextAlignmentOptions.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

using namespace AudioTweaks;
DEFINE_TYPE(AudioTweaks, SecretCreditsView);

void SecretCreditsView::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation && addedToHierarchy)
    {
        UnityEngine::UI::VerticalLayoutGroup* container = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        container->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        container->set_childForceExpandHeight(true);
        container->set_childControlHeight(false);
        
        auto* titlecontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_transform());
        titlecontainer->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::newcsstr("panel-top"));
        titlecontainer->set_padding(UnityEngine::RectOffset::New_ctor(10, 10, 0, 0));
        titlecontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);
        titlecontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(2);
        auto* title_thanks = QuestUI::BeatSaberUI::CreateText(titlecontainer->get_rectTransform(), "Special Thanks <3", false);
        title_thanks->set_fontSize(8);

        UnityEngine::UI::VerticalLayoutGroup* maincontainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(container->get_rectTransform());
        maincontainer->set_padding(UnityEngine::RectOffset::New_ctor(2, 2, 2, 2));
        maincontainer->set_childAlignment(UnityEngine::TextAnchor::MiddleLeft);
        maincontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(2);
        maincontainer->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(2);
        maincontainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(110);

        auto* thanks_blu = QuestUI::BeatSaberUI::CreateText(maincontainer->get_rectTransform(), "This mod was commissioned by b1uscr34m.\nAudioTweaks would not exist without their support, so a huge thanks goes out to them!");
        thanks_blu->set_enableWordWrapping(true);
        thanks_blu->set_fontSize(3.8f);

        UnityEngine::UI::VerticalLayoutGroup* subContainer = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(maincontainer->get_rectTransform());
        subContainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(110);
        subContainer->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredHeight(33);
        subContainer->set_childAlignment(UnityEngine::TextAnchor::UpperLeft);
        subContainer->set_childForceExpandHeight(false);
        subContainer->set_childControlHeight(false);
    }
}

void SecretCreditsView::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)
{
    getConfig().Write();
}