#include <iostream>
//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>

#include "PointVector2D.h"
#include "RayIntersectableStraightLine.h"
#include "OutputSensitiveConvexHull.h"
#include "ConvexHullHelpers.h"

#include "OutputSensitiveAlphaShape.h"


///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure that generates the (k+n)-th convergent, 
 * given the u_k, u_{k+1}, ... u_{k+n} quotients of the 
 * range [ @e itb , @e ite )  
 * 
 * @param itb quotient begin iterator
 * @param ite quotient end iterator 
 * @param aConv2 (k-2)-th convergent
 * @param aConv1 (k-1)-th convergent 
 * 
 * @return (k+n)-th convergent
 * (NB: default point if the range is empty)
 * 
 * @tparam ForwardIterator a model of forward iterator
 * @tparam Point a model of point
 */
template<typename ForwardIterator, typename Point>
Point generatePoint(const ForwardIterator& itb, const ForwardIterator& ite,
		    Point aConv2, Point aConv1)
{
  Point aConv; //k-th convergent
  for (ForwardIterator it = itb; it != ite; ++it)
    {
      aConv = (*it)*aConv1 + aConv2;
      aConv2 = aConv1; 
      aConv1 = aConv; 
    }
  return aConv; 
}

///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure that checks whether the 
 * output-sensitive algorithm returns the same
 * alpha-shape as the tracking-based algorithm
 * for a straight-line, which is described by
 * a point @e O and the continued fractions
 * expansion of its slope. 
 * 
 * @param itb quotient begin iterator
 * @param ite quotient end iterator 
 * @param O any digital point 
 * 
 * @return 'true' if the test passed, 'false' otherwise
 * 
 * @tparam ForwardIterator a model of forward iterator
 * @tparam Point a model of point
 */
template<typename ForwardIterator, typename Point>
bool test(const ForwardIterator& itb, const ForwardIterator& ite,
	  const Point& O)
{

  typedef RayIntersectableStraightLine<Point> StraightLine; 
  typedef Point Vector; 

  std::cout << "Continued fraction expansion" << std::endl; 
  std::copy(itb, ite, std::ostream_iterator<int>(std::cout, ", ") ); 
  std::cout << std::endl; 

  Point P = generatePoint(itb, ite, Point(0,1), Point(1,0));
  std::cout << "Direction vector : " << P << std::endl; 
  std::cout << std::endl; 
 
  StraightLine sl( O, P );

  std::vector<Point> boundary; 
  Vector dir(1,0); 
  openTracking( sl, O, P, dir, std::back_inserter(boundary) );
  // std::cout << "Boundary" << std::endl; 
  // std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  // std::cout << std::endl; 

  //tracking-based algorithm
  std::vector<Point> ch;
  CircumcircleRadiusPredicate<> predicate1(1,1); //CHOOSE A RADIUS (1 here)  
  openGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate1 ); 
  std::cout << "1-shape of the boundary --openGrahamScan" << std::endl; 
  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 

  //output-sensitive algorithm
  std::vector<Point> ch2; 
  //TODO
  convAlphaShape(predicate1, O, P, std::back_inserter(ch2) );  
  std::cout << "1-shape of the boundary --convMethod" << std::endl; 
  std::copy(ch2.begin(), ch2.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 
  
  //COMPARE WITH YOUR ALGO HERE
	if (ch.size() == ch2.size())
  {
		if ( std::equal(ch2.begin(), ch2.end(), ch.begin()) )
		{
				return true; 
		}
	}
	else
	{
		return false;
	}

}

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
  while( qkstop - qkstart >= 0 ) 
  {
    // Find the middle between qkstart and qkstop
    mid = (qkstart + qkstop)/2;

    // lower triangle predicate
    if (aPredicate(aPointa, (aPointa + aConvM2 + (mid-1)*aConvM1), 
          (aPointa + aConvM2 + mid*aConvM1)) == true)
    { // the vertex is higher
      qkstart = mid + 1;
    }
    else
    { // the vertex is lower
      qkstop = mid - 1;
    }
  } 
  // return the coefficient of the first vertex inside the alpha-shape.
  return((qkstart + qkstop)/2);
}


/**
 * Given a straight line, find its alpha Hull,
 * @param aPredicate determine the alpha shape radius
 * @param aPoint, bPoint, the starting and ending point of the straight
 * line.
 * @return the alpha-hull of the straight line in the OutputIterator.
 */
  template <typename CircumcircleRadiusPredicate, typename Point, typename OutputIterator>
