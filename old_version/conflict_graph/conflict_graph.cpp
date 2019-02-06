#include "conflict_graph.h"

conflict_graph::conflict_graph() {}

/**
 * @brief conflict_graph::add_new_point
 *  Aggiunge un punto nodo punto al conflict graph
 * @param p: punto da aggiungere
 * @return il riferimento al nodo inserito.
 */
point_pointer conflict_graph::add_new_point(CGPointf p)
{
    point_node n;
    n.set_point(p);
    points.push_back(n);
    return points.size()-1;
}

/**
 * @brief conflict_graph::add_new_face
 *  Aggiunge un nuovo nodo faccia al conflict graph
 * @param f: id della faccia da aggiungere
 * @return il riferimento al nodo inserito.
 */
face_pointer conflict_graph::add_new_face(face_id f)
{
    face_node n;
    n.set_face(f);
    faces.push_back(n);
    return faces.size()-1;
}

/**
 * @brief conflict_graph::add_new_conflict
 *  aggiunge un conflitto tra il nodo punto e il nodo faccia indicati in
 *  input mediante riferimento.
 * @param p_pointer: riferimento al nodo punto
 * @param f_pointer: riferimento al nodo facca
 */
void conflict_graph::add_new_conflict(point_pointer p_pointer, face_pointer f_pointer)
{
    points[p_pointer].add_conflict_face(f_pointer);
    faces[f_pointer].add_conflict_point(p_pointer);
}

face_pointer conflict_graph::get_face_pointer(face_id f){
    int i;
    for (i=0; i<faces.size(); i++){
        if (faces[i].get_face()==f) return i;
    }
    return -1;
}

void conflict_graph::reset_conflict_list_face(face_pointer f_pointer)
{
    int i;
    if (f_pointer >= faces.size()) return;
    QVector<point_pointer> pp =  faces[f_pointer].get_conflict_points(); // punti in conflitto con la faccia da resettare
    for (i=0; i<pp.size()-1; i++){ // da ogni punto in conflitto, rimuovo l'arco verso la faccia da resettare
        bool b=points[pp[i]].remove_conflict_face(f_pointer);
        //std::cout << b << std::endl;
    }
    faces[f_pointer].reset_conflict_list();
}

/**
 * @brief conflict_graph::get_visible_points
 *  restituisce l'unione ORDINATA delle conflict list (ordinate) delle facce f1 e f2.
 *  f1 e f2, sono gli indici sulla DCEL di due facce.
 * @param f1: indice di faccia f1 = al riferimento al nodo di f1
 * @param f2: indice di faccia f2 = al riferimento al nodo di f2
 * @return: l'unione delle due conflict list.
 */
QVector<point_pointer> conflict_graph::get_visible_points(face_id f1, face_id f2)
{
    // l'ultimo punto (quello che deve essere inserito) non farà parte dell'unione
    point_pointer last_point = points.size()-1;

    /**
     * Vi è una corrispondenza biunivoca tra id di una faccua nella DCEL e riferimento
     * al nodo della faccia del conflict graph, in quanto l'array delle facce nella DCEL
     * viene gestito nello stesso modo in cui viene gestito l'array dei nodi faccia nel
     * conflict graph. Non vi è quindi la necessità di cercare gli indici dell'array
     * delle facce del conflict graph, in quanto gli indici corridondono ai face_id stessi.
     */
    //face_pointer fp1, fp2;
    //fp1=get_face_pointer(f1);
    //fp2=get_face_pointer(f2);
    QVector<point_pointer> cf1 = faces[f1].get_conflict_points();
    QVector<point_pointer> cf2 = faces[f2].get_conflict_points();
    QVector<point_pointer> result;

    result = conflict_graph::merge(cf1, cf2); // faccio il merge delle due conflict list
    if(!result.empty() && result.last()==last_point) result.pop_back();
    // se l'ultimo punto dell'untione è il next point, lo rimuovo

    return result;
}

/**
 * @brief conflict_graph::merge
 *  dati due array ordinati, la funzione merge restituisce un array contenente tutti gli
 *  elementi di v1 e v2 ordinati e senza ripetizioni.
 * @param v1: array ordinato v1
 * @param v2: array ordinato v2
 * @return l'array ordinato contenente l'unione tra v1 e v2.
 */
