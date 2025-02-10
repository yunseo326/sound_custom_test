#include <iostream>
#include <vector>
#include <portaudio.h>
#include <algorithm>
#include <thread>


// 샘플레이트와 버퍼 크기
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 22050
#define THRESHOLD 0.05

// 두 개의 오디오 입력 스트림을 위한 변수
float inputData1[FRAMES_PER_BUFFER];
float inputData2[FRAMES_PER_BUFFER];

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


// 스트림 시작 (마이크 1, 마이크 2)
void startStream1(PaStream *stream1){
    PaError err = Pa_StartStream(stream1);
    if (err != paNoError) {
	std::cerr << "첫 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;

	}
}
void startStream2(PaStream *stream2){
    PaError err = Pa_StartStream(stream2);
    if (err != paNoError) {
	std::cerr << "두 번째 스트림 시작 실패: " << Pa_GetErrorText(err) << std::endl;

	}
}
    
int main() {
    // PortAudio 초기화
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio 초기화 실패: " << Pa_GetErrorText(err) << std::endl;
        return -1;
    }
    

    // 두 번째 오디오 장치에 대한 스트림 설정
    PaStream *stream1, *stream2;
    PaStreamParameters inputParameters1, inputParameters2;

    // 첫 번째 오디오 장치 설정 (마이크 1)
    inputParameters1.device = 28;  // 첫 번째 USB 마이크의 device ID
    inputParameters1.channelCount = 1;
    inputParameters1.sampleFormat = paFloat32;
    inputParameters1.suggestedLatency = Pa_GetDeviceInfo(inputParameters1.device)->defaultLowInputLatency;
    inputParameters1.hostApiSpecificStreamInfo = nullptr;

    // 두 번째 오디오 장치 설정 (마이크 2)
    inputParameters2.device = 30;  // 두 번째 USB 마이크의 device ID
    inputParameters2.channelCount = 1;
    inputParameters2.sampleFormat = paFloat32;
    inputParameters2.suggestedLatency = Pa_GetDeviceInfo(inputParameters2.device)->defaultLowInputLatency;
    inputParameters2.hostApiSpecificStreamInfo = nullptr;

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



    std::thread streamThread1(startStream1, stream1);
    std::thread streamThread2(startStream2, stream2);




    std::cout << "오디오 입력을 시작합니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;

    // 실시간으로 오디오 데이터를 처리하고 그래프를 그리는 루프
    while (true) {

        // Find the index of the maximum value in both inputData1 and inputData2
        float* maxPtr1 = std::max_element(inputData1, inputData1+FRAMES_PER_BUFFER);
        float* maxPtr2 = std::max_element(inputData2, inputData2+FRAMES_PER_BUFFER);
        
        if (*maxPtr1 >= THRESHOLD && *maxPtr2 >= THRESHOLD ){
		int maxIndex1 = std::distance(inputData1, maxPtr1);        
		int maxIndex2 = std::distance(inputData2, maxPtr2);

		// Print the index of the max values
		std::cout << "Max Value 1:   " << *maxPtr1 << "  index  "<< maxIndex1 <<std::endl;
		std::cout << "Max Value 2:   " << *maxPtr2 << "  index  "<< maxIndex2 <<std::endl;
		std::cout << "difference :   " << *maxPtr1 - *maxPtr2 << "  index  "<< maxIndex1 - maxIndex2-90 <<std::endl;
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

    Pa_Terminate();
    return 0;
}

