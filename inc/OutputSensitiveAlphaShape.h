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
        int mid;

        int plus0;
        int plus1;
        // middle
        if (aPredicate.getArea(aPointa, aPointa + aConvM2, aPointa + aConvM2 + aConvM1) >= 0)
        {
          plus0 = 0;
          plus1 = 1;
        }
        else
        {
          plus0 = 1;
          plus1 = 0;
        }


        // while we finish the dichotomy 
        while( qkstop != qkstart ) 
        {
          // Find the (integer) middle between qkstart and qkstop
          mid = (qkstart + qkstop)/2;

          // higher triangle predicate
          if (aPredicate(aPointa, (aPointa + aConvM2 + (mid+plus0)*aConvM1), 
                (aPointa + aConvM2 + (mid+plus1)*aConvM1)) == true)
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

        Point pNext;
        Point pLast;
        
        // pConvM2 + qkalpha * vConvM1 is the first vertex in the alpha shape.
        int qkalpha;

        // True : We have add new vertex/ices for this convergent.
        bool nextVertex = true;

        // k is the convergent number. Usefull to know if the convergent is odd or even
        // ie : if the convergent is below or above the straight line
        int k = 0;


				while (myShape.dray(pConvM2, vConvM1, qk, pConv) == true)
        {
          // pConv is calculate in lineRatio.dray(), so we update vConv
          vConv = pConv - aPointa;

					if ( k % 2 != 0 && aPredicate(aPointa, pConv, pConv-vConvM1) == false)
		  		{
						qkalpha = dichotomous(aPredicate, aPointa, vConvM2, vConvM1, qk);
 				 
						if (qkalpha == 0)
						{
						  /**
						   * If qkalpha == 0, we have to deal with special case.
						   * In every case, pConvM2 is a new vertex.
						   */
						  *aAlphaShapeHull++ = pConvM2;
					  	return(pConvM2); 

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
					}//end k even and predicate
					else // Update Convergent
					{
						// pConv lie on the alpha-shape
				 		if (myShape(pConv) == 0)
				 		{
				 			if (aPredicate(aPointa, pConv- vConvM1, pConv ) == false)
					 		{
							
								qkalpha = dichotomous(aPredicate, aPointa, vConvM2, vConvM1, qk);
	
								int qks = qkalpha;
								qkalpha = 1;

								while ( qkalpha <= qk-qks)
								{
									*aAlphaShapeHull++ = aPointa + qkalpha*vConvM1;
									qkalpha++;
								}

					 		}
							*aAlphaShapeHull++ = pConv;
							return(pConv);
					  }
				 		else 
				 		{
				 			if (k > 0 && qk <= 0)
				 			{
								if (myShape(pConv) >0)	
					 			{
					 				*aAlphaShapeHull++ = pConv;
					 				return(pConv);
					 			}
					 			else	
					 			{
					 				*aAlphaShapeHull++ = pConvM1;
					 				return(pConvM1);
					 			}	
					 		}
							else 
				 			{
								k++;
								pLast   = pConvM2;
								pConvM2 = pConvM1;
								pConvM1 = pConv;
								vConvM2 = vConvM1;
								vConvM1 = pConv-aPointa;
							} 
						}
					}
        } //end while - ray shooting
        
        /**
         * No more intersection
         * We add and return the last convergent inside the alpha-hull.
         */ 

        if (myShape(pConvM2) >= 0)
        {
           // We search for the next vertices in vConvM2 direction
          pConv = aPointa;

          // VConvM2 translation
          while(myShape(pConv + vConvM2) >= 0)
          {
            pConv += vConvM2;
            *aAlphaShapeHull++ = pConv;
          }
          return(pConv);
        }
        else
        {
           // We search for the next vertices in vConvM1 direction
          pConv = aPointa;

          // VConvM1 translation
          while(myShape(pConv + vConvM1) >= 0)
          {
            pConv += vConvM1;
            *aAlphaShapeHull++ = pConv;
          }
          return(pConv);
        }
 
      } // end - proc

}; 
#endif
