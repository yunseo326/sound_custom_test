#include "Audio.hpp"

    int audioCallback1(const void *inputBuffer, void *outputBuffer,
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
     
    int audioCallback2(const void *inputBuffer, void *outputBuffer,
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

    int audioCallback3(const void *inputBuffer, void *outputBuffer,
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

    int audioCallback4(const void *inputBuffer, void *outputBuffer,
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