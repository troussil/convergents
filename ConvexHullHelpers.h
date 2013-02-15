//////////////////////////////////////////////////////////////////////
////////////////// tracking /////////////////////////////////////////
////////////////////////////////////////////////////////////////////
/**
 * @brief Class implementing a contour tracking routine, which
 * retrieves the point of the digital boundary following
 * a given point in a given direction. 
 * 
 * @tparam TShape a point functor (positive value for 
 * a point belonging to the shape).   
 */
template <typename TShape>
class Tracker
{
  public: 
    /////////////////////// inner types /////////////////
    typedef TShape Shape; 
    typedef typename Shape::Point Point; 

  private: 
    /////////////////////// members /////////////////////
    /**
     * const reference on a shape
     */
    const Shape& myShape;  

  public:
    ///////////////////// standard services /////////////
    /**
     * Standard constructor
     * @param aShape
     */
    Tracker(const Shape& aShape)
      : myShape(aShape) {}

  private:
    /**
     * Copy constructor
     * @param other other object to copy
     */
    Tracker(const Tracker& other) {}

    /**
     * Assignement operator
     * @param other other object to copy
     * @return reference on *this
     */
    Tracker& operator=(const Tracker& other) 
    { return *this; }

  public: 
    /**
     * Default destructor
     */
    ~Tracker() {}


    ///////////////////// main methods ///////////////////
    /**
     * Given a point of the digital boundary, find the next
     * point in a counter-clockwise order
     * @param aPoint any point of the digital boundary
     * @param aDir returned tracking direction
     * @return the next point (can be the same as @a aPoint)
     *
     * @tparam Vector a model of vector
     */
    template<typename Vector>
      Point next(const Point& aPoint, Vector& aDir)
      {
        Point next; //next point to return 
        Vector shift; 
        if (aDir[0] == 0)
          shift = Vector(aDir[1], aDir[0]);
        else 
          shift = Vector(aDir[1], -aDir[0]);
        Point out = aPoint + shift;
        // std::cout << aPoint << " " << out << " " << out + aDir << std::endl; 
        if ( myShape(out) < 0 )
        { //if out is outside
          if ( myShape(out + aDir) >= 0 )
          { //if out + aDir is inside
            // std::cout << out + aDir << " is inside " << std::endl; 
            next = out + aDir; 
            // std::cout << next << " is next " << std::endl; 
            aDir = shift; 
          }
          else 
          { //otherwise
            //      std::cout << out + aDir << " is outside " << std::endl; 
            if ( myShape(aPoint + aDir) >= 0 )
            { //if aPoint + aDir is inside
              next = aPoint + aDir; 
            }
            else
            { //otherwise
              next = aPoint; 
              aDir = -shift; 
            }
          }
        }
        else
        {
          std::cerr << "Error in next of Tracker: " << aPoint 
            << " is not a point of the digital boundary " 
            << std::endl; 
          next = aPoint; 
        }
        // std::cout << next << " is next " << std::endl; 
        return next; 
      }
}; 

/**
 * @brief Procedure that retrieves the boundary of 
 * the Gauss digitization of a shape. 
 * 
 * @param aShape shape we want to track the boundary
 * of its Gauss digitization
 * @param aStartingPoint point belonging to the 
 * boundary of the Gauss digitization of @a aShape
 * @param aDir vector indicating the tracking orientation
 * @param res output iterator using to export the retrieved points
 * 
 * @tparam TShape a point functor (positive value for 
 * a point belonging to the shape).
 * @tparam Point a model of point
 * @tparam Vector a model of vector
 * @tparam OutputIterator a model of output iterator   
 */
template <typename Shape, typename Point, typename Vector, typename OutputIterator>
void tracking(const Shape& aShape, const Point& aStartingPoint, Vector& aDir,  
    OutputIterator res)
{
  Tracker<Shape> t(aShape);
  //get the first point
  Point current = aStartingPoint; 
  Point tmp; 
  do {
    //store the current point
    *res++ = current; 
    //get the next DIFFERENT point
    do {
      tmp = t.next(current, aDir);
    } while (tmp == current); 
    current = tmp; 
    //while it is not the first one
  } while (current != aStartingPoint); 
}

//////////////////////////////////////////////////////////////////////
////////////////// convex hull, alpha-shape /////////////////////////
////////////////////////////////////////////////////////////////////

/**
 * @brief Class implementing a point predicate,
 * which returns true if the signed area of the triangle
 * made up with three given points is positive, ie. the
 * three points are in a counter-clockwise orientation
 *
 * This class is a model of ternary predicate.
 */
