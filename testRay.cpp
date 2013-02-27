#include <iostream>
#include <math.h>
#include "PointVector2D.h"
#include "RayIntersectableStraightLine.h"
#include "RayIntersectableCircle.h"

int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  std::cout << "testRayIntersectableStraightLine (0,0) - (8,5)" << std::endl; 
  {
    typedef RayIntersectableStraightLine<Point> SL; 
    SL line(Point(0,0), Point(8,5));

    //constructor
    if ( (line.a() == -5)&&(line.b() == 8)&&(line.c() == 0) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    //point functor
    if ( (line(Point(8,0)) < 0)&&(line(Point(8,5)) == 0)&&(line(Point(8,10)) > 0) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    //digital ray
    int qMax = 0; 
    Point ptMax(0,0); 

    if ( line.dray(Point(0,0), Vector(1,1), qMax, ptMax ) ) 
      if ( (qMax == 0)&&(ptMax == Point(0,0)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( line.dray(Point(0,1), Vector(1,1), qMax, ptMax ) == false )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( line.dray(Point(3,0), Vector(1,1), qMax, ptMax ) )
      if ( (qMax == 5)&&(ptMax == Point(8,5)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( line.dray(Point(2,0), Vector(1,1), qMax, ptMax ) )
      if ( (qMax == 3)&&(ptMax == Point(5,3)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( line.dray(Point(1,0), Vector(1,1), qMax, ptMax ) )
      if ( (qMax == 1)&&(ptMax == Point(2,1)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( line.dray(Point(2,1), Vector(3,2), qMax, ptMax ) )
      if ( (qMax == 2)&&(ptMax == Point(8,5)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( line.dray(Point(0,1), Vector(1,0), qMax, ptMax ) )
      if ( (qMax == 1)&&(ptMax == Point(1,1)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }
  std::cout << "testRayIntersectableStraightLine (1,1) - (9,6)" << std::endl; 
  {
    typedef RayIntersectableStraightLine<Point> SL; 
    SL line(Point(1,1), Point(9,6));

    //constructor
    if ( (line.a() == -5)&&(line.b() == 8)&&(line.c() == -3) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    //point functor
    if ( (line(Point(8,0)) < 0)&&(line(Point(9,6)) == 0)&&(line(Point(8,10)) > 0) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }

  std::cout << "testRayIntersectableCircle" << std::endl; 
  {
    typedef RayIntersectableCircle<Point> Circle; 

    //////////////////////////////////////////////////////////////////////////////////
    //constructor from 3 points
    Point P(5,0); 
    Point Q(0,5); 
    Point R(-5,0); 
    Circle circle(P,Q,R);

    //circle of center (0,0) and radius 5
    //std::cout << circle.getCenterX() << " " << circle.getCenterY() << " " << circle.getRadius() << std::endl; 
    double epsilon = 0.000001; 
    if ( ((circle.getCenterX() - 0)<= epsilon)&&((circle.getCenterY() - 0)<= epsilon)
        &&((circle.getRadius() - 5)<= epsilon) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    //parameters a,b,c,d
    //std::cout << circle.a() << " " << circle.b() << " " << circle.c() << " " << circle.d() << std::endl; 
    Vector u = Q - P; 
    Vector v = R - P; 
    int det = u[0]*v[1]-u[1]*v[0]; 
    //std::cout << circle.a() << " " << circle.b() << " " << -det << " " << -25*circle.c() << std::endl; 
    //c is equal to -det, 
    //-a/(2c) (resp. -b/(2c)) are the x- (resp. y-) coordinate of the center, ie. 0 
    //d/c is the power of the origin with respect to the circle, ie. 25 
    if ( (circle.c() == -det)&&(circle.a() == 0)&&(circle.b() == 0)&&(circle.d() == -25*circle.c()) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    //point functor
    if ( (circle(Point(0,0)) > 0)&&(circle(Point(5,0)) == 0)
        &&(circle(Point(3,4)) == 0)&&(circle(Point(6,1)) < 0) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    //////////////////////////////////////////////////////////////////////////////////
    //constructor from 3 points
    Point P2(6,1); 
    Point Q2(1,6); 
    Point R2(-4,1); 
    Circle circle2(P2,Q2,R2);

    //circle of center (0,0) and radius 5
    //std::cout << circle2.getCenterX() << " " << circle2.getCenterY() << " " << circle2.getRadius() << std::endl; 
    if ( ((circle2.getCenterX() - 1)<= epsilon)&&((circle2.getCenterY() - 1)<= epsilon)
        &&((circle2.getRadius() - 5)<= epsilon) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    //parameters a,b,c,d
    //std::cout << circle2.a() << " " << circle2.b() << " " << circle2.c() << " " << circle2.d() << std::endl; 
    Vector u2 = Q2 - P2; 
    Vector v2 = R2 - P2; 
    det = u2[0]*v2[1]-u2[1]*v2[0]; 
    //std::cout << circle2.a() << " " << circle2.b() << " " << -det << " " << circle2.c()*(2-25) << std::endl; 
    //c is equal to -det, 
    //-a/(2c) (resp. -b/(2c)) are the x- (resp. y-) coordinate of the center, ie. 1 
    //d/c is the power of the origin with respect to the circle, ie. 2-25 
    if ( (circle2.c() == -det)&&(circle2.a() == -2*circle.c())&&(circle2.b() == -2*circle.c())
        &&(circle2.d() == (2-25)*circle2.c()) ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    //////////////////////////////////////////////////////////////////////////////////
    //digital ray
    int qMax = 0; 
    Point ptMax(0,0); 
    // No intersection
    if ( circle.dray(Point(6,0), Vector(1,1), qMax, ptMax ) == false ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    if ( circle.dray(Point(7,2), Vector(2,0), qMax, ptMax ) == false ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    if ( circle.dray(Point(5,2), Vector(2,0), qMax, ptMax ) == false ) 
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    // Intersection, aS lie on the circle
    if ( circle.dray(Point(3,4), Vector(1,1), qMax, ptMax ) )
      if ( (qMax == 0)&&(ptMax == Point(3,4)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( circle.dray(Point(-5,0), Vector(1,1), qMax, ptMax ) )
      if ( (qMax == 0)&&(ptMax == Point(-5,0)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    if ( circle.dray(Point(5,0), Vector(1,0), qMax, ptMax ) )
      if ( (qMax == 0)&&(ptMax == Point(5,0)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    // Intersection, aS inside the circle
    if ( circle.dray(Point(2,2), Vector(1,1), qMax, ptMax ) )
      if ( (qMax == 1)&&(ptMax == Point(3,3)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    if ( circle.dray(Point(2,2), Vector(-1,-1), qMax, ptMax ) )
      if ( (qMax == 5)&&(ptMax == Point(-3,-3)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    // Intersection, aS outside the circle
    if ( circle.dray(Point(8,0), Vector(-2,0), qMax, ptMax ) )
      if ( (qMax == 1)&&(ptMax == Point(6,0)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    if ( circle.dray(Point(5,6), Vector(-1,-1), qMax, ptMax ) )
      if ( (qMax == 2)&&(ptMax == Point(3,4)) ) 
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    /////////////////////////////////////////////////////////////// 
    // vertex of convex hull
    if ( circle.getConvexHullVertex() == Point(0,-5) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    Circle circle3(Point(0,0), Point(5,1), Point(5,3));
    if ( circle3.getConvexHullVertex() == Point(4,0) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    Circle circle4(Point(0,0), Point(5,1), Point(6,3));
    if ( circle4.getConvexHullVertex() == Point(3,0) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  }

  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
