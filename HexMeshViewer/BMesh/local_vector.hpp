/*
 * @Author: your name
 * @Date: 2020-10-05 08:41:31
 * @LastEditTime: 2020-11-26 21:14:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/local_vector.hpp
 */
#ifndef __LOCAL_VECTOR_HPP__
#define __LOCAL_VECTOR_HPP__
#include "point.hpp"
#include <iostream>
namespace BMesh {
class Lvector {
public:
    Lvector();
    Lvector(double _x, double _y, double _z);
    Lvector(Point3D a, Point3D b);
    ~Lvector();

public:
    bool operator==(Lvector &p);
    bool operator==(const Lvector &p);
    bool operator<(Lvector &p);
    Lvector operator+(Lvector &rhs);
    Point3D operator+(const Point3D &rhs) const;
    Lvector operator-(Lvector &rhs);
    Lvector operator*(double val);
    Lvector operator*(double val) const;
    double operator*(Lvector &rhs);
    double operator*(const Lvector &rhs);
    double operator*(const Lvector &rhs) const;
    Lvector operator/(double val);
    Lvector operator%(Lvector &rhs);
    Lvector operator+=(Lvector &rhs);
    Lvector operator-=(Lvector &rhs);
    bool operator!=(Lvector &rhs);
    Lvector operator/=(double val);
    Lvector operator=(Lvector &rhs);
    Lvector operator=(const Lvector &rhs);

public:
    double norm();
    double squaredNorm();
    Lvector normalize();
    Lvector getNormalize();
    void printXYZ();
    std::string getXYZ();
    std::string getXYZ() const;
    Lvector getOppoVec();
    Lvector orthogonalVec() const;

public:
    double x, y, z;
    std::vector<Point3D> v;

private:
    const double ESP = 1.0e-7;
};
} // namespace BMesh

#endif