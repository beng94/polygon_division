#include "test.hpp"
#include "graham_scan.hpp"

int main(void)
{
    std::vector<Coordinate> vec;
    vec = gen_data(20);

    write_data("out.txt", vec);

    std::stack<Coordinate> stack;
    stack = graham_scan(vec);

    while(!stack.empty())
    {
        std::cout << stack.top() << std::endl;
        stack.pop();
    }

    std::cout << "Seems to be working" << std::endl;
    return 0;
}
