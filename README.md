# Convergents

## Introduction

Introduction of programming best practices in c++ through the learning of different knowledges and skills :

* Oriented object programming and design patterns :
* Building Makefile.
* Automatic testing.

It will deserve a wider context in digital geometry.

## Repository Organisation

### Folders

* *doc* is a folder containing Latex and bibtex source files for the report and the algorithm used durign the programmation.
* *inc* is a folder containing the .h files. 
* *src* is a folder containing all the program test.

### Files

* *Makefile* : Compiling all files. (See below)
* *.gitignore* : ignoring upload of temporary and binaries files. 
* *README.md* : This wiki presentation

## Classes and Algorithms

* *PointVector2D.h* is the main class implementing point and vector in two dimensions.
* *RayIntersectableStraightLine.h* and *RayIntersectableCircle.h* implement some 'ray intersectable' class. At the beginning, we start with straight line and circle.
* We follow with algorithms computing the convergent of a rationnel : *testConvergents.cpp*, the convex hull of a convex shape : *OutputSensitiveConvexHull.h* and the alpha-shape of a straight-line and a convex shape : *testAlphaShapeStraightLine.cpp* and *OutputSensitiveAlphaShape.h*.


## Concept

### Oriented object programming and design patterns

Class and templates initialization explain with the creation of functional tools in planar geometry. We elaborate some usefull concepts in discrete geometry : 

1. Points
2. Vectors
3. Lines
4. Circles

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
make # Compiling sources
make test # Testing sources
```

### Automatic testing

1. Implementing testing : Basic operators, arithmetic operators, norms...
2. Intersecable testing : Various intersections cases...
3. Concrete application : Geometric gcd. 

#### License

The source code will be available under the GPLv3. 
You can download it [here](http://www.gnu.org/licenses/gpl-3.0.txt).

#### Contributors

* Instructor : ROUSSILLON Tristan : https://github.com/troussil
* Trainee : LAFOND Thomas : https://github.com/homeostasie
