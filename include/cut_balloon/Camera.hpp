#include "DEFINITIONS.hpp"


//class Detect;
class Camerax{
public:
    Camerax();                                      //默认构造函数
    Camerax(std::string& Cam_para);                  //开几号摄像头
    virtual ~Camerax();                             //析构函数
    void close();                                   //释放对象

    bool refresh_rgb(cv::Mat& out_put);             //更新当前rgb帧
    bool refresh_rgb(sl::Mat& out_put); 

    void view_cv_dep(cv::Mat& out_put,sl::Mat& in_put);             //更新深度图
    bool refresh_sl_measure_dep(sl::Mat& out_put);  //更新测量深度时使用的sl深度
    bool refresh_sl_measure_cld(sl::Mat& out_put);
    bool isok();//TODO

    sl::Mat create_sl_mat_rgb();
    sl::Mat create_sl_mat_dep();
    int n_slrgb();
    int n_sldep();
    int n_slcld();
    cv::Mat slMat2cvMat(sl::Mat& input);             //sl::mat转cv::mat
    sl::Camera zed;
    sl::RuntimeParameters runtime_parameters;        //zed的运行参数
private:
    int Cam_n;//TODO
    int Mat_num;                                        //创建mat的数量
    int sl_rgb;
    int sl_dep;
    int sl_cld;
    cv::VideoCapture Cam; 
    
    
    

    enum Cam_num { USB = 0 , ZED };                      //开的摄像头参数

    
};
