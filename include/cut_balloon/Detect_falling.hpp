#include "DEFINITIONS.hpp"
#include <queue>

#define VEC_ALTITUDE 50
#define VEC_DEPTH 50
#define FALLING 40.0


class Detect_falling {
public:
    //Detect_falling();
    Detect_falling(sl::Mat& image_in,int mode = 1);
    bool start(cv::Point2d& point,float altitude);
    bool fell;
    float fall;
    float depth_temp;
    void Falling_pause();
private:

    const int MODE;
    sl::Mat& image_input;
    std::queue<float> que_depth;
    //int vec_depth_size;
    std::queue<float> que_altitude;
    //int vec_altitude_size;
    cv::Point2d a_point;
    bool is_falling();
    bool falling_pause;
};