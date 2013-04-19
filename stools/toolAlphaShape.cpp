///////////////////////////////////////////////////////////////////////////////
//requires STL
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

//requires C++ 0x ou 11
#include <chrono>
#include <time.h>

// requires random
#include <cstdlib>
#include <ctime>

//containers and iterators
#include <iterator>
#include <vector>
#include <deque>

//requires boost
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

//requires DGtal
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"

using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
//our work
#include "../inc/PointVector2D.h"
#include "../inc/RayIntersectableCircle.h"
#include "../inc/OutputSensitiveConvexHull.h"
#include "../inc/OutputSensitiveAlphaShape.h"
#include "../inc/ConvexHullHelpers.h"
#include "../inc/CircumcircleRadiusPredicate.h"



///////////////////////////////////////////////////////////////////////
/**
 * @brief Procedure thar return the circle convex hull
 * 
 * @param aCircle
 * @param res  
 * 
 * @return
 * @tparam 
 * 
 */
  template <typename Shape, typename OutputIterator>
void convexHull(const Shape& aShape, OutputIterator res)
{
  OutputSensitiveConvexHull<Shape> ch(aShape); 
  ch.all(res);
}

/**
 * @brief Procedure thar return the circle alpha-shape
 * 
 * @param aCircle
 * @param res  
 * @param aPredicate
 *
 * @return
 * @tparam 
 */
//////////////////////////////////////////////////////////////////////
template <typename Shape, typename OutputIterator,
  typename Predicate>
void alphaShape(const Shape& aShape, OutputIterator res, 
  const Predicate& aPredicate)
{
  OutputSensitiveAlphaShape<Shape, Predicate> ch(aShape, aPredicate);
  ch.all(res);
}

 ("output,o", po::value<int>()->default_value(1), "Output: No hull (=0), Alpha-Shape (=1), Convexe Hull(=2), both (=3)")
    ("time,t", po::value<bool>()->default_value(true), "Output: time (=true), or not (=false)")
    ("edgeVertices,ev", po::value<bool>()->default_value(true), "Take (=1), or not (=0) the vertices which lie on the edge")
    ("firstRadius,fr",  po::value<int>()->default_value(5), "First radius of the disc : rs^Rf" )
    ("lastRadius,lr",  po::value<int>()->default_value(20), "Last radius of the disc : rs^Rl" )
    ("stepRadius,rs",  po::value<int>()->default_value(2), "Increasing radius of disc : rs" )
    ("alphaCoefficient,k",  po::value<int>()->default_value(1000), "1/k : Alpha coefficient" )
    ("circleperRadius,mk",  po::value<int>()->default_value(100), "Number of circle per radius" );  

/**
 * @brief Procedure thar create a files with information : 
 * time, radius, predicate and alpha-shape and convex hull number of vertices
 * files is create in the folder outcome with the name of data-moy.txt. 
 *
 * @param aHull : Choose the ouput; No hull (=0), Alpha-Shape (=1), Convexe Hull(=2), both (=3).
 * @param aTime : Ouput computation time (=true) or not (=false).
 * @param aEdgeVertices : Take (=1), or not (=0) the vertices which lie on the edge
 * @param aFirstR : First radius of the disc : aradiusStep^aFirstR
 * @param aLastR : Last radius of the disc : aradiusStep^aLastR
 * @param aradiusStep : Increasing radius of disc : aradiusStep
 * @param akalpha : 1/Alpha coefficient; 1/akalpha * 1/-alpha = predicate radius.
 * @param aTestNb :Number of circle per radius
 * @return Standart ouput
 * 
 * @tparam
 */
