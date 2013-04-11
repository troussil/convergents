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
#include "../inc/PointVector2D.h"
#include "../inc/RayIntersectableCircle.h"
#include "../inc/OutputSensitiveConvexHull.h"
#include "../inc/ConvexHullHelpers.h"

///////////////////////////////////////////////////////////////////////////////
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
template<typename Circle>
void myMainProcedure(const Circle& aCircle)
{
  typedef typename Circle::Point Point; 
  typedef typename Circle::Vector Vector; 

  //computation of the boundary and convex hull
  std::chrono::time_point<std::chrono::system_clock> ta, tb;
  ta = std::chrono::system_clock::now();

  std::vector<Point> boundary; 
  Vector dir(1,0); 
  closedTracking( aCircle, aCircle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 

  std::vector<Point> ch; 
  grahamScan( boundary.begin(), boundary.end(), std::back_inserter(ch) ); 

  tb = std::chrono::system_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::microseconds>(tb-ta).count() << std::endl;  


  //display in the standard output
  std::cout << "Graham's convex hull of the boundary" << std::endl; 
  std::copy(ch.begin(), ch.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
  std::cout << std::endl; 
  
  //display in an output file
  display(ch.begin(), ch.end(), "convexHullByTracking"); 

}

///////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
  // parse command line ----------------------------------------------
  po::options_description general_opt("Allowed options are: ");
  general_opt.add_options()
    ("help,h", "display this message")
    ("radius,R",  po::value<int>(), "Radius of the disc" )
    ("a,a",  po::value<int>(), "x-coordinate of the first point" )
    ("b,b",  po::value<int>(), "y-coordinate of the first point" )
    ("c,c",  po::value<int>(), "x-coordinate of the second point" )
    ("d,d",  po::value<int>(), "y-coordinate of the second point" )
    ("e,e",  po::value<int>(), "x-coordinate of the third point" )
    ("f,f",  po::value<int>(), "y-coordinate of the third point" );
  
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
      trace.info()<< "Display convex hull of grid points lying inside the specified disc" 
                  <<std::endl << "Basic usage: "<<std::endl
		  << "\ttoolDisplay -R 15" << std::endl
		  << general_opt << "\n";
      return 0;
    }
 

  typedef PointVector2D<int> Point; //DGtal point redefinition
  typedef PointVector2D<int> Vector; //DGtal point redefinition
  // does not work (lacking operators)
  // typedef PointVector<2,int> Point; //DGtal point redefinition
  // typedef PointVector<2,int> Vector; //DGtal point redefinition
  typedef RayIntersectableCircle<Point> Circle; //Circle


  if (vm.count("radius"))
    { //if radius option specified
      int radius = vm["radius"].as<int>();
      std::cout << "Display disc of radius " << radius << std::endl; 

      Circle circle( Point(radius,0), Point(0,radius), Point(-radius,0) );

      myMainProcedure( circle ); 
    }
  else 
    { 
      if ( vm.count("a") && 
	   vm.count("b") &&
	   vm.count("c") &&
	   vm.count("d") && 
	   vm.count("e") &&
	   vm.count("f") )
	{
	  //if point options specified
	  std::cout << "Display disc passing through 3 points " << std::endl;
	  Point p, q, r; 
	  p[0] = vm["a"].as<int>();
	  p[1] = vm["b"].as<int>();
	  q[0] = vm["c"].as<int>();
	  q[1] = vm["d"].as<int>();
	  r[0] = vm["e"].as<int>();
	  r[1] = vm["f"].as<int>();

	  Circle circle( p, q, r );

	  myMainProcedure( circle ); 
	}
      else
	std::cerr << "Bad input arguments. Try option --help. " << std::endl; 
    }

  return 0;
}
