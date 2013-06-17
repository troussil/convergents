#ifndef InexactRayIntersectableCircle_h
#define InexactRayIntersectableCircle_h

#include <cmath>

#include <DGtal/kernel/NumberTraits.h>
#include <DGtal/base/BasicFunctors.h>

 
/**
 * Class implementing a circle that is 'ray intersectable', 
 * ie. the intersection between the circle and 
 * a ray emanating from a given point along a given direction
 * is computable.
 * 
 * @tparam Point a model of point/vector.  
 * @tparam Parameter a model of integer for the circle parameters
 *
 * This class is a model of point functor and of ray intersectable shape
 */
template <typename TPoint, typename TParameter = long long int>
class InInexactRayIntersectableCircle 
{
public: 
  /////////////////////// inner types /////////////////
  typedef TPoint Point; 
  typedef TPoint Vector; 
  typedef typename TPoint::Coordinate Coordinate; 
  typedef TParameter Integer; 
  typedef Integer Value; 

private: 
  /////////////////////// members /////////////////////
  /**
   * Circle parameters. The circle is defined
   * as the set {(x,y) | ax + by + c(x^2 + y^2) + d = 0}
   * The center is equal to ( -a/(2c) , -b/(2c) ) and the 
   * radius is equal to sqrt( (a^2 + b^2 - 4cd) / 4c^2 )
   */
  Integer myA, myB, myC, myD;  

public:
  ///////////////////// standard services /////////////
  /**
   * Default constructor
   */
  InexactRayIntersectableCircle(): myA(0), myB(0), myC(0), myD(0) {}

  /**
   * Standard constructor
   * @param a a-parameter
   * @param b b-parameter
   * @param c c-parameter
   * @param d d-parameter
   */
  InexactRayIntersectableCircle(const Integer& a, const Integer& b, const Integer& c, const Integer& d)
    : myA(a), myB(b), myC(c), myD(d) {}

  /**
   * Constructor from three points
   * @param aP first point
   * @param aQ second point
   * @param aR third point
   */
  InexactRayIntersectableCircle(const Point& aP, const Point& aQ, const Point& aR)
  {
    Integer px = aP[0]; 
    Integer qx = aQ[0]; 
    Integer rx = aR[0];
    Integer py = aP[1]; 
    Integer qy = aQ[1]; 
    Integer ry = aR[1];
    Integer pz = px*px + py*py; 
    Integer qz = qx*qx + qy*qy; 
    Integer rz = rx*rx + ry*ry; 
 
    myA = - py*(qz - rz) + qy*(pz - rz) - ry*(pz - qz); 
    myB = px*(qz - rz) - qx*(pz - rz) + rx*(pz - qz);
    myC = - px*(qy - ry) + qx*(py - ry) - rx*(py - qy); 
    myD = px*(qy*rz - ry*qz) - qx*(py*rz - ry*pz) + rx*(py*qz - qy*pz); 
  }

  /**
   * Copy constructor
   * @param p other object to copy
   */
  InexactRayIntersectableCircle(const InexactRayIntersectableCircle& p): myA(p.a()), myB(p.b()), myC(p.c()), myD(p.d()) {}

  /**
   * Assignement operator
   * @param p other object to copy
   * @return reference on *this
   */
  InexactRayIntersectableCircle& operator=(const InexactRayIntersectableCircle& p) 
  {
    if (p != *this)
      {
        myA = p.a(); 
        myB = p.b(); 
        myC = p.c(); 
        myD = p.d();
      }
    return *this; 
  }

  /**
   * Default destructor
   */
  ~InexactRayIntersectableCircle() {}

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

  /**
   * d-parameter accessor
   * @return d-parameter. 
   */
  Integer d() const { return myD; }

  /**
   * radius accessor
   * @return circle radius, ie. sqrt( (a^2 + b^2 - 4cd) / 4c^2 ). 
   */
  double getRadius() const 
  { 
    double den = DGtal::NumberTraits<Integer>::castToDouble(4*myC*myC);
    double v1 = DGtal::NumberTraits<Integer>::castToDouble(myA*myA) / den;
    double v2 = DGtal::NumberTraits<Integer>::castToDouble(myB*myB) / den; 
    double v3 = DGtal::NumberTraits<Integer>::castToDouble(myD)
      / DGtal::NumberTraits<Integer>::castToDouble(myC); 
    return std::sqrt(v1 + v2 - v3); 
  }

