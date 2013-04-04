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
//#include "../inc/ConvexHullHelpers.h"
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
  template<typename Circle>
void radiusTool(const Circle aCircle)
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition

  // Max convergents 
  int maxConv = 50;	

  // On prend le rayon du prédicat inversement proportionnel au rayon du cercle.
  int myDen = 1;
  int myNum = floor( aCircle.getRadius() * aCircle.getRadius() );

  // Predicate
  CircumcircleRadiusPredicate<> predicate(myNum, myDen);
	CircumcircleRadiusPredicate<> predicate1(1, 0);
  // On récupère les sommets de l'alpha-shape et l'enveoppe convexe
  std::vector<Point> ch0;
  std::vector<Point> ch1;
  
  // Enveloppe convexe
  //alphaShape( aCircle, aCircle.getConvexHullVertex(), std::back_inserter(ch1), maxConv, predicate1 );
  convexHull(aCircle, aCircle.getConvexHullVertex(), std::back_inserter(ch1));
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
    files << (myNum / (double)myDen) << "\t";
    // Temps
    files << std::chrono::duration_cast<std::chrono::microseconds>(tb-ta).count() << "\t"; 
    // Nombre de sommets alpha-shape
    files << (ch0.size()-1) << "\t";
    // Nombre de sommets enveloppe convexe
    files << ch1.size() << std::endl;
    // On ferme le fichier
    files.close();
  }
  else
  {	
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
  }

}
//////////////////////////////////////////////////////////////////////
template <typename Shape, typename Point, typename OutputIterator>
void convexHull(const Shape& aShape, const Point& aStartingPoint, 
    OutputIterator res)
{
  OutputSensitiveConvexHull<Shape> ch(aShape); 
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
    files << "# Alpha-shape vertices" << "\t";
     // Nombre de sommets
    files << "# Convexe Hull vertices" << std::endl;
    // On ferme le fichier
    files.close();
  }
  else
  {	
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
  }

  Point pta, ptb, ptc;


   // Test number
  int nb_test = 50;
	int nb_point[nb_test];
	nb_point[0]=2;
	
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

	return 0;
}
