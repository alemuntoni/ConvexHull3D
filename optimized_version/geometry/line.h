/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Alessandro Muntoni (muntoni.alessandro@gmail.com)
 * @author Andreas Scalas (andreasscalas@gmail.com)
 */

#ifndef CG3_LINE_H
#define CG3_LINE_H

#include "point.h"

namespace cg3 {

/**
 * @ingroup cg3core
 * @brief The Line class
 */
class Line : public SerializableObject
{
public:
    Line();
    Line(const Pointd& p0, const Vec3& dir);
    const Vec3& dir() const;
    const Pointd& startingPoint() const;

    bool pointLiesOnLine(const Pointd &p) const;

    // SerializableObject interface
    void serialize(std::ofstream& binaryFile) const;
    void deserialize(std::ifstream& binaryFile);

    #ifdef CG3_OLD_NAMES_COMPATIBILITY
    inline const Vec3& getDir() const {return dir();}
    inline const Pointd& getStartingPoint() const {return startingPoint();}
    #endif

protected:
    Pointd p0;
    Vec3 _dir;
};

} //namespace cg3

//hash specialization
namespace std {

template <>
struct hash<cg3::Line> {
    size_t operator()(const cg3::Line& k) const;
};

} //namespace std


#endif // LINE_H