  /**
   * center x-coordinate accessor
   * @return x-coordinate of the circle center. 
   */
  double getCenterX() const 
  { 
    return DGtal::NumberTraits<Integer>::castToDouble(-myA) 
      / DGtal::NumberTraits<Integer>::castToDouble(2*myC);  
  }

  /**
   * center y-coordinate accessor
   * @return y-coordinate of the circle center. 
   */
  double getCenterY() const 
  { 
    return DGtal::NumberTraits<Integer>::castToDouble(-myB)
      / DGtal::NumberTraits<Integer>::castToDouble(2*myC);
  }

  /**
   * Returns the minimal y-coordinate of the digital points
   * lying inside the circle.
   * @return minimal y-coordinate. 
   */
  Coordinate getBottom() const 
  { 
    double y = getCenterY() - getRadius(); 
    return (Coordinate) std::ceil( y ); 
  }


public: 
  ///////////////////// main methods ///////////////////
  /**
   * Function operator
   * @param aPoint any point
   * @return 0 if @a aPoint is on the circle,
   * a value <0 if @a aPoint is outside
   * a value >0 if @a aPoint is inside
   */
  Value operator()(const Point& aPoint) const
  {
    Integer x = aPoint[0]; 
    Integer y = aPoint[1];
    Integer z = x*x + y*y;  
    return (myA*aPoint[0] + myB*aPoint[1] + myC*z + myD); 
  }

private:
  /**
   * Retrieves integer @a q such that 
   * the point @a aPoint + @a q * @a aDir 
   * and the points @a aPoint + (@a q+1) * @a aDir
   * lie in either side of the circle. 
   * @param aPoint source of the ray casting
   * @param aDir direction of the ray casting
   * @param qmax maximal integer bounding the 
   * dichotomic search
   * @return integer @a q
   */
  template <typename Predicate>
  Coordinate dichotomicSearch(const Point& aPoint, const Vector& aDir, 
			      const Coordinate& qmax, const Predicate& aPredicate) const
  {    

    ASSERT( aPredicate( (*this)(aPoint)) ); 
    ASSERT( (!aPredicate( (*this)(aPoint + qmax * aDir))) ); 

    // init search bounds
    Coordinate qStart = 0;
    Coordinate qStop  = qmax;
    Point pStart = aPoint; 
    Point pStop = aPoint + qmax * aDir; 

    Coordinate qMid;
    Point pMid, pMid1; 

    // while not yet located 
    bool isNotFound = true; 
    while( (qStop > qStart) && (isNotFound) ) 
      {
	// middle between qStart and qStop
	qMid = (qStart + qStop)/2;
	pMid = aPoint + qMid * aDir;
	pMid1 = aPoint + (qMid+1) * aDir;
 
	// inclusion tests
	if ( aPredicate( (*this)(pMid)) )
	  { 
	    if (!aPredicate( (*this)(pMid1)) )
	      { //found!
		isNotFound = false; 
		qStart = qMid; 
	      }
	    else //search in the upper range
	      qStart = qMid + 1;
	  }
	else //search in the lower range
	  qStop = qMid;
      }
    // return the integer 
    return(qStart);
  }

public: 
  /**
   * Intersection between the circle and the digital ray
   *
   * @param aStartingPoint point from which the ray is emanating
   * @param aDirection ray direction
   * @param returned maximal integer such that
   * @a aStartingPoint + @a aQuotient * @a aDirection lies close
   * (but on the same side) to the circle
   * @param returned point equal to @a aStartingPoint + @a aQuotient * @a aDirection
   *
   * @return 'true' if the ray and the circle instercest, 'false' otherwise 
   */
   bool dray(const Point& aStartingPoint, const Vector& aDirection,
             int& aQuotient, Point& aClosest) const
    {
      /*
* We look for the intersection between aS + aQ * aD and the circle
*
*/

     double aEq = myC*(aDirection[0]*aDirection[0] + aDirection[1]*aDirection[1]);
     double bEq = myA*aDirection[0] + myB*aDirection[1] +
       2*myC*aStartingPoint[0]*aDirection[0] +
       2*myC*aStartingPoint[1]*aDirection[1];

     double cEq = myA*aStartingPoint[0] + myB*aStartingPoint[1] +
       myC*aStartingPoint[0]*aStartingPoint[0] +
       myC*aStartingPoint[1]*aStartingPoint[1] + myD;


     double Delta = std::sqrt(bEq*bEq - 4*aEq*cEq);
// Integer Delta = bEq*bEq - 4*aEq*cEq;
//std::cout << "Delta : "<<Delta<< " ; aS, aD : "<<aStartingPoint<<aDirection<<std::endl;
//std::cout << "a : "<<aEq<<", b : "<<bEq<<", c : "<<cEq<<" | "<<myA<<myB<<myC<<std::endl;
     //Delta = 0 - 1 solution
     // aS+aQ*aD is a tangent to the circle
     if (Delta == 0 && -bEq/(2*aEq) >= 0)
     {
       aQuotient = floor(-bEq/(2*aEq));
       aClosest = aStartingPoint + aQuotient*aDirection;
       return true;
     }

     // Delta > 0 - 2 solutions
     else if (Delta > 0)
     {
       double x1 = (-bEq - Delta)/(2*aEq);
       double x2 = (-bEq + Delta)/(2*aEq);
//std::cout << "x1, x2 : "<<x1<<" , "<<x2<<std::endl;
       // aS lie on the circle
       if ( (*this)(aStartingPoint) == 0.0)
       {
         aQuotient = 0;
         aClosest = aStartingPoint;
         return true;
       }
       // aS is outside the circle
       else if ( (*this)(aStartingPoint) < 0.0)
       {
         if ( -bEq/aEq < 0 ){return false;}
         else
         {
           // We pick the smallest solution
          if ( bEq < 0 )
          {
          aQuotient = floor(x1);
          }
          else
          {
          aQuotient = floor(x2);
          }
        
          //aQuotient = floor(x1<x2 ? x1:x2);
           aClosest = aStartingPoint + aQuotient*aDirection;
           return true;
         }
       }
       // aS is inside the circle
       else
       {
         // We pick the positive solution
         aQuotient = floor(x1<x2 ? x2:x1);
         aClosest = aStartingPoint + aQuotient*aDirection;
         return true;
       }
     }
     // Delta < 0 & and case where there is no solution
     else {return false;}
    }

