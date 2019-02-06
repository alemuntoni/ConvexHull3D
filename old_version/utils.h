#ifndef UTILS_H
#define UTILS_H

#endif // UTILS_H

#include <QVector4D>
#include <QMatrix4x4>

#include "structures.hh"

float complanar(CGPointf &p0, CGPointf &p1, CGPointf &p2, CGPointf &p3);

int binary_search(int n, int first, int last, QVector<int> & v);

int search_insert_index(int n, int first, int last, QVector<int> & v);

QVector<int> merge (QVector<int> &v1, QVector<int> &v2);
