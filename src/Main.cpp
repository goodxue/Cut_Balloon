#include "Detect.hpp"
#include <ctime>

#ifndef TESTING
std::string cam_num("1");

//geometry_msgs::Pose2D black_mid_pos;
char key = ' ';
bool flag = true;
int remain = RED_BALLOON;
std_msgs::Float64 Height;

void detect_falling(const std_msgs::Float64 Heights) {
    Height = Heights;
}

int main(int argc,char **argv) 
{
    std::string arg1;
    if (argv[1] != nullptr){
        arg1 = argv[1];
    }
    else {
        arg1 = "";
    }
    std::string test("test");

    ros::init(argc,argv,"Detect_Falling");
    ros::NodeHandle nh;

    ros::Publisher black_mid_pub = nh.advertise<geometry_msgs::Pose2D>
            ("black_mid_pos",1);
    ros::Subscriber uav_height = nh.subscribe<std_msgs::Float64>
            ("xdd_z",1,detect_falling);
    ros::Rate rate(18);
    
    bool cdetect_height;
    if (argv[2] == "false") {
        cdetect_height = false;
    } else {
        cdetect_height = true;
    }

    Detect detect(cam_num);

    long long int pic_time = 0;
    while (ros::ok() && key != 'q') {
        if (key != 'q') {
            clock_t start = clock();
            std_msgs::Float64 altitude;
            altitude.data = 0.00;
            if (flag){
                cv::Point2d fall_pos = detect.detecting(Height);
                if( fall_pos != cv::Point2d(-1,-1)){
    #ifdef DETECT_FALLING
                    flag = false;
                    std::cout<<"..........Falling detected,pausing..............."<<std::endl;
                    std::cout<<"...............type 'g' to restart..............."<<std::endl;
                    
                    if (fall_pos != detect.red_mid)
                        remain--;
    #endif
                }
            } 
            if (key == 'g') {
                flag = true;
                key = ' ';

    #ifdef DETECT_FALLING
                detect.reinit(remain); //重新分配需要检测的气球下落数量
    #endif
            }
            key = cv::waitKey(1);
    #ifdef DETECT_FALLING
            if (key == 'p'){ //开启检测下落并当下落时暂停
                detect.Falling_pause();
            }
    #endif
            clock_t end = clock();
        //std::cout<<"结束时间："<<end<<std::endl;
        //std::cout<<"算法执行持续时间："<<end-start<<"毫秒"<<std::endl;
            }

        pic_time++;
        //black_mid_pub.publish(black_mid_pos);
        ros::spinOnce();
        rate.sleep();
    }
    

}

#endif

#ifdef TESTING
int main(int argc,char** argv)
{
    // if (argv[1] == NULL) {
    //     char cam_num_[] = "0";
    //     argv[1] = cam_num_;
    // }
    // std::string cam_num("1");
    // Camerax camera(cam_num);
    // //cv::Mat test;
    // cv::namedWindow("Test1",0);
    // //cv::namedWindow("Test2",0);
    // sl::Mat image_zed = camera.create_sl_mat_rgb();
    // sl::Mat image_depth_zed = camera.create_sl_mat_dep();
    // //cv::Mat image_depth_ocv;
    // while (camera.isok()) {
    //     camera.refresh_rgb(image_zed);
    //     camera.refresh_sl_measure_dep(image_depth_zed);
    //     //camera.refresh_rgb(test);
    //     cv::imshow("Test1",0);
    //     //cv::imshow("Test2",test);
    //     cv::waitKey(1);
    // }
    // return 0;
    


    bool flag = true;
    if (argv[1] == NULL) {
        char cam_num_[] = "0";
        argv[1] = cam_num_;
    }
    std::string cam_num("1");
    Detect detect(cam_num);
    char key = ' ';
    int remain = RED_BALLOON;
    while (key != 'q') {
        clock_t start = clock();
        std_msgs::Float64 altitude;
        altitude.data = 0.00;
        if (flag){
            cv::Point2d fall_pos = detect.detecting(altitude);
            if( fall_pos != cv::Point2d(-1,-1)){
#ifdef DETECT_FALLING
                flag = false;
                std::cout<<"..........Falling detected,pausing..............."<<std::endl;
                std::cout<<"...............type 'g' to restart..............."<<std::endl;
                
                if (fall_pos != detect.red_mid)
                    remain--;
#endif
            }
        } 
        if (key == 'g') {
            flag = true;
            key = ' ';

#ifdef DETECT_FALLING
            detect.reinit(remain); //重新分配需要检测的气球下落数量
#endif
        }
        key = cv::waitKey(1);
#ifdef DETECT_FALLING
        if (key == 'p'){ //开启检测下落并当下落时暂停
            detect.Falling_pause();
        }
#endif
    clock_t end = clock();
	//std::cout<<"结束时间："<<end<<std::endl;
	std::cout<<"算法执行持续时间："<<end-start<<"毫秒"<<std::endl;
    }
    return 0;
}
#endif