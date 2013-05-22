#include <iostream>
//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>
// Core geometry
#include "../inc/PointVector2D.h"
// Circle
#include "../inc/RayIntersectableCircle.h"
// Convex Hull
#include "../inc/OutputSensitiveConvexHull.h"
#include "../inc/ConvexHullHelpers.h"

//uncomment to use in DEBUG_VERBOSE mode 
//#define DEBUG_VERBOSE

template <typename Shape, typename OutputIterator>
void convexHull(const Shape& aShape, OutputIterator res, bool aVertOnEdges)
{
  OutputSensitiveConvexHull<Shape> ch(aShape); 
  ch.all(res, aVertOnEdges);
}


///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point,  DGtal::BigInteger> CircleBig;
  typedef RayIntersectableCircle<Point> Circle;
  
  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests
  
#ifdef DEBUG_VERBOSE
  std::cout << std::endl; 
  std::cout << "1 - Convex hull on a simple circle" << std::endl; 
#endif  
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
    
#ifdef DEBUG_VERBOSE
    std::cout << "Expected" << std::endl; 
    std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 
#endif

    std::vector<Point> v; 
    convexHull( circle, std::back_inserter(v), false ); 
    
#ifdef DEBUG_VERBOSE
    std::cout << "Get" << std::endl; 
    std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 
#endif
    
    if (v.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), v.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    
#ifdef DEBUG_VERBOSE
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 
#endif
    
    std::vector<Point> mch; 
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
    
#ifdef DEBUG_VERBOSE
    std::cout << "Graham's convex hull of the boundary" << std::endl; 
    std::copy(mch.begin(), mch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 
#endif
    
    if (mch.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), mch.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }
#ifdef DEBUG_VERBOSE  
  std::cout << std::endl; 
  std::cout << "2 - Convex hull on a the same circle but another triangle orientation" << std::endl; 
#endif  
  {
    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> v; 
    convexHull( circle, std::back_inserter(v), false ); 
 
#ifdef DEBUG_VERBOSE
    std::cout << "Get" << std::endl; 
    std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 
#endif
    
    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 

    std::vector<Point> mch; 
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
 
#ifdef DEBUG_VERBOSE
    std::cout << "Graham's convex hull of the boundary" << std::endl; 
    std::copy(mch.begin(), mch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 
#endif
    
    if (mch.size() == v.size())
      if ( std::equal(v.begin(), v.end(), mch.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }

  // Random Initialisation
  srand ( time(NULL) );
  
  // Max radius size
  int maxRadius = 500;
  
  // Test number
  int nb_test = 200;
  
  // Circle parameter : ax + by + c(x^2 + y^2)   
  int a; 
  int b;
  int c;
  int d;
  c =  -25;
  
  // We fixed c = -20.  pt_c = [-a/2c ; -b/2c] and R² = (a^2 + b^2 - 4*c*d)/(4c²)
  // so 4c²*R² = a² + b² - 4*c*d <=>  = (a² + b² - 4c²*R²)/4*c = d
  int R;

#ifdef DEBUG_VERBOSE
  std::cout << std::endl; 
  std::cout << "3 - Convex hull on "<<nb_test<<" random circle" << std::endl; 
#endif
 
  {

    for (nb_test;nb_test>0;nb_test--)
      {
#ifdef DEBUG_VERBOSE
	std::cout << std::endl; 
	std::cout << " - 3."<<nb_test<<" - Convex hull on a random circle" << std::endl; 
#endif      

	// random radius between 1 and maxRadius
	R = 1 + rand() % maxRadius;

	a = - rand() %(2*c);
	b = - rand() %(2*c);  
	d = ( a*a + b*b - 4*R*R*c*c)/(4*c);

	// Create a circle from the Euclidian parameter a, b, c, d.
	Circle circle( a, b, c, d );	

#ifdef DEBUG_VERBOSE
	std::cout << "Disk[ Center : (" << circle.getCenterX() << ", " 
		  << circle.getCenterY()<< " )"; 
	std::cout << " Radius : " << circle.getRadius() << " ] "; 
	std::cout << " - First vertex : " 
		  << circle.getConvexHullVertex() << std::endl;
#endif
      
	std::vector<Point> v; 
	convexHull( circle, std::back_inserter(v), false ); 

#ifdef DEBUG_VERBOSE
	std::cout << "---Get :" << std::endl; 
	std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
	std::cout << std::endl; 
#endif
      
	std::vector<Point> boundary; 
	Vector dir(1,0); 
	closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
	std::vector<Point> mch; 
	grahamScan( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 

#ifdef DEBUG_VERBOSE
	std::cout << "---Graham's convex hull of the boundary" << std::endl; 
	std::copy(mch.begin(), mch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
	std::cout << std::endl; 
#endif
      
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
