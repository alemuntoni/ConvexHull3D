/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Alessandro Muntoni (muntoni.alessandro@gmail.com)
 */
#include <iostream>

#include "convex_hull/convexhull.h"
#include "utilities/timer.h"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ConvexHull3D input_mesh_name.obj output_mesh_name.obj";
    }
    else {
        cg3::Dcel d(argv[1]);

        cg3::Timer chTimer("Convex Hull");
        cg3::Dcel ch = cg3::convexHull(d);
        chTimer.stopAndPrint();

        ch.saveOnObj(argv[2]);
    }

    return 0;
}
