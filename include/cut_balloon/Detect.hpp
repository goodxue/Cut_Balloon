#include "DEFINITIONS.hpp"
#include "Camera.hpp"
#include "functions.hpp"
#include "Detect_Black.hpp"
#include "Detect_falling.hpp"

class Detect {
public:

    Detect();
    Detect(std::string& cam_para);
    void reinit(int num);
#ifdef DETECT_FALLING
    void Falling_pause();
#endif
    //~Detect();
    cv::Point2d detecting(std_msgs::Float64 altitude);                       //主函数
    cv::Point2d black_pos();
    cv::Point2d red_mid_n;
    cv::Point2d vect_yellow_mid_n[3];
    cv::Point2d black_mid_n;
    cv::Point2d red_mid;
    cv::Point2d vect_yellow_mid[3];
    cv::Point2d black_mid;

private:
    void im_show();
    //cv::Mat cv_image_show;
    cv::Mat cv_image_raw;
    sl::Mat image_depth_ocv;               //warning: cv_image_raw shared the same Mat with sl_image_left
    sl::Mat sl_image_left;
    sl::Mat sl_image_depth;
    sl::Mat sl_point_cloud;
    Camerax Cam;
    std::vector<cv::Rect> vect_rect_red;
    std::vector<cv::Rect> vect_rect_yellow;
    std::vector<cv::Rect> vect_rect_black;
    //cv::Rect rect_red;
    //cv::Rect rect_yellow [3];

    Detect_Color detect_red;
    Detect_Color detect_yellow;
    //Detect_Red detect_red;
    Detect_Black detect_black;
    //Detect_Yellow detect_yellow;
    char flag;
#ifdef DETECT_FALLING
    cv::Point2d Black_mid;
    int red_tf;
    int yellow1_tf;
    int yellow2_tf;
    int yellow3_tf;

    Detect_falling* f_red;
    Detect_falling* f_yellow1;
    Detect_falling* f_yellow2;
    Detect_falling* f_yellow3;

    bool has_fell_red;
    bool has_fell_yellow1;
    bool has_fell_yellow2;
    bool has_fell_yellow3;

    void init_detect_falling(int mode);
    int uav_tf(std_msgs::Float64 altitude);
    void record_tf();
    

#endif
    bool update_image();
    void update_mid();



    //flag vars
    bool falling_pause_flag;
};