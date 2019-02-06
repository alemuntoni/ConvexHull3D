#include "dcel_operations.h"

/**
 * @brief insert_tethraedrus
 *  Inserisce il tetraedro formato dai 4 punti p0, p1, p2, p3, dove p3
 *  vede p0,p1 e p2 ordinati in senso antiorario. Viene restituita la
 *  la DCEL contenente il tetraedro, e ff0, ff1, ff2, ff3 conterranno gli
 *  indici delle 4 facce del tetraedro nella DCEL.
 */
DCEL insert_tethraedrus(CGPointf & p0, CGPointf & p1, CGPointf & p2, CGPointf & p3, face_id* ff0, face_id* ff1, face_id* ff2, face_id* ff3) {
    DCEL tethraedrus;
    vertex v;                   // utilizzata per l'inserimento dei vertici
    vertex_id v0, v1, v2, v3;   // id dei 4 vertici nella DCEL
    half_edge e;                // utilizzata per l'inserimento degli half_edge
    edge_id e01, e12, e20;      // id half edge di faccia f0
    edge_id e10, e03, e31;      // id half edge di faccia f1
    edge_id e23, e30, e02;      // id half edge di faccia f2
    edge_id e21, e13, e32;      // id half edge di faccia f3
    face f;                     // utilizzata per l'inserimento delle facce
    face_id f0, f1, f2, f3;     // id delle 4 facce nella DCEL

    /** Inserimento Vertici */
    v.set_coord(p0);
    v0 = tethraedrus.add_vertex(v);     // v0
    v.set_coord(p1);
    v1 = tethraedrus.add_vertex(v);     // v1
    v.set_coord(p2);
    v2 = tethraedrus.add_vertex(v);     // v2
    v.set_coord(p3);
    v3 = tethraedrus.add_vertex(v);     // v3

    /** Inserimento Half Edge */
    // half edge di f0
    e.set_from_vertex(v0);
    e.set_to_vertex(v1);
    e01 = tethraedrus.add_half_edge(e); // e01

    e.set_from_vertex(v1);
    e.set_to_vertex(v2);
    e12 = tethraedrus.add_half_edge(e); // e12

    e.set_from_vertex(v2);
    e.set_to_vertex(v0);
    e20 = tethraedrus.add_half_edge(e); // e20

    // half edge di f1
    e.set_from_vertex(v1);
    e.set_to_vertex(v0);
    e10 = tethraedrus.add_half_edge(e); // e10

    e.set_from_vertex(v0);
    e.set_to_vertex(v3);
    e03 = tethraedrus.add_half_edge(e); // e03

    e.set_from_vertex(v3);
    e.set_to_vertex(v1);
    e31 = tethraedrus.add_half_edge(e); // e31

    // half edge di f2
    e.set_from_vertex(v2);
    e.set_to_vertex(v3);
    e23 = tethraedrus.add_half_edge(e); // e23

    e.set_from_vertex(v3);
    e.set_to_vertex(v0);
    e30 = tethraedrus.add_half_edge(e); // e30

    e.set_from_vertex(v0);
    e.set_to_vertex(v2);
    e02 = tethraedrus.add_half_edge(e); // e02

    // half edge di f3
    e.set_from_vertex(v2);
    e.set_to_vertex(v1);
    e21 = tethraedrus.add_half_edge(e); // e21

    e.set_from_vertex(v1);
    e.set_to_vertex(v3);
    e13 = tethraedrus.add_half_edge(e); // e13

    e.set_from_vertex(v3);
    e.set_to_vertex(v2);
    e32 = tethraedrus.add_half_edge(e); // e32

    /** Inserimento Facce */
    f.set_inner_half_edge(e01);
    f0 = *ff0 = tethraedrus.add_face(f); // f0
    f.set_inner_half_edge(e10);
    f1 = *ff1 = tethraedrus.add_face(f); // f1
    f.set_inner_half_edge(e23);
    f2 = *ff2 = tethraedrus.add_face(f); // f2
    f.set_inner_half_edge(e21);
    f3 = *ff3 = tethraedrus.add_face(f); // f3

    /** Inserimento Incident Edge sui Vertici */
    tethraedrus.set_vertex_incident(v0,e01);
    tethraedrus.set_vertex_incident(v1,e10);
    tethraedrus.set_vertex_incident(v2,e23);
    tethraedrus.set_vertex_incident(v3,e32);

    /** Inserimento campi Half Edge */

    // e01
    tethraedrus.set_half_edge_twin(e01,e10);
    tethraedrus.set_half_edge_face(e01,f0);
    tethraedrus.set_half_edge_next(e01,e12);
    tethraedrus.set_half_edge_prev(e01,e20);

    // e12
    tethraedrus.set_half_edge_twin(e12,e21);
    tethraedrus.set_half_edge_face(e12,f0);
    tethraedrus.set_half_edge_next(e12,e20);
    tethraedrus.set_half_edge_prev(e12,e01);

    // e20
    tethraedrus.set_half_edge_twin(e20,e02);
    tethraedrus.set_half_edge_face(e20,f0);
    tethraedrus.set_half_edge_next(e20,e01);
    tethraedrus.set_half_edge_prev(e20,e12);

    // e10
    tethraedrus.set_half_edge_twin(e10,e01);
    tethraedrus.set_half_edge_face(e10,f1);
    tethraedrus.set_half_edge_next(e10,e03);
    tethraedrus.set_half_edge_prev(e10,e31);

    // e03
    tethraedrus.set_half_edge_twin(e03,e30);
    tethraedrus.set_half_edge_face(e03,f1);
    tethraedrus.set_half_edge_next(e03,e31);
    tethraedrus.set_half_edge_prev(e03,e10);

    // e31
    tethraedrus.set_half_edge_twin(e31,e13);
    tethraedrus.set_half_edge_face(e31,f1);
    tethraedrus.set_half_edge_next(e31,e10);
    tethraedrus.set_half_edge_prev(e31,e03);

    // e23
    tethraedrus.set_half_edge_twin(e23,e32);
    tethraedrus.set_half_edge_face(e23,f2);
    tethraedrus.set_half_edge_next(e23,e30);
    tethraedrus.set_half_edge_prev(e23,e02);

    // e30
    tethraedrus.set_half_edge_twin(e30,e03);
    tethraedrus.set_half_edge_face(e30,f2);
    tethraedrus.set_half_edge_next(e30,e02);
    tethraedrus.set_half_edge_prev(e30,e23);

    // e02
    tethraedrus.set_half_edge_twin(e02,e20);
    tethraedrus.set_half_edge_face(e02,f2);
    tethraedrus.set_half_edge_next(e02,e23);
    tethraedrus.set_half_edge_prev(e02,e30);

    // e21
    tethraedrus.set_half_edge_twin(e21,e12);
    tethraedrus.set_half_edge_face(e21,f3);
    tethraedrus.set_half_edge_next(e21,e13);
    tethraedrus.set_half_edge_prev(e21,e32);

    // e13
    tethraedrus.set_half_edge_twin(e13,e31);
    tethraedrus.set_half_edge_face(e13,f3);
    tethraedrus.set_half_edge_next(e13,e32);
    tethraedrus.set_half_edge_prev(e13,e21);

    // e32
    tethraedrus.set_half_edge_twin(e32,e23);
    tethraedrus.set_half_edge_face(e32,f3);
    tethraedrus.set_half_edge_next(e32,e21);
    tethraedrus.set_half_edge_prev(e32,e13);

    return tethraedrus;
}

