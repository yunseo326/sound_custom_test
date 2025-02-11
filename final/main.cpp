#include "src/CalDegree.hpp"
#include <iostream>
#include <algorithm>
#include <portaudio.h>
#include <cmath>
#include <vector>

using namespace CalDegree;
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 44100
#define THRESHOLD 0.05
#define DEVICE_ID1 5
#define DEVICE_ID2 6
#define DEVICE_ID3 7
#define DEVICE_ID4 8

float inputData1[FRAMES_PER_BUFFER];
float inputData2[FRAMES_PER_BUFFER];
float inputData3[FRAMES_PER_BUFFER];
float inputData4[FRAMES_PER_BUFFER];

float savedata1[FRAMES_PER_BUFFER];
float savedata2[FRAMES_PER_BUFFER];
float savedata3[FRAMES_PER_BUFFER];
float savedata4[FRAMES_PER_BUFFER];

static int audioCallback1(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
float *inBuffer = (float *)inputBuffer;

// 첫 번째 입력 (마이크 1)
for (unsigned long i = 0; i < framesPerBuffer; ++i) {
inputData1[i] = inBuffer[i];  // 첫 번째 채널
}

return paContinue;
}

static int audioCallback2(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
float *inBuffer = (float *)inputBuffer;

// 두 번째 입력 (마이크 2)
for (unsigned long i = 0; i < framesPerBuffer; ++i) {
inputData2[i] = inBuffer[i];  // 두 번째 채널
}

return paContinue;
}

static int audioCallback3(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
float *inBuffer = (float *)inputBuffer;

// 세 번째 입력 (마이크 3)
for (unsigned long i = 0; i < framesPerBuffer; ++i) {
inputData3[i] = inBuffer[i];  // 세 번째 채널
}

return paContinue;
}

static int audioCallback4(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
float *inBuffer = (float *)inputBuffer;

// 네 번째 입력 (마이크 4)
for (unsigned long i = 0; i < framesPerBuffer; ++i) {
inputData4[i] = inBuffer[i];  // 네 번째 채널
}

return paContinue;
}

int main(){
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio 초기화 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 네 개의 오디오 장치에 대한 스트림 설정
    PaStream *stream1, *stream2, *stream3, *stream4;
    PaStreamParameters inputParameters1, inputParameters2, inputParameters3, inputParameters4;

    // 첫 번째 오디오 장치 설정 (마이크 1)
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
        return -1;
    }

    // 두 번째 스트림 열기 (마이크 2)
    err = Pa_OpenStream(&stream2,
                        &inputParameters2, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, audioCallback2, nullptr);
    if (err != paNoError) {
        std::cerr << "두 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 세 번째 스트림 열기 (마이크 3)
    err = Pa_OpenStream(&stream3,
                        &inputParameters3, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, audioCallback3, nullptr);
    if (err != paNoError) {
        std::cerr << "세 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 네 번째 스트림 열기 (마이크 4)
    err = Pa_OpenStream(&stream4,
                        &inputParameters4, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, audioCallback4, nullptr);
    if (err != paNoError) {
        std::cerr << "네 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    // 스트림 시작 (마이크 1, 마이크 2, 마이크 3, 마이크 4)
    err = Pa_StartStream(stream1);
    if (err != paNoError) {
        std::cerr << "첫 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    err = Pa_StartStream(stream2);
    if (err != paNoError) {
        std::cerr << "두 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    err = Pa_StartStream(stream3);
    if (err != paNoError) {
        std::cerr << "세 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    err = Pa_StartStream(stream4);
    if (err != paNoError) {
        std::cerr << "네 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    std::cout << "오디오 입력을 시작합니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;

    while (true) {
        float* maxPtr1 = std::max_element(inputData1, inputData1+FRAMES_PER_BUFFER);
        float* maxPtr2 = std::max_element(inputData2, inputData2+FRAMES_PER_BUFFER);
        float* maxPtr3 = std::max_element(inputData3, inputData3+FRAMES_PER_BUFFER);
        float* maxPtr4 = std::max_element(inputData4, inputData4+FRAMES_PER_BUFFER);
        if (*maxPtr1 >= THRESHOLD && *maxPtr2 >= THRESHOLD && *maxPtr3 >= THRESHOLD && *maxPtr4 >= THRESHOLD){
            int maxIndex1 = std::distance(savedata1, maxPtr1);        
            int maxIndex2 = std::distance(savedata2, maxPtr2);
            int maxIndex3 = std::distance(savedata3, maxPtr3);
            int maxIndex4 = std::distance(savedata4, maxPtr4);
            
            std::vector<double> vecinput1(savedata1, savedata1 + FRAMES_PER_BUFFER);
            std::vector<double> vecinput2(savedata2, savedata2 + FRAMES_PER_BUFFER);
            std::vector<double> vecinput3(savedata3, savedata3 + FRAMES_PER_BUFFER);
            std::vector<double> vecinput4(savedata4, savedata4 + FRAMES_PER_BUFFER);

            AudioResult result = getAudioAngle(vecinput1, vecinput2, vecinput3, vecinput4);
            cout << "계산된 최종 방향: " << result.direction << " 도" << endl;
            cout << " 1번째 방향: " << result.angle_1 << " 도" << endl;
            cout << " 2번째 방향: " << result.angle_2 << " 도" << endl;
            cout << " 3번째 방향: " << result.angle_3 << " 도" << endl;
            cout << " 4번째 방향: " << result.angle_4 << " 도" << endl;
        }
    }
    // 스트림 종료
    err = Pa_StopStream(stream1);
    if (err != paNoError) {
        std::cerr << "첫 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_StopStream(stream2);
    if (err != paNoError) {
        std::cerr << "두 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_StopStream(stream3);
    if (err != paNoError) {
        std::cerr << "세 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_StopStream(stream4);
    if (err != paNoError) {
        std::cerr << "네 번째 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }

    Pa_Terminate();
return 0;
}
