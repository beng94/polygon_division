#include "test.hpp"
#include "graham_scan.hpp"
#include "polygon_division.hpp"

int main(void)
{
    //Generating points
    std::vector<Point> vec = gen_data(1500);
    print_vector("points.txt", vec);

    //Finding the convex hall
    std::vector<Point> stack = graham_scan(vec);
    print_vector("convex_hall.txt", stack);

    //Include all points in the concave polygon
    include_all_points(vec, stack);
    print_vector("concav_polygon.txt", stack);

    std::cout << "Program terminated" << std::endl;
    return 0;
}
