#include <iostream>
#include <vector>
#include <iterator>

#include "PointVector2D.h"
#include "RayIntersectableCircle.h"
#include "OutputSensitiveConvexHull.h"

/**
 * Class implementing a contour tracking routine, which
 * retrieves the point of the digital boundary following
 * a given point in a given direction. 
 * 
 * @tparam TShape a point functor (positive value for 
 * a point belonging to the shape).   
 */
template <typename TShape>
class Tracker
{
public: 
  /////////////////////// inner types /////////////////
  typedef TShape Shape; 
  typedef typename Shape::Point Point; 

private: 
  /////////////////////// members /////////////////////
  /**
   * const reference on a shape
   */
  const Shape& myShape;  

public:
  ///////////////////// standard services /////////////
  /**
   * Standard constructor
   * @param aShape
   */
  Tracker(const Shape& aShape)
    : myShape(aShape) {}

private:
  /**
   * Copy constructor
   * @param other other object to copy
   */
  Tracker(const Tracker& other) {}

  /**
   * Assignement operator
   * @param other other object to copy
   * @return reference on *this
   */
  Tracker& operator=(const Tracker& other) 
  { return *this; }

public: 
  /**
   * Default destructor
   */
  ~Tracker() {}


  ///////////////////// main methods ///////////////////
  /**
   * Given a point of the digital boundary, find the next
   * point in a counter-clockwise order
   * @param aPoint any point of the digital boundary
   * @param aDir returned tracking direction
   * @return the next point (can be the same as @a aPoint)
   *
   * @tparam Vector a model of vector
   */
  template<typename Vector>
  Point next(const Point& aPoint, Vector& aDir)
  {
    Point next; //next point to return 
    Vector shift; 
    if (aDir[0] == 0)
      shift = Vector(aDir[1], aDir[0]);
    else 
      shift = Vector(aDir[1], -aDir[0]);
    Point out = aPoint + shift;
    // std::cout << aPoint << " " << out << " " << out + aDir << std::endl; 
    if ( myShape(out) < 0 )
      { //if out is outside
	if ( myShape(out + aDir) >= 0 )
	  { //if out + aDir is inside
	    // std::cout << out + aDir << " is inside " << std::endl; 
	    next = out + aDir; 
	    // std::cout << next << " is next " << std::endl; 
	    aDir = shift; 
	  }
	else 
	  { //otherwise
	    //	    std::cout << out + aDir << " is outside " << std::endl; 
	    if ( myShape(aPoint + aDir) >= 0 )
	      { //if aPoint + aDir is inside
		next = aPoint + aDir; 
	      }
	    else
	      { //otherwise
		next = aPoint; 
		aDir = -shift; 
	      }
	  }
      }
    else
      {
	std::cerr << "Error in next of Tracker: " << aPoint 
		  << " is not a point of the digital boundary " 
		  << std::endl; 
	next = aPoint; 
      }
    // std::cout << next << " is next " << std::endl; 
	        return next; 
  }
}; 

template <typename Shape, typename Point, typename Vector, typename OutputIterator>
void tracking(const Shape& aShape, const Point& aStartingPoint, Vector& aDir,  
	      OutputIterator res)
{
  Tracker<Shape> t(aShape);
  //get the first point
  Point current = aStartingPoint; 
  Point tmp; 
  do {
    //store the current point
    *res++ = current; 
    //get the next DIFFERENT point
    do {
      tmp = t.next(current, aDir);
    } while (tmp == current); 
    current = tmp; 
    //while it is not the first one
  } while (current != aStartingPoint); 
}

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

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

  }


  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
