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

  private: 
    /////////////////////// members /////////////////////
    /**
     * const reference on a shape
     */
    const Shape& myShape;
    const double alpha;

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
     * Given a vertex of the convex hull, find the next
     * vertex in a counter-clockwise order
     * @param aPoint any vertex of the convex hull
     * @return the next vertex
     */
    template <typename Point, typename OutputIterator>
      void rec_alphaShape(const Shape& aShape, const Point& aPoint, const Point bPoint, OutputIterator AlphaShapeHull)
      {

        // aPoint est le premier point de l'enveloppe Alpha-Shape
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

        bool candidat;

        // The discrete straight-line [a, b]
        RayIntersectableStraightLine<Point> DroiteRatio(aPoint, bPoint-aPoint);

        while ( pconv != bPoint ) // on est en b
        {
          Point pm2 = pStart + cm2;
          Point pm1 = pStart + cm1;
          
          if (1==1) // pm2 appartient au cercle
          {

            AlphaShapeHull += pm2; 
            candidat = true;
          }
          else
          {
            candidat = false;
          }

          // We stop when the ray is parallel to the straight line
          while( DroiteRatio.dray(pm2, pm1, qk, pconv) == true && candidat = false)
          {
            if (1==1) // pconv appartient au cercle
            {
              // on rajoute le sommet
              AlphaShapeHull += pconv;
              // on repart de là 
              pStart = pconv;
              
              candidat = true;
            }
            else
            {
            // on continue le calcul des convergents
              pm2 = pm1;
              pm1 = pconv;
            }


          }

        }    

      }
}; 

