#include "test.hpp"
static const int X_RANGE = 5000;
static const int Y_RANGE = 5000;

static bool valid_point (const std::vector<Point>& points, Point tst)
{
    if(std::find(points.begin(), points.end(), tst) == points.end())
        return true;
    return false;
}

std::vector<Point> gen_data(const int n)
{
    std::srand(std::time(0));
    std::vector<Point> vec;

    while(vec.size() != n)
    {
        int x = std::rand() % X_RANGE;
        int y = std::rand() % Y_RANGE;

        Point new_point(x, y);

        if(valid_point(vec, new_point))
            vec.push_back(new_point);
    }

    return vec;
}

void print_vector (const char* file, const std::vector<Point>& points)
{
    std::ofstream myfile;
    myfile.open(file);

    for(std::vector<Point>::const_iterator i = points.begin(); i != points.end(); i++)
        myfile << *i;

    myfile.close();
}
