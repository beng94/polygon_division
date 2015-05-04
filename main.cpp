#include "test.hpp"
#include "graham_scan.hpp"
#include "polygon_division.hpp"

int main(void)
{
    //Generating points
    std::list<Point> points = gen_data(500);
    print_list("points.txt", points);

    //Include all points in the concave polygon
    std::list<Point> polygon = include_all_points(points);
    print_list("concave_polygon.txt", polygon);

    std::cout << "Program terminated" << std::endl;
    return 0;
}
