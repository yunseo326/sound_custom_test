#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <portaudio.h>
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
#define INPUT_DEVICE_ID 26  // 사용할 오디오 입력 장치 ID
#define WIDTH 800
#define HEIGHT 400

std::vector<float> audioBuffer(FRAMES_PER_BUFFER, 0.0f);  // 오디오 버퍼

// 오디오 콜백 함수 (실시간 데이터 저장)
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    (void)outputBuffer;
    (void)timeInfo;
    (void)statusFlags;

    float *data = (float *)inputBuffer;
    if (data != nullptr) {
        for (unsigned int i = 0; i < framesPerBuffer; i++) {
            audioBuffer[i] = data[i];
        }
    }
    return paContinue;
}

int main() {
    // PortAudio 초기화
    Pa_Initialize();
    PaStream *stream;
    PaStreamParameters inputParameters;

    inputParameters.device = INPUT_DEVICE_ID;
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(INPUT_DEVICE_ID)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    // PortAudio 스트림 시작
    Pa_OpenStream(&stream, &inputParameters, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, audioCallback, nullptr);
    Pa_StartStream(stream);

    // SFML 윈도우 생성
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Real-time Audio Waveform");
    sf::VertexArray waveform(sf::LineStrip, FRAMES_PER_BUFFER);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // 파형 업데이트
        for (unsigned int i = 0; i < FRAMES_PER_BUFFER; i++) {
            float sample = audioBuffer[i] * HEIGHT / 2.0f;  // 신호 크기 조정
            waveform[i].position = sf::Vector2f((float)i / FRAMES_PER_BUFFER * WIDTH, HEIGHT / 2 - sample);
            waveform[i].color = sf::Color::Green;
        }

        // 그래프 그리기
        window.clear();
        window.draw(waveform);
        window.display();
    }

    // 정리
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}


