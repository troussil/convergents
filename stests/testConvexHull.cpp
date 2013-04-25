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
  typedef RayIntersectableCircle<Point,  DGtal::BigInteger> Circle; 

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
    convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
    
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
    convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
 
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

  srand ( time(NULL) );
  int max = 50;
  int nb_test = 500;
  
  Point pta;
  Point ptb;
  Point ptc;
  
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

      pta = Point( (rand() % max)             , (rand() % max) );
      ptb = Point( (pta[0]-1- (rand() % max) ), (pta[1]-1- (rand() % max)) );
      ptc = Point( (ptb[0]+1+ (rand() % max) ), (ptb[1]-1- (rand() % max)) );

      Circle circle(pta , ptb , ptc);

      #ifdef DEBUG_VERBOSE
      std::cout << "Disk[ Center : (" << circle.getCenterX() << ", " 
      		<< circle.getCenterY()<< " )"; 
      std::cout << " Radius : " << circle.getRadius() << " ] "; 
           std::cout << " Points : "<< pta<< ptb<< ptc; 
      std::cout << " - First vertex : " 
      		<< circle.getConvexHullVertex() << std::endl;
      #endif
      
      std::vector<Point> v; 
      convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 

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
  
  #ifdef DEBUG_VERBOSE  
  {

      std::cout << std::endl; 
      std::cout << " - 4 - Convex hull on a big circle" << std::endl; 
      
      DGtal::BigInteger a; 
      DGtal::BigInteger b;
      DGtal::BigInteger c;
      DGtal::BigInteger d;
      c =  -25;
      // We fixed c = -20.  pt_c = [-a/2c ; -b/2c] and R² = (a^2 + b^2 - 4*c*d)/(4c²)
	    
	    // so 4c²*R² = a² + b² - 4*c*d <=>  = (a² + b² - 4c²*R²)/4*c = d
      
      // Random Initialisation
      srand ( time(NULL) );
      
      // 2**24 = 16777216; 2**25 = 33554432;
      DGtal::BigInteger R = 33554432;
         
	    a = - rand() %(2*c);
	    b = - rand() %(2*c);  
	    d = ( a*a + b*b - 4*R*R*c*c)/(4*c);

	    // Create a circle from the Euclidian parameter a, b, c, d.
	    Circle circle( a, b, c, d );	


      std::cout << "Disk[ Center : (" << circle.getCenterX() << ", " 
      		<< circle.getCenterY()<< " )"; 
      std::cout << " Radius : " << circle.getRadius() << " ] "; 
      std::cout << " - First vertex : " 
      		<< circle.getConvexHullVertex() << std::endl;

      std::vector<Point> v; 
      convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
      std::cout << "---Get :" << std::endl; 
      //std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      //std::cout << std::endl; 
 
 }
 #endif 
  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
