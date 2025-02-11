#include <iostream>
#include <vector>
#include <portaudio.h>
#include <algorithm>
#include <thread>

// 샘플레이트와 버퍼 크기
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 22050
#define THRESHOLD 0.05
#define DEVICE_ID1 5
#define DEVICE_ID2 6
#define DEVICE_ID3 7
#define DEVICE_ID4 8

// 네 개의 오디오 입력 스트림을 위한 변수
float inputData1[FRAMES_PER_BUFFER];
float inputData2[FRAMES_PER_BUFFER];
float inputData3[FRAMES_PER_BUFFER];
float inputData4[FRAMES_PER_BUFFER];

// 콜백 함수: 오디오 데이터를 실시간으로 읽고 처리하는 부분
static int audioCallback1(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
    float *inBuffer = (float *)inputBuffer;
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        inputData1[i] = inBuffer[i];
    }
    return paContinue;
}

static int audioCallback2(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
    float *inBuffer = (float *)inputBuffer;
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        inputData2[i] = inBuffer[i];
    }
    return paContinue;
}

static int audioCallback3(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
    float *inBuffer = (float *)inputBuffer;
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        inputData3[i] = inBuffer[i];
    }
    return paContinue;
}

static int audioCallback4(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {
    float *inBuffer = (float *)inputBuffer;
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        inputData4[i] = inBuffer[i];
    }
    return paContinue;
}

// 스트림 시작 함수
void startStream(PaStream *stream, const char* name){
    PaError err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << name << " 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;
    }
}
    
int main() {
    // PortAudio 초기화
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio 초기화 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    
    // 네 개의 오디오 스트림 및 파라미터 설정
    PaStream *stream1, *stream2, *stream3, *stream4;
    PaStreamParameters inputParameters1, inputParameters2, inputParameters3, inputParameters4;

    // 마이크 1 설정
    inputParameters1.device = DEVICE_ID1;
    inputParameters1.channelCount = 1;
    inputParameters1.sampleFormat = paFloat32;
    inputParameters1.suggestedLatency = Pa_GetDeviceInfo(inputParameters1.device)->defaultLowInputLatency;
    inputParameters1.hostApiSpecificStreamInfo = nullptr;
    
    // 마이크 2 설정
    inputParameters2.device = DEVICE_ID2;
    inputParameters2.channelCount = 1;
    inputParameters2.sampleFormat = paFloat32;
    inputParameters2.suggestedLatency = Pa_GetDeviceInfo(inputParameters2.device)->defaultLowInputLatency;
    inputParameters2.hostApiSpecificStreamInfo = nullptr;
    
    // 마이크 3 설정
    inputParameters3.device = DEVICE_ID3;
    inputParameters3.channelCount = 1;
    inputParameters3.sampleFormat = paFloat32;
    inputParameters3.suggestedLatency = Pa_GetDeviceInfo(inputParameters3.device)->defaultLowInputLatency;
    inputParameters3.hostApiSpecificStreamInfo = nullptr;
    
    // 마이크 4 설정
    inputParameters4.device = DEVICE_ID4;
    inputParameters4.channelCount = 1;
    inputParameters4.sampleFormat = paFloat32;
    inputParameters4.suggestedLatency = Pa_GetDeviceInfo(inputParameters4.device)->defaultLowInputLatency;
    inputParameters4.hostApiSpecificStreamInfo = nullptr;
    
    // 스트림 열기
    err = Pa_OpenStream(&stream1,
                        &inputParameters1, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, audioCallback1, nullptr);
    if (err != paNoError) {
        std::cerr << "마이크 1 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    
    err = Pa_OpenStream(&stream2,
                        &inputParameters2, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, audioCallback2, nullptr);
    if (err != paNoError) {
        std::cerr << "마이크 2 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    
    err = Pa_OpenStream(&stream3,
                        &inputParameters3, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, audioCallback3, nullptr);
    if (err != paNoError) {
        std::cerr << "마이크 3 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    
    err = Pa_OpenStream(&stream4,
                        &inputParameters4, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER,
                        paClipOff, audioCallback4, nullptr);
    if (err != paNoError) {
        std::cerr << "마이크 4 스트림 열기 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    
    // 각 스트림 시작을 위한 스레드 생성
    std::thread t1(startStream, stream1, "마이크 1");
    std::thread t2(startStream, stream2, "마이크 2");
    std::thread t3(startStream, stream3, "마이크 3");
    std::thread t4(startStream, stream4, "마이크 4");
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    std::cout << "오디오 입력을 시작합니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;
    
    // 실시간으로 오디오 데이터 처리 (예시: 각 마이크의 최대값 출력)
    while (true) {
        float* maxPtr1 = std::max_element(inputData1, inputData1 + FRAMES_PER_BUFFER);
        float* maxPtr2 = std::max_element(inputData2, inputData2 + FRAMES_PER_BUFFER);
        float* maxPtr3 = std::max_element(inputData3, inputData3 + FRAMES_PER_BUFFER);
        float* maxPtr4 = std::max_element(inputData4, inputData4 + FRAMES_PER_BUFFER);
        
        if (*maxPtr1 >= THRESHOLD || *maxPtr2 >= THRESHOLD ||
            *maxPtr3 >= THRESHOLD || *maxPtr4 >= THRESHOLD) {
                
            int maxIndex1 = std::distance(inputData1, maxPtr1);
            int maxIndex2 = std::distance(inputData2, maxPtr2);
            int maxIndex3 = std::distance(inputData3, maxPtr3);
            int maxIndex4 = std::distance(inputData4, maxPtr4);
            
            std::cout << "마이크1 - 최대값: " << *maxPtr1 << " (index " << maxIndex1 << ")" << std::endl;
            std::cout << "마이크2 - 최대값: " << *maxPtr2 << " (index " << maxIndex2 << ")" << std::endl;
            std::cout << "마이크3 - 최대값: " << *maxPtr3 << " (index " << maxIndex3 << ")" << std::endl;
            std::cout << "마이크4 - 최대값: " << *maxPtr4 << " (index " << maxIndex4 << ")" << std::endl;
            std::cout << "-------------------------" << std::endl;
        }
    }
    
    // 스트림 종료 코드 (프로그램 종료 시 도달하지 않음)
    err = Pa_StopStream(stream1);
    if (err != paNoError) {
        std::cerr << "마이크 1 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }
    err = Pa_StopStream(stream2);
    if (err != paNoError) {
        std::cerr << "마이크 2 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }
    err = Pa_StopStream(stream3);
    if (err != paNoError) {
        std::cerr << "마이크 3 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }
    err = Pa_StopStream(stream4);
    if (err != paNoError) {
        std::cerr << "마이크 4 스트림 중지 실패: " << Pa_GetErrorText(err) << std::endl;
    }
    
    Pa_Terminate();
    return 0;
}

