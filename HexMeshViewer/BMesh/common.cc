/*
 * @Author: your name
 * @Date: 2020-10-05 09:30:31
 * @LastEditTime: 2021-03-18 09:20:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/common.cc
 */
#include "common.hpp"
using namespace BMesh;
/**
 * @description: calc the distance from a point to b point
 * @param {type}
 * @return {type}
 */
double common::distance(Point3D &a, Point3D &b) {
    Point3D c = a - b;
    double dis = std::sqrt(c.x * c.x + c.y * c.y + c.z * c.z);
    return dis;
}

/**
 * @description: 3 * 3 * 3 * 1 matrix
 * @param {type}
 * @return {type}
 */
void common::matVec3(const double m[9], Point3D x, Point3D &y) {
    y.x = m[0] * x.x + m[1] * x.y + m[2] * x.z;
    y.y = m[3] * x.x + m[4] * x.y + m[5] * x.z;
    y.z = m[6] * x.x + m[7] * x.y + m[8] * x.z;
}

/**
 * @description:  rotate by a certain Angle in the in point out the out point
 *                Rotate counterclockwise about the X-axis
 *                Rotate counterclockwise about the Y-axis
 *                Rotate counterclockwise about the Z-axis
 * @param {type}
 * @return {type}
 */
void common::rotationThreeAngle(double angle_x, double angle_y, double angle_z, const Point3D &in,
                                       Point3D &out) {
    angle_x *= PI / 180.0;
    angle_y *= PI / 180.0;
    angle_z *= PI / 180.0;
    const double Rx[9] = {1, 0, 0, 0, cos(angle_x), -sin(angle_x), 0, sin(angle_x), cos(angle_x)};
    const double Ry[9] = {cos(angle_y), 0, sin(angle_y), 0, 1, 0, -sin(angle_y), 0, cos(angle_y)};
    const double Rz[9] = {cos(angle_z), -sin(angle_z), 0, sin(angle_z), cos(angle_z), 0, 0, 0, 1};
    Point3D res;
    matVec3(Rx, in, out);
    res = out;
    matVec3(Ry, res, out);
    res = out;
    matVec3(Rz, res, out);
    return;
}

/**
 * @description: Read a line of the file
 * @param {type}
 * @return {type}
 */
bool common::getStrLine(std::istream &ifs, std::string &str) {
    while (true) {
        getline(ifs, str);
        trimString(str);
        if (str.size() != 0) {
            if (str[0] != '#') {
                return true;
            }
        }
        if (ifs.eof() || str.compare("End")) {
            std::cout << "[DEBUG] End of file reached" << std::endl;
            return false;
        }
    }
    return false;
}

/**
 * @description: Delete the Spaces before and after
 * @param {type}
 * @return {type}
 */
void common::trimString(std::string &str) {
    size_t start = str.find_first_not_of(" \t\r\n"); // \t represents four empty characters, also known as indentation,
                                                     // equivalent to pressing the Tab key
    size_t end = str.find_last_not_of(" \t\r\n");

    if (std::string::npos == start || std::string::npos == end) {
        str = "";
    } else {
        str = str.substr(start, end - start + 1);
    }
}

/**
 * @description: Optimize the Angle of the rectangle to store TODO
 * @param {type}
 * @return {type}
 */
Frame common::optimization(std::vector<Lvector> &vec, std::vector<double> &alphaBetaGamma, std::vector<double> &lwh) {
    alphaBetaGamma.resize(3);
    double minF = INT_MAX;
    double piceOfAngle = 90.0 / 20.0;
    double uvw[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    Frame result;
    for (int i = 0; i < 41; i++) {
        for (int j = 0; j < 41; j++) {
            for (int k = 0; k < 41; k++) {
                double curF = 0;
                Point3D x = Point3D(1, 0, 0);
                Point3D xx;
                rotationThreeAngle(i * piceOfAngle, j * piceOfAngle, k * piceOfAngle, x, xx);
                Point3D y = Point3D(0, 1, 0);
                Point3D yy;
                rotationThreeAngle(i * piceOfAngle, j * piceOfAngle, k * piceOfAngle, y, yy);
                Point3D z = Point3D(0, 0, 1);
                Point3D zz;
                rotationThreeAngle(i * piceOfAngle, j * piceOfAngle, k * piceOfAngle, z, zz);
                Lvector u(Point3D(0, 0, 0), xx);
                u = u * lwh[0];
                Lvector v(Point3D(0, 0, 0), yy);
                v = v * lwh[1];
                Lvector w(Point3D(0, 0, 0), zz);
                w = w * lwh[2];
                for (int l = 0; l < vec.size(); l++) {
                    curF += fabs(u * vec[l]) + fabs(v * vec[l]) + fabs(w * vec[l]);
                }
                if (minF > curF) {
                    minF = curF;
                    alphaBetaGamma[0] = i * piceOfAngle;
                    alphaBetaGamma[1] = j * piceOfAngle;
                    alphaBetaGamma[2] = k * piceOfAngle;

                    result.t = Lvector(Point3D(0, 0, 0), zz);
                    result.r = Lvector(Point3D(0, 0, 0), xx);
                    result.s = Lvector(Point3D(0, 0, 0), yy);
                }
            }
        }
    }
    return result;
}


quat common::inAngleAxis(vec3 RotationAxis, double RotationAngle) {
    RotationAngle = RotationAngle * DEGTORAD;
    RotationAxis = normalize(RotationAxis);
    quat t;
    t.x = RotationAxis.x * sin(RotationAngle / 2);
    t.y = RotationAxis.y * sin(RotationAngle / 2);
    t.z = RotationAxis.z * sin(RotationAngle / 2);
    t.w = cos(RotationAngle / 2);
    return t;
}

Point3D common::rotateByQuat(const quat& q, const Point3D& in) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::mat4_cast(q) * model; // 旋转模型矩阵
    vec4 p00(in.x, in.y, in.z, 0);
    vec4 out = model * p00;
    Point3D ou;
    ou.x = out.x;
    ou.y = out.y;
    ou.z = out.z;
    return ou;
}


Point3D common::rotateByQuatToCenter(const quat& q, const Point3D& in, const Point3D& center) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::mat4_cast(q) * model; // 旋转模型矩阵
    vec4 p00(in.x - center.x, in.y - center.y, in.z - center.z, 0);
    vec4 out = model * p00;
    Point3D ou;
    ou.x = out.x + center.x;
    ou.y = out.y + center.y;
    ou.z = out.z + center.z;
    return ou;
}