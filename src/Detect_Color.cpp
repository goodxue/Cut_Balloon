#include "Detect_Color.hpp"

int Detect_Color::N_DETECT_COLOR = 0;

void Detect_Color::create_window() {
    // cv::namedWindow(windowname,0);
    // cv::moveWindow(windowname,0,0);
    cv::namedWindow(windowthre,0);
    cv::moveWindow(windowthre,0 + DETECT_NUM * 600,0);
}


void Detect_Color::create_trackbar() {
    cv::createTrackbar("H1:",windowthre,&range_hsv[0],180,0,0);
    cv::createTrackbar("S1:",windowthre,&range_hsv[1],255,0,0);
    cv::createTrackbar("V1:",windowthre,&range_hsv[2],255,0,0);
    cv::createTrackbar("H2:",windowthre,&range_hsv[3],180,0,0);
    cv::createTrackbar("S2:",windowthre,&range_hsv[4],255,0,0);
    cv::createTrackbar("V2:",windowthre,&range_hsv[5],255,0,0);
}


Detect_Color::Detect_Color (cv::Mat& input,std::vector<cv::Rect>& output,std::string winname ,int hmany):in_put(input),vec_rect_color(output),howmany(hmany),DETECT_NUM(N_DETECT_COLOR+1) {  //tips:必须使用列表初始化这两个引用
    N_DETECT_COLOR++;
    iof.open(winname +".txt");
    if (!iof.is_open()) {
        std::cout<<"open "<<winname<<".txt failed in "<<__FUNCTION__<<std::endl;
    }
    for (int i = 0;i < 6;i++) {
        iof>>range_hsv[i];
    }
    iof.close();
    if (winname == "Detect_Color") {
        //windowname = "Detect_Color" + std::to_string(N_DETECT_COLOR);
        windowthre = "thredshold" + std::to_string(N_DETECT_COLOR);
    } else {
        //windowname = winname;
        windowthre =  winname;
    }
#ifdef IMSHOW_WINDOW
    create_window();
    create_trackbar();
#endif
    threshold = in_put.clone();
    find_ball_num = 0;
    find_ball = false;
    first_rect_pos.resize(howmany,cv::Point(0,0));
    now_has_point.resize(howmany,false);
}


Detect_Color::~Detect_Color() {
    if (windowthre != "Detect_black")
        record();
}



void Detect_Color::start() {
        cv::cvtColor(in_put,threshold,cv::COLOR_BGR2HSV);
        cv::inRange(threshold,cv::Scalar(range_hsv[0],range_hsv[1],range_hsv[2]),cv::Scalar(range_hsv[3],range_hsv[4],range_hsv[5]),threshold);
        Proc_Img(threshold);
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(threshold,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
        std::vector<std::vector<cv::Point> > convexHulls(contours.size());
        
        for(unsigned int i = 0; i < contours.size(); i++)
        {
            cv::convexHull(contours[i], convexHulls[i]);
        }

        std::vector<cv::Rect> rectangles;
        for(int i = 0;i < contours.size();i++)
        {
            rectangles.push_back(cv::boundingRect(convexHulls[i]));//框选住的矩形框，在使用气球大小时最好不使用。TODO
        }
        std::sort(rectangles.begin(),rectangles.end(),comp_area); //需要有一定顺序，否则给气球对象分配会乱。按面积排序
//#ifdef DETECT_FALLING
        
        
        rectangles.resize(howmany); //howmany的大小和size的大小谁大？
        //                                           [comp_area] before
        if (howmany > 1)
            std::sort(rectangles.begin(),rectangles.end(),comp_rect_y);
//#endif
        
    if(!find_ball) {
        if (rectangles.size() <= howmany) {
            vec_rect_color.assign(rectangles.begin(),rectangles.begin() + rectangles.size());
            while (vec_rect_color.size() < howmany) {
                vec_rect_color.push_back(cv::Rect());
            }
            find_ball = false;
        } else {
            vec_rect_color.assign(rectangles.begin(),rectangles.begin() + howmany);
        }

        find_ball_num++;
        if (find_ball_num > FINDBALLNUM) {
            find_ball = true;
        }
        if (howmany > 1) {
            // if (find_ball_num > 5) {
            //     for (int i = 0;i < rectangles.size();i++) {
            //         cv::Point aim_point = rectangles[i].br();
            //         int _temp_ = match_up(aim_point,first_rect_pos);
            //         now_has_point[_temp_] = true;
            //     }
            //     for (int i = 0;i < howmany;i ++) {
            //         if (!now_has_point[i]) {
            //             vec_rect_color.insert(vec_rect_color.begin()+i,cv::Rect());
            //         }
            //     }
            // }
            for (int i = 0;i < howmany;i++) {
                first_rect_pos[i] = vec_rect_color[i].br();
            }
        }


        for (int i = 0; i < howmany; i++) {
            now_has_point[i] = false;
        }

    }
    
    // for (int i = 0;i < N_DETECT_COLOR;i++) {
    //     cv::rectangle(in_put,vec_rect_color[i],cv::Scalar(255,0,0),2);
    // }
#ifdef IMSHOW_WINDOW
    cv::imshow(windowthre,threshold);
#endif
    //record();   现在放在了析构函数中，程序关闭时才记录
}


void Detect_Color::record() {
    iof.close();
    iof.open(windowthre +".txt",std::ios::out);
    for (int i = 0;i < 6;i++) {
        iof<<range_hsv[i]<<' ';
    }
    iof.close();
    std::cout<<windowthre<<" has been recorded!"<<std::endl;
}


int Detect_Color::Howmany() {
    return howmany;
}


int* Detect_Color::Range_hsv6() {
    return range_hsv;
}


cv::Mat& Detect_Color::In_put() {
    return in_put;
}


cv::Mat& Detect_Color::Threshold() {
    return threshold;
}


std::vector<cv::Rect>& Detect_Color::Vec_rect() {
    return vec_rect_color;
}


std::string& Detect_Color::Windowthre() {
    return windowthre;
}


std::fstream& Detect_Color::Iof() {
    return iof;
}


bool Detect_Color::Find_Ball() {
    return find_ball_num;
}


void Detect_Color::restart_find(int num) {
    find_ball_num = 0;
    find_ball = false;
    howmany = num;
}


int Detect_Color::match_up(cv::Point& point,std::vector<cv::Point>& map) {
    int n = map.size();
    double min = INT_MAX;
    int number = 0;
    double temp = 0;
    for (int i = 0;i < n;i++) {
        if (map[i] == cv::Point(0,0)) continue;
        temp = point_dis(point,map[i]);
        if (temp < min) {
            min = temp;
            number = i;
        }
    }
    return number;
}