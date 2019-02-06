/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Alessandro Muntoni (muntoni.alessandro@gmail.com)
 */

#include "dcel_face_iterators.h"
#include "dcel_vertex_iterators.h"
#include <geometry/transformations.h>
#include <geometry/utils3d.h>
#ifdef CG3_CGAL_DEFINED
#include <cg3/cgal/triangulation.h>
#endif

namespace cg3 {

/****************
 * Constructors *
 ****************/
/**
 * \~Italian
 * @brief Costruttore di default.
 *
 * Crea una faccia con:
 * - outer half edge settato a nullptr;
 * - nessun inner half edge;
 * - normale pari a (0, 0, 0);
 * - area pari a 0;
 * - id pari a 0;
 * - flag pari a 0.
 */
#ifdef NDEBUG
Dcel::Face::Face(Dcel& parent) :
    parent(&parent),
    _outerHalfEdge(nullptr),
    _area(0),
    _id(0),
    _flag(0)
{
    _innerHalfEdges.clear();
}
#else
Dcel::Face::Face() : _outerHalfEdge(nullptr), _area(0), _id(0), _flag(0){
    _innerHalfEdges.clear();
}
#endif

/**
 * \~Italian
 * @brief Distruttore vuoto.
 *
 * La classe Dcel dovrà occuparsi di eliminare tutti i riferimenti in essa contenuti (e quindi contenuti di conseguenza anche nella classe Dcel::Face).
 */
Dcel::Face::~Face(void)
{
}


/******************
 * Public Methods *
 ******************/

/**
 * \~Italian
 * @brief Funzione che verifica se la faccia è un triangolo
 * @return True se la faccia è un triangolo, false altrimenti
 */
bool Dcel::Face::isTriangle() const
{
    assert(_outerHalfEdge != nullptr && "Face's Outer HalfEdge is null.");
    assert(_outerHalfEdge->next() != nullptr && "HalfEdge's Next is null.");
    assert(_outerHalfEdge->next()->next() != nullptr && "HalfEdge's Next is null.");
    return (_outerHalfEdge == _outerHalfEdge->next()->next()->next());
}

bool Dcel::Face::isAdjacentTo(const Dcel::Face* ad) const
{
    for (const Dcel::HalfEdge* he : incidentHalfEdgeIterator()){
        assert(he != nullptr && "Next component of Previous HalfEdge is null.");
        assert(he->twin() != nullptr && "HalfEdge's Twin is null.");
        if (he->twin()->face() == ad) return true;
    }
    for (ConstInnerHalfEdgeIterator ihe = innerHalfEdgeBegin(); ihe != innerHalfEdgeEnd(); ++ihe){
        for (ConstIncidentHalfEdgeIterator heit = incidentHalfEdgeBegin(*ihe), hend = incidentHalfEdgeEnd(); heit != hend; ++heit){
            assert(*(heit) != nullptr && "Next component of Previous HalfEdge is null.");
            assert((*heit)->twin() != nullptr && "HalfEdge's Twin is null.");
            if ((*heit)->twin()->face() == ad) return true;
        }
    }
    return false;
}

bool Dcel::Face::isIncidentTo(const Dcel::Vertex* v) const
{
    for (const Dcel::HalfEdge* he : v->outgoingHalfEdgeIterator()) {
        assert(he != nullptr && "Twin component of an HalfEdge is null.");
        if (he->face() == this) return true;
    }
    return false;
}

/**
 * \~Italian
 * @brief Restituisce il numero di vertici incidenti alla faccia
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return Il numero di vertici incidenti alla faccia
 */
int Dcel::Face::numberIncidentVertices() const
{
    int n = 0;
    for (ConstIncidentVertexIterator vi = incidentVertexBegin(), vend = incidentVertexEnd(); vi != vend; ++vi) n++;
    return n;
}

/**
 * \~Italian
 * @brief Restituisce il numero di half edge incidenti alla faccia
 * @warning Utilizza Dcel::Face::ConstIncidentHalfEdgeIterator
 * @return Il numero di half edge incidenti alla faccia
 */
int Dcel::Face::numberIncidentHalfEdges() const
{
    int n = 0;
    for (ConstIncidentHalfEdgeIterator hei = incidentHalfEdgeBegin(), hend = incidentHalfEdgeEnd(); hei != hend; ++hei) n++;
    return n;
}

/**
 * \~Italian
 * @brief Calcola e restituisce il baricentro della faccia.
 *
 * Il baricentro è calcolato come media dei vertici incidenti alla faccia.
 *
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return Il baricentro della faccia.
 */
Pointd Dcel::Face::barycenter() const
{
    int n = 0;
    Pointd p;
    for (const Dcel::Vertex* v : incidentVertexIterator()){
        assert (v != nullptr && "HalfEdge's to vertex is null.");
        p += v->coordinate();
        n++;
    }
    p /= n;
    return p;
}

#ifdef CG3_CGAL_DEFINED
/**
 * \~Italian
 * @brief Dcel::Face::getTriangulation
 * @param triangles
 */
void Dcel::Face::triangulation(std::vector<std::array<const Dcel::Vertex*, 3> > &triangles) const
{
    // Taking all the coordinates on vectors
    std::vector<Pointd> borderCoordinates;
    std::vector< std::vector<Pointd> > innerBorderCoordinates;
    std::map<Pointd, const Dcel::Vertex*> pointsVerticesMap;
    for (const Dcel::HalfEdge* he : incidentHalfEdgeIterator()){
        assert(he != nullptr && "Next component of Previous HalfEdge is null.");
        assert(he->fromVertex() != nullptr && "HalfEdge's from vertex is null.");
        borderCoordinates.push_back(he->fromVertex()->coordinate());
        std::pair<const Dcel::Vertex*, const Dcel::Vertex*> pp;
        pp.first = he->fromVertex();
        pp.second = he->toVertex();
        pointsVerticesMap[he->fromVertex()->coordinate()] = he->fromVertex();
    }

    if (hasHoles()){
        innerBorderCoordinates.reserve(numberInnerHalfEdges());
        int i = 0;
        for (Dcel::Face::ConstInnerHalfEdgeIterator ihe = innerHalfEdgeBegin(), ihend = innerHalfEdgeEnd(); ihe != ihend; ++ihe, ++i){
            const Dcel::HalfEdge* he = *ihe;
            std::vector<Pointd> inner;
            for (Dcel::Face::ConstIncidentHalfEdgeIterator heit = incidentHalfEdgeBegin(he), hend = incidentHalfEdgeEnd(); heit != hend; ++heit) {
                assert(*(heit) != nullptr && "Next component of Previous HalfEdge is null.");
                assert((*heit)->fromVertex() != nullptr && "HalfEdge's from vertex is null.");
                inner.push_back((*heit)->fromVertex()->coordinate());
                std::pair<const Dcel::Vertex*, const Dcel::Vertex*> pp;
                pp.first = (*heit)->fromVertex();
                pp.second = (*heit)->toVertex();
                pointsVerticesMap[(*heit)->fromVertex()->coordinate()] = (*heit)->fromVertex();
            }
            innerBorderCoordinates.push_back(inner);
        }
    }

    #ifdef NDEBUG
    std::vector<std::array<Pointd, 3> > trianglesP = cgal::triangulate(parent->faceNormals[_id], borderCoordinates, innerBorderCoordinates);
    #else
    std::vector<std::array<Pointd, 3> > trianglesP = cgal::triangulate(_normal, borderCoordinates, innerBorderCoordinates);
    #endif

    triangles.clear();
    for (unsigned int i = 0; i < trianglesP.size(); ++i) {
            std::array<Pointd, 3> triangle = trianglesP[i];
            Pointd p1 = triangle[0];
            Pointd p2 = triangle[1];
            Pointd p3 = triangle[2];
            assert(pointsVerticesMap.find(p1) != pointsVerticesMap.end() && "Triangulation vertex not founded on original face.");
            assert(pointsVerticesMap.find(p2) != pointsVerticesMap.end() && "Triangulation vertex not founded on original face.");
            assert(pointsVerticesMap.find(p3) != pointsVerticesMap.end() && "Triangulation vertex not founded on original face.");
            const Dcel::Vertex* a = pointsVerticesMap[p1];
            const Dcel::Vertex* b = pointsVerticesMap[p2];
            const Dcel::Vertex* c = pointsVerticesMap[p3];
            std::array<const Dcel::Vertex*, 3> tuple = {a, b, c};
            triangles.push_back(tuple);
    }
}
#endif

/**
 * \~Italian
 * @brief Funzione toString di una faccia
 * @return Una stringa rappresentativa della faccia
 * @todo Da aggiornare
 */
std::string Dcel::Face::toString() const
{
    std::stringstream ss;

    #ifdef NDEBUG
    ss << "ID: " << _id << "; Normal: " << parent->faceNormals[_id] << "; Outer Component: ";
    #else
    ss << "ID: " << _id << "; Normal: " << _normal << "; Outer Component: ";
    #endif
    if (_outerHalfEdge != nullptr) ss << _outerHalfEdge->id();
    else ss << "nullptr";
    ss << "; N Inner Components: " << _innerHalfEdges.size() << "; Inner Components: "
       << innerComponentsToString() << ".";
    std::string s1 = ss.str();
    return s1;
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start. \n
 * È meno efficiente rispetto a Dcel::Face::constIncidentVertexBegin(const Dcel::HalfEdge* start).
 *
 * @param[in] start: vertice di partenza
 * @warning Se start non risulta essere incidente alla faccia (ossia non possiede un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexBegin(const Dcel::Vertex* start) const
{
    for (const Dcel::HalfEdge* he : start->incomingHalfEdgeIterator()) {
    //for (Dcel::Vertex::ConstIncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin(), hend = start->incomingHalfEdgeEnd(); heit!= hend; ++heit) {
        assert(he != nullptr && "Half Edge is null.");
        if (he->face() == this) return ConstIncidentVertexIterator(he, he, this);
    }
    assert(0 && "Start vertex is not incident to iterated face.");
    return ConstIncidentVertexIterator();
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start e fino al vertice end. \n
 * È meno efficiente rispetto a Dcel::Face::constIncidentVertexBegin(const Dcel::HalfEdge* start, const Dcel::HalfEdge* end).
 *
 * @param[in] start: vertice di partenza
 * @param[in] end: vertice di arrivo, \b non \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia (ossia non possiedono un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexBegin(const Dcel::Vertex* start, const Dcel::Vertex* end) const
{
    Vertex::ConstIncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin(), hend = start->incomingHalfEdgeEnd();
    while (heit!= hend && ((*heit)->face() != this)) {
        ++heit;
        assert((heit == hend || (*heit) != nullptr) && "Half Edge is null.");
    }
    assert((*heit)->face() == this && "Start vertex is not incident to iterated face.");
    const Dcel::HalfEdge* s = *heit;
    for (heit= end->incomingHalfEdgeBegin(), hend = end->incomingHalfEdgeEnd(); heit!= hend; ++heit){
        assert((*heit) != nullptr && "Half Edge is null.");
        if ((*heit)->face() == this) return ConstIncidentVertexIterator(s, *heit, this);
    }
    assert(0 && "End vertex is not incident to iterated face.");
    return ConstIncidentVertexIterator();
}

/**
 * \~Italian
 * @brief Funzione che aggiorna la normale alla faccia
 * @warning Funziona se e solo se la faccia è un triangolo
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return La normale alla faccia aggiornata
 */
Vec3 Dcel::Face::updateNormal()
{
    assert(_outerHalfEdge != nullptr && "Face's Outer HalfEdge is null.");
    Vertex* a = _outerHalfEdge->fromVertex();
    assert(a != nullptr && "HalfEdge's From Vertex is null.");
    Vertex* b = _outerHalfEdge->toVertex();
    assert(b != nullptr && "HalfEdge's To Vertex is null.");
    Vertex* c = _outerHalfEdge->next()->toVertex();
    assert(c != nullptr && "HalfEdge's To Vertex is null.");

    Vec3 normal;
    if (isTriangle()){
        normal = (b->coordinate() - a->coordinate()).cross(c->coordinate() - a->coordinate());
        if (normal == Vec3())
            std::cerr << "Warning: degenerate triangle; ID: " << id() << "\n";
        else
            normal.normalize();
    }
    else {
        bool end = false;
        HalfEdge* edge = _outerHalfEdge->next()->next();
        Vertex* first = a;
        while (areCollinear(a->coordinate(), b->coordinate(), c->coordinate()) && !end){
            a = b;
            b = c;
            c = edge->toVertex();
            edge = edge->next();
            if (a == first)
                end = true;
        }
        if (end) {
            normal = Vec3(0,0,0);
            std::cerr << "Warning: degenerate polygon; ID: " << id() << "\n";
        }
        else {
            normal = (b->coordinate() - a->coordinate()).cross(c->coordinate() - a->coordinate());
            assert(normal != Vec3(0,0,0));
            normal.normalize();

            std::vector<Pointd> pol;
            for (const Vertex* v : incidentVertexIterator())
                pol.push_back(v->coordinate());
            if (! isPolygonCounterClockwise(pol, normal))
                normal = -normal;
        }
    }
    #ifdef NDEBUG
    parent->faceNormals[_id] = normal;
    #else
    this->_normal = normal;
    #endif
    return normal;
}

/**
 * \~Italian
 * @brief Funzione che aggiorna l'area della faccia
 * @warning Funziona se e solo se la faccia è un triangolo
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return L'area della faccia aggiornata
 */
double Dcel::Face::updateArea()
{
    updateNormal();
    if (isTriangle()) {
        assert(_outerHalfEdge != nullptr && "Face's Outer HalfEdge is null.");
        assert(_outerHalfEdge->fromVertex() != nullptr && "HalfEdge's From Vertex is null.");
        assert(_outerHalfEdge->toVertex() != nullptr && "HalfEdge's To Vertex is null.");
        assert(_outerHalfEdge->prev() != nullptr && "HalfEdge's prev is null.");
        assert(_outerHalfEdge->prev()->fromVertex() != nullptr && "HalfEdge's From Vertex is null.");
        Pointd v1 = _outerHalfEdge->fromVertex()->coordinate();
        Pointd v2 = _outerHalfEdge->toVertex()->coordinate();
        Pointd v3 = _outerHalfEdge->prev()->fromVertex()->coordinate();
        _area = (((v3 - v1).cross(v2 - v1)).length() / 2);
    }
    #ifdef CG3_CGAL_DEFINED
    else {
        _area = 0;
        std::vector<std::array<const Dcel::Vertex*, 3> > t;

        triangulation(t);
        for (unsigned int i = 0; i <t.size(); ++i){
            std::array<const Dcel::Vertex*, 3> tr =  t[i];
            assert(tr[0] != nullptr && "Vertex is null.");
            assert(tr[1] != nullptr && "Vertex is null.");
            assert(tr[2] != nullptr && "Vertex is null.");
            Pointd v1 = tr[0]->coordinate();
            Pointd v2 = tr[1]->coordinate();
            Pointd v3 = tr[2]->coordinate();
            _area += (((v3 - v1).cross(v2 - v1)).length() / 2);
        }
    }
    #endif
    return _area;
}

/**
 * \~Italian
 * @brief Funzione che rimuove un inner half edge dalla faccia
 * @param[in] iterator: iteratore che punta all'inner half edge da eliminare
 */
void Dcel::Face::removeInnerHalfEdge(const Face::InnerHalfEdgeIterator& iterator)
{
    _innerHalfEdges.erase(iterator);
}

/**
 * \~Italian
 * @brief Funzione che rimuove un inner half edge dalla faccia
 *
 * È meno efficiente rispetto a Dcel::Face::removeInnerHalfEdge(const Face::innerHalfEdgeIterator &ihe).
 *
 * @param[in] halfEdge: inner half edge da eliminare
 * @return True se la rimozione è andata a buon fine, false altrimenti.
 */
bool Dcel::Face::removeInnerHalfEdge(const Dcel::HalfEdge* halfEdge)
{
    InnerHalfEdgeIterator i = std::find(_innerHalfEdges.begin(), _innerHalfEdges.end(), halfEdge);
    if (i != _innerHalfEdges.end()){
        _innerHalfEdges.erase(i);
        return true;
    }
    return false;
}

void Dcel::Face::removeAllInnerHalfEdges() {
    _innerHalfEdges.clear();
}

void Dcel::Face::invertOrientation()
{
    HalfEdge* first = _outerHalfEdge;
    HalfEdge* actual = first;
    HalfEdge* next;
    do{
        next = actual->next();

        Vertex* from = actual->fromVertex();
        if (from->incidentHalfEdge() == actual)
            from->setIncidentHalfEdge(actual->prev());
        actual->setFromVertex(actual->toVertex());
        actual->setToVertex(from);

        HalfEdge* prev = actual->prev();
        actual->setPrev(actual->next());
        actual->setNext(prev);

        actual = next;

    } while (actual != first);

    for (HalfEdge* inner : _innerHalfEdges){
        HalfEdge* first = inner;
        HalfEdge* actual = first;
        HalfEdge* next;
        do{
            next = actual->next();

            Vertex* from = actual->fromVertex();
            if (from->incidentHalfEdge() == actual)
                from->setIncidentHalfEdge(actual->prev());
            actual->setFromVertex(actual->toVertex());
            actual->setToVertex(from);

            HalfEdge* prev = actual->prev();
            actual->setPrev(actual->next());
            actual->setNext(prev);

            actual = next;

        } while (actual != first);
    }

    updateNormal();
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start. \n
 * È meno efficiente rispetto a Dcel::Face::incidentVertexBegin(const Dcel::HalfEdge* start).
 *
 * @param[in] start: vertice di partenza
 * @warning Se start non risulta essere incidente alla faccia (ossia non possiede un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexBegin(Dcel::Vertex* start)
{
    for (Dcel::HalfEdge* he : start->incomingHalfEdgeIterator()) {
    //for (Dcel::Vertex::ConstIncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin(), hend = start->incomingHalfEdgeEnd(); heit!= hend; ++heit) {
        assert(he != nullptr && "Half Edge is null.");
        if (he->face() == this) return IncidentVertexIterator(he, he, this);
    }
    assert(0 && "Start vertex is not incident to iterated face.");
    return IncidentVertexIterator();
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start e fino al vertice end. \n
 * È meno efficiente rispetto a Dcel::Face::incidentVertexBegin(const Dcel::HalfEdge* start, const Dcel::HalfEdge* end).
 *
 * @param[in] start: vertice di partenza
 * @param[in] end: vertice di arrivo, \b non \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia (ossia non possiedono un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexBegin(Dcel::Vertex* start, Dcel::Vertex* end)
{
    Vertex::IncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin(), hend = start->incomingHalfEdgeEnd();
    while (heit!= hend && ((*heit)->face() != this)) {
        ++heit;
        assert((heit == hend || (*heit) != nullptr) && "Half Edge is null.");
    }
    assert((*heit)->face() == this && "Start vertex is not incident to iterated face.");
    Dcel::HalfEdge* s = *heit;
    for (heit= end->incomingHalfEdgeBegin(), hend = end->incomingHalfEdgeEnd(); heit!= hend; ++heit){
        assert((*heit) != nullptr && "Half Edge is null.");
        if ((*heit)->face() == this) return IncidentVertexIterator(s, *heit, this);
    }
    assert(0 && "End vertex is not incident to iterated face.");
    return IncidentVertexIterator();
}


/*********************
 * Protected Methods *
 *********************/

/**
 * \~Italian
 * @brief Funzione che restituisce una stringa degli inner half edge
 * @return Una stringa rappresentativa degli inner half edge della faccia
 */
std::string Dcel::Face::innerComponentsToString() const
{
    std::stringstream ss;
    ss << "(";
    for (unsigned int i=0; i<_innerHalfEdges.size(); i++){
        if (_innerHalfEdges[i] != nullptr) {
            if (i != 0) ss << "; ";
            ss << _innerHalfEdges[i]->id();
        }
        else ss << "nullptr";
    }
    ss << ")";
    std::string s1 = ss.str();
    return s1;
}


std::ostream&operator<<(std::ostream& inputStream, const Dcel::Face* f)
{
    if (f == nullptr)
        inputStream << "null; ";
    else
        inputStream << f->toString() << "; ";
    return inputStream;
}

} //namespace cg3
