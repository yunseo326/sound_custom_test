// src/AudioInput.cpp
#include "AudioInput.hpp"
#include <iostream>

// 생성자: 버퍼 초기화
AudioInput::AudioInput(int deviceId, int sampleRate, int framesPerBuffer)
    : deviceId(deviceId), sampleRate(sampleRate), framesPerBuffer(framesPerBuffer), stream(nullptr) {
    buffer.resize(framesPerBuffer, 0.0f);
}

AudioInput::~AudioInput() {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }
}

// PortAudio 콜백 함수
int AudioInput::audioCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData) {
    AudioInput* self = reinterpret_cast<AudioInput*>(userData);
    const float* in = static_cast<const float*>(inputBuffer);
    // 입력 데이터를 클래스 내부의 버퍼에 저장
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        self->buffer[i] = in[i];
    }
    return paContinue;
}

bool AudioInput::openStream() {
    PaStreamParameters inputParams;
    inputParams.device = deviceId;
    inputParams.channelCount = 1;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = nullptr;

    PaError err = Pa_OpenStream(&stream, &inputParams, nullptr, sampleRate, framesPerBuffer,
                                paClipOff, AudioInput::audioCallback, this);
    if (err != paNoError) {
        std::cerr << "스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool AudioInput::startStream() {
    if (!stream) return false;
    PaError err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool AudioInput::stopStream() {
    if (!stream) return false;
    PaError err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

const std::vector<float>& AudioInput::getBuffer() const {
    return buffer;
}
