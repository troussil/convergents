#include <iostream>
#include <string>
#include <fstream>
//requires C++ 0x ou 11
#include <chrono>
//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>

#include <cmath>
// Core geometry
#include "../inc/PointVector2D.h"
// Circle
#include "../inc/RayIntersectableCircle.h"
// Convex Hull
#include "../inc/OutputSensitiveConvexHull.h"
#include "../inc/ConvexHullHelpers.h"
// Alpha-shape
#include "../inc/OutputSensitiveAlphaShape.h"

///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure that checks whether the 
 * output-sensitive algorithm returns the same
 * alpha-shape as the tracking-based algorithm
 * for a given circle. 
 * 
 * @param aCircle
 * @param aPredicate  
 * 
 * @return 'true' if the test passed, 'false' otherwise
 * 
 * @tparam
 * @tparam CircumcircleRadiusPredicate
 */
  template<typename Circle, typename CircumcircleRadiusPredicate>
bool test(const Circle aCircle, const CircumcircleRadiusPredicate& aPredicate)
{

  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition

  // Max convergents 
  int maxConv = 50;

  Point pStart = aCircle.getConvexHullVertex();

  std::vector<Point> ch0;
  std::vector<Point> ch1;
  std::vector<Point> boundary; 

  //tracking-based algorithm
  Vector dir(1,0); 
  openTracking( aCircle, pStart, pStart, dir, std::back_inserter(boundary) );

  openGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch0), aPredicate ); 


  std::cout << "#3.1 - alpha-shape of the boundary using OpenGrahamScan" << std::endl; 
  std::copy(ch0.begin(), ch0.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 

  alphaShape( aCircle, pStart, std::back_inserter(ch1), maxConv, aPredicate ); 

  std::cout << "#2 - alpha-shape" << std::endl; 
  std::copy(ch1.begin(), ch1.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 

  //COMPARE WITH YOUR ALGO HERE
  if (ch0.size() == ch1.size())
  {
    if ( std::equal(ch1.begin(), ch1.end(), ch0.begin()) )
    {
      return true;
    }
  }
  else {return false;}
}
///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure that checks whether the 
 * output-sensitive algorithm returns the same
 * alpha-shape as the tracking-based algorithm
 * for a given circle. 
 * 
 * @param aCircle
 * @param aPredicate  
 * 
 * @return 'true' if the test passed, 'false' otherwise
 * 
 * @tparam
 * @tparam CircumcircleRadiusPredicate
 */
  template<typename Circle>
void radiusTool(const Circle aCircle)
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition

  // Max convergents 
  int maxConv = 50;	

  // On prend le rayon du prédicat inversement proportionnel au rayon du cercle.
  int myDen = 100;
  int myNum = floor( aCircle.getRadius() * myDen );

  // Predicate
  CircumcircleRadiusPredicate<> predicate(myNum, myDen);

  // On récupère les sommets de l'alpha-shape
  std::vector<Point> ch0;

  // Mesure du temps d'exécution
  std::chrono::time_point<std::chrono::system_clock> ta, tb;
  ta = std::chrono::system_clock::now();

  alphaShape( aCircle, aCircle.getConvexHullVertex(), std::back_inserter(ch0), maxConv, predicate );

  tb = std::chrono::system_clock::now();
  
  // On ouvre le fichier en écriture avec effacement du fichier s'il est ouvert
  std::ofstream files("outcome/data.txt", std::ios::out | std::ios_base::app);

  if(files)
  {
    // Circle radius
    files << aCircle.getRadius() << "\t";
    // Predicate
    files << (-myDen / (double)myNum) << "\t";
    // Temps
    files << std::chrono::duration_cast<std::chrono::microseconds>(tb-ta).count() << "\t"; 
    // Nombre de sommets
    files << (ch0.size()-1) << std::endl;
    // On ferme le fichier
    files.close();
  }
  else
  {	
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
  }

}

//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename OutputIterator, 
  typename Predicate>
void alphaShape(const Shape& aShape, const Point& aStartingPoint, 
    OutputIterator res, int aMaxConv, const Predicate& aPredicate)
{
  OutputSensitiveAlphaShape<Shape, Predicate> ch(aShape, aPredicate);

  //get the first vertex
  Point tmp = aStartingPoint; 
  int k = 0;

  *res++ = tmp; 
  while (tmp != aStartingPoint || k == 0)
  {
    k++;
    // get the next alpha-shape vertices
    tmp = ch.next(aPredicate, tmp, aMaxConv, res);

  }//while we not return to aStartingPoint
}

