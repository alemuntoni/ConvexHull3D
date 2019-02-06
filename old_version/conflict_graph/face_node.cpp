#include "face_node.h"

face_node::face_node(){}

/**
 * @brief face_node::set_face
 *  setta l'id della faccia (della DCEL) associato al nodo
 * @param f: l'id della faccia
 */
void face_node::set_face(face_id f)
{
    face=f;
}

/**
 * @brief face_node::get_face
 * @return l'id della faccia associata al nodo.
 */
face_id face_node::get_face()
{
    return face;
}

/**
 * @brief face_node::get_conflict_points
 *  restituisce l'array contenente gli indici ai punti in conflitto col
 *  nodo attuale. Ne restituisce il riferimento per evitare copie di array
 *  inutili e dispendiose.
 * @return
 */
QVector<point_pointer> face_node::get_conflict_points()
{
    return conflict_list_p;
}

/**
 * @brief face_node::add_conflict_point
 *  Inserisce al nodo un arco uscente, verso il punto in posizione p.
 *  L'array è ordinato per come vengono gestiti i punti nell'algoritmo.
 * @param p
 */
void face_node::add_conflict_point(point_pointer p)
{
    conflict_list_p.push_back(p);
}

/**
 * @brief face_node::remove_conflict_point
 *  Elimina l' arco uscente verso l'indice di punto indicato.
 * @param p
 * @return true se la rimozione è andata a buon fine, false altrimenti
 */
bool face_node::remove_conflict_point(point_pointer p)
{
    // l'array dei punti in conflitto è ordinato, faccio una ricerca binaria
    int index = binary_search(p, 0, conflict_list_p.size()-1, conflict_list_p);
    if (index < 0) return false;
    conflict_list_p.remove(index); // tutti gli elementi successivi a index sono scalati di una posizione
    return true;
}

void face_node::reset_conflict_list()
{
    conflict_list_p.clear();
}
