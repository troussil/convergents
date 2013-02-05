#include<cmath>
/**
 * Class implementing an on-line and ouput-sensitive algorithm
 * that retrieves the vertices of the convex hull of all digital
 * points lying inside an implicit shape, which is 'ray-intersectable', 
 * ie. the intersection between the shape boundary and 
 * a ray emanating from a given point along a given direction
 * is computable.
 * 
 * @tparam TShape a model of ray-intersectable shape.  
 *
 */
template <typename TShape>
class OutputSensitiveConvexHull 
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
    OutputSensitiveConvexHull(const Shape& aShape)
      : myShape(aShape) {}

  private:
    /**
     * Copy constructor
     * @param other other object to copy
     */
    OutputSensitiveConvexHull(const OutputSensitiveConvexHull& other) {}

    /**
     * Assignement operator
     * @param other other object to copy
     * @return reference on *this
     */
    OutputSensitiveConvexHull& operator=(const OutputSensitiveConvexHull& other) 
    { return *this; }

   public: 
    /**
     * Default destructor
     */
    ~OutputSensitiveConvexHull() {}


    ///////////////////// main methods ///////////////////
    /**
     * Given a vertex of the convex hull, find the next
     * vertex in a counter-clockwise order
     * @param aPoint any vertex of the convex hull
     * @return the next vertex
     */
    Point next(const Point& aPoint)
    {

      return aPoint; 
    }


}; 
