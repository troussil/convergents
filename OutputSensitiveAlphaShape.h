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
        AlphaShapeHull += aPpoint;

        // La variable pour calculer les différents convergents
        std::vector<Vector> convergents;

        // Ralpha = -1/alpha;
        double Ralpha = -1/alpha;


  // boucle while -- tant qu'on a pas rajouter le point bPoint.
  // A FAIRE -- On va avoir besoin de deux points supplémentaires
  // pour jouer les bornes a et b qui seront les valeurs initales

        // Lowest Bezout Point
        // lastest convergent under [a, b] 

        dVer = RayIntersectableStraightLine<Point>(aPoint, bPoint);

        convergents.clear(); 
        // gcd and convergent
        geometricConvergents(bPoint - aPoint, std::back_inserter(convergents)); 
        
        Point pPoint = aPoint + convergents.end();

        // While (dray(cPoint) >=0 ) 
        {
          // on va à la valeur précédente de l'itérateur.
          pPoint = (convergent--);
        }

        // Soit Rexp le rayon tel que P = convPoint appartienne au cercle 
        // de centre C=(cx, cy). On a :
        // norm(CA) = norm(CP) = R et norm(CB) = norm(CP) = R

        double cy = -1/2 * ( (bPoint.x() - pPoint.x())*(-normL22(aPoint) + normL22(pPoint)) -
            (aPoint.x() - pPoint.x())*(-normL22(bPoint) + normL22(pPoint)) ) 
          / ( (aPoint.y() - pPoint.y())*(bPoint.x() - pPoint.x()) * 
              (bPoint.y() - pPoint.y())*(aPoint.x() - pPoint.x()) );

        if(aPoint.x() - pPoint.x() != 0)
        {
          double cx = -1/2 * (normL22(pPoint) - normL22(aPoint) + 2*cy*(aPoint.y()-pPoint.y())) 
            / (aPoint.x() - pPoint.x());
        }
        else
        {
          double cx = -1/2 * (normL22(pPoint) - normL22(bPoint) + 2*cy*(bPoint.y()-pPoint.y())) 
            / (bPoint.x() - pPoint.x());
        }

        // On mesure Rexp²
        double Rexp = (cx- aPoint.x())*(cx- aPoint.x()) + (cy- aPoint.y())*(cy- aPoint.y());

        // P n'appartient pas au disque
        if (Rexp > Ralpha)
        {
          // b est le prochain sommet de l'alpha-shape
          // On remplace a par b et b par le "convergent" suivant.

        }
        // P appartient au disque et donc à l'alpha-shape
        else
        {
          // On conserve a et on remplace b par le convergent précédent.

        }

      }
}; 