void rToolMeans(int aHull, bool aTime, bool aEdgeVertices, int aFirstR, int aLastR, int aradiusStep, int akalpha, int aTestNb)
{
  typedef PointVector2D<int> Point; //type redefinition
  typedef PointVector2D<int> Vector; //type redefinition
  typedef RayIntersectableCircle<Point,  DGtal::BigInteger> Circle; 
  
  typedef std::chrono::time_point<std::chrono::system_clock> clock;
  typedef std::chrono::milliseconds milliseconds;
  typedef long long Integer;
  
  // Circle parameter : ax + by + c(x^2 + y^2)
  DGtal::BigInteger a; 
  DGtal::BigInteger b;
  DGtal::BigInteger c;
  DGtal::BigInteger d;
  c =  -25;

  // Init radius with R = aradiusStep^aRadiusmin.
  DGtal::BigInteger R =aradiusStep;
  for (int k = 1; k < aFirstR; k++)	{R *= aradiusStep;}

  DGtal::BigInteger myDen;
  DGtal::BigInteger myNum;

  // Circle
  Circle circle;

  // Predicate with infinite radius : Convex Hull 
  CircumcircleRadiusPredicate<> predicate0(1, 0);

  // Random Initialisation
  srand ( time(NULL) );

  // Save the first values for min and max.
  bool bfirst;

  
  // We want to keep some values : 
  // Min, max and means of execution time.
  // Min, max and means of convex hull, alpha-shape with infinite radius, and alphashape with 
  // proportional radius number of vertices.
  //  

  // Time
  clock ta, tb;
  Integer tmptime, time_min, time_max;
  double time_means;
  // Vertices number
  Integer tmpnb;
  Integer cv_min,  cv_max;  double cv_means;  // convex hull
  Integer as_min,  as_max;	double as_means;  // alpha-shape

  
    // Circle radius
    std::cout << "Radius|" << "\t";
    // Predicate
    std::cout << "predicate|" << "\t";
    if (aTime)
    {
      // Convex Hull computation time : Means, min, max
      std::cout << "time - means," << "\t"<< "time - min," << "\t"<< "time - max|" << "\t";
    } 
    if(aHull == 2 || aHull == 3)
    {
      // Vertices number of the convex hull : Means, min, max
      std::cout << "# Convexe Hull - means,"   << "\t"<< "# Convexe Hull - min,"   << "\t"
            << "# Convexe Hull - max|"   << "\t";
    }
    if(aHull == 1 || aHull == 3)
    {
      // Vertices number of the alpha-Shape : Means, min, max
      std::cout << "# Alpha-shape - means,"    << "\t"<< "# Alpha-shape - min,"    << "\t"
        << "# Alpha-shape - max";
    }
    std::cout << std::endl;
    
  // For a circle radius from aFirstR to aLastR (both include)  
  for (int j = aFirstR; j <= aLastR; j++)
  {
    // We take a radius for the predicate proportional to the radius of 
    // tha alpha-shape. R_alpha = 1/akalpha * myNum / myDen = akalpha *1/R^2 
    myDen = akalpha;
    myNum = R*R;

    // Predicate
    typedef CircumcircleRadiusPredicate<DGtal::BigInteger> Predicate; 
    Predicate predicate(myNum, myDen);


    // Reset values
    bfirst = true;
    time_min = 0; time_max = 0; time_means = 0.0; tmptime = 0;
    cv_min   = 0; cv_max   = 0; cv_means   = 0.0; tmpnb = 0;
    as_min   = 0; as_max   = 0; as_means   = 0.0;

    // Take a mean of aTestNb values.
    for (int i = 0; i < aTestNb; i++)
    {
       // We create a new circle from a, b, c, d : ax + by + c(x^2 + y^2)
       // The circle have a random center pt_c in [0;1]*[0;1] and a fixed radius = R
       // We fixed c = -20.  pt_c = [-a/2c ; -b/2c] and R² = (a^2 + b^2 - 4*c*d)/(4c²)
       // so 4c²*R² = a² + b² - 4*c*d <=>  = (a² + b² - 4c²*R²)/4*c = d
       
      a = - rand() %(2*c);
      b = - rand() %(2*c);  
      d = ( a*a + b*b - 4*R*R*c*c)/(4*c);

      // Create a circle from the Euclidian parameter a, b, c, d.
      Circle circle( a, b, c, d );	

      // Test
			//std::cout<<"a = "<<a<<", b = "<<b<<", c = "<<c<<", d = "<<d<<std::endl;   
			//std::cout<<"R : "<<R<<", "<< circle.getRadius()<<std::endl;

      // Convex Hull
      if(aHull == 2 || aHull == 3)
      {
        // chcv recover the number of vertices of the convex hull.
        std::vector<Point> chcv;
        
        if (aTime && aHull == 2) 
        {
          ta = std::chrono::system_clock::now();
          convexHull(circle, std::back_inserter(chcv));
          tb = std::chrono::system_clock::now();
          
          // Computation time
          tmptime = (tb - ta).count();
          if (tmptime <= time_min || bfirst == true) {time_min = tmptime;}
          if (tmptime >= time_max || bfirst == true) {time_max = tmptime;}
          time_means += tmptime/(double)aTestNb;  
        }
        else
          convexHull(circle, std::back_inserter(chcv));
          
        // Convex hull vertices number
        tmpnb = chcv.size();
        if (tmpnb <= cv_min || bfirst == true) {cv_min = tmpnb;}
        if (tmpnb >= cv_max || bfirst == true) {cv_max = tmpnb;}
        cv_means += tmpnb/(double)aTestNb;             
      }
      
      // Alpha-Shape
      if(aHull == 1 || aHull == 3)
      {
        // chas recover the number of vertices of the alpha-shape.
        std::vector<Point> chas;
        
        if (aTime)
        { 
          ta = std::chrono::system_clock::now();
          alphaShape( circle, std::back_inserter(chas), predicate);
          tb = std::chrono::system_clock::now();
          
          // Computation time
          tmptime = (tb - ta).count();
          if (tmptime <= time_min || bfirst == true) {time_min = tmptime;}
          if (tmptime >= time_max || bfirst == true) {time_max = tmptime;}
          time_means += tmptime/(double)aTestNb;
        }
        else
          alphaShape( circle, std::back_inserter(chas), predicate );
          
          
        // Alpha-Shape vertices number
        tmpnb = chas.size()-1;
        if (tmpnb <= as_min || bfirst == true) {as_min = tmpnb;}
        if (tmpnb >= as_max || bfirst == true) {as_max = tmpnb;}
        as_means += tmpnb/(double)aTestNb;
      }     

      // reset first min, max entries
      bfirst = false;
    } //aTestnb loop for a fixed radius

    // Circle radius
    std::cout << R << "\t";
    // Predicate
    std::cout << (DGtal::NumberTraits<BigInteger>::castToDouble(myNum) / 
      DGtal::NumberTraits<BigInteger>::castToDouble(myDen)) << "\t";
    if (aTime)
    {
      // time : Means, min, max
      std::cout << time_means << "\t" << time_min<< "\t" << time_max << "\t"; 
    } 
    if(aHull == 2 || aHull == 3)
    {
      // Concex Hull : Means, min, max
      std::cout << cv_means << "\t" << cv_min<< "\t" << cv_max << "\t";
    }
    if(aHull == 1 || aHull == 3)
    {
      // Alpha-Shape : Means, min, max
      std::cout << as_means << "\t" << as_min<< "\t" << as_max;
    }
     std::cout << std::endl;
    // Restart with a radius increase by 4.
    R = R*aradiusStep; 
  }// end loop - R reach its maximum asked aRadiusmax
}

