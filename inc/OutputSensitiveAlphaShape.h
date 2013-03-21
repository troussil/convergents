#ifndef OutputSensitiveAlphaShape_h
#define OutputSensitiveAlphaShape_h

#include<cmath>

#include"CircumcircleRadiusPredicate.h"
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
     * Dichotomous search procedure
     * @param aPredicate determine the alpha shape radius
     * @param aPointa is the first vertex of the triangle
     * @param Points aConvM2 and aConvM1 are the next convergents vectors
     * @param aqk pConv = aqk*aConvM1 + aConvM2
     * @return coefficient of the first/ next convergent in the alpha shape
     */
    template <typename CircumcircleRadiusPredicate, typename Point>
      int dichotomous(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa,
          const Point aConvM2, const Point aConvM1, const int aqk)
      {
        // init search milestone
        int qkstart = 0;
        int qkstop  = aqk;

        // middle
        int mid;

        // while we finish the dichotomy 
        while( qkstop != qkstart ) 
        {
          // Find the (integer) middle between qkstart and qkstop
          mid = (qkstart + qkstop)/2;

          // higher triangle predicate
          if (aPredicate(aPointa, (aPointa + aConvM2 + (mid+1)*aConvM1), 
                (aPointa + aConvM2 + (mid)*aConvM1)) == true)
          { // the vertex is higher
            qkstart = mid + 1;
          }
          else
          { // the vertex is lower
            qkstop = mid;
          }
        } 
        // return the coefficient of the first vertex inside the alpha-shape.
        return(qkstart);
      }   

    /**
     * Given a vertex of the alpha-shape, find a serie of vertices
     * in a counter-clockwise order
     * 
     * @param aPredicate determine the alpha-shape radius
     * @param aPoint any vertex of the alpha-shape 
     * @param aAlphaShape Output Iterator which log a serie of vertices
     * @return the last vertex log by the output iterator
     */
    template <typename CircumcircleRadiusPredicate, typename Point, typename OutputIterator>
      Point next(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa, int aMaxConv, OutputIterator aAlphaShapeHull)
      {
        // Initialisation of the convergent.
        // First convergents vectors : 
        int rot_pi2[4];
        rot_pi2[0] = 0; rot_pi2[1] = -1; rot_pi2[2] = 1; rot_pi2[3] = 0;

        Point vConvM2 = Point(1,0);
        Point vConvM1 = Point(0,1);
        Point vConv   = Point(0,0);

        // Orientation of the convergent
        // vm2 outside and vm1 inside
        while (myShape(aPointa + vConvM2) > 0 || myShape(aPointa + vConvM1) < 0)
        {
          // pi/2 counter clockwise rotation
          vConvM2 = vConvM2.rotate(rot_pi2);
          vConvM1 = vConvM1.rotate(rot_pi2);
        }

        // First convergents
        Point pConvM2 = aPointa + vConvM2;
        Point pConvM1 = aPointa + vConvM1;
        // pConv is the next convergent pconv = pConvM2 + qk * vConvM1.

        Point pConv;
        int qk;

        // pConvM2 + qkalpha * vConvM1 is the first vertex in the alpha shape.
        int qkalpha;

        // True : We have add new vertex/ices for this convergent.
        bool enter = true;

        // k is the convergent number. Usefull to know if the convergent is odd or even
        // ie : if the convergent is below or above the straight line
        int k = 0;

        /**
         * p0 does not exist.
         * We can only translate in the pConvM1 direction.
         * We could add the translation in one time.
         */
        if (myShape.dray(pConvM2, vConvM1, qk, pConv) == false)
        {
          // We search for the next vertices in vConvM1 direction
          pConv = aPointa;

          // VConvM1 translation
          while(myShape(pConv + vConvM1) >= 0)
          {
            pConv += vConvM1;
            *aAlphaShapeHull++ = pConv;
          }
          return (pConv);
        } // end - p_0 does not exist
        // The else is not need because of the return.

        /**
         * We start searching for next vertices and convergents.
         * Everytime, we found a set of vertices, we restart this procedure.
         * It also stop if there is no intersection between the ray and the shape.
         */ 

        while (myShape.dray(pConvM2, vConvM1, qk, pConv) == true)
        {
          // pConv is calculate in lineRatio.dray(), so we update vConv
          vConv = pConv - aPointa;
          if (k % 2 == 0 && myShape(pConv) >=0)
          {  
          	
            qkalpha = dichotomous(aPredicate, pConvM2, -vConvM2, vConvM1, qk);
            int qks = qkalpha;
            qkalpha = 1;
            while ( qkalpha <= qk-qks)
            {
              *aAlphaShapeHull++ = aPointa + qkalpha*vConvM1;
              qkalpha++;

            }
            // We can have a new vertex between in aPointb - vConvM2.
            if (qks == 0 && aPredicate.getNum2() < aPredicate.getDen2())
            {
              *aAlphaShapeHull++ = (pConv - vConvM2);
            }
            *aAlphaShapeHull++ = pConv;
            return pConv;
          }
					else if ( k % 2 != 0 )
          {
            /**
             * We test the parity of k :
             * In the case, k is even, we follow searching for new convergent.
             * In the other case, ie : k is odd, we are above the straight-line. 
             * We could have new alpha-shape vertices if pConv lie on the shape.
             */ 
            if (aPredicate(aPointa, pConv-vConvM1, pConv) == false)
            {
              /**
               * In the family of triangle shaped by the three points : pStart, 
               * pConv - q*vConvM1, pConv - (q+1)*vConvM1, the triangle T(pStart, 
               * pConv-vConvM1, pConv) have the greatest circumcircle radius.
               * If its radius is smaller than the predicate radius, we have to search
               * for new vertices.
               * We throw the dichotomous method in order to find the first point
               * in the alpha-shape.
               */

              qkalpha = dichotomous(aPredicate, aPointa, vConvM2, vConvM1, qk);
              if (qkalpha == 0)
              {
                /**
                 * If qkalpha == 0, we have to deal with special case.
                 * In every case, pConvM2 is a new vertex.
                 */
                *aAlphaShapeHull++ = pConvM2;

                if (pConvM1.normL22() == 1)
                {
                  // We can have a new vertex between pConvM2 and pConv.
                  *aAlphaShapeHull++ = (pConvM2 + vConvM1);
                  return(pConvM2 + vConvM1);
                }
                else
                {
                  return(pConvM2);
                }
              }
              else
              {
                /**
                 * We add all the vertices between qkalpha and qk in the alpha-Shape.
                 * We restart from the last vertex add : pConv.
                 */
                while (qkalpha <= qk)
                {
                  *aAlphaShapeHull++ = pConvM2 + qkalpha*vConvM1;
                  qkalpha++;  
                }
                return(pConv);
              }
              // We have to reset the convergent computation from pStart

            } // if new vertex. 
          } // if k is odd.

          // Update Convergent
          k++;

          pConvM2 = pConvM1;
          pConvM1 = pConv;
          vConvM2 = vConvM1;
          vConvM1 = pConv-aPointa;
        } // No new convergent or new vertex

        /**
         * No intersection
         * We add and return the last convergent to the alpha-shape.
         */ 

        if (myShape(pConvM2) >= 0)
        {
        	//std::cout<<"#2 - Paire : pConvM1 : "<<k<<pConvM1<<std::endl;  
          *aAlphaShapeHull++ = pConvM1;
          return pConvM1;
        }
        else
        {
        	//std::cout<<"#2 - Paire: pConv : "<<pConv<<std::endl;  
          *aAlphaShapeHull++ = pConvM2;
          return pConvM2;
        }
      } // end - proc

}; 
#endif
