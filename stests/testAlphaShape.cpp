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
#include "../inc/OutputSensitiveAlphaShape.h"

// BigInteger
#include <DGtal/base/Common.h>

//uncomment to use in DEBUG_VERBOSE mode 
//#define DEBUG_VERBOSE

//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename OutputIterator, 
  typename Predicate>
void alphaShape(const Shape& aShape, const Point& aStartingPoint, 
    OutputIterator res, const Predicate& aPredicate)
{
  OutputSensitiveAlphaShape<Shape, Predicate> ch(aShape, aPredicate);
  ch.all(res); 
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

  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition

  // Max convergents 
  int maxConv = 50;

  Point pStart = aCircle.getConvexHullVertex();

  std::vector<Point> ch0;
  std::vector<Point> ch1;
  std::vector<Point> boundary; 

  //tracking-based algorithm
  Vector dir(1,0); 
  closedTracking( aCircle, pStart, dir, std::back_inserter(boundary) );

  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch0), aPredicate ); 

  #ifdef DEBUG_VERBOSE
  std::cout << "# - alpha-shape of the boundary using closedGrahamScan" << std::endl; 
  std::copy(ch0.begin(), ch0.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 
  #endif

  alphaShape( aCircle, pStart, std::back_inserter(ch1), aPredicate ); 

  #ifdef DEBUG_VERBOSE
  std::cout << "# - alpha-shape" << std::endl; 
  std::copy(ch1.begin(), ch1.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 
  #endif 

  if (ch0.size() == ch1.size())
  {
    if ( std::equal(ch1.begin(), ch1.end(), ch0.begin()) )
    {
      return true;
    }
  }
  else 
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
  {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    #ifdef DEBUG_VERBOSE
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; std::cout << std::endl; 
    std::cout << "  A) infinite radius" << std::endl; 
    #endif
    {
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
      std::cout << "1 - Expected - handmade" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      #ifdef DEBUG_VERBOSE
      std::cout << "2 - Graham's convex hull of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    }

    #ifdef DEBUG_VERBOSE
    std::cout << std::endl; 
    std::cout << "  B) radius == 1" << std::endl; 
    #endif
    {
      #ifdef DEBUG_VERBOSE
      std::cout << "Expected (boundary)" << std::endl; 
      std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(1,1); //radius 1  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      #ifdef DEBUG_VERBOSE
      std::cout << "1-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (ch.size() == boundary.size())
        if ( std::equal(boundary.begin(), boundary.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    }

    #ifdef DEBUG_VERBOSE
    std::cout << std::endl; std::cout << "  B) radius == 3" << std::endl; 
    #endif

    {
      std::vector<Point> groundTruth; 
      groundTruth.push_back(Point(0,-5)); 
      groundTruth.push_back(Point(2,-4)); 
      groundTruth.push_back(Point(3,-4)); 
      groundTruth.push_back(Point(4,-3)); 
      groundTruth.push_back(Point(4,-2)); 
      groundTruth.push_back(Point(5,0));
      groundTruth.push_back(Point(4,2));  
      groundTruth.push_back(Point(4,3));
      groundTruth.push_back(Point(3,4)); 
      groundTruth.push_back(Point(2,4)); 
      groundTruth.push_back(Point(0,5)); 
      groundTruth.push_back(Point(-2,4)); 
      groundTruth.push_back(Point(-3,4)); 
      groundTruth.push_back(Point(-4,3)); 
      groundTruth.push_back(Point(-4,2)); 
      groundTruth.push_back(Point(-5,0)); 
      groundTruth.push_back(Point(-4,-2)); 
      groundTruth.push_back(Point(-4,-3));
      groundTruth.push_back(Point(-3,-4));
      groundTruth.push_back(Point(-2,-4)); 
      #ifdef DEBUG_VERBOSE
      std::cout << "Expected" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(9,1); //radius 3  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      #ifdef DEBUG_VERBOSE
      std::cout << "3-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    }
  }

  #ifdef DEBUG_VERBOSE
  std::cout << std::endl; 
  std::cout << "I) Alpha-shape on a simple circle" << std::endl;
  #endif

  {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    #ifdef DEBUG_VERBOSE
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; std::cout << std::endl; 
    std::cout << "  A) infinite radius" << std::endl;
    #endif
    {
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
      std::cout << "1 - Expected - handmade" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      #ifdef DEBUG_VERBOSE
      std::cout << "2 - Graham's convex hull of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

      std::vector<Point> as; 
      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), predicate );

      #ifdef DEBUG_VERBOSE
      std::cout << "3 - Alpha shape" << std::endl; 
      std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (as.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    #ifdef DEBUG_VERBOSE
    std::cout << std::endl; std::cout << "  B) radius == 1" << std::endl; 
    #endif

    {
      #ifdef DEBUG_VERBOSE
      std::cout << "Expected (boundary)" << std::endl; 
      std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(1,1); //radius 1  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      #ifdef DEBUG_VERBOSE
      std::cout << "1-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (ch.size() == boundary.size())
        if ( std::equal(boundary.begin(), boundary.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

      std::vector<Point> as; 

      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), predicate );
      #ifdef DEBUG_VERBOSE
      std::cout << "Alpha shape" << std::endl; 
      std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (as.size() == ch.size())
        if ( std::equal(ch.begin(), ch.end(), as.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    #ifdef DEBUG_VERBOSE
    std::cout << std::endl; std::cout << "  B) radius == 3" << std::endl; 
    #endif
    {
      std::vector<Point> groundTruth; 
      groundTruth.push_back(Point(0,-5)); 
      groundTruth.push_back(Point(2,-4)); 
      groundTruth.push_back(Point(3,-4)); 
      groundTruth.push_back(Point(4,-3)); 
      groundTruth.push_back(Point(4,-2)); 
      groundTruth.push_back(Point(5,0));
      groundTruth.push_back(Point(4,2));  
      groundTruth.push_back(Point(4,3));
      groundTruth.push_back(Point(3,4)); 
      groundTruth.push_back(Point(2,4)); 
      groundTruth.push_back(Point(0,5)); 
      groundTruth.push_back(Point(-2,4)); 
      groundTruth.push_back(Point(-3,4)); 
      groundTruth.push_back(Point(-4,3)); 
      groundTruth.push_back(Point(-4,2)); 
      groundTruth.push_back(Point(-5,0)); 
      groundTruth.push_back(Point(-4,-2)); 
      groundTruth.push_back(Point(-4,-3));
      groundTruth.push_back(Point(-3,-4));
      groundTruth.push_back(Point(-2,-4)); 

      #ifdef DEBUG_VERBOSE
      std::cout << "Expected" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(9,1); //radius 3  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      #ifdef DEBUG_VERBOSE
      std::cout << "3-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 

      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

      std::vector<Point> as; 

      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), predicate );

      #ifdef DEBUG_VERBOSE
      std::cout << "Alpha shape" << std::endl; 
      std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 
      #endif

      if (as.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }
  }

  #ifdef DEBUG_VERBOSE
  std::cout << std::endl; 
  std::cout << "II) Automatic testing -shape of the boundary" << std::endl << std::endl; 
  #endif

  // Test number
  int nb_test = 100;

  //random value
  srand ( time(NULL) );
  // Max origin coordinate
  int maxPoint = 100;

  // Number predicate test
  int nbPredicate = 10;
  int valuePredicate[10] = {3, 4, 10, 20, 200, 2000, 20000, 100000, 200000, 2000000};
  // Circumcircle triangle vertices
  Point pta, ptb, ptc;

  for (nb_test;nb_test>0;nb_test--)
  {
    {
      // random circumcircle 
      pta = Point( (rand() % maxPoint)             , (rand() % maxPoint) );
      ptb = Point( (pta[0]-1- (rand() % maxPoint) ), (pta[1]-1- (rand() % maxPoint)) );
      ptc = Point( (ptb[0]+1+ (rand() % maxPoint) ), (ptb[1]-1- (rand() % maxPoint)) );

      Circle circle( pta, ptb, ptc );

      #ifdef DEBUG_VERBOSE
      std::cout << "II - "<<nb_test<<" - Alpha-shape on the circle : " << std::endl; 
      std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
        << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
        << " ] | Points : "<< pta<< ptb<< ptc<< " - First vertex : " 
        << circle.getConvexHullVertex() << std::endl;
      #endif

      for (int i = 0; i < nbPredicate; i++)
      {

        {
	  #ifdef DEBUG_VERBOSE
          std::cout << " ----------- Next predicate ----------- " << std::endl; 
          std::cout << std::endl;
	  #endif

          CircumcircleRadiusPredicate<> predicate(valuePredicate[i], 2);

	  #ifdef DEBUG_VERBOSE
          std::cout << "Radius predicate : Num2 / Den2 : "<<valuePredicate[i]<<"/" 
            << 2 << std::endl;
	  #endif

          if (test(circle, predicate))
            nbok++;
          nb++; 
	  
          std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
        }
      }

    }
  }
  //  (0,2), (1,3), (2,4), (2,5), (2,6), (2,7), (1,8), (-1,9), (-3,8), (-4,7), (-4,6), (-4,5), (-4,4), (-4,3), (-2,2), (-1,2), (0,2), 
  //-- Disk[ Center : (-1.27273, 5.40909 ), Radius : 3.63892 ] | Points : (2,7)(-4,3)(0,2) - First vertex : (0,2)
  //10/2
  {
    pta = Point(2,7);
    ptb = Point(-4,3);
    ptc = Point(0,2);
    Circle circle( pta, ptb, ptc );

    #ifdef DEBUG_VERBOSE
    std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
      << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
      << " ] | Points : "<< pta<< ptb<< ptc<< " - First vertex : " 
      << circle.getConvexHullVertex() << std::endl;

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
  {

    pta = Point(1025, 0);
    ptb = Point(0, 1025);
    ptc = Point(-1025, 0);

    RayIntersectableCircle<Point,DGtal::BigInteger> circle( pta, ptb, ptc );

    #ifdef DEBUG_VERBOSE
    std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
      << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
      << " ] | Points : "<< pta<< ptb<< ptc<< " - First vertex : " 
      << circle.getConvexHullVertex() << std::endl;

    std::cout << " ----------- Next predicate ----------- " << std::endl; 
    std::cout << std::endl;
    #endif

    CircumcircleRadiusPredicate<DGtal::BigInteger> predicate(20,2);
    
    #ifdef DEBUG_VERBOSE
    std::cout << "Radius predicate : Num2 / Den2 : 10/2"<< std::endl;
    #endif

    if (test(circle, predicate))
    {nbok++;} 
    nb++; 

    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  }


  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
