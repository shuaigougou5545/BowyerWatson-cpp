#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <Eigen/Dense>
#include <stdio.h>
#include "BowyerWatson.hpp"

using namespace cv;

const int width = 2560, height = 1440;
BowyerWatson bw(width,height);
 
void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat *img = reinterpret_cast<cv::Mat*>(param);
    switch (event)
    {
        case EVENT_LBUTTONDOWN:
            bw.addPoint(Vector2f{x,y});
            bw.show(img);
            imshow("window", *img);
            break;
    }
}

int main()
{
    // opencv左上角为原点,点的范围是分辨率范围
    Mat img = Mat::zeros(Size(width,height), CV_8UC3);
    img.setTo(255);
    
    bw.show(& img);
    imshow("window", img);
    setMouseCallback("window",onMouse,reinterpret_cast<void*>(&img));
    
    waitKey();
    return 0;
}
