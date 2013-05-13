#ifndef OutputSensitiveAlphaShape_h
#define OutputSensitiveAlphaShape_h

#include<cmath>

#include"CircumcircleRadiusPositivePredicate.h"
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
        
//    std::cout << "Convex Hull" << std::endl; 
//    std::copy(it.begin(), it.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
//    std::cout << std::endl;
    
        int i = 0;

        // Init point
        Point pStart = it[i++];
        Point pFol1  = it[i++];
        Point pFol2  = it[i++];       
       
        it.push_back(pStart);
        
        // Init Container
        aContainer.push_back(pStart);
        bool cycle = false;
     
     
//std::cout <<"IN : "<<it.size()<< std::endl;
        
        while( !cycle )
        {
std::cout <<"Enter : " <<i<<pStart<< pFol1 <<pFol2 << std::endl; 
          // Add second point in the container
          aContainer.push_back(pFol1);        
          
          // TRUE -- R_T(a,b1,b2)>R_alpha -- b1 not in the alpha-shape
          if (myPredicate(pStart, pFol1, pFol2))
          {
std::cout <<"POP --> " << pFol1 << aContainer.front()<<std::endl;

            // remove b1
            if (pFol1 == aContainer.front())
            {
              aContainer.pop_front();
              cycle = true;
            }
            aContainer.pop_back();
          }
          // FALSE --  R_T(a,b1,b2)<R_alpha -- b1 in the alpha-shape
          else
          {
std::cout <<"PUSH --> " << pFol1 << aContainer.front()<<std::endl;
            // New start
            if (pFol1 == aContainer.front())
            {
              cycle = true;
              aContainer.pop_back();
            }
            else
            {
              pStart = pFol1;
              it.push_back(pFol1);
            }
          }
          // update Points
          pFol1 = pFol2;
          pFol2 = it[i++];    
//std::cout <<"OUT ; "<<i<<pStart<< pFol1 <<pFol2 << std::endl;    
        }
             
      }//end proc


}; 
#endif