/**
 * @brief see
 *  Dato un punto, l'indice di una faccia e la DCEL dove si trova la
 *  faccia, viene restituito true se il punto vede la faccia, false
 *  altrimenti.
 */
bool see (CGPointf &point, face_id face, DCEL &ch) {
    edge_id e0, e1;             // edge incidenti alla faccia
    CGPointf p1, p2, p3;        // punti incidenti alla faccia
    vertex_id v1, v2, v3;       // id dei punti incidenti alla faccia
    float determinant;          // determinante

    /** Trovo i vertici incidenti alla faccia e ne ricavo i punti */
    e0 = ch.get_face(face).get_inner_half_edge(); // edge incidente a face
    v1 = ch.get_half_edge(e0).get_from_vertex();  // from vertex di e0
    v2 = ch.get_half_edge(e0).get_to_vertex();    // to vertex di e0
    e1 = ch.get_half_edge(e0).get_next();         // next di e0
    v3 = ch.get_half_edge(e1).get_to_vertex();    // to vertex di e1

    p1 = ch.get_vertex(v1).get_coord();
    p2 = ch.get_vertex(v2).get_coord();
    p3 = ch.get_vertex(v3).get_coord();

    /** Calcolo il determinante */
    determinant = complanar(p1, p2, p3, point);

    // se point vede p1, p2 e p3 in senso antiorario restituisco true, altrimenti false.
    if (determinant > 0) return true;
    else return false;
}