void convAlphaShape(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa,
    const Point aPointb, OutputIterator aAlphaShapeHull)
{
  // aPoint is the first Alpha-Shape vertex
  *aAlphaShapeHull++ = aPointa;

  // Initialisation of the convergent
  // Convergent arise from pStart
  Point pStart = aPointa;

  // First convergents vectors : 
  Point vConvM2(1,0);
  Point vConvM1(0,1);
  Point vConv;

  // First convergents points :
  Point pConvM2 = pStart + vConvM2;
  Point pConvM1 = pStart + vConvM1;

  // pConv is the next convergent pconv = pConvM2 + qk * vConvM1
  Point pConv = pStart;
  int qk;

	// qkode remember the last coefficient, it's qk-1
  int qkode;

  // pConvM2 + qkalpha * vConvM1 is the first vertex of the alpha shape
  int qkalpha;
	
  // The convergent number is even : the convergent is below the straight line
  bool evenConv = true;

  // The discrete straight-line [a, b]
  RayIntersectableStraightLine<Point> lineRatio(aPointa, aPointb);

  /**
   * we do not have compute all the candidat.
   * We stop the computing when we reach the end of the straight-line : aPointb
   * or if the next convergent do not intersect the straight-line
   */
  while ( pStart != aPointb && lineRatio.dray(pConvM2, vConvM1, qk, pConv)) 
  {
    // New pConv is calculate in lineRatio.dray(), so We update vConv
    vConv = pConv - pStart;

    if ( evenConv == true && (aPredicate(pStart, pConv-vConvM1, pConv) == false ))
    {
      /**
       * We test if the convergent is inside the circle with the boolean evenConv
       * We test if the circumcircle of the last triangle T(pStart, pConv-vConvM1, pConv)
       * is lower than the radius / predicate or if the normL22 of vConv is greater than
       * the radius.
       */ 

			if ( vConvM1.normL22() < vConvM1.normL22() )
			{ 
			 /**
        * We test if vConM2 is longer than vConvM1.
        * In that case, we add pConvM2 to the alpha-shape and continue the algo
        * from pConvM2.
        * In the other cas, we throw the dichotmomous method in order to find
        * the first point in the alpha-shape.
        */
        
				*aAlphaShapeHull++ = pConvM2;		
				pStart = pConvM2;
			}
			else
			{
		    // Throw the dichotomous method to find qkalpha
		    qkalpha = dichotomous(aPredicate, pStart, vConvM2, vConvM1, qk);

		    // We add all the vertex to the alpha-shape from qkalpha to qk
		    while (qkalpha <= qk)
		    {
		      *aAlphaShapeHull++ = pConvM2 + qkalpha*vConvM1;
		      qkalpha++;
		    }

				pStart = pConv;
			}

      // Frist convergents
      vConvM2[0]=1; vConvM2[1]=0; 
      vConvM1[0]=0; vConvM1[1]=1; 

      pConvM2 = pStart + vConvM2;
      pConvM1 = pStart + vConvM1;       

      // Ready for comput p_0
      evenConv = true;
    } // end new alpha-shape vertex calculation
    else 
    { // We search for the next convergent

      // Updating convergent
      pConvM2 = pConvM1;
      pConvM1 = pConv;
      vConvM2 = vConvM1;
      vConvM1 = pConv-pStart;

      // even, odd, even...
      evenConv = (evenConv == false);

      // Stock qk. Need for last step.
      qkode = qk;
    } // New convergent have been found
  } // end while :: all vertex have been found

  if (pConvM1 == aPointb)
  {
    /**
     * if the last convergent before aPointb is above the straight line, ie : if
     * pConvM1 have reach aPointb, we might have missed some point. 
     * At least, aPointb.
     */ 
std::cout<<std::endl<<"#3 - Finish -"<<std::endl; std::cout<<"pS : "<<pStart<<"| pm2 : "<<pConvM2<<vConvM2<<"| pm1 : "<<pConvM1<<vConvM1<<std::endl;
    if (evenConv == true)
    {
      int i = 0;
      while( i < qkode )
      {
        if (aPredicate(pStart, pConvM2 + i*pConvM2, aPointb) == false)
        {
          /**
           * pConvM2 + i*pConvM2 and pConvM2 + (i+1)*pConvM2 are collineare.
           * We test if the circumcircle of the last triangle 
           * T(pStart, pConvM2 + i*pConvM2, aPointb) is lower than the 
           * radius / predicate
           */

          *aAlphaShapeHull++ = pConvM2 + i*pConvM2;
        }
        i++;
      } 
    }// evenCon == true
    // aPoint b is the last vertex
    *aAlphaShapeHull++ = aPointb;
  } //pConvM1 == aPointb
} // end proc


///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableStraightLine<Point> StraightLine; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  ////////////////////////////////////////////////////////////////////////
  std::cout << std::endl; 
  std::cout << "I) Alpha-shape on a simple straight line" << std::endl; 
  {

    StraightLine sl( Point(0,0), Point(8,5) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    openTracking( sl, Point(0,0), Point(8,5), dir, std::back_inserter(boundary) );
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
    std::vector<Point> groundTruth; 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(8,5)); 
    std::cout << "Expected" << std::endl; 
    std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> ch;
    CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
    openGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
    std::cout << "Graham's convex hull of the boundary" << std::endl; 
    std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (ch.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    std::cout << "  B) radius == 1" << std::endl; 

    std::cout << "Expected (boundary)" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> ch1;
    CircumcircleRadiusPredicate<> predicate1(1,1); //radius 1  
    openGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch1), predicate1 ); 
    std::cout << "1-shape of the boundary" << std::endl; 
    std::copy(ch1.begin(), ch1.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (ch1.size() == boundary.size())
      if ( std::equal(boundary.begin(), boundary.end(), ch1.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
  }

  ////////////////////////////////////////////////////////////////////////
  std::cout << std::endl; 
  std::cout << "II) Alpha-shape on other straight lines" << std::endl; 
  {
    Point O(0,0); 

    {
      std::vector<int> quotients; 
      quotients.push_back(1); 
      quotients.push_back(1); 
      quotients.push_back(1); 
      quotients.push_back(1); 
      quotients.push_back(1); 

      if (test(quotients.begin(), quotients.end(), O))
	nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    {
      std::vector<int> quotients; 
      quotients.push_back(2); 
      quotients.push_back(2); 
      quotients.push_back(2); 
      quotients.push_back(2); 
      quotients.push_back(2); 

      if (test(quotients.begin(), quotients.end(), O))
	nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    //OTHER TESTS TO DO

  }

  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
