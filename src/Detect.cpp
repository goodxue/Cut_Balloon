#include "Detect.hpp"
#include <sstream>
#include <iostream>
//#include <cstring>
//#include <opencv2/opencv.hpp>
// typedef Detect::Detect_Red DDR;
// typedef Detect::Detect_Black DDB;
// typedef Detect::Detect_Yellow DDY;
//typedef Detect::Detect_Fall DDF;


Detect::Detect():Cam(),vect_rect_red(),vect_rect_yellow(),
                detect_red(cv_image_raw,vect_rect_red,"Detect_yellow",YELLOW_BALLOON),
                detect_yellow(cv_image_raw,vect_rect_yellow,"Detect_red",RED_BALLOON),
                detect_black(cv_image_raw,vect_rect_black,"Detect_black")/*,detect_black(),detect_yellow()*/ 
                //sl_image_left(Cam.zed.getResolution(), sl::MAT_TYPE_8U_C4),
                //sl_image_depth(Cam.zed.getResolution(),sl:: MAT_TYPE_8U_C4)
{
    // sl::Mat temp_image1;
    // Cam.refresh_rgb(temp_image1);
    // cv_image_show = Cam.slMat2cvMat(temp_image1); 
#ifdef IMSHOW_WINDOW
    cv::namedWindow("image_raw",0);
#endif
    sl::Mat temp_image2;
    Cam.refresh_rgb(temp_image2);
    cv_image_raw = Cam.slMat2cvMat(temp_image2);
    flag = 0;
#ifdef DETECT_FALLING

    init_detect_falling(FALLING_MODE);
#endif
}


Detect::Detect(std::string& cam_para):Cam(cam_para),vect_rect_red(),vect_rect_yellow(),
                                    detect_red(cv_image_raw,vect_rect_red,"Detect_yellow",YELLOW_BALLOON),
                                    detect_yellow(cv_image_raw,vect_rect_yellow,"Detect_red",RED_BALLOON),
                                    detect_black(cv_image_raw,vect_rect_black,"Detect_black",1)/*,detect_black(),detect_yellow()*/ 
                                    //sl_image_left(Cam.zed.getResolution(), sl::MAT_TYPE_8U_C4),
                                    //sl_image_depth(Cam.zed.getResolution(), sl::MAT_TYPE_8U_C4)
{
    
    // sl::Mat temp_image1;
    // Cam.refresh_rgb(temp_image1);
    // cv_image_show = Cam.slMat2cvMat(temp_image1); 
#ifdef IMSHOW_WINDOW  
    cv::namedWindow("image_raw",0);
#endif
    sl::Mat temp_image2;
    //Cam.refresh_sl_measure_dep(sl_image_depth);
    //Cam.refresh_sl_measure_cld(sl_point_cloud);
    Cam.refresh_rgb(temp_image2);
    cv_image_raw = Cam.slMat2cvMat(temp_image2);
    flag = 0;
    falling_pause_flag = false;
#ifdef DETECT_FALLING
    
        init_detect_falling(FALLING_MODE);
#endif
}


//Detect::~Detect() {

//}


cv::Point2d Detect::detecting(std_msgs::Float64 Altitude) {
    //std::cout<<Show_Height<<std::endl<<Detect_Falling<<std::endl;
    int tf = 0;
    if (!update_image()) {
        return cv::Point2d(-1,-1);
    }
#ifdef DETECT_FALLING

    if (!has_fell_red)
#endif
        detect_red.start();
#ifdef DETECT_FALLING

    if (!(has_fell_yellow1 && has_fell_yellow2 && has_fell_yellow3))
#endif
        detect_yellow.start();
        detect_black.start();
    
    update_mid();

#ifdef DETECT_FALLING

    tf = uav_tf(Altitude);
#endif
#ifdef IMSHOW_WINDOW
    im_show();
#endif
    //flag = cv::waitKey(1);
//#ifdef DETECT_FALLING
    // if (flag == 'r') {
    //     record_tf();
    // }
    switch(tf) {
        case 0: return cv::Point2d(-1,-1);
        case 1: return red_mid;
        case 2: return vect_yellow_mid[0];
        case 3: return vect_yellow_mid[1];
        case 4: return vect_yellow_mid[2];
        //default: return vect_yellow_mid[tf];
    }
//#endif
}