/**
 * @brief horizon_edge_list
 *  Calcola l'orizzonte visibile.
 *  L'orizzonte visibile è la lista degli id degli half_edge sull'orizzonte incidenti su facce che non
 *  sono visibili da next_point: sono quindi gli half edge esterni (quelli interni verranno poi eliminati).
 *  @param convex_hull: DCEL del convex hull dove si deve calcolare l'orizzonte
 *  @param faces: array contenente le facce visibili da next_point
 *          ATTENZIONE: L'array delle facce visibili è ordinato sugli ID delle facce.
 *  @param horizon_vertex: a fine esecuzione conterrà la lista di vertici sull'orizzonte
 *  @return l'array di edge sull'orizzonte, ordinato in senso antiorario rispetto alla vista di next_point
 */
QVector<edge_id> horizon_edge_list(DCEL & convex_hull, QVector<face_id> * & faces, QVector<vertex_id> *horizon_vertex, QVector<face_id> *horizon_faces, CGPointf & next_point) {
    QVector<edge_id> horizon;       // array che conterrà la lista ordinata degli edge esterni sull'orizzonte
    half_edge he;                   // half edge temporaneo
    edge_id e0 ,e1;                 // e0: in generale è l'half_edge sull'orizzonte incidente alla faccia visibile dal next_point
                                    // e1: twin di e0: in generale è l'half edge sull'orizzonte incidente ad una faccia non visibile
    edge_id first_boundary_edge;    // id del twin del primo half_edge dell'orizzonte
    face_id adiacent_face;          // id della faccia esterna all'orizzonte, solitamente incidente in e1.
    face f;                         // faccia temporanea
    bool finded = false, sees;
    face_id f_id = 0;               // id faccia usato per scorrere l'array delle facce visibili "faces"

    /** Ciclo di ricerca della Faccia sul boundary*/
    while (!finded){
        f = convex_hull.get_face((*faces)[f_id]);

        // Verifico se il suo primo edge è incidente su una faccia non presente nel vettore di input
        e0 = f.get_inner_half_edge(); // e0: edge incidente in f
        e1 = convex_hull.get_half_edge(e0).get_twin(); // e1: twin di e0: bisogna verificare se è  incidente su una faccia non visibile
        adiacent_face = convex_hull.get_half_edge(e1).get_face();
        sees=see(next_point, adiacent_face, convex_hull);
        if (sees){ // La faccia si trova nell'array faces: è visibile da next_point
            // Se la faccia è nel vettore in input, mi sposto al next dell'half edge
            e0 = convex_hull.get_half_edge(e0).get_next();
            e1 = convex_hull.get_half_edge(e0).get_twin();
            adiacent_face = convex_hull.get_half_edge(e1).get_face();
            sees=see(next_point, adiacent_face, convex_hull);
            if (sees){ // La faccia si trova nell'array faces: è visibile da next_point
                // Se la faccia è nel vettore in input, mi sposto all'ultimo half edge
                e0 = convex_hull.get_half_edge(e0).get_next();
                e1 = convex_hull.get_half_edge(e0).get_twin();
                adiacent_face = convex_hull.get_half_edge(e1).get_face();
                sees=see(next_point, adiacent_face, convex_hull);
                if (!sees) finded = true;
                // Altrimenti,anche la terza faccia si trova nell'array faces ed è visibile da next_point.
                // Ho analizzato una faccia che è interna e non ha edge sull'orizzonte. Analizzo un'altra faccia.
            }
            else finded = true;
        }
        else finded = true;
        f_id++;
    }

    // alla fine del ciclo:
    // - e0: half edge interno alla faccia col bordo nell'orizzonte
    // - e1: halfedge esterno alla faccia col bordo nell'orizzonte

    horizon.push_back(e1); // e1 è il primo edge sull'orizzonte
    he = convex_hull.get_half_edge(e0);
    horizon_vertex->push_back(he.get_from_vertex()); // inserisco il from vertex di e0 in horizon_vertex
    horizon_faces->push_back(he.get_face()); // inserisco la faccia incidente di e0 in horizon_faces
    first_boundary_edge = e0;
    e0 = convex_hull.get_half_edge(e0).get_next(); // e0 al passo successivo: next di e0
    do { // finchè non incontro nuovamente first_boundary_edge
        e1 = convex_hull.get_half_edge(e0).get_twin(); // e1: twin di e0
        adiacent_face = convex_hull.get_half_edge(e1).get_face(); // f: faccia incidente a e1
        sees=see(next_point, adiacent_face, convex_hull);
        if (!sees) { // se f è una faccia non visibile
            // Allora e0/e1 sono sull'orizzonte!
            horizon.push_back(e1);
            he = convex_hull.get_half_edge(e0);
            horizon_vertex->push_back(he.get_from_vertex());
            horizon_faces->push_back(he.get_face()); // inserisco la faccia incidente di e0 in horizon_faces
            e0 = convex_hull.get_half_edge(e0).get_next(); // e0 al passo successivo: next di e0
        }
        else { // altrimenti sono sul triangolo adiacente che non sta sull'orizzonte
            // devo continuare la ricerca stando sul from vertex di e0
            e0 = convex_hull.get_half_edge(e1).get_next(); // e0 al passo successivo: next di e1
            // in questo caso, e1 è incidente ad una faccia visibile da next_point
            // in questo modo, sto girando sul from vertex di e0
        }
    } while (e0 != first_boundary_edge);
    // finché non ho ritrovaro il primo bordo
    return horizon;
}

