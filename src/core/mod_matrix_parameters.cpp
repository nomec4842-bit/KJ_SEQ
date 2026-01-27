#include "core/mod_matrix_parameters.h"

#include "core/track_type_sample.h"
#include "core/track_type_synth.h"
#include "core/tracks.h"

#include <algorithm>
#include <array>

namespace
{

constexpr uint32_t kTrackTypeMaskAll = modMatrixTrackTypeToMask(TrackType::Synth) |
                                        modMatrixTrackTypeToMask(TrackType::Sample) |
                                        modMatrixTrackTypeToMask(TrackType::MidiOut) |
                                        modMatrixTrackTypeToMask(TrackType::VST);
constexpr uint32_t kTrackTypeMaskSynth = modMatrixTrackTypeToMask(TrackType::Synth);
constexpr uint32_t kTrackTypeMaskSample = modMatrixTrackTypeToMask(TrackType::Sample);

float synthOscWavetableGetter(int trackId, int oscIndex)
{
    return trackGetSynthOscWavetableEnabled(trackId, oscIndex) ? 1.0f : 0.0f;
}

void synthOscWavetableSetter(int trackId, int oscIndex, float value)
{
    trackSetSynthOscWavetableEnabled(trackId, oscIndex, value >= 0.5f);
}

float synthOsc1Wavetable(int trackId) { return synthOscWavetableGetter(trackId, 0); }
float synthOsc2Wavetable(int trackId) { return synthOscWavetableGetter(trackId, 1); }
float synthOsc3Wavetable(int trackId) { return synthOscWavetableGetter(trackId, 2); }
void setSynthOsc1Wavetable(int trackId, float value) { synthOscWavetableSetter(trackId, 0, value); }
void setSynthOsc2Wavetable(int trackId, float value) { synthOscWavetableSetter(trackId, 1, value); }
void setSynthOsc3Wavetable(int trackId, float value) { synthOscWavetableSetter(trackId, 2, value); }

float synthOsc1Formant(int trackId) { return trackGetSynthOscFormant(trackId, 0); }
float synthOsc2Formant(int trackId) { return trackGetSynthOscFormant(trackId, 1); }
float synthOsc3Formant(int trackId) { return trackGetSynthOscFormant(trackId, 2); }
void setSynthOsc1Formant(int trackId, float value) { trackSetSynthOscFormant(trackId, 0, value); }
void setSynthOsc2Formant(int trackId, float value) { trackSetSynthOscFormant(trackId, 1, value); }
void setSynthOsc3Formant(int trackId, float value) { trackSetSynthOscFormant(trackId, 2, value); }

float synthOsc1Resonance(int trackId) { return trackGetSynthOscResonance(trackId, 0); }
float synthOsc2Resonance(int trackId) { return trackGetSynthOscResonance(trackId, 1); }
float synthOsc3Resonance(int trackId) { return trackGetSynthOscResonance(trackId, 2); }
void setSynthOsc1Resonance(int trackId, float value) { trackSetSynthOscResonance(trackId, 0, value); }
void setSynthOsc2Resonance(int trackId, float value) { trackSetSynthOscResonance(trackId, 1, value); }
void setSynthOsc3Resonance(int trackId, float value) { trackSetSynthOscResonance(trackId, 2, value); }

float synthOsc1Feedback(int trackId) { return trackGetSynthOscFeedback(trackId, 0); }
float synthOsc2Feedback(int trackId) { return trackGetSynthOscFeedback(trackId, 1); }
float synthOsc3Feedback(int trackId) { return trackGetSynthOscFeedback(trackId, 2); }
void setSynthOsc1Feedback(int trackId, float value) { trackSetSynthOscFeedback(trackId, 0, value); }
void setSynthOsc2Feedback(int trackId, float value) { trackSetSynthOscFeedback(trackId, 1, value); }
void setSynthOsc3Feedback(int trackId, float value) { trackSetSynthOscFeedback(trackId, 2, value); }

float synthOsc1Pitch(int trackId) { return trackGetSynthOscPitch(trackId, 0); }
float synthOsc2Pitch(int trackId) { return trackGetSynthOscPitch(trackId, 1); }
float synthOsc3Pitch(int trackId) { return trackGetSynthOscPitch(trackId, 2); }
void setSynthOsc1Pitch(int trackId, float value) { trackSetSynthOscPitch(trackId, 0, value); }
void setSynthOsc2Pitch(int trackId, float value) { trackSetSynthOscPitch(trackId, 1, value); }
void setSynthOsc3Pitch(int trackId, float value) { trackSetSynthOscPitch(trackId, 2, value); }

float synthOsc1PitchRange(int trackId) { return trackGetSynthOscPitchRange(trackId, 0); }
float synthOsc2PitchRange(int trackId) { return trackGetSynthOscPitchRange(trackId, 1); }
float synthOsc3PitchRange(int trackId) { return trackGetSynthOscPitchRange(trackId, 2); }
void setSynthOsc1PitchRange(int trackId, float value) { trackSetSynthOscPitchRange(trackId, 0, value); }
void setSynthOsc2PitchRange(int trackId, float value) { trackSetSynthOscPitchRange(trackId, 1, value); }
void setSynthOsc3PitchRange(int trackId, float value) { trackSetSynthOscPitchRange(trackId, 2, value); }

float synthOsc1Attack(int trackId) { return trackGetSynthOscAttack(trackId, 0); }
float synthOsc2Attack(int trackId) { return trackGetSynthOscAttack(trackId, 1); }
float synthOsc3Attack(int trackId) { return trackGetSynthOscAttack(trackId, 2); }
void setSynthOsc1Attack(int trackId, float value) { trackSetSynthOscAttack(trackId, 0, value); }
void setSynthOsc2Attack(int trackId, float value) { trackSetSynthOscAttack(trackId, 1, value); }
void setSynthOsc3Attack(int trackId, float value) { trackSetSynthOscAttack(trackId, 2, value); }

float synthOsc1Decay(int trackId) { return trackGetSynthOscDecay(trackId, 0); }
float synthOsc2Decay(int trackId) { return trackGetSynthOscDecay(trackId, 1); }
float synthOsc3Decay(int trackId) { return trackGetSynthOscDecay(trackId, 2); }
void setSynthOsc1Decay(int trackId, float value) { trackSetSynthOscDecay(trackId, 0, value); }
void setSynthOsc2Decay(int trackId, float value) { trackSetSynthOscDecay(trackId, 1, value); }
void setSynthOsc3Decay(int trackId, float value) { trackSetSynthOscDecay(trackId, 2, value); }

float synthOsc1Sustain(int trackId) { return trackGetSynthOscSustain(trackId, 0); }
float synthOsc2Sustain(int trackId) { return trackGetSynthOscSustain(trackId, 1); }
float synthOsc3Sustain(int trackId) { return trackGetSynthOscSustain(trackId, 2); }
void setSynthOsc1Sustain(int trackId, float value) { trackSetSynthOscSustain(trackId, 0, value); }
void setSynthOsc2Sustain(int trackId, float value) { trackSetSynthOscSustain(trackId, 1, value); }
void setSynthOsc3Sustain(int trackId, float value) { trackSetSynthOscSustain(trackId, 2, value); }

float synthOsc1Release(int trackId) { return trackGetSynthOscRelease(trackId, 0); }
float synthOsc2Release(int trackId) { return trackGetSynthOscRelease(trackId, 1); }
float synthOsc3Release(int trackId) { return trackGetSynthOscRelease(trackId, 2); }
void setSynthOsc1Release(int trackId, float value) { trackSetSynthOscRelease(trackId, 0, value); }
void setSynthOsc2Release(int trackId, float value) { trackSetSynthOscRelease(trackId, 1, value); }
void setSynthOsc3Release(int trackId, float value) { trackSetSynthOscRelease(trackId, 2, value); }

struct ModParameterEntry
{
    ModMatrixParameter id;
    ModParameterInfo info;
};

constexpr std::array<ModParameterEntry, 46> kModParameters = {
    ModParameterEntry{ModMatrixParameter::Volume,
                      ModParameterInfo{L"Volume", trackGetVolume, trackSetVolume, 0.0f, 1.0f, kTrackTypeMaskAll}},
    ModParameterEntry{ModMatrixParameter::Pan,
                      ModParameterInfo{L"Pan", trackGetPan, trackSetPan, -1.0f, 1.0f, kTrackTypeMaskAll}},
    ModParameterEntry{ModMatrixParameter::SynthPitch,
                      ModParameterInfo{L"Synth Pitch", trackGetSynthPitch, trackSetSynthPitch, -12.0f, 12.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthFormant,
                      ModParameterInfo{L"Synth Formant", trackGetSynthFormant, trackSetSynthFormant, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthResonance,
                      ModParameterInfo{L"Synth Resonance", trackGetSynthResonance, trackSetSynthResonance, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthFeedback,
                      ModParameterInfo{L"Synth Feedback", trackGetSynthFeedback, trackSetSynthFeedback, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthPitchRange,
                      ModParameterInfo{L"Synth Pitch Range", trackGetSynthPitchRange, trackSetSynthPitchRange, 1.0f, 24.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthAttack,
                      ModParameterInfo{L"Synth Attack", trackGetSynthAttack, trackSetSynthAttack, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthDecay,
                      ModParameterInfo{L"Synth Decay", trackGetSynthDecay, trackSetSynthDecay, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthSustain,
                      ModParameterInfo{L"Synth Sustain", trackGetSynthSustain, trackSetSynthSustain, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthRelease,
                      ModParameterInfo{L"Synth Release", trackGetSynthRelease, trackSetSynthRelease, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Wavetable,
                      ModParameterInfo{L"Synth Osc 1 Wavetable", synthOsc1Wavetable, setSynthOsc1Wavetable, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Formant,
                      ModParameterInfo{L"Synth Osc 1 Formant", synthOsc1Formant, setSynthOsc1Formant, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Resonance,
                      ModParameterInfo{L"Synth Osc 1 Resonance", synthOsc1Resonance, setSynthOsc1Resonance, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Feedback,
                      ModParameterInfo{L"Synth Osc 1 Feedback", synthOsc1Feedback, setSynthOsc1Feedback, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Pitch,
                      ModParameterInfo{L"Synth Osc 1 Pitch", synthOsc1Pitch, setSynthOsc1Pitch, -12.0f, 12.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1PitchRange,
                      ModParameterInfo{L"Synth Osc 1 Pitch Range", synthOsc1PitchRange, setSynthOsc1PitchRange, 1.0f, 24.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Attack,
                      ModParameterInfo{L"Synth Osc 1 Attack", synthOsc1Attack, setSynthOsc1Attack, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Decay,
                      ModParameterInfo{L"Synth Osc 1 Decay", synthOsc1Decay, setSynthOsc1Decay, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Sustain,
                      ModParameterInfo{L"Synth Osc 1 Sustain", synthOsc1Sustain, setSynthOsc1Sustain, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc1Release,
                      ModParameterInfo{L"Synth Osc 1 Release", synthOsc1Release, setSynthOsc1Release, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Wavetable,
                      ModParameterInfo{L"Synth Osc 2 Wavetable", synthOsc2Wavetable, setSynthOsc2Wavetable, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Formant,
                      ModParameterInfo{L"Synth Osc 2 Formant", synthOsc2Formant, setSynthOsc2Formant, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Resonance,
                      ModParameterInfo{L"Synth Osc 2 Resonance", synthOsc2Resonance, setSynthOsc2Resonance, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Feedback,
                      ModParameterInfo{L"Synth Osc 2 Feedback", synthOsc2Feedback, setSynthOsc2Feedback, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Pitch,
                      ModParameterInfo{L"Synth Osc 2 Pitch", synthOsc2Pitch, setSynthOsc2Pitch, -12.0f, 12.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2PitchRange,
                      ModParameterInfo{L"Synth Osc 2 Pitch Range", synthOsc2PitchRange, setSynthOsc2PitchRange, 1.0f, 24.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Attack,
                      ModParameterInfo{L"Synth Osc 2 Attack", synthOsc2Attack, setSynthOsc2Attack, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Decay,
                      ModParameterInfo{L"Synth Osc 2 Decay", synthOsc2Decay, setSynthOsc2Decay, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Sustain,
                      ModParameterInfo{L"Synth Osc 2 Sustain", synthOsc2Sustain, setSynthOsc2Sustain, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc2Release,
                      ModParameterInfo{L"Synth Osc 2 Release", synthOsc2Release, setSynthOsc2Release, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Wavetable,
                      ModParameterInfo{L"Synth Osc 3 Wavetable", synthOsc3Wavetable, setSynthOsc3Wavetable, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Formant,
                      ModParameterInfo{L"Synth Osc 3 Formant", synthOsc3Formant, setSynthOsc3Formant, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Resonance,
                      ModParameterInfo{L"Synth Osc 3 Resonance", synthOsc3Resonance, setSynthOsc3Resonance, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Feedback,
                      ModParameterInfo{L"Synth Osc 3 Feedback", synthOsc3Feedback, setSynthOsc3Feedback, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Pitch,
                      ModParameterInfo{L"Synth Osc 3 Pitch", synthOsc3Pitch, setSynthOsc3Pitch, -12.0f, 12.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3PitchRange,
                      ModParameterInfo{L"Synth Osc 3 Pitch Range", synthOsc3PitchRange, setSynthOsc3PitchRange, 1.0f, 24.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Attack,
                      ModParameterInfo{L"Synth Osc 3 Attack", synthOsc3Attack, setSynthOsc3Attack, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Decay,
                      ModParameterInfo{L"Synth Osc 3 Decay", synthOsc3Decay, setSynthOsc3Decay, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Sustain,
                      ModParameterInfo{L"Synth Osc 3 Sustain", synthOsc3Sustain, setSynthOsc3Sustain, 0.0f, 1.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SynthOsc3Release,
                      ModParameterInfo{L"Synth Osc 3 Release", synthOsc3Release, setSynthOsc3Release, 0.0f, 4.0f,
                                       kTrackTypeMaskSynth}},
    ModParameterEntry{ModMatrixParameter::SampleAttack,
                      ModParameterInfo{L"Sample Attack", trackGetSampleAttack, trackSetSampleAttack, 0.0f, 4.0f,
                                       kTrackTypeMaskSample}},
    ModParameterEntry{ModMatrixParameter::SampleRelease,
                      ModParameterInfo{L"Sample Release", trackGetSampleRelease, trackSetSampleRelease, 0.0f, 4.0f,
                                       kTrackTypeMaskSample}},
    ModParameterEntry{ModMatrixParameter::DelayMix,
                      ModParameterInfo{L"Delay Mix", trackGetDelayMix, trackSetDelayMix, 0.0f, 1.0f, kTrackTypeMaskAll}},
    ModParameterEntry{ModMatrixParameter::CompressorThreshold,
                      ModParameterInfo{L"Compressor Threshold", trackGetCompressorThresholdDb, trackSetCompressorThresholdDb,
                                       -60.0f, 0.0f, kTrackTypeMaskAll}},
    ModParameterEntry{ModMatrixParameter::CompressorRatio,
                      ModParameterInfo{L"Compressor Ratio", trackGetCompressorRatio, trackSetCompressorRatio, 1.0f, 20.0f,
                                       kTrackTypeMaskAll}},
};

} // namespace

const ModParameterInfo* modMatrixGetParameterInfo(int index)
{
    if (index < 0 || index >= static_cast<int>(kModParameters.size()))
        return nullptr;
    return &kModParameters[static_cast<size_t>(index)].info;
}

int modMatrixGetParameterCount()
{
    return static_cast<int>(kModParameters.size());
}

int modMatrixGetParameterIndex(ModMatrixParameter parameter)
{
    for (size_t i = 0; i < kModParameters.size(); ++i)
    {
        if (kModParameters[i].id == parameter)
            return static_cast<int>(i);
    }
    return -1;
}

bool modMatrixParameterSupportsTrackType(const ModParameterInfo& info, TrackType trackType)
{
    uint32_t mask = modMatrixTrackTypeToMask(trackType);
    return (info.trackTypeMask & mask) != 0;
}

float modMatrixClampNormalized(float normalized)
{
    return std::clamp(normalized, -1.0f, 1.0f);
}

float modMatrixNormalizedToValue(float normalized, const ModParameterInfo& info)
{
    float depth = modMatrixClampNormalized(normalized);
    float range = info.maxValue - info.minValue;
    return depth * range;
}

float modMatrixValueToNormalized(float value, const ModParameterInfo& info)
{
    float range = info.maxValue - info.minValue;
    if (range <= 0.0f)
        return 0.0f;

    float clamped = std::clamp(value, -range, range);
    return modMatrixClampNormalized(clamped / range);
}
