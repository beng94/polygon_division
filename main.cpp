#include "test.hpp"
#include "graham_scan.hpp"

int main(void)
{
    std::vector<Coordinate> vec;
    vec = gen_data(20);

    write_data_vector("points.txt", vec);

    std::stack<Coordinate> stack;
    stack = graham_scan(vec);

    write_data_stack("convex_hall.txt", stack);

    std::cout << "Seems to be working" << std::endl;
    return 0;
}
