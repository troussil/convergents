#include <iostream>
#include <string>
#include <fstream>

//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>

#include <cmath>
// Core geometry
#include "../inc/PointVector2D.h"
// Circle
#include "../inc/RayIntersectableCircle.h"
// Convex Hull

#include "../inc/OutputSensitiveConvexHull.h"

// Alpha-shape
#include "../inc/PositiveAlphaShape.h"

// BigInteger
#include <DGtal/base/Common.h>

//uncomment to use in DEBUG_VERBOSE mode 
#define DEBUG_VERBOSE

typedef PointVector2D<int> Point; //type redefinition
typedef PointVector2D<int> Vector; //type redefinition
typedef std::deque<Point> Container;

//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Container, typename Predicate>
void PositiveAlpha(const Shape& aShape, Container& container, const Predicate& aPredicate)
{

  PositiveAlphaShape<Shape, Predicate> ch(aShape, aPredicate);
  ch.all(container, aShape.getConvexHullVertex()); 
}
///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure that checks whether the 
 * output-sensitive algorithm returns the same
 * alpha-shape as the tracking-based algorithm
 * for a given circle. 
 * 
 * @param aCircle
 * @param aPredicate  
 * 
 * @return 'true' if the test passed, 'false' otherwise
 * 
 * @tparam
 * @tparam CircumcircleRadiusPredicate
 */
  template<typename Circle, typename CircumcircleRadiusPredicate>
bool test(const Circle aCircle, const CircumcircleRadiusPredicate& aPredicate)
{

  Container container; 
  PositiveAlpha( aCircle, container, aPredicate ); 
  
  std::cout << "Get" << std::endl; 
  std::copy(container.begin(), container.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 
  return false;
}


///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point> Circle; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  #ifdef DEBUG_VERBOSE
  std::cout << std::endl; 
  std::cout << "I) Alpha-shape on a simple circle" << std::endl; 
  #endif
  
  //  (0,2), (1,3), (2,4), (2,5), (2,6), (2,7), (1,8), (-1,9), (-3,8), (-4,7), (-4,6), (-4,5), (-4,4), (-4,3), (-2,2), (-1,2), (0,2), 
  //-- Disk[ Center : (-1.27273, 5.40909 ), Radius : 3.63892 ] | Points : (2,7)(-4,3)(0,2) - First vertex : (0,2)
  //10/2
  {
    Point pta = Point(0,6);
    Point ptb = Point(-4,3);
    Point ptc = Point(0,2);
    Circle circle( pta, ptb, ptc );

    #ifdef DEBUG_VERBOSE
    std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
      << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
      << " ] | Points : "<< pta<< ptb<< ptc<< " - First vertex : " 
      << std::endl;

    std::cout << " ----------- Next predicate ----------- " << std::endl; 
    std::cout << std::endl;
    #endif

    CircumcircleRadiusPredicate<> predicate(10,2);
    
    #ifdef DEBUG_VERBOSE
    std::cout << "Radius predicate : Num2 / Den2 : 10/2"<< std::endl;
    #endif

    if (test(circle, predicate))
      nbok++; 
    nb++; 

    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  }
  //(4,2)(2,1)(6,-5) - (4,2)(0,-6)(10,-14) - (7,8)(-1,-1)(3,-8) - (5,3)(2,2)(4,-7)
  // Circle parameter : ax + by + c(x^2 + y^2)   
  
  
  {
  int R = 10;
  //random value
  srand ( time(NULL) );
  
  int c = -25;
  
  //int a = - rand() %(2*c);
  //int b = - rand() %(2*c);
  
  int a = 20;
  int b = 32;  
  
  int d = ( a*a + b*b - 4*R*R*c*c)/(4*c);;

  
  // We fixed c = -20.  pt_c = [-a/2c ; -b/2c] and R² = (a^2 + b^2 - 4*c*d)/(4c²)
	// so 4c²*R² = a² + b² - 4*c*d <=>  = (a² + b² - 4c²*R²)/4*c = d

	    // Create a circle from the Euclidian parameter a, b, c, d.
	  Circle circle( a, b, c, d );	
	  
	   #ifdef DEBUG_VERBOSE
    std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
      << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
      << std::endl;

    std::cout << " ----------- Next predicate ----------- " << std::endl; 
    std::cout << std::endl;
    #endif
	  
	  for (int k=0; k<10; k++)
	  {
      CircumcircleRadiusPredicate<DGtal::BigInteger> predicate5(R+(k*10),1);
      
      #ifdef DEBUG_VERBOSE
      std::cout << "Radius predicate : Num2 / Den2 : 110/1"<< std::endl;
      #endif

     test(circle, predicate5);

    }
    
	  }  
  	  
	  
    

  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
