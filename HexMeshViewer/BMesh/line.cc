/*
 * @Author: your name
 * @Date: 2020-10-05 21:02:30
 * @LastEditTime: 2021-03-18 16:57:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/line.cc
 */
#include "line.hpp"
using namespace BMesh;
/**
 * @description:
 * @param {type}
 * @return {type}
 */
Line::Line(Point3D a, Point3D b) {
    if (a.x >= b.x) {
        l = Lvector(a, b);
    }
    else {
        l = Lvector(b, a);
    }
    v.push_back(a);
    v.push_back(b);
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Line::~Line() {}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Line::Line() { l = Lvector(0, 0, 0); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Line::Line(Line &rhs) {
    l = rhs.l;
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Line::Line(const Line &rhs) {
    l = rhs.l;
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Line &Line::operator=(const Line &rhs) {
    this->l = rhs.l;
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
    return *this;
}
Line &Line::operator=(Line &rhs) {
    this->l = rhs.l;
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
    return *this;
}

/**
 * @description: 判断线段是否相等
 * @param {type}
 * @return {type}
 */
bool Line::operator==(Line &rhs) {
    for (int i = 0; i < this->v.size(); i++) {
        if (std::find(rhs.v.begin(), rhs.v.end(), this->v[i]) == rhs.v.end()) {
            return false;
        }
    }
    return true;
}

/**
 * @description: line not equal
 * @param {type}
 * @return {type}
 */
bool Line::operator!=(Line &rhs) {
    if ((*this) == rhs) {
        return false;
    }
    return true;
}

/**
 * @description: Is the point online
 * If there is a line, then the vector formed by a point and any point on the line is either equal or opposite
 * @param {type}
 * @return {type}
 */
bool Line::isPointInLine(Point3D &p) {
    Lvector a(p, v[0]);
    a = a.normalize();
    Lvector b(p, v[1]);
    b = b.normalize();
    Lvector c = b.getOppoVec();
    if (a == b || a == c) {
        return true;
    }
    return false;
}
