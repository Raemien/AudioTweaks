#include "main.hpp"
#include "config.hpp"
#include "UI/AudioFlowCoordinator.hpp"
#include "UI/AdvancedAudioView.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/FlowCoordinator.hpp"

using namespace AudioTweaks;
DEFINE_TYPE(AudioTweaks, AudioFlowCoordinator);

void AudioFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation) 
    {
        this->SetTitle(il2cpp_utils::newcsstr("Audio Tweaks"), (int)1);
        this->showBackButton = true;

        if (!this->mainView) this->mainView = QuestUI::BeatSaberUI::CreateViewController<AdvancedAudioView*>();
        AudioFlowCoordinator::ProvideInitialViewControllers(mainView, nullptr, nullptr, nullptr, nullptr);
    }
}

void AudioFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topView)
{
    getConfig().Write();
    HMUI::FlowCoordinator* activeFC = QuestUI::GetModSettingsFlowCoordinator();
    if (activeFC == nullptr) activeFC = QuestUI::BeatSaberUI::GetMainFlowCoordinator();

    activeFC->DismissFlowCoordinator(this, (int)0, nullptr, false);
}