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
      
      // Initialisation of the convergents.
      Point vConvM2 = Point(1,0); //(k-2)-th convergent
      Point vConvM1 = Point(0,1); //(k-1)-th convergent

      // pConv is the k-th convergent such that 
      // pConv = pConvM2 + qk * vConvM1.
      int qk = 0;

      Point pConv;
      Point vConv;

      // determinant result
      DGtal::BigInteger res; 

      // Orientation of the convergent
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

      // The best "next vertex" candidate
      // with the smallest angle
      Point pNext = pConvM1; 
      Point vNext = pNext - aPoint;
      

      // p0 exisence an iteration
      if (myShape.dray(pConvM2, vConvM1, qk, pConv) == false)
      {
        myShape.dray(pConvM1, vConvM1, qk, pConv);    
        return (pConv);
      }

      // p0 lie on the circle
      if (myShape(pConv) >= 0)
      {
        pNext = pConv;
        vNext = pNext - aPoint;
      }
      //p0 outise the circle
      else if (myShape(pConv + vConvM1) >= 0)
      {
        pNext = pConv + vConvM1;
        vNext = pNext - aPoint;
      }

       // We iterate the vectors to build the next convergent
       // p_-1 become p_-2
       // p_0 become p_-1
       // and their vector vConvM2, vConvM1

      vConv = pConv - aPoint;
      pConvM2 = pConvM1;
      pConvM1 = pConv;

      vConvM2 = vConvM1;
      vConvM1 = pConvM1 - aPoint;

      //even or odd
      bool even = false;

      // The ray shooting is efficient.
      while (myShape.dray(pConvM2, vConvM1, qk, pConv) && qk != 0)
      {
        // New Convergent, 
        vConv = pConv - aPoint;
        
        if (even) // even
        {
          // if the vertex lie on the circle
          if (myShape(pConv) == 0) 
          {
            // We keep the smallest angle
            res = Determinant<DGtal::BigInteger>::get(vNext,vConv);  
            if (res < 0)
            {
              pNext = pConv;
              vNext = vConv;
            }
          }
          else if (myShape(pConv + vConvM1) >= 0)
          {
            // We keep the smallest angle
            res = Determinant<DGtal::BigInteger>::get(vNext,vConv + vConvM1);  
            if (res < 0)
            {
              pNext = pConv+vConvM1;
              vNext = vConv+vConvM1;
            }
          }
        } // end even
        else // odd
        {
          // We keep the smallest angle
          res = Determinant<DGtal::BigInteger>::get(vNext,vConv);  
          if ( res  < 0 )
          {
            pNext = pConv;
            vNext = vConv;
          }
        } // end odd

        // even, odd, even, ...
        even = (even == false); 
        
        // update pConvM2 and pConvM1
        pConvM2 = pConvM1;
        pConvM1 = pConv;

        vConvM2 = vConvM1;
        vConvM1 = pConv - aPoint;

      }
      
      // translation in vNext direction, 
      // in order to not keep the vertices 
      // on the convex hull edges
      myShape.dray(pNext, vNext, qk, pConv);
      return(pConv);

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
