#include <iostream>
//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>

#include "PointVector2D.h"
#include "RayIntersectableCircle.h"
#include "RayIntersectableStraightLine.h"
#include "ConvexHullHelpers.h"
#include "OutputSensitiveConvexHull.h"
//#include "OutputSensitiveAlphaShape.h"


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

    if ( evenConv == true && (aPredicate(pStart, pConv-vConvM1, pConv) == false ||
          (vConv.normL22()*aPredicate.getDen2() > aPredicate.getNum2()) ))
    {
      /**
       * We test if the convergent is inside the circle with the boolean evenConv
       * We test if the circumcircle of the last triangle T(pStart, pConv-vConvM1, pConv)
       * is lower than the radius / predicate or if the normL22 of vConv is greater than
       * the radius.
       */ 

      // Throw the dichotomous method to find qkalpha
      qkalpha = dichotomous(aPredicate, pStart, vConvM2, vConvM1, qk);

      // We add all the vertex to the alpha-shape from qkalpha to qk

      while (qkalpha <= qk)
      {
        *aAlphaShapeHull++ = pConvM2 + qkalpha * vConvM1;
        qkalpha++;
      }

      // We reset the convergent from a new start.
      pStart = pConv;  

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


#include "OutputSensitiveAlphaShape.h"

//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename OutputIterator, 
  typename Predicate>
void alphaShape(const Shape& aShape, const Point& aStartingPoint, 
    OutputIterator res, const Predicate& aPredicate)
{
  OutputSensitiveAlphaShape<Shape,Predicate> ch(aShape, aPredicate); 
  //get the first vertex
  Point tmp = aStartingPoint; 
  do {
    //store the current vertex
    *res++ = tmp; 
    //get the next vertex
    tmp = ch.next(tmp); 
    //while it is not the first one
  } while (tmp != aStartingPoint); 
}

///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point> Circle;
  //typedef OutputSensitiveAlphaShape<Circle> Shape; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  std::cout << std::endl; 
  std::cout << "I) Alpha-shape on a straight line between [0,0] and [5,8]" << std::endl; 
  {

    std::vector<Vector> convergents;
    convergents.clear(); 

    // Straight line between a = (0,0) ad b = (5,8) 
    Point aPoint(0,0); 
    Point bPoint(5,8);  

    // Infinite radius
    CircumcircleRadiusPredicate<> predicate1;
    convergents.clear(); 

    std::vector<Point> groundTruth; 

    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate1, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - Infinite radius = Convexe hull" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;  


    // Very large radius
    CircumcircleRadiusPredicate<> predicate2(100000,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate2, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - Very large radius, R² = 100'000" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // Add the next vertices
    CircumcircleRadiusPredicate<> predicate3(39337,4);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate3, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- 1446.25 <= R² < 9834.5 : (39338,4)  -- R² = 9834.25 " << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate4(23139,16);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate4, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- 14 <= R² < 1446.25 : (23140/16)  -- R² = 1446.18 " << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate5(12,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(3,4));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate5, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- 2 <= R² < 13  -- R² = 12" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;   
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate6(6,10);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(3,4));
    groundTruth.push_back(Point(4,5));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,7));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate6, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- 0.5 <= R² < 2 -- R² =  0.9" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;  
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate7(4,10);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(3,3));
    groundTruth.push_back(Point(3,4));
    groundTruth.push_back(Point(4,4));
    groundTruth.push_back(Point(4,5));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,6));
    groundTruth.push_back(Point(5,7));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate7, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² < 0.5 -- R² =  0.4" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;    
    std::cout << std::endl;    
  }

  std::cout << std::endl; 
  std::cout << "I) Alpha-shape on a straight line between [1,1] and [9,6]" << std::endl; 
  {

    std::vector<Vector> convergents;
    convergents.clear(); 

    // Straight line between a = (0,0) ad b = (8,5) 
    Point aPoint(0,0); 
    Point bPoint(8,5);  

    // Infinite radius
    CircumcircleRadiusPredicate<> predicate1;
    convergents.clear(); 

    std::vector<Point> groundTruth; 

    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(8,5)); 

    convAlphaShape(predicate1, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - Infinite radius = Convexe hull" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;  

    // Add the next vertices
    CircumcircleRadiusPredicate<> predicate3(39337,4);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(5,3)); 
    groundTruth.push_back(Point(8,5)); 

    convAlphaShape(predicate3, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² = 9834.25 " << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate5(86,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(2,1)); 
    groundTruth.push_back(Point(5,3)); 
    groundTruth.push_back(Point(8,5)); 

    convAlphaShape(predicate5, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² = 86" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;   
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate6(26,10);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(2,1)); 
    groundTruth.push_back(Point(4,2));
    groundTruth.push_back(Point(5,3)); 
    groundTruth.push_back(Point(7,4));
    groundTruth.push_back(Point(8,5)); 

    convAlphaShape(predicate6, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- 2 <= R² < 5  -- R² = 2.9" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;   
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate7(19,10);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(1,0));
    groundTruth.push_back(Point(2,1)); 
    groundTruth.push_back(Point(3,1)); 
    groundTruth.push_back(Point(4,2));
    groundTruth.push_back(Point(5,3)); 
    groundTruth.push_back(Point(6,3)); 
    groundTruth.push_back(Point(7,4));
    groundTruth.push_back(Point(8,5)); 

    convAlphaShape(predicate7, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- 0.5 <= R² < 2 -- R² =  1.9" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;  
    std::cout << std::endl;

    CircumcircleRadiusPredicate<> predicate8(4,10);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(1,0));
    groundTruth.push_back(Point(2,0)); 
    groundTruth.push_back(Point(2,1)); 
    groundTruth.push_back(Point(3,1)); 
    groundTruth.push_back(Point(4,1)); 
    groundTruth.push_back(Point(4,2));
    groundTruth.push_back(Point(5,2)); 
    groundTruth.push_back(Point(5,3)); 
    groundTruth.push_back(Point(6,3)); 
    groundTruth.push_back(Point(7,3)); 
    groundTruth.push_back(Point(7,4));
    groundTruth.push_back(Point(8,4)); 
    groundTruth.push_back(Point(8,5)); 

    convAlphaShape(predicate8, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² < 0.5 -- R² =  0.4" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;    
    std::cout << std::endl;    
  }   
}
