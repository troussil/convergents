#include <iostream>
//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>

#include "../lib/PointVector2D.h"
#include "../lib/RayIntersectableCircle.h"
//#include "../lib/OutputSensitiveConvexHull.h"
#include "../lib/OutputSensitiveAlphaShape.h"
#include "../lib/ConvexHullHelpers.h"



//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename OutputIterator, 
  typename Predicate>
void alphaShapeNaive(const Shape& aShape, const Point& aStartingPoint, 
    OutputIterator res, int aMaxConv, const Predicate& aPredicate)
{
  OutputSensitiveAlphaShape<Shape, Predicate> ch(aShape, aPredicate);

  //get the first vertex
  Point tmp1 = aStartingPoint; 
  Point tmp2;

  *res++ = tmp1; 
  while (tmp2 != aStartingPoint)
  {

    //get the next convex hull vertex
    tmp2 = ch.nextCH(tmp1);
    // get the next alpha-shape vertices
    ch.nextSLAS(aPredicate, tmp1, tmp2, aMaxConv, res);

    tmp1 = tmp2;
    //while it is not the first one
  }
}

//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename OutputIterator, 
  typename Predicate>
void alphaShape(const Shape& aShape, const Point& aStartingPoint, 
    OutputIterator res, int aMaxConv, const Predicate& aPredicate)
{
  OutputSensitiveAlphaShape<Shape, Predicate> ch(aShape, aPredicate);

  //get the first vertex
  Point tmp = aStartingPoint; 
	int k = 0;
	
  *res++ = tmp; 
  while (tmp != aStartingPoint || k == 0)
  {
		k++;
    // get the next alpha-shape vertices
    tmp = ch.next(aPredicate, tmp, aMaxConv, res);
  }//while we not return to aStartingPoint
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
  
  std::cout << "I) Alpha-shape on a simple circle - Naïve" << std::endl; 
  {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
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
		  std::cout << "1 - Expected - handmade" << std::endl; 
		  std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  std::vector<Point> ch;
		  CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
		  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
		  std::cout << "2 - Graham's convex hull of the boundary" << std::endl; 
		  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (ch.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

		  std::vector<Point> as; 
		  alphaShapeNaive( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
		  as.erase(as.end()); 
		  std::cout << "3 - Alpha shape" << std::endl; 
		  std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (as.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
		}
		
    std::cout << std::endl; std::cout << "  B) radius == 1" << std::endl; 
		{
		  std::cout << "Expected (boundary)" << std::endl; 
		  std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  std::vector<Point> ch;
		  CircumcircleRadiusPredicate<> predicate(1,1); //radius 1  
		  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
		  std::cout << "1-shape of the boundary" << std::endl; 
		  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (ch.size() == boundary.size())
		    if ( std::equal(boundary.begin(), boundary.end(), ch.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
		
			std::vector<Point> as; 
		  alphaShapeNaive( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
		  as.erase(as.end()); 
		  std::cout << "Alpha shape" << std::endl; 
		  std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (as.size() == ch.size())
		    if ( std::equal(ch.begin(), ch.end(), as.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
		}
		
		std::cout << std::endl; std::cout << "  B) radius == 3" << std::endl; 
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
		  std::cout << "Expected" << std::endl; 
		  std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  std::vector<Point> ch;
		  CircumcircleRadiusPredicate<> predicate(9,1); //radius 3  
		  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
		  std::cout << "3-shape of the boundary" << std::endl; 
		  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (ch.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
	
			std::vector<Point> as; 
		  alphaShapeNaive( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
		  as.erase(as.end()); 
		  std::cout << "Alpha shape" << std::endl; 
		  std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (as.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  	}
  }
std::cout << std::endl; 
  
  std::cout << "I) Alpha-shape on a simple circle" << std::endl; 
  {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
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
		  std::cout << "1 - Expected - handmade" << std::endl; 
		  std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  std::vector<Point> ch;
		  CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
		  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
		  std::cout << "2 - Graham's convex hull of the boundary" << std::endl; 
		  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (ch.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

		  std::vector<Point> as; 
		  alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
		  as.erase(as.end()); 
		  std::cout << "3 - Alpha shape" << std::endl; 
		  std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (as.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
		}
		
    std::cout << std::endl; std::cout << "  B) radius == 1" << std::endl; 
		{
		  std::cout << "Expected (boundary)" << std::endl; 
		  std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  std::vector<Point> ch;
		  CircumcircleRadiusPredicate<> predicate(1,1); //radius 1  
		  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
		  std::cout << "1-shape of the boundary" << std::endl; 
		  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (ch.size() == boundary.size())
		    if ( std::equal(boundary.begin(), boundary.end(), ch.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
		
			std::vector<Point> as; 
		  alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
		  as.erase(as.end()); 
		  std::cout << "Alpha shape" << std::endl; 
		  std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (as.size() == ch.size())
		    if ( std::equal(ch.begin(), ch.end(), as.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
		}
		
		std::cout << std::endl; std::cout << "  B) radius == 3" << std::endl; 
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
		  std::cout << "Expected" << std::endl; 
		  std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  std::vector<Point> ch;
		  CircumcircleRadiusPredicate<> predicate(9,1); //radius 3  
		  closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
		  std::cout << "3-shape of the boundary" << std::endl; 
		  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (ch.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
	
			std::vector<Point> as; 
		  alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
		  as.erase(as.end()); 
		  std::cout << "Alpha shape" << std::endl; 
		  std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
		  std::cout << std::endl; 

		  if (as.size() == groundTruth.size())
		    if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
		      nbok++; 
		  nb++; 
		  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  	}
  }
  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