QVector<point_pointer> conflict_graph::merge (QVector<point_pointer> &v1, QVector<point_pointer> &v2){
    QVector<point_pointer> merge;
    int i=0 ,j=0; // indici: i scorre l'array v1, j scorre l'array v2
    int sizev1 = v1.size();
    int sizev2 = v2.size();
    while (i != sizev1  ||  j != sizev2){ // finché entrambi gli indici non hanno raggiungo l'ultimo elemento degli array
        if (i == sizev1){ // se i ha raggiunto l'ultimo elemento di v1 (e j no)
            for (; j<sizev2; j++) merge.push_back(v2[j]); // copio i restanti elementi di v2 nell'array merge
        }
        else {
            if (j == sizev2){ // se j ha raggiunto l'ultimo elemento di v2 (e i no)
                for (; i<sizev1; i++) merge.push_back(v1[i]); // copio i restanti elementi di v1 nell'array merge
            }
            else { // altrimenti, sia i che j non hanno ancora raggiunto l'ultimo elemento
                if (v1[i] <= v2[j]){ // se l'elemento i di v1 è minore o uguale dell'elemento j di v2
                    merge.push_back(v1[i]); // inserisco v1[i] nell'array merge
                    if (v1[i] == v2[j]) j++; // se v2[j] è uguale a v1[i], incremento anche j per non avere ripetizioni
                    i++; // incremento i
                }
                else { // altrimenti, v2[j] è strettamente minore di v1[i]
                    merge.push_back(v2[j]); // inserisco v2[j] nell'array merge
                    j++; // incremento j
                }
            }
        }

    }
    return merge;
}

/**
 * @brief conflict_graph::get_next_conflict_faces
 *  Restituisce il riferimento alle facce in conflitto col prossimo punto,
 *  che è quello in cima allo stack.
 * @return: le facce in conflitto col next point
 */
QVector<face_id> * conflict_graph::get_next_conflict_faces()
{
    point_pointer pp = points.size()-1;
    return  points[pp].get_conflict_faces();
}

/**
 * @brief conflict_graph::get_point
 *  restituisce il punto nello spazio indicizzato dal point_pointer in input
 * @param pp: riferimento al nodo punto del conflict_graph
 * @return: il punto nello spazio contenuto nel nodo.
 */
CGPointf conflict_graph::get_point(point_pointer pp)
{
    return points[pp].get_point();
}

/**
 * @brief conflict_graph::get_next_point
 *  Restituisce il prossimo punto nello spazio che deve essere
 *  inserito nel convex hull
 * @return
 */
CGPointf conflict_graph::get_next_point()
{
    point_pointer p = points.size()-1;
    return points[p].get_point();
}

/**
 * @brief conflict_graph::remove_next_point
 *  Rimuove il next_point dal conflict graph, andando ad eliminare
 *  tutte le facce in conflitto con esso ed aggiustando i riferimenti.
 */
void conflict_graph::remove_next_point()
{
    /**
     * Ogni volta che rimuove una faccia dal conflict graph, tutte le facce
     * con indice minore della faccia rimossa vengono decrementati.
     */
    int i, j;
    point_pointer p = points.size()-1;
    // la conflict list del next point contiene i riferimenti a tutte le facce da eliminare
    QVector<face_pointer> fp = *(points[p].get_conflict_faces());
    for (i=0; i<fp.size(); i++) { // scorro la conflict list
        remove_face(fp[i]); // rimuovo la faccia (vedi funzione remove_face)
        /*for (j=i+1; j<fp.size(); j++) // scorro le restanti facce da eliminare ed eventualmente aggiusto il riferimento
            if (fp[j] > fp[i]) fp[j]--;*/
    }
    points.pop_back();
}

void conflict_graph::remove_conflict_next_point(face_pointer fp)
{
    point_pointer p = points.size()-1;
    points[p].remove_conflict_face(fp);
    faces[fp].remove_conflict_point(p);
}

/**
 * @brief conflict_graph::delete_next_point
 * Richiamata quando un punto ha la conflict list vuota.
 * La funzione semplicemente elimina il nodo dal conflict graph.
 */
void conflict_graph::delete_next_point()
{
    points.pop_back();
}

/**
 * @brief conflict_graph::is_empty
 * @return true se non vi sono più nodi punto nel conflict graph
 */
bool conflict_graph::is_empty()
{
    return points.size() == 0;
}

/**
 * @brief conflict_graph::next_point_has_no_conflicts
 * @return true se il next_point non ha conflitti.
 */
bool conflict_graph::next_point_has_no_conflicts()
{
    point_pointer p=points.size()-1;
    return points[p].get_conflict_faces()->size() == 0;
}

/**
 * @brief conflict_graph::remove_face
 *  rimuove un nodo faccia dal conflict graph aggiustando tutti i riferimenti.
 * @param f_pointer: riferimento alla faccia da eliminare
 */
void conflict_graph::remove_face(face_pointer f_pointer)
{
    int i;
    if (f_pointer >= faces.size()) return;
    QVector<point_pointer> pp =  faces[f_pointer].get_conflict_points(); // punti in conflitto con la faccia da eliminare
    for (i=0; i<pp.size(); i++){ // da ogni punto in conflitto, rimuovo l'arco verso il nodo da eliminare
        points[pp[i]].remove_conflict_face(f_pointer);
        //std::cout << b << std::endl;
    }
    //for (i=0; i<points.size(); i++) points[i].adjust_face_pointers(f_pointer); // aggiusto i riferimenti in tutte le conflict list dei punti
    faces[f_pointer].set_face(-1); // rimuovo la faccia
    /*for (i=f_pointer; i<faces.size(); i++){ // aggiusto i face_id nei restanti nodi faccia nel conflict graph
        faces[i].set_face(i);
    }*/
}
