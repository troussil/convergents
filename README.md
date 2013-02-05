# Convergents

## Introduction

Introduction of programming best practices in c++ through the learning of different knowledges and skills :

* Oriented object programming and design patterns :
* Building Makefile.
* Automatic testing.

It will deserve a wider context in digital geometry.

## Folders & Files

### Folders

* *rapport.d/* : It's a subfolder which containing Latex and bibtex source files for the report.
* *main* : All the files belong to programming.

### Files

#### System

* *Makefile* : Compiling all files. (See below)
* *.gitignore* : ignoring upload of temporary and binaries files. 
* *README.md* : This wiki presentation

#### Programming

* *PointVector2D.h* : class implementing 2d point or vector strutures.
* *RayIntersectableStraightLine.h* : class implementing a straight line that is 'ray intersectable'
* *RayIntersectableCircle.h* : class implementing a circle that is 'ray intersectable', 

#### Testing

* *testPointVector2D.cpp* : Testing class PointVector2D implementation.
* *testRay.cpp* : Testing the intersection between ray and whole : straight line and circle.
* *testConvergents.cpp* : Testing straight line intersection with ray by implemented geometric gcd.

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