cv::Point2d Detect::black_pos() {
    detect_black.start();

    if (!vect_rect_black.empty() && !vect_rect_black[0].empty()) {
        black_mid.x = vect_rect_black[0].x + vect_rect_black[0].width / 2.0;
        black_mid_n.x = black_mid.x - cv_image_raw.cols / 2;
        black_mid.y = vect_rect_black[0].y + vect_rect_black[0].height / 2.0;
        black_mid_n.y = - (black_mid.y - cv_image_raw.rows / 2);
    } else {red_mid = red_mid_n = cv::Point2d(0.0,0.0);}
    return black_mid;
}


void Detect::im_show() {
    Cam.zed.retrieveImage(image_depth_ocv, sl::VIEW_DEPTH);
    if (!has_fell_red) //黄球没下落的话
        cv::rectangle(cv_image_raw,vect_rect_red[0],cv::Scalar(255,0,0),2);
    for (int i = 0;i < 3;i++) {
        if ((i==0 && !has_fell_yellow1) || (i==1&& !has_fell_yellow2) || i==2&&!has_fell_yellow3)
            cv::rectangle(cv_image_raw,vect_rect_yellow[i],cv::Scalar(255,255,0),2);
    }
    cv::rectangle(cv_image_raw,vect_rect_black[0],cv::Scalar(0,255,255),2);
    int x1 = static_cast<int>(vect_rect_black[0].x + vect_rect_black[0].width/2.0 - vect_rect_black[0].y - vect_rect_black[0].height/2.0);
    int x2 = static_cast<int>(vect_rect_black[0].x + vect_rect_black[0].width/2.0 + cv_image_raw.rows - vect_rect_black[0].y - vect_rect_black[0].height/2.0);
    int x3 = static_cast<int>(vect_rect_black[0].x + vect_rect_black[0].width/2.0 + vect_rect_black[0].y + vect_rect_black[0].height/2.0);
    int x4 = static_cast<int>(vect_rect_black[0].x + vect_rect_black[0].width/2.0 - cv_image_raw.rows + vect_rect_black[0].y + vect_rect_black[0].height/2.0);
    cv::line(cv_image_raw, cv::Point(x1,0), cv::Point(x2,cv_image_raw.rows), cv::Scalar(0,255,255),2);
    cv::line(cv_image_raw, cv::Point(x3,0), cv::Point(x4,cv_image_raw.rows), cv::Scalar(0,255,255),2);

#ifdef IMSHOW_WINDOW
    cv::namedWindow("image_raw",0);
#endif

#ifdef SHOW_POINT
    cv::circle(cv_image_raw,black_mid,2,cv::Scalar(0,0,255),-1);
    cv::circle(cv_image_raw,red_mid,2,cv::Scalar(0,255,255),-1);
    for (int i = 0;i < RED_BALLOON;i++)
        cv::circle(cv_image_raw,vect_yellow_mid[i],2,cv::Scalar(255,0,255),-1);
#ifdef IMSHOW_WINDOW
#ifdef SHOW_BLACKLOCAL
    cv::putText(cv_image_raw,"("+std::to_string(int(black_mid.x))+","+std::to_string(int(black_mid.y))+")",black_mid,cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,255,255),2);
#endif
#endif
#endif
#ifdef IMSHOW_WINDOW
    cv::imshow("image_raw",cv_image_raw);
    cv::Mat temp = Cam.slMat2cvMat(image_depth_ocv);
    cv::namedWindow("image_dep",0);
    cv::imshow("image_dep",temp);
#endif
    //cv::waitKey(1);
}
// void Detect::im_show(Detect_Red& detect_red) {
//     cv::rectangle(cv_image_raw,detect_red.rect_red,cv::Scalar(255,0,0),2);
//     cv::imshow(detect_red.windowname,cv_image_raw);
//     cv::imshow(detect_red.windowthre,detect_red.threshold);
//     cv::waitKey(1);
// }


// void DDR::create_window() {
//     cv::namedWindow(windowname,0);
//     cv::moveWindow(windowname,0,0);
//     cv::namedWindow(windowthre,0);
//     cv::moveWindow(windowthre,300,0);
// }


