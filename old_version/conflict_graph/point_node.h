#ifndef POINT_NODE_H
#define POINT_NODE_H

#include "structures.hh"
#include "utils.h"
#include <QVector>

/**
 * @brief face_pointer
 *  tipo usato per rappresentare gli indici ai nodi faccia.
 */
typedef int face_pointer;


/**
 * @brief The point_node class
 *  rappresenta un nodo di tipo punto nel conflict graph.
 *  è composto da un punto nello spazio (CGPointf) e un array
 *  di archi uscenti (array di indici indicanti la posizione
 *  delle facce con cui il punto è in conflitto).
 */
class point_node
{
public:
    point_node();

    void set_point(CGPointf p);

    CGPointf get_point();

    void add_conflict_face(face_pointer f);

    bool remove_conflict_face(face_pointer f);

    void adjust_face_pointers(face_pointer removed_face);

    QVector<face_pointer>* get_conflict_faces();

private:
    CGPointf point;

    QVector<face_pointer> conflict_list_f;

};

#endif // POINT_NODE_H
