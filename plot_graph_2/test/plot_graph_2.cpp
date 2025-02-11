#include <iostream>
#include <vector>
#include <portaudio.h>
#include <matplotlibcpp.h>
#include <algorithm>
namespace plt = matplotlibcpp;


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

int main() {
    
    // PortAudio 초기화
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

    // 그래프 초기화
    plt::ion();  // 인터랙티브 모드 활성화
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

    // 실시간으로 오디오 데이터를 처리하고 그래프를 그리는 루프
    while (true) {
        try {
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

            // Find the index of the maximum value in all inputData arrays
            float* maxPtr1 = std::max_element(inputData1, inputData1+FRAMES_PER_BUFFER);
            float* maxPtr2 = std::max_element(inputData2, inputData2+FRAMES_PER_BUFFER);
            float* maxPtr3 = std::max_element(inputData3, inputData3+FRAMES_PER_BUFFER);
            float* maxPtr4 = std::max_element(inputData4, inputData4+FRAMES_PER_BUFFER);
            
            if (*maxPtr1 >= THRESHOLD && *maxPtr2 >= THRESHOLD && *maxPtr3 >= THRESHOLD && *maxPtr4 >= THRESHOLD){
                int maxIndex1 = std::distance(inputData1, maxPtr1);        
                int maxIndex2 = std::distance(inputData2, maxPtr2);
                int maxIndex3 = std::distance(inputData3, maxPtr3);
                int maxIndex4 = std::distance(inputData4, maxPtr4);

                // Print the index of the max values
                std::cout << "Max Value 1:   " << *maxPtr1 << "  index  "<< maxIndex1 <<std::endl;
                std::cout << "Max Value 2:   " << *maxPtr2 << "  index  "<< maxIndex2 <<std::endl;
                std::cout << "Max Value 3:   " << *maxPtr3 << "  index  "<< maxIndex3 <<std::endl;
                std::cout << "Max Value 4:   " << *maxPtr4 << "  index  "<< maxIndex4 <<std::endl;
                std::cout << "difference :   " << *maxPtr1 - *maxPtr2 << "  index  "<< maxIndex1 - maxIndex2 <<std::endl;
            }
            plt::pause(0.01);
        } catch (const std::runtime_error& e) {
            std::cerr << "Error during plotting: " << e.what() << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cerr << "Standard exception during plotting: " << e.what() << std::endl;
            break;
        } catch (...) {
            std::cerr << "Unknown error during plotting." << std::endl;
            break;
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

