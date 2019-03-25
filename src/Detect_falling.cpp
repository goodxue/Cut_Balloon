#include "Detect_falling.hpp"

//Detect_falling::Detect_falling():MODE(1) {}


Detect_falling::Detect_falling(sl::Mat& image_in,int mode):MODE(mode),image_input(image_in) {
    fell = false;
    depth_temp = 0;
    falling_pause = FALLING_PAUSE;
}


bool Detect_falling::start(cv::Point2d& point,float altitude) {
    if (fell) {
        return false;
    }
    //a_point = point;
    
    if (que_altitude.size() >= VEC_ALTITUDE) {  //更新队列
        que_altitude.pop(); //可能会有bug
    }
    
    que_altitude.push(altitude);
    std::vector<cv::Point2d> vec_point;
    vec_point.push_back(point);
    vec_point.push_back(cv::Point2d(point.x,point.y+18));
    vec_point.push_back(cv::Point2d(point.x,point.y-18));
    
    if (MODE == 1) {
        image_input.getValue(point.x,point.y,&depth_temp);
        //std::cout<<"x : "<<point.x<<std::endl<<"y : "<<point.y<<std::endl;
    }
    
    else if (MODE == 2) {
        for (int i = 0; i < vec_point.size();i++) {
            sl::float4 point3d;
            image_input.getValue(vec_point[i].x,vec_point[i].y,&point3d);
            float x = point3d.x;
            float y = point3d.y;
            float z = point3d.z;
            depth_temp += sqrt(x*x+y*y+z*z);
        //std::cout<<"x : "<<point.x<<std::endl<<"y : "<<point.y<<std::endl;//<<"z : "<<z<<std::endl;
        //std::cout<<depth_temp<<std::endl;
        }
        depth_temp /= vec_point.size();
    }

    if (que_depth.size() >= VEC_DEPTH) {
        que_depth.pop();
    }
    //std::cout<<"is init   "<<image_input.getInfos()<<std::endl;
    //std::cout<<depth_temp<<std::endl;
    if (depth_temp != INFINITY && depth_temp != NAN) {
        que_depth.push(depth_temp);
    }
    if (falling_pause) {
        return is_falling();
    }
    else {
        return false;
    }
}


bool Detect_falling::is_falling() {
    if (que_altitude.size() < 5) return false;
    if (que_depth.size() < 5) return false;
    //vec_depth_size = vec_depth.size();
    //vec_altitude_size = vec_altitude.size();
    // auto p_altitude = vec_altitude.end()-1;
    // auto p_depth = vec_depth.end()-1;

       //vector可能没有数据，fall的输出为0 
        //fall = abs(abs(float(*p_altitude) - float(*(p_altitude-1)))-abs(*p_depth-*(p_depth-1)));
    fall = abs(depth_temp-que_depth.front()); //注意气球深度是depth，飞机高度是altitude，别搞混了
    std::cout<<que_depth.front()<<std::endl<<fall<<std::endl; //检查向vector内传入深度数据的代码
    if (fall > FALLING) {
        fell = 1;
        return true;
    }
    return false;
}


void Detect_falling::Falling_pause() {
    falling_pause = !falling_pause;
    // if (falling_pause) {
    //     std::cout<<"start falling_pause"<<std::endl;
    // }
    // else {
    //     std::cout<<"close falling_pause"<<std::endl;
    // }
}