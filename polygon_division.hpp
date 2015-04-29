#include <cmath>
#include "data.hpp"
#include <vector>
#include <algorithm>
#include "test.hpp"

class Closest
{
private:
    double dist;
    Point p1, p2;
    Point point;
public:
    Closest(Point p1, Point p2, Point point, double dist) : p1(p1), p2(p2), dist(dist), point(point) {}
    Point get_point() {return point;}
    Point get_p1() {return p1;}
    Point get_p2() {return p2;}
    double get_dist() {return dist;}
    void set_dist (double dist) {this->dist = dist;}
    void set_p1 (Point p1) {this->p1 = p1;}
    void set_p2 (Point p2) {this->p2 = p2;}
};

double point_line_dist (Point, Point, Point);
void include_all_points(std::vector<Point>, std::vector<Point>&);
