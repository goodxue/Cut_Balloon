#include <opencv2/opencv.hpp>

void Proc_Img(cv::Mat& thresh);
bool comp_area(cv::Rect& Rect1,cv::Rect& Rect2);
bool comp_cross(cv::Point2i& point1,cv::Point2i& point2);
bool comp_rect_cross(cv::Rect& rect1,cv::Rect& rect2);
bool comp_rect_y(cv::Rect& rect1,cv::Rect& rect2);
double point_dis(cv::Point2i& point1,cv::Point2i& point2);
