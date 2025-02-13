#include "include/CalDegree.hpp"
#include <iostream>
#include <algorithm>
#include <portaudio.h>
#include <cmath>
#include <vector>

#include <thread>
#include <chrono>

using namespace CalDegree;
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 44100*2
#define THRESHOLD 0.3
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
        cout << 1.1 << endl;
        float *inBuffer = (float *)inputBuffer;
        cout << 1.2 << endl;
// 첫 번째 입력 (마이크 1)
for (unsigned long i = 0; i < framesPerBuffer; ++i) {
inputData1[i] = inBuffer[i];  // 첫 번째 채널
}
cout << 1.3 << endl;

return paContinue;
}

static int audioCallback2(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
        cout << 2.1 << endl;
        float *inBuffer = (float *)inputBuffer;
        cout << 2.2 << endl;
        // 두 번째 입력 (마이크 2)
        for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        inputData2[i] = inBuffer[i];  // 두 번째 채널
        }
        cout << 2.3 << endl;

        return paContinue;
    }

static int audioCallback3(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
        cout << 3.1 << endl;
        float *inBuffer = (float *)inputBuffer;
        cout << 3.2 << endl;
        // 세 번째 입력 (마이크 3)
        for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        inputData3[i] = inBuffer[i];  // 세 번째 채널
    }
    cout << 3.3 << endl;


return paContinue;
}

static int audioCallback4(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData) {
        cout << 4.1 << endl;
        float *inBuffer = (float *)inputBuffer;
        cout << 4.2 << endl;
        // 네 번째 입력 (마이크 4)
        for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        inputData4[i] = inBuffer[i];  // 네 번째 채널
    }
    cout << 4.3 << endl;


return paContinue;
}



// 네 개의 오디오 장치에 대한 스트림 설정
PaStream *stream1, *stream2, *stream3, *stream4;
PaStreamParameters inputParameters1, inputParameters2, inputParameters3, inputParameters4;

// 🎵 첫 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio1() {
    // cout << 1.1 << endl;

    float buffer[FRAMES_PER_BUFFER];
    // cout << 1.2 << endl;

    PaError err = Pa_ReadStream(stream1, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // cout << 1.3 << endl;
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData1);

    // 읽은 데이터를 저장 (첫 번째 버퍼에)
    // inputData1.insert(audioBuffer1.end(), buffer, buffer + FRAMES_PER_BUFFER);
    }

// 🎵 두 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio2() {
    // cout << 2.1 << endl;
    float buffer[FRAMES_PER_BUFFER];
    // cout << 2.2 << endl;

    PaError err = Pa_ReadStream(stream2, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // cout << 2.3 << endl;

    // 읽은 데이터를 저장 (두 번째 버퍼에)
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData2);
    }

// 🎵 세 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio3() {
    // cout << 3.1 << endl;

    float buffer[FRAMES_PER_BUFFER];
    // cout << 3.2 << endl;

    PaError err = Pa_ReadStream(stream3, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // cout << 3.3 << endl;

    // 읽은 데이터를 저장 (세 번째 버퍼에)
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData3);
    }

