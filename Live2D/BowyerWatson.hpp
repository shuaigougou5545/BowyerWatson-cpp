#ifndef BowyerWatson_hpp
#define BowyerWatson_hpp

#include <stdio.h>
#include <vector>
#include <set>
#include <queue>
#include <Eigen/Dense>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;
using namespace Eigen;

class BowyerWatson{
private:
    vector<Vector2f> points;
    vector<Vector3i> triangles;
    vector<Vector2f> circle_centers;
    vector<float> radiuses; 
    
    int width, height;
    Mat *img;
public:
    BowyerWatson(int _width, int _height);
    void addPoint(Vector2f point);
    void show(void* param);
    void drawLine(Vector2f p1, Vector2f p2);
    void calcTriangleInfo(int i1, int i2, int i3, float& radius, Vector2f& circle_center);
};

#endif /* BowyerWatson_hpp */
