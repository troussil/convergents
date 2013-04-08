#include <iostream>
#include <string>
#include <fstream>
//requires C++ 0x ou 11
#include <chrono>
#include <time.h>
//containers and iterators
#include <iterator>
#include <vector>
#include <deque>
// random
#include <cstdlib>
#include <ctime>

#include <cmath>
#include <stdlib.h>
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
 * @brief Procedure thar return the circle convex hull
 * 
 * @param aCircle
 * @param aStartingPoint
 * @param res  
 * 
 * @return 
 * 
 * @tparam
 * @tparam CircumcircleRadiusPredicate
 */
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

/**
 * @brief Procedure thar return the circle alpha-shape
 * 
 * @param aCircle
 * @param aStartingPoint
 * @param res  
 * @param aMaxConv  
 * @param aPredicate
 *
 * @return 
 * 
 * @tparam
 */
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

//////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure thar create a files with information : 
 * time, radius, predicate and alpha-shape and convex hull number of vertices
 * files is create in the folder outcome with the name of data.txt.
 * @param aTestnb
 *
 * @return 
 * 
 * @tparam
 */ 
void rToolValue(int aTestnb)
{
  typedef PointVector2D<int> Point; 
  //typedef PointVector2D<int> Vector; 
  typedef RayIntersectableCircle<Point> Circle; 

  // We open the files at first line.
  std::ofstream files("outcome/data.txt", std::ios::out | std::ios::trunc);
  if(files)
  {
    // Circle radius
    files << "Radius" << "\t";
    // Predicate
    files << "predicate" << "\t";
    // Time
    files << "time" << "\t"; 
    // Alpha-shape vertices number
    files << "# Alpha-shape vertices" << "\t";
    // Convex-hull vertices number
    files << "# Convexe Hull vertices" << std::endl;
    // We close the files 
    files.close();
  }
  else
  {	
    std::cerr << "We can't open the files !" << std::endl;
  }

  // Triangle
  Point pta, ptb, ptc;

  // Circle formed by the circumcircle of the triangle formed by pta, ptb, ptc.
  Circle circle;

  // Convergents maximum iteration (ie for irationnal) 
  int maxConv = 50;

  // Predicate
  int myDen = 100;
  int myNum;

  /** 
   * Predicate with infinite radius :
   * Convex Hull plus vertices whick lie on the edge.
   */
  CircumcircleRadiusPredicate<> predicate1(1, 0);

  // First radius
  int R = 2;

  // Circumcircle creation
  for (int i = 0; i< aTestnb/2; i++)
  {
    // Circumcircle triangle vertices
    R += 2;

    pta = Point(R, 0);
    ptb = Point(0, R);
    ptc = Point(-R, 0);

    Circle circle( pta, ptb, ptc );

    myNum = floor( circle.getRadius() * circle.getRadius() *100);

    // Predicate
    CircumcircleRadiusPredicate<> predicate(myNum, myDen);

    /** 
     * Reset iterator 
     * chcv recover the number of vertices of the convex hull.
     * chas recover the number of vertices of the alpha-shape.
     * 
     */
    std::vector<Point> ch0;
    std::vector<Point> ch1;

    // Convex Hull
    convexHull(circle, circle.getConvexHullVertex(), std::back_inserter(ch1));

    // Execution time
    std::chrono::time_point<std::chrono::system_clock> ta, tb;
    ta = std::chrono::system_clock::now();
    // Alpha Shape
    alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(ch0), maxConv, predicate );
    tb = std::chrono::system_clock::now();

    // We writing at the end  the files
    std::ofstream files("outcome/data.txt", std::ios::out | std::ios_base::app);
    if(files)
    {
      // Circle radius
      files << R << "\t";
      // Predicate
      files << (myNum / (double)myDen) << "\t";
      // Time
      files << std::chrono::duration_cast<std::chrono::microseconds>(tb-ta).count() << "\t"; 
      // Nombre de sommets alpha-shape
      files << (ch0.size()-1) << "\t";
      // Alpha-Shape Vertices Number
      files << ch1.size() << std::endl;
      // On close the files
      files.close();
    }
    else
    {	
      std::cerr << "We can't open the files !" << std::endl;
    }
  }
}

