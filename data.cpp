#include "data.hpp"

bool Coordinate::operator== (const Coordinate& c) const
{
    if(this->x == c.x && this->y == c.y) return true;
    return false;
}

bool Coordinate::operator> (const Coordinate& c) const
{
    if(this->y > c.get_y()) return true;
    if(this->y == c.get_y())
    {
        if(this->x > c.get_x()) return true;
    }

    return false;
}

std::ostream& operator<< (std::ostream& os, const Coordinate& c)
{
    os << c.x << ' ' << c.y << std::endl;

    return os;
}
