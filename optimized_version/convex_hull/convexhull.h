/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Alessandro Muntoni (muntoni.alessandro@gmail.com)
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */

#ifndef CG3_CONVEXHULL_H
#define CG3_CONVEXHULL_H

#include "dcel/dcel.h"
#include "bipartite_graph/bipartite_graph.h"


namespace cg3 {

Dcel convexHull(const Dcel& inputDcel);

template <class InputContainer>
Dcel convexHull(const InputContainer& points);

template <class InputIterator>
Dcel convexHull(InputIterator first, InputIterator end);

} //namespace cg3

#include "convexhull.tpp"

#endif // CG3_CONVEXHULL_H
