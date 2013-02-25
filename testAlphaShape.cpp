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
 * @param Points aConvM2 and aConvM1 are the next convergents
 * @param aqk pConv = aqk*aConvM1 + aConvM2
 * @return first convergent in the alpha shape
 */
  template <typename CircumcircleRadiusPredicate, typename Point>
int dichotomous(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa,
    const Point aConvM2, const Point aConvM1, const int aqk)
{
  // Convergent vector
  Point vConvM1 = aConvM1 - aPointa;

  // init search milestone
  int qkstart = 0;
  int qkstop  = aqk;
  // middle
  int mid;

  // else 
  do 
  {
    mid = (qkstart + qkstop)/2;
    // lower triangle predicate
    if (aPredicate(aPointa, (aConvM2 + (mid-1)*vConvM1), (aConvM2 + mid*vConvM1)) == true)
    { 
      // the vertex is higher
      qkstart = mid + 1;
    }
    else
    { // the vertex is lower
      qkstop = mid - 1;
    }
  } while( qkstop - qkstart >= 0 ); 
  // return a new point
  return(mid);
}


/**
 * Given a straight line, find the alpha Hull,
 * @param aPredicate determine the alpha shape radius
 * @param aPoint, bPoint, the starting and ending point of the straight
 * line.
 * @return the alpha-hull of the straight line.
 */
  template <typename CircumcircleRadiusPredicate, typename Point, typename OutputIterator>
void convAlphaShape(const CircumcircleRadiusPredicate& aPredicate, const Point& aPointa,
    const Point aPointb, OutputIterator aAlphaShapeHull)
{
  // aPoint is the first Alpha-Shape vertex
  *aAlphaShapeHull++ = aPointa;

  // First calculating convergent come from pStart
  Point pStart = aPointa;

  // First convergents vectors : 
  Point vConvM2(1,0);
  Point vConvM1(0,1);

  // First convergents :
  Point pConvM2 = pStart + vConvM2;
  Point pConvM1 = pStart + vConvM1;

  // pConv is the next convergent pconv = pConvM2 + qk * vConvM1
  Point pConv = pStart;
  int qk;
  
  // pConvM2 + qkalpha * vConvM1 is the first vertex of the alpha shape
  int qkalpha;
  
  // qkode remember the last coefficient
  int qkode;
  
  // The convergent number is even : the convergent is below the straight line
  bool evenConv = true;

  // The discrete straight-line [a, b]
  RayIntersectableStraightLine<Point> lineRatio(aPointa, aPointb);

  // we do not have compute all the candidat.
  while ( pStart != aPointb && lineRatio.dray(pConvM2, vConvM1, qk, pConv)) 
  {
    // We test if the convergent is inside the circle
    // ie : if the circumcircle radius of (pStart, pconv, pConvM2) triangle
    // is lower than -1*/alpha.
    // and if the convergent is below the straight line (even)
    
    if ( evenConv == true && aPredicate(pStart, pConv-vConvM1, pConv) == false)
    {

      qkalpha = dichotomous(aPredicate, pStart, pConvM2, pConvM1, qk);
      // We add the next alpha-shape vertex
      
      while (qkalpha <= qk)
      {
        *aAlphaShapeHull++ = pConvM2 + qkalpha * vConvM1;
        qkalpha++;
      }

      // we restart convergent calculation from pStart
      vConvM2[0]=1; vConvM2[1]=0; 
      vConvM1[0]=0; vConvM1[1]=1; 
   
      pStart = pConv;  
   
      pConvM2 = pStart + vConvM2;
      pConvM1 = pStart + vConvM1;       
      
      evenConv = true;

      
    }
    else // We search for the next convergent
    {
      // Updating convergent
      pConvM2 = pConvM1;
      pConvM1 = pConv;
      vConvM2 = vConvM1;
      vConvM1 = pConv-pStart;

      // even, odd, even...
      evenConv = (evenConv == false);
      // Stock qk;
      qkode = qk;
    }
  } // end while :: all vertex have been found
    
  // if the last convergent before aPointb is above the straight line, we search
  // if have not missed some Points.
  // should be using the dichotomic proc
  if (evenConv == true && pConvM1 == aPointb)
  {
    int i = 0;
    
    while( i < qkode )
    {
      if (aPredicate(pStart, pConvM2 + i*pConvM2, aPointb) == false)
      {
        *aAlphaShapeHull++ = pConvM2 + i*pConvM2;
      }
      i++;
    }
    // If not compute, aPoint b is the last vertex
    *aAlphaShapeHull++ = aPointb;
  }

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
    CircumcircleRadiusPredicate<> predicate;
    convergents.clear(); 

    std::vector<Point> groundTruth; 

    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate, aPoint, bPoint, std::back_inserter(convergents) );  
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
    CircumcircleRadiusPredicate<> predicate1(100000,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicate1, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - Very large radius, R² = 100'000" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // Convergent lie on the circle
    CircumcircleRadiusPredicate<> predicat2(39337,4);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat2, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  39337/4." << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

       
    // Add the next vertex
    CircumcircleRadiusPredicate<> predicat6(2210,4);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat6, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  2209/4" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " <<nb<< " tests)" << std::endl;   
    std::cout << std::endl;

    // Add the next vertex
    CircumcircleRadiusPredicate<> predicat7(85,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(3,4));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat7, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  32" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // Add the next vertex
    CircumcircleRadiusPredicate<> predicat8(9,10);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(3,4));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat8, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  0.9" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;   
    std::cout << std::endl;

    // Add the next vertex
    CircumcircleRadiusPredicate<> predicat9(16,10);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(3,4));
    groundTruth.push_back(Point(4,5));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat9, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  1.6" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;  
    std::cout << std::endl;

    // Add the next vertex
    CircumcircleRadiusPredicate<> predicat10(4,10);
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

    convAlphaShape(predicat10, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  0.4" << std::endl; 
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