/**
 * @brief calculate_P
 *  per ogni half edge sull'orizzonte, calcola l'insieme di punti in conflitto con l'half edge,
 *  che è dato dall'unione dei insiemi di punti in conflitto con le due facce adiacenti sulll'half
 *  edge.
 * @param ch: convex hull attuale
 * @param cg: conflict graph
 * @param horizon: l'orizzonte di half edge
 * @return
 *      un array di array:
 *          Ad ogni array (riga) è associato un half edge sull'orizzonte.
 *          Per ogni half edge, vi è un array contenente i punti in conflitto con l'half edge.
 *          I punti sono rappresentati con dei point_pointer, ossia il riferimento alla posizione
 *          del punto nel conflict graph.
 */
QVector< QVector<point_pointer> > calculate_P(DCEL &ch, conflict_graph &cg, QVector<edge_id> &horizon) {
    QVector< QVector<point_pointer> > P;    // array di array P
    half_edge he0, he1;                     // hald edge temporanei: he0 edge sull'orizzonte e he1 il suo twin
    face_id f0, f1;                         // f0: faccia incidente a he0; f1: faccia incidente a he1
    QVector<point_pointer> point_array;     // array di point_pointer contenente i punti in conflitto con l'half_edge i
    int i;

    P.clear();
    for (i=0; i<horizon.size(); i++){                   // per ogni half edge sull'orizzonte
        point_array.clear();                            // reset di point_array
        P.push_back(point_array);
        he0 = ch.get_half_edge(horizon[i]);             // he0: half edge i sull'orizzonte
        he1 = ch.get_half_edge(he0.get_twin());         // he1: twin di he0
        f0 = he0.get_face();                            // f0: faccia incidente su he0
        f1 = he1.get_face();                            // f1: faccia incidente su he1
        P[i] = cg.get_visible_points(f0, f1);           // viene inserito in P[i] l'array ordinato contente i point_pointer
                                                        // visibili da f0 e f1,
    }
    return P;
}

/**
 * @brief delete_visible_faces
 *  Elimina le facce visibili dal next point. NON MODIFICA IL CONFLICT GRAPH.
 * @param ch: convex hull in input, viene modificato.
 * @param horizon: array di half_edge esterni sull'orizzonte. Questi half_edge non verranno eliminati.
 * @param horizon_vertex: array di vertici sull'orizzonte. Questi vertici non verranno eliminati.
 * @param visible_faces: array degli id delle facce da eliminare dal convex hull.
 */
