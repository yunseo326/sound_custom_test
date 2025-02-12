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
        // vector<int> mp3_list = make_mp3file(mp3file, i, i / 90 + 1, i / 45, random_list);

        std::vector<double> vec1(mp3_list[0].begin(), mp3_list[0].end());
        std::vector<double> vec2(mp3_list[1].begin(), mp3_list[1].end());
        std::vector<double> vec3(mp3_list[2].begin(), mp3_list[2].end());
        std::vector<double> vec4(mp3_list[3].begin(), mp3_list[3].end());

        // Call your detection function here
        int delay1 = gcc_phat(vec1, vec2, sample_rate);
        int delay2 = gcc_phat(vec2, vec3, sample_rate);
        int delay3 = gcc_phat(vec3, vec4, sample_rate);
        int delay4 = gcc_phat(vec4, vec1, sample_rate);
        // std::cout << i << " ";

        // for (int i = 0; i < 3; ++i) {
        //     std::cout << mp3_list[i] << " ";
        // }
        // std::cout << std::endl;
                        
        
        // Example: direction, angle = detection.process_audio(mp3_list[0], mp3_list[1], mp3_list[2], mp3_list[3])
        cout << "Estimated delay: " << delay1 << " " << delay2 << " " << delay3 << " " << delay4 << " samples" << endl;

    }

    cout << "end" << endl;
    return 0;
}