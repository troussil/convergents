#ifndef OutputSensitiveAlphaShape_h
#define OutputSensitiveAlphaShape_h

#include<cmath>

#include"../lib/CircumcircleRadiusPredicate.h"
#include "../lib/RayIntersectableStraightLine.h"
//#include "../lib/OutputSensitiveConvexHull.h"
#include "../lib/ConvexHullHelpers.h"

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
      Point nextLeft(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa, const Point& aPointb, int aMaxConv, OutputIterator aAlphaShapeHull)
      {

        // Initialisation of the convergent.
        // Convergent arise from pStart.
        Point pStart = aPointa;

        // First convergents vectors : 
        int rot_pi2[4];
        rot_pi2[0] = 0; rot_pi2[1] = -1; rot_pi2[2] = 1; rot_pi2[3] = 0;

        Point vm2 = Point(1,0);
        Point vm1 = Point(0,1);


        // Orientation of the convergent
        // vm2 outside and vm1 inside
        while (myShape(aPointa + vm2) > 0 || myShape(aPointa + vm1) < 0)
        {
          // pi/2 counter clockwise rotation
          vm2 = vm2.rotate(rot_pi2);
          vm1 = vm1.rotate(rot_pi2);
        }

        Point vConvM2;
        Point vConvM1;
        Point vConv;

        // First convergents points :
        Point pConvM2;
        Point pConvM1;
        // pConv is the next convergent pconv = pConvM2 + qk * vConvM1.
        Point pConv;
        int qk;

        // pConvM2 + qkalpha * vConvM1 is the first vertex in the alpha shape.
        int qkalpha;

        // True : We have add new vertex/ices for this convergent.
        bool nextVertex;

        // k is the convergent number. Usefull to know if the convergent is odd or even
        // ie : if the convergent is below or above the straight line
        int k;

        // The discrete straight-line [a, b]
        RayIntersectableStraightLine<Point> lineRatio(aPointa, aPointb);

        /**
         * Algorithm start.
         * We stop computing when we reach the end of the straight-line : aPointb
         * or if we reach a certain number = aMaxconv of convergent.
         */
        while (pStart != aPointb || k >= aMaxConv)
        {
          // We reset the convergents.
          vConvM2=vm2; 
          vConvM1=vm1; 

          pConvM2 = pStart + vConvM2;
          pConvM1 = pStart + vConvM1;  

          k = 0;
          nextVertex = false;

          /**
           * We start searching for next vertices and convergents.
           * We look after the intersection of the ray and the straight-line.
           * At every new vertices add, we reset the computation from a new start.
           */     
          while ( nextVertex == false )
          {
            if (lineRatio.dray(pConvM2, vConvM1, qk, pConv) == false)
            {
              nextVertex = true;
              *aAlphaShapeHull++ = aPointb;
              pStart = aPointb;

            }
            else
            {
              // pConv is calculate in lineRatio.dray(), so We update vConv
              vConv = pConv - pStart;

              if ( k % 2 != 0 || (pConv == aPointb && k % 2 == 0))
              {
                /**
                 * We test the parity of k :
                 * In the case, k is even, we follow searching for new convergent.
                 * In the other case, ie : k is odd, we are above the straight-line. We could  
                 * have new alpha-shape vertices.
                 */ 

                if (aPredicate(pStart, pConv-vConvM1, pConv) == false)
                {
                  /**
                   * In the family of triangle shaped by the three points : pStart, 
                   * pConv - q*vConvM1, pConv - (q+1)*vConvM1, the triangle T(pStart, 
                   pConv-vConvM1, pConv) have the greatest circumcircle radius.
                   * If its radius is smaller than the predicate radius, we have to search
                   * for new vertices.
                   * We throw the dichotomous method in order to find the first point
                   * in the alpha-shape.
                   */



                  if (pConv == aPointb && k % 2 == 0)
                  {

                    qkalpha = dichotomous(aPredicate, pConvM2, -vConvM2, vConvM1, qk);

                    int qks = qkalpha;
                    qkalpha = 1;

                    while ( qkalpha <= qk-qks)
                    {
                      *aAlphaShapeHull++ = pStart + qkalpha*vConvM1;
                      qkalpha++;

                    }
                    // We can have a new vertex between in aPointb - vConvM2.
                    if (qks == 0 && aPredicate.getNum2() < aPredicate.getDen2())
                    {
                      *aAlphaShapeHull++ = (pConv - vConvM2);
                    }
                    nextVertex = true;
                    *aAlphaShapeHull++ = aPointb;
                    pStart = aPointb;
                  }
                  else
                  {
                    qkalpha = dichotomous(aPredicate, pStart, vConvM2, vConvM1, qk);
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
                        pStart = (pConvM2 + vConvM1);
                      }
                      else
                      {
                        pStart = pConvM2;
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
                      pStart = pConv;

                    }
                    // We have to reset the convergent computation from pStart
                    nextVertex = true;
                  }
                } // if new vertex. 
              } // if k is odd.

              // Update Convergent
              k++;

              pConvM2 = pConvM1;
              pConvM1 = pConv;
              vConvM2 = vConvM1;
              vConvM1 = pConv-pStart;
            } // No new convergent or new vertex
          }
        }// found all the vertices
      }

    ///////////////////// main methods ///////////////////
    /**
     * Given a vertex of the convex hull, find the next
     * vertex in a counter-clockwise order
     * @param aPoint any vertex of the convex hull
     * @return the next vertex
     */

    Point next(const Point& aPoint)
    {
      /**
       * Initialisation
       * 
       */
      Point vm2( 1, 0);
      Point vm1( 0, 1);

      int rot_pi2[4];
      rot_pi2[0] = 0; rot_pi2[1] = -1; rot_pi2[2] = 1; rot_pi2[3] = 0;

      int pQuotient = 0;

      Point pConv;
      Point vConv;

      // Orientation of the convergent
      // vm2 outside and vm1 inside
      while (myShape(aPoint + vm2) > 0 || myShape(aPoint + vm1) < 0)
      {
        // pi/2 counter clockwise rotation
        vm2 = vm2.rotate(rot_pi2);
        vm1 = vm1.rotate(rot_pi2);
      }
      Point pm2 = aPoint + vm2;
      Point pm1 = aPoint + vm1;

      Point pNext = pm1; 
      Point vNext = pNext - aPoint;

      // p0 exisence an iteration
      if (myShape.dray(pm2, vm1, pQuotient, pConv) == false)
      {
        while(myShape(pm1+vm1) >= 0){pm1 += vm1;}
        return (pm1);
      }

      // p0 lie on the circle
      if (myShape(pConv) >= 0)
      {
        pNext = pConv;
        vNext = pNext - aPoint;
      }
      //p0 outise the circle
      else if (myShape(pConv + vm1) >= 0)
      {
        pNext = pConv + vm1;
        vNext = pNext - aPoint;
      }

      /**
       * We iterate the vectors to build the next convergent
       * p_-1 become p_-2
       * p_0 become p_-1
       * and their vector vm2, vm1
       */

      vConv = pConv - aPoint;
      pm2 = pm1;
      pm1 = pConv;

      vm2 = vm1;
      vm1 = pm1 - aPoint;

      //even or odd
      int ite = 1;

      // p1 and more
      while (myShape.dray(pm2, vm1, pQuotient, pConv) && pQuotient != 0)
      {
        // New Convergent, new v_i
        vConv = pConv - aPoint;
        if (!(ite & 1)) // even
        {
          if (myShape(pConv) == 0) // if the vertex lie on the circle
          {
            if (vNext.det(vConv) < 0)
            {
              pNext = pConv;
              vNext = vConv;
            }
          }
          else if (myShape(pConv + vm1) >= 0)
          {
            if (vNext.det(vConv + vm1) < 0)
            {
              pNext = pConv+vm1;
              vNext = vConv+vm1;
            }
          }
        }

        else // odd
        {
          if ( vNext.det(vConv)  < 0 )
          {
            pNext = pConv;
            vNext = vConv;
          }
        }

        ite++; 
        // update pm2 and pm1
        pm2 = pm1;
        pm1 = pConv;

        vm2 = vm1;
        vm1 = pConv - aPoint;

      }
      while(myShape(pNext+vNext) >= 0){pNext += vNext;}
      return pNext; 
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
      Point nextLeftInc(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa, int aMaxConv, OutputIterator aAlphaShapeHull)
      {
        //std::cout<<std::endl;   
        //std::cout<<"#0 - Enter : pStart :"<<aPointa<<std::endl;
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
         * ============================================= 
         */
        if (myShape.dray(pConvM2, vConvM1, qk, pConv) == false)
        {
          // We search for the next vertices in vConvM1 direction
          pConv = pConvM1;
          // Maximum length to the next alpha-neighbour.
          int kmax = aPredicate.getNum2()/aPredicate.getDen2();

          // VConvM1 translation
          while(myShape(pConv + vConvM1) >= 0)
          {
            pConv += vConvM1;
            k++;
            if (k == kmax)
            {// We need an alpha-neighbour to reach the end of the straight-line.
              *aAlphaShapeHull++ = pConv;
              k = 0;
            }
          }
          // We add the last vertex
          if (k != 0){*aAlphaShapeHull++ = pConv;}
          // We found at least one new vertex.
          return (pConv);
        } // end - p_0 does not exist
        // The else is not need because of the return.
        /*
         * =============================================
         */ 


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
          	//std::cout<<"#3.2 - paire - k : "<<k<<"| pConv : "<<pConv<<", pConvM2, pConvM1"<<pConvM2<<pConvM1<<std::endl;
            //std::cout<<"#3.2 - entrée calcul : "<<myShape(pConv)<<" resul : "<<aPredicate(aPointa, pConv-vConvM1, pConv)<<"; k = "<<k<<std::endl;  
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

          if ( k % 2 != 0 )
          {
            /**
             * We test the parity of k :
             * In the case, k is even, we follow searching for new convergent.
             * In the other case, ie : k is odd, we are above the straight-line. 
             * We could have new alpha-shape vertices if pConv lie on the shape.
             */ 
            //std::cout<<"#3.1 - parité - k : "<<k<<"| pConv : "<<pConv<<", pConvM2, pConvM1"<<pConvM2<<pConvM1<<std::endl;  
            //std::cout<<"#3.1 - entrée calcul : "<<myShape(pConv)<<" resul : "<<aPredicate(aPointa, pConv-vConvM1, pConv)<<"; k = "<<k<<std::endl;  
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

              //std::cout<<"#3.3 - impaire - k : "<<k<<"| pConv : "<<pConv<<", pConvM2, pConvM1"<<pConvM2<<pConvM1<<std::endl;
              //std::cout<<"#3.3 - entrée calcul : "<<myShape(pConv)<<" resul : "<<aPredicate(aPointa, pConv-vConvM1, pConv)<<"; k = "<<k<<std::endl;  
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

        if (myShape(pConvM2) > 0)
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
