#ifndef CircumcircleRadiusPredicate_h
#define CircumcircleRadiusPredicate_h
/**
 * @brief Class implementing a point predicate,
 * which compares a stored radius with the radius 
 * of the circle passing through three given points. 
 * It returns true if stored radius is greater than
 * the computed one. 
 *
 * This class is a model of ternary predicate.
 *
 * @tparam TInteger any integer type for the 
 * numerator and denominator of the squared radius
 */
template<typename TInteger = long long int>
struct CircumcircleRadiusPredicate
{
public: 
  /* type of parameters */
  typedef TInteger Integer;
 
private: 
  /////////////////////// members /////////////////////
  /**
   * The radius R is viewed as the fraction of two squares, 
   * R = myNum2 / myDen2
   */
  Integer myNum2; 
  Integer myDen2;
  bool positive; 


public:
  ///////////////////// standard services /////////////
  /**
   * Standard constructor
   * @param aNum2 squared numerator of the radius (default 1)
   * @param aDen2 squared denominator of the radius (default 0)
   * @param aPositive sign of alpha (default true)
   *
   * NB: Default values stands for an infinite radius. 
   */
  CircumcircleRadiusPredicate(const Integer& aNum2 = 1, const Integer& aDen2 = 0, const bool& aPositive = true)
    : myNum2(aNum2), myDen2(aDen2), positive(aPositive){}

  /**
   * Copy constructor
   * @param other other object to copy
   */
  CircumcircleRadiusPredicate(const CircumcircleRadiusPredicate& other)
    : myNum2(other.myNum2), myDen2(other.myDen2), positive(other.positive) {}

private:

  /**
   * Assignement operator
   * @param other other object to copy
   * @return reference on *this
   */
  CircumcircleRadiusPredicate& operator=(const CircumcircleRadiusPredicate& other) 
  { return *this; }

public: 
  /**
   * Radius² denominateur
   * @return myDen2
   */
  Integer getDen2() const { return (myDen2);  }

  /**
   * Radius² Numerator
   * @return myNum2. 
   */
  Integer getNum2() const { return (myNum2);  }
  
  /**
   * Alpha sign
   * @return myNum2. 
   */
  bool getSign() const { return (positive);  }

  /**
   * Default destructor
   */
  ~CircumcircleRadiusPredicate() {}


  ///////////////////// main methods ///////////////////
  /**
   * Given three points, returns the area of the parallegram. 
   *
   * @param a first point
   * @param b second point
   * @param c third point
   * @return signed area of the parallelogram
   *
   * @tparam Point a model of point.   
   */
  template<typename Point>
  Integer 
  getArea(const Point& a, const Point& b, const Point& c) const
  {
    Integer a0 = a[0]; Integer a1 = a[1];
    Integer b0 = b[0]; Integer b1 = b[1];
    Integer c0 = c[0]; Integer c1 = c[1];
    
    return a0*(b1 - c1) - b0*(a1 - c1) + c0*(a1 - b1);
  }

  /**
   * Given three points, returns the predicate value. 
   *
   * @param a first point
   * @param b second point
   * @param c third point
   * @return 'true' if the radius of the circle abc is greater 
   * that the stored radius equal to @a myNum2 / @a myDen2
   *
   * @tparam Point a model of point.   
   */
  template<typename Point>
  bool
  operator()(const Point& a, const Point& b, const Point& c) const
  {
    
    
    Integer area = getArea(a,b,c);
    
    if (area == 0)
    {
      return( (myDen2 == 0 || positive)?true:false);
    }
    
    if (( positive && area > 0) || (!positive && area < 0) )
    {
      return ( (true && positive) );
    }
    
    Point ab = (b-a); 
    Point bc = (c-b); 
    Point ac = (c-a);
    
    //myX * myX + myY * myY
   
    Integer ab0 = ab[0]; 	Integer ab1 = ab[1];
    Integer bc0 = bc[0]; 	Integer bc1 = bc[1];
    Integer ac0 = ac[0]; 	Integer ac1 = ac[1];
    
    Integer rightPart = (ab0*ab0 + ab1*ab1)*(bc0*bc0 + bc1*bc1)*
      (ac0*ac0 + ac1*ac1)*myDen2; 

    Integer leftPart = 4*area*area*myNum2;

    return (positive == (leftPart < rightPart));
    //return ( positive?(leftPart <= rightPart):(leftPart >= rightPart) );

  }
}; 

#endif
