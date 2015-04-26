#include "graham_scan.hpp"

static Coordinate low;

static void find_lowest (std::vector<Coordinate>& vec)
{
    Coordinate lowest = vec[0];
    int id = 0;
    int len = vec.size();

    for(int i = 1; i<len; i++)
    {
        if(lowest > vec[i])
        {
            lowest = vec[i];
            id = i;
        }
    }

    vec.erase(vec.begin() + id);
    low = lowest;
}

static double distance (Coordinate a, Coordinate b)
{
    int x = a.get_x() - b.get_x();
    int y = a.get_y() - b.get_y();

    return std::sqrt(x*x + y*y);
}

static double rotation (Coordinate c)
{
    int y = c.get_y() - low.get_y();
    double angle = std::asin(y / distance(c, low)) * 180 / M_PI;

    if(c.get_x() < low.get_x()) angle += 90 - angle;

    return angle; //in rad
}

static bool cmp (Coordinate a, Coordinate b)
{
    double rot_a = rotation(a);
    double rot_b = rotation(b);

    if(rot_a == rot_b) return distance(a, low) < distance(b,low);
    return rotation(a) < rotation(b);
}

static int rotation_section(Coordinate beg, Coordinate end, Coordinate new_p)
{
    double val = (end.get_x() - beg.get_x()) *
                 (new_p.get_y() - beg.get_y()) -
                 (new_p.get_x() - beg.get_x()) *
                 (end.get_y() - beg.get_y());

    if(val < 0) return -1;
    if(val > 0) return 1;
    return 0;
}

std::stack<Coordinate> graham_scan (std::vector<Coordinate>& vec)
{
find_lowest(vec);
std::sort(vec.begin(), vec.end(), cmp);
for(std::vector<Coordinate>::iterator i = vec.begin(); i!=vec.end(); i++)
{
    std::cout << rotation(*i) << ' '<< *i << std::endl;
}

    std::stack<Coordinate> stack;
    stack.push(low);
    stack.push(vec.at(0));
    stack.push(vec.at(1));

    for(int i = 2; i<vec.size(); i++)
    {
        Coordinate new_c = vec.at(i);

        while(1)
        {
            Coordinate top = stack.top();
            stack.pop();
            Coordinate top_1 = stack.top();

            if(rotation_section(top_1, top, new_c) == 1)
            {
                stack.push(top);
                break;
            }
        }
        stack.push(new_c);
    }
    stack.push(low);

    return stack;
}
