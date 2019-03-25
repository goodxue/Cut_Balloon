#include "functions.hpp"

void Proc_Img(cv::Mat& thresh)
{
    cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));

    int erodenumber = 1;
    int dilatenumber = 1;

    for(int i=0;i<erodenumber;i++)
    {
        cv::erode(thresh,thresh,erodeElement);
    }

    for(int i=0;i<dilatenumber;i++)
    {
        cv::dilate(thresh,thresh,dilateElement);
    }
}

bool comp_area(cv::Rect& Rect1,cv::Rect& Rect2)
{
    return Rect1.area() > Rect2.area();
}


bool comp_cross(cv::Point2i& point1,cv::Point2i& point2) {
    return (point1.x * point2.y - point1.y * point2.x > 0);
}


bool comp_rect_cross(cv::Rect& rect1,cv::Rect& rect2) {
    cv::Point2i p1 = rect1.br();
    cv::Point2i p2 = rect2.br();
    return comp_cross(p1,p2);
}


bool comp_rect_y(cv::Rect& rect1,cv::Rect& rect2) {
    return rect1.y > rect2.y;
}


double point_dis(cv::Point2i& point1,cv::Point2i& point2) {
    return sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}
