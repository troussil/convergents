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
		       const int aqk, const bool aEven)
{
  //TO IMPROVE

  // Convergent vector
  Point vConvM1 = aConvM1 - aPointa;

  // next convergent
  Point pConv = aConvM2 + aqk*vConvM1;

  // init search milestone
  int qkstart = 1;
  int qkstop = aqk;
  // middle
  int mid;

  // We found the correct vertex
  bool boolconv = false;

  if (aEven == true) // Even case
  {
    while ( boolconv == false )
    {
      mid = (qkstart + qkstop)/2;
      if (myPredicate(aPointa, pConv, pConv - mid*vConvM1) == false)
      {
        if (qkstop == 1 || myPredicate(aPointa, pConv, pConv - (mid-1)*vConvM1) == true)
        { // We found the good one
          boolconv = true;
        }
        else
        { // the vertex is higher
          qkstop = mid - 1;
        }
      }
      else
      { // the vertex is lower
        qkstart = mid + 1;
      }
    }
    // return a new point
    return(Point(pConv - mid*vConvM1));
  }
  else //Odd case
  {
    while ( boolconv == false )
    {
      mid = (qkstart + qkstop)/2;
      if (aPredicate(aPointa, pConv, aPointa + mid*vConvM1) == false)
      {
        if (qkstart == 1 || aPredicate(aPointa, pConv, aPointa + (mid+1)*vConvM1) == true)
        { // We found the good one
          boolconv = true;
        }
        else
        { // the vertex is higher
          qkstart = mid + 1;
        }
      }
      else
      { // the vertex is lower
        qkstop = mid - 1;
      }
    }
    // return a new point
    return(Point(aPointa + mid*vConvM1));
  }
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
