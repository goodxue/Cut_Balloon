#include "Detect_Color.hpp"

// class Detect_Black{
// public:
//     Detect_Black(cv::Mat& input,std::vector<cv::Rect>& output);
//     void start
// private:
//     std::string windowname;
//     int thre_range;
//     std::fstream iof;
//     cv::Mat& in_put;
//     cv::Mat threshold;
//     std::vector<cv::Rect>& vec_rect_color;
// };
class Detect_Black:public Detect_Color {
public:
    Detect_Black(cv::Mat& input,std::vector<cv::Rect>& output,std::string winname = "Detect_Color",int hmany = 1);
    void start();
    virtual ~Detect_Black();
private:
    int thre;
    unsigned int find_ball_num;
    bool find_ball;
    void record();
    void create_trackbar();
};