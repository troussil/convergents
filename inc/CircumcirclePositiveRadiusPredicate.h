#ifndef CircumcircleRadiusPositivePredicate_h
#define CircumcircleRadiusPositivePredicate_h
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
struct CircumcircleRadiusPositivePredicate
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


public:
  ///////////////////// standard services /////////////
  /**
   * Standard constructor
   * @param aNum2 squared numerator of the radius (default 1)
   * @param aDen2 squared denominator of the radius (default 0)
   *
   * NB: Default values stands for an infinite radius. 
   */
  CircumcircleRadiusPositivePredicate(const Integer& aNum2 = 1, const Integer& aDen2 = 0)
    : myNum2(aNum2), myDen2(aDen2) {}

  /**
   * Copy constructor
   * @param other other object to copy
   */
  CircumcircleRadiusPositivePredicate(const CircumcircleRadiusPositivePredicate& other)
    : myNum2(other.myNum2), myDen2(other.myDen2) {}

private:

  /**
   * Assignement operator
   * @param other other object to copy
   * @return reference on *this
   */
  CircumcircleRadiusPositivePredicate& operator=(const CircumcircleRadiusPositivePredicate& other) 
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
   * Default destructor
   */
  ~CircumcircleRadiusPositivePredicate() {}


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
    return a[0]*(b[1] - c[1]) - b[0]*(a[1] - c[1]) + c[0]*(a[1] - b[1]);
  }

  /**
   * Given three points, returns the predicate value. 
   *
   * @param a first point
   * @param b second point
   * @param c third point
   * @return 'true' if the radius of the circle abc is smaller 
   * that the stored radius equal to @a myNum2 / @a myDen2
   *
   * @tparam Point a model of point.   
   */
  template<typename Point>
  bool
  operator()(const Point& a, const Point& b, const Point& c) const
  {
    
    Integer area = getArea(a,b,c);
    if (area >= 0)
      {return true;}

    Point ab = (b-a); 
    Point bc = (c-b); 
    Point ac = (c-a);
    Integer nab = ab.normL22(); 	
    Integer nbc = bc.normL22(); 
    Integer nac = ac.normL22(); 
    Integer rightPart = nab*nbc*nac*myDen2;  
    Integer leftPart = (4*area*area*myNum2);

//std::cout << a<<b<<c<< "| result : "<<(leftPart <= rightPart) << " - aire : "<<area<<std::endl; 

    return leftPart <= rightPart;
  }
}; 

#endif