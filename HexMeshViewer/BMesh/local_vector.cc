/*
 * @Author: your name
 * @Date: 2020-10-05 09:11:59
 * @LastEditTime: 2020-11-26 21:27:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/local_vector.cc
 */
#include "local_vector.hpp"
using namespace BMesh;
/**
 * @description:
 * @param {type}
 * @return {type}
 */
Lvector::Lvector() {}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Lvector::~Lvector() {}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Lvector::Lvector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

/**
 * @description: Judge whether it is equal or not
 * @param {Input point}
 * @return {type}
 */
bool Lvector::operator==(Lvector &p) {
    if (this->getXYZ() == p.getXYZ()) {
        return true;
    }
    return false;
}

/**
 * @description: Judge whether it is equal or not
 * @param {Input point}
 * @return {type}
 */
bool Lvector::operator==(const Lvector &p) {
    if (this->getXYZ() == p.getXYZ()) {
        return true;
    }
    return false;
}

/**
 * @description: Judge whether it is equal or not
 * @param {type}
 * @return {type}
 */
bool Lvector::operator!=(Lvector &rhs) {
    if (this->getXYZ() != rhs.getXYZ()) {
        return true;
    }
    return false;
}

/**
 * @description: Summation of points
 * @param {rhs}
 * @return {type}
 */
Lvector Lvector::operator+(Lvector &rhs) { return Lvector(x + rhs.x, y + rhs.y, z + rhs.z); }

/**
 * @description: Realize cross product of vector
 * @param {type}
 * @return {type}
 */
Lvector Lvector::operator%(Lvector &rhs) {
    return Lvector(this->y * rhs.z - this->z * rhs.y, this->z * rhs.x - this->x * rhs.z,
                   this->x * rhs.y - this->y * rhs.x);
}

/**
 * @description: reload +=
 * @param {type}
 * @return {type}
 */
Lvector Lvector::operator+=(Lvector &rhs) {
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
Lvector Lvector::operator-=(Lvector &rhs) {
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
Lvector Lvector::operator-(Lvector &rhs) { return Lvector(x - rhs.x, y - rhs.y, z - rhs.z); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Lvector Lvector::operator/(double val) { return Lvector(x / val, y / val, z / val); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Lvector Lvector::operator/=(double val) {
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
Lvector Lvector::operator*(double val) { return Lvector(x * val, y * val, z * val); }

/**
 * @description:
 * @param {type}
 * @return {type}
 */
bool Lvector::operator<(Lvector &p) {
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
 * @description: Finding the module length of a vector
 * @param {type}
 * @return {type}
 */
double Lvector::norm() { return std::sqrt(x * x + y * y + z * z); }

/**
 * @description: unit length of the vector
 * @param {type}
 * @return {type}
 */
Lvector Lvector::normalize() {
    double norm = this->norm();
    this->x /= norm;
    this->y /= norm;
    this->z /= norm;
    return Lvector(this->x, this->y, this->z);
}

Lvector Lvector::getNormalize() {
    double norm = this->norm();
    return Lvector(this->x / norm, this->y / norm, this->z / norm);
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Lvector Lvector::operator=(Lvector &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        this->v.push_back(rhs.v[i]);
    }
    return *this;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Lvector Lvector::operator=(const Lvector &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        this->v.push_back(rhs.v[i]);
    }
    return *this;
}

/**
 * @description: Two points are used to construct VEC (AB) vector from a to B
 * @param {type}
 * @return {type}
 */
Lvector::Lvector(Point3D a, Point3D b) {
    this->x = b.x - a.x;
    this->y = b.y - a.y;
    this->z = b.z - a.z;
    v.push_back(a);
    v.push_back(b);
}

/**
 * @description: Constructing point multiplication of vectors
 * @param {type}
 * @return {type}
 */
double Lvector::operator*(Lvector &rhs) { return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z; }

/**
 * @description: Constructing point multiplication of vectors
 * @param {type}
 * @return {type}
 */
double Lvector::operator*(const Lvector &rhs) { return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z; }

double Lvector::operator*(const Lvector &rhs) const {
    return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
}

Lvector Lvector::operator*(double val) const { return Lvector(x * val, y * val, z * val); }

/**
 * @description: A vector plus a point is still a point
 * @param {type}
 * @return {type}
 */
Point3D Lvector::operator+(const Point3D &rhs) const {
    return Point3D(rhs.x + this->x, rhs.y + this->y, rhs.z + this->z);
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
void Lvector::printXYZ() { std::cout << "[DEBUG printXYZ]" << x << " " << y << " " << z << std::endl; }

/**
 * @description: get string xyz
 * @param {type}
 * @return {type}
 */
std::string Lvector::getXYZ() {
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "\n";
}
std::string Lvector::getXYZ() const {
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "\n";
};

/**
 * @description: Get the opposite vector
 * @param {type}
 * @return {type}
 */
Lvector Lvector::getOppoVec() { return Lvector(this->x * -1, this->y * -1, this->z * -1); }

/**
 * @description: Get the squaredNorm
 * @param {type}
 * @return {type}
 */
double Lvector::squaredNorm() {
    return x * x + y * y + z * z;
}


/**
 * @description: find a vec that vertical this
 * @param {type}
 * @return {type}
 */
Lvector Lvector::orthogonalVec() const {
    // Find smallest component. Keep equal case for null values.
    if ((fabs(y) >= 0.9 * fabs(x)) && (fabs(z) >= 0.9 * fabs(x)))
        return Lvector(0.0, -z, y);
    else if ((fabs(x) >= 0.9 * fabs(y)) && (fabs(z) >= 0.9 * fabs(y)))
        return Lvector(-z, 0.0, x);
    else
        return Lvector(-y, x, 0.0);
}
