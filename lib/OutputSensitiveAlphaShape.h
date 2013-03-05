#ifndef OutputSensitiveAlphaShape_h
#define OutputSensitiveAlphaShape_h

#include<cmath>

#include"../lib/CircumcircleRadiusPredicate.h"
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
public:
   
  /**
   * Dichotomic search procedure to retrieve the next edge
   * of the alpha-shape in a sequence of edge-connected triangles
   * of increasing circumcircle radius. 
   * 
   * @param aPoint the first vertex of the triangle
   * @param aConvM2 (k-2)-th convergent
   * @param aConvM1 (k-1)-th convergent
   * @param aQk k-th quotient such that the k-th convergent
   * is equal to aQk*aConvM1 + aConvM2
   * @return integer mid between 0 and aQk such that
   * aPoint and mid*aConvM1 + aConvM2 define the next edge
   * of the alpha-shape
   */
  int dichotomicSearch(const Point& aPoint,
		       const Point& aConvM2, const Point& aConvM1, 
		       const int& aQk)
  {
    // Convergent vector
    Point vConvM1 = aConvM1 - aPoint;

    // init search milestone
    int qkstart = 0;
    int qkstop = aQk;
    // middle
    int mid;

    do
      {
	mid = (qkstart + qkstop)/2;
	// lower triangle predicate
	if (aPredicate(aPoint, (aConvM2 + (mid-1)*vConvM1), (aConvM2 + mid*vConvM1)) == true)
	  {
	    // the vertex is higher
	    qkstart = mid + 1;
	  }
	else
	  { // the vertex is lower
	    qkstop = mid - 1;
	  }
      } while( qkstop - qkstart >= 0 );
    // return the index
    return(mid);
  } 

  /**
   * Given a vertex of the alpha-shape, 
   * find the next ones in a counter-clockwise order. 
   * All retrieved vertices (including @e aPoint, but
   * excluding the last one, which is returned) 
   * are written in @e res. 
   * @param aPoint any vertex of the alpha-shape
   * @return the last retrieved vertex
   */
  template <typename OutputIterator>
  Point nextOnes(const Point& aPoint, const OutputIterator& res)
  {
    //TO DO
    
    return aPoint; 
  }

}; 
#endif
