#include "CalDegree.hpp"

using namespace std;
using namespace CalDegree;
int i, j, k, l, one=0;
namespace CalDegree {
    AudioResult getAudioAngle(const vector<double> &a, const vector<double> &b, const vector<double> &c, const vector<double> &d, int sample_rate) {
    	
        AudioResult cal_result;
    	
        double time_delay_1 = _gcc_phat(a, b, sample_rate) / SAMPLE_RATE;
        double time_delay_2 = _gcc_phat(b, c, sample_rate) / SAMPLE_RATE;
        double time_delay_3 = _gcc_phat(c, d, sample_rate) / SAMPLE_RATE;
        double time_delay_4 = _gcc_phat(d, a, sample_rate) / SAMPLE_RATE;
        if (one==0){
        one++;
        i=time_delay_1;
        j=time_delay_2;
        k=time_delay_3;
        l=time_delay_4;
        }
        int i1, j1, k1, l1;
        if (time_delay_1>51){
        i1=time_delay_1-51;
        i=i-i1;
        } else if(time_delay_1<-51){
        i1=time_delay_1+51;
        i=i+i1;
        }
        if (time_delay_2>51){
        j1=time_delay_2-51;
        j=j-j1;
        } else if(time_delay_2<-51){
        j1=time_delay_2+51;
        j=j+j1;
        }
        if (time_delay_3>51){
        k1=time_delay_3-51;
        k=k-k1;
        } else if(time_delay_3<-51){
        k1=time_delay_3+51;
        k=k+k1;
        }
        if (time_delay_4>51){
        l1=time_delay_4-51;
        l=l-l1;
        } else if(time_delay_4<-51){
        l1=time_delay_4+51;
        l=l+l1;
        }
	double value1 = (SPEED_SOUND * i) / DISTANCE_MIC;
	double value2 = (SPEED_SOUND * j) / DISTANCE_MIC;
	double value3 = (SPEED_SOUND * k) / DISTANCE_MIC;
	double value4 = (SPEED_SOUND * l) / DISTANCE_MIC;
	value1 = std::max(-1.0, std::min(1.0, value));
	value2 = std::max(-1.0, std::min(1.0, value));
	value3 = std::max(-1.0, std::min(1.0, value));
	value4 = std::max(-1.0, std::min(1.0, value));
        double alpha = std::acos(value1) * 180.0 / M_PI;
        double beta  = std::acos(value2) * 180.0 / M_PI;
        double gamma = std::acos(value3) * 180.0 / M_PI;
        double omega = std::acos(value4) * 180.0 / M_PI;
        
        int direction = _categorize_values(alpha,beta,gamma,omega);
        cal_result = _calculate_8_angles(alpha,beta,gamma,omega,direction);
        cal_result.direction = direction;
        return cal_result;
    }
    
}

// 마이크 간 시간 차이 및 최종 방향 계산
