///////////////////////////////////////////////////////////////////////////////
//requires STL
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

//requires C++ 0x ou 11
#include <chrono>

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
//#include "../inc/PointVector2D.h"
#include "../inc/ExactRayIntersectableCircle.h"
#include "../inc/OutputSensitiveConvexHull.h"
#include "../inc/IncrementalNegativeAlphaShape.h"
#include "../inc/BottomUpPositiveAlphaShape.h"
#include "../inc/ConvexHullHelpers.h"
#include "../inc/CircumcircleRadiusPredicate.h"

typedef PointVector<2,int> Point; //DGtal point redefinition
typedef PointVector<2,int> Vector; //DGtal point redefinition
typedef ExactRayIntersectableCircle<Point,DGtal::BigInteger> Circle; //Circle
typedef CircumcircleRadiusPredicate<DGtal::BigInteger> Predicate;
typedef std::deque<Point> Container; 
///////////////////////////////////////////////////////////////////////////////
template <typename ForwardIterator>
void display(const ForwardIterator& itb, const ForwardIterator& ite, 
	     std::string aFilename)
{
  typedef PointVector<2,int> DGtalPoint; 

  DGtal::Board2D aBoard;

  ForwardIterator it = itb; 
  if (it != ite)
    {//for the first point
      DGtalPoint p1(it->operator[](0), it->operator[](1));
      DGtalPoint p = p1;
      //display
      aBoard << SetMode( p.className(), "Grid" )
	     << p; 
       
      DGtalPoint prev = p; 
      for (++it; it != ite; ++it, prev = p)
	{//for all other points
	  //conversion
	  p = DGtalPoint(it->operator[](0), it->operator[](1));
	  //display
	  aBoard << p; 
	  aBoard.drawArrow(prev[0], prev[1], p[0], p[1]); 
	}

      //display the last edge too
      aBoard.drawArrow(prev[0], prev[1], p1[0], p1[1]); 
    }
  
  aFilename.insert(aFilename.size(), ".eps"); 
  aBoard.saveEPS(aFilename.c_str());

}

///////////////////////////////////////////////////////////////////////////////
// Comput Convex Hull
  template <typename Shape>
void convexHull(const Shape& aShape, std::vector<Point> res, bool aVertOnEdges)
{
  //computation of the alpha-shape
  std::chrono::time_point<std::chrono::system_clock> ta, tb;
  ta = std::chrono::system_clock::now();

  OutputSensitiveConvexHull<Shape> ch(aShape);
  ch.all(std::back_inserter(res), aVertOnEdges);

  tb = std::chrono::system_clock::now();
  
  std::cout << "# Time: "
    << std::chrono::duration_cast<std::chrono::microseconds>(tb-ta).count()
    << " ms " << std::endl;
  std::cout << "# Numb: "
    << res.size()
    << " vertices " << std::endl;

  //display in the standard output
  std::cout << "Convex Hull"<< std::endl;
  std::copy(res.begin(), res.end(), std::ostream_iterator<Point>(std::cout, ", ") );
  std::cout << std::endl;

  //display in an output file
  std::string filename = "OutputSensitiveConvexHull";
  display(res.begin(), res.end(), filename);
  std::cout << "# Display stored in file "
    << filename << ".eps" << std::endl;

}

///////////////////////////////////////////////////////////////////////////////
// Output Sensitive Alpha-Shape, alpha <= 0
template <typename Shape, typename Predicate>
void NegAlphaShape(const Shape& aShape, std::vector<Point> res, const Predicate& aPredicate)
{
  typedef typename Circle::Point Point;
  typedef typename Circle::Vector Vector;

  std::cout << "output sensitive proc" << std::endl;

  //computation of the alpha-shape
  std::chrono::time_point<std::chrono::system_clock> ta, tb;
  ta = std::chrono::system_clock::now();

  IncrementalNegativeAlphaShape<Shape, Predicate> nas(aShape, aPredicate);
  nas.all(std::back_inserter(res));

  tb = std::chrono::system_clock::now();
  std::cout << "# Time: "
    << std::chrono::duration_cast<std::chrono::microseconds>(tb-ta).count()
    << " ms " << std::endl;
  std::cout << "# Numb: "
    << res.size()
    << " vertices " << std::endl;

  //display in the standard output
  std::cout << "Output-sensitive (-"
    << ( aPredicate.getNum2() / (double)aPredicate.getDen2() )
    << ")-shape " << std::endl;
  std::copy(res.begin(), res.end(), std::ostream_iterator<Point>(std::cout, ", ") );
  std::cout << std::endl;

  //display in an output file
  std::string filename = "OutputSensitiveAlphaShape";
  display(res.begin(), res.end(), filename);
  std::cout << "# Display stored in file "
    << filename << ".eps" << std::endl;
    
}

//////////////////////////////////////////////////////////////////////
// Alpha-Shape, alpha > Rmin
  template <typename Shape, typename Point, typename Container, typename Predicate>
void PosAlphaShape(const Shape& aShape, const Point& aStart, Container& res, const Predicate& aPredicate)
{

  //computation of the alpha-shape
  std::chrono::time_point<std::chrono::system_clock> ta, tb;
  ta = std::chrono::system_clock::now();
  
  BottomUpPositiveAlphaShape<Shape, Predicate> pas(aShape, aPredicate);
  pas.all(res, aStart);

  tb = std::chrono::system_clock::now();
  std::cout << "# Time: "
    << std::chrono::duration_cast<std::chrono::microseconds>(tb-ta).count()
    << " ms " << std::endl;
  std::cout << "# Numb: "
    << res.size()
    << " vertices " << std::endl;

  //display in the standard output
  std::cout << "("
    << ( aPredicate.getNum2() / (double)aPredicate.getDen2() )
    << ")-shape " << std::endl;
  std::copy(res.begin(), res.end(), std::ostream_iterator<Point>(std::cout, ", ") );
  std::cout << std::endl;

  //display in an output file
  std::string filename = "PositiveAlphaShape";
  display(res.begin(), res.end(), filename);
  std::cout << "# Display stored in file "
    << filename << ".eps" << std::endl;


}


