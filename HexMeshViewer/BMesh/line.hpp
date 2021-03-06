/*
 * @Author: your name
 * @Date: 2020-10-05 20:58:37
 * @LastEditTime: 2021-01-29 23:02:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/line.hpp
 */
#ifndef __LINE_HPP__
#define __LINE_HPP__
#include "local_vector.hpp"
#include <algorithm>
#include <vector>
namespace BMesh {
class Line {
public:
    Line(Point3D a, Point3D b);
    ~Line();
    Line();
    Line(Line &rhs);
    Line(const Line &rhs);

public:
    Line &operator=(const Line &rhs);
    Line &operator=(Line &rhs);
    bool operator==(Line &rhs);
    bool operator!=(Line &rhs);
    bool isPointInLine(Point3D &p);

public:
    Lvector l;              // line direction vec
    std::vector<Point3D> v; // point in line
};
} // namespace BMesh
#endif