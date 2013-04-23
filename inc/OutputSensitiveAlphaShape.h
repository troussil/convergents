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
      int qkstop  = aQk-1;
      int mid = 0;

      // while not yet located 
      while( qkstart < qkstop ) 
      {
        // middle between qkstart and qkstop
        mid = (qkstart + qkstop)/2;

        // radius test
        if ( myPredicate(aPoint, 
              (aPoint + aConvM2 + (mid+plus0)*aConvM1), 
              (aPoint + aConvM2 + (mid+plus1)*aConvM1)) )
        { //search in the upper range
          if ( !myPredicate(aPoint, 
                (aPoint + aConvM2 + (mid+2*plus0+plus1)*aConvM1), 
                (aPoint + aConvM2 + (mid+2*plus1+plus0)*aConvM1)) )
          {
            return(mid+1);
          }
          else
          {
            qkstart = mid+1;
          }    

        }
        else
        { //search in the lower range
          if(myPredicate(aPoint, 
                (aPoint + aConvM2 + (mid-2*plus0-plus1)*aConvM1), 
                (aPoint + aConvM2 + (mid-2*plus1-plus0)*aConvM1)))
          {
            return(mid-1);
          }
          else
          {
            qkstop = mid;
          }
        }
      } 
      // return the maximal integer such that the predicate is true
      return(qkstart);
    }   


    /**
     * Given a triangle formed by aStart, aStart+aBase, aStart+aBase+qk*aSide)
     * We add corresponding points to the alpha-shape vertices and return 
     * the last point in aNext.
     * @param aStart is the first, and constant vertices of the triangle.
     * @param aBase translate aStart to the first triangle
     * @param aSide translate aBase in aqk time to the last triangle.
     * @param aNext is the next point to start in the alpha-shape computation.
     * @param aqk is the coefficient to reach the last triangle vertex.
     * @param res output iterator that stores the sequence of vertices
     * @return the last retrieved vertex (not stored). 
     */
    template <typename OutputIterator>
      bool getASVertex(const Point& aStart, const Point& aBase, const Point& aSide,
          Point& aNext, int aqk, OutputIterator res)
      {
        // last triangles vertices.
        Point last = aStart + aBase + aqk * aSide;
        Point secondLast = last - aSide;

        // dicotomic search result
        int qkalpha;
        // coefficient to locate the newt vertex in the alpha-shape.
        int qkmove;

        // triangle orientation,
        // allowed to distinguished the odd or even case
        // counter clockwise = false, clockwise = true;
        bool aTrigoAngle = (myPredicate.getArea(aStart, aStart + aBase, aStart + aBase + aSide) >= 0 );

        // We looked for next vertices
        if( (aTrigoAngle && !myPredicate(aStart, secondLast, last)) ||
            (!aTrigoAngle && !myPredicate(aStart, last, secondLast)))
        {

          // We run the dichotomic search between 
          // aStart + aBase and 
          // aStart + aBase + qk * aSide
          qkalpha = dichotomicSearch(aStart, aBase, aSide, aqk);

          if (!aTrigoAngle)
          {
            // If qkalpha == 0, we have to deal with a special case
            // where pConvM2 is the last vertex of the alpha-shape.
            if (qkalpha == 0)
            {
              aNext = aStart + aBase;
              return(true);
            }
            else { qkmove = qkalpha;} 
          }
          else
          {
            aqk--;
            qkmove = aqk;
            qkalpha--;
          } 

          // We add all the vertices of the form pConvM2 + i * vConvM1
          // for all i between qkalpha and qk (excluded).
          // pConv (qk included) is the last vertex of the alpha-shape.

          while ( qkalpha < aqk)
          {
            *res++ = aStart + aBase + qkmove*aSide;                   
            move(qkalpha, aqk, qkmove, aTrigoAngle); 
          }
          aNext = last;
          return(true);

        } // end of predicate is false
        return (false);
      }

    /**
     * Change all the value which determined the next point to add in
     * the alpha-shape.
     * @param aqkalpha 
     * @param aqk
     * @param aqkmove
     * @param aDirection determined the triangle orientation
     * @return 
     */
    void move(int& aqkalpha, int& aqk, int& aqkmove, bool aDirection)
    {
      if(!aDirection)
      {
        aqkalpha++;
        aqkmove++;
      }
      else 
      {
        aqk--;
        aqkmove--;
      }
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
      Point next(const Point& aPoint, OutputIterator res, bool aAlphaInf)
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

        // Next starting vertex
        Point next;

        //Ray casting from pConvM2 in the direction vConvM1
        while (myShape.dray(pConvM2, vConvM1, qk, pConv))
        {
          //If pConv is outside the shape (k is odd) 
          if (myShape(pConv) < 0)
          {
            // If qk == 0 (ie. pConv == pConvM2) 
            // pConvM1, ie. the last convergent inside the shape
            // is the next vertex of the alpha-shape. 
            // Otherwise, we just update the convergents and loop.
            if (k > 0 && qk <= 0) 
            {
              // pConvM1 translation
              Point prevLastPoint = pConvM1;
              Point lastPoint = prevLastPoint + vConvM1;

              while (myShape(lastPoint) >= 0)
              {
                // Convex Hull case, we do not add the vertex
                if (!aAlphaInf)
                { 
                  *res++ = prevLastPoint;
                }
                prevLastPoint = lastPoint; 
                lastPoint += vConvM1;
              } 
              return(prevLastPoint);
            }
          }
          else
          { //If pConv is inside or on the shape
            // NB: we return at least pConv as a vertex of the alpha-shape.
            // but possibly several other vertices before pConv. 
            if (k % 2 == 0)
            { // If k is even; aPoint, pConv, pConv-vConvM1 are CCW-oriented. 
              // If the radius of the circumcircle of aPoint, pConv, pConv-vConvM1 
              // is NOT greater than (or equal to) 1/alpha
              // then there must be vertices of the alpha-shape
              // of the form aPoint + vConvM2 + i * vConvM1

              getASVertex(pConv, -vConvM2, -vConvM1, next, qk, res);
              return(pConv);

            }
            else
            { // If k is odd; aPoint, pConv, pConv-vConvM1 are CW-oriented. 
              // If the radius of the circumcircle of aPoint, pConv, pConv-vConvM1 
              // is NOT greater than (or equal to) 1/alpha
              // then there must be vertices of the alpha-shape
              // of the form aPoint + vConvM2 + i * vConvM1

              if ( getASVertex(aPoint, vConvM2, vConvM1, next, qk, res))
              {
                return(next);
              }

            }//end of k is even
          }// end of not ouside

          //If pConv is outside the shape and different from pConvM2
          //or pConv is inside the shape but without being a vertex, 
          //we have to updade the convergents
          k++;
          pConvM2 = pConvM1;
          pConvM1 = pConv;
          vConvM2 = vConvM1;
          vConvM1 = pConv-aPoint;  
        }// end ray-shooting and loop	

        // retrieval of the points lying 
        // on an edge of the alpha-shape. 
        // enable or disable the
        // retrieval of all such points  
        // with aAlphaInf 
        Point prevLastPoint = pConvM1;
        Point lastPoint = prevLastPoint + vConvM1; 

        while (myShape(lastPoint) >= 0)
        {
          // Convex Hull case, we do not add the vertex
          if (!aAlphaInf)
          { 
            *res++ = prevLastPoint;
          }
          prevLastPoint = lastPoint; 
          lastPoint += vConvM1;
        } 
        return(prevLastPoint);
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
        // get the first vertex
        Point tmp = aStartingPoint; 
        bool alphainf = false;

        // if the denominator == 0, the radius is infinite.
        // We don't keep colinear vertices.
        if((*this).getPredicate().getDen2() == 0)
          alphainf = true;

        do 
        {
          // stores the last retrieved vertex
          *res++ = tmp; 
          // get the next alpha-shape vertices
          tmp = next(tmp, res, alphainf);
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