void delete_visible_faces(DCEL & ch, QVector<face_id> & horizon_faces, QVector<vertex_id> & horizon_vertex, QVector<face_id> visible_faces, QVector<face_id>& deleted_faces, QVector<vertex_id>& deleted_vertex, QVector<edge_id>& deleted_edges, conflict_graph & cg) {
    int i, j;                               // indici per gli array
    edge_id e1, e2, e3;                     // id degli edge incidenti alla faccia da eliminare
    vertex_id v1, v2, v3;                   // id dei vertici incidenti alla faccia da eliminare
    QVector<vertex_id> garbage_vertex;      // array di vertici da eliminare a fine computazione
    face f;                                 // faccia temporanea
    half_edge he1, he2;                     // half edge temporanei
    int index;

    /**
     * Scorro le facce da eliminare, e elimino gli half edge ad esse incidenti e le facce.
     * Non elimino subito i vertici in quanto potrebbero essere condivisi tra più facce,
     * ed è fondamentale che ogni vertice venga eliminato una sola volta.
     */
    for (i=0; i<visible_faces.size(); i++){
        f = ch.get_face(visible_faces[i]); // f: i-esima faccia da eliminare

        /** calcolo gli id degli half_edge di f */
        e1 = f.get_inner_half_edge();   // e1: inner di f
        he1 = ch.get_half_edge(e1);
        e2 = he1.get_next();            // e2: next di e1
        e3 = he1.get_prev();            // e3: prev di e1

        /** calcolo gli id dei vertici di f */
        v1 = he1.get_from_vertex();     // v1: from vertex di e1
        v2 = he1.get_to_vertex();       // v2: to vertex di e1
        he2 = ch.get_half_edge(e2);
        v3 = he2.get_to_vertex();       // v3: to vertex di e2

        /**
         * Ogni volta che viene eliminato un elemento dalla DCEL, a tutti gli elementi dello stesso
         * tipo con indice maggiore dell'elemento eliminato, viene decrementato l'indice.
         * è quindi necessario, ogni volta che si elimina un elemento, è necessario aggiornare tutti
         * gli indici che fanno riferimento ad oggetti dello stesso tipo nella DCEL, decrementandoli
         * se hanno indice maggiore dell'indice dell'elemento eliminato.
         */

        if (horizon_faces.indexOf(visible_faces[i])<0 || visible_faces.size()==1){
            /** eliminazione degli half_edge */
            if (deleted_edges.empty()) index=0;
            else
                index=search_insert_index(e1, 0, deleted_edges.size()-1, deleted_edges);
            deleted_edges.insert(index, e1);
            //deleted_edges.push_back(e1);
            //ch.delete_half_edge(e1); // elimino e1
            //for (j=0; j<horizon.size(); j++) if (horizon[j] > e1) horizon[j]--; // aggiorno gli indici di horizon
            //if (e2 > e1) e2--; // eventualmente aggiorno e2 e e3
            //if (e3 > e1) e3--;
            index=search_insert_index(e2, 0, deleted_edges.size()-1, deleted_edges);
            deleted_edges.insert(index, e2);
            //deleted_edges.push_back(e2);
            //ch.delete_half_edge(e2); // elimino e2
            //for (j=0; j<horizon.size(); j++) if (horizon[j] > e2) horizon[j]--; // aggiorno gli indici di horizon
            //if (e3 > e2) e3--; // eventualmente aggiorno e3
            index=search_insert_index(e3, 0, deleted_edges.size()-1, deleted_edges);
            deleted_edges.insert(index, e3);
            //ch.delete_half_edge(e3); // elimino e3
            //for (j=0; j<horizon.size(); j++) if (horizon[j] > e3) horizon[j]--; // aggiorno gli indici di horizon

            /** eliminazione della faccia f */
            if (deleted_faces.empty()) index=0;
            else
                index=search_insert_index(visible_faces[i], 0, deleted_faces.size()-1, deleted_faces);
            deleted_faces.insert(index, visible_faces[i]);
            //ch.delete_face(visible_faces[i]); // elimino la faccia i
            /*for (j=i+1; j<visible_faces.size(); j++) { // aggiorno eventualmente tutte le altre facce ancora da eliminare
                if (visible_faces[j] > visible_faces[i]) visible_faces[j]--;
            }*/
        }
        else cg.remove_conflict_next_point(visible_faces[i]);
        /** Salvo i vertici da eliminare nell'array garbage_vertex */

        if (horizon_vertex.indexOf(v1) < 0) {
            if (garbage_vertex.empty()) garbage_vertex.push_back(v1);
            else{
                index=search_insert_index(v1, 0, garbage_vertex.size()-1, garbage_vertex);
                if (index==garbage_vertex.size()) garbage_vertex.push_back(v1);
                else if (v1!=garbage_vertex[index]) garbage_vertex.insert(index, v1);
            }
        }
        /*if (horizon_vertex.indexOf(v1) < 0) // se v1 non è nell'orizzonte
            if (garbage_vertex.indexOf(v1) < 0) // se v1 non è già tra i garbage vertex
                garbage_vertex.push_back(v1); // aggiungo v1 ai garbage vertex*/
        if (horizon_vertex.indexOf(v2) < 0) {
            if (garbage_vertex.empty()) garbage_vertex.push_back(v2);
            else{
                index=search_insert_index(v2, 0, garbage_vertex.size()-1, garbage_vertex);
                if (index==garbage_vertex.size()) garbage_vertex.push_back(v2);
                else if (v2!=garbage_vertex[index]) garbage_vertex.insert(index, v2);
            }
        }
        /*if (horizon_vertex.indexOf(v2) < 0) // se v2 non è nell'orizzonte
            if (garbage_vertex.indexOf(v2) < 0) // se v2 non è già tra i garbage vertex
                garbage_vertex.push_back(v2); // aggiungo v2 ai garbage vertex*/
        if (horizon_vertex.indexOf(v3) < 0) {
            if (garbage_vertex.empty()) garbage_vertex.push_back(v3);
            else{
                index=search_insert_index(v3, 0, garbage_vertex.size()-1, garbage_vertex);
                if (index==garbage_vertex.size()) garbage_vertex.push_back(v3);
                else if (v3!=garbage_vertex[index]) garbage_vertex.insert(index, v3);
            }
        }
        /*if (horizon_vertex.indexOf(v3) < 0) // se v3 non è nell'orizzonte
            if (garbage_vertex.indexOf(v3) < 0) // se v3 non è già tra i garbage vertex
                garbage_vertex.push_back(v3); // aggiungo v3 ai garbage vertex*/

    }

    /** Elimino i vertici */
    deleted_vertex=merge(deleted_vertex, garbage_vertex);
    //deleted_vertex += garbage_vertex;
    /*for (i=0; i<garbage_vertex.size(); i++){ // scorro tutti i vertici da eliminare
        ch.delete_vertex(garbage_vertex[i]); // elimino il vertice i-esimo
        for (j=i+1; j<garbage_vertex.size(); j++){ // aggiorno i prossimi vertici
            if (garbage_vertex[j] > garbage_vertex[i]) garbage_vertex[j]--;
        }
    }*/
}

