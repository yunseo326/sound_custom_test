#include "Audio.hpp"
using namespace Audio;

namespace Audio{
    void audioInit(){
        inputParameters1.device = DEVICE_ID1;  // 첫 번째 USB 마이크의 device ID
        inputParameters1.channelCount = 1;
        inputParameters1.sampleFormat = paFloat32;
        inputParameters1.suggestedLatency = Pa_GetDeviceInfo(inputParameters1.device)->defaultLowInputLatency;
        inputParameters1.hostApiSpecificStreamInfo = nullptr;

        // 두 번째 오디오 장치 설정 (마이크 2)
        inputParameters2.device = DEVICE_ID2;  // 두 번째 USB 마이크의 device ID
        inputParameters2.channelCount = 1;
        inputParameters2.sampleFormat = paFloat32;
        inputParameters2.suggestedLatency = Pa_GetDeviceInfo(inputParameters2.device)->defaultLowInputLatency;
        inputParameters2.hostApiSpecificStreamInfo = nullptr;

        // 세 번째 오디오 장치 설정 (마이크 3)
        inputParameters3.device = DEVICE_ID3;  // 세 번째 USB 마이크의 device ID
        inputParameters3.channelCount = 1;
        inputParameters3.sampleFormat = paFloat32;
        inputParameters3.suggestedLatency = Pa_GetDeviceInfo(inputParameters3.device)->defaultLowInputLatency;
        inputParameters3.hostApiSpecificStreamInfo = nullptr;

        // 네 번째 오디오 장치 설정 (마이크 4)
        inputParameters4.device = DEVICE_ID4;  // 네 번째 USB 마이크의 device ID
        inputParameters4.channelCount = 1;
        inputParameters4.sampleFormat = paFloat32;
        inputParameters4.suggestedLatency = Pa_GetDeviceInfo(inputParameters4.device)->defaultLowInputLatency;
        inputParameters4.hostApiSpecificStreamInfo = nullptr;
        // 첫 번째 스트림 열기 (마이크 1)
        err = Pa_OpenStream(&stream1,
            &inputParameters1, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
            paClipOff, audioCallback1, nullptr);
        if (err != paNoError) {
        std::cerr << "첫 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        }

        // 두 번째 스트림 열기 (마이크 2)
        err = Pa_OpenStream(&stream2,
                &inputParameters2, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                paClipOff, audioCallback2, nullptr);
        if (err != paNoError) {
        std::cerr << "두 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        }

        // 세 번째 스트림 열기 (마이크 3)
        err = Pa_OpenStream(&stream3,
                &inputParameters3, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                paClipOff, audioCallback3, nullptr);
        if (err != paNoError) {
        std::cerr << "세 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        }

        // 네 번째 스트림 열기 (마이크 4)
        err = Pa_OpenStream(&stream4,
                &inputParameters4, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                paClipOff, audioCallback4, nullptr);
        if (err != paNoError) {
        std::cerr << "네 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        }
        std::cout << "오디오 입력을 시작합니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;
    }
}