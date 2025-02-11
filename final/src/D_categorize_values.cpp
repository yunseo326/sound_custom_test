// 4분면 정하기
namespace CalDegree {
    int categorize_values(double value, double value2, double value3, double value4) {
        if (value < 90 && value3 > 90) {
            if (value2 < 90 && value4 >= 90)
                return 1;
            else if (value >= 90 && value3 < 90){
                return 2;
            }
            else 
                return 0;
        } else {
            if (value2 < 90 && value4 > 90)
                return 4;
            else if (value2 >= 90 && value3 < 90){
                return 3;
            }
            else
                return 0;
        }
    }
}

