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
  nextLeftShape(predicate1, O, P, 50, std::back_inserter(ch2) );  
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
 * Given a straight line, find its right alpha Hull,
 * @param aPredicate determine the alpha shape radius
 * @param aPoint, bPoint, the starting and ending point of the straight
 * line.
 * @return the alpha-hull of the straight line in the OutputIterator.
 */
 template <typename CircumcircleRadiusPredicate, typename Point, typename OutputIterator>
Point nextLeftShape(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa, const Point& aPointb, const int aMaxConv, OutputIterator aAlphaShapeHull)
{
  // aPoint is the first Alpha-Shape vertex.
  *aAlphaShapeHull++ = aPointa;

  // Initialisation of the convergent.
  // Convergent arise from pStart.
  Point pStart = aPointa;
  
  // First convergents vectors : 
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
    vConvM2[0]=1; vConvM2[1]=0; 
    vConvM1[0]=0; vConvM1[1]=1; 
      
    pConvM2 = pStart + vConvM2;
    pConvM1 = pStart + vConvM1;  
    
    k = 0;
    nextVertex = false;
   
   /**
    * We start searching for next vertices and convergents.
    * We look after the intersection of the ray and the straight-line.
    * At every new vertices add, we reset the computation from a new start.
    */     
   while ( lineRatio.dray(pConvM2, vConvM1, qk, pConv) || nextVertex == false )
   {
      // pConv is calculate in lineRatio.dray(), so We update vConv
      vConv = pConv - pStart;

      if ( k % 2 != 0 )
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
        } // if new vertex. 
      } // if k is odd.
      
      // Update Convergent
      k++;
     
      pConvM2 = pConvM1;
      pConvM1 = pConv;
      vConvM2 = vConvM1;
      vConvM1 = pConv-pStart;
    } // No new convergent or new vertex
  }// found all the vertices
}



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
