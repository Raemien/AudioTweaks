#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Renderer.hpp"

#include "UnityEngine/AudioListener.hpp"
#include "UnityEngine/Camera.hpp"


DECLARE_CLASS_CODEGEN(AudioTweaks, InvertAudioFilter, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(UnityEngine::AudioListener*, listener);
    DECLARE_INSTANCE_FIELD(bool, filterReady);

    DECLARE_STATIC_METHOD(void, ApplyAudioSettings);

    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, OnAudioFilterRead, Array<float>* data, int channels);
)