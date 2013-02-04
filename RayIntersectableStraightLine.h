/**
 * Class implementing a straight line that is 'ray intersectable', 
 * ie. the intersection between the straight line and 
 * a ray emanating from a given point along a given direction
 * is computable.
 * 
 * @tparam T a model of point/vector.  
 *
 * This class is a model of point functor and of ray intersectable shape
 */
template <typename T>
class RayIntersectableStraightLine 
{
  public: 
    /////////////////////// inner types /////////////////
    typedef T Point; 
    typedef T Vector; 
    typedef typename T::Coordinate Integer; 
    typedef Integer Value; 

  private: 
    /////////////////////// members /////////////////////
    /**
     * Straight-line parameters. The straight-line is defined
     * as the set {(x,y) | ax + by + c = 0}
     */
    Integer myA, myB, myC;  

  public:
    ///////////////////// standard services /////////////
    /**
     * Default constructor
     */
    RayIntersectableStraightLine(): myA(0), myB(0), myC(0) {}

    /**
     * Standard constructor
     * @param a a-parameter
     * @param b b-parameter
     * @param c c-parameter
     */
    RayIntersectableStraightLine(const Integer& a, const Integer& b, const Integer& c): myA(a), myB(b), myC(c) {}

    /**
     * Constructor from two points
     * @param aP first point
     * @param aQ second point
     */
    RayIntersectableStraightLine(const Point& aP, const Point& aQ):
      myA(aP[1]-aQ[1]), myB(aQ[0]-aP[0]), myC(myA*aP[0]+myB*aP[1]) {}

    /**
     * Copy constructor
     * @param p other object to copy
     */
    RayIntersectableStraightLine(const RayIntersectableStraightLine& p): myA(p.a()), myB(p.b()), myC(p.c()) {}

    /**
     * Assignement operator
     * @param p other object to copy
     * @return reference on *this
     */
    RayIntersectableStraightLine& operator=(const RayIntersectableStraightLine& p) 
    {
      if (p != *this)
      {
        myA = p.a(); 
        myB = p.b(); 
        myC = p.c(); 
      }
      return *this; 
    }

    /**
     * Default destructor
     */
    ~RayIntersectableStraightLine() {}

    ///////////////////// read access ///////////////////
    /**
     * a-parameter accessor
     * @return a-parameter. 
     */
    Integer a() const { return myA; }

    /**
     * b-parameter accessor
     * @return b-parameter. 
     */
    Integer b() const { return myB; }

    /**
     * c-parameter accessor
     * @return c-parameter. 
     */
    Integer c() const { return myC; }

    ///////////////////// main methods ///////////////////
    /**
     * Function operator
     * @param aPoint any point
     * @return 0 if @a aPoint is on the straight-line,
     * a value <0 if @a aPoint is below
     * a value >0 if @a aPoint is above 
     */
    Value operator()(const Point& aPoint)
    {
      return (myA*aPoint[0] + myB*aPoint[1] + myC);
    }

    /**
     * Intersection between the straight-line and the digital ray
     *
     * @param aStartingPoint point from which the ray is emanating
     * @param aDirection ray direction
     * @param returned maximal integer such that
     * @a aStartingPoint + @a aQuotient * @a aDirection lies close
     * (but on the same side) to the straight line
     * @param returned point equal to @a aStartingPoint + @a aQuotient * @a aDirection
     *
     * @return 'true' if the ray and the straight-line instercest, 'false' otherwise 
     */
    bool dray(const Point& aStartingPoint, const Vector& aDirection, 
             int& aQuotient, Point& aClosest) const 
    {
      
      // Initialise value
      aQuotient = 0;
      aClosest = aStartingPoint;
      

      /**
       *  There is no intersection if aDirection and the straight-line 
       *  are parallel.
       *
       */
      if (myA*aDirection[0] + myB*aDirection[1] == 0 ){return false;}
      else 
      { 
        aQuotient = -(myA*aStartingPoint[0] + myB*aStartingPoint[1] + myC)/
          (myA*aDirection[0] + myB*aDirection[1]);

        if(aQuotient < 0){return false;}
        else
        {
          aClosest = aStartingPoint + aQuotient * aDirection; 
          return true;
        }
      }
    }
}; 
