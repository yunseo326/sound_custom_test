#include "CalDegree.hpp"

using namespace std;
using namespace CalDegree;

namespace CalDegree {
    AudioResult getAudioAngle(const vector<double> &a, const vector<double> &b, const vector<double> &c, const vector<double> &d) {
    
        AudioResult cal_result;
    
        double time_delay_1 = _gcc_phat(a, b) / SAMPLE_RATE;
        double time_delay_2 = _gcc_phat(b, c) / SAMPLE_RATE;
        double time_delay_3 = _gcc_phat(c, d) / SAMPLE_RATE;
        double time_delay_4 = _gcc_phat(d, a) / SAMPLE_RATE;
        
        double alpha = std::acos((SPEED_SOUND * time_delay_1) / DISTANCE_MIC) * 180.0 / M_PI;
        double beta  = std::acos((SPEED_SOUND * time_delay_2) / DISTANCE_MIC) * 180.0 / M_PI;
        double gamma = std::acos((SPEED_SOUND * time_delay_3) / DISTANCE_MIC) * 180.0 / M_PI;
        double omega = std::acos((SPEED_SOUND * time_delay_4) / DISTANCE_MIC) * 180.0 / M_PI;
        
        int direction = _categorize_values(alpha,beta,gamma,omega);
        cal_result = _calculate_8_angles(alpha,beta,gamma,omega,direction);
        cal_result.direction = direction;
        return cal_result;
    }
    
}

// 마이크 간 시간 차이 및 최종 방향 계산