///////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
  po::options_description general_opt("Allowed options are: ");
  general_opt.add_options()
    ("help,h", "display this message")
    ("output,o", po::value<int>()->default_value(1), "Output: 0: No hull, 1:Alpha-Shape, 2:Convexe Hull, 3: both")
    ("time,t", po::value<bool>()->default_value(true), "Output: True: Time, Fasle: No time")
    ("edgeVertices,ev", po::value<bool>()->default_value(true), "Take :1, or not :0 the vertices which lie on the edge")
    ("firstRadius,fr",  po::value<int>()->default_value(5), "First radius of the disc : 2^Rf" )
    ("lastRadius,lr",  po::value<int>()->default_value(20), "Last radius of the disc : 2^Rl" )
    ("stepRadius,rs",  po::value<int>()->default_value(2), "Radius of the disc : r" )
    ("alphaCoefficient,k",  po::value<int>()->default_value(1000), "1/k : Alpha coefficient" )
    ("circleperRadius,mk",  po::value<int>()->default_value(100), "Number of circle per radius" );   
  
  bool parseOK=true;
  po::variables_map vm;
  try{
    po::store(po::parse_command_line(argc, argv, general_opt), vm);  
  }catch(const std::exception& ex){
    parseOK=false;
    trace.info()<< "Error checking program options: "<< ex.what()<< std::endl; 
  }
  po::notify(vm);    
  if(!parseOK || vm.count("help")||argc<=1)
    {
      trace.info()<< "Display the number of vertices depending to the radius of the disk" 
                  <<std::endl << "Basic usage: "<<std::endl
		  << "\t toolAlphaShape -o 1 -fr 5 -lr 8 > files.txt" << std::endl
		  << general_opt << "\n";
      return 0;
    }
    
  int hull = vm["output"].as<int>();
  
  bool time = vm["time"].as<bool>();
  bool edgeVertices = vm["edgeVertices"].as<bool>();
  
  int rf = vm["firstRadius"].as<int>();
  int rl = vm["lastRadius"].as<int>();
  int rs = vm["stepRadius"].as<int>();

  int k = vm["alphaCoefficient"].as<int>();
  int mk = vm["circleperRadius"].as<int>();

  
  //2^5 = 32, 2^15 = 32768, 2^25 = 16777216
  rToolMeans(hull, time, edgeVertices, rf, rl, rs, k, mk);
  
  //rToolValue(16777216);


  return 0;
}
