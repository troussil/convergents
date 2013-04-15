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
   * Dichotomic search procedure to retrieve the maximal integer q
   * such that @e myPredicate is true, ie. 
   * the circumcircle of @e aPoint,  
   * @e aPoint + q * @e aConvM1 + @e aConvM2
   * and @e aPoint + @e aQk * @e aConvM1 + @e aConvM2
   * is greater than 1/alpha.

   * @param aPoint origin of the local domain of computation
   * @param aConvM2 (k-2)-th convergent
   * @param aConvM1 (k-1)-th convergent
   * @param aQk integer such that the k-th convergent is
   * equal to aQk*aConvM1 + aConvM2
   * @return maximal integer such that the predicate is true
   */
  int dichotomicSearch(const Point& aPoint,
		       const Point aConvM2, const Point aConvM1, const int aQk)
  {

    // orientation test
    int plus0;
    int plus1;
    if (myPredicate.getArea(aPoint, aPoint + aConvM2, aPoint + aConvM2 + aConvM1) >= 0)
      {
	plus0 = 0;
	plus1 = 1;
      }
    else
      {
	plus0 = 1;
	plus1 = 0;
      }

    // init search bounds
    int qkstart = 0;
    int qkstop  = aQk;
    int mid = 0;

    // while not yet located 
    while( qkstop != qkstart ) 
      {
	// middle between qkstart and qkstop
	mid = (qkstart + qkstop)/2;

	// radius test
	if ( myPredicate(aPoint, 
			 (aPoint + aConvM2 + (mid+plus0)*aConvM1), 
			 (aPoint + aConvM2 + (mid+plus1)*aConvM1)) )
          { //search in the upper range
            qkstart = mid + 1;
          }
	else
          { //search in the lower range
            qkstop = mid;
          }
      } 
    // return the maximal integer such that the predicate is true
    return(qkstart);
  }   

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
  template <typename OutputIterator>
  Point next(const Point& aPoint, OutputIterator res)
  {
    // Initialisation of the convergents.
    Point vConvM2 = Point(1,0); //(k-2)-th convergent
    Point vConvM1 = Point(0,1); //(k-1)-th convergent

    // Orientation of the first convergents.
    // vConvM2 outside and vConvM1 inside
    int rot_pi2[4];
    rot_pi2[0] = 0; rot_pi2[1] = -1; rot_pi2[2] = 1; rot_pi2[3] = 0;
    while (myShape(aPoint + vConvM2) > 0 || myShape(aPoint + vConvM1) < 0)
    {
	    // pi/2 counter clockwise rotation
	    vConvM2 = vConvM2.rotate(rot_pi2);
	    vConvM1 = vConvM1.rotate(rot_pi2);
    }

    // First convergent points
    Point pConvM2 = aPoint + vConvM2;
    Point pConvM1 = aPoint + vConvM1;

    // k is the convergent index. 
    // Useful to know if the convergent is odd or even
    // ie : if the convergent is below or above the shape
    int k = 0;

    // pConv is the k-th convergent such that 
    // pConv = pConvM2 + qk * vConvM1.
    int qk;
    Point pConv;

    // pConvM2 + qkalpha * vConvM1 is the first vertex in the alpha shape.
    int qkalpha;
 
 
    /**
     * We enter the loop if p_0 exist, ie if we do not rise a side of the shape.
     * Because, there is no point toward p_-2, we must translate in p_-1 
     * direction.
     */
  
    while (myShape.dray(pConvM2, vConvM1, qk, pConv))
    {
	    /**
	     * We look for the k-th convergent ; pConv placement toward the shape 
	     * and start with the case where pConv is outside the shape. 
	     * As pConv is inside, k must be odd.
	     */
	    if (myShape(pConv) < 0)
	    {
	      /**
	       * We look after the efficiency of the ray shooting. If we stay at the same 
	       * convergent for k>0, ie if k==0. We have to restart from the last 
	       * convergent inside the shape : pConvM1.
	       * Else, we just update the convergent and follow our computation.
	       */
	      if (k > 0 && qk <= 0) 
	        return(pConvM1);
	    }//end of ouside
	    else
	    {
	      /**
	       * pConv, the k-th convergent can not be outside the shape. So it
	       * must be inside or lying on the shape. In the same time k can be odd
	       * if pConv lie on the shape, or even. We start looking for the case 
	       * where k is odd. Because pConv lie on the shape, we knew that we have
	       * at least to return pConv as the next vertex of the alpha-shape.
	       */
	      if (k % 2 == 0)
	      {
	        /**
	         * We have to deal with a special case. pConv lie on the shape and k
	         * is odd. So pConvM2 is outside the shape so as pConvM2 + qkalpha*pconvM1
	         * witk qkalpha < qk. Nevertheless, the situation is pretty symetric, so 
	         * we can looking for the predicate outside the shape. We just have to 
	         * take care to add the symetric point to the alpha-shape.
	         * At least, we only restart from pConv.
	         */
	        if ( !myPredicate(aPoint, pConv- vConvM1, pConv) )
		      {
		        /**
		         * We run the dichotomic search between 
		         * aPoint and 
		         * aPoint + qk * vConvM1
		         * We take care of the triangle orientation issue in the dichotomic
		         * search fonction.
		         */
		        qkalpha = dichotomicSearch(aPoint, vConvM2, vConvM1, qk);
		        
		        // We add all the vertices between 1 and qk-qkalpha in the alpha-Shape.
		        // We add and restart from the last vertex add : pConv.
		        int qks = qkalpha;
		        qkalpha = 1;

		        while ( qkalpha <= qk-qks)
		        {
			        *res++ = aPoint + qkalpha*vConvM1;
			        qkalpha++;
		        }
		      }
		      return(pConv);
	      }// end of k odd
	      else
	      {
	        /**
	         * k is even and pConv is inside or lying on the circle.
	         * This is the common situation where we look for new alpha-shape
	         * vertices by chekcing if the predicate is false.
	         * If this is not the case, we just go further to update the 
	         * convergent.
	         */
	        if(!myPredicate(aPoint, pConv, pConv-vConvM1))
	        {
	          // We run the dichotomic search between 
	          // aPoint + vConvM2 and 
	          // aPoint + vConvM2 + qk * vConvM1
            qkalpha = dichotomicSearch(aPoint, vConvM2, vConvM1, qk);
            
            // If qkalpha == 0, we have to deal with a special case.
            // pConvM2 is the last vertex of the alpha-shape.
	          if (qkalpha == 0)
	          {
		          return(pConvM2); 
	          }
            else
	          {
		          // We add all the vertices of the form pConvM2 + i * vConvM1
		          // for all i between qkalpha and qk (excluded).
		          // pConv (qk included) is the last vertex of the alpha-shape.
		          while (qkalpha < qk)
		          {
		            *res++ = pConvM2 + qkalpha*vConvM1;
		            qkalpha++;  
		          }
		          return(pConv);
	          }
	        } // end of predicate is false
	      }//end of k is even
	    }// end of not ouside
	   /**
	    * We can come from two situations. pConv can be outside the shape and different 
	    * from pConvM2 or pConv can also be inside the shape but without being an 
	    * alpha-shape vertex.
	    * So we have to update the convergent and checking again the looping
	    * condition.
	    */
		  k++;
		  pConvM2 = pConvM1;
		  pConvM1 = pConv;
		  vConvM2 = vConvM1;
		  vConvM1 = pConv-aPoint;  
    }// end ray-shooting and loop	
    
    /**
     * The ray shooting failed. 
     * We look for how many vertices in pConvM1 direction we can add to the
     * alpha-shape and restart from the last one.
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
	}//end proc


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
