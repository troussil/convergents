#include <iostream>
//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>

#include "PointVector2D.h"
#include "RayIntersectableCircle.h"
#include "OutputSensitiveConvexHull.h"
#include "ConvexHullHelpers.h"

//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename OutputIterator>
void convexHull(const Shape& aShape, const Point& aStartingPoint, 
    OutputIterator res)
{
  OutputSensitiveConvexHull<Shape> ch(aShape); 
  //get the first vertex
  Point tmp = aStartingPoint; 
  do {
    //store the current vertex
    *res++ = tmp; 
    //get the next vertex
    tmp = ch.next(tmp); 
    //while it is not the first one
  } while (tmp != aStartingPoint); 
}


///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point> Circle; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests
  std::cout << std::endl; 
  std::cout << "1 - Convex hull on a simple circle" << std::endl; 
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
    std::cout << "Expected" << std::endl; 
    std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 


    std::vector<Point> v; 
    convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
    std::cout << "Get" << std::endl; 
    std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (v.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), v.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> mch; 
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
    std::cout << "Graham's convex hull of the boundary" << std::endl; 
    std::copy(mch.begin(), mch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (mch.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), mch.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }
  std::cout << std::endl; 
  std::cout << "2 - Convex hull on a the same circle but another triangle orientation" << std::endl; 
  {
    Circle circle( Point(-5,0), Point(0,5), Point(5,0) );

    std::vector<Point> v; 
    convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
    std::cout << "Get" << std::endl; 
    std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 

    std::vector<Point> mch; 
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
    std::cout << "Graham's convex hull of the boundary" << std::endl; 
    std::copy(mch.begin(), mch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (mch.size() == v.size())
      if ( std::equal(v.begin(), v.end(), mch.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }

  srand ( time(NULL) );
  int max = 200;
  int nb_test = 10;

  std::cout << std::endl; 
  std::cout << "3 - Convex hull on "<<nb_test<<" random circle" << std::endl; 
  {

    for (nb_test;nb_test>0;nb_test--)
    {

      std::cout << std::endl; 
      std::cout << " - 3."<<nb_test<<" - Convex hull on a random circle" << std::endl; 

      Circle circle( Point((rand() % (2*max) -max),(rand() % (2*max) -max)), 
          Point((rand() % (2*max) -max),(rand() % (2*max) -max)), 
          Point((rand() % (2*max) -max),(rand() % (2*max) -max)) );

      std::cout << "Disk[ (" << circle.getCenterX() << ", " << circle.getCenterY()<< " ), " 
		<< circle.getRadius()<<" ] | aStartingPoint : "
		<< circle.getConvexHullVertex() << std::endl;

      std::vector<Point> v; 
      convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
      std::cout << "---Get :" << std::endl; 
      std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> boundary; 
      Vector dir(1,0); 
      tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
      std::vector<Point> mch; 
      grahamScan( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
      std::cout << "---Graham's convex hull of the boundary" << std::endl; 
      std::copy(mch.begin(), mch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (mch.size() == v.size())
        if ( std::equal(v.begin(), v.end(), mch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }
  }


  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
