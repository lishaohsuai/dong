/*
 * @Author: your name
 * @Date: 2020-10-05 08:32:34
 * @LastEditTime: 2020-11-26 21:35:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/point.cc
 */
#include "point2d.hpp"
using namespace BMesh;
/**
 * @description: No argument constructor
 * @param {type}
 * @return {type}
 */
Point2D::Point2D() {
    x = 0;
    y = 0;
    id = 0;
    // id = count;
    // count++;
}

/**
 * @description: destructor
 * @param {type}
 * @return {type}
 */
Point2D::~Point2D() {}

/**
 * @description: There are parameter constructors
 * @param {type}
 * @return {type}
 */
Point2D::Point2D(double _x, double _y) : x(_x), y(_y) {
    id = Point2D::count;
    Point2D::count++;
}

/**
 * @description: Judge whether it is equal
 * @param {point}
 * @return {type}
 */
bool Point2D::operator==(Point2D &p) {
    if (p.x == x && p.y == y) {
        return true;
    } else {
        return false;
    }
}
/**
 * @description: sum point
 * @param {rhs}
 * @return {type}
 */
Point2D Point2D::operator+(Point2D &rhs) { return Point2D(this->x + rhs.x, this->y + rhs.y); }

/**
 * @description: reload +=
 * @param {type}
 * @return {type}
 */
Point2D &Point2D::operator+=(Point2D &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}
/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point2D &Point2D::operator-=(Point2D &rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point2D Point2D::operator-(Point2D &rhs) { return Point2D(this->x - rhs.x, this->y - rhs.y); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point2D Point2D::operator/(double val) { return Point2D(this->x / val, this->y / val); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point2D &Point2D::operator/=(double val) {
    this->x /= val;
    this->y /= val;
    return *this;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point2D Point2D::operator*(double val) { return Point2D(this->x * val, this->y * val); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
bool Point2D::operator<(Point2D &p) {
    if (std::abs(x - p.x) > ESP) {
        return x < p.x;
    } else {
        if (std::abs(y - p.y) > ESP) {
            return y < p.y;
        } else {
            return false;
        }
    }
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point2D &Point2D::operator=(const Point2D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->id = rhs.id;
    return *this;
}

Point2D &Point2D::operator=(Point2D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->id = rhs.id;
    return *this;
}

/**
 * @description: You get the vector that vec describes
 * @param {type}
 * @return {type}
 */
std::vector<double> Point2D::getVec() {
    std::vector<double> v;
    v.push_back(this->x);
    v.push_back(this->y);
    return v;
}

int Point2D::count = 0;

/**
 * @description: For the convenience of the output file
 * @param {type}
 * @return {type}
 */
std::string Point2D::getXY() { return std::to_string(x) + " " + std::to_string(y) + "\n"; }

/**
 * @description: copy constructor
 * @param {type}
 * @return {type}
 */
Point2D::Point2D(const Point2D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->id = rhs.id;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point2D::Point2D(Point2D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->id = rhs.id;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
void Point2D::setX(double x) { this->x = x; }
void Point2D::setY(double y) { this->y = y; }
