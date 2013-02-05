#include <iostream>
#include <vector>
#include <iterator>

#include "PointVector2D.h"
#include "RayIntersectableCircle.h"
#include "OutputSensitiveConvexHull.h"

template <typename Shape, typename Point, typename OutputIterator>
void convexHull(const Shape& aShape, const Point& aStartingPoint, 
                OutputIterator res)
{
  OutputSensitiveConvexHull<Shape> ch(aShape); 
  //store the first vertex
  Point tmp = aStartingPoint; 
  *res++; 
  do {
    //store the next vertex
    tmp = ch.next(tmp); 
    *res++ = tmp; 
    //while it is not the first one
  } while (tmp != aStartingPoint); 
}



int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point> Circle; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  std::cout << "convex hull on a simple circle" << std::endl; 
  {
    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> groundTruth; 
    groundTruth.push_back(Point(0,-5)); 
    groundTruth.push_back(Point(3,-4)); 
    groundTruth.push_back(Point(4,-3)); 
    groundTruth.push_back(Point(5,0)); 
    groundTruth.push_back(Point(4,3));
    groundTruth.push_back(Point(3,4)); 
    groundTruth.push_back(Point(0,5)); 
    groundTruth.push_back(Point(-3,4)); 
    groundTruth.push_back(Point(-4,3)); 
    groundTruth.push_back(Point(-5,0)); 
    groundTruth.push_back(Point(-4,-3));
    groundTruth.push_back(Point(-3,-4)); 
    std::cout << "expected" << std::endl; 
    std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

 
    std::vector<Point> v; 
    convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
    std::cout << "get" << std::endl; 
    std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (v.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), v.begin()) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }


  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
