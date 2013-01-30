#include <iostream>
#include <math.h>
#include "PointVector2D.h"
#include "RayIntersectableStraightLine.h"

int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  std::cout << "testRayIntersectableStraightLine" << std::endl; 
  {
    typedef RayIntersectableStraightLine<Point> SL; 
    SL line(Point(0,0), Point(8,5));
 
    //constructor
    if ( (line.a() == 8)&&(line.b() == 5)&&(line.c() == 0) ) 
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
    if ( line.dray(Point(0,0), Vector(1,1), qMax, ptMax ) == false ) 
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


  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
