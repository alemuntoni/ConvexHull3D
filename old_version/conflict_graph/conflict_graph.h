#ifndef CONFLICT_GRAPH_H
#define CONFLICT_GRAPH_H

#include <iostream>

#include "point_node.h"
#include "face_node.h"

/**
 * @brief The conflict_graph class
 *  Fornisce una serie di metodi specifici per la gestione di un conflict graph
 *  studiato per l'algoritmo del convex hull 3D.
 *  è composto da un array di nodi punto e un array di nodi di tipo faccia (ogni
 *  nodo di tipo punto possiede al suo interno riferimenti all'array di nodi faccia
 *  e ogni nodo di tipo faccia possiede al suo interno riferimenti all'array di nodi
 *  punto). L'array di facce è ordinato per naturale associazione con gli indici delle
 *  facce nella DCEL. L'array di punti viene invece gestito come uno stack: vengono
 *  inizialmente inseriti tutti, poi finché l'array non è vuoto viene svuotato, partendo
 *  dall'ultimo punto inserito.
 */
class conflict_graph
{
public:
    conflict_graph();

    point_pointer add_new_point(CGPointf p);

    face_pointer add_new_face(face_id f);

    void add_new_conflict (point_pointer p_pointer, face_pointer f_pointer);

    QVector<point_pointer> get_visible_points(face_id f1, face_id f2);

    QVector<face_id> * get_next_conflict_faces();

    CGPointf get_point(point_pointer pp);

    CGPointf get_next_point();

    void remove_next_point();

    void remove_conflict_next_point(face_pointer fp);

    void delete_next_point();

    void print_conflict_graph();

    bool is_empty();

    bool next_point_has_no_conflicts();

    face_pointer get_face_pointer(face_id f);

    void reset_conflict_list_face(face_pointer f);
private:
    QVector<face_node> faces;

    QVector<point_node> points;

    QVector<point_pointer> merge(QVector<point_pointer> &v1, QVector<point_pointer> &v2);

    void remove_face(face_pointer f_pointer);
};

#endif // CONFLICT_GRAPH_H
