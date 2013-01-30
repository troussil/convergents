/**
 * Class implementing 2d point or vector strutures.
 * @tparam T a model of number (for the coordinates).  
 */
template <typename T>
class PointVector2D 
{
  private: 
    /////////////////////// members /////////////////////
    T myX; 
    T myY; 

  public: 
    /////////////////////// inner types /////////////////
    typedef T Coordinate; 

    ///////////////////// standard services /////////////
    /**
     * Default constructor
     */
    PointVector2D(): myX(0), myY(0) {}

    /**
     * Standard constructor
     * @param x x-coordinate
     * @param y y-coordinate
     */
    PointVector2D(const T& x, const T& y): myX(x), myY(y) {}

    /**
     * Copy constructor
     * @param p other object to copy
     */
    PointVector2D(const PointVector2D& p): myX(p.x()), myY(p.y()) {}

    /**
     * Assignement operator
     * @param p other object to copy
     * @return reference on *this
     */
    PointVector2D& operator=(const PointVector2D& p) 
    {
      if (p != *this)
      {
        myX = p.x(); 
        myY = p.y(); 
      }
      return *this; 
    }

    /**
     * Default destructor
     */
    ~PointVector2D() {}

    ///////////////////// read access ///////////////////
  private: 
    /**
     * x-coordinate accessor
     * @return x-coordinate. 
     */
    T x() const { return myX; }

    /**
     * y-coordinate accessor
     * @return y-coordinate. 
     */
    T y() const { return myY; }

  public: 
    /**
     * Array-like access operator 
     * @param i index of the coordinates (0 for x-, 1 for y-)
     * @return reference on the coordinates of index i. 
     */
    T& operator[](const unsigned short& i) 
    { 
      if (i == 0){ return myX; }
      else { return myY; }
    }

    /**
     * Array-like access operator 
     * @param i index of the coordinates (0 for x-, 1 for y-)
     * @return reference on the coordinates of index i. 
     */
    const T& operator[](const unsigned short& i) const 
    { 
      if (i == 0){ return myX; }
      else { return myY; }
    }

    ///////////////////// comparisons ///////////////////
    /**
     * Equality operator
     * @param p object to compare with
     * @return 'true' if the coordinates of *this and p
     * are pairwise equal, 'false' otherwise 
     */
    bool operator==(const PointVector2D& p) const 
    {
      return (myX == p.x() && myY == p.y());
    }

    /**
     * Difference operator
     * @param p object to compare with
     * @return 'false' if *this and p are equal
     */
    bool operator!=(const PointVector2D& p) const 
    { 
      return (myX != p.x() || myY != p.y());
    }

    ///////////////////// operations ///////////////////
    /**
     * Addition operator
     * @param p object to add 
     * @return the sum *this + p
     */
    PointVector2D operator+(const PointVector2D& p) const 
    { 
      return (PointVector2D(myX+p.x(), myY+p.y())); 
    }

    /**
     * Compound Addition assignment operator
     * @param p object to add 
     * @return *this = *this + p
     */
    PointVector2D operator+=(const PointVector2D& p)  
    { 
      myX = myX + p.x();
      myY = myY + p.y();
      return ( *this); 
    }


    /**
     * Substraction operator
     * @param p object to substract 
     * @return the difference *this - p
     */
    PointVector2D operator-(const PointVector2D& p) const
    { 
      return (PointVector2D(myX-p.x(), myY-p.y())); 
    }

    /**
     * Compound Substraction assignment operator
     * @param p object to add 
     * @return *this = *this + p
     */
    PointVector2D operator-=(const PointVector2D& p)  
    { 
      myX = myX - p.x();
      myY = myY - p.y();
      return ( *this); 
    }

    /**
     * Multiplication operator (*this * k)
     * @param k scalar value all the coordinates are multiplied with
     * @return a points whose coordinates are multiplied with k
     */
    PointVector2D operator*(const T& k) const
    { 
      return (PointVector2D(k*myX, k*myY)); 
    }
    /**
     * Opposite operator
     * @return the opposite - *this
     */
    PointVector2D operator-()  const
    {
     return (PointVector2D(-myX, -myY));
    }

    /**
     * Multiplication operator
     * @param p object with which the dot product is computed
     * @return dot product *this . p
     */
    T dot(const PointVector2D& p) const 
    { 
      return (p.x() * myX + p.y() * myY); 
    }

    ///////////////////// norms   //////////////////////
    /**
     * @return L1 norm (ie. |x|+|y|)
     */
    T normL1() const 
    { 
      T norm; 
      if (myX >= 0){norm = myX;}
      else {norm = - myX;}
      if (myY >= 0){norm += myY;}
      else {norm -= myY;}

      return norm;
    } 

    /**
     * @return L2 norm (ie. sqrt(x²+y²) )
     */
    double normL2() const 
    { 
      return sqrt( myX * myX + myY * myY);	
    }

    /**
     * @return Linf norm (ie. max(|x|,|y|) )
     */
    T normLinf() const 
    {
      if (myX >= 0)
	{
	  if (myY >= 0)
	      return myX>myY?myX:myY; 
	  else
	    return myX>(-myY)?myX:(-myY); 
	}
      else
	{
	  if (myY >= 0)
	    return (-myX)>myY?(-myX):myY; 
	  else
	    return (-myX)>(-myY)?(-myX):(-myY); 
	}
    }
}; 

//Multiplication operator ( k * (*this) )
  template<typename Point>
Point operator*(const typename Point::Coordinate& k, Point& p) 
{
  return p * k; 
}

//Display
template<typename Coordinate>
std::ostream& operator<<( std::ostream & out, const PointVector2D<Coordinate>& object )
{
  out << "(" << object[0] << "," << object[1] << ")"; 
}
