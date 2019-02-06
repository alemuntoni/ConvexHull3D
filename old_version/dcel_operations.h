#ifndef DCEL_OPERATIONS_H
#define DCEL_OPERATIONS_H

#include <iostream>
#include <QVector>
#include "DCEL.hh"
#include "conflict_graph/conflict_graph.h"
#include "utils.h"

DCEL insert_tethraedrus(CGPointf & p0, CGPointf & p1, CGPointf & p2, CGPointf & p3, face_id* ff0, face_id* ff1, face_id* ff2, face_id* ff3);

bool see (CGPointf & point, face_id face, DCEL & ch);

QVector<edge_id> horizon_edge_list(DCEL & convex_hull, QVector<face_id> * & faces, QVector<vertex_id> *horizon_vertex, QVector<face_id> *horizon_faces, CGPointf &next_point);

void delete_visible_faces(DCEL & ch, QVector<face_id> & horizon_faces, QVector<vertex_id> & horizon_vertex, QVector<face_id> visible_faces, QVector<face_id>& deleted_faces, QVector<vertex_id>& deleted_vertex, QVector<edge_id>& deleted_edges, conflict_graph & cg);

QVector<QVector<point_pointer> > calculate_P(DCEL & ch, conflict_graph & cg, QVector<edge_id> & horizon);

void insert_new_faces (DCEL & ch, QVector<edge_id> & horizon, CGPointf  & p, conflict_graph & cg, QVector<QVector<point_pointer> > & P, QVector<face_id> & horizon_faces);

void copy_real_convex_hull(DCEL & old_ch, DCEL & new_ch, QVector<face_id>& deleted_faces, QVector<vertex_id>& deleted_vertex, QVector<edge_id>& deleted_edges);

#endif // DCEL_OPERATIONS_H