bool Detect::update_image() {
    if (Cam.zed.grab() == sl::SUCCESS) {
        Cam.zed.retrieveImage(sl_image_left,sl::VIEW_LEFT);
        if (FALLING_MODE == 1)
            Cam.zed.retrieveMeasure(sl_image_depth,sl::MEASURE_DEPTH);
        else
            Cam.zed.retrieveMeasure(sl_point_cloud,sl::MEASURE_XYZRGBA);
    }
    else {std::cout<<"grab error fatel error!!!"<<std::endl;
        return false;
    }
    //std::cout<<"is init   "<<sl_image_depth.getInfos()<<std::endl;
    // Cam.refresh_rgb(sl_image_left);
    // Cam.refresh_sl_measure_dep(sl_image_depth);
    // if(!Cam.refresh_sl_measure_cld(sl_point_cloud)){
    //     std::cout<<"point_cloud retrived false!!"<<std::endl;
    // }
    cv_image_raw = Cam.slMat2cvMat(sl_image_left);
    return true;
    
}


void Detect::update_mid() {
    if (!vect_rect_red.empty() && !vect_rect_red[0].empty()) {
        red_mid.x = vect_rect_red[0].x + vect_rect_red[0].width / 2.0;
        red_mid.y = vect_rect_red[0].y + vect_rect_red[0].height / 2.0;
        red_mid_n.x = red_mid.x - cv_image_raw.cols / 2;
        red_mid_n.y = - (red_mid.y - cv_image_raw.rows / 2);
    } else {red_mid = red_mid_n = cv::Point2d(0.0,0.0);}
    for (int i = 0;i < 3;i++) {
        if (!vect_rect_yellow[i].empty() && !vect_rect_yellow[i].empty()) {
            vect_yellow_mid[i].x = vect_rect_yellow[i].x + vect_rect_yellow[i].width / 2.0;
            vect_yellow_mid_n[i].x =  vect_yellow_mid[i].x- cv_image_raw.cols / 2;
            vect_yellow_mid[i].y = vect_rect_yellow[i].y + vect_rect_yellow[i].height / 2.0;
            vect_yellow_mid_n[i].y = - (vect_yellow_mid[i].y - cv_image_raw.rows / 2);
        } else {vect_yellow_mid[i] = vect_yellow_mid_n[i] = cv::Point2d(0.0,0.0);}
    }
    if (!vect_rect_black.empty() && !vect_rect_black[0].empty()) {
        black_mid.x = vect_rect_black[0].x + vect_rect_black[0].width / 2.0;
        black_mid.y = vect_rect_black[0].y + vect_rect_black[0].width / 2.0;
    }
}

#ifdef DETECT_FALLING
void Detect::init_detect_falling(int mode) {
    if (mode == 1) {
        f_red = new Detect_falling(sl_image_depth,1);
        f_yellow1 = new Detect_falling(sl_image_depth,1);
        if (RED_BALLOON >= 2)
            f_yellow2 = new Detect_falling(sl_image_depth,1);
        if (RED_BALLOON >= 3)
        f_yellow3 = new Detect_falling(sl_image_depth,1);
    }
    else if (mode == 2) {
        f_red = new Detect_falling(sl_point_cloud,2);
        f_yellow1 = new Detect_falling(sl_point_cloud,2);
        if (RED_BALLOON >= 2)
            f_yellow2 = new Detect_falling(sl_point_cloud,2);
        if (RED_BALLOON >= 3)
            f_yellow3 = new Detect_falling(sl_point_cloud,2);
    }
    has_fell_red = false;
    has_fell_yellow1 = false;
    has_fell_yellow2 = false;
    has_fell_yellow3 = false;
}


