#include "graham_scan.hpp"

static Point low;

//Finds the lowest point according to the y coordinate, if there are
//multiple points with the same y coordinate, selects the one with the
//smaller x coordinate (operator>)
static void find_lowest (std::vector<Point>& points)
{
    low = points.at(0);

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        if(low > *i) low = *i;

    //TODO: Should I delete the lowest point from points?
    std::vector<Point>::iterator low_id = std::find(points.begin(), points.end(), low);
    points.erase(low_id);
}

//Returns the distance between 2 points
static double distance (Point a, Point b)
{
    int x = a.get_x() - b.get_x();
    int y = a.get_y() - b.get_y();

    return std::sqrt(x*x + y*y);
}

//Returns a degree under which the given c point can be seen from the
//lowest point (low)
static double rotation (Point c)
{
    int y = c.get_y() - low.get_y();
    double angle = std::asin(y / distance(c, low)) * 180 / M_PI;

    if(c.get_x() < low.get_x()) angle = 180 - angle;

    return angle; //in rad
}

//Return whethr the new point is to the left from the given section or
//to the right or they are at the same line.
static int rotation_section(Point beg, Point end, Point new_p)
{
    double val = (end.get_x() - beg.get_x()) *
                 (new_p.get_y() - beg.get_y()) -
                 (new_p.get_x() - beg.get_x()) *
                 (end.get_y() - beg.get_y());

    if(val < 0) return -1;
    if(val > 0) return 1;
    return 0;
}

static bool cmp (Point a, Point b)
{
    double rot_a = rotation(a);
    double rot_b = rotation(b);

    if(rot_a == rot_b) return distance(a, low) < distance(b,low);
    return rotation(a) < rotation(b);
}

//Implements graham scan, return a vector containing the convex hall
std::vector<Point> graham_scan (std::vector<Point> points)
{
    find_lowest(points);
    std::sort(points.begin(), points.end(), cmp);

    std::vector<Point> stack;
    stack.push_back(low);
    stack.push_back(points.at(0));
    stack.push_back(points.at(1));

    for(int i = 2; i<points.size(); i++)
    {
        Point new_c = points.at(i);

        while(1)
        {
            std::vector<Point>::iterator i = stack.end();
            Point top = *(--i); //does not get the right value
            Point top_1 = *(--i); //but it gets

            if(rotation_section(top_1, top, new_c) == 1)
            {
                break;
            }

            stack.erase(stack.end());
        }
        stack.push_back(new_c);
    }
    stack.push_back(low);

    return stack;
}