///////////////////////////////////////////////////////////////////////////////
// Main Procedure///////////////////////////////////////////////////////////////////////////////
  template<typename Shape, typename Point, typename Predicate>
void mainProcedure(const Shape& aShape, const Point& aStart, const Predicate& aPredicate,
    const bool& aVertOnEdges, const int& methodId)
{
  if (methodId == 1)
  {
    std::vector<Point> res;
    convexHull(aShape, res, aVertOnEdges);
  }
  else
  {
    if (methodId == 3)
    {
      Container container;

      PosAlphaShape( aShape, aStart, container, aPredicate );
    }
    else
    {
      std::vector<Point> res;
      NegAlphaShape(aShape, (res), aPredicate);
    }
  }
}



///////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
// parse command line ----------------------------------------------
  po::options_description general_opt("Allowed options are: ");
  general_opt.add_options()
    ("help,h", "display this message")
    ("algorithm,a,", po::value<int>()->default_value(1), "algorithm: Convex-Hull (=1), Negative Alpha-Shape (=2) or Positive Alpha-Shape (=3)")
    ("numerator2,n", po::value<int>()->default_value(1), "squared numerator of 1/alpha")
    ("denominator2,m", po::value<int>()->default_value(0), "squared denominator of 1/alpha")
    ("edgeVertices,ev", po::value<bool>()->default_value(false), "Take (true), or not (true) the vertices which lie on the edge")
    ("radius,R", po::value<int>(), "Radius of the disc" )
    ("a", po::value<int>(), "a parameter of the disc" )
    ("b", po::value<int>(), "b parameter of the disc" )
    ("c", po::value<int>(), "c parameter of the disc" )
    ("d", po::value<int>(), "d parameter of the disc" )
    ("x1", po::value<int>(), "x-coordinate of the first point" )
    ("y1", po::value<int>(), "y-coordinate of the first point" )
    ("x2", po::value<int>(), "x-coordinate of the second point" )
    ("y2", po::value<int>(), "y-coordinate of the second point" )
    ("x3", po::value<int>(), "x-coordinate of the third point" )
    ("y3", po::value<int>(), "y-coordinate of the third point" );

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
    trace.info()<< "Display alpha-shape of grid points lying inside the specified disc"
      <<std::endl << "Basic usage: "<<std::endl
      << "\ttoolDisplay -R 15" << std::endl
      << general_opt << "\n";
    return 0;
  }
 
 
  int methodId = vm["algorithm"].as<int>();
  int num2 = vm["numerator2"].as<int>();
  int den2 = vm["denominator2"].as<int>();

  bool edgeVertices = vm["edgeVertices"].as<bool>();
  
  bool positive = (methodId == 3);
   
  if (vm.count("radius"))
  { //if radius option specified
    int radius = vm["radius"].as<int>();
    std::cout << "Disc of radius " << radius << std::endl;

  
    Circle circle( Point(radius,0), Point(0,radius), Point(-radius,0) );
    CircumcircleRadiusPredicate<> predicate(num2, den2, positive);

    mainProcedure( circle, Point(0,-radius), predicate, edgeVertices, methodId );
  }
  else
  {
    if ( vm.count("x1") &&
        vm.count("y1") &&
        vm.count("x2") &&
        vm.count("y2") &&
        vm.count("x3") &&
        vm.count("y3") )
    {
      //if point options specified
      std::cout << "Display disc passing through 3 points " << std::endl;
      Point p, q, r;
      p[0] = vm["x1"].as<int>();
      p[1] = vm["y1"].as<int>();
      q[0] = vm["x2"].as<int>();
      q[1] = vm["y2"].as<int>();
      r[0] = vm["x3"].as<int>();
      r[1] = vm["y3"].as<int>();

      Point pStart = p;
      
      //orientation test
      Vector u = p-q;
      Vector v = r-q;
      if ( (u[0]*v[1] - u[1]*v[0]) >= 0)
      {
        std::cerr << "Your three points should be in a counter-clockwise order (and not collinear)" << std::endl;
        return 1;
      }

      Circle circle( p, q, r );
      CircumcircleRadiusPredicate<> predicate(num2, den2, positive);

      mainProcedure( circle, p, predicate, edgeVertices, methodId );
      
    }
    else
    {
      if ( vm.count("a") &&
          vm.count("b") &&
          vm.count("c") &&
          vm.count("d") )
      {
        int a = vm["a"].as<int>();
        int b = vm["b"].as<int>();
        int c = vm["c"].as<int>();
        int d = vm["d"].as<int>();
        std::cout << "Disc of parameters "
          << a << " "
          << b << " "
          << c << " "
          << d << std::endl;

        //orientation test
        if ( ( c >= 0 )&&( d >= c) )
        {
          std::cerr << "Parameter c should be strictly negative (0 = straight line; >0 = bad orientation)" << std::endl;
          return 1;
        }
        
        Circle circle( a, b, c, d );
        CircumcircleRadiusPredicate<> predicate(num2, den2, positive);
       
        mainProcedure( circle, circle.getConvexHullVertex(), predicate, edgeVertices, methodId );
         
      }
      else
        std::cerr << "Bad input arguments. Try option --help. " << std::endl;
    }

  }

  return 0;
}

