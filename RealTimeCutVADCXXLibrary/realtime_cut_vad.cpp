//
// Created by Yasushi Sakita on 25/02/01.
// Refactored to remove WebRTC dependency
//

#include "realtime_cut_vad.h"
#include "process.h"
#include "algorithm.h"
#include "silero.h"
#include "misc.h"

RealTimeCutVAD::RealTimeCutVAD() {
    pImpl = std::make_unique<ProcessImpl>();

    // WebRTC configuration has been removed.
    // If needed, add alternative processing initialization here.
    // For example:
    // pImpl->initializeDummyProcessing();

    aImpl = std::make_unique<AlgorithmImpl>();
    aImpl->resetVariables();

    this->vad_start_detection_probability_threshold = SAMPLE_VAD_START_DETECTION_PROBABILITY_THRESHOLD;
    this->vad_end_detection_probability_threshold = SAMPLE_VAD_END_DETECTION_PROBABILITY_THRESHOLD;
    this->voice_start_vad_true_ratio_threshold = SAMPLE_VOICE_START_VAD_TRUE_RATIO_THRESHOLD;
    this->voice_end_vad_false_ratio_threshold = SAMPLE_VOICE_END_VAD_FALSE_RATIO_THRESHOLD;
    this->voice_start_frame_count_threshold = SAMPLE_VOICE_START_FRAME_COUNT_THRESHOLD;
    this->voice_end_frame_count_threshold = SAMPLE_VOICE_END_FRAME_COUNT_THRESHOLD;
}

RealTimeCutVAD::~RealTimeCutVAD() {
    pImpl.reset();
    aImpl.reset();
    sImpl.reset();
}

void RealTimeCutVAD::setSampleRate(SAMPLE_RATE choice_sample_rate_param) {
    this->choice_sample_rate = choice_sample_rate_param;
    int sampleRate = getSampleRateValue(this->choice_sample_rate);
    // Removed webrtc::StreamConfig usage.
    // If necessary, store sampleRate or configure dummy settings here.

    // Configure processing parameters based on sample rate.
    switch (this->choice_sample_rate) {
        case K_8:
            pImpl->input_audio_split_chunk_size = K_8_SPLIT_CHUNK_SIZE;
            pImpl->apm_sample_size = K_8_SPLIT_CHUNK_SIZE * K_16_SAMPLE_RATE / K_8_SAMPLE_RATE;
            break;
        case K_16:
            pImpl->input_audio_split_chunk_size = K_16_SPLIT_CHUNK_SIZE;
            pImpl->apm_sample_size = K_16_SPLIT_CHUNK_SIZE;
            break;
        case K_24:
            pImpl->input_audio_split_chunk_size = K_24_SPLIT_CHUNK_SIZE;
            pImpl->apm_sample_size = K_24_SPLIT_CHUNK_SIZE * K_16_SAMPLE_RATE / K_24_SAMPLE_RATE;
            break;
        case K_48:
            pImpl->input_audio_split_chunk_size = K_48_SPLIT_CHUNK_SIZE;
            pImpl->apm_sample_size = K_48_SPLIT_CHUNK_SIZE * K_16_SAMPLE_RATE / K_48_SAMPLE_RATE;
            break;
        default:
            throw std::runtime_error("Choose sample rate must be 8khz, 16khz, 24khz, or 48khz.");
    }
}

void RealTimeCutVAD::setThreshold(float vad_start_detection_probability,
                                  float vad_end_detection_probability,
                                  float voice_start_vad_true_ratio,
                                  float voice_end_vad_false_ratio,
                                  int voice_start_frame_count,
                                  int voice_end_frame_count) {
    this->vad_start_detection_probability_threshold = vad_start_detection_probability;
    this->vad_end_detection_probability_threshold = vad_end_detection_probability;
    this->voice_start_vad_true_ratio_threshold = voice_start_vad_true_ratio;
    this->voice_end_vad_false_ratio_threshold = voice_end_vad_false_ratio;
    this->voice_start_frame_count_threshold = voice_start_frame_count;
    this->voice_end_frame_count_threshold = voice_end_frame_count;
}

void RealTimeCutVAD::setModel(SILERO_VER ver, const std::string& model_path) {
    // Delete any existing model object
    if (sImpl) {
        sImpl.reset();
    }

    switch (ver) {
        case V4:
            sImpl = std::make_unique<SileroV4>();
            break;
        case V5:
            sImpl = std::make_unique<SileroV5>();
            break;
        default:
            throw std::invalid_argument("Invalid SILERO_VER specified");
    }

    sImpl->init_onnx_model(model_path);
}

