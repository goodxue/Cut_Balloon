#include <opencv2/opencv.hpp>
#include <sl_zed/Camera.hpp>
#include <fstream>
#include <string>
#include <ros/ros.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Float64.h>
//#define TESTING   //测试模式 ， 没有使用ros传输
#define UAV_CUT_BALLOON
#define DETECT_FALLING
#define FALLING_MODE 1  //检测下落模式 1是直接深度 2是点云深度
#define IMSHOW_WINDOW

#define RED_FALLING
#define YELLOW_FALLING
#define FINDBALLNUM 10000000
#define FALLING_PAUSE false //开启自动下落暂停
#define SHOW_HEIGHT  //控制台输出深度信息
#define SHOW_POINT //检测圆心画点
#define SHOW_BLACKLOCAL //黑圆中心坐标显示文字

#define RED_BALLOON 3 //3个红球
#define YELLOW_BALLOON 1 //1个黄球