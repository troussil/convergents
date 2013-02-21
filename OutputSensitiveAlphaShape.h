#include<cmath>
/**
 * Class implementing an on-line and ouput-sensitive algorithm
 * that retrieves the vertices of the alpha-shape of all digital
 * points lying inside an implicit shape, which is 'ray-intersectable', 
 * ie. the intersection between the shape boundary and 
 * a ray emanating from a given point along a given direction
 * is computable.
 * 
 * @tparam TShape a model of ray-intersectable shape.  
 *
 */

template <typename TShape>
class OutputSensitiveAlphaShape 
{
  public: 
    /////////////////////// inner types /////////////////
    typedef TShape Shape; 
    typedef typename Shape::Point Point; 
    typedef typename Shape::Vector Vector; //type redefinition
    typedef long long Integer;

  private: 
    /////////////////////// members /////////////////////
    /**
     * const reference on a shape
     */
    const Shape& myShape;
    /**
     * The alpha is viewed as the fraction, 
     * alpha = myNum / myDen
     */
    const Integer myNum; 
    const Integer myDen; 

  public:
    ///////////////////// standard services /////////////
    /**
     * Standard constructor
     * @param aShape
     */
    OutputSensitiveAlphaShape(const Shape& aShape)
      : myShape(aShape) {}

  private:
    /**
     * Copy constructor
     * @param other other object to copy
     */
    OutputSensitiveAlphaShape(const OutputSensitiveAlphaShape& other) {}

    /**
     * Assignement operator
     * @param other other object to copy
     * @return reference on *this
     */
    OutputSensitiveAlphaShape& operator=(const OutputSensitiveAlphaShape& other) 
    { return *this; }

  public: 
    /**
     * Default destructor
     */
    ~OutputSensitiveAlphaShape() {}


    ///////////////////// main methods ///////////////////
    /**
     * Given a straight line, find the alpha Hull,
     * @param aPoint, bPoint, the starting and ending point 
     * of the line
     * @return the alpha-hull
     */
    template <typename Point, typename OutputIterator>
      void convAlphaShape(const RadiusCirclePredicate predicat, const Point& aPoint, const Point bPoint, OutputIterator AlphaShapeHull)
      {

        // aPoint is the first Alpha-Shape vertex
        AlphaShapeHull += aPoint;

        // Init
        Point cm2(1,0);
        Point cm1(0,1);

        Point pm2;
        Point pm1;

        Point pStart = aPoint;

        // pconv is the next convergent pconv = pm2 + qk * pm1
        Point pconv;
        int qk;

        // we found a new vertex
        bool candidat;

        // The discrete straight-line [a, b]
        RayIntersectableStraightLine<Point> DroiteRatio(aPoint, bPoint-aPoint);

        while ( pconv != bPoint ) // we have add the last vertex b
        {
          Point pm2 = pStart + cm2;
          Point pm1 = pStart + cm1;

          if (predicat(pStart, pm2, bPoint)) // pm2 inside the circumcircle
          {
            //pm2 is a vertex
            AlphaShapeHull += pm2;

            // next iteration will start from pm2
            pStart = pm2;
            candidat = true;
          }
          else
          {
            candidat = false;
          }

          // We stop when the ray is parallel to the straight line
          while(DroiteRatio.dray(pm2, pm1, qk, pconv) == true && candidat = false)
          {
            // pconv inside the circumcircle
            if (DroiteRAtion(pconv) <= 0 && predicat(pStart, pconv, bPoint) )
            {
              // pconv is a vertex
              AlphaShapeHull += pconv;
              // next iteration will start from pconv
              pStart = pconv;

              candidat = true;
            }
            else
            {
              // we search for the next convergent
              pm2 = pm1;
              pm1 = pconv;
            }
          }
        }    
      }
}; 

