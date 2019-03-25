#include "Detect_Black.hpp"

Detect_Black::Detect_Black(cv::Mat& input,std::vector<cv::Rect>& output,std::string winname,int hmany):
Detect_Color::Detect_Color(input,output,winname,hmany) {
#ifdef IMSHOW_WIMDOW
    cv::destroyWindow(Windowthre());
    cv::namedWindow(Windowthre(),0);
    cv::moveWindow(Windowthre(),200,600);
    create_trackbar();
#endif
    Iof()>>thre;
    Iof().close();
    find_ball_num = 0;
    find_ball = false;
}


Detect_Black::~Detect_Black() {
    record();
}


void Detect_Black::start() {
    cv::cvtColor(In_put(),Threshold(),CV_BGR2GRAY);

    cv::threshold(Threshold(),Threshold(),thre,255,CV_THRESH_BINARY_INV);
    Proc_Img(Threshold());
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(Threshold(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point> > convexHulls(contours.size());
    for(unsigned int i = 0; i < contours.size(); i++)
    {
        cv::convexHull(contours[i], convexHulls[i]);
    }

    std::vector<cv::Rect> rectangles;
    for(unsigned int i = 0; i < convexHulls.size(); i++)
    {
        rectangles.push_back(cv::boundingRect(convexHulls[i]));
    }
    std::sort(rectangles.begin(),rectangles.end(),comp_area);
    if(!find_ball) {
        if (rectangles.size() < Howmany()) {
            Vec_rect().assign(rectangles.begin(),rectangles.begin() + rectangles.size());
            while (Vec_rect().size() < Howmany()) {
                Vec_rect().push_back(cv::Rect());
            }
        } else {
            Vec_rect().assign(rectangles.begin(),rectangles.begin() + Howmany());
            find_ball = false;
        }
        find_ball_num++;
        if (find_ball_num > FINDBALLNUM) {
            find_ball = true;
        }
    }
#ifdef IMSHOW_WINDOW
    cv::imshow(Windowthre(),Threshold());
#endif
    //record();
}


void Detect_Black::create_trackbar() {
    cv::createTrackbar("H1:",Windowthre(),&thre,255,0,0);
}


void Detect_Black::record() {
    Iof().close();
    Iof().open("black_thre.txt");
    Iof()<<thre;
    std::cout<<"Black_thred has been recorded!"<<std::endl;
}