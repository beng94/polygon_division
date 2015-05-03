#include "polygon_division.hpp"
#include <stdio.h>
#include <unistd.h>
#include "geometria.h"
#include "graham_scan.hpp"

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

std::list<Point> include_all_points(std::list<Point>& points)
{
    std::list<Point> polygon = graham_scan(points);
    delete_points(polygon, points);

    FILE *pipe = popen("gnuplot -persist", "w");
    while(!points.empty())
    {
        std::list<Point> convex_hull = graham_scan(points);
        delete_points(convex_hull, points);

        print_list("conv_hull.txt", convex_hull);
         fprintf(pipe, "plot 'points.txt' using 1:2, 'conv_hull.txt' using 1:2 with lines, 'polygon.txt' using 1:2 with line\n");
         fflush(pipe);
         getchar();


        for(std::list<Point>::iterator i = convex_hull.begin(); i != convex_hull.end(); i++)
        {
            //find minimal distance
            std::list<Point>::iterator p1 = polygon.begin();
            std::list<Point>::iterator p2 = ++(polygon.begin());
            double dist = point_line_dist(*p1, *p2, *i);

            for(std::list<Point>::iterator j = p2; j != polygon.end(); j++)
            {
                std::list<Point>::iterator tmp_p1 = j;
                std::list<Point>::iterator tmp_p2 = ++j; --j;
                double tmp_dist = point_line_dist(*tmp_p1, *tmp_p2, *i);

                if(tmp_dist < dist)
                {
                    dist = tmp_dist;
                    //TODO: or maybe tmp_p2??
                    p1 = tmp_p1;
                }
            }

            polygon.insert(++p1, *i);
        }

        print_list("polygon.txt", polygon);

         fprintf(pipe, "plot 'points.txt' using 1:2, 'polygon.txt' using 1:2 with line\n");
         fflush(pipe);
         //usleep(500000);
         getchar();

    }

    pclose(pipe);

    /*
    FILE *pipe = popen("gnuplot -persist", "w");
    while(!dist.empty())
    {
        //finding the point with the minimal distance form segments
        std::list<Closest>::iterator min_dist = dist.begin();
        for(std::list<Closest>::iterator i = ++dist.begin(); i != dist.end(); i++)
        {
            if((*min_dist).get_dist() > (*i).get_dist())
                min_dist = i;
        }

        //insert the new point into the polygon
        polygon.insert(++(*min_dist).get_p1(), *((*min_dist).get_point())); --(*min_dist).get_p1();
        print_list("new_polygon.txt", polygon);

        if(pipe == NULL) std::cout << "fuck you";
        fprintf(pipe, "plot 'points.txt' using 1:2, 'convex_hull.txt' using 1:2 with lines, 'new_polygon.txt' using 1:2 with line\n");
        fflush(pipe);
        //usleep(500000);
        getchar();

        //delete min_dist form dist
        dist.erase(min_dist);

        std::cout << *((*min_dist).get_p1()) << std::endl;

        //update distances in dist
        for(std::list<Closest>::iterator i = dist.begin(); i != dist.end(); i++)
        {
            //check the distance from the new segments
            std::list<Point>::iterator p1 = (*min_dist).get_p1();
            std::list<Point>::iterator p2 = ++p1; --p1;
            std::list<Point>::iterator p3 = ++p2; --p2;
            double dist_1 = point_line_dist(*(p1), *(p2), *((*i).get_point()));
            double dist_2 = point_line_dist(*(p2), *(p3), *((*i).get_point()));

            if(std::min(dist_1, dist_2) < (*i).get_dist())
            {
                if(dist_1 == dist_2)
                {
                    Pont _p1 ((*p1).get_x(), (*p1).get_y());
                    Pont _p2 ((*p2).get_x(), (*p2).get_y());
                    Pont _p3 ((*p3).get_x(), (*p3).get_y());

                    Egyenes e1(_p1, _p2);

                    int x = (*(*i).get_point()).get_x();
                    int y = (*(*i).get_point()).get_y();
                    double _y1 = e1.getM()*x + e1.getC();

                    if(*p1 > *p2 && *p3 > *p2)
                    {
                        if((*p2).get_x() > (*p1).get_x())
                        {
                            if(y > _y1)
                            {
                                (*i).set_p1(p1);
                            }
                            else {
                                (*i).set_p1(p2);
                            }
                        }
                        else
                        {
                            if(y < _y1)
                            {
                                (*i).set_p1(p1);
                            }
                            else {
                                (*i).set_p1(p2);
                            }
                        }

                        std::cout << 1 << " " << *p1 << " " << dist_1 << std::endl;
                    }
                    else if(*p1 > *p2 && *p2 > *p3)
                    {
                        if((*p2).get_x() > (*p1).get_x())
                        {
                            if(y > _y1)
                            {
                                (*i).set_p1(p1);
                            }
                            else {
                                (*i).set_p1(p2);
                            }
                        }
                        else {
                            if(y < _y1)
                            {
                                (*i).set_p1(p1);
                            }
                           else {
                                (*i).set_p1(p2);
                            }
                        }

                        std::cout << 2 << " "<< *p1 << " "<< dist_1 << std::endl;
                    }
                    else if(*p2 > *p1 && *p2 > *p3)
                    {
                        if((*p2).get_x() > (*p1).get_x())
                        {
                            if(y > _y1)
                            {
                                (*i).set_p1(p1);
                            }
                            else {
                                (*i).set_p1(p2);
                            }

                        }
                        else
                        {
                            if(y < _y1)
                            {
                                (*i).set_p1(p1);
                            }
                            else {
                                (*i).set_p1(p2);
                            }
                        }
                        std::cout << 3 << " " << *p1 << " "<< dist_1 <<std::endl;
                    }
                    else if(*p2 > *p1 && *p3 > *p2)
                    {
                        if((*p2).get_x() > (*p1).get_x())
                        {
                            if(_y1 < y)
                            {
                                (*i).set_p1(p1);
                            }
                            else {
                                (*i).set_p1(p2);
                            }
                        }
                        else {
                            if(_y1 > y)
                            {
                                (*i).set_p1(p1);
                            }
                            else {
                                (*i).set_p1(p2);
                            }
                        }
                        std::cout << 4 << " "<<*p1 << " " << dist_1 <<std::endl;
                    }
                    else {
                        std::cout << "not all cases covered" << std::endl;
                    }
                    (*i).set_dist(dist_1);
                }
                else if (dist_1 < dist_2)
                {
                    (*i).set_dist(dist_1);
                    (*i).set_p1(p1);
                }
                else
                {
                    (*i).set_dist(dist_2);
                    (*i).set_p1(p2);
                }

            }
        }

    }
    pclose(pipe);
    */
}
