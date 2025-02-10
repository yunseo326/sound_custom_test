
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

int main() {
    // 예시 신호 (두 개의 신호)
    vector<float> signal1 = {0.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f, 0.0f};  // 첫 번째 신호
    vector<float> signal2 = {0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 2.0f, 1.0f};  // 두 번째 신호 (지연된 버전)

    int sample_rate = 44100;  // 샘플 레이트 (예시)

    std::vector<double> double_data1(signal1.begin(),signal1.end());
    std::vector<double> double_data2(signal2.begin(),signal2.end());
    // GCC-PHAT를 이용하여 두 신호의 지연 계산
    int delay = gcc_phat(double_data1, double_data2, sample_rate);

    // 지연 결과 출력
    cout << "Estimated delay: " << delay << " samples" << endl;
    return 0;
}


