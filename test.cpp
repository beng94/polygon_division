#include "test.hpp"
static const int X_RANGE = 5000;
static const int Y_RANGE = 5000;

static bool valid_point (const std::list<Point>& points, Point tst)
{
    if(std::find(points.begin(), points.end(), tst) == points.end())
        return true;
    return false;
}

std::list<Point> gen_data(const int n)
{
    std::srand(std::time(0));
    std::list<Point> points;

    while(points.size() != n)
    {
        int x = std::rand() % X_RANGE;
        int y = std::rand() % Y_RANGE;

        Point new_point(x, y);

        if(valid_point(points, new_point))
            points.push_back(new_point);
    }

    return points;
}

void print_vector (const char* file, const std::vector<Point>& points)
{
    std::ofstream myfile;
    myfile.open(file);

    for(std::vector<Point>::const_iterator i = points.begin(); i != points.end(); i++)
        myfile << *i << std::endl;

    myfile.close();
}

void print_list (const char* file, const std::list<Point>& points)
{
    std::ofstream myfile;
    myfile.open(file);

    for(std::list<Point>::const_iterator i = points.begin(); i != points.end(); i++)
        myfile << *i << std::endl;

    myfile.close();
}
