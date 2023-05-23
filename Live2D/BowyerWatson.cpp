#include "BowyerWatson.hpp"

BowyerWatson::BowyerWatson(int _width, int _height):width(_width),height(_height)
{
    /*
     p0---p1
    |       |
     p2---p3
     */
    Vector2f p0(0,0);
    Vector2f p1(width,0);
    Vector2f p2(0,height);
    Vector2f p3(width, height);
    
    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    
    triangles.push_back(Vector3i(0,1,2));
    triangles.push_back(Vector3i(1,2,3));
    
    Vector2f center(width/2.0, height/2.0);
    circle_centers.push_back(center);
    circle_centers.push_back(center);
    
    float radius = sqrt(pow(width/2.0, 2.0)+pow(height/2.0, 2.0));
    radiuses.push_back(radius);
    radiuses.push_back(radius);
}

void BowyerWatson::addPoint(Vector2f point)
{
    points.push_back(point);
    int n = (int)points.size()-1;
    int cnt = (int)triangles.size();
    
    vector<Vector3i> new_triangles;
    vector<Vector2f> new_circle_centers;
    vector<float> new_radiuses;
    
    vector<vector<int>> edges; // 坏三角形的边
    
    set<int> m_set;
    
    for(int i=0; i<cnt; i++)
    {
        auto triangle = triangles[i];
        auto circle_center = circle_centers[i];
        auto radius = radiuses[i];
        
        float d = (point-circle_center).norm();
        
        if(d > radius){
            new_triangles.push_back(triangle);
            new_circle_centers.push_back(circle_center);
            new_radiuses.push_back(radius);
            continue;
        }
            
        int i1 = triangle[0], i2 = triangle[1], i3 = triangle[2];
        edges.push_back({i1, i2});
        edges.push_back({i2, i3});
        edges.push_back({i3, i1});
    }
    
    triangles = new_triangles;
    circle_centers = new_circle_centers;
    radiuses = new_radiuses;

    // 处理空腔
    vector<bool> repeat_lines(edges.size(), false);
    for(int i=0; i<(int)edges.size(); i++)
    {
        for(int j=0; j<(int)edges.size(); j++)
        {
            if(i == j)
                continue;
            
            if((edges[i][0] == edges[j][0] && edges[i][1] == edges[j][1])
               || (edges[i][1] == edges[j][0] && edges[i][0] == edges[j][1]))
            {
                repeat_lines[i] = true;
                repeat_lines[j] = true;
            }
        }
    }
    
    for(int i=0; i<(int)edges.size(); i++)
    {
        if(repeat_lines[i] == true)
            continue;
        
        int i1 = edges[i][0], i2 = edges[i][1];
        triangles.push_back(Vector3i(i1, i2, n));
        float radius;
        Vector2f circle_center;
        calcTriangleInfo(i1, i2, n, radius, circle_center);
        radiuses.push_back(radius);
        circle_centers.push_back(circle_center);
    }
}

void BowyerWatson::show(void* param)
{
    img = reinterpret_cast<cv::Mat*>(param);
    img->setTo(255); // 记得清空画布
    
    for(int i=0; i<(int)points.size(); i++)
    {
        if(i <= 3)
            continue;
        circle(*img, Point2f(points[i][0],points[i][1]), 6, Scalar(0, 0, 255), -1);
    }
    
    for(auto triangle : triangles)
    {
        int i1 = triangle[0];
        int i2 = triangle[1];
        int i3 = triangle[2];
        
        if(i1 <= 3 || i2 <= 3 || i3 <= 3)
            continue;
        
        auto p1 = points[i1];
        auto p2 = points[i2];
        auto p3 = points[i3];
        
        drawLine(p1, p2);
        drawLine(p2, p3);
        drawLine(p3, p1);
    }
    imshow("window", *img);
}

void BowyerWatson::drawLine(Vector2f p1, Vector2f p2)
{
    line(*img, Point(p1[0],p1[1]), Point(p2[0],p2[1]), Scalar(0,0,0), 4);
}

void BowyerWatson::calcTriangleInfo(int i1, int i2, int i3, float& radius, Vector2f& circle_center)
{
    auto x1 = points[i1].x(), y1 = points[i1].y();
    auto x2 = points[i2].x(), y2 = points[i2].y();
    auto x3 = points[i3].x(), y3 = points[i3].y();
    
    Matrix3f m1, m2, m3, m4;
    m1 << (x1*x1+y1*y1), y1, 1,
        (x2*x2+y2*y2), y2, 1,
        (x3*x3+y3*y3), y3, 1;
    m2 << x1, y1, 1,
        x2, y2, 1,
        x3, y3, 1;
    m3 << x1, (x1*x1+y1*y1), 1,
        x2, (x2*x2+y2*y2), 1,
        x3, (x3*x3+y3*y3), 1;
    m4 << x1, y1, 1,
        x2, y2, 1,
        x3, y3, 1;
    
    circle_center[0] = m1.determinant()/(2.0*m2.determinant());
    circle_center[1] = m3.determinant()/(2.0*m4.determinant());
    
    auto p1 = points[i1];
    radius = (circle_center-p1).norm();
}

void BowyerWatson::interpolation()
{
    auto store_circle_centers = circle_centers;
    auto store_triangles = triangles;
    for(int i = 0; i < (int)store_circle_centers.size(); i++)
    {
        auto triangle = store_triangles[i];
        if(triangle[0] <= 3 || triangle[1] <= 3 || triangle[2] <= 3)
            continue;
        
        // TODO:debug
        auto cc = store_circle_centers[i];
        addPoint(cc);
    }
}