// 🎵 네 번째 오디오 입력을 읽는 함수 (스레드에서 실행)
void captureAudio4() {
    // cout << 4.1 << endl;
    float buffer[FRAMES_PER_BUFFER];
    // cout << 4.2 << endl;

    PaError err = Pa_ReadStream(stream4, buffer, FRAMES_PER_BUFFER);
    if (err != paNoError) {
        std::cerr << "Pa_ReadStream error: " << Pa_GetErrorText(err) << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // cout << 4.3 << endl;

    // 읽은 데이터를 저장 (네 번째 버퍼에)
    std::copy(buffer, buffer + FRAMES_PER_BUFFER, inputData4);
    }

int main(){
    int EN = 0;

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio 초기화 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    
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
                        paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cerr << "첫 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 두 번째 스트림 열기 (마이크 2)
    err = Pa_OpenStream(&stream2,
                        &inputParameters2, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cerr << "두 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 세 번째 스트림 열기 (마이크 3)
    err = Pa_OpenStream(&stream3,
                        &inputParameters3, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, nullptr, nullptr);
    if (err != paNoError) {
        std::cerr << "세 번째 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }

    // 네 번째 스트림 열기 (마이크 4)
    err = Pa_OpenStream(&stream4,
                        &inputParameters4, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, nullptr, nullptr);
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
    // plt::ion();  // 인터랙티브 모드 활성화

    while (true) {

        // 🎯 첫 번째 오디오 입력을 받을 스레드 실행
        std::thread captureThread1(captureAudio1);

        // 🎯 두 번째 오디오 입력을 받을 스레드 실행
        std::thread captureThread2(captureAudio2);

        // 🎯 세 번째 오디오 입력을 받을 스레드 실행
        std::thread captureThread3(captureAudio3);

        // 🎯 네 번째 오디오 입력을 받을 스레드 실행
        std::thread captureThread4(captureAudio4);

        captureThread1.join();
        captureThread2.join();
        captureThread3.join();
        captureThread4.join();
        /*
        plt::clf();  // 그래프 초기화

        // 첫 번째 마이크의 오디오 데이터를 그래프에 그리기
        plt::subplot(4, 1, 1);
        plt::plot(std::vector<float>(inputData1, inputData1 + FRAMES_PER_BUFFER));
        plt::title("Input 1");
        
        plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // 두 번째 마이크의 오디오 데이터를 그래프에 그리기
        plt::subplot(4, 1, 2);
        plt::plot(std::vector<float>(inputData2, inputData2 + FRAMES_PER_BUFFER));
        plt::title("Input 2");
        
        plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // 세 번째 마이크의 오디오 데이터를 그래프에 그리기
        plt::subplot(4, 1, 3);
        plt::plot(std::vector<float>(inputData3, inputData3 + FRAMES_PER_BUFFER));
        plt::title("Input 3");
        
        plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // 네 번째 마이크의 오디오 데이터를 그래프에 그리기
        plt::subplot(4, 1, 4);
        plt::plot(std::vector<float>(inputData4, inputData4 + FRAMES_PER_BUFFER));
        plt::title("Input 4");
        
        plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        plt::ylim(-0.5, 0.5);              // y축 범위 설정
        */

        
        
        // Copy the live input data into savedata arrays.
        std::copy(inputData1, inputData1 + FRAMES_PER_BUFFER, savedata1);
        std::copy(inputData2, inputData2 + FRAMES_PER_BUFFER, savedata2);
        std::copy(inputData3, inputData3 + FRAMES_PER_BUFFER, savedata3);
        std::copy(inputData4, inputData4 + FRAMES_PER_BUFFER, savedata4);

        float* maxPtr1 = std::max_element(savedata1, savedata1+FRAMES_PER_BUFFER);
        float* maxPtr2 = std::max_element(savedata2, savedata2+FRAMES_PER_BUFFER);
        float* maxPtr3 = std::max_element(savedata3, savedata3+FRAMES_PER_BUFFER);
        float* maxPtr4 = std::max_element(savedata4, savedata4+FRAMES_PER_BUFFER);

        if (*maxPtr1 >= THRESHOLD && *maxPtr2 >= THRESHOLD && *maxPtr3 >= THRESHOLD && *maxPtr4 >= THRESHOLD){
            // EN = EN + 1;
            // if (EN == 100){
                // int maxIndex1 = std::distance(savedata1, maxPtr1);        
                // int maxIndex2 = std::distance(savedata2, maxPtr2);
                // int maxIndex3 = std::distance(savedata3, maxPtr3);
                // int maxIndex4 = std::distance(savedata4, maxPtr4);
                // cout <<"maxindex: "<< maxIndex1 - maxIndex2 << " " << maxIndex2 - maxIndex3 << " " << maxIndex3 - maxIndex4 << " " << maxIndex4 - maxIndex1 << endl;
                
                std::vector<double> vecinput1(savedata1, savedata1 + FRAMES_PER_BUFFER);
                std::vector<double> vecinput2(savedata2, savedata2 + FRAMES_PER_BUFFER);
                std::vector<double> vecinput3(savedata3, savedata3 + FRAMES_PER_BUFFER);
                std::vector<double> vecinput4(savedata4, savedata4 + FRAMES_PER_BUFFER);
                    
                AudioResult result = getAudioAngle(vecinput1, vecinput2, vecinput3, vecinput4);

                cout << "계산된 사분면: " << result.direction << endl;
                // cout << " 1번째 방향: " << result.angle_1  << " 도 " << endl;
                // cout << " 2번째 방향: " << result.angle_2  << " 도 " << endl;
                // cout << " 3번째 방향: " << result.angle_3  << " 도 " << endl;
                // cout << " 4번째 방향: " << result.angle_4  << " 도" << endl;
                cout << "최종 방향: " << (result.angle_1 + result.angle_2 + result.angle_3 + result.angle_4) / 4.0 << " 도" << endl;
                cout << endl;
            
        }
        else {
            EN = 0;
        }
        // plt::pause(0.01);  // 잠시 대기 (그래프 갱신을 위한 시간 조정)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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