/**
 * @brief Procedure thar create a files with information : 
 * time, radius, predicate and alpha-shape and convex hull number of vertices
 * files is create in the folder outcome with the name of data-moy.txt. 
 *
 * @param aRadiusmin : Computation start for a radius R = 2^aRadiusmin
 * @param aRadiusmax : Computation finish for a radius R = 2^(aRadiusmin-1)
 * @param aTestnb : We compute aTestnb number of circle with a certain radius  
 *
 * @return 
 * 
 * @tparam
 */ 
void rToolMeans(int aRadiusmin, int aRadiusmax, int aTestnb)
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point> Circle; 
  typedef std::chrono::time_point<std::chrono::system_clock> clock;
  typedef std::chrono::milliseconds milliseconds;

  // Circle creation : ax + by + c(x^2 + y^2)
  long int a, b, c, d;
  c =  -50;

  // Convergents maximum iteration (ie for irationnal).
  int maxConv = 50;

  // Init radius with R = 2^aRadiusmin.
  long int R =2;
  for (int k = 1; k < aRadiusmin; k++)	{R *= 2;}

  int myDen = 100;
  int myNum;

  // Circle
  Circle circle;

  // Predicate with infinite radius : Convex Hull 
  CircumcircleRadiusPredicate<> predicate0(1, 0);

  //random value Initialisation
  srand ( time(NULL) );

  // Save the first values for min and max.
  bool bfirst;

  /**
   * We want to keep some values : 
   * Min, max and means of execution time.
   * Min, max and means of convex hull, alpha-shape with infinite radius, and alphashape with 
   * proportional radius number of vertices.
   */  

  // Time
  clock ta, tb;
  int tmptime, time_min, time_max;
  double time_means;
  // Vertices number
  int tmpnb;
  int cv_min,  cv_max;  double cv_means;  // convex hull
  int as0_min, as0_max;	double as0_means; // alpha-shape with infinite radius
  int as_min,  as_max;	double as_means;  // alpha-shape

  // We open the files from start and add a title on the first line of the table.
  std::ofstream files("outcome/data-moy.txt", std::ios::out | std::ios::trunc);
  if(files)
  {
    // Circle radius
    files << "Radius|" << "\t";
    // Predicate
    files << "predicate" << std::endl;
    // time : Means, min, max
    files << "time - means," << "\t"<< "time - min," << "\t"<< "time - max|" << "\t"; 
    // Concex Hull : Means, min, max
    files << "# Convexe Hull - means,"   << "\t"<< "# Convexe Hull - min,"   << "\t"
      << "# Convexe Hull - max|"   << "\t";
    // Alpha-Shape with predicate = 0 : Means, min, max
    files << "# Alpha-shape-CV - means," << "\t"<< "# Alpha-shape-CV - min," << "\t"
      << "# Alpha-shape-CV - max|" << "\t";
    // Alpha-Shape : Means, min, max
    files << "# Alpha-shape - means,"    << "\t"<< "# Alpha-shape - min,"    << "\t"
      << "# Alpha-shape - max|" << std::endl;; 
    files.close();
  }
  else
  {	
    std::cerr << "We can't open the files !" << std::endl;
  }


  for (int j = aRadiusmin; j < aRadiusmax; j++)
  {
    /**
     * We take a radius for the predicate proportional to the radius of 
     * tha alpha-shape. R_alpha = 100* R^2 / 100
     */ 
    myNum = floor(R*R*100);

    // Predicate
    CircumcircleRadiusPredicate<> predicate(myNum, myDen);

    // Reset values
    bfirst = true;
    time_min = 0; time_max = 0; time_means = 0.0;
    cv_min   = 0; cv_max   = 0; cv_means   = 0.0;
    as0_min  = 0; as0_max  = 0; as0_means  = 0.0;
    as_min   = 0; as_max   = 0; as_means   = 0.0;

    for (int i = 0; i < aTestnb; i++)
    {
      /**
       * We create a new circle from a, b, c, d : ax + by + c(x^2 + y^2)
       * The circle have a random center pt_c in [0;1]*[0;1] and a fiexed radius = R
       * We fixed c = -50.  pt_c = [-a/2c ; -b/2c] and R² = (a^2 + b^2 - 4*c*d)/(4c²)
       */
      a = rand() %(2*c);
      b = rand() %(2*c);  
      d = ( a*a + b*b - 4*R*R*c*c)/(4*c);

      // Rinit circle from a, b, c, d.
      Circle circle( a, b, c, d );	

      /** 
       * Reset iterator 
       * chcv recover the number of vertices of the convex hull.
       * chas recover the number of vertices of the alpha-shape with infinite radius.
       * chas recover the number of vertices of the alpha-shape.
       * 
       */
      std::vector<Point> chcv;
      std::vector<Point> chas0;
      std::vector<Point> chas;

      // Convex Hull
      convexHull( circle, circle.getConvexHullVertex(), std::back_inserter(chcv));
      // alpha-shape with infinite radius
      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(chas0), maxConv, predicate0 );

      // Execution time
      ta = std::chrono::system_clock::now();
      // alpha-shape
      alphaShape( circle, circle.getConvexHullVertex(), std::back_inserter(chas), maxConv, predicate );
      tb = std::chrono::system_clock::now();

      // Test
      //std::cout<<"R : "<<R<<", "<<myNum / (double)myDen<<", "<< circle.getRadius()<<std::endl;
      //std::cout<< circle.getCenterX()<<circle.getCenterY()<<std::endl;

      // time
      tmptime = (tb - ta).count();
      if (tmptime < time_min || bfirst == true) {time_min = tmptime;}
      if (tmptime > time_max || bfirst == true) {time_max = tmptime;}
      time_means += tmptime/(double)aTestnb;

      // Concex Hull Vertices Number
      tmpnb = chcv.size();
      if (tmpnb < cv_min || bfirst == true) {cv_min = tmpnb;}
      if (tmpnb > cv_max || bfirst == true) {cv_max = tmpnb;}
      cv_means += tmpnb/(double)aTestnb;

      // Alpha-Shape with predicate = 0 - Vertices Number
      tmpnb = chas0.size()-1;
      if (tmpnb < as0_min || bfirst == true) {as0_min = tmpnb;}
      if (tmpnb > as0_max || bfirst == true) {as0_max = tmpnb;}
      as0_means += tmpnb/(double)aTestnb;

      // Alpha-Shape with predicate = 0 - Vertices Number
      tmpnb = chas.size()-1;
      if (tmpnb < as_min || bfirst == true) {as_min = tmpnb;}
      if (tmpnb > as_max || bfirst == true) {as_max = tmpnb;}
      as_means += tmpnb/(double)aTestnb;
    } //aTestnb loop with a fixed radius

    // We open the files from start and add a title on the first line of the table.
    std::ofstream files("outcome/data-moy.txt", std::ios::out | std::ios::app);
    if(files)
    {
      // Circle radius
      files << R << "\t";
      // Predicate
      files << (myNum / (double)myDen) << "\t";
      // time : Means, min, max
      files << time_means << "\t" << (tb-ta).count()<< "\t" << time_max << "\t"; 
      // Concex Hull : Means, min, max
      files << cv_means << "\t" << cv_min<< "\t" << cv_max << "\t";
      // Alpha-Shape with predicate = 0 : Means, min, max
      files << as0_means << "\t" << as0_min<< "\t" << as0_max << "\t";
      // Alpha-Shape : Means, min, max
      files << as_means << "\t" << as_min<< "\t" << as_max << std::endl;
      files.close();
    }
    else
    {	
      std::cerr << "We can't open the files !" << std::endl;
    }
    // Restart with a bigger radius 
    R = R*2; 
    bfirst = false;
  }// end loop - R reach its maximum asked aRadiusmax
}


///////////////////////////////////////////////////////////////////////
int main() 
{
  ///////////////////////////////////////////////////////////////////////
  std::cout << "V) Output" << std::endl; 

  //2^5 = 32, 2^15 = 32768

  // second graph
  rToolMeans(5,15,100);

  // First graph
  rToolValue(36000);  

  return 0;
}
