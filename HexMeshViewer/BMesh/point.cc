/*
 * @Author: your name
 * @Date: 2020-10-05 08:32:34
 * @LastEditTime: 2020-11-27 09:42:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/point.cc
 */
#include "point.hpp"
using namespace BMesh;
/**
 * @description: No argument constructor
 * @param {type}
 * @return {type}
 */
Point3D::Point3D() {
    x = 0;
    y = 0;
    z = 0;
    id = 0;
    // id = count;
    // count++;
}

/**
 * @description: destructor
 * @param {type}
 * @return {type}
 */
Point3D::~Point3D() {}

/**
 * @description: There are parameter constructors
 * @param {type}
 * @return {type}
 */
Point3D::Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {
    id = Point3D::count;
    Point3D::count++;
}

/**
 * @description: Judge whether it is equal
 * @param {point}
 * @return {type}
 */
bool Point3D::operator==(Point3D &p) {
    if (std::fabs(this->x - p.x) + std::fabs(this->y - p.y) + std::fabs(this->z - p.z) < ESP) {
        return true;
    }
    //if (this->getXYZ() == p.getXYZ()) {
    //    return true;
    //}
    return false;
}

bool Point3D::operator==(const Point3D &p) {
    Point3D c = p;
    //if (this->getXYZ() == c.getXYZ()) {
    //    return true;
    //}
    if (std::fabs(this->x - c.x) + std::fabs(this->y - c.y) + std::fabs(this->z - c.z) < ESP) {
        return true;
    }
    return false;
}


/**
 * @description: Judge whether it is not equal
 * @param {point}
 * @return {type}
 */
bool Point3D::operator!=(Point3D& p) {
    if (*this == p) {
        return false;
    }
    return true;
}

bool Point3D::operator!=(const Point3D& p) {
    if (*this == p) {
        return false;
    }
    return true;
}

/**
 * @description: sum point
 * @param {rhs}
 * @return {type}
 */
Point3D Point3D::operator+(Point3D &rhs) {
    return Point3D(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

/**
 * @description: reload +=
 * @param {type}
 * @return {type}
 */
Point3D &Point3D::operator+=(Point3D &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D &Point3D::operator-=(Point3D &rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D Point3D::operator-(Point3D &rhs) {
    return Point3D(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D Point3D::operator/(double val) { return Point3D(this->x / val, this->y / val, this->z / val); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D &Point3D::operator/=(double val) {
    this->x /= val;
    this->y /= val;
    this->z /= val;
    return *this;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D Point3D::operator*(double val) { return Point3D(this->x * val, this->y * val, this->z * val); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
bool Point3D::operator<(Point3D &p) {
    if (std::abs(x - p.x) > ESP) {
        return x < p.x;
    } else {
        if (std::abs(y - p.y) > ESP) {
            return y < p.y;
        } else {
            if (std::abs(z - p.z) > ESP) {
                return z < p.z;
            } else {
                return false;
            }
        }
    }
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D &Point3D::operator=(const Point3D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->id = rhs.id;
    return *this;
}

Point3D &Point3D::operator=(Point3D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->id = rhs.id;
    return *this;
}

/**
 * @description: You get the vector that vec describes
 * @param {type}
 * @return {type}
 */
std::vector<double> Point3D::getVec() {
    std::vector<double> v;
    v.push_back(this->x);
    v.push_back(this->y);
    v.push_back(this->z);
    return v;
}

int Point3D::count = 0;

/**
 * @description: For the convenience of the output file
 * @param {type}
 * @return {type}
 */
std::string Point3D::getXYZ() {
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "\n";
}

/**
 * @description: copy constructor
 * @param {type}
 * @return {type}
 */
Point3D::Point3D(const Point3D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->id = rhs.id;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D::Point3D(Point3D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->id = rhs.id;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D Point3D::operator+(const Point3D &rhs) const {
    return Point3D(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}
