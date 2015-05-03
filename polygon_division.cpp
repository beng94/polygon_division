#include "polygon_division.hpp"
#include <stdio.h>
#include <unistd.h>
#include "geometria.h"

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

void include_all_points(std::vector<Point> points, std::list<Point>& polygon)
{
    //Delete points existing in polygon from points
    for(std::list<Point>::iterator i = polygon.begin(); i != polygon.end(); i++)
    {
        std::vector<Point>::iterator del_i = std::find(points.begin(), points.end(), *i);
        if(del_i != points.end()) points.erase(del_i);
    }

    print_vector("del_points.txt", points);

    //Adding every point from points to dist
    std::list<Closest> dist;
    for(std::vector<Point>::iterator i = points.begin(); i!=points.end(); i++)
    {
        std::list<Point>::iterator p1 = polygon.begin();
        std::list<Point>::iterator p2 = ++p1; --p1;
        double min_dist = point_line_dist(*p1, *p2, (*i));

        for(std::list<Point>::iterator j = ++polygon.begin(); j != --polygon.end(); j++)
        {
            std::list<Point>::iterator k = ++j; --j;
            double dist = point_line_dist(*j, *k, *i);

            if(dist < min_dist)
            {
                p1 = j;
                min_dist = dist;
            }
        }

        Closest new_clo (p1, i, min_dist);
        dist.push_back(new_clo);
    }

    FILE *pipe = popen("gnuplot -persist", "w");
    while(!dist.empty())
    {
        //finding the point with the minimal distance form segments
        std::list<Closest>::iterator min_dist = dist.begin();
        for(std::list<Closest>::iterator i = ++dist.begin(); i != dist.end(); i++)
        {
            if((*min_dist).get_dist() > (*i).get_dist())
                min_dist = i;

            //TODO: what if equal?

            //--dist.begin();
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

        //update distances in dist
        for(std::list<Closest>::iterator i = dist.begin(); i != dist.end(); i++)
        {
            //calculate new max_dist if the element was closest to the newly refreshed point
            if((*i).get_p1() == (*min_dist).get_p1())
            {
                std::list<Point>::iterator seg_beg = polygon.begin();
                std::list<Point>::iterator seg_end = ++seg_beg; --seg_beg;
                double new_dist = point_line_dist(*seg_beg, *(seg_end), *((*i).get_point()));

                for(std::list<Point>::iterator j = ++polygon.begin(); j != --polygon.end(); j++)
                {
                    std::list<Point>::iterator p1= j;
                    std::list<Point>::iterator p2 = ++j; --j;
                    double dist = point_line_dist(*p1, *(p2), *((*i).get_point()));

                    if(dist == new_dist && seg_beg == p1)
                    {
                        Pont _p1 ((*p1).get_x(), (*p1).get_y());
                        Pont _p2 ((*p2).get_x(), (*p2).get_y());

                        Egyenes e1(_p1, _p2);

                        int x = (*(*i).get_point()).get_x();
                        int y = (*(*i).get_point()).get_y();
                        double _y1 = e1.getM()*x + e1.getC();

                            if(*p1 > *p2)
                            {
                                if((*p1).get_x() > (*p2).get_x())
                                {
                                    if(_y1 > y)
                                    {
                                        (*i).set_p1(p1);
                                    }
                                    else {
                                        (*i).set_p1(p2);
                                    }
                                }
                                else
                                {
                                    if(_y1 > y)
                                    {
                                        (*i).set_p1(p2);
                                    }
                                    else {
                                        (*i).set_p1(p1);
                                    }
                                }
                            }
                            else
                            {
                                if((*p1).get_x() > (*p2).get_x())
                                {
                                    if(_y1 > y)

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
                            }


                        std::cout << *((*i).get_point()) << "now what\n";
                    }

                    if(dist < new_dist)
                    {
                        seg_beg = j;
                        new_dist = dist;
                    }
                }

                (*i).set_dist(new_dist);
                (*i).set_p1(seg_beg);
            }

            //check the distance from the new segments
            else
            {
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
                        Egyenes e2(_p2, _p3);

                        int x = (*(*i).get_point()).get_x();
                        int y = (*(*i).get_point()).get_y();
                        double _y1 = e1.getM()*x + e1.getC();
                        double _y2 = e2.getM()*x + e1.getC(); //could be avoided

                        //if(*p1 > *p3)
                        //{
                            if(*p1 > *p2)
                            {
                                if((*p1).get_x() > (*p2).get_x())
                                {
                                    if(_y1 > y)
                                    {
                                        (*i).set_p1(p1);
                                    }
                                    else {
                                        (*i).set_p1(p2);
                                    }
                                }
                                else
                                {
                                    if(_y1 > y)
                                    {
                                        (*i).set_p1(p2);
                                    }
                                    else {
                                        (*i).set_p1(p1);
                                    }
                                }
                            }
                            else
                            {
                                if((*p1).get_x() > (*p2).get_x())
                                {
                                    if(_y1 > y)

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
                            }
                        std::cout << *((*i).get_point()) << "should do sth" << std::endl;
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

    }
    pclose(pipe);
}