int Detect::uav_tf(std_msgs::Float64 Altitude) {
double altitude = Altitude.data;
#ifdef YELLOW_FALLING
    if(!has_fell_red) {
        has_fell_red = f_red->start(red_mid,altitude);
#ifdef SHOW_HEIGHT

        std::cout<<"yellow_depth: "<<f_red->depth_temp<<std::endl;
#endif
        //std::cout<<"red mid x: "<<red_mid.x<<"   y :"<<red_mid.y<<std::endl;
        if(has_fell_red) {
            for (int i = 5;i > 0;i--)
            std::cout<<"yellow is falling!"<<std::endl;
            return 1;
        }
    }
#endif
#ifdef RED_FALLING
    int _n = 0;  //颜色排序，气球矩形框按向量角度从高到低
    if(!has_fell_yellow1) {
        has_fell_yellow1 = f_yellow1->start(vect_yellow_mid[0],altitude);
        //std::cout<<f_yellow1->depth_temp<<std::endl;
#ifdef SHOW_HEIGHT

        std::cout<<"red1_depth: "<<f_yellow1->depth_temp<<std::endl;
#endif
        if(has_fell_yellow1) {
            for (int i = 5;i > 0;i--)
            std::cout<<"red1 is falling"<<std::endl;
            return 2;
        }
        //std::cout<<"yellow1 mid:   "<<vect_yellow_mid[0]<<std::endl;
    }
    if(!has_fell_yellow2) {
        has_fell_yellow2 = f_yellow2->start(vect_yellow_mid[!has_fell_yellow1],altitude);
#ifdef SHOW_HEIGHT

        std::cout<<"red2_depth: "<<f_yellow2->depth_temp<<std::endl;
#endif
        if(has_fell_yellow2) {
            for (int i = 5;i > 0;i--)
            std::cout<<"red2 is falling"<<std::endl;
            return 3;
        }
        
    }
    if(!has_fell_yellow3) {
        has_fell_yellow3 = f_yellow3->start(vect_yellow_mid[!has_fell_yellow1+!has_fell_yellow2],altitude);
#ifdef SHOW_HEIGHT

        std::cout<<"red3_depth: "<<f_yellow3->depth_temp<<std::endl<<std::endl;
#endif
        if(has_fell_yellow3) {
            for (int i = 5;i > 0;i--)
            std::cout<<"red3 is falling"<<std::endl;
            return 4;
        }
        
    }

#endif
    return 0;
}



void Detect::record_tf() {
   Black_mid = black_mid;
   std::cout<<"Black mid has been recorded"<<std::endl;
}


void Detect::reinit(int num) {
    detect_yellow.restart_find(num);
}


void Detect::Falling_pause() {
    f_red->Falling_pause();
    f_yellow1->Falling_pause();
    f_yellow2->Falling_pause();
    f_yellow3->Falling_pause();
    falling_pause_flag = !falling_pause_flag;
    if (falling_pause_flag)
        std::cout<<"start falling_pause"<<std::endl;
    else
        std::cout<<"close falling_pause"<<std::endl;
}

#endif
// void DDR::create_trackbar() {
//     cv::createTrackbar("H1:",windowname,&range_hsv[0],180,0,0);
//     cv::createTrackbar("S1:",windowname,&range_hsv[1],255,0,0);
//     cv::createTrackbar("V1:",windowname,&range_hsv[2],255,0,0);
//     cv::createTrackbar("H2:",windowname,&range_hsv[3],180,0,0);
//     cv::createTrackbar("S2:",windowname,&range_hsv[4],255,0,0);
//     cv::createTrackbar("V2:",windowname,&range_hsv[5],255,0,0);
// }



// DDR::Detect_Red (cv::Mat& input):in_put(input) {  //tips:必须使用列表初始化这两个引用
//     iof.open("red_hsv.txt");
//     if (!iof.is_open()) {
//         std::cout<<"open red_hsv.txt failed in "<<__FUNCTION__<<std::endl;
//     }
//     for (int i = 0;i < 6;i++) {
//         iof>>range_hsv[i];
//     }
//     windowname = "Detect_Red";
//     windowthre = "thredshold";
//     create_window();
//     create_trackbar();
//     threshold = in_put.clone();
// }


// void DDR::start() {
//     cv::cvtColor(in_put,threshold,cv::COLOR_BGR2HSV);
//     cv::inRange(threshold,cv::Scalar(range_hsv[0],range_hsv[1],range_hsv[2]),cv::Scalar(range_hsv[3],range_hsv[4],range_hsv[5]),threshold);
//     Proc_Img(threshold);
//     std::vector<std::vector<cv::Point> > contours;
//     cv::findContours(threshold,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
//     std::vector<std::vector<cv::Point> > convexHulls(contours.size());
//     for(unsigned int i = 0; i < contours.size(); i++)
//     {
//         cv::convexHull(contours[i], convexHulls[i]);
//     }

//     std::vector<cv::Rect> rectangles;
//     for(int i = 0;i < contours.size();i++)
//     {
//         rectangles.push_back(cv::boundingRect(convexHulls[i]));
//     }
//     std::sort(rectangles.begin(),rectangles.end(),comp_area);
//     if (!rectangles.empty()) {
//         rect_red = rectangles[0];
//     }
//     cv::rectangle(in_put,rect_red,cv::Scalar(255,0,0),2);
//     record_hsv();
// }


// void DDR::record_hsv() {
//     iof.close();
//     iof.open("red_hsv.txt");
//     for (int i = 0;i < 6;i++) {
//         iof<<range_hsv[i]<<' ';
//     }
// }