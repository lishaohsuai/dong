/*
 * @Author: your name
 * @Date: 2020-10-03 21:14:58
 * @LastEditTime: 2020-12-03 16:05:09
 * @LastEditors: Please set LastEditors
 * @Description: 图形学算法
 * @FilePath: /my_skel/header/geometry.hpp
 */
#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__
#include "face.hpp"
#include "triangle.hpp"
#include "rays.hpp"
#include "hex.hpp"
#include "line.hpp"
#include "node.hpp"
#include "omp.h"
#include "point.hpp"
#include "point2d.hpp"
#include <algorithm>
#include <limits.h>
#include <vector>
#define __OPENMP__
namespace BMesh {
class Node;
class Hex;
class Face;
class Geometry {
public:
    Geometry();
    ~Geometry();

public:
    static bool intersectionLine3DFace3D(const Line &l, const Face &f, Point3D &p);
    static bool intersectionRays3DTriangle(const Rays& l, const Triangle& t, Point3D& p);
    static bool intersectionLine3DTriangle(const Line& l, const Triangle& t, Point3D& p);
    static bool splitFaceUseIntersectionPoints(const Face &f, const std::vector<Point3D> &p,
                                               std::vector<std::pair<Point3D, Face>> &out);
    static unsigned int isAHexFaceContainBHexFace(Hex *a, Hex *b, std::vector<Point3D> &diffPoint,
                                                  std::vector<Point3D> &samePoint, int &ai, int &bi);
    static unsigned int isAFaceContainBFace(Face &a, Face &b, std::vector<Point3D> &samePoint);
    static bool splitHexUseALine(Hex &a, Hex &b, std::vector<Point3D> &diffPoint, std::vector<Point3D> &samePoint,
                                 int ai);
    // 多态
    static bool splitHexUseALine(Hex &a, Hex &b, Hex& c,std::vector<Point3D> &diffPoint, std::vector<Point3D> &samePoint,
                                 int ai, int bi, int& rlt);

public:
    static double angleTwoVector(Lvector &a, Lvector &b);
    static double aVecProjbVecLength(Lvector &a, Lvector &b);
    static bool intersectionPointInALine(const Line &l, const Point3D &p);
    static bool intersectionPointInALineNotSeg(const Line& l, const Point3D& p);
    static bool intersectionPointInAFace(const Face &f, const Point3D &p);
    static bool intersectionPointInARays(const Rays& l, const Point3D& p);
    static bool intersectionPointInATriangle(const Triangle& t, const Point3D& p);
    static double angleFourPoint(Point3D &a, Point3D &b, Point3D &c, Point3D &d);
    static bool changeLink(Node *a, Node *aChild, Node *addChild);
    static double calcAreaOfThreePoints(Point3D &a, Point3D &b, Point3D &c);
    static double distanceTwoPoint(Point3D &a, Point3D &b);
    static bool vecIsSameLike(Lvector &a, Lvector &b);
    static bool isFacetoFace(Face &a, Face &b);

private:
};
} // namespace BMesh

#endif