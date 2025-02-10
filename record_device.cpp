#include <iostream>
#include <portaudio.h>
#define SAMPLE_RATE 44100  // 샘플링 레이트 (Hz)
#define FRAMES_PER_BUFFER 512  // 버퍼 크기
#define RECORD_SECONDS 5  
// 녹음 시간 (초)// 사용할 입력 장치 ID (장치 목록 출력 코드에서 확인한 값으로 설정)
#define INPUT_DEVICE_ID 20

// 오디오 콜백 함수
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    (void) outputBuffer; // 출력은 사용하지 않음
    (void) timeInfo;
    (void) statusFlags;

    // 입력 데이터를 출력 (간단한 디버깅 용도)
    const float* input = (const float*) inputBuffer;
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        std::cout << input[i] << " ";
    }
    std::cout << std::endl;

    return paContinue;
}

int main() {
    Pa_Initialize(); // PortAudio 초기화

    PaStream *stream;
    PaStreamParameters inputParameters;

    // 입력 장치 설정
    inputParameters.device = INPUT_DEVICE_ID;
    inputParameters.channelCount = 1; // 모노 입력
    inputParameters.sampleFormat = paFloat32; // 32비트 부동소수점 샘플
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(INPUT_DEVICE_ID)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    // 오디오 스트림 열기
    PaError err = Pa_OpenStream(&stream, &inputParameters, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, audioCallback, nullptr);
    if (err != paNoError) {
        std::cerr << "PortAudio 오류: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // 스트림 시작
    Pa_StartStream(stream);
    std::cout << "🎤 Recording... (" << RECORD_SECONDS << " seconds)" << std::endl;

    // 지정된 시간 동안 녹음
    Pa_Sleep(RECORD_SECONDS * 1000);

    // 스트림 종료
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    std::cout << "✅ Recording finished!" << std::endl;
    return 0;
}


