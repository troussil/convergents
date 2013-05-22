# Repository - Convergent

Introduction of programming best practices in c++ through the learning of different knowledges and skills :

* Generic and oriented object programming.
* Design patterns.
* Building Makefile.
* Automatic testing.

It will deserve a wider context in digital geometry.

## Organization

### Folders

* *doc* is a folder containing Latex and bibtex source files for the report and the algorithm used during the programming.
* *inc* is a folder containing the .h files.
* *stests* is a folder containing all the program test.
* *stools* is a folder containing the display tool.
* *tools* is a folder containing all the GNUPlot plot

### Files

* *Makefile* : Compiling all files. ( --depreciated -- see CMakeLists.txt)
* *CMakeLists* : Building project.
* *.gitignore* : ignoring upload of temporary and binaries files.
* *README.md* : This wiki presentation

## Classes and Algorithms

* *PointVector2D.h* is the main class implementing point and vector in two dimensions.
* *RayIntersectableStraightLine.h* and *RayIntersectableCircle.h* implement some 'ray intersectable' class. We try to used some generic concept in order to computed the same way on different shape. At the beginning, we start with the straight line then the circle.
* We follow with algorithms computing the convergent points of a rationnel : *testConvergents.cpp*, the convex hull of a discrete circle : *OutputSensitiveConvexHull.h* and the alpha-shape of a straight-line and a discrete circle : *testAlphaShapeStraightLine.cpp* and *OutputSensitiveAlphaShape.h*.
* *toolAlphaShape.cpp* produce a graph witch visualize the number of vertices of the convex hull, the alpha-shape with alpha = 0, and the alpha-shape with alpha < 0 in function to the the size of the radius. We create 100 random circles with a center in [0,1]x[0,1] and a radius proportional to the predicate increasing by 2^2.
* *toolDisplay.cpp* used DGtal librairie to print the shape on a board.


## Structure

### Oriented object programming and design patterns

Class and templates initialization explain with the creation of functional tools in planar geometry. We elaborate some useful concepts in discrete geometry :

1. Points / Vectors
2. Lines
3. Circles

We provide some programming conveniences :

1. Class members
2. Inner types
3. Standard services
4. Read access
5. Comparisons
6. Operations
7. Norms

### Building Makefile


Compile and execute the test files by launching this command in a terminal :

```
# Create build folder and go inside
mkdir build; cd build
# Generate files
cmake ..
# Compile files
make
# Execute all the test
make test
# Specific testing
./stests/test*
# For post-processing
./stools/tool*

```

### Unitary and automatic testing

1. Implementing testing : Basic operators, arithmetic operators, norms...
2. Intersecable testing : Various intersections cases...
3. Concrete application : Geometric gcd.
4. Accuracy and validity of the algorithms : geometric-gcd, convex hull, alpha-shape.


# Subject

## Main topic

In the huge topic of digital geometry, I am mainly interested by the well-known issue of discrete circle shape in 2d on regular lattice. Particularly, I try to design and implement the alpha-shape computation in a *output sensitive* complexity.

## Alpha-shape

An alpha-hull is a generalization of the convex-hull algorithm for alpha = 0. It consist on recover the the edge of the circle with a new constraint, neighbors vertices have to be closed in a same ball of radius 1/alpha.

A more accurate and mathematical definition,

*For alpha < 0*, **the alpha-shape is defined as the intersection of all closed complements of discs with radius -1/alpha that contain all the points inside the circle**.

*For alpha > 0*, **the alpha-shape is defined as the intersection of all the closed discs with radius 1/alpha that contain all the points inside the circle.**

## Discrete circle

If I put an Euclidean circle on a lattice, all the points which belong to the inside are part of discrete circle.

In our repository, we constructed them by different methods :

* The circumcircle formed by a triangle itself defined by three point of the grid.
* The integer value to the quadruple (a,b,c,d) of the circle equation : **ax + by + c(x^2 + y^2) + d = 0**.

#### License

The source code will be available under the GPLv3.
You can download it [here](http://www.gnu.org/licenses/gpl-3.0.txt).

#### Contributors

* Instructor : ROUSSILLON Tristan : https://github.com/troussil
* Trainee : LAFOND Thomas : https://github.com/homeostasie
