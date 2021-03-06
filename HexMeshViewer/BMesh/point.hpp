/*
 * @Author: your name
 * @Date: 2020-10-05 08:25:37
 * @LastEditTime: 2020-11-26 21:15:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/point.hpp
 */
#ifndef __POINT_HPP__
#define __POINT_HPP__
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
namespace BMesh {
class Point3D {
public:
    explicit Point3D();
    ~Point3D();
    explicit Point3D(double _x, double _y, double _z);
    Point3D(const Point3D &rhs);
    Point3D(Point3D &rhs);

public:
    bool operator==(Point3D &p);
    bool operator==(const Point3D &p);
    bool operator!=(Point3D& p);
    bool operator!=(const Point3D& p);
    bool operator<(Point3D &p);
    Point3D operator+(Point3D &rhs);
    Point3D operator+(const Point3D &rhs) const;
    Point3D operator-(Point3D &rhs);
    Point3D operator*(double val);
    Point3D operator/(double val);
    Point3D &operator+=(Point3D &rhs);
    Point3D &operator-=(Point3D &rhs);
    Point3D &operator/=(double val);
    Point3D &operator=(const Point3D &rhs);
    Point3D &operator=(Point3D &rhs);

public:
    std::vector<double> getVec();
    std::string getXYZ();

public:
    double x, y, z;
    static int count;
    int id;

private:
    const double ESP = 1.0e-7;
    
};
} // namespace BMesh
#endif