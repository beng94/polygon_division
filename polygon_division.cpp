#include "polygon_division.hpp"
#include <stdio.h>
#include <unistd.h>
#include "geometria.h"
#include "graham_scan.hpp"
#include <limits>

static double point_point_dist (Point p1, Point p2)
{
    double x = p1.get_x() - p2.get_x();
    double y = p1.get_y() - p2.get_y();

    return std::sqrt(x*x + y*y);
}

static double section_length (Point p1, Point p2)
{
    double x = p1.get_x() - p2.get_x();
    double y = p1.get_y() - p2.get_y();

    return (x*x + y*y);
}

static double dot_prod (Point& p1, Point& p2)
{
    return (p1.get_x() * p2.get_x() + p1.get_y() * p2.get_y());
}

double sqr(double x) {return x*x;}
double dist2(Point v, Point w)
{
    return sqr((double)v.get_x() - (double)w.get_x()) + sqr((double)v.get_y() - (double)w.get_y());
}
//TODO: doesn't find min distance from segment
double point_line_dist1 (Point p1, Point p2, Point point)
{
    double l2 = dist2(p1, p2);
    if(l2 == 0.0) return dist2(p1, point);
    double t = (double)((point.get_x() - p1.get_x()) * (p2.get_x() - p1.get_x()) +
                (point.get_y() - p1.get_y()) * (p2.get_y() - p1.get_y())) / l2;

    if(t < 0.0)
    {
        return dist2(point, p1);
    }
    if(t > 1.0)
    {
        return dist2(point, p2);
    }

    double p1_x = (double)p1.get_x();
    double p1_y = (double)p1.get_y();
    double p2_x = (double)p2.get_x();
    double p2_y = (double)p2.get_y();

    double x = p1_x + t*(p2_x - p1_x);
    double y = p1_y + t*(p2_y - p1_y);

    double fck = sqr(point.get_x() - x) + sqr(point.get_y() - y);
    return fck;
}

double point_line_dist(Point p1, Point p2, Point point)
{
    return std::sqrt(point_line_dist1(p1, p2, point));
}

static bool cmp (Closest a, Closest b)
{
    return (a.get_dist() < b.get_dist());
}

static void delete_points(const std::list<Point> del_points, std::list<Point>& points)
{
    for(std::list<Point>::const_iterator i = del_points.begin(); i != del_points.end(); i++)
    {
        for(std::list<Point>::iterator j = points.begin(); j != points.end(); j++)
        {
            if(*i == *j)
            {
                points.erase(j);
                break;
            }
        }
    }
}

