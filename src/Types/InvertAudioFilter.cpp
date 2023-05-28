#include "Types/InvertAudioFilter.hpp"
#include "main.hpp"
#include "config.hpp"

#include "beatsaber-hook/shared/utils/utils.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Camera.hpp"

#include "UnityEngine/AudioListener.hpp"
#include "UnityEngine/AudioSettings.hpp"
#include "UnityEngine/AudioConfiguration.hpp"
#include "UnityEngine/AudioSettings_Mobile.hpp"
#include "UnityEngine/AudioSpeakerMode.hpp"

#include "GlobalNamespace/OVRPlugin.hpp"

DEFINE_TYPE(AudioTweaks, InvertAudioFilter);

namespace AudioTweaks
{
	void InvertAudioFilter::Start()
    {
        if (!listener) listener = this->GetComponentInChildren<UnityEngine::AudioListener*>();
        filterReady = true;
	}

    void InvertAudioFilter::ApplyAudioSettings()
    {
        auto& modcfg = getConfig().config;
        bool mono = modcfg["UseMonoAudio"].GetBool();
        int buffersize = modcfg["DSPBufferSize"].GetInt();

        auto audiocfg = UnityEngine::AudioSettings::GetConfiguration();
        getLogger().info("[AudioTweaks] Previous DSP size: %d", audiocfg.dspBufferSize);
        audiocfg.dspBufferSize = buffersize;
        getLogger().info("[AudioTweaks] New DSP size: %d", audiocfg.dspBufferSize);
        audiocfg.speakerMode = mono ? UnityEngine::AudioSpeakerMode::Mono : UnityEngine::AudioSpeakerMode::Stereo;

        static auto aud_ResetConf = reinterpret_cast<function_ptr_t<bool, ByRef<UnityEngine::AudioConfiguration>>>(il2cpp_functions::resolve_icall("UnityEngine.AudioSettings::SetConfiguration_Injected"));
        bool success = aud_ResetConf(audiocfg);
        if (!success) getLogger().info("[AudioTweaks] Failed to apply new configuration!");

    }

    void InvertAudioFilter::OnAudioFilterRead(Array<float>* data, int channels)
    {
        auto& modcfg = getConfig().config;
        bool mono = modcfg["UseMonoAudio"].GetBool();
        bool invert = modcfg["SwapChannels"].GetBool();
        float bias = std::clamp(modcfg["Pan"].GetFloat(), -1.0f, 1.0f);
        // if (!listener || (!invert && bias == 0.0f)) return;
        if (!listener ) return;
        if (channels != 2) {
            getLogger().info("[StereoSwap] Input is not stereo!");
            return;
        }

        if (invert) bias *= -1;
        if (mono) bias = 0.0f;
        int fullLen = data->Length();
		ArrayW<float> newdata = ArrayW<float>(fullLen);
		for (int i = 0; i < fullLen; i++)
		{
			int leftIndex = invert ? std::min(fullLen - 1, i + 1) : i;
			int rightIndex = invert ? i - 1 : i;
            // if (mono) leftIndex = rightIndex;
            float sample_l = data->values[leftIndex] * (1 - bias);
            float sample_r =  data->values[rightIndex] * (1 + bias);
			if (i % 2 == 0) {
				newdata[i] = sample_l;
			}
			else newdata[i] = mono ? (newdata[i-1]) : sample_r;
		}
        newdata.CopyTo(data, 0);
    }  
    
}
