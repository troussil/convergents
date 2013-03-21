#include <iostream>
#include <math.h>
#include "PointVector2D.h"

int main() 
{
  std::cout << "testPointVector2D" << std::endl; 
  typedef PointVector2D<int> Point; //type redefinition

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests

  std::cout << "Basic operators" << std::endl; 
  {
    Point p;      //default construction
    Point q(1,2); //standard construction
    Point a = p, b = q; //copy construction
    Point c, d; 
    c = a; 
    d = b; //assignement

    if ( (a == p)&&(c == p)&&(b == q)&&(d == b)&&(p != q) )
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    if ( (q[0] == 1)&&(q[1] == 2) ) //read operators
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 
  }

  std::cout << "Arithmetic operators" << std::endl; 
  {
    Point p(5,18); 
    Point q(1,1);
    Point s = p;
    s[0] += 1; 
    s[1] += 1; 

    if ( (p + q) == s ) //addition test
      nbok++; 
    nb++;  
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    if ( (2*p) == ( p + p ) ) //multiplication by a scalar value
      nbok++; 
    nb++;  
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    Point r(2,1); 
    if ( r.dot(q) == 3 ) //dot product
      nbok++; 
    nb++;  
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 
  }

  std::cout << "Norms " << std::endl; 
  {
    Point p(5,18); 

    if ( (p.normL1()) == 23 ) 
      nbok++; 
    nb++;  
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 
    if ( (p.normLinf()) == 18 ) 
      nbok++; 
    nb++;  
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

    p = Point(5,-18); 

    if ( (p.normL1()) == 23 ) 
      nbok++; 
    nb++;  
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 
    if ( (p.normLinf()) == 18 ) 
      nbok++; 
    nb++;  
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl; 

  }
  //1 if at least one test failed
  //0 otherwise
  return (nb != nbok); 
}
