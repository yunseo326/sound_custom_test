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
#define FRAMES_PER_BUFFER 44100
#define THRESHOLD 0.05
#define DEVICE_ID1 0
#define DEVICE_ID2 1
#define DEVICE_ID3 2
#define DEVICE_ID4 3

// 네 개의 오디오 입력 스트림을 위한 변수
float inputData1[FRAMES_PER_BUFFER];
float inputData2[FRAMES_PER_BUFFER];
float inputData3[FRAMES_PER_BUFFER];
float inputData4[FRAMES_PER_BUFFER];


float savedata1[FRAMES_PER_BUFFER];
float savedata2[FRAMES_PER_BUFFER];
float savedata3[FRAMES_PER_BUFFER];
float savedata4[FRAMES_PER_BUFFER];


struct AudioResult{
	double angle_1;
	double angle_2;
	double angle_3;
	double angle_4;
	int direction;
};

struct arrau_struct{
	double values[4];
};


#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

const double DISTANCE_MIC = 0.4; // 마이크 간 거리
const double SPEED_SOUND = 343.0;


AudioResult calculate_8_angles(double alpha,double beta,double gamma,double omega, int direction,double frame1,double frame2,double frame3,double frame4);
int categorize_values(double value, double value2, double value3, double value4);
AudioResult detection(double angle1_1,double angle1_2,double angle2_1,double angle2_2,double angle3_1,double angle3_2,double angle4_1,double angle4_2);

// 마이크 간 시간 차이 및 최종 방향 계산
AudioResult process_audio(double frame1,double frame2,double frame3,double frame4) {
    AudioResult cal_result;

    double time_delay_1 = frame1 / SAMPLE_RATE;
    double time_delay_2 = frame2 / SAMPLE_RATE;
    double time_delay_3 = frame3 / SAMPLE_RATE;
    double time_delay_4 = frame4 / SAMPLE_RATE;
    
    
    double alpha = std::acos((SPEED_SOUND * time_delay_1) / DISTANCE_MIC) * 180.0 / M_PI;
    double beta  = std::acos((SPEED_SOUND * time_delay_2) / DISTANCE_MIC) * 180.0 / M_PI;
    double gamma = std::acos((SPEED_SOUND * time_delay_3) / DISTANCE_MIC) * 180.0 / M_PI;
    double omega = std::acos((SPEED_SOUND * time_delay_4) / DISTANCE_MIC) * 180.0 / M_PI;
    
    int direction = categorize_values(frame1,frame2,frame3,frame4);
    
    cal_result = calculate_8_angles(alpha,beta,gamma,omega,direction,frame1,frame2,frame3,frame4); 
    // cout << " " << cal_result.angle_1 << " " << cal_result.angle_2 << " " << cal_result.angle_3 << " " << cal_result.angle_4 << endl;
    cal_result.direction = direction;
    return cal_result;
}


// 마이크 간 시간 차이 및 최종 방향 계산
AudioResult detection(double angle1_1,double angle1_2,double angle2_1,double angle2_2,double angle3_1,double angle3_2,double angle4_1,double angle4_2) {
    
    AudioResult detection_result;
    vector<double> angle_case_1 = {angle1_1, angle2_1, angle3_1, angle4_1};
    vector<double> angle_case_2 = {angle1_2, angle2_2, angle3_2, angle4_2};
    double difference1 = 0;
    double difference2 = 0;
    vector<double> pair_1 = {0,0,0};
    vector<double> pair_2 = {0,0,0};

    vector<double> compare1 = {0,0,0};
    vector<double> compare2 = {0,0,0};
    arrau_struct array_angle;


    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (i != j ) {
                difference1 = abs(angle_case_1[i] - angle_case_1[j]);
                difference2 = abs(angle_case_1[i] - angle_case_2[j]);
                
                if (difference1 <= difference2) {
                    pair_1[j] = {difference1};
                }
                else{
                    pair_1[j] = {difference2};
                }
            }
        }

        for (size_t j = 0; j < 3; j++) {
            if (i != j ) {
                difference1 = abs(angle_case_2[i] - angle_case_1[j]);
                difference2 = abs(angle_case_2[i] - angle_case_2[j]);
                if (difference1 <= difference2) {
                    pair_2[j] = {difference1};
                }
                else{
                    pair_2[j] = {difference2};
                }
            }
        }
        sort(pair_1.begin(), pair_1.end());
        sort(pair_2.begin(), pair_2.end());
        
        if (pair_1[0] + pair_1[1] < pair_2[0] + pair_2[1]){
            array_angle.values[i] = angle_case_1[i];
        }
        else{
            array_angle.values[i] = angle_case_2[i];
        }
        
        
    }
    detection_result.angle_1 = array_angle.values[0];
    detection_result.angle_2 = array_angle.values[1];
    detection_result.angle_3 = array_angle.values[2];
    detection_result.angle_4 = array_angle.values[3];
    // cout << " " << detection_result.angle_1 << " " << detection_result.angle_2 << " " << detection_result.angle_3 << " " << detection_result.angle_4 << endl;

    return detection_result;

}


