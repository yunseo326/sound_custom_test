#include <iostream>
#include <vector>
#include <portaudio.h>
#include "matplotlibcpp.h"  // 그래프 라이브러리

namespace plt = matplotlibcpp;

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

std::vector<float> audioData;  // 오디오 샘플 저장

// 콜백 함수 (오디오 데이터 저장)
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    const float* in = (const float*)inputBuffer;
    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        audioData.push_back(in[i]);  // 오디오 샘플 저장
    }
    return paContinue;
}

int main() {
    PaError err;

    // PortAudio 초기화
    err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio 초기화 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 입력 스트림 설정
    PaStream *stream;
    PaStreamParameters inputParams;
    inputParams.device = Pa_GetDefaultInputDevice();
    inputParams.channelCount = 1;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = nullptr;

    // 오디오 스트림 열기
    err = Pa_OpenStream(&stream, &inputParams, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, audioCallback, nullptr);
    if (err != paNoError) {
        std::cerr << "스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 스트림 시작
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    std::cout << "5초 동안 오디오 입력을 받습니다..." << std::endl;
    Pa_Sleep(5000);  // 5초 동안 녹음

    // 스트림 정지 및 닫기
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    std::cout << "오디오 데이터 개수: " << audioData.size() << std::endl;

    // x축 데이터 생성 (시간)
    std::vector<double> time(audioData.size());
    for (size_t i = 0; i < audioData.size(); i++) {
        time[i] = i / (double)SAMPLE_RATE;  // 초 단위 변환
    }

    // 그래프 그리기
    plt::plot(time, audioData);
    plt::title("Real-time Audio Input");
    plt::xlabel("Time (seconds)");
    plt::ylabel("Amplitude");
    plt::show();

    return 0;
}

