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
     * @param aPoint any vertex of the alpha-shape 
     * @param res output iterator that stores the sequence of vertices
     * @return the last retrieved vertex (not stored). 
     */
    template <typename OutputIterator>
      Point next(const Point& aPoint, OutputIterator res)
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
        while (myShape(aPoint + vConvM2) > 0 || myShape(aPoint + vConvM1) < 0)
        {
          // pi/2 counter clockwise rotation
          vConvM2 = vConvM2.rotate(rot_pi2);
          vConvM1 = vConvM1.rotate(rot_pi2);
        }

        // First convergents
        Point pConvM2 = aPoint + vConvM2;
        Point pConvM1 = aPoint + vConvM1;

        // k is the convergent index. Useful to know if the convergent is odd or even
        // ie : if the convergent is below or above the shape
        int k = 0;

        // pConv is the next convergent pConv = pConvM2 + qk * vConvM1.
        int qk;
        Point pConv;

        // pConvM2 + qkalpha * vConvM1 is the first vertex in the alpha shape.
        int qkalpha;

        while (myShape.dray(pConvM2, vConvM1, qk, pConv) == true)
        {
          // pConv is computed in dray, so we update vConv
          vConv = pConv - aPoint;

          /**
           * We search for new vertices insides the alpha-hull, ie : k is odd.
           * The circumcircle radius of the triangle formed by the new vertex 
           * T(aPoint, pConv, pConv-vConvM1) also need to be shorter than the
           * stored radius
           */
          if ( k % 2 != 0 && myPredicate(aPoint, pConv, pConv-vConvM1) == false)
          {
            // We throw the dichotomus search
            qkalpha = dichotomous(myPredicate, aPoint, vConvM2, vConvM1, qk);

            /**
             * If qkalpha == 0, we have to deal with a special case.
             * We have to restart from pConvM2 in order to not missed any vertex.
             */
            if (qkalpha == 0)
            {
              //*res++ = pConvM2;
              return(pConvM2); 
            }
            else
            {
              /**
               * We add all the vertices between qkalpha and qk in the alpha-Shape.
               * We restart from the last vertex add : pConv.
               */
              while (qkalpha < qk)//instead of <= 
              {
                *res++ = pConvM2 + qkalpha*vConvM1;
                qkalpha++;  
              }
              return(pConv);
            }
          }//end k odd and predicate ok.
          else 
          {
            /**
             * If pConv lie on the alpha-shape, we reach a vertex of the 
             * convex-shape. 
             */
            if (myShape(pConv) == 0)
            {
              if (myPredicate(aPoint, pConv- vConvM1, pConv ) == false)
              {
                // We throw the dichotomus search
                qkalpha = dichotomous(myPredicate, aPoint, vConvM2, vConvM1, qk);

                /**
                 * We add all the vertices between 1 and qk-qkalpha in the alpha-Shape.
                 * We add and restart from the last vertex add : pConv.
                 */
                int qks = qkalpha;
                qkalpha = 1;
                while ( qkalpha <= qk-qks)
                {
                  *res++ = aPoint + qkalpha*vConvM1;
                  qkalpha++;
                }

              }
              //*res++ = pConv;
              return(pConv);
            }
            else 
            {
              // The ray shooting doesn't give a new convergent ie qk = 0
              if (k > 0 && qk <= 0)
              { 
                // We add and restart from the last convergent inside the alpha-hull
                if (myShape(pConv) > 0)	
                {
                  //*res++ = pConv;
                  return(pConv);
                }
                else	
                {
                  //*res++ = pConvM1;
                  return(pConvM1);
                }	
              }
              else 
              {
                // Update Convergent
                k++;
                pConvM2 = pConvM1;
                pConvM1 = pConv;
                vConvM2 = vConvM1;
                vConvM1 = pConv-aPoint;
              } 
            }
          }
        } //end while - ray shooting

        /**
         * No more intersection. We search for pConvM1 translation.
         * We can't change aPoint, so we use pConv instead.
         */
        Point prevLastPoint = aPoint;
	Point lastPoint = prevLastPoint + vConvM1; 
	if (myShape(lastPoint) >= 0)
	  {
	    prevLastPoint = lastPoint; 
	    lastPoint += vConvM1;
	    while (myShape(lastPoint) >= 0)
	      {
		*res++ = prevLastPoint;
		prevLastPoint = lastPoint; 
		lastPoint += vConvM1;
	      } 
	    return(prevLastPoint);
	  }
	else 
	  return lastPoint; 

	// obsolete
        // while(myShape(pConv + vConvM1) >= 0)
        // {
        //   pConv += vConvM1;
	//   std::cerr << "stored " << pConv[0] << "," << pConv[1] << std::endl; 
        //   *res++ = pConv;
        // }
        // return(pConv);
      } // end - proc

  /**
   * Retrieves all the vertices of the alpha-shape
   * in a counter-clockwise order from a given vertex
   *  
   * @param aStartingPoint a vertex of the alpha-shape
   * @param res output iterator that stores the sequence of vertices
   */
  template <typename OutputIterator>
  void all(const Point& aStartingPoint, OutputIterator res)
  {
    //get the first vertex
    Point tmp = aStartingPoint; 

    do 
      {
	//stores the last retrieved vertex
	*res++ = tmp; 
	// get the next alpha-shape vertices
	tmp = next(tmp, res);
	//while it is not the first one
      } while (tmp != aStartingPoint); 
  }

  /**
   * Retrieves all the vertices of the alpha-shape
   * in a counter-clockwise order
   *  
   * @param res output iterator that stores the sequence of vertices
   */
  template <typename OutputIterator>
  void all(OutputIterator res)
  {
    all(myShape.getConvexHullVertex(), res); 
  }

}; 
#endif