/**
 * @brief insert_new_faces
 *  Inserisce le nuove facce sul convex hull, unendo il punto p con l'orizzonte. MODIFICA IL CONFLICT GRAPH.
 * @param ch: convex hull su cui devono essere inserite le nuove facce
 * @param horizon: edge sull'orizzonte
 * @param p: nuovo punto da inserire
 * @param cg: conflict graph
 * @param P: punti visibili dagli edge sull'orizzonte
 */
void insert_new_faces (DCEL & ch, QVector<edge_id> & horizon, CGPointf & p, conflict_graph & cg, QVector<QVector<point_pointer> > & P, QVector<face_id> & horizon_faces) {
    int i, j;                               // indici
    vertex_id v3, v1, v2;                   // id di vertici della faccia inserita: v3 è SEMPRE l'id del nuovo punto inserito nel ch.
    edge_id e1, e2, e3;                     // id degli half edge della faccia inserita: e1 è il twin dell'edge sull'orizzonte
                                            //                                           e2 è il next di e1
                                            //                                           e3 è il prev di e1
    edge_id old_e2, old_e3;                 // old_e2: e2 al passo precedente; old_e3: e3 al primo passo.
    face_id f;                              // id della faccia inserita
    vertex new_vertex;                      // nuovo vertice da inserire, corrisponde a p.
    half_edge extern_he, he1, he2, he3;     // half edge inseriti; extern_he: half edge esterno sull'orizzonte
    face ff;                                // faccia sull'orizzonte
    CGPointf point;                         // variabile temporanea per il check di visibilità
    face_pointer fid;                       // indice del nodo di f nel conflict graph

    /**
     * Una volta costruiti gli half edge, devono essere aggiustate le relazioni di twin.
     * L'inserimeno delle facce è fatta in senso orario, quindi ad ogni passo si può collegare
     * il prev di e1 (e3) col next di e1 al passo precedente (old_e2).
     * Fa eccezione la prima faccia in quanto non vi è una faccia al passo precedente.
     * Si salva quindi il prev di e1 della prima faccia (old_e3) e lo si collega al next di e1
     * dell'ultima faccia (e2).
     */

    /** Inserisco il nuovo punto (v3) */
    new_vertex.set_coord(p); // assegno le coordinate di p
    v3 = ch.add_vertex(new_vertex); // inserisco il nuovo punto

    /** Costruisco il primo  triangolo: */

    if (horizon_faces[0]==horizon_faces[horizon_faces.size()-1]){ //nuovo triangolo
        /** Half Edge */
        // e1:
        extern_he = ch.get_half_edge(horizon[0]); // edge sull'orizzonte
        v2 = extern_he.get_from_vertex(); // v2: from di extern_he (to di e1!)
        v1 = extern_he.get_to_vertex(); // v1: to di extern_he (from di e1!)
        he1.set_from_vertex(v1);
        he1.set_to_vertex(v2);
        e1 = ch.add_half_edge(he1); // e1: id half_edge twin di extern_he

        // e2:
        he2.set_from_vertex(v2); // he2: da v2 a v3
        he2.set_to_vertex(v3);
        e2 = ch.add_half_edge(he2); // e2: id half_edge next di e1

        // e3:
        he3.set_from_vertex(v3); // he3: da v3 a v1
        he3.set_to_vertex(v1);
        e3 = ch.add_half_edge(he3); // e3: id half_edge prev di e1

        old_e3=e3; // e3 sarà il twin di e2 dell'ultima faccia

        // relazioni half_edge:
        ch.set_half_edge_next(e1, e2);
        ch.set_half_edge_next(e2, e3);
        ch.set_half_edge_next(e3, e1);
        ch.set_half_edge_prev(e2, e1);
        ch.set_half_edge_prev(e3, e2);
        ch.set_half_edge_prev(e1, e3);
        ch.set_half_edge_twin(horizon[0], e1);
        ch.set_half_edge_twin(e1, horizon[0]);

        // faccia
        ff.set_inner_half_edge(e1);
        f = ch.add_face(ff); // f: id faccia inserita

        // half edge incident face
        ch.set_half_edge_face(e1, f);
        ch.set_half_edge_face(e2, f);
        ch.set_half_edge_face(e3, f);

        // vertex incident edge
        ch.set_vertex_incident(v1, e1);
        ch.set_vertex_incident(v2, e2);
        ch.set_vertex_incident(v3, e3);

        fid = cg.add_new_face(f); // aggiungo f al conflict_graph
    }
    else{ // vecchia faccia riciclata
        /** Half Edge */
        // e1:
        extern_he = ch.get_half_edge(horizon[0]); // edge sull'orizzonte
        e1=extern_he.get_twin();
        e2=ch.get_half_edge(e1).get_next();
        e3=ch.get_half_edge(e1).get_prev();
        ch.set_half_edge_to(e2,v3);
        ch.set_half_edge_from(e3,v3);
        he1=ch.get_half_edge(e1);
        f=he1.get_face();
        v1=he1.get_from_vertex();
        v2=he1.get_to_vertex();

        old_e3=e3; // e3 sarà il twin di e2 dell'ultima faccia

        ch.set_vertex_incident(v1,e1);
        ch.set_vertex_incident(v2,e2);
        ch.set_vertex_incident(v3,e3);

        fid=f;

        cg.reset_conflict_list_face(fid);

    }

    /** CHECK VISIBILITà f */
    for (j=0; j<P[0].size(); j++){
        point = cg.get_point(P[0][j]); // j-esimo punto in conflitto con l'half_edge 0 sull'orizzonte
        bool b = see(point, f, ch);
        if (b == true) cg.add_new_conflict(P[0][j], fid); // se point vede f, aggiungo il conflitto nel conflict graph
    }

    //ad ogni ciclo, il twin del nuovo e3 è il vecchio e2.
    for (i=1; i<horizon.size(); i++){ // per ogni edge  sull'orizzonte
        old_e2 = e2; // salvo e2 del passo precedente

        if (horizon_faces[i]==(horizon_faces[i-1])){ //nuova faccia
            /** Half Edge */
            // e1:
            extern_he = ch.get_half_edge(horizon[i]); // edge sull'orizzonte
            v2 = extern_he.get_from_vertex(); // v2: from di extern_he (to di e1!)
            v1 = extern_he.get_to_vertex(); // v1: to di extern_he (from di e1!)
            he1.set_from_vertex(v1);
            he1.set_to_vertex(v2);
            e1 = ch.add_half_edge(he1); // e1: id half_edge twin di extern_he

            // e2:
            he2.set_from_vertex(v2); // he2: da v2 a v3
            he2.set_to_vertex(v3);
            e2 = ch.add_half_edge(he2); // e2: id half_edge next di e1

            // e3:
            he3.set_from_vertex(v3); // he3: da v3 a v1
            he3.set_to_vertex(v1);
            e3 = ch.add_half_edge(he3); // e3: id half_edge prev di e1

            // relazioni half edge:
            ch.set_half_edge_next(e1, e2);
            ch.set_half_edge_next(e2, e3);
            ch.set_half_edge_next(e3, e1);
            ch.set_half_edge_prev(e2, e1);
            ch.set_half_edge_prev(e3, e2);
            ch.set_half_edge_prev(e1, e3);
            ch.set_half_edge_twin(horizon[i], e1);
            ch.set_half_edge_twin(e1, horizon[i]);
            ch.set_half_edge_twin(e3, old_e2);
            ch.set_half_edge_twin(old_e2, e3);

            //faccia
            ff.set_inner_half_edge(e1);
            f = ch.add_face(ff);

            //half edge incident face
            ch.set_half_edge_face(e1,f);
            ch.set_half_edge_face(e2,f);
            ch.set_half_edge_face(e3,f);

            // vertex incident edge
            ch.set_vertex_incident(v1,e1);
            ch.set_vertex_incident(v2,e2);

            fid = cg.add_new_face(f); // aggiungo f al conflict_graph
        }
        else { //riciclo faccia
            extern_he = ch.get_half_edge(horizon[i]); // edge sull'orizzonte
            e1=extern_he.get_twin();
            e2=ch.get_half_edge(e1).get_next();
            e3=ch.get_half_edge(e1).get_prev();
            ch.set_half_edge_to(e2,v3);
            ch.set_half_edge_from(e3,v3);
            he1=ch.get_half_edge(e1);
            f=he1.get_face();
            v1=he1.get_from_vertex();
            v2=he1.get_to_vertex();

            ch.set_half_edge_twin(e3, old_e2);
            ch.set_half_edge_twin(old_e2, e3);
            fid=f;
            cg.reset_conflict_list_face(fid);
            // vertex incident edge
            ch.set_vertex_incident(v1,e1);
            ch.set_vertex_incident(v2,e2);
        }

        /** CHECK VISIBILITà f */
        //fid = cg.add_new_face(f); // aggiungo f al conflict_graph
        for (j=0; j<P[i].size(); j++){
            point = cg.get_point(P[i][j]); // j-esimo punto in conflitto con l'half_edge 0 sull'orizzonte
            bool b = see(point, f, ch);
            if (b == true) cg.add_new_conflict(P[i][j], fid); // se point vede f, aggiungo il conflitto nel conflict graph
        }

    }

    ch.set_half_edge_twin(e2, old_e3); // collego e2 dell'ultima faccia con e3 della prima
    ch.set_half_edge_twin(old_e3, e2);
}