struct StraightLinePredicate
{
  /**
   * Given three points, returns their orientation. 
   *
   * @param a first point
   * @param b second point
   * @param c third point
   * @return true if the signed area of the triangle is positive
   *
   * @tparam Point a model of point.   
   */
  template<typename Point>
  bool
  operator()(const Point& a, const Point& b, const Point& c) const
  {
    return ( a[0]*(b[1] - c[1]) - b[0]*(a[1] - c[1]) + c[0]*(a[1] - b[1]) ) >= 0;
  }
}; 

/**
 * @brief Class implementing a point predicate,
 * which compares a stored radius with the radius 
 * of the circle passing through three given points. 
 * It returns true if stored radius is greater than
 * the computed one. 
 *
 * This class is a model of ternary predicate.
 */
struct RadiusCirclePredicate
{
  public: 
    /* type of parameters */
    typedef long long Integer;
 
  private: 
    /////////////////////// members /////////////////////
    /**
     * The radius is viewed as the fraction of two squares, 
     * R = myNum2 / myDen2
     */
  Integer myNum2; 
  Integer myDen2; 


  public:
    ///////////////////// standard services /////////////
    /**
     * Standard constructor
     * @param aNum2 squared numerator of the radius (default 1)
     * @param aDen2 squared denominator of the radius (default 0)
     *
     * NB: Default values stands for an infinite radius. 
     */
  RadiusCirclePredicate(const Integer& aNum2 = 1, const Integer& aDen2 = 0)
    : myNum2(aNum2), myDen2(aDen2) {}

  private:
    /**
     * Copy constructor
     * @param other other object to copy
     */
    RadiusCirclePredicate(const RadiusCirclePredicate& other) {}

    /**
     * Assignement operator
     * @param other other object to copy
     * @return reference on *this
     */
    RadiusCirclePredicate& operator=(const RadiusCirclePredicate& other) 
    { return *this; }

  public: 
    /**
     * Default destructor
     */
    ~RadiusCirclePredicate() {}


  ///////////////////// main methods ///////////////////
  /**
   * Given three points, returns the area of the parallegram. 
   *
   * @param a first point
   * @param b second point
   * @param c third point
   * @return signed area of the parallelogram
   *
   * @tparam Point a model of point.   
   */
  template<typename Point>
  Integer 
  getArea(const Point& a, const Point& b, const Point& c) const
  {
    return a[0]*(b[1] - c[1]) - b[0]*(a[1] - c[1]) + c[0]*(a[1] - b[1]);
  }

  /**
   * Given three points, returns the predicate value. 
   *
   * @param a first point
   * @param b second point
   * @param c third point
   * @return 'true' if the radius of the circle abc is greater 
   * that the stored radius equal to @a myNum2 / @a myDen2
   *
   * @tparam Point a model of point.   
   */
  template<typename Point>
  bool
  operator()(const Point& a, const Point& b, const Point& c) const
  {
    Point ab = (b-a); 
    Point bc = (c-b); 
    Point ac = (c-a);
    Integer nab = ab.normL22(); 	
    Integer nbc = bc.normL22(); 
    Integer nac = ac.normL22(); 
    Integer rightPart = nab*nbc*nac*myDen2;

    Integer area = getArea(a,b,c);
    int signArea;
    if (area >= 0) signArea = 1;
    else signArea = -1;
    Integer leftPart = (4*area*area*signArea*myNum2);
    return leftPart >= rightPart;
  }
}; 

/**
 * @brief Procedure that update the convex hull when
 * considering an extra point P, added to the end of a
 * sorted list of points, ie. sets of three consecutive
 * vertices P, Vn, Vn-1 should be in a clockwise orientation.
 * If not, n is decremented. 
 * @see grahamScan   
 * 
 * @param container (returned) stack-like container
 * @param newPoint new point
 * @param p predicate giving the orientation
 * (true if in a counter-clockwise orientation) 
 * 
 * @tparam Container a model of 'back-pushable' sequence container 
 * @tparam Point a model of points    
 * @tparam Predicate a model of ternary predicate
 */
template <typename Container, typename Point, typename Predicate>
void updateConvexHull(Container& container, const Point& newPoint, const Predicate& p)
{
  Point Q = container.back(); 
  container.pop_back(); 
  if (container.size() != 0) 
    {
      Point R = container.back(); 
      //std::cout << " signed area of " << newPoint << " " << Q << " " << R << " : " << signedArea(newPoint,Q,R) << std::endl; 
      while ( ( p(newPoint,Q,R) )&&(container.size() != 0) )
	{
	  //remove Q
	  //std::cout << " remove from back" << Q << std::endl; 
	  Q = R; 
	  container.pop_back(); 
	  if (container.size() != 0) 
	    R = container.back(); 
	}
      //add Q
      container.push_back(Q); 
    }
}

