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


  template <typename Point>
Point dichotomous(const RadiusCirclePredicate& aPredicate, const Point& aPointa,
  const Point aConvM2, const Point aConvM1, const int aqk)
{
    // convergent vector
    Point vConvM1 = aConvM2 - aPointa;
    
    // next convergent
    Point pConv = aConvM2 + aqk*aConvM1;
    
    // init search milestone
    int qkstart = 0;
    int qkstop  = aqk;
    // middle
    int mid;
    
    // We found the correct vertex
    bool boolconv = false;
    
    while ( boolconv == false )
    {
      mid = (qkstart + qkstop)/2;
      if (aPredicate(aPointa, pConv, aConvM2 + mid*vConvM1) == false)
      { 
        if (aPredicate(aPointa, pConv, aConvM2 + (mid+1)*vConvM1))
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
  return(Point(aConvM2 + mid*vConvM1));
}


/**
 * Given a straight line, find the alpha Hull,
 * @param aPoint, bPoint, the starting and ending point 
 * of the line
 * @return the alpha-hull
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
  int i;
  
  // The convergent number is even : the convergent is below the straight line
  bool evenConv = true;


  // dichotomie bool
  bool next = false;
  
  // The discrete straight-line [a, b]
  RayIntersectableStraightLine<Point> lineRatio(aPointa, aPointb);

  // we do not have compute all the candidat.
  while ( pConv != aPointb && lineRatio.dray(pConvM2, vConvM1, qk, pConv)) 
  {
  //std::cout<<"pconv : "<<pConv<<" -- as :"<<pStart<<std::endl;
    // We test if the convergent is inside the circle
    // ie : if the circumcircle radius of (pStart, pconv, pConvM2) triangle
    // is lower than -1*/alpha.
    // and if the convergent is below the straight line
    if ( (evenConv == true && aPredicate(pStart, pConvM2, pConv) == false) ||
     pConv == aPointb && aPredicate(pStart, pConvM1, pConv) == false)
    {
      // We have a new vertex pConvM2 or pConvM1 if we lie on the straight line
      if(pConv == aPointb){aAlphaShapeHull++ = pConvM1;}
      else{aAlphaShapeHull++ = pConvM2;}
        
      // dichotomous search
      pStart = dichotomous(aPredicate, pStart, pConvM2, pConvM1, qk);
      
      // We add the next alpha-shape vertex
      *aAlphaShapeHull++ = pConv;
      
      // We update pStart
      pStart = pConv;
      
      // we restart convergent calculation from pStart
      vConvM2[0]=1; vConvM2[1]=0; 
      vConvM1[0]=0; vConvM1[1]=1; 
      
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

    }
        
  } // end while :: all vertex have been found
  //*aAlphaShapeHull++ = aPointb;
  
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
    std::cout << " - Very large radius = Convexe hull" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;
        
    // Convergent lie on the circle
    RadiusCirclePredicate predicat3(39338,4);
    convergents.clear(); 
    
    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(5,8)); 
        
    convAlphaShape(predicat3, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - Convergent lie on the circle : No new vertex" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;
        
    // First new vertex : 2/3
    RadiusCirclePredicate predicat4(39337,4);
    convergents.clear(); 
    
    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(5,8)); 
        
    convAlphaShape(predicat4, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - New vertex : (2,3) -- predicat 4" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;
        
    // Add the next vertex
    RadiusCirclePredicate predicat5(2209,4);
    convergents.clear(); 
    
    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 
        
    convAlphaShape(predicat5, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - New vertex : (4,6)" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;
        
    // Add the next vertex
    RadiusCirclePredicate predicat6(11569,36);
    convergents.clear(); 
    
    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 
        
    convAlphaShape(predicat6, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - New vertex : (1,1) -- predicat 6" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;
        
    // Add the next vertex
    RadiusCirclePredicate predicat7(5849,36);
    convergents.clear(); 
    
    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0));
    groundTruth.push_back(Point(1,1));
    groundTruth.push_back(Point(2,2));
    groundTruth.push_back(Point(2,3));
    groundTruth.push_back(Point(4,6));
    groundTruth.push_back(Point(5,8)); 
        
    convAlphaShape(predicat7, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - New vertex : (2,2)" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " <<nb<< " tests)" << std::endl;   
    std::cout << std::endl;
        
    // Add the next vertex
    RadiusCirclePredicate predicat8(1359,16);
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
    std::cout << " - New vertex : (3,4)" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << std::endl;
        
    // Add the next vertex
    RadiusCirclePredicate predicat9(7119,256);
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
    std::cout << " - New vertex : (1,0)" << std::endl; 
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
    std::cout << " - New vertex : (4,5) -- predicat 10" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;  
    std::cout << std::endl;
        
    // Add the next vertex
    RadiusCirclePredicate predicat11(2,3);
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
    std::cout << " - New vertex : (4,5)" << std::endl; 
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