///////////////////////////////////////////////////////////////////////
int main() 
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point> Circle; 

  int nbok = 0; //number of tests ok
  int nb = 0;   //total number of tests
  std::cout << std::endl; 

  std::cout << "I) Alpha-shape on a simple circle" << std::endl; 
  {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
    {
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
      std::cout << "1 - Expected - handmade" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      std::cout << "2 - Graham's convex hull of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    }

    std::cout << std::endl; std::cout << "  B) radius == 1" << std::endl; 
    {
      std::cout << "Expected (boundary)" << std::endl; 
      std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(1,1); //radius 1  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      std::cout << "1-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (ch.size() == boundary.size())
        if ( std::equal(boundary.begin(), boundary.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    }

    std::cout << std::endl; std::cout << "  B) radius == 3" << std::endl; 
    {
      std::vector<Point> groundTruth; 
      groundTruth.push_back(Point(0,-5)); 
      groundTruth.push_back(Point(2,-4)); 
      groundTruth.push_back(Point(3,-4)); 
      groundTruth.push_back(Point(4,-3)); 
      groundTruth.push_back(Point(4,-2)); 
      groundTruth.push_back(Point(5,0));
      groundTruth.push_back(Point(4,2));  
      groundTruth.push_back(Point(4,3));
      groundTruth.push_back(Point(3,4)); 
      groundTruth.push_back(Point(2,4)); 
      groundTruth.push_back(Point(0,5)); 
      groundTruth.push_back(Point(-2,4)); 
      groundTruth.push_back(Point(-3,4)); 
      groundTruth.push_back(Point(-4,3)); 
      groundTruth.push_back(Point(-4,2)); 
      groundTruth.push_back(Point(-5,0)); 
      groundTruth.push_back(Point(-4,-2)); 
      groundTruth.push_back(Point(-4,-3));
      groundTruth.push_back(Point(-3,-4));
      groundTruth.push_back(Point(-2,-4)); 
      std::cout << "Expected" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(9,1); //radius 3  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      std::cout << "3-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

    }
  }
  std::cout << std::endl; 

  std::cout << "I) Alpha-shape on a simple circle" << std::endl; 
  {

    Circle circle( Point(5,0), Point(0,5), Point(-5,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
    closedTracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl; std::cout << std::endl; 

    std::cout << "  A) infinite radius" << std::endl; 
    {
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
      std::cout << "1 - Expected - handmade" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate; //by default infinite radius (denominator = 0)  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      std::cout << "2 - Graham's convex hull of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

      std::vector<Point> as; 
      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
      as.erase(as.end()); 
      std::cout << "3 - Alpha shape" << std::endl; 
      std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (as.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    std::cout << std::endl; std::cout << "  B) radius == 1" << std::endl; 
    {
      std::cout << "Expected (boundary)" << std::endl; 
      std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(1,1); //radius 1  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      std::cout << "1-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (ch.size() == boundary.size())
        if ( std::equal(boundary.begin(), boundary.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

      std::vector<Point> as; 
      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
      as.erase(as.end()); 
      std::cout << "Alpha shape" << std::endl; 
      std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (as.size() == ch.size())
        if ( std::equal(ch.begin(), ch.end(), as.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }

    std::cout << std::endl; std::cout << "  B) radius == 3" << std::endl; 
    {
      std::vector<Point> groundTruth; 
      groundTruth.push_back(Point(0,-5)); 
      groundTruth.push_back(Point(2,-4)); 
      groundTruth.push_back(Point(3,-4)); 
      groundTruth.push_back(Point(4,-3)); 
      groundTruth.push_back(Point(4,-2)); 
      groundTruth.push_back(Point(5,0));
      groundTruth.push_back(Point(4,2));  
      groundTruth.push_back(Point(4,3));
      groundTruth.push_back(Point(3,4)); 
      groundTruth.push_back(Point(2,4)); 
      groundTruth.push_back(Point(0,5)); 
      groundTruth.push_back(Point(-2,4)); 
      groundTruth.push_back(Point(-3,4)); 
      groundTruth.push_back(Point(-4,3)); 
      groundTruth.push_back(Point(-4,2)); 
      groundTruth.push_back(Point(-5,0)); 
      groundTruth.push_back(Point(-4,-2)); 
      groundTruth.push_back(Point(-4,-3));
      groundTruth.push_back(Point(-3,-4));
      groundTruth.push_back(Point(-2,-4)); 
      std::cout << "Expected" << std::endl; 
      std::copy(groundTruth.begin(), groundTruth.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      std::vector<Point> ch;
      CircumcircleRadiusPredicate<> predicate(9,1); //radius 3  
      closedGrahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch), predicate ); 
      std::cout << "3-shape of the boundary" << std::endl; 
      std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (ch.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), ch.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;

      std::vector<Point> as; 
      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(as), 50, predicate );
      as.erase(as.end()); 
      std::cout << "Alpha shape" << std::endl; 
      std::copy(as.begin(), as.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
      std::cout << std::endl; 

      if (as.size() == groundTruth.size())
        if ( std::equal(groundTruth.begin(), groundTruth.end(), as.begin()) )
          nbok++; 
      nb++; 
      std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    }
  }
  std::cout << std::endl<<"II) Automatic testing -shape of the boundary" << std::endl<< std::endl; 


  // Test number
  int nb_test = -10;

  // Max origin coordinate
  int maxPoint = 100;

  // Number predicate test
  int nbPredicate = 10;
  int valuePredicate[10] = {3, 4, 10, 20, 200, 2000, 20000, 100000, 200000, 2000000};
  // Circumcircle triangle vertices
  Point pta, ptb, ptc;

  for (nb_test;nb_test>0;nb_test--)
  {
    {
      // random circumcircle 
      pta = Point( (rand() % maxPoint)             , (rand() % maxPoint) );
      ptb = Point( (pta[0]-1- (rand() % maxPoint) ), (pta[1]-1- (rand() % maxPoint)) );
      ptc = Point( (ptb[0]+1+ (rand() % maxPoint) ), (ptb[1]-1- (rand() % maxPoint)) );

      Circle circle( pta, ptb, ptc );

      std::cout << "II - "<<nb_test<<" - Alpha-shape on the circle : " << std::endl; 

      std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
        << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
        << " ] | Points : "<< pta<< ptb<< ptc<< " - First vertex : " 
        << circle.getConvexHullVertex() << std::endl;


      for (int i = 0; i < nbPredicate; i++)
      {

        {
          CircumcircleRadiusPredicate<> predicate(valuePredicate[i], 2);
          std::cout << "Radius predicate : Num2 / Den2 : "<<valuePredicate[i]<<"/" 
            << 2 << std::endl;


          if (test(circle, predicate))
            nbok++;
          nb++; 
          std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
          std::cout << " ----------- Next predicate ----------- " << std::endl; 
          std::cout << std::endl;

        }
        //if(nb != nbok){break;}
      }

    }
  }
  //  (0,2), (1,3), (2,4), (2,5), (2,6), (2,7), (1,8), (-1,9), (-3,8), (-4,7), (-4,6), (-4,5), (-4,4), (-4,3), (-2,2), (-1,2), (0,2), 
  //-- Disk[ Center : (-1.27273, 5.40909 ), Radius : 3.63892 ] | Points : (2,7)(-4,3)(0,2) - First vertex : (0,2)
  //10/2
  {
    pta = Point(2,7);
    ptb = Point(-4,3);
    ptc = Point(0,2);
    Circle circle( pta, ptb, ptc );

    std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
      << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
      << " ] | Points : "<< pta<< ptb<< ptc<< " - First vertex : " 
      << circle.getConvexHullVertex() << std::endl;

    CircumcircleRadiusPredicate<> predicate(10,2);
    std::cout << "Radius predicate : Num2 / Den2 : 10/2"<< std::endl;


    if (test(circle, predicate))
      nbok++; 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << " ----------- Next predicate ----------- " << std::endl; 
    std::cout << std::endl;
  }
  //(4,2)(2,1)(6,-5) - (4,2)(0,-6)(10,-14) - (7,8)(-1,-1)(3,-8) - (5,3)(2,2)(4,-7)
  {
    pta = Point(4,0);
    ptb = Point(0,4);
    ptc = Point(-4,0);
    Circle circle( pta, ptb, ptc );

    std::cout << "-- Disk[ Center : (" << circle.getCenterX() << ", " 
      << circle.getCenterY()<< " ), Radius : " << circle.getRadius()
      << " ] | Points : "<< pta<< ptb<< ptc<< " - First vertex : " 
      << circle.getConvexHullVertex() << std::endl;

    CircumcircleRadiusPredicate<> predicate(3,2);
    std::cout << "Radius predicate : Num2 / Den2 : 10/2"<< std::endl;


    if (test(circle, predicate))
    {nbok++;} 
    nb++; 
    std::cout << "(" << nbok << " tests passed / " << nb << " tests)" << std::endl;
    std::cout << " ----------- Next predicate ----------- " << std::endl; 
    std::cout << std::endl;
  }
  //1 if at least one test failed
  //0 otherwise

  ///////////////////////////////////////////////////////////////////////
  std::cout << "V) Output" << std::endl; 

	// On ouvre le fichier en écriture avec effacement du fichier s'il est ouvert
  std::ofstream files("outcome/data.txt", std::ios::out | std::ios::trunc);

  if(files)
  {
    // Circle radius
    files << "Radius" << "\t";
    // Predicate
    files << "predicate" << "\t";
    // Temps
    files << "time" << "\t"; 
    // Nombre de sommets
    files << "# Vertices" << std::endl;
    // On ferme le fichier
    files.close();
  }
  else
  {	
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
  }

   // Test number
  nb_test = 600;
	int nb_point[nb_test];
	nb_point[0]=1;
	
  // progression au carré
  for (int i = 1; i<nb_test; i++)
  {
  	nb_point[i] = nb_point[i-1]+1; 
  }
  

  for (int i = 0; i<nb_test; i++)
  {
  // Circumcircle triangle vertices
    pta = Point(nb_point[i], 0);
    ptb = Point(0, nb_point[i]);
    ptc = Point(-nb_point[i], 0);

    Circle circle( pta, ptb, ptc );

  	radiusTool(circle);
  }


    return (nb != nbok); 
}
