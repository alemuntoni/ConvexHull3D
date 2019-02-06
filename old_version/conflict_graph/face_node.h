#ifndef FACE_NODE_H
#define FACE_NODE_H

#include <QVector>
#include "utils.h"

/**
 * @brief face_id
 *  tipo usato per rappresentare l'indice di una faccia sulla DCEL.
 */
typedef int face_id;

/**
 * @brief edge_id
 *  tipo usato per rappresentare l'indice di un half_edge sulla DCEL.
 */
typedef int edge_id;

/**
 * @brief vertex_id
 *  tipo usato per rappresentare l'indice di un vertice sulla DCEL.
 */
typedef int vertex_id;

/**
 * @brief point_pointer
 *  tipo usato per rappresentare gli indici ai nodi punto.
 */
typedef int point_pointer;

/**
 * @brief The face_node class
 *  rappresenta un nodo di tipo faccia nel conflict graph.
 *  è composto da un face_id e un array di archi uscenti
 *  (array di indici indicanti la posizione dei punti con
 *  cui la faccia è in conflitto).
 */
class face_node
{
public:
    face_node();

    void set_face(face_id f);

    face_id get_face();

    QVector<point_pointer> get_conflict_points();

    void add_conflict_point(point_pointer p);

    bool remove_conflict_point(point_pointer p);

    void reset_conflict_list();

private:
    face_id face;

    QVector<point_pointer> conflict_list_p;

};

#endif // FACE_NODE_H
