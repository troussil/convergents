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

/////////////////////////////////////////////////////////////////////////////
// Class OutputIteratorCounter
/**
 * \brief Aim: This model of output iterator
 * counts the number of times data are copied.
 */
class OutputIteratorCounter:
  public std::iterator<std::output_iterator_tag,void,void,void,void>
{
  
public:
  /**
   * Constructor.
   * @param aCounterPtr a pointer to a counter
   */
  OutputIteratorCounter(int* aCounterPtr)
    : myCounterPtr(aCounterPtr) {}

  /**
   * Copy Constructor.
   * @param other another instance of the same class
   */
  OutputIteratorCounter(const OutputIteratorCounter& other)
    : myCounterPtr(other.myCounterPtr) {}

  // ----------------------- Interface --------------------------------------
public:

  /**
   * Assignment operator.
   * @a myCounter is incremented.
   * @param aData any input data
   * @return a reference to *this
   */
  template<typename Data>
  OutputIteratorCounter& operator=(const Data& /*aData*/) 
  { 
    *myCounterPtr += 1; 
    return *this; 
  }

  /**
   * Dereference operator.
   * Does nothing.
   * @return a reference to *this
   */
  OutputIteratorCounter& operator*() 
  { 
    return *this; 
  }

  /**
   * Pre-increment operator.
   * Does nothing.
   * @return a reference to *this
   */
  OutputIteratorCounter& operator++() 
  { 
    return *this; 
  }

  /**
   * Post-increment operator.
   * Does nothing.
   * @return a reference to *this
   */
  OutputIteratorCounter& operator++(int) 
  { 
    return *this; 
  }

  /**
   * Accessor.
   * @return the current state of the counter
   */
  int get() const 
  {
    return (*myCounterPtr); 
  }

  // ------------------------- Private Datas --------------------------------
private:
  /**
   * pointer to an integral counter
   */
  int* myCounterPtr;  

}; // end of class OutputIteratorCounter

 
///////////////////////////////////////////////////////////////////////// 
template <typename Shape, typename OutputIterator>
void convexHull(const Shape& aShape, OutputIterator res, bool aVertOnEdges)
{
  OutputSensitiveConvexHull<Shape> ch(aShape); 
  ch.all(res, aVertOnEdges);
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
  DGtal::BigInteger R = aradiusStep;
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
  // Min, max and average of execution time.
  // Min, max and average of convex hull, alpha-shape with infinite radius, and alphashape with 
  // proportional radius number of vertices.
  //  

  // Time
  clock ta, tb;
  Integer tmptime;
  double time_min, time_max, time_average;
  // Vertices number
  Integer tmpnb;
  Integer cv_min,  cv_max;  double cv_average;  // convex hull
  Integer as_min,  as_max;	double as_average;  // alpha-shape
  
  // Number of vertices divide by radius^(2/3)
  double tmpvdr;
  double vdrcv_min, vdrcv_max, vdrcv_average;  // convex hull
  double vdras_min, vdras_max, vdras_average;  // convex hull
  

  // Circle radius
  std::cout << "RADIUS|" << "\t";
  if(aHull == 1 || aHull == 3)
    {
      // Predicate
      std::cout << "PREDICATE|" << "\t";
    }
  if (aTime)
    {
      // Convex Hull computation time : average, min, max
      std::cout << "TIME - average," << "\t"<< " min," << "\t"<< " max|" << "\t";
    } 
  if(aHull == 2 || aHull == 3)
    {
      // Vertices number of the convex hull : average, min, max
      std::cout << "# CONVEX HULL - average,"   << "\t"<< " min,"   << "\t"
		<< " max|"   << "\t";
      
      // Vertices number of the convex hull divide by the radius^(2/3) : average, min, max
      std::cout << "# CONVEX HULL - CH / r^(2/3) - average,"   << "\t"<< " min,"   << "\t"
		<< " max|"   << "\t";  
    }
  if(aHull == 1 || aHull == 3)
    {
      // Vertices number of the alpha-Shape : average, min, max
      std::cout << "# ALPHA-SHAPE - average,"    << "\t"<< " min,"    << "\t"
		<< " max|";
    
      // Vertices number of the alpha-shape divide by the radius^(2/3) : average, min, max
      std::cout << "# ALPHA-SHAPE - AS / r^(2/3) - average,"   << "\t"<< " min,"   << "\t"
		<< " max|"   << "\t";
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
      time_min = 0; time_max = 0; time_average = 0.0; tmptime = 0;
      cv_min   = 0; cv_max   = 0; cv_average   = 0.0; tmpnb = 0;
      as_min   = 0; as_max   = 0; as_average   = 0.0;
    
      tmpvdr = 0.0;
      vdrcv_min= 0.0; vdrcv_max= 0.0; vdrcv_average= 0.0;  
      vdras_min= 0.0; vdras_max= 0.0; vdras_average= 0.0;
  

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
	      int verticesCounter = 0; 
	      OutputIteratorCounter counter(&verticesCounter); 

	      if (aTime && aHull == 2) 
		{
		  ta = std::chrono::system_clock::now();
		  convexHull(circle, counter, aEdgeVertices);
		  tb = std::chrono::system_clock::now();

		  // Computation time
		  tmptime = (tb - ta).count();
		  if ((double)tmptime <= time_min || bfirst == true) {time_min = (double)tmptime/1000;}
		  if ((double)tmptime >= time_max || bfirst == true) {time_max = (double)tmptime/1000;}
		  time_average += tmptime/((double)aTestNb * 1000);
		}
	      else
		convexHull(circle, counter, aEdgeVertices);

	      // Convex hull vertices number
	      tmpnb = counter.get();
	      if (tmpnb <= cv_min || bfirst == true) {cv_min = tmpnb;}
	      if (tmpnb >= cv_max || bfirst == true) {cv_max = tmpnb;}
	      cv_average += tmpnb/(double)aTestNb;
        
	      tmpvdr = tmpnb/pow(DGtal::NumberTraits<BigInteger>::castToDouble(R), 2/(double) 3);
	      if (tmpvdr <= vdrcv_min || bfirst == true) {vdrcv_min = tmpvdr;}
	      if (tmpvdr >= vdrcv_max || bfirst == true) {vdrcv_max = tmpvdr;}
	      vdrcv_average += tmpvdr/aTestNb;        

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
		  if ((double)tmptime <= time_min || bfirst == true) {time_min = (double)tmptime/1000;}
		  if ((double)tmptime >= time_max || bfirst == true) {time_max = (double)tmptime/1000;}
		  time_average += tmptime/((double)aTestNb * 1000);
		}
	      else
		alphaShape( circle, std::back_inserter(chas), predicate );


	      // Alpha-Shape vertices number
	      tmpnb = chas.size();
	      if (tmpnb <= as_min || bfirst == true) {as_min = tmpnb;}
	      if (tmpnb >= as_max || bfirst == true) {as_max = tmpnb;}
	      as_average += tmpnb/(double)aTestNb;
        
	      tmpvdr = tmpnb/pow(DGtal::NumberTraits<BigInteger>::castToDouble(R), 2/(double) 3);
	      if (tmpvdr <= vdras_min || bfirst == true) {vdras_min = tmpvdr;}
	      if (tmpvdr >= vdras_max || bfirst == true) {vdras_max = tmpvdr;}
	      vdras_average += tmpvdr/aTestNb;  
	    }     

	  // reset first min, max entries
	  bfirst = false;
	} //aTestnb loop for a fixed radius

      // Circle radius
      std::cout << R << "\t";
      if(aHull == 1 || aHull == 3)
	{   // Predicate
	  std::cout << (DGtal::NumberTraits<BigInteger>::castToDouble(myNum) / 
			DGtal::NumberTraits<BigInteger>::castToDouble(myDen)) << "\t";
	}
      if (aTime)
	{
	  // time : average, min, max
	  std::cout << time_average << "\t" << time_min<< "\t" << time_max << "\t"; 
	} 
      if(aHull == 2 || aHull == 3)
	{
	  // Concex Hull : average, min, max
	  std::cout << cv_average << "\t" << cv_min<< "\t" << cv_max << "\t"   
		    << vdrcv_average << "\t" << vdrcv_min<< "\t" << vdrcv_max << "\t";
   
	}
      if(aHull == 1 || aHull == 3)
	{
	  // Alpha-Shape : average, min, max
	  std::cout << as_average << "\t" << as_min<< "\t" << as_max << "\t"
		    << vdras_average << "\t" << vdras_min<< "\t" << vdras_max << "\t";
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
    ("output,o", po::value<int>()->default_value(1), "Alpha-Shape (=1), Convexe Hull (=2), both (=3)")
    ("time,t", po::value<bool>()->default_value(true), "Output: time (=true), or not (=false)")
    ("edgeVertices,ev", po::value<bool>()->default_value(false), "Take (true), or not (true) the vertices which lie on the edge")
    ("firstRadius,f",  po::value<int>()->default_value(5), "First radius of the disc : s^f" )
    ("lastRadius,l",  po::value<int>()->default_value(20), "Last radius of the disc : s^l" )
    ("stepRadius,s",  po::value<int>()->default_value(2), "Increasing radius of disc : s" )
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
		  << "\t toolAlphaShape -o 1 -f 5 -l 8 > files.txt" << std::endl
		  << general_opt << "\n";
      return 0;
    }

  // retrieve values from boost - po
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

  return 0;
}
