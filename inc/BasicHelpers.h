#ifndef BasicHelpers_h
  #define BasicHelpers_h

/**
 * Class implementing the computation
 * of the determinant between two vectors. 
 *
 * Basic usage: 
 * @code
 DGtal::BigInteger res = Determinant<DGtal::BigInteger>::get(u,v);  
 * @endcode
 * 
 * @tparam T a model of number, the type of 
 * the vector components.  
 *
 */
template <typename T = long long int>
struct Determinant
{
  /**
   * Given two vectors, return their orientation. 
   *
   * @param u first vector
   * @param v second vector
   * @return the determinant value
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

    return (u0*v1 - u1*v0); 
  }
};

/**
 * Class implementing a functor
 * that is able to apply a transformation 
 * to a 2d point (eg. a rotation). 
 *
 * Basic usage: 
 * @code
 Transformer2D<Point> t; 
 Point resultingPoint = t(aPoint);  
 * @endcode
 * 
 * @tparam Point a model of point.  
 *
 */
template <typename Point>
struct Transformer2D
{
public: 

  /**
   * Constructor 
   * (rotation of pi/2 by default)
   * @param aP1 a first point
   * @param aP2 a second point
   */
  Transformer2D(const Point& aP1 = Point(0,-1), 
		const Point& aP2 = Point(1,0) )
  : my1(aP1), my2(aP2) {} 
  
  /**
   * Function operator
   * @param aPoint any point
   * @return the image of @a aPoint by a transformation 
   * given by @a my1 and @a my2
   */
  Point operator()(const Point& aPoint) const 
  {
    return Point(my1[0]*aPoint[0] + my1[1]*aPoint[1], my2[0]*aPoint[0] + my2[1]*aPoint[1]); 
  }

private: 
  Point my1; /* coefficients for the computation of the x-coordinate */
  Point my2; /* coefficients for the computation of the y-coordinate */
}; 


#endif
