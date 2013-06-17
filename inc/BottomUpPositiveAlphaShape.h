#ifndef BottomUpPositiveAlphaShape_h
#define BottomUpPositiveAlphaShape_h

#include<cmath>

#include"CircumcircleRadiusPredicate.h"

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
class BottomUpPositiveAlphaShape
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
    BottomUpPositiveAlphaShape(const Shape& aShape, const Predicate& aPredicate)
      : myShape(aShape), myPredicate(aPredicate) {}

  private:
    /**
     * Copy constructor
     * @param other other object to copy
     */
    BottomUpPositiveAlphaShape(const BottomUpPositiveAlphaShape& other) {}

    /**
     * Assignement operator
     * @param other other object to copy
     * @return reference on *this
     */
    BottomUpPositiveAlphaShape& operator=(const BottomUpPositiveAlphaShape& other)
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
    ~BottomUpPositiveAlphaShape() {}


    ///////////////////// main methods ///////////////////
  public:

    /**
     * Given a vertex of the alpha-shape, find the next
     * vertex in a counter-clockwise order
     * @param aPoint any vertex of the alpha-shape
     * @param res which contained the alpha-shape vertices
     * @return
     */

    void next(const Point& aPoint, Container& res)
    {
      
      //we hav not enough vertices to test predicate with a triangle
      if(res.size() < 2)
      {
        res.push_back( aPoint ); 
      }
      else
      {
        //maintaining convexity with the new point
        updateConvexHull(res, aPoint, myPredicate); 
        //add new point
        res.push_back( aPoint ); 
      }

    }

     /**
     * Retrieves all the vertices of the alpha-shape
     * in a counter-clockwise order from a given vertex
     *  
     * @param aStartingPoint a vertex of the alpha-shape
     * @param res output iterator that stores the sequence of vertices
     */

    template <typename Container, typename Point>
      void all(Container& res, Point aStartingPoint)
      {
        // Retrieve the convex hull vertices
        OutputSensitiveConvexHull<TShape> ch(myShape);
        std::vector<Point> resCH;
        ch.all(aStartingPoint, std::back_inserter(resCH), false);

        typename std::vector<Point>::iterator it = resCH.begin();
        //auto it = resCH.begin(); // c++ 11
        Point tmp = *it;
                
        do{
          
          // add the next alpha-shape vertices
          next(tmp, res);
          tmp = *it;

          //while it is not the first one
        }while (it++ != resCH.end());

        //maintaining convexity with the starting point
        updateConvexHull(res, aStartingPoint, myPredicate);
      }//end proc


}; 
#endif



