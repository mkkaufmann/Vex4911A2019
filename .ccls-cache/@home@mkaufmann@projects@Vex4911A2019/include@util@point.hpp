#ifndef POINT_HPP
#define POINT_HPP
//models a cartesian point
class Point{
  public:
    Point(double x, double y);

    //return separate point as the sum of this point and another
    Point add(Point other);

    //add another point to this point
    void addLocally(Point other);

    double getX();
    
    double getY();
  private:
    double x;
    double y;
};
#endif
