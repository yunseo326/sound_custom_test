#include <portaudio.h>
#include <iostream>
#include <cmath>
#include <vector>

namespace Audio{
    #define SAMPLE_RATE 44100
    #define FRAMES_PER_BUFFER 44100
    #define THRESHOLD 0.05
    #define DEVICE_ID1 6
    #define DEVICE_ID2 7
    #define DEVICE_ID3 8
    #define DEVICE_ID4 9

    extern float inputData1[FRAMES_PER_BUFFER];
    extern float inputData2[FRAMES_PER_BUFFER];
    extern float inputData3[FRAMES_PER_BUFFER];
    extern float inputData4[FRAMES_PER_BUFFER];
    extern PaStream *stream1, *stream2, *stream3, *stream4;
    extern PaStreamParameters inputParameters1, inputParameters2, inputParameters3, inputParameters4;
    extern PaError err;

    void audioInit();

    int audioCallback1(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);

    int audioCallback2(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);

    int audioCallback3(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);

    int audioCallback4(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);
};
