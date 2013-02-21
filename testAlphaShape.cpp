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
 * @param aEven is bool which determine if the last odd convergent lie on the
 * straight line.
 * @return first convergent in the alpha shape
 */
  template <typename Point>
Point dichotomous(const RadiusCirclePredicate& aPredicate, const Point& aPointa,
    const Point aConvM2, const Point aConvM1, const int aqk, const bool aEven)
{

  // Convergent vector
  Point vConvM1 = aConvM1 - aPointa;

  // next convergent
  Point pConv = aConvM2 + aqk*vConvM1;

  // init search milestone
  int qkstart = 1;
  int qkstop  = aqk;
  // middle
  int mid;

  // We found the correct vertex
  bool boolconv = false;

  if (aEven == true) // Even case
  {
    while ( boolconv == false )
    {
      mid = (qkstart + qkstop)/2;
      if (aPredicate(aPointa, pConv, pConv - mid*vConvM1) == false)
      { 
        if (qkstop == 1 || aPredicate(aPointa, pConv, pConv - (mid-1)*vConvM1) == true)
        { // We found the good one 
          boolconv = true;
        }
        else
        { // the vertex is higher
          qkstop = mid - 1;
        }
      }
      else
      { // the vertex is lower
        qkstart = mid + 1;
      }
    } 
    // return a new point
    return(Point(pConv - mid*vConvM1));
  }
  else //Odd case
  {
    while ( boolconv == false )
    {
      mid = (qkstart + qkstop)/2;
      if (aPredicate(aPointa, pConv, aPointa + mid*vConvM1) == false)
      { 
        if (qkstart == 1 || aPredicate(aPointa, pConv, aPointa + (mid+1)*vConvM1) == true)
        { // We found the good one 
          boolconv = true;
        }
        else
        { // the vertex is higher
          qkstart = mid + 1;
        }
      }
      else
      { // the vertex is lower
        qkstop = mid - 1;
      }
    } 
    // return a new point
    return(Point(aPointa + mid*vConvM1));
  }
}


/**
 * Given a straight line, find the alpha Hull,
 * @param aPredicate determine the alpha shape radius
 * @param aPoint, bPoint, the starting and ending point of the straight
 * line.
 * @return the alpha-hull of the straight line.
 */
  template <typename Point, typename OutputIterator>
void convAlphaShape(const RadiusCirclePredicate& aPredicate, const Point& aPointa,
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

std::cout<<"pStart : "<<pStart<< " + p-2 : "<<pConvM2<< " + p-1 : "<<pConvM1<< " + pk = "<<pConv<< qk<<" pair : "<<evenConv<<" So : p-2 : "<<(aPredicate(pStart, pConvM2, pConv)==false)<<" So : p-1 : "<<(aPredicate(pStart, pConvM1, pConv)==false)<<std::endl;
    
    if ( (evenConv == true && aPredicate(pStart, pConvM2, pConv) == false) ||
        (evenConv == false && aPredicate(pStart, pConvM1, pConv) == false))
    {
      // pConv and pConvM2 are confounded
      std::cout<<"--Vertex : ";
      if (qk == 0)
      {
         // We add the next alpha-shape vertex
        *aAlphaShapeHull++ = pConvM2;
        pStart = pConv;
        std::cout<<"0 "<<pConv;
      }
      else 
      {
       if (qk == 1)
       {
         // We add the two next alpha-shape vertices
        *aAlphaShapeHull++ = pConvM2;
        // We start from the second vertex
        *aAlphaShapeHull++ = pConv;
        pStart = pConv;
        std::cout<<"1 "<<pConvM2<<pConv;
       }      
        else //if(qk > 1)
        {
          pStart = dichotomous(aPredicate, pStart, pConvM2, pConvM1, qk, evenConv);
          // We add the next alpha-shape vertex
          *aAlphaShapeHull++ = pStart;
          std::cout<<"2++ "<<pStart;
        }
      }
      std::cout<<std::endl;

      // we restart convergent calculation from pStart
      vConvM2[0]=1; vConvM2[1]=0; 
      vConvM1[0]=0; vConvM1[1]=1; 
   
      while (lineRatio(pStart + vConvM1) <=0)
      { 
        pConvM1 = pStart + vConvM1;  
        *aAlphaShapeHull++ = pConvM1;
        pStart = pConvM1;
        std::cout<<"--rotation : "<< pConvM1<<std::endl;
      }
      
      pConvM2 = pStart + vConvM2;
      pConvM1 = pStart + vConvM1;       
      
      evenConv = true;

    }
    else // We search for the next convergent
    {         std::cout<<"--convergent : "<< std::endl;
      // Updating convergent
      pConvM2 = pConvM1;
      pConvM1 = pConv;
      vConvM2 = vConvM1;
      vConvM1 = pConv-pStart;

      // even, odd, even...
      evenConv = (evenConv == false);

    }

  } // end while :: all vertex have been found
  *aAlphaShapeHull++ = aPointb;

} // end proc




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
    RadiusCirclePredicate predicate;
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
    RadiusCirclePredicate predicate2(100000,1);
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

    // Convergent lie on the circle
    RadiusCirclePredicate predicat3(1000,2);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat3, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  500." << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // First new vertex : 2/3
    RadiusCirclePredicate predicat4(35,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat4, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  35." << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // Add the next vertex
    RadiusCirclePredicate predicat5(34,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat5, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  34." << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // Add the next vertex
    RadiusCirclePredicate predicat6(33,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat6, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  33." << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // Add the next vertex
    RadiusCirclePredicate predicat7(32,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat7, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  32." << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " <<nb<< " tests)" << std::endl;   
    std::cout << std::endl;

    // Add the next vertex
    RadiusCirclePredicate predicat8(31,1);
    convergents.clear(); 

    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(3,4));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 

    convAlphaShape(predicat8, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  31" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;

    // Add the next vertex
    RadiusCirclePredicate predicat9(30,1);
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

    convAlphaShape(predicat9, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  30" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;   
    std::cout << std::endl;

    // Add the next vertex
    RadiusCirclePredicate predicat10(13,10);
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

    convAlphaShape(predicat10, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << "-- R² =  1.3" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;

    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;  
    std::cout << std::endl;

    // Add the next vertex
    RadiusCirclePredicate predicat11(4,10);
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

    convAlphaShape(predicat11, aPoint, bPoint, std::back_inserter(convergents) );  
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
