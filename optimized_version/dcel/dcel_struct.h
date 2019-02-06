/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Alessandro Muntoni (muntoni.alessandro@gmail.com)
 */

#ifndef CG3_DCEL_STRUCT_H
#define CG3_DCEL_STRUCT_H

#include <geometry/bounding_box.h>
#include <utilities/color.h>

#ifdef  CG3_EIGENMESH_DEFINED
namespace cg3 {
    class SimpleEigenMesh;
    class EigenMesh;
} //namespace cg3
#endif

#ifdef CG3_CINOLIB_DEFINED
#include <cinolib/meshes/trimesh.h>
#endif //CG3_CINOLIB_DEFINED

namespace cg3 {

/**
 * @class Dcel
 *
 * Main Dcel Page: @ref DcelPage
 *
 * @brief Double Connected Edge List data structure.
 *
 * The Dcel class is composed by three main lists of:
 * - Dcel::Vertex*
 * - Dcel::HalfEdge*
 * - Dcel::Face*
 *
 * The data structure is half-edge based: all the local adjacence and incidence relations are stored inside the Dcel::HalfEdge.
 * It is possible to modify these relations by simply use set methods. Dcel::Vertex and Dcel::Face contain some pointers to incident
 * half edges (Dcel::Vertex contains an outgoing half edge, Dcel::Face contains an incident outer half edge and one inner half edge
 * for every hole of the face), all the other adjacent/incident relations can be obtained through the half edges.
 * Dcel allows to access all its elements through iterators. For example, we can iterate through all the vertices of a Dcel d in the
 * following way:
 * \code{.cpp}
 * for (Dcel::VertexIterator vit = d.vertexBegin(); vit != d.vertexEnd(); ++vit){
 *     Dcel::Vertex* v = *vit;
 *     // use v here
 * }
 * \endcode
 *
 * or:
 *
 * \code{.cpp}
 * for (Dcel::Vertex* v : d.vertexIterator()){
 *     // use v here
 * }
 * \endcode
 *
 * We can do the same thing with Dcel::HalfEdge and Dcel::Face. For const Dcel, you can use const iterators.
 * Dcel::Vertex and Dcel::Face classes have also other type of iterators (which are mostly circular iterators)
 * that allows to access to incident/adjacent elements. See the documentation for all the specific iterators.
 */

class Dcel : public SerializableObject
{
public:

    /*********************
    * Associated Classes *
    **********************/

    class Vertex;
    class Face;
    class HalfEdge;

    /************
    * Iterators *
    *************/

    class VertexIterator;
    class ConstVertexIterator;
    class HalfEdgeIterator;
    class ConstHalfEdgeIterator;
    class FaceIterator;
    class ConstFaceIterator;
    class ConstVertexRangeBasedIterator;
    class ConstHalfEdgeRangeBasedIterator;
    class ConstFaceRangeBasedIterator;
    class VertexRangeBasedIterator;
    class HalfEdgeRangeBasedIterator;
    class FaceRangeBasedIterator;

    /***************
    * Constructors *
    ****************/

    Dcel();
    Dcel(const char* filename);
    Dcel(const std::string& filename);
    Dcel(const Dcel& dcel);
    Dcel(Dcel&& dcel);
    #ifdef  CG3_EIGENMESH_DEFINED
    Dcel(const cg3::SimpleEigenMesh &eigenMesh);
    Dcel(const cg3::EigenMesh &eigenMesh);
    #endif // CG3_EIGENMESH_DEFINED
    #ifdef CG3_CINOLIB_DEFINED
    Dcel(const cinolib::Trimesh<> &trimesh);
    #endif //CG3_CINOLIB_DEFINED
    ~Dcel();

    /************************
    * Public Inline Methods *
    *************************/

