/*
 * @Author: your name
 * @Date: 2020-10-05 09:27:10
 * @LastEditTime: 2021-03-18 09:19:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/common.hpp
 */
#ifndef __COMMON_HPP__
#define __COMMON_HPP__
#include "limits.h"
#include "local_vector.hpp"
#include "point.hpp"
#include <iostream>
#include <string>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>
#include <qmath.h>
#include "BMesh/frame.hpp"
using namespace glm;

//# define M_PI		3.14159265358979323846	/* pi */
#define DEGTORAD M_PI/double(180.0)
#define GLM_FORCE_RADIANS
namespace BMesh {
namespace common {
const double PI = 3.1415926;
double distance(Point3D &a, Point3D &b);
void matVec3(const double m[9], Point3D x, Point3D &y);
void rotationThreeAngle(double angle_x, double angle_y, double angle_z, const Point3D &in, Point3D &out);
bool getStrLine(std::istream &ifs, std::string &str);
void trimString(std::string &str);
Frame optimization(std::vector<Lvector> &vec, std::vector<double> &alphaBetaGamma, std::vector<double> &lwh);
quat inAngleAxis(vec3 RotationAxis, double RotationAngle);
Point3D rotateByQuat(const quat& q, const Point3D& in);
Point3D rotateByQuatToCenter(const quat& q, const Point3D& in, const Point3D& center);
} // namespace common
} // namespace BMesh
#endif