#include "data.hpp"

bool Point::operator== (const Point& c) const
{
    if(this->x == c.x && this->y == c.y) return true;
    return false;
}

bool Point::operator> (const Point& c) const
{
    if(this->y > c.get_y()) return true;
    if(this->y == c.get_y())
    {
        if(this->x > c.get_x()) return true;
    }

    return false;
}

Point Point::operator+ (const Point& c) const
{
    Point out(this->x + c.get_x(), this->y + c.get_y());
    return out;
}

Point Point::operator- (const Point& c) const
{
    Point out(this->x - c.get_x(), this->y - c.get_y());
    return out;
}


std::ostream& operator<< (std::ostream& os, const Point& c)
{
    os << c.x << ' ' << c.y;

    return os;
}