// 4분면 정하기
int categorize_values(double value, double value2, double value3, double value4) {
    // if (value >= 0 && value3 <= 0) {
    //     if (value2 >= 0 && value4 <= 0){
    //         return 1;
    //     } 
    //     else if (value2 <= 0 && value4 >= 0){
    //         return 2;
    //     } 
    //     else {
    //         return 0;
    //     }
    // } 
    // else if (value <= 0 && value3 >= 0) {
    //     if (value2 >= 0 && value4 <= 0) {
    //         return 4;
    //     } 
    //     else if (value2 <= 0 && value3 >= 0){
    //         return 3;
    //     } 
    //     else {
    //         return 0;
    //     }
    // }

    return 0;
}

// 초기 각도로부터 8개의 방향 각도 계산
AudioResult calculate_8_angles(double alpha,double beta,double gamma,double omega, int direction,double frame1,double frame2,double frame3,double frame4) {

    AudioResult result;
    AudioResult case1;
    AudioResult case2;
    
    switch (direction) {
        case 0:
            if (frame1 >= 0) {
                //1 or 2 
                    case1.angle_1 = 90-alpha; 
                    case2.angle_1 = 90+alpha;
                } 
            else {
                    //3 or 4
                    case1.angle_1 = 90+alpha;
                    case2.angle_1 = 450-alpha;
                }
            // frame 2
            case1.angle_2 = beta;
            case2.angle_2 = 360-beta;
                

            if (frame3 >= 0) {
                    //3 or 4
                    case1.angle_3 = 270-gamma;
                    case2.angle_3 = 270+gamma;
                } 
            else {
                    //1 or 2
                    case1.angle_3 = gamma-90; 
                    case2.angle_3 = 270-gamma;
                }
            
            // frame 4
            //2 or 3
            case1.angle_4 = 180-omega;
            case2.angle_4 = 180+omega;
            // cout << " " << case1.angle_1 << " " << case1.angle_2 << " " << case1.angle_3  << " " << case1.angle_4 << endl;
            // cout << " " << case2.angle_1 << " " << case2.angle_2 << " " << case2.angle_3  << " " << case2.angle_4 << endl;

            result = detection(case1.angle_1,case2.angle_1,case1.angle_2,case2.angle_2,case1.angle_3,case2.angle_3,case1.angle_4,case2.angle_4);
            break;


        case 1:
		result.angle_1 = 90-alpha; 
		result.angle_2 = beta;
		result.angle_3 = gamma-90; 
		result.angle_4 = 180-omega;
               break;
        case 2:
		result.angle_1 = 90+alpha;
		result.angle_2 = beta;
		result.angle_3 = 270-gamma;
		result.angle_4 = 180-omega;
               break;
        case 3:
		result.angle_1 = 90+alpha;
		result.angle_2 = 360-beta;
		result.angle_3 = 270-gamma;
		result.angle_4 = 180+omega;
               break;
        case 4:
		result.angle_1 = 450-alpha;
		result.angle_2 = 360-beta; 
		result.angle_3 = 270+gamma;
		result.angle_4 = 180+omega;
               break;
    }
    return result;
}




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
    // plt::ion();  // 인터랙티브 모드 활성화

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
        // 네 개의 입력 데이터에 대해 실시간 그래프 갱신
        // plt::clf();  // 그래프 초기화

        // // 첫 번째 마이크의 오디오 데이터를 그래프에 그리기
        // plt::subplot(4, 1, 1);
        // plt::plot(std::vector<float>(inputData1, inputData1 + FRAMES_PER_BUFFER));
        // plt::title("Input 1");
        
        // plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        // plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // // 두 번째 마이크의 오디오 데이터를 그래프에 그리기
        // plt::subplot(4, 1, 2);
        // plt::plot(std::vector<float>(inputData2, inputData2 + FRAMES_PER_BUFFER));
        // plt::title("Input 2");
        
        // plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        // plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // // 세 번째 마이크의 오디오 데이터를 그래프에 그리기
        // plt::subplot(4, 1, 3);
        // plt::plot(std::vector<float>(inputData3, inputData3 + FRAMES_PER_BUFFER));
        // plt::title("Input 3");
        
        // plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        // plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // // 네 번째 마이크의 오디오 데이터를 그래프에 그리기
        // plt::subplot(4, 1, 4);
        // plt::plot(std::vector<float>(inputData4, inputData4 + FRAMES_PER_BUFFER));
        // plt::title("Input 4");
        
        // plt::xlim(0, FRAMES_PER_BUFFER);   // x축 범위 설정
        // plt::ylim(-0.5, 0.5);              // y축 범위 설정

        // Find the index of the maximum value in all inputData arrays
        float* maxPtr1 = std::max_element(inputData1, inputData1+FRAMES_PER_BUFFER);
        float* maxPtr2 = std::max_element(inputData2, inputData2+FRAMES_PER_BUFFER);
        float* maxPtr3 = std::max_element(inputData3, inputData3+FRAMES_PER_BUFFER);
        float* maxPtr4 = std::max_element(inputData4, inputData4+FRAMES_PER_BUFFER);
        
	std::copy(std::begin(inputData1), std::end(inputData1), std::begin(savedata1));
	std::copy(std::begin(inputData2), std::end(inputData2), std::begin(savedata2));
	std::copy(std::begin(inputData3), std::end(inputData3), std::begin(savedata3));
	std::copy(std::begin(inputData4), std::end(inputData4), std::begin(savedata4));


        
        if (*maxPtr1 >= THRESHOLD && *maxPtr2 >= THRESHOLD && *maxPtr3 >= THRESHOLD && *maxPtr4 >= THRESHOLD){
            int maxIndex1 = std::distance(savedata1, maxPtr1);        
            int maxIndex2 = std::distance(savedata2, maxPtr2);
            int maxIndex3 = std::distance(savedata3, maxPtr3);
            int maxIndex4 = std::distance(savedata4, maxPtr4);

            // Print the index of the max values
            // std::cout << "Max Value 1:   " << *maxPtr1 << "  index  "<< maxIndex1 <<std::endl;
            // std::cout << "Max Value 2:   " << *maxPtr2 << "  index  "<< maxIndex2 <<std::endl;
            // std::cout << "Max Value 3:   " << *maxPtr3 << "  index  "<< maxIndex3 <<std::endl;
            // std::cout << "Max Value 4:   " << *maxPtr4 << "  index  "<< maxIndex4 <<std::endl;

            
            std::vector<double> vecinput1(savedata1, savedata1 + FRAMES_PER_BUFFER);
            std::vector<double> vecinput2(savedata2, savedata2 + FRAMES_PER_BUFFER);
            std::vector<double> vecinput3(savedata3, savedata3 + FRAMES_PER_BUFFER);
            std::vector<double> vecinput4(savedata4, savedata4 + FRAMES_PER_BUFFER);
            
            // GCC-PHAT를 이용하여 두 신호의 지연 계산
            int delay12 = gcc_phat(vecinput1, vecinput2, SAMPLE_RATE);
            int delay23 = gcc_phat(vecinput2, vecinput3, SAMPLE_RATE);
            int delay34 = gcc_phat(vecinput3, vecinput4, SAMPLE_RATE);
            int delay41 = gcc_phat(vecinput4, vecinput1, SAMPLE_RATE);

	     AudioResult print_result;
             print_result = process_audio(delay12, delay23, delay34, delay41);

            
	    if (1){
            // 지연 결과 출력
            	cout << "direction " << print_result.direction << " section" << endl;
            	cout << "Estimated degree between mic 1 and mic 2: " << print_result.angle_1 << " samples" << endl;
            	cout << "Estimated degree between mic 2 and mic 3: " << print_result.angle_2 << " samples" << endl;
            	cout << "Estimated degree between mic 3 and mic 4: " << print_result.angle_3 << " samples" << endl;
            	cout << "Estimated degree between mic 4 and mic 1: " << print_result.angle_4 << " samples" << endl;
                cout <<(print_result.angle_1 + print_result.angle_2 + print_result.angle_3 + print_result.angle_4) / 4.0 << " degree" << endl;
            }
        }
        
        // plt::pause(0.01);  // 잠시 대기 (그래프 갱신을 위한 시간 조정)
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