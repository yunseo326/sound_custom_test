#include <iostream>
#include <portaudio.h>

#define SAMPLE_RATE 44100  // 샘플링 주파수
#define FRAMES_PER_BUFFER 512  // 버퍼 크기

// 콜백 함수 (오디오 데이터를 입력받음)
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    // 입력 데이터를 처리 (여기서는 콘솔에 표시)
    const float* in = (const float*)inputBuffer;
    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        std::cout << in[i] << std::endl;  // 오디오 샘플 출력
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

    std::cout << "오디오 입력을 받는 중입니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;
    Pa_Sleep(5000);  // 5초 동안 실행

    // 스트림 정지 및 닫기
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    std::cout << "프로그램 종료" << std::endl;
    return 0;
}

