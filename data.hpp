#ifndef KONVEX_HULL_H
#define KONVEX_HULL_H

#include <iostream>

class Point
{
    private:
        int x, y;
    public:
        Point() {}
        Point(int x, int y) : x(x), y(y) {}

        int get_x() const { return x; }
        int get_y() const { return y; }

        bool operator== (const Point& c) const;
        bool operator> (const Point& c) const;
        Point operator+ (const Point& c) const;
        Point operator- (const Point& c) const;
        friend std::ostream& operator<< (std::ostream& os, const Point& c);
};

#endif
