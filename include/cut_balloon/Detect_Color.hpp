#include "DEFINITIONS.hpp"
#include "functions.hpp"

class Detect_Color{
    public:
        Detect_Color(cv::Mat& input,std::vector<cv::Rect>& output,std::string winname = "Detect_Color",int hmany = 1);
        virtual void start(); 
        virtual ~Detect_Color();   
//        friend void Detect::im_show(Detect_Red& detect_red);
        //返回值函数
        std::string& Windowthre();
        int Howmany();
        int* Range_hsv6();
        std::fstream& Iof();
        cv::Mat& In_put();
        cv::Mat& Threshold();
        std::vector<cv::Rect>& Vec_rect();
    
        bool Find_Ball();
        void restart_find(int num = 3);
    private:
        void record();
        void create_trackbar();
        void create_window();
        static int N_DETECT_COLOR;
        const int DETECT_NUM;
        //std::string windowname;
        std::string windowthre;
        int howmany;
        int range_hsv[6];
        std::fstream iof;
        cv::Mat& in_put;
        cv::Mat threshold;
        std::vector<cv::Rect>& vec_rect_color;

        std::vector<cv::Point> first_rect_pos;
        std::vector<bool> now_has_point;
        unsigned int find_ball_num;
        bool find_ball;
        int match_up(cv::Point& point,std::vector<cv::Point>& map);
};