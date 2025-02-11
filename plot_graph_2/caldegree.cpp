#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

const double DISTANCE_MIC = 0.4; // 마이크 간 거리
const double SPEED_SOUND = 343.0;
const int SAMPLE_RATE = 44100;

double calculate_8_angles(double alpha, int direction);

// 마이크 간 시간 차이 및 최종 방향 계산
double process_audio(double frame, int direction) {
    double time_delay_1 = frame / SAMPLE_RATE;
    double alpha = std::acos((SPEED_SOUND * time_delay_1) / DISTANCE_MIC) * 180.0 / M_PI;
    double alpha_2 = calculate_8_angles(alpha, direction);
    return alpha_2;
}

// 4분면 정하기
int categorize_values(double value, double value2, double value3, double value4) {
    if (value < 90 && value3 > 90) {
        if (value2 < 90 && value4 > 90)
            return 1;
        else
            return 2;
    } else {
        if (value2 < 90 && value4 > 90)
            return 4;
        else
            return 3;
    }
}

// 초기 각도로부터 8개의 방향 각도 계산
double calculate_8_angles(double alpha, int direction) {
    double phi_alpha1;
    switch (direction) {
        case 1:
            phi_alpha1 = 90 - alpha;
            break;
        case 2:
        case 3:
            phi_alpha1 = 90 + alpha;
            break;
        case 4:
            phi_alpha1 = 450 - alpha;
            break;
        default:
            phi_alpha1 = 0; // 예외 처리
            break;
    }
    return phi_alpha1;
}

// 6개의 각도 중 가장 적은 오차를 보이는 두 각도를 선택하여 평균 계산
std::pair<double, double> select_final_direction(const std::vector<double>& angles) {
    double min_difference = std::numeric_limits<double>::infinity();
    std::pair<double, double> best_pair = {0, 0};
    
    for (size_t i = 0; i < angles.size(); i++) {
        for (size_t j = i + 1; j < angles.size(); j++) {
            double difference = std::abs(angles[i] - angles[j]);
            if (difference < min_difference) {
                min_difference = difference;
                best_pair = {angles[i], angles[j]};
            }
        }
    }
    return best_pair;
}

int main() {
    double frame = -49; // 예제 입력값
    int direction = 4; // 예제 입력값
    double result = process_audio(frame, direction);
    std::cout << "Calculated angle: " << result << " degrees" << std::endl;
    return 0;
}