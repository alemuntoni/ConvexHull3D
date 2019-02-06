# ConvexHull3D

[Alessandro Muntoni](http://vcg.isti.cnr.it/~muntoni/)

Project of the Geometric Algorithms and Spatial Data Structures course for the Master's Degree in Computer Science at the University of Cagliari. 
Implementation of an incremental randomized 3D Convex Hull algorithm of a set of points.

#Download
```bash
git clone --recursive https://github.com/muntonialessandro/ConvexHull3D.git
```

## Requirements, Compilation and Execution (optimized version)

The only library required is [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page), and for the generation of Makefile qmake is used.

Compilation:
```bash
qmake ConvexHull3D.pro
make
```

Usage:
```bash
./ConvexHull3D inputMesh.obj outputMesh.obj
```
