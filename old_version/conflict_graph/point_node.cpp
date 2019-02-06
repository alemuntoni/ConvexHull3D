#include "point_node.h"

point_node::point_node(){}

/**
 * @brief point_node::set_point
 *  setta il punto nello spazio associato al nodo
 * @param p: il punto (CGPointf)
 */
void point_node::set_point(CGPointf p)
{
    point = p;
}

/**
 * @brief point_node::get_point
 * @return il punto nello spazio associato al nodo.
 */
CGPointf point_node::get_point()
{
    return point;
}

/**
 * @brief point_node::add_conflict_face
 *  Inserisce al nodo un arco uscente, verso la faccia in posizione f.
 *  Tiene l'array delle facce in conflitto ordinato.
 * @param f: indice della faccia in conflitto nel conflict graph.
 */
void point_node::add_conflict_face(face_pointer f)
{
    if (conflict_list_f.empty()) conflict_list_f.push_back(f);
    else {
        // trovo l'indice dove verrà inserito f
        int index = search_insert_index(f, 0, conflict_list_f.size()-1, conflict_list_f);
        conflict_list_f.insert(index,f); // tutti gli elementi successivi a index sono scalati di una posizione
    }
}

/**
 * @brief point_node::remove_conflict_face
 *  Elimina l' arco uscente verso l'indice di faccia indicato.
 * @param f
 * @return true se la rimozione è andata a buon fine, false altrimenti
 */
bool point_node::remove_conflict_face(face_pointer f)
{
    // l'array delle facce in conflitto è ordinato, faccio una ricerca binaria
    int index = binary_search(f, 0, conflict_list_f.size()-1, conflict_list_f);
    if (index < 0) return false;
    conflict_list_f.remove(index); // tutti gli elementi successivi a index sono scalati di una posizione
    return true;
}

/**
 * @brief point_node::adjust_face_pointers
 *  Quando una faccia viene rimossa dall'array delle facce, l'array viene ricompattato
 *  è necessario quindi aggiornare anche tutti i riferimenti alle facce, che si trovano
 *  nelle conflict list di ogni nodo punto.
 *  dato l'indice della faccia rimossa, tutti gli elementi nella conflict_list maggiori
 *  di removed_face vengono decrementati.
 * @param removed_face: indice della faccia rimossa dall'array dei nodi faccia.
 */
void point_node::adjust_face_pointers(face_pointer removed_face)
{
    int i=0, size = conflict_list_f.size();
    // finché gli elementi sono <= a removed_face, non faccio niente
    while (i < size  &&  conflict_list_f[i] <= removed_face) i++;
    // dal primo elemento magiore di removed_face in poi, decremento
    for (; i<size; i++){
        conflict_list_f[i]--;
    }
}

/**
 * @brief point_node::get_conflict_faces
 *  restituisce l'array contenente gli indici alle facce in conflitto col
 *  nodo attuale. Ne restituisce il riferimento per evitare copie di array
 *  inutili e dispendiose.
 * @return: puntatore all'array delle facce in conflitto col nodo attuale
 */
QVector<face_pointer> * point_node::get_conflict_faces()
{
    return &conflict_list_f;
}