    const Vertex* vertex(unsigned int idVertex)          const;
    const HalfEdge* halfEdge(unsigned int idHalfEdge)    const;
    const Face* face(unsigned int idFace)                const;
    BoundingBox boundingBox()                            const;
    inline unsigned int numberVertices()        const;
    inline unsigned int numberHalfEdges()       const;
    inline unsigned int numberFaces()           const;
    inline bool contains(const Vertex* v)               const;
    inline bool contains(const HalfEdge* he)            const;
    inline bool contains(const Face* f)                 const;
    inline ConstVertexIterator vertexBegin()       const;
    inline ConstVertexIterator vertexEnd()         const;
    inline ConstHalfEdgeIterator halfEdgeBegin()   const;
    inline ConstHalfEdgeIterator halfEdgeEnd()     const;
    inline ConstFaceIterator faceBegin()           const;
    inline ConstFaceIterator faceEnd()             const;
    inline const ConstVertexRangeBasedIterator vertexIterator() const;
    inline const ConstHalfEdgeRangeBasedIterator halfEdgeIterator() const;
    inline const ConstFaceRangeBasedIterator faceIterator() const;

    Vertex* vertex(unsigned int idVertex);
    HalfEdge* halfEdge(unsigned int idHalfEdge);
    Face* face(unsigned int idFace);
    VertexIterator deleteVertex(const VertexIterator& vit);
    HalfEdgeIterator deleteHalfEdge(const HalfEdgeIterator& heit);

    FaceIterator deleteFace(const FaceIterator& fit);
    inline VertexIterator vertexBegin();
    inline VertexIterator vertexEnd();
    inline HalfEdgeIterator halfEdgeBegin();
    inline HalfEdgeIterator halfEdgeEnd();
    inline FaceIterator faceBegin();
    inline FaceIterator faceEnd();
    inline VertexRangeBasedIterator vertexIterator();
    inline HalfEdgeRangeBasedIterator halfEdgeIterator();
    inline FaceRangeBasedIterator faceIterator();

    /*****************
    * Public Methods *
    ******************/

    bool deleteVertex (Vertex* v);
    bool deleteVertex (unsigned int vid);
    bool vertexBelongsToThis(const Vertex* v)               const;
    bool halfEdgeBelongsToThis(const HalfEdge* he)          const;
    bool faceBelongsToThis(const Face* f)                   const;
    bool isTriangleMesh()                                   const;
    double surfaceArea()                                 const;
    Pointd barycenter()                                  const;
    double averageHalfEdgesLength()                      const;
    bool saveOnObj(const std::string& fileNameObj)             const;
    bool saveOnPly(const std::string& fileNamePly)             const;
    void saveOnDcelFile(const std::string& fileNameDcel)           const;

    Vertex* addVertex(const Pointd& p = Pointd(), const Vec3& n = Vec3(), const Color &c = Color(128, 128, 128));
    HalfEdge* addHalfEdge();
    Face* addFace(const Vec3& n = Vec3(), const Color& c = Color(128,128,128));
    bool deleteHalfEdge (HalfEdge* he);
    bool deleteHalfEdge (unsigned int heid);
    bool deleteFace (Face* f);
    bool deleteFace(unsigned int fid);
    void invertFaceOrientations();
    void deleteUnreferencedVertices();
    void deleteDuplicatedVertices();
    void updateFaceNormals();
    void updateVertexNormals();
    BoundingBox updateBoundingBox();
    void setColor(const Color &c);
    void scale(double scaleFactor);
    void scale(const cg3::Vec3& scaleVector);
    void scale(const BoundingBox &newBoundingBox);
    #ifdef CG3_WITH_EIGEN
    void rotate(const Eigen::Matrix3d& matrix);
    void rotate(const Eigen::Matrix3d& matrix, const Pointd& centroid);
    #endif
    void rotate(const Vec3& axis, double angle, const Pointd& centroid = Pointd());
    void rotate(double matrix[3][3], const Pointd& centroid = Pointd());
    void translate(const Vec3 &c);
    void recalculateIds();
    void resetFaceColors();
    void clear();
    #ifdef  CG3_CGAL_DEFINED
    unsigned int triangulateFace(uint idf);
    void triangulate();
    #endif
    bool loadFromFile(const std::string& filename);
    bool loadFromObj(const std::string& filename);
    bool loadFromPly(const std::string& filename);
    bool loadFromDcelFile(const std::string& filename);

