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
// Alpha-shape
#include "../inc/OutputSensitiveAlphaShape.h"


///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure that generates the (k+n)-th convergent, 
 * given the u_k, u_{k+1}, ... u_{k+n} quotients of the 
 * range [ @e itb , @e ite )  
 * 
 * @param itb quotient begin iterator
 * @param ite quotient end iterator 
 * @param aConv2 (k-2)-th convergent
 * @param aConv1 (k-1)-th convergent 
 * 
 * @return (k+n)-th convergent
 * (NB: default point if the range is empty)
 * 
 * @tparam ForwardIterator a model of forward iterator
 * @tparam Point a model of point
 */
template<typename ForwardIterator, typename Point>
Point generatePoint(const ForwardIterator& itb, const ForwardIterator& ite,
		    Point aConv2, Point aConv1)
{
  Point aConv; //k-th convergent
  for (ForwardIterator it = itb; it != ite; ++it)
    {
      aConv = (*it)*aConv1 + aConv2;
      aConv2 = aConv1; 
      aConv1 = aConv; 
    }
  return aConv; 
}

///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure that checks whether the 
 * output-sensitive algorithm returns the same
 * alpha-shape as the tracking-based algorithm
 * for a straight-line, which is described by
 * a point @e O and the continued fractions
 * expansion of its slope. 
 * 
 * @param itb quotient begin iterator
 * @param ite quotient end iterator 
 * @param O any digital point 
 * 
 * @return 'true' if the test passed, 'false' otherwise
 * 
 * @tparam ForwardIterator a model of forward iterator
 * @tparam Point a model of point
 */
template<typename ForwardIterator, typename Point>
bool test(const ForwardIterator& itb, const ForwardIterator& ite,
	  const Point& O)
{

  typedef RayIntersectableStraightLine<Point> StraightLine; 
  typedef Point Vector; 

  std::cout << "Continued fraction expansion" << std::endl; 
  std::copy(itb, ite, std::ostream_iterator<int>(std::cout, ", ") ); 
  std::cout << std::endl; 

  Point P = generatePoint(itb, ite, Point(0,1), Point(1,0));
  std::cout << "Direction vector : " << P << std::endl; 
  std::cout << std::endl; 
 
  StraightLine sl( O, P );

  std::vector<Point> boundary; 
  Vector dir(1,0); 
  openTracking( sl, O, P, dir, std::back_inserter(boundary) );
  // std::cout << "Boundary" << std::endl; 
  // std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  // std::cout << std::endl; 

  //tracking-based algorithm
  std::vector<Point> ch;
  CircumcircleRadiusPredicate<> predicate1(1,1); //CHOOSE A RADIUS (1 here)  
  openGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate1 ); 
  std::cout << "1-shape of the boundary" << std::endl; 
  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 

  //output-sensitive algorithm
  std::vector<Point> ch2; 
  //TODO

  //COMPARE WITH YOUR ALGO HERE
  return false; 

}

///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableStraightLine<Point> StraightLine; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  ////////////////////////////////////////////////////////////////////////
  std::cout << std::endl; 
  std::cout << "I) Alpha-shape on a simple straight line" << std::endl; 
  {

    StraightLine sl( Point(0,0), Point(8,5) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    openTracking( sl, Point(0,0), Point(8,5), dir, std::back_inserter(boundary) );
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
    std::vector<Point> groundTruth; 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(8,5)); 
    std::cout << "Expected" << std::endl; 
    std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> ch;
    CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
    openGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
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
    CircumcircleRadiusPredicate<> predicate1(1,1); //radius 1  
    openGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch1), predicate1 ); 
    std::cout << "1-shape of the boundary" << std::endl; 
    std::copy(ch1.begin(), ch1.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (ch1.size() == boundary.size())
      if ( std::equal(boundary.begin(), boundary.end(), ch1.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  }

  ////////////////////////////////////////////////////////////////////////
  std::cout << std::endl; 
  std::cout << "II) Alpha-shape on other straight lines" << std::endl; 
  {
    Point O(0,0); 

    {
      std::vector<int> quotients; 
      quotients.push_back(1); 
      quotients.push_back(1); 
      quotients.push_back(1); 
      quotients.push_back(1); 
      quotients.push_back(1); 

      if (test(quotients.begin(), quotients.end(), O))
	nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    {
      std::vector<int> quotients; 
      quotients.push_back(2); 
      quotients.push_back(2); 
      quotients.push_back(2); 
      quotients.push_back(2); 
      quotients.push_back(2); 

      if (test(quotients.begin(), quotients.end(), O))
	nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    //OTHER TESTS TO DO

  }

  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
