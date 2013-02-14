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
#include "RayIntersectableStraightLine.h"
#include "ConvexHullHelpers.h"
//#include "OutputSensitiveConvexHull.h"
#include "OutputSensitiveAlphaShape.h"




///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point> Circle;
  typedef OutputSensitiveAlphaShape<TShape> Shape; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests
  
  std::cout << std::endl; 
  std::cout << "I) Alpha-shape on a straight line" << std::endl; 
  {
  
    Shape myShape;
    
    std::vector<Vector> convergents;
    convergents.clear(); 
    
    Point aPoint(0,0); 
    Point bPoint(5,8);  

    myShape.convAlphaShape(aPoint, bPoint, std::back_inserter(convergents) );  

    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 

    if ( (convergents.size() == 4)&&(convergents.back() == aPoint) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  
  
  
  }
  
  std::cout << std::endl; 
  std::cout << "II) Alpha-shape on a simple circle" << std::endl; 
  {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
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

    std::vector<Point> ch;
    RadiusCirclePredicate predicate; //by default infinite radius (denominator = 0)  
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
    std::cout << "Graham's convex hull of the boundary" << std::endl; 
    std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (ch.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    std::cout << "  B) radius == 1" << std::endl; 

    std::cout << "Expected (boundary)" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> ch1;
    RadiusCirclePredicate predicate1(1,1); //radius 1  
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch1), predicate1 ); 
    std::cout << "1-shape of the boundary" << std::endl; 
    std::copy(ch1.begin(), ch1.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (ch1.size() == boundary.size())
      if ( std::equal(boundary.begin(), boundary.end(), ch1.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    std::cout << "  B) radius == 3" << std::endl; 

    std::vector<Point> groundTruth3; 
    groundTruth3.push_back(Point(0,-5)); 
    groundTruth3.push_back(Point(2,-4)); 
    groundTruth3.push_back(Point(3,-4)); 
    groundTruth3.push_back(Point(4,-3)); 
    groundTruth3.push_back(Point(4,-2)); 
    groundTruth3.push_back(Point(5,0));
    groundTruth3.push_back(Point(4,2));  
    groundTruth3.push_back(Point(4,3));
    groundTruth3.push_back(Point(3,4)); 
    groundTruth3.push_back(Point(2,4)); 
    groundTruth3.push_back(Point(0,5)); 
    groundTruth3.push_back(Point(-2,4)); 
    groundTruth3.push_back(Point(-3,4)); 
    groundTruth3.push_back(Point(-4,3)); 
    groundTruth3.push_back(Point(-4,2)); 
    groundTruth3.push_back(Point(-5,0)); 
    groundTruth3.push_back(Point(-4,-2)); 
    groundTruth3.push_back(Point(-4,-3));
    groundTruth3.push_back(Point(-3,-4));
    groundTruth3.push_back(Point(-2,-4)); 
    std::cout << "Expected" << std::endl; 
    std::copy(groundTruth3.begin(), groundTruth3.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> ch3;
    RadiusCirclePredicate predicate3(9,1); //radius 3  
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch3), predicate3 ); 
    std::cout << "3-shape of the boundary" << std::endl; 
    std::copy(ch3.begin(), ch3.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (ch3.size() == groundTruth3.size())
      if ( std::equal(groundTruth3.begin(), groundTruth3.end(), ch3.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }

  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
