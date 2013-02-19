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

  // The discrete straight-line [a, b]
  RayIntersectableStraightLine<Point> lineRatio(aPointa, aPointb);

  // we do not have compute all the candidat.
  while ( pConv != aPointb && lineRatio.dray(pConvM2, vConvM1, qk, pConv)) 
  {
std::cout<<pConv<<std::endl;     
std::cout<<"Resul : "<<aPredicate(pStart, pConv, pConvM2)<<std::endl;   
    
    // Next convergent calculation
    //DroiteRatio.dray(pConvM2, vConvM1, qk, pConv);
    
    // We test if the convergent is inside the circle
    // ie : if the circumcircle radius of (pStart, pconv, pConvM2) triangle
    // is minus than -1*/alpha.
    // and if the convergent is below the straight line
    if ( ((evenConv == true) || (pConv == aPointb)) && aPredicate(pStart, pConv, pConvM2) == false)
    {
      // We have a new vertex pConvM2
      *aAlphaShapeHull++ = pConvM2;
        
      // should be an independante procedure
      i = qk/2;
      while (aPredicate(pStart, pConvM2, pConv - (i+1)*vConvM1) == true)
      {
        // dichotomous search
        // first half
        //
        if (aPredicate(pStart, pConvM2, pConv - i*vConvM1) == false)
        {
          i = i/2; 
        }
        else // second half
        {
          i = (i - i/2)/2;
        }
      }
      
      // We add the next alpha-shape vertex
      *aAlphaShapeHull++ = pConvM2 + i*vConvM1;
      
      // We update pStart
      pStart = pConvM2 + i*vConvM1;
      
      // we restart convergent calculation from pStart
      vConvM2[0]=1; vConvM2[1]=0; 
      vConvM1[0]=0; vConvM1[1]=1; 
      
      pConvM2 = pStart + vConvM2;
      pConvM1 = pStart + vConvM1;
      
      evenConv = true;
      
    }
    else // We search for the next convergent
    {   
//std::cout<<pConv<<std::endl;       
      // Updating convergent
      pConvM2 = pConvM1;
      pConvM1 = pConv;
      vConvM2 = vConvM1;
      vConvM1 = pConv-pStart;
      
      // even, odd, even...
      evenConv = (evenConv == false);

    }
        
  } // end while :: all vertex have been found

} // end proc

  
template <typename Point, typename OutputIterator>
void recAlphaShape(const RadiusCirclePredicate& predicat, const Point& aPoint, const Point bPoint, RayIntersectableStraightLine<Point> DroiteRatio, OutputIterator AlphaShapeHull)
{

  *AlphaShapeHull++ = aPoint;
  // Init p_-2=(1,0) & p_-1=(0,1)
  Point cm2, cm1, pm2, pm1;

  cm2[0]=1; cm2[1]=0; 
  cm1[0]=0; cm1[1]=1; 

  pm2 = aPoint + cm2;
  pm1 = aPoint + cm1;
  
  // pconv is the next convergent pconv = pm2 + qk * pm1
  Point pconv;
  int qk = 0;
  int i;
  
  // Bezout point
  Point pcandid = aPoint;
    
  // The discrete straight-line
  //RayIntersectableStraightLine<Point> DroiteRatio(aPoint, bPoint-aPoint);

  // Bezout Point
std::cout <<"#1 - pm2, cm2 : "<<pm2<<cm2 <<" +-+ pm1,cm1 : "<<pm1<<cm1<<std::endl; 
  while( DroiteRatio.dray(pm2, cm1, qk, pconv) == true)
  {

    i = 1;
std::cout <<"#1 - a : "<<aPoint <<" +-+ b : "<<bPoint<<std::endl; 


    while (i <= qk)
    {
      if (DroiteRatio(pm2 + (i*cm1)) < 0)
      {
        pcandid = pm2 + (i*cm1);
        i = qk+1;
          std::cout <<"#refactor - pcandid : "<<pcandid <<std::endl;
      }
      i++;
    }
    pm2 = pm1;
    pm1 = pconv;
    cm2 = cm1;
    cm1 = pconv-aPoint;
std::cout <<"#1 - pm2, cm2 : "<<pm2<<cm2 <<" +-+ pm1,cm1 : "<<pm1<<cm1<<std::endl; 
std::cout <<"#3 - pconv : "<<pconv <<std::endl;
std::cout <<"#test : "<<DroiteRatio.dray(pm2, cm1, qk, pconv) <<std::endl;
  }
  std::cout <<"#2 - pcandid : "<<pcandid <<std::endl;
   
  if (predicat(aPoint, pcandid, bPoint) == false)
  {  
  std::cout <<"++ AA ++"<<std::endl;   
     recAlphaShape(predicat, aPoint, pcandid, DroiteRatio, AlphaShapeHull );
  std::cout <<"-- BB --"<<std::endl;     
     recAlphaShape(predicat, pcandid, bPoint, DroiteRatio, AlphaShapeHull );
  std::cout <<"** CC **"<<std::endl;
  }
  *AlphaShapeHull++ = bPoint;


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
    RadiusCirclePredicate predicat;
    convergents.clear(); 
    
    std::vector<Point> groundTruth; 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(5,8)); 
        
    convAlphaShape(predicat, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - Infinite radius = Convexe hull" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
           
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
   
    // Very large radius
    RadiusCirclePredicate predicat2(100000,1);
    convergents.clear(); 
    
    groundTruth.clear(); 
    groundTruth.push_back(Point(0,0)); 
    groundTruth.push_back(Point(5,8)); 
        
    convAlphaShape(predicat2, aPoint, bPoint, std::back_inserter(convergents) );  
    std::cout << " - Very large radius = Convexe hull" << std::endl; 
    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    std::cout << std::endl;
    
    if (convergents.size() == groundTruth.size())
      if ( std::equal(groundTruth.begin(), groundTruth.end(), convergents.begin()) )
        nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    
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
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;   
    
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
    
    // Add the next vertex
    RadiusCirclePredicate predicat10(399,16);
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
    
    convergents.clear(); 
    //RayIntersectableStraightLine<Point> DroiteRatio(aPoint, bPoint);
    
    //recAlphaShape(predicat6, aPoint, bPoint, DroiteRatio, std::back_inserter(convergents) );
    //std::cout << " - autre méthode, predicat 8" << std::endl; 
    //std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 
    //std::cout << std::endl;
    
  }


  /*if ( (convergents.size() == 4)&&(convergents.back() == aPoint) )
    nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;



    }

    std::cout << std::endl; 
    std::cout << "II) Alpha-shape on a simple circle" << std::endl; 
    {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
    std::vector<Point> groundTruth; 
    groundTruth.push_back(Point(0,-5)); 
    groundTruth.push_back(Point(3,-4)); 
    groundTruth.push_back(Point(4,-3)); 
    groundTruth.push_back(Point(5,0)); 
    groundTruth.push_back(Point(4,3));
    groundTruth.push_back(Point(3,4)); 
    groundTruth.push_back(Point(0,5)); 
    groundTruth.push_back(Point(-3,4)); 
    groundTruth.push_back(Point(-4,3)); 
    groundTruth.push_back(Point(-5,0)); 
    groundTruth.push_back(Point(-4,-3));
    groundTruth.push_back(Point(-3,-4)); 
    std::cout << "Expected" << std::endl; 
    std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    std::vector<Point> ch;
    RadiusCirclePredicate predicate; //by default infinite radius (denominator = 0)  
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
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
    RadiusCirclePredicate predicate1(1,1); //radius 1  
    grahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch1), predicate1 ); 
    std::cout << "1-shape of the boundary" << std::endl; 
    std::copy(ch1.begin(), ch1.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; 

    if (ch1.size() == boundary.size())
    if ( std::equal(boundary.begin(), boundary.end(), ch1.begin()) )
    nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

  std::cout << "  B) radius == 3" << std::endl; 

  std::vector<Point> groundTruth3; 
  groundTruth3.push_back(Point(0,-5)); 
  groundTruth3.push_back(Point(2,-4)); 
  groundTruth3.push_back(Point(3,-4)); 
  groundTruth3.push_back(Point(4,-3)); 
  groundTruth3.push_back(Point(4,-2)); 
  groundTruth3.push_back(Point(5,0));
  groundTruth3.push_back(Point(4,2));  
  groundTruth3.push_back(Point(4,3));
  groundTruth3.push_back(Point(3,4)); 
  groundTruth3.push_back(Point(2,4)); 
  groundTruth3.push_back(Point(0,5)); 
  groundTruth3.push_back(Point(-2,4)); 
  groundTruth3.push_back(Point(-3,4)); 
  groundTruth3.push_back(Point(-4,3)); 
  groundTruth3.push_back(Point(-4,2)); 
  groundTruth3.push_back(Point(-5,0)); 
  groundTruth3.push_back(Point(-4,-2)); 
  groundTruth3.push_back(Point(-4,-3));
  groundTruth3.push_back(Point(-3,-4));
  groundTruth3.push_back(Point(-2,-4)); 
  std::cout << "Expected" << std::endl; 
  std::copy(groundTruth3.begin(), groundTruth3.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 

  std::vector<Point> ch3;
  RadiusCirclePredicate predicate3(9,1); //radius 3  
  grahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch3), predicate3 ); 
  std::cout << "3-shape of the boundary" << std::endl; 
  std::copy(ch3.begin(), ch3.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 

  if (ch3.size() == groundTruth3.size())
    if ( std::equal(groundTruth3.begin(), groundTruth3.end(), ch3.begin()) )
      nbok++; 
  nb++; 
  std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

}

//1 if at least one test failed
//0 otherwise
return (nb != nbok);*/ 
}
