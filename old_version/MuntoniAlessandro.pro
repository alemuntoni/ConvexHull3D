TEMPLATE = app

TARGET = ConvexHull

QT += opengl

LIBS = -lglut -lGLU

QMAKE_CXXFLAGS += -std=c++11

DEPENDPATH += . dcel
INCLUDEPATH += . dcel

HEADERS +=  \
            engine.h \
            window_gl.h \
            dcel/DCEL.hh \
            dcel/structures.hh \
    timer.h \
    utils.h \
    dcel_operations.h \
    conflict_graph/point_node.h \
    conflict_graph/face_node.h \
    conflict_graph/conflict_graph.h

SOURCES +=  \
            engine.cpp \
            main.cpp \
            window_gl.cpp \
            dcel/DCEL.cpp \
    utils.cpp \
    dcel_operations.cpp \
    conflict_graph/point_node.cpp \
    conflict_graph/face_node.cpp \
    conflict_graph/conflict_graph.cpp