/**
 * @brief Procedure that retrieves the vertices
 * of the convex hull of a sorted list of 2d points
 * (Graham's scan).  
 * 
 * @param itb begin iterator
 * @param ite end iterator 
 * @param res output iterator using to export the retrieved points
 * 
 * @tparam ForwardIterator a model of forward iterator
 * @tparam OutputIterator a model of output iterator   
 */
template <typename ForwardIterator, typename OutputIterator>
void grahamScan(const ForwardIterator& itb, const ForwardIterator& ite,  
		OutputIterator res)
{
  grahamScan(itb, ite, res, StraightLinePredicate()); 
}

/**
 * @brief Procedure that retrieves the vertices
 * of the alpha-shape of a sorted list of 2d points
 * (based on Graham's scan).  
 * 
 * @param itb begin iterator
 * @param ite end iterator 
 * @param res output iterator using to export the retrieved points
 * @param f predicate giving the orientation
 * (positive if in a counter-clockwise orientation) 
 * 
 * @tparam ForwardIterator a model of forward iterator
 * @tparam OutputIterator a model of output iterator   
 * @tparam Predicate a model of ternary predicate
 */
template <typename ForwardIterator, typename OutputIterator, typename Predicate>
void grahamScan(const ForwardIterator& itb, const ForwardIterator& ite,  
		OutputIterator res, const Predicate& p)
{
  typedef typename std::iterator_traits<ForwardIterator>::value_type Point; 
  std::deque<Point> container; 

  //for all points
  for(ForwardIterator it = itb; it != ite; ++it)
  {
    if(container.size() < 2)
    {
      container.push_back( *it ); 
      //std::cout << " add (to back) " << *it << std::endl; 
    }
    else
    {
      //maintaining convexity with the new point
      updateConvexHull(container, *it, p); 
      //add new point
      container.push_back( *it ); 
      //std::cout << " add (to back) " << *it << std::endl; 
    }
  }//end for all points

  //maintaining convexity with the starting point
  updateConvexHull(container, *itb, p); 

  //copy
  std::copy(container.begin(), container.end(), res); 
}

// template <typename ForwardIterator, typename OutputIterator>
// void melkmanConvexHull(const ForwardIterator& itb, const ForwardIterator& ite,  
//     OutputIterator res )
// {
//   typedef typename std::iterator_traits<ForwardIterator>::value_type Point; 
//   std::deque<Point> container; 

//   //for all points
//   for(ForwardIterator it = itb; it != ite; ++it)
//   {
//     if(container.size() < 3)
//     {
//       container.push_back( *it ); 
//       //std::cout << " add (to back) " << *it << std::endl; 
//     }
//     else
//     {
//       //front
//       {
//         Point P = *it; 
//         Point Q = container.front(); 
//         container.pop_front(); 
//         if (container.size() != 0) 
//         {
//           Point R = container.front(); 
//           //std::cout << " signed area of " << P << " " << Q << " " << R << " : " << signedArea(P,Q,R) << std::endl; 
//           while ( ( signedArea(P,Q,R) >= 0 )&&(container.size() != 0) )
//           {
//             //remove Q
//             //std::cout << " remove from front " << Q << std::endl; 
//             Q = R; 
//             container.pop_front(); 
//             if (container.size() != 0) 
//               R = container.front(); 
//           }
//           //add Q
//           container.push_front(Q);
//         }
//       }

//       //back
//       {
//         Point P = *it; 
//         Point Q = container.back(); 
//         container.pop_back(); 
//         if (container.size() != 0) 
//         {
//           Point R = container.back(); 
//           //std::cout << " signed area of " << P << " " << Q << " " << R << " : " << signedArea(P,Q,R) << std::endl; 
//           while ( ( signedArea(P,Q,R) <= 0 )&&(container.size() != 0) )
//           {
//             //remove Q
//             //std::cout << " remove from back" << Q << std::endl; 
//             Q = R; 
//             container.pop_back(); 
//             if (container.size() != 0) 
//               R = container.back(); 
//           }
//           //add Q
//           container.push_back(Q); 
//         }
//       }
//       //add new point
//       if ( signedArea(container.front(), *it, container.back()) > 0 )
//       {
//         container.push_front(*it); 
//         //std::cout << " add to front " << *it << std::endl; 
//         container.push_back(*it); 
//         //std::cout << " add to back " << *it << std::endl; 
//       }
//     }
//   }//end for all points

//   //copy
//   std::copy(++container.rbegin(), container.rend(), res); 
// }

