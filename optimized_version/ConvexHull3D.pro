TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

unix:!macx{
    QMAKE_CXXFLAGS += -O2 -DNDEBUG
}

include(find_eigen.pri)

#core
#geometry
HEADERS += \
    cg3lib.h \
    geometry/bounding_box.h \
    geometry/line.h \
    geometry/plane.h \
    geometry/point.h \
    geometry/segment.h \
    geometry/transformations.h \
    geometry/triangle.h \
    geometry/utils3d.h \
    geometry/2d/point2d.h \
    geometry/2d/segment2d.h \
    geometry/2d/utils2d.h

SOURCES += \
    geometry/bounding_box.tpp \
    geometry/line.cpp \
    geometry/plane.cpp \
    geometry/point.tpp \
    geometry/segment.tpp \
    geometry/transformations.cpp \
    geometry/triangle.tpp \
    geometry/utils3d.tpp \
    geometry/2d/point2d.tpp \
    geometry/2d/utils2d.tpp

#io
HEADERS += \
    io/load_save_file.h \
    io/serializable_object.h \
    io/serialize.h \
    io/serialize_eigen.h \
    io/serialize_qt.h \
    io/serialize_std.h

SOURCES += \
    io/load_save_file.tpp \
    io/serialize.tpp \
    io/serialize_eigen.tpp \
    io/serialize_qt.tpp \
    io/serialize_std.tpp

#utilities
HEADERS += \
    utilities/color.h \
    utilities/comparators.h \
    utilities/const.h \
    utilities/eigen.h \
    utilities/hash.h \
    $utilities/map.h \
    utilities/nested_initializer_lists.h \
    utilities/pair.h \
    utilities/set.h \
    utilities/string.h \
    utilities/system.h \
    utilities/timer.h \
    utilities/tokenizer.h \
    utilities/vector.h \
    utilities/utils.h

SOURCES += \
    utilities/color.tpp \
    utilities/eigen.tpp \
    utilities/hash.tpp \
    utilities/map.tpp \
    utilities/pair.tpp \
    utilities/set.tpp \
    utilities/string.tpp \
    utilities/system.tpp \
    utilities/timer.tpp \
    utilities/tokenizer.tpp \
    utilities/vector.tpp \
    utilities/utils.tpp

#dcel
HEADERS += \
    dcel/dcel.h \
    dcel/dcel_face.h \
    dcel/dcel_face_iterators.h \
    dcel/dcel_half_edge.h \
    dcel/dcel_iterators.h \
    dcel/dcel_struct.h \
    dcel/dcel_vertex.h \
    dcel/dcel_vertex_iterators.h

SOURCES += \
    dcel/dcel_face.cpp \
    dcel/dcel_half_edge.cpp \
    dcel/dcel_vertex.cpp \
    dcel/dcel_struct.cpp \
    dcel/dcel_inline.tpp \
    dcel/dcel_half_edge_inline.tpp \
    dcel/dcel_face_iterators_inline.tpp \
    dcel/dcel_iterators_inline.tpp \
    dcel/dcel_vertex_inline.tpp \
    dcel/dcel_vertex_iterators_inline.tpp \
    dcel/dcel_face_inline.tpp

#Bipartite graph

HEADERS += \
    bipartite_graph/bipartite_graph.h \
    bipartite_graph/bipartite_graph_iterators.h \
    bipartite_graph/undirected_node.h

SOURCES += \
    bipartite_graph/bipartite_graph.tpp \
    bipartite_graph/bipartite_graph_iterators.tpp

#Convex Hull
HEADERS += \
    convex_hull/convexhull.h

SOURCES += \
    convex_hull/convexhull.tpp

SOURCES += \
        main.cpp
