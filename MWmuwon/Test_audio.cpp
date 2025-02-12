// src/main.cpp
#include "include/AudioInput.hpp"
#include <portaudio.h>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio 초기화 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 사용할 오디오 장치의 ID (환경에 맞게 수정)

    AudioInput mic1(DEVICE_ID1, SAMPLE_RATE, FRAMES_PER_BUFFER);
    AudioInput mic2(DEVICE_ID2, SAMPLE_RATE, FRAMES_PER_BUFFER);
    AudioInput mic3(DEVICE_ID3, SAMPLE_RATE, FRAMES_PER_BUFFER);
    AudioInput mic4(DEVICE_ID4, SAMPLE_RATE, FRAMES_PER_BUFFER);

    if (!mic1.openStream() || !mic2.openStream() ||
        !mic3.openStream() || !mic4.openStream()) {
        Pa_Terminate();
        return -1;
    }

    if (!mic1.startStream() || !mic2.startStream() ||
        !mic3.startStream() || !mic4.startStream()) {
        Pa_Terminate();
        return -1;
    }

    std::cout << "오디오 입력을 시작합니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;

    while (true) {
        const std::vector<float>& buffer1 = mic1.getBuffer();
        const std::vector<float>& buffer2 = mic2.getBuffer();
        const std::vector<float>& buffer3 = mic3.getBuffer();
        const std::vector<float>& buffer4 = mic4.getBuffer();

        float max1 = *std::max_element(buffer1.begin(), buffer1.end());
        float max2 = *std::max_element(buffer2.begin(), buffer2.end());
        float max3 = *std::max_element(buffer3.begin(), buffer3.end());
        float max4 = *std::max_element(buffer4.begin(), buffer4.end());
        std :: cout << "Max Value 1:   " << max1 << std::endl;
        std :: cout << "Max Value 2:   " << max2 << std::endl;
        std :: cout << "Max Value 3:   " << max3 << std::endl;
        std :: cout << "Max Value 4:   " << max4 << std::endl;

        if (max1 >= THRESHOLD && max2 >= THRESHOLD &&
            max3 >= THRESHOLD && max4 >= THRESHOLD) {
            std::vector<double> vec1(buffer1.begin(), buffer1.end());
            std::vector<double> vec2(buffer2.begin(), buffer2.end());
            std::vector<double> vec3(buffer3.begin(), buffer3.end());
            std::vector<double> vec4(buffer4.begin(), buffer4.end());

        }
    }

    // 종료 시 스트림 중지 및 PortAudio 종료
    mic1.stopStream();
    mic2.stopStream();
    mic3.stopStream();
    mic4.stopStream();
    Pa_Terminate();
    return 0;
}
