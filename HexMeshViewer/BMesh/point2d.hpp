/*
 * @Author: your name
 * @Date: 2020-10-05 08:25:37
 * @LastEditTime: 2020-11-26 21:15:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/point.hpp
 */
#ifndef __POINT2D_HPP__
#define __POINT2D_HPP__
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
namespace BMesh {
class Point2D {
public:
    explicit Point2D();
    ~Point2D();
    explicit Point2D(double _x, double _y);
    Point2D(const Point2D &rhs);
    Point2D(Point2D &rhs);

public:
    bool operator==(Point2D &p);
    bool operator<(Point2D &p);
    Point2D operator+(Point2D &rhs);
    Point2D operator-(Point2D &rhs);
    Point2D operator*(double val);
    Point2D operator/(double val);
    Point2D &operator+=(Point2D &rhs);
    Point2D &operator-=(Point2D &rhs);
    Point2D &operator/=(double val);
    Point2D &operator=(const Point2D &rhs);
    Point2D &operator=(Point2D &rhs);

public:
    std::vector<double> getVec();
    std::string getXY();
    void setX(double x);
    void setY(double y);

public:
    double x, y;
    static int count;
    int id;

private:
    const double ESP = 1.0e-7;
};
} // namespace BMesh
#endif