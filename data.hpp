#ifndef KONVEX_HULL_H
#define KONVEX_HULL_H

#include <iostream>

class Coordinate
{
    private:
        int x, y;
    public:
        Coordinate() {}
        Coordinate(int x, int y) : x(x), y(y) {}

        int get_x() const { return x; }
        int get_y() const { return y; }

        bool operator== (const Coordinate& c) const;
        bool operator> (const Coordinate& c) const;
        friend std::ostream& operator<< (std::ostream& os, const Coordinate& c);
};

#endif
