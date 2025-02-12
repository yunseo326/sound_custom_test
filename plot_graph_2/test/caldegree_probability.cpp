
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;
// 샘플레이트와 버퍼 크기
#define SAMPLE_RATE 44100

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
    cout << " " << cal_result.angle_1 << " " << cal_result.angle_2 << " " << cal_result.angle_3 << " " << cal_result.angle_4 << endl;

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
    cout << " " << detection_result.angle_1 << " " << detection_result.angle_2 << " " << detection_result.angle_3 << " " << detection_result.angle_4 << endl;

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

    // Default return value to prevent control from reaching end of non-void function
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
            cout << " " << case1.angle_1 << " " << case1.angle_2 << " " << case1.angle_3  << " " << case1.angle_4 << endl;
            cout << " " << case2.angle_1 << " " << case2.angle_2 << " " << case2.angle_3  << " " << case2.angle_4 << endl;

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

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

const double DISTANCE = 0.4;

using namespace std;

#include <iostream>
#include <vector>
#include <complex>
#include <fftw3.h>
using namespace std;

// FFTW에서 사용하는 복소수 타입을 정의
typedef complex<double> Complex;
typedef vector<Complex> CVector;

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




vector<vector<int>> make_mp3file(const vector<int>& mp3file, float angle, int direction, int direction2, vector<int>& random_list) {
    vector<vector<int>> file_list(4);
    vector<int> delay_list(4);

    file_list[0] = mp3file;

    int delay_1 = 0, delay_2 = 0, delay_3 = 0;

    if (direction == 1) {
        angle = angle * M_PI / 180;
        //std::cout << angle << std::endl;
        delay_1 = static_cast<int>(DISTANCE * sin(angle) / 340 * 44100) + random_list[0];
        vector<int> temp(delay_1, 0);
        temp.insert(temp.end(), mp3file.begin(), mp3file.end() - delay_1);
        file_list[1] = temp;

        delay_3 = static_cast<int>(DISTANCE * cos(angle) / 340 * 44100) + random_list[2];
        temp.assign(delay_3, 0);
        temp.insert(temp.end(), mp3file.begin(), mp3file.end() - delay_3);
        file_list[3] = temp;
    } else if (direction == 2) {
        angle = (angle - 90) * M_PI / 180;
        delay_1 = static_cast<int>(DISTANCE * cos(angle) / 340 * 44100) + random_list[0];
        vector<int> temp(delay_1, 0);
        temp.insert(temp.end(), mp3file.begin(), mp3file.end() - delay_1);
        file_list[1] = temp;

        delay_3 = static_cast<int>(DISTANCE * sin(angle) / 340 * 44100) + random_list[2];
        temp.assign(mp3file.begin() + delay_3, mp3file.end());
        temp.insert(temp.end(), delay_3, 0);
        file_list[3] = temp;
    } else if (direction == 3) {
        angle = (angle - 180) * M_PI / 180;
        delay_1 = static_cast<int>(DISTANCE * sin(angle) / 340 * 44100) + random_list[0];
        vector<int> temp(mp3file.begin() + delay_1, mp3file.end());
        temp.insert(temp.end(), delay_1, 0);
        file_list[1] = temp;

        delay_3 = static_cast<int>(DISTANCE * cos(angle) / 340 * 44100) + random_list[2];
        temp.assign(mp3file.begin() + delay_3, mp3file.end());
        temp.insert(temp.end(), delay_3, 0);
        file_list[3] = temp;
    } else if (direction == 4) {
        angle = (angle - 270) * M_PI / 180;
        delay_1 = static_cast<int>(DISTANCE * cos(angle) / 340 * 44100) + random_list[0];
        vector<int> temp(mp3file.begin() + delay_1, mp3file.end());
        temp.insert(temp.end(), delay_1, 0);
        file_list[1] = temp;

        delay_3 = static_cast<int>(DISTANCE * sin(angle) / 340 * 44100) + random_list[2];
        temp.assign(delay_3, 0);
        temp.insert(temp.end(), mp3file.begin(), mp3file.end() - delay_3);
        file_list[3] = temp;
    }

    if (direction2 >= 3 && direction2 <= 6) {
        if (direction2 == 3) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * sin(fabs(angle - M_PI / 4)) / 340 * 44100);
        } else if (direction2 == 4) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * cos(fabs(M_PI / 4 - angle)) / 340 * 44100);
        } else if (direction2 == 5) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * cos(fabs(angle - M_PI / 4)) / 340 * 44100);
        } else if (direction2 == 6) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * sin(fabs(M_PI / 4 - angle)) / 340 * 44100);
        }
        delay_2 += random_list[1];
        vector<int> temp(mp3file.begin() + delay_2, mp3file.end());
        temp.insert(temp.end(), delay_2, 0);
        file_list[2] = temp;
    } else {
        if (direction2 == 0) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * cos(fabs(M_PI / 4 - angle)) / 340 * 44100);
        } else if (direction2 == 1) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * cos(fabs(angle - M_PI / 4)) / 340 * 44100);
        } else if (direction2 == 2) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * sin(fabs(M_PI / 4 - angle)) / 340 * 44100);
        } else if (direction2 == 7) {
            delay_2 = static_cast<int>(DISTANCE * 1.4 * sin(fabs(angle - M_PI / 4)) / 340 * 44100);
        }
        delay_2 += random_list[1];
        vector<int> temp(delay_2, 0);
        temp.insert(temp.end(), mp3file.begin(), mp3file.end() - delay_2);
        file_list[2] = temp;
    }
    // delay_list[0] = delay_1;
    // delay_list[1] = delay_2;
    // delay_list[2] = delay_3;

    return file_list;
}

int main() {
    cout << "start" << endl;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-50000, 50000);
    vector<int> mp3file(10000);
    generate(mp3file.begin(), mp3file.end(), [&]() { return dis(gen); });

    vector<int> random_list = {0, 0, 0};
    int sample_rate = 44100;  // 샘플 레이트 (예시)
    for (int i = 1; i < 359; ++i) {
        vector<vector<int>> mp3_list = make_mp3file(mp3file, i, i / 90 + 1, i / 45, random_list);

        std::vector<double> vec1(mp3_list[0].begin(), mp3_list[0].end());
        std::vector<double> vec2(mp3_list[1].begin(), mp3_list[1].end());
        std::vector<double> vec3(mp3_list[2].begin(), mp3_list[2].end());
        std::vector<double> vec4(mp3_list[3].begin(), mp3_list[3].end());

        // Call your detection function here
        int delay1 = gcc_phat(vec1, vec2, sample_rate);
        int delay2 = gcc_phat(vec2, vec3, sample_rate);
        int delay3 = gcc_phat(vec3, vec4, sample_rate);
        int delay4 = gcc_phat(vec4, vec1, sample_rate);
                        
        AudioResult print_result;
        print_result = process_audio(delay1, delay2, delay3, delay4);

        // Example: direction, angle = detection.process_audio(mp3_list[0], mp3_list[1], mp3_list[2], mp3_list[3])
        cout << "Estimated delay: " << i << " " << print_result.angle_1 << " " << print_result.angle_2 << " " << print_result.angle_3 << " " << print_result.angle_4  << endl;

    }

    cout << "end" << endl;
    return 0;
}