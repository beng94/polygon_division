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
        std::vector<Point>::iterator p1 = polygon.begin();
        double min_dist = point_line_dist(*p1, *(p1+1), (*i));

        for(std::vector<Point>::iterator j = polygon.begin() + 1; j != polygon.end() - 1; j++)
        {
            double dist = point_line_dist(*j, *(j+1), *i);

            if(dist < min_dist)
            {
                p1 = j;
                min_dist = dist;
            }
        }

        Closest new_clo (p1, i, min_dist);
        dist.push_back(new_clo);
    }

    while(!dist.empty())
    {
        //finding the point with the minimal distance form segments
        std::vector<Closest>::iterator min_dist = dist.begin();
        for(std::vector<Closest>::iterator i = dist.begin() + 1; i != dist.end(); i++)
        {
            if((*min_dist).get_dist() > (*i).get_dist())
                min_dist = i;
        }

        //insert the new point into the polygon
        polygon.insert((*min_dist).get_p1(), *((*min_dist).get_point()));

        //update distances in dist
        for(std::vector<Closest>::iterator i = dist.begin(); i != dist.end(); i++)
        {
            //calculate new max_dist if the element was closest to the newly refreshed point
            if((*i).get_p1() == (*min_dist).get_p1())
            {
                std::vector<Point>::iterator seg_beg = polygon.begin();
                double new_dist = point_line_dist(*seg_beg, *(seg_beg+1), *((*i).get_point()));

                for(std::vector<Point>::iterator j = polygon.begin() + 1; j != polygon.end() - 1; j++)
                {
                    double dist = point_line_dist(*j, *(j+1), *((*i).get_point()));

                    if(dist < new_dist)
                    {
                        seg_beg = j;
                        new_dist = dist;
                    }
                }
            }

            //check the distance from the new segments
            else
            {
                double dist_1 = point_line_dist(*((*min_dist).get_p1()), *((*min_dist).get_p1() + 1), *((*i).get_point()));
                double dist_2 = point_line_dist(*((*min_dist).get_p1() + 1), *((*min_dist).get_p1() + 2), *((*i).get_point()));

                if(std::min(dist_1, dist_2) < (*i).get_dist())
                {
                    if(dist_1 < dist_2)
                    {
                        (*i).set_dist(dist_1);
                        (*i).set_p1((*min_dist).get_p1());
                    }
                    else
                    {
                        (*i).set_dist(dist_2);
                        (*i).set_p1((*min_dist).get_p1() + 1);
                    }
                }
            }
        }

        //delete min_dist form dist
        dist.erase(min_dist);
    }
}
