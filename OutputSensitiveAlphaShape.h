#ifndef OutputSensitiveAlphaShape_h
   #define OutputSensitiveAlphaShape_h

#include<cmath>

#include"CircumcircleRadiusPredicate.h"
/**
 * Class implementing an on-line and ouput-sensitive algorithm
 * that retrieves the vertices of the alpha-shape of all digital
 * points lying inside an implicit shape, which is 'ray-intersectable',
 * ie. the intersection between the shape boundary and
 * a ray emanating from a given point along a given direction
 * is computable.
 *
 * @tparam TShape a model of ray-intersectable shape.
 * @tparam TPredicate a model of ternary predicate: 
 * given three points, the operator() returns a bool.  
 */
template <typename TShape, typename TPredicate>
class OutputSensitiveAlphaShape
{
public:
  /////////////////////// inner types /////////////////
  typedef TShape Shape;
  typedef typename Shape::Point Point;
  typedef typename Shape::Vector Vector; //type redefinition
  typedef TPredicate Predicate;

private:
  /////////////////////// members /////////////////////
  /**
   * const reference on a shape
   */
  const Shape& myShape;
  /**
   * Predicate that returns 'true' if the radius 
   * of the circumcircle of three given points
   * is greater than 1/alpha, 'false' otherwise.
   *
   * NB. alpha is implicitely defined by the predicate. 
   */
  const Predicate& myPredicate; 

public:
  ///////////////////// standard services /////////////
  /**
   * Standard constructor
   * @param aShape any 'ray-intersectable' shape
   * @param aPredicate any predicate
   */
  OutputSensitiveAlphaShape(const Shape& aShape, const Predicate& aPredicate)
    : myShape(aShape), myPredicate(aPredicate) {}

private:
  /**
   * Copy constructor
   * @param other other object to copy
   */
  OutputSensitiveAlphaShape(const OutputSensitiveAlphaShape& other) {}

  /**
   * Assignement operator
   * @param other other object to copy
   * @return reference on *this
   */
  OutputSensitiveAlphaShape& operator=(const OutputSensitiveAlphaShape& other)
  { return *this; }

public:
  /**
   * Default destructor
   */
  ~OutputSensitiveAlphaShape() {}


  ///////////////////// main methods ///////////////////
private: 
   
/**
* Dichotomic search procedure to retrieve the next edge
* of the alpha-shape in a sequence of edge-connected triangles
* of increasing circumcircle radius. 
* 
* TO IMPROVE doc
* @param aPointa is the first vertex of the triangle
* @param Points aConvM2 and aConvM1 are the next convergents
* @param aqk pConv = aqk*aConvM1 + aConvM2
* @param aEven is bool which determine if the last odd convergent lie on the
* straight line.
* @return first convergent in the alpha shape
*/
Point dichotomicSearch(const Point& aPointa,
		       const Point aConvM2, const Point aConvM1, 
		       const int aqk)
{
  // Convergent vector
  Point vConvM1 = aConvM1 - aPointa;

  // init search milestone
  int qkstart = 1;
  int qkstop  = aqk;
  // middle
  int mid;

  do 
  {
    mid = (qkstart + qkstop)/2;
    // lower triangle predicate
    if (aPredicate(aPointa, aPointa + (mid-1)*vConvM1, aPointa + mid*vConvM1) == true)
    { 
      // the vertex is higher
      qkstart = mid + 1;
    }
    else
    { // the vertex is lower
      qkstop = mid - 1;
    }
  } while( qkstop - qkstart == 0 ); 
  // return a new point
  return(Point(aPointa + mid*vConvM1));
}
public:
    /**
     * Given a vertex of the convex hull, find the next
     * vertex in a counter-clockwise order
     * @param aPoint any vertex of the convex hull
     * @return the next vertex
     */
    Point next(const Point& aPoint)
    {
      return aPoint; 
    }

}; 
#endif
