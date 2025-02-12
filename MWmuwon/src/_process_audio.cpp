#include "../include/CalDegree.hpp"
#include "../include/AudioInput.hpp"

using namespace std;
using namespace CalDegree;

struct asdf{
	double angle_1;
	double angle_2;
	double angle_3;
	double angle_4;
	int direction;
};




namespace CalDegree {
    


    // 마이크 간 시간 차이 및 최종 방향 계산
    asdf fixing(double frame1,double frame2,double frame3,double frame4) {
        asdf delay;
        if (frame1<105 || frame1>-105){
            if (frame1 < -51) { delay.angle_1 = frame1+51; }
            else if(frame1 > 51){ delay.angle_1 = frame1-51; }
            else {delay.angle_1 = 0;}
        }

        if (frame2<105 || frame2>-105){
            if (frame2 < -51) { delay.angle_2 = frame2+51; }
            else if(frame2 > 51){ delay.angle_2 = frame2-51; }
            else {delay.angle_2 = 0;}
        }

        if (frame3<105 || frame3>-105){
            if (frame3 < -51) { delay.angle_3 = frame3+51; }
            else if(frame3 > 51){ delay.angle_3 = frame3-51; }
            else {delay.angle_3 = 0;}
        }

        if (frame4<105 || frame4>-105){
            if (frame4 < -51) { delay.angle_4 = frame4+51; }
            else if(frame4 > 51){ delay.angle_4 = frame4-51; }
            else {delay.angle_4 = 0;}
        }
        


        return delay;
    }
    AudioResult getAudioAngle(const vector<double> &a, const vector<double> &b, const vector<double> &c, const vector<double> &d) {
    
        AudioResult cal_result;
        asdf fixingdelay;
        double frame1 = _gcc_phat(a, b);
        double frame2 = _gcc_phat(b, c);
        double frame3 = _gcc_phat(c, d);
        double frame4 = _gcc_phat(d, a);
        cout <<"frame: "<< frame1 << " " << frame2 << " " << frame3 << " " << frame4 << endl;
        fixingdelay = fixing(frame1,frame2,frame3,frame4);
        cout <<"delay: "<< fixingdelay.angle_1 << " " << fixingdelay.angle_2 << " " << fixingdelay.angle_3 << " " << fixingdelay.angle_4 << endl;

        double dist_diff_1 = frame1 / SAMPLE_RATE * SPEED_SOUND /  DISTANCE_MIC;
        double dist_diff_2 = frame2 / SAMPLE_RATE * SPEED_SOUND / DISTANCE_MIC;
        double dist_diff_3 = frame3 / SAMPLE_RATE * SPEED_SOUND / DISTANCE_MIC ;
        double dist_diff_4 = frame4 / SAMPLE_RATE * SPEED_SOUND / DISTANCE_MIC;
        if (dist_diff_1 < -1) dist_diff_1 = -1;
        if (dist_diff_2 < -1) dist_diff_2 = -1;
        if (dist_diff_3 < -1) dist_diff_3 = -1;
        if (dist_diff_4 < -1) dist_diff_4 = -1;
        if (dist_diff_1 > 1) dist_diff_1 = 1;
        if (dist_diff_2 > 1) dist_diff_2 = 1;
        if (dist_diff_3 > 1) dist_diff_3 = 1;
        if (dist_diff_4 > 1) dist_diff_4 = 1;

        double alpha = std::acos(dist_diff_1) * 180.0 / M_PI;
        double beta  = std::acos(dist_diff_2) * 180.0 / M_PI;
        double gamma = std::acos(dist_diff_3) * 180.0 / M_PI;
        double omega = std::acos(dist_diff_4) * 180.0 / M_PI;
        
        int direction = _categorize_values(frame1,frame2,frame3,frame4);
        cal_result = _calculate_8_angles(alpha,beta,gamma,omega,direction);
        cal_result.direction = direction;
        return cal_result;
    }
}

// 마이크 간 시간 차이 및 최종 방향 계산
