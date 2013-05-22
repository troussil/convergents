#ifndef OutputSensitiveAlphaShape_h
#define OutputSensitiveAlphaShape_h

#include<cmath>

#include"CircumcirclePositiveRadiusPredicate.h"
#include "RayIntersectableStraightLine.h"

#include "ConvexHullHelpers.h"

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
class PositiveAlphaShape
{
  public:
    /////////////////////// inner types /////////////////
    typedef TShape Shape;
    typedef typename Shape::Point Point;
    typedef typename Shape::Vector Vector; //type redefinition
    typedef TPredicate Predicate;
    typedef std::deque<Point> Container;
  
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
    PositiveAlphaShape(const Shape& aShape, const Predicate& aPredicate)
      : myShape(aShape), myPredicate(aPredicate) {}

  private:
    /**
     * Copy constructor
     * @param other other object to copy
     */
    PositiveAlphaShape(const PositiveAlphaShape& other) {}

    /**
     * Assignement operator
     * @param other other object to copy
     * @return reference on *this
     */
    PositiveAlphaShape& operator=(const PositiveAlphaShape& other)
    { return *this; }

  public:
    /**
     * Get Circle Predicate 
     * @param aShape any 'ray-intersectable' shape
     * @return aPredicate any predicate
     */
    Predicate getPredicate()
    {
      return ((*this).myPredicate);
    }
    /**
     * Default destructor
     */
    ~PositiveAlphaShape() {}


    ///////////////////// main methods ///////////////////
  public:
       

    /**
     * Given a vertex of the alpha-shape, 
     * retrieves a sequence of consecutive 
     * vertices of the alpha-shape
     * in a counter-clockwise order. 
     * 
     * @param aPoint any vertex of the alpha-shape 
     * @param res output iterator that stores the sequence of vertices
     * @return the last retrieved vertex (not stored). 
     */
template <typename Container, typename Point>
      void all(Container& aContainer, Point aPointa)
      {

        // Retrieve Convex Hull vertices
        OutputSensitiveConvexHull<TShape> ch(myShape); 

        std::vector<Point> it;
        ch.all(aPointa, std::back_inserter(it));

        //convex hull computation
        buildConvexHull(aContainer, it.begin(), it.end(), myPredicate); 

        //maintaining convexity with the starting point
        updateConvexHull(aContainer, *it.begin(), myPredicate); 

        //copy
        //std::copy(container.begin(), container.end(), res); 
     

      }//end proc


}; 
#endif