//Checks whether point can be connected to the p1-p2 segment
static bool valid_segment (Point p1, Point p2, Point p3, Point point)
{
    Pont _p1 ((p1).get_x(), (p1).get_y());
    Pont _p2 ((p2).get_x(), (p2).get_y());
    Pont _p3 ((p3).get_x(), (p3).get_y());

    Egyenes e1(_p1, _p2);

    int x = point.get_x();
    int y = point.get_y();
    double _y1 = e1.getM()*x + e1.getC();

    if(p1 > p2 && p3 > p2)
    {
        std::cout << point<< " " << 1 << " " << p1 <<  std::endl;
        if((p2).get_x() > (p1).get_x())
        {
            if(y > _y1)
            {
                return true;
            }
            else {
                return false;
            }
        }
        else
        {
            if(y < _y1)
            {
                return true;
            }
            else {
                return false;
            }
        }
    }
    else if(p1 > p2 && p2 > p3)
    {
        std::cout << point << " " << 2 << " " << p1 << std::endl;
        if((p2).get_x() > (p1).get_x())
        {
            if(y > _y1)
            {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            if(y < _y1)
            {
                return true;
            }
           else {
                return false;
            }
        }
    }
    else if(p2 > p1 && p2 > p3)
    {
        std::cout << point << " " << 3 << " " << p1 << std::endl;
        if((p2).get_x() > (p1).get_x())
        {
            if(y > _y1)
            {
                return true;
            }
            else {
                return false;
            }

        }
        else
        {
            if(y < _y1)
            {
                return true;
            }
            else {
                return false;
            }
        }
    }
    else if(p2 > p1 && p3 > p2)
    {
        std::cout << point << " " << 4 << " " << p1 << std::endl;
        if((p2).get_x() > (p1).get_x())
        {
            if(_y1 < y)
            {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            if(_y1 > y)
            {
                return true;
            }
            else {
                return false;
            }
        }
    }
    else {
        std::cout << "not all cases covered" << std::endl;
        getchar();
        return true;
    }
}

std::list<Point> include_all_points(std::list<Point>& points)
{
    std::list<Point> polygon = graham_scan(points);
    delete_points(polygon, points);

    FILE *pipe = popen("gnuplot -persist", "w");
    while(points.size() >= 3)
    {
        std::list<Point> convex_hull = graham_scan(points);
        delete_points(convex_hull, points);

        for(std::list<Point>::iterator i = convex_hull.begin(); i != --convex_hull.end(); i++)
        {
            //find minimal distance
            std::list<Point>::iterator p1;
            double dist = std::numeric_limits<double>::infinity();
            double seg_length = std::numeric_limits<double>::infinity();

            for(std::list<Point>::iterator j = polygon.begin(); j != polygon.end(); j++)
            {
                std::list<Point>::iterator tmp_p1 = j;
                std::list<Point>::iterator tmp_p2 = ++j; --j;
                //TODO: might cause seg_fault if it's at the end of the polygon
                std::list<Point>::iterator tmp_p3 = ++tmp_p2; --tmp_p2;
                double tmp_dist = point_line_dist(*tmp_p1, *tmp_p2, *i);
                double tmp_seg_lengths = point_point_dist(*tmp_p1, *i) + point_point_dist(*tmp_p2, *i);

                if(tmp_dist < dist && tmp_seg_lengths < seg_length)
                {
                    if(tmp_dist < std::min(point_point_dist(*tmp_p1, *i), point_point_dist(*tmp_p2, *i)))
                    {
                        dist = tmp_dist;
                        p1 = tmp_p1;
                    }
                    else if(valid_segment(*tmp_p1, *tmp_p2, *tmp_p3, *i))
                    {
                        dist = tmp_dist;
                        p1 = tmp_p1;
                    }
                }
            }

            polygon.insert(++p1, *i);

            /*
            print_list("polygon.txt", polygon);
            print_list("convex_hull.txt", convex_hull);
            fprintf(pipe, "plot 'points.txt' using 1:2, 'convex_hull.txt' using 1:2 with lines, 'polygon.txt' using 1:2 with line\n");
            fflush(pipe);
            getchar();
            */
        }

        print_list("polygon.txt", polygon);

        //fprintf(pipe, "plot 'points.txt' using 1:2, 'polygon.txt' using 1:2 with line\n");
        //fflush(pipe);
        //usleep(500000);
        //getchar();
    }

    if(!points.empty())
    {
        for(std::list<Point>::iterator i = points.begin(); i != points.end(); i++)
        {
            //find minimal distance
            std::list<Point>::iterator p1;
            double dist = std::numeric_limits<double>::infinity();
            double seg_length = std::numeric_limits<double>::infinity();

            for(std::list<Point>::iterator j = polygon.begin(); j != polygon.end(); j++)
            {
                std::list<Point>::iterator tmp_p1 = j;
                std::list<Point>::iterator tmp_p2 = ++j; --j;
                //TODO: might cause seg_fault if it's at the end of the polygon
                std::list<Point>::iterator tmp_p3 = ++tmp_p2; --tmp_p2;
                double tmp_dist = point_line_dist(*tmp_p1, *tmp_p2, *i);
                double tmp_seg_lengths = point_point_dist(*tmp_p1, *i) + point_point_dist(*tmp_p2, *i);



                if(tmp_dist < dist && tmp_seg_lengths < seg_length)
                {
                    if(tmp_dist < std::min(point_point_dist(*tmp_p1, *i), point_point_dist(*tmp_p2, *i)))
                    {
                        dist = tmp_dist;
                        p1 = tmp_p1;
                    }
                    else if(valid_segment(*tmp_p1, *tmp_p2, *tmp_p3, *i))
                    {
                        dist = tmp_dist;
                        p1 = tmp_p1;
                    }
                }
            }

            polygon.insert(++p1, *i);
        }

    }

    print_list("polygon.txt", polygon);

    fprintf(pipe, "plot 'polygon.txt' using 1:2 with line\n");
    fflush(pipe);
    //usleep(500000);
    //getchar();

    pclose(pipe);

    return polygon;
}
