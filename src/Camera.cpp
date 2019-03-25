#include "Camera.hpp"
#include <sstream>
#include <iostream>
#include <cstring>
#include <opencv2/opencv.hpp>

Camerax::Camerax() {
    Cam.open(0);
    Mat_num = 0;
    sl_rgb = sl_dep = sl_cld = 0;
    if (!Cam.isOpened()){
        std::cout<<"Open default camera '0' failed"<<std::endl;
        exit(-1);
    }
}


Camerax::Camerax(std::string& cam_para) {
    
    std::istringstream IS(cam_para);
    Cam_n = 2;                         //字符串流
    if (!(IS>>Cam_n)) {
        std::cout<<"wrong Cam_num in "<<__FUNCTION__<<std::endl;
        exit(-1);
    }
    if (Cam_n == USB) {
        Cam.open(Cam_n);
    }
    else if (Cam_n == ZED) {
        sl::InitParameters init_params;
        init_params.depth_mode = sl::DEPTH_MODE_QUALITY; // Use PERFORMANCE depth mode
        init_params.coordinate_units = sl::UNIT_MILLIMETER; // Use millimeter units (for depth measurements)
        //init_params.depth_minimum_distance = 50;

        sl::ERROR_CODE err = zed.open(init_params);
        if (err != sl::SUCCESS) {
            exit(-1);
        }
        runtime_parameters.sensing_mode = sl::SENSING_MODE_STANDARD;  //设置运行参数为标准
    }
    Mat_num = 0;
}


Camerax::~Camerax() {
    if (Cam_n == USB) {
        Cam.release();
    }
    else if (Cam_n == ZED) {
        zed.close();
    }

}


void Camerax::close() {
    this -> ~Camerax();
}


bool Camerax::refresh_rgb(cv::Mat& out_put) {
    if (!Cam.isOpened()) {
        std::cout<<"camera hasnt been started in "<<__FUNCTION__<<std::endl;
        return false;
    }
    if(Cam.read(out_put)){
        return true;
    }
}


bool Camerax::refresh_rgb(sl::Mat& out_put){
    if (Cam_n != ZED) {
        std::cout<<"haven't started ZED camera yet, fail occurred in "<<__FUNCTION__<<std::endl;
        return false;
    }
    //if (zed.grab(runtime_parameters) == sl::SUCCESS) {
        zed.retrieveImage(out_put,sl::VIEW_LEFT);
    //}
    // else {
    //     return false;
    // }
}


bool Camerax::isok() {
    return Cam.isOpened();
}


void Camerax::view_cv_dep(cv::Mat& out_put,sl::Mat& in_put) {
    out_put = slMat2cvMat(in_put);
}


bool Camerax::refresh_sl_measure_dep(sl::Mat& out_put) {
    if (Cam_n != ZED) {
        std::cout<<"haven't started ZED camera yet, fail occurred in "<<__FUNCTION__<<std::endl;
        return false;
    }
    if (zed.grab(runtime_parameters) == sl::SUCCESS) {
        zed.retrieveMeasure(out_put,sl::MEASURE_DEPTH);
        return true;
     }
    // else {
    //     return false;
    // }
}


bool Camerax::refresh_sl_measure_cld(sl::Mat& out_put) {
    if (Cam_n != ZED) {
        std::cout<<"haven't started ZED camera yet, fail occurred in "<<__FUNCTION__<<std::endl;
        return false;
    }
    else ;
    //if (zed.grab(runtime_parameters) == sl::SUCCESS) {
        if(zed.retrieveMeasure(out_put,sl::MEASURE_XYZRGBA))
        return false; 
    //}
    // else {
    //     return false;
    // }
}


sl::Mat Camerax::create_sl_mat_rgb() {
    sl::Mat image_zed(zed.getResolution(), sl::MAT_TYPE_8U_C4);
    Mat_num++;
    sl_rgb++;
    return image_zed;
}


sl::Mat Camerax::create_sl_mat_dep() {
    sl::Mat image_depth_zed(zed.getResolution(), sl::MAT_TYPE_8U_C4);
    Mat_num++;
    sl_dep++;
    return image_depth_zed;
}



int Camerax::n_slrgb() {
    return sl_rgb;
}


int Camerax::n_sldep() {
    return sl_dep;
}


int Camerax::n_slcld() {
    return sl_cld;
}


cv::Mat Camerax::slMat2cvMat(sl::Mat& input) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
        case sl::MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
        case sl::MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
        case sl::MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
        case sl::MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
        case sl::MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
        case sl::MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
        case sl::MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
        case sl::MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
        default: break;
    }

    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM_CPU));
}