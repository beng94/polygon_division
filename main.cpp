#include "test.hpp"
#include "graham_scan.hpp"
#include "polygon_division.hpp"

int main(void)
{
    //Generating points
    std::vector<Point> vec = gen_data(100);
    /*std::vector<Point> vec;
    Point a(0,0);
    Point b(10,0);
    Point c(10,10);
    Point d(0,10);
    Point e(2,4);
    Point g(1,4);
    Point h(1,5);
    Point i(4,6);
    vec.push_back(a);
    vec.push_back(b);
    vec.push_back(c);
    vec.push_back(d);
    vec.push_back(e);
    vec.push_back(g);
    vec.push_back(h);
    vec.push_back(i);*/
    print_vector("points.txt", vec);

    //Finding the convex hall
    std::list<Point> convex_hull = graham_scan(vec);
    print_list("convex_hull.txt", convex_hull);

    //Include all points in the concave polygon
    include_all_points(vec, convex_hull);
    print_list("concave_polygon.txt", convex_hull);

    std::cout << "Program terminated" << std::endl;
    return 0;
}
