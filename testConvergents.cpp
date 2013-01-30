#include <iostream>
#include <vector>
#include <iterator>

#include "PointVector2D.h"
#include "RayIntersectableStraightLine.h"

template <typename Point, typename OutputIterator>
void geometricConvergents(const Point& direction, 
                          OutputIterator res)
{
 //TODO
}


template <typename Point, typename OutputIterator>
void recursiveConvergents(const Point& direction, 
                          const Point& convergent1, const Point& convergent2, 
                          OutputIterator res)
{
  if (direction[0] > 0)
    {
      //Euclidean div
      int q = direction[1] / direction[0];   //quotient
      int r = direction[1] - q*direction[0]; //remainder
      //new convergent
      Point convergent3 = q * convergent2 + convergent1; 
      *res++ = convergent3; 
      //recursive call
      recursiveConvergents(Point(r,direction[0]), convergent2, convergent3, res);  
    }
}

int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  std::cout << "convergents" << std::endl; 
  {
    std::vector<Vector> convergents, convergents2;

    Point startingPt; 
    Point cm2(1,0); 
    Point cm1(0,1);
 
    //5/8
    convergents.clear(); 
    convergents2.clear(); 
    startingPt = Point(5,8);  

    recursiveConvergents(startingPt, cm2, cm1, std::back_inserter(convergents) );  

    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 

    if ( (convergents.size() == 4)&&(convergents.back() == startingPt) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    geometricConvergents(startingPt, std::back_inserter(convergents2) ); 
    if ( (convergents.size() == convergents2.size()) 
         &&( std::equal(convergents2.begin(), convergents2.end(), convergents.begin()) ) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;


    //11/16
    convergents.clear(); 
    convergents2.clear(); 
    startingPt = Point(11,16);
 
    recursiveConvergents(startingPt, cm2, cm1, std::back_inserter(convergents) );  

    std::copy(convergents.begin(), convergents.end(), std::ostream_iterator<Vector>(std::cout, ", ") ); 

    if ( (convergents.size() == 3)&&(convergents.back() == startingPt) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    geometricConvergents(startingPt, std::back_inserter(convergents2) ); 
    if ( (convergents.size() == convergents2.size()) 
         &&( std::equal(convergents2.begin(), convergents2.end(), convergents.begin()) ) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    //other tests to do...
 
  }


  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
