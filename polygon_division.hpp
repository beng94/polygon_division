#include <cmath>
#include "data.hpp"
#include <vector>
#include <algorithm>
#include "test.hpp"

class Closest
{
private:
    double dist;
    std::vector<Point>::iterator p1;
    std::vector<Point>::iterator point;
public:
    Closest(std::vector<Point>::iterator p1, std::vector<Point>::iterator point, double dist) :
        p1(p1), dist(dist), point(point) {}
    std::vector<Point>::iterator get_point() {return point;}
    std::vector<Point>::iterator get_p1() {return p1;}
    double get_dist() {return dist;}
    void set_dist (double dist) {this->dist = dist;}
    void set_p1 (std::vector<Point>::iterator p1) {this->p1 = p1;}
};

double point_line_dist (Point, Point, Point);
void include_all_points(std::vector<Point>, std::vector<Point>&);
