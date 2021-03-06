/*
 * @Author: your name
 * @Date: 2020-10-05 21:30:52
 * @LastEditTime: 2020-11-26 21:05:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/face.hpp
 */
#ifndef __FACE_HPP__
#define __FACE_HPP__
#include "line.hpp"
#include "local_vector.hpp"
#include "point.hpp"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
const double MAXDATA = 10000000;
const double MINDATA = -MAXDATA;
const double PI = 3.1415926;
const double ESP = 0.000001;
namespace BMesh {
class Face {
public:
    Face(Point3D a, Point3D b, Point3D c, Point3D d);
    Face(Line a, Line b);
    explicit Face();
    Face(Face &rhs);
    Face(const Face &rhs);
    ~Face();

public:
    Face &operator=(const Face &rhs);
    Face &operator=(Face &rhs);
    bool operator==(Face &rhs);

public:
    void setName(std::string name);
    std::string getPointsString();
    Point3D getMidFacePoint();
    double getArea();
    std::vector<Point3D> getOtherPointFromAFace(std::vector<Point3D> &rhs);
    bool isInTheFace(Point3D &a);
    void genFrame();

private:
    double calcAreaOfThreePoints(Point3D &a, Point3D &b, Point3D &c);
    double angleTwoVector(Lvector &a, Lvector &b);

public:
    Lvector n;              // Normal vector of surface
    std::vector<Point3D> v; // The point on the surface
    std::string name;
    int id;
    Point3D smallPoint;
    Point3D bigPoint;
};
} // namespace BMesh

#endif