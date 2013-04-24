#ifndef OutputSensitiveConvexHull_h
  #define OutputSensitiveConvexHull_h

#include<cmath>

/**
 * Class implementing a functor...
 * Basic usage: 
 * @code
  DGtal::BigInteger res; 
  res = Determinant<DGtal::BigInteger>::get(u,v);  
 * @endcode
 * 
 * @tparam TShape a model of ray-intersectable shape.  
 *
 */
template <typename T = long long int>
struct Determinant
{
  /**
   * Given three points, returns their orientation. 
   *
   * @param a first point
   * @param b second point
   * @param c third point
   * @return true if the signed area of the triangle is positive
   *
   * @tparam Point a model of point.   
   */
  template<typename Vector>
  static T
  get(const Vector& u, const Vector& v)
  {
    T u0 = u[0];
    T u1 = u[1];
    T v0 = v[0];
    T v1 = v[1]; 
//    return ( u0*v1 - ... );
    return (u0*v1 - u1*v0); 
  }
};

/**
 * Class implementing an on-line and ouput-sensitive algorithm
 * that retrieves the vertices of the convex hull of all digital
 * points lying inside an implicit shape, which is 'ray-intersectable', 
 * ie. the intersection between the shape boundary and 
 * a ray emanating from a given point along a given direction
 * is computable.
 * 
 * @tparam TShape a model of ray-intersectable shape.  
 *
 */
template <typename TShape, typename T = long long int>
class OutputSensitiveConvexHull 
{
  public: 
    /////////////////////// inner types /////////////////
    typedef TShape Shape; 
    typedef typename Shape::Point Point; 

  private: 
    /////////////////////// members /////////////////////
    /**
     * const reference on a shape
     */
    const Shape& myShape;  

  public:
    ///////////////////// standard services /////////////
    /**
     * Standard constructor
     * @param aShape
     */
    OutputSensitiveConvexHull(const Shape& aShape)
      : myShape(aShape) {}

  private:
    /**
     * Copy constructor
     * @param other other object to copy
     */
    OutputSensitiveConvexHull(const OutputSensitiveConvexHull& other) {}

    /**
     * Assignement operator
     * @param other other object to copy
     * @return reference on *this
     */
    OutputSensitiveConvexHull& operator=(const OutputSensitiveConvexHull& other) 
    { return *this; }

  public: 
    /**
     * Default destructor
     */
    ~OutputSensitiveConvexHull() {}


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
  //std::cout << aPoint << "\t"<< vm1 << "\t"<< vm2<<std::endl;
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
      bool even = false;

      // p1 and more
      while (myShape.dray(pm2, vm1, pQuotient, pConv) && pQuotient != 0)
      {
  std::cout << aPoint << "\t"<< vm1 << "\t"<< vm2<<", pConv = "<<pConv <<", qk = "<<pQuotient<<std::endl;      
        // New Convergent, new v_i
        vConv = pConv - aPoint;
        if (even) // even
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
            DGtal::BigInteger res; 
            res = Determinant<DGtal::BigInteger>::get(vNext,vConv + vm1);  
            if (res < 0)
            {
              pNext = pConv+vm1;
              vNext = vConv+vm1;
            }
          }
        }

        else // odd
        {
          DGtal::BigInteger res; 
          res = Determinant<DGtal::BigInteger>::get(vNext,vConv);  
          if ( vNext.det(vConv)  < 0 )
          {
            pNext = pConv;
            vNext = vConv;
          }
        }

        even = (even == false); 
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

    do {
      //store the current vertex
      *res++ = tmp; 
      //get the next vertex
      tmp = next(tmp); 
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
