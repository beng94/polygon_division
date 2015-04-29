#include "polygon_division.hpp"

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
double dist2(Point v, Point w) {return sqr(v.get_x() - w.get_x()) + sqr(v.get_y() - w.get_y());}
//TODO: doesn't find min distance from segment
double point_line_dist1 (Point p1, Point p2, Point point)
{
    double l2 = dist2(p1, p2);
    if(l2 == 0.0) return dist2(p1, point);
    double t = ((point.get_x() - p1.get_x()) * (p2.get_x() - p1.get_x()) +
                (point.get_y() - p1.get_y()) * (p2.get_y() - p1.get_y())) / l2;

    if(t < 0.0) return dist2(point, p1);
    if(t > 1.0) return dist2(point, p2);

    Point c (p1.get_x() + t * (p2.get_x() - p1.get_x()),
                  p1.get_y() + t * (p2.get_y() - p1.get_y()));

    return dist2(point, c);
}

double point_line_dist(Point p1, Point p2, Point point)
{
    return std::sqrt(point_line_dist1(p1, p2, point));
}

static bool cmp (Closest a, Closest b)
{
    return (a.get_dist() < b.get_dist());
}

void include_all_points(std::vector<Point> points, std::vector<Point>& polygon)
{
    //Delete points existing in polygon from points
    for(std::vector<Point>::iterator i = polygon.begin(); i != polygon.end(); i++)
    {
        std::vector<Point>::iterator del_i = std::find(points.begin(), points.end(), *i);
        points.erase(del_i);
    }

    //Adding every point from points to dist
    std::vector<Closest> dist;
    for(std::vector<Point>::iterator i = points.begin(); i!=points.end(); i++)
    {
        Point p1 = *(polygon.begin());
        Point p2 = *(polygon.begin() + 1);
        double min_dist = point_line_dist(p1, p2, (*i));

        for(std::vector<Point>::iterator j = polygon.begin() + 1; j != polygon.end(); j++)
        {
            Point seg_beg = *j;
            Point seg_end = *(j + 1);
            double dist = point_line_dist(seg_beg, seg_end, *i);

            if(dist < min_dist)
            {
                p1 = seg_beg;
                p2 = seg_end;
                min_dist = dist;
            }
        }

        Closest new_clo (p1, p2, *i, min_dist);
        dist.push_back(new_clo);
    }

    //Sort points by distance, increasing
    std::sort(dist.begin(), dist.end(), cmp);

    while(!dist.empty())
    {
        //TODO: should store iterator in closest, wouldn't have to go through the array
        Point p1 = dist.at(0).get_p1();
        for(std::vector<Point>::iterator i = polygon.begin(); i != polygon.end(); i++)
        {
            if((*i) == p1)
            {
                //++i; //the element will be inserted before the given iterator
                polygon.insert(i, dist.at(0).get_point());
                print_vector("test.txt", polygon);

                //refresh distances
                for(std::vector<Closest>::iterator j = dist.begin() + 1; j!= dist.end(); j++)
                {
                    //i points to the end of the divided segment
                    Point p3 = *(i);
                    Point p2 = *(i-1);
                    Point p1 = *(i-2);

                    double dist_1 = point_line_dist(p3, p2, (*j).get_point());
                    double dist_2 = point_line_dist(p2, p1, (*j).get_point());
                    if(std::min(dist_1, dist_2) < (*j).get_dist())
                    {
                        if(dist_1 < dist_2)
                        {
                            (*j).set_dist(dist_1);
                            (*j).set_p1(*(i+1));
                            (*j).set_p2(*(i+2));
                        }
                        else
                        {
                            (*j).set_dist(dist_2);
                            (*j).set_p1(*i);
                            (*j).set_p2(*(i+1));
                        }
                    }
                }

                //sort the refreshed element
                std::sort(dist.begin(), dist.end(), cmp);

                break;
            }
        }

        dist.erase(dist.begin());
    }
}
