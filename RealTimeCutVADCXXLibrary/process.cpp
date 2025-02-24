//
// Created by Yasushi Sakita on 25/02/01.
//
#include "realtime_cut_vad.h"
#include "process.h"
#include "silero.h"

void RealTimeCutVAD::process(std::vector<float_t>& input_audio_data) {

    std::vector<std::vector<float_t>> splitted_audio_data = pImpl->split_audio_data_with_remainder(pImpl->remaining_audio_data_1, input_audio_data, pImpl->input_audio_split_chunk_size);

    for (std::vector<float_t> segment_audio_data : splitted_audio_data) {
        std::vector<float_t> rtc_processing_audio_data(pImpl->apm_sample_size);

        // Dummy processing: Just copy input to output without modifying it
        std::copy(segment_audio_data.begin(), segment_audio_data.end(), rtc_processing_audio_data.begin());

        std::vector<std::vector<float_t>> splitted_rtc_processing_audio_data = pImpl->split_audio_data_with_remainder(pImpl->remaining_audio_data_2, rtc_processing_audio_data, pImpl->silero_vad_sample_size);
        for (std::vector<float_t> segment_rtc_processing_audio_data : splitted_rtc_processing_audio_data) {
            float prob = sImpl->predict(segment_rtc_processing_audio_data);
            algorithm(prob, segment_rtc_processing_audio_data);
        }
    }
}

std::vector<std::vector<float_t>> RealTimeCutVAD::ProcessImpl::split_audio_data_with_remainder(std::vector<float_t>& remaining_audio_data, std::vector<float_t>& input_audio_data, size_t chunk_size) {
    std::vector<std::vector<float_t>> result;

    if (!remaining_audio_data.empty()) {
        input_audio_data.insert(input_audio_data.begin(), remaining_audio_data.begin(), remaining_audio_data.end());
        remaining_audio_data.clear();
    }

    for (size_t i = 0; i + chunk_size <= input_audio_data.size(); i += chunk_size) {
        std::vector<float_t> chunk(input_audio_data.begin() + i, input_audio_data.begin() + i + chunk_size);
        result.push_back(chunk);
    }

    remaining_audio_data = std::vector<float_t>(input_audio_data.begin() + chunk_size * (input_audio_data.size() / chunk_size), input_audio_data.end());

    return result;
}