    void swap(Dcel& d);
    void merge(const Dcel& d);
    void merge(Dcel&& d);

    Dcel& operator= (Dcel dcel);

    // SerializableObject interface
    void serialize(std::ofstream& binaryFile) const;
    void deserialize(std::ifstream& binaryFile);

    #ifdef CG3_OLD_NAMES_COMPATIBILITY
    inline const Vertex* getVertex(unsigned int v) const {return vertex(v);}
    inline Vertex* getVertex(unsigned int v) {return vertex(v);}
    inline const HalfEdge* getHalfEdge(unsigned int h) const {return halfEdge(h);}
    inline HalfEdge* getHalfEdge(unsigned int h) {return halfEdge(h);}
    inline const Face* getFace(unsigned int f) const {return face(f);}
    inline Face* getFace(unsigned int f) {return face(f);}
    inline BoundingBox getBoundingBox() const {return boundingBox();}
    inline unsigned int getNumberVertices() const {return numberVertices();}
    inline unsigned int getNumberHalfEdges() const {return numberHalfEdges();}
    inline unsigned int getNumberFaces() const {return numberFaces();}
    inline double getSurfaceArea() const {return surfaceArea();}
    Pointd getBarycenter() const {return barycenter();}
    double getAverageHalfEdgesLength() const {return averageHalfEdgesLength();}
    #endif

protected:

    /*************
    * Attributes *
    **************/

    std::vector<Vertex* >   vertices;
    std::vector<HalfEdge* > halfEdges;
    std::vector<Face* >     faces;
    std::set<int>           unusedVids;
    std::set<int>           unusedHeids;
    std::set<int>           unusedFids;
    unsigned int            nVertices;
    unsigned int            nHalfEdges;
    unsigned int            nFaces;
    BoundingBox             bBox;

    //Data
    #ifdef NDEBUG
    std::vector<Pointd> vertexCoordinates;
    std::vector<Vec3> vertexNormals;
    std::vector<Color> vertexColors;
    std::vector<Vec3> faceNormals;
    std::vector<Color> faceColors;
    #endif

    /******************
    * Private Methods *
    *******************/

    Vertex* addVertex(int id);
    HalfEdge* addHalfEdge(int id);
    Face* addFace(int id);

    std::vector<const Vertex*> makeSingleBorder(const Face *f)     const;
    void toStdVectors(
            std::vector<double> &vertices,
            std::vector<double> &verticesNormals,
            std::vector<int> &faces,
            std::vector<unsigned int> &faceSizes,
            std::vector<float> &faceColors) const;

    void afterLoadFile(
            const std::list<double>& coords,
            const std::list<unsigned int>& faces,
            int mode, const std::list<double>& vnorm,
            const std::list<Color>& vcolor,
            const std::list<Color>& fcolor,
            const std::list<unsigned int>& fsizes);

    #ifdef  CG3_EIGENMESH_DEFINED
    void copyFrom(const SimpleEigenMesh &eigenMesh);
    void copyFrom(const EigenMesh &eigenMesh);
    #endif // CG3_EIGENMESH_DEFINED
    #ifdef CG3_CINOLIB_DEFINED
    void copyFrom(const cinolib::Trimesh<> &trimesh);
    #endif //CG3_CINOLIB_DEFINED
};

void swap(Dcel& d1, Dcel& d2);

Dcel merge(const Dcel& d1, const Dcel& d2);

} //namespace cg3

#include "dcel_inline.tpp"

#endif // CG3_DCEL_STRUCT_H