  /**
   * Returns a vertex of the convex hull of the digital points
   * lying inside the circle, such that it has a minimal y-coordinate, 
   * and among the digital points of minimal y-coordinate, it has
   * a maximal x-coordinate. 
   * @return vertex of the convex hull. 
   */
  Point getConvexHullVertex() const 
  { 
    //computation of one integer point, 
    //inside the circle and having min y-coordinate
    Point startingPoint; 
 
    Coordinate ymin = getBottom();
    Coordinate x = (Coordinate) DGtal::NumberTraits<Integer>::castToInt64_t(-myA/(2*myC)); 
    Point ptf(x, ymin);
    if ( this->operator()( ptf ) < 0 )
      { //if ptf is outside the circle
	Point ptc( (x+1), ymin ); 
	if ( this->operator()( ptc ) < 0 )
	  { //if ptc is outside the circle
	    Point pt( x, (ymin+1) );
	    startingPoint = pt; 
	  }
	else
	  startingPoint = ptc; 
      }
    else 
      startingPoint = ptf; 

    //ray casting
    Coordinate q = 0;  
    Point ptRes(0,0); 
    if ( dray(startingPoint, Vector(1,0), q, ptRes) )
      return ptRes; 
    else 
      {
	ASSERT( false && "Error in getConvexHullVertex of InexactRayIntersectableCircle" ); 
	return Point(0,0); 
      } 
  }

}; 
#endif

