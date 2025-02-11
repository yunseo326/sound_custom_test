#include <iostream>
#include <vector>
#include <complex>
#include <fftw3.h>
using namespace std;

// FFTW에서 사용하는 복소수 타입을 정의
typedef complex<double> Complex;
typedef vector<Complex> CVector;


#include <portaudio.h>
#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

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


// 푸리에 변환 수행
void fft(CVector &signal, bool inverse = false) {
    int N = signal.size();
    fftw_plan plan;

    if (inverse) {
        // 역 푸리에 변환 (IFFT)
        plan = fftw_plan_dft_1d(N, reinterpret_cast<fftw_complex*>(signal.data()),
                                reinterpret_cast<fftw_complex*>(signal.data()), FFTW_BACKWARD, FFTW_ESTIMATE);
    } else {
        // 푸리에 변환 (FFT)
        plan = fftw_plan_dft_1d(N, reinterpret_cast<fftw_complex*>(signal.data()),
                                reinterpret_cast<fftw_complex*>(signal.data()), FFTW_FORWARD, FFTW_ESTIMATE);
    }

    fftw_execute(plan);  // 변환 수행
    fftw_destroy_plan(plan);
}

// GCC-PHAT 계산 함수
int gcc_phat(const vector<double> &x, const vector<double> &y, int sample_rate) {
    int N = x.size();

    // 입력 신호를 복소수로 변환 (정상적인 FFT를 위해)
    CVector X(N), Y(N);
    for (int i = 0; i < N; ++i) {
        X[i] = Complex(x[i], 0.0);
        Y[i] = Complex(y[i], 0.0);
    }

    // FFT 수행
    fft(X);
    fft(Y);

    // Cross Power Spectrum 계산 (PHAT 적용)
    CVector X_conj_Y(N);
    for (int i = 0; i < N; ++i) {
        X_conj_Y[i] = conj(X[i]) * Y[i] / abs(X[i]);  // PHAT: 위상 정보만 사용
    }

    // 역 FFT 수행하여 시간 도메인으로 변환
    fft(X_conj_Y, true);

    // 가장 큰 값을 찾음 (시간 지연)
    double max_corr = 0.0;
    int max_index = 0;
    for (int i = 0; i < N; ++i) {
        if (abs(X_conj_Y[i]) > max_corr) {
            max_corr = abs(X_conj_Y[i]);
            max_index = i;
        }
    }

    // 시간 지연 (초 단위로 반환)
    int delay_samples = max_index;
    if (delay_samples > N / 2) {
        delay_samples -= N;  // 시간 지연이 음수일 수 있기 때문
    }
    return delay_samples;
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
    inputParameters1.device = 22;  // 첫 번째 USB 마이크의 device ID
    inputParameters1.channelCount = 1;
    inputParameters1.sampleFormat = paFloat32;
    inputParameters1.suggestedLatency = Pa_GetDeviceInfo(inputParameters1.device)->defaultLowInputLatency;
    inputParameters1.hostApiSpecificStreamInfo = nullptr;

    // 두 번째 오디오 장치 설정 (마이크 2)
    inputParameters2.device = 24;  // 두 번째 USB 마이크의 device ID
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

    // 그래프 초기화
    plt::ion();  // 인터랙티브 모드 활성화

    // 스트림 시작 (마이크 1, 마이크 2)
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

    std::cout << "오디오 입력을 시작합니다. 종료하려면 Ctrl+C를 누르세요..." << std::endl;

    // 실시간으로 오디오 데이터를 처리하고 그래프를 그리는 루프
    while (true) {
        // 두 개의 입력 데이터에 대해 실시간 그래프 갱신
        plt::clf();  // 그래프 초기화

        // 첫 번째 마이크의 오디오 데이터를 그래프에 그리기
        plt::subplot(2, 1, 1);
        plt::plot(std::vector<float>(inputData1, inputData1 + FRAMES_PER_BUFFER));
        plt::title("Input 1");
        
        plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        plt::ylim(-0.5, 0.5);              // y축 범위 설정
        // 두 번째 마이크의 오디오 데이터를 그래프에 그리기
        plt::subplot(2, 1, 2);
        plt::plot(std::vector<float>(inputData2, inputData2 + FRAMES_PER_BUFFER));
        plt::title("Input 2");
        
        plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // Find the index of the maximum value in both inputData1 and inputData2
        float* maxPtr1 = std::max_element(inputData1, inputData1+FRAMES_PER_BUFFER);
        float* maxPtr2 = std::max_element(inputData2, inputData2+FRAMES_PER_BUFFER);
        
        if (*maxPtr1 >= THRESHOLD && *maxPtr2 >= THRESHOLD ){
		int maxIndex1 = std::distance(inputData1, maxPtr1);        
		int maxIndex2 = std::distance(inputData2, maxPtr2);

		// Print the index of the max values
		std::cout << "Max Value 1:   " << *maxPtr1 << "  index  "<< maxIndex1 <<std::endl;
		std::cout << "Max Value 2:   " << *maxPtr2 << "  index  "<< maxIndex2 <<std::endl;
		std::cout << "difference :   " << *maxPtr1 - *maxPtr2 << "  index  "<< maxIndex1 - maxIndex2 <<std::endl;
		
		std::vector<double> vecinput1(inputData1, inputData1 + FRAMES_PER_BUFFER);
		std::vector<double> vecinput2(inputData2, inputData2 + FRAMES_PER_BUFFER);
	        // GCC-PHAT를 이용하여 두 신호의 지연 계산
	        int delay = gcc_phat(vecinput1, vecinput2, SAMPLE_RATE);

	        // 지연 결과 출력
	        cout << "Estimated delay: " << delay << " samples" << endl;
	        
	        
		}
        plt::pause(0.01);  // 잠시 대기 (그래프 갱신을 위한 시간 조정)
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


