void copy_real_convex_hull(DCEL &old_ch, DCEL &new_ch, QVector<face_id>& deleted_faces, QVector<vertex_id>& deleted_vertex, QVector<edge_id>& deleted_edges)
{
    QVector<vertex_id> real_vertex;
    QVector<edge_id> real_edges;
    QVector<face_id> real_faces;
    edge_id he, new_he;
    vertex_id v, new_v;
    face_id f, new_f;
    int i, j;
    j=0;

    for (i=0; i<old_ch.get_vertex_n(); i++){
        if (deleted_vertex.empty() || i!=deleted_vertex[j]){
            new_ch.add_vertex(old_ch.get_vertex(i));
            real_vertex.push_back(i);
        }
        else
            j++;
    }
    j=0;
    for (i=0; i<old_ch.get_half_edge_n(); i++){
        if (deleted_edges.empty() || i!=deleted_edges[j]){
            new_ch.add_half_edge(old_ch.get_half_edge(i));
            real_edges.push_back(i);
        }
        else
            j++;
    }
    j=0;
    for (i=0; i<old_ch.get_faces_n(); i++){
        if (deleted_faces.empty() || i!=deleted_faces[j]){
            new_ch.add_face(old_ch.get_face(i));
            real_faces.push_back(i);
        }
        else
            j++;
    }
    //vertex
    for (i=0; i<new_ch.get_vertex_n(); i++){
        he=new_ch.get_vertex(i).get_incident_half_edge();
        new_he=binary_search(he, 0, real_edges.size()-1, real_edges);
        new_ch.set_vertex_incident(i, new_he);
    }
    //faces
    for (i=0; i<new_ch.get_faces_n(); i++){
        he=new_ch.get_face(i).get_inner_half_edge();
        new_he=binary_search(he, 0, real_edges.size()-1, real_edges);
        new_ch.set_face_inner_half_edge(i, new_he);
    }
    //edges
    for (i=0; i<new_ch.get_half_edge_n(); i++){
        //origin
        v=new_ch.get_half_edge(i).get_from_vertex();
        new_v=binary_search(v, 0, real_vertex.size()-1, real_vertex);
        new_ch.set_half_edge_from(i, new_v);
        //destination
        v=new_ch.get_half_edge(i).get_to_vertex();
        new_v=binary_search(v, 0, real_vertex.size()-1, real_vertex);
        new_ch.set_half_edge_to(i, new_v);
        //twin
        he=new_ch.get_half_edge(i).get_twin();
        new_he=binary_search(he, 0, real_edges.size()-1, real_edges);
        new_ch.set_half_edge_twin(i, new_he);
        //prev
        he=new_ch.get_half_edge(i).get_prev();
        new_he=binary_search(he, 0, real_edges.size()-1, real_edges);
        new_ch.set_half_edge_prev(i, new_he);
        //next
        he=new_ch.get_half_edge(i).get_next();
        new_he=binary_search(he, 0, real_edges.size()-1, real_edges);
        new_ch.set_half_edge_next(i, new_he);
        //face
        f=new_ch.get_half_edge(i).get_face();
        new_f=binary_search(f, 0, real_faces.size()-1, real_faces);
        new_ch.set_half_edge_face(i, new_f);
    }
}
