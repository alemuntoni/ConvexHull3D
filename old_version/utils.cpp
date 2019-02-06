#include "utils.h"

/**
 * @brief complanar
 *  Dati 4 CGPointf, restituisce il determinante della matrice:
 *
 *      |                                |
 *      |  p0(x)    p0(y)    p0(z)    1  |
 *      |                                |
 *      |  p1(x)    p1(y)    p1(z)    1  |
 *      |                                |
 *      |  p2(x)    p2(y)    p2(z)    1  |
 *      |                                |
 *      |  p3(x)    p3(y)    p3(z)    1  |
 *      |                                |
 *
 *  che viene poi restituito.
 */
float complanar(CGPointf & p0, CGPointf & p1, CGPointf & p2, CGPointf & p3) {
    QVector4D v;
    QMatrix4x4 m;

    // prima riga
    v.setW(p0.getX());
    v.setX(p0.getY());
    v.setY(p0.getZ());
    v.setZ(1);
    m.setRow(0,v);

    // seconda riga
    v.setW(p1.getX());
    v.setX(p1.getY());
    v.setY(p1.getZ());
    v.setZ(1);
    m.setRow(1,v);

    // terza riga
    v.setW(p2.getX());
    v.setX(p2.getY());
    v.setY(p2.getZ());
    v.setZ(1);
    m.setRow(2,v);

    // quarta riga
    v.setW(p3.getX());
    v.setX(p3.getY());
    v.setY(p3.getZ());
    v.setZ(1);
    m.setRow(3,v);

    return m.determinant();
}


/**
 * @brief binary_search
 *  Effettua una ricerca binaria di n sull'array ordinato v.
 *  Restituisce -1 se n non è presente nell'array.
 * @param n:        numero da cercare
 * @param first:    indice del primo elemento dell'array
 * @param last:     indice dell'ultimo elemento dell'array
 * @param v:        array su cui effettuare la ricerca
 * @return          l'indice di n nell'array.
 *                  se n non è presente, restituisce -1.
 */
int binary_search(int n, int first, int last, QVector<int> & v) {
    int mid;
    if (first <= last){
        mid = (first + last) / 2;
        if (v[mid] < n) return (binary_search (n, mid+1, last, v) );
        if (v[mid] == n) return mid;
        if (v[mid] > n) return (binary_search (n, first, mid-1, v) );
    }
    return -1;
}


/**
 * @brief search_insert_index
 *  Effettua una ricerca binaria di n sull'array ordinato v,
 *  e restituisce l'indice del primo elemento nell'array
 *  >= a n.
 * @param n:        numero da cercare
 * @param first:    indice del primo elemento dell'array
 * @param last:     indice dell'ultimo elemento dell'array
 * @param v:        array su cui effettuare la ricerca
 * @return          l'indice del primo elemento >= a n.
 */
int search_insert_index(int n, int first, int last, QVector<int> & v) {
    int mid;
    if (first <= last){
        mid = (first + last) / 2;
        if (v[mid] < n) return (search_insert_index (n, mid+1, last, v) );
        if (v[mid] == n) return mid;
        if (v[mid] > n) return (search_insert_index (n, first, mid-1, v) );
    }
    return first;
}

/**
 * @brief merge
 *  dati due array ordinati, la funzione merge restituisce un array contenente tutti gli
 *  elementi di v1 e v2 ordinati e senza ripetizioni.
 * @param v1: array ordinato v1
 * @param v2: array ordinato v2
 * @return l'array ordinato contenente l'unione tra v1 e v2.
 */
QVector<int> merge(QVector<int> &v1, QVector<int> &v2){
    QVector<int> merge;
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
