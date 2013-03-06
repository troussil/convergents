///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"

using namespace DGtal;
namespace po = boost::program_options;


///////////////////////////////////////////////////////////////////////////////
#include "PointVector2D.h"
#include "RayIntersectableCircle.h"
#include "OutputSensitiveConvexHull.h"

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
 

  typedef PointVector<2,int> Point; //DGtal point redefinition
  typedef PointVector<2,int> Vector; //DGtal point redefinition
  typedef RayIntersectableCircle<Point> Circle; //Circle


      if (vm.count("radius"))
        { //if radius option specified
          int radius = vm["radius"].as<int>();
          std::cout << "Display disc of radius " << radius << std::endl; 


    Circle circle( Point(radius,0), Point(0,radius), Point(-radius,0) );

    std::vector<Point> boundary; 
    Vector dir(1,0); 
//    tracking( circle, circle.getConvexHullVertex(), dir, std::back_inserter(boundary) ); 
    std::cout << "Boundary" << std::endl; 
    std::copy(boundary.begin(), boundary.end(), std::ostream_iterator<Point>(std::cout, ", ") ); 
    std::cout << std::endl;     

        }
     else 
        { //if point options specified
          std::cout << "Display disc of passing through 3 points " << std::endl; 

        }

  return 0;
}
