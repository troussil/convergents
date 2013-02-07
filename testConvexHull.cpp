#include <iostream>
#include <vector>
#include <deque>
#include <iterator>
// random
#include <cstdlib>
#include <ctime>

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
template <typename Point>
  typename Point::Coordinate
signedArea(const Point& a, const Point& b, const Point&c)
{
  return a[0]*(b[1] - c[1]) - b[0]*(a[1] - c[1]) + c[0]*(a[1] - b[1]);
}

  template <typename ForwardIterator, typename OutputIterator>
void melkmanConvexHull(const ForwardIterator& itb, const ForwardIterator& ite,  
    OutputIterator res )
{
  typedef typename std::iterator_traits<ForwardIterator>::value_type Point; 
  std::deque<Point> container; 

  //for all points
  for(ForwardIterator it = itb; it != ite; ++it)
  {
    if(container.size() < 3)
    {
      container.push_back( *it ); 
      //std::cout << " add (to back) " << *it << std::endl; 
    }
    else
    {
      //front
      {
        Point P = *it; 
        Point Q = container.front(); 
        container.pop_front(); 
        if (container.size() != 0) 
        {
          Point R = container.front(); 
          //std::cout << " signed area of " << P << " " << Q << " " << R << " : " << signedArea(P,Q,R) << std::endl; 
          while ( ( signedArea(P,Q,R) >= 0 )&&(container.size() != 0) )
          {
            //remove Q
            //std::cout << " remove from front " << Q << std::endl; 
            Q = R; 
            container.pop_front(); 
            if (container.size() != 0) 
              R = container.front(); 
          }
          //add Q
          container.push_front(Q);
        }
      }

      //back
      {
        Point P = *it; 
        Point Q = container.back(); 
        container.pop_back(); 
        if (container.size() != 0) 
        {
          Point R = container.back(); 
          //std::cout << " signed area of " << P << " " << Q << " " << R << " : " << signedArea(P,Q,R) << std::endl; 
          while ( ( signedArea(P,Q,R) <= 0 )&&(container.size() != 0) )
          {
            //remove Q
            //std::cout << " remove from back" << Q << std::endl; 
            Q = R; 
            container.pop_back(); 
            if (container.size() != 0) 
              R = container.back(); 
          }
          //add Q
          container.push_back(Q); 
        }
      }
      //add new point
      if ( signedArea(container.front(), *it, container.back()) > 0 )
      {
        container.push_front(*it); 
        //std::cout << " add to front " << *it << std::endl; 
        container.push_back(*it); 
        //std::cout << " add to back " << *it << std::endl; 
      }
    }
  }//end for all points

  //copy
  std::copy(++container.rbegin(), container.rend(), res); 
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
    melkmanConvexHull( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
    std::cout << "Melkman's convex hull of the boundary" << std::endl; 
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
    melkmanConvexHull( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
    std::cout << "Melkman's convex hull of the boundary" << std::endl; 
    std::copy(mch.begin(), mch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (mch.size() == v.size())
      if ( std::equal(v.begin(), v.end(), mch.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  }

  srand ( time(NULL) );
  int max = 25;
  int nb_test = 4;

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

      std::cout << "Disk[ (" << circle.getCenterX() << ", " << circle.getCenterY()<< " ), " << circle.getRadius()<<" ] | aStartingPoint : "<<circle.getConvexHullVertex()<<std::endl;

      std::vector<Point> v; 
      convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(v) ); 
      std::cout << "Get" << std::endl; 
      std::copy(v.begin(), v.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> boundary; 
      Vector dir(1,0); 
      tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 

      std::vector<Point> mch; 
      melkmanConvexHull( boundary.begin(), boundary.end(), std::back_inserter(mch) ); 
      std::cout << "Melkman's convex hull of the boundary" << std::endl; 
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
