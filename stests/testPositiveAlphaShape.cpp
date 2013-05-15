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
#include "PointVector2D.h"
// Circle
#include "RayIntersectableCircle.h"
// Convex Hull

#include "OutputSensitiveConvexHull.h"

// Alpha-shape
#include "PositiveAlphaShape.h"

// BigInteger
#include <DGtal/base/Common.h>

//uncomment to use in DEBUG_VERBOSE mode 
#define DEBUG_VERBOSE

typedef PointVector2D<int> Point; //type redefinition
typedef PointVector2D<int> Vector; //type redefinition
typedef std::deque<Point> Container;

//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename Container, typename Predicate>
void PositiveAlpha(const Shape& aShape, const Point& aStart, Container& container, const Predicate& aPredicate)
{

  PositiveAlphaShape<Shape, Predicate> ch(aShape, aPredicate);
  ch.all(container, aStart); 
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
template<typename Circle, typename CircumcirclePositiveRadiusPredicate, typename Point>
bool test(const Circle aCircle, const CircumcirclePositiveRadiusPredicate& aPredicate, const Point aStart)
{

  // Computed Alpha-Shape
  Container container; 
  PositiveAlpha( aCircle, aStart, container, aPredicate ); 
  
  
#ifdef DEBUG_VERBOSE
  std::cout << "# - alpha-shape" << std::endl; 
  std::copy(container.begin(), container.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl;
#endif 
  
  
  // Computed Alpha-Shape with GrahamScan
  std::vector<Point> boundary;
  std::vector<Point> ch0;
  
  //tracking-based algorithm
  //Point pStart = aCircle.getConvexHullVertex();
  Vector dir(1,0); 
  closedTracking( aCircle, aStart, dir, std::back_inserter(boundary) );
  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch0), aPredicate ); 
  
#ifdef DEBUG_VERBOSE
  std::cout << "# - alpha-shape of the boundary using closedGrahamScan" << std::endl; 
  std::copy(ch0.begin(), ch0.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl;
#endif
  
  if (ch0.size() == container.size())
    if ( std::equal(container.begin(), container.end(), ch0.begin()) )
	    return true;

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
    std::cout << " #1 -  First Circle " << std::endl;
#endif
    {
      Point pta = Point(0,5);
      Point ptb = Point(-3,3);
      Point ptc = Point(4,-3);
      Circle circle( pta, ptb, ptc );
      	  
  #ifdef DEBUG_VERBOSE
      std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
	        << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
	        << std::endl;

      std::cout << " ----------- Next predicate ----------- " << std::endl; 
      std::cout << std::endl;
  #endif
	    
      for (int k=0; k<10; k++)
      {
	      CircumcirclePositiveRadiusPredicate<> predicate5(20+(3*k),1);
	      std::cout <<"Predicate = "<< 20+(3*k) << " /"<<1<<std::endl;  
	      if (test(circle, predicate5, ptc))
	        nbok++;
	      nb++; 
	        
	      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
        std::cout << std::endl;   
      }
   }  
     
  #ifdef DEBUG_VERBOSE
      std::cout << " #2 -  Random Circle " << std::endl;
  #endif 	  
	  
// Test number
  int nb_test = 20;

  //random value
  srand ( time(NULL) );
  // Max origin coordinate
  int maxPoint = 100;

  // Number predicate test
  int nbPredicate = 10;

  // Circumcircle triangle vertices
  Point pta, ptb, ptc;

  for (int k = 1; k<= nb_test ;k++)
  {
    {
      // random circumcircle 
      pta = Point(0,  -2*k);
      ptb = Point( 2*k, -k);
      ptc = Point(-2*k, -k);

      Circle circle( pta, ptb, ptc );

      int Rint = ceil(circle.getRadius());
      
      #ifdef DEBUG_VERBOSE
      std::cout << "II - "<<nb_test<<" - Alpha-shape on the circle : " << std::endl; 
      std::cout << "-- Disk - R : " << circle.getRadius() << ", "
        << " | Points : "<< pta<< ptb<< ptc << std::endl;
      #endif

      for (int i = 1; i <= nbPredicate; i++)
      {

        {
	  #ifdef DEBUG_VERBOSE
          std::cout << " ----------- Next predicate ----------- " << std::endl; 
          std::cout << std::endl;
	  #endif

          CircumcirclePositiveRadiusPredicate<> predicate(Rint*Rint + 2*i*Rint, 1);

	  #ifdef DEBUG_VERBOSE
          std::cout << "Radius predicate : Num2 / Den2 : "<<(Rint*Rint + 2*i*Rint)<<"/" 
            << 1 << std::endl;
	  #endif

          if (test(circle, predicate, pta))
            {nbok++;}
          nb++; 
	  
          std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
        }
      }

    }
  }
  
  
  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
