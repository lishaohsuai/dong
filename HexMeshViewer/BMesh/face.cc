/*
 * @Author: your name
 * @Date: 2020-10-05 21:33:43
 * @LastEditTime: 2020-11-26 21:20:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/face.cc
 */
#include "face.hpp"
using namespace BMesh;
/**
 * @description:
 * @param {type}
 * @return {type}
 */
Face::Face(Point3D a, Point3D b, Point3D c, Point3D d) {
    double aa = angleTwoVector(Lvector(a, b), Lvector(a, d));
    double ab = angleTwoVector(Lvector(a, b), Lvector(a, c));
    double ac = angleTwoVector(Lvector(a, c), Lvector(a, d));
    if (aa > ab && aa > ac) {
        Lvector u(a, b);
        Lvector vv(a, d);
        n = (u % vv).normalize();
        v.push_back(b);
        v.push_back(a);
        v.push_back(d);
        v.push_back(c);
    }
    else if (ab > aa && ab > ac) {
        Lvector u(a, b);
        Lvector vv(a, c);
        n = (u % vv).normalize();
        v.push_back(b);
        v.push_back(a);
        v.push_back(c);
        v.push_back(d);
    }
    else if (ac > aa && ac > ab) {
        Lvector u(a, c);
        Lvector vv(a, d);
        n = (u % vv).normalize();
        v.push_back(c);
        v.push_back(a);
        v.push_back(d);
        v.push_back(b);
    }
    else {
        v.push_back(a);
        v.push_back(b);
        v.push_back(c);
        v.push_back(d);
        std::cout << "[ERROR] face use point gen error\n";
        std::cout << a.getXYZ() << b.getXYZ() << c.getXYZ() <<d.getXYZ();
    }
    id = -1;
}

Face::Face(Line a, Line b) {
    // It depends on the Angle between the vectors
    Lvector u(a.v[0], a.v[1]);
    Lvector vv1(a.v[0], b.v[0]);
    Lvector vv2(a.v[0], b.v[1]);
    double angle1 = std::fabs(angleTwoVector(u, vv1));
    double angle2 = std::fabs(angleTwoVector(u, vv2));
    if (angle1 >= angle2) {
        Face tmp = Face(a.v[0], a.v[1], b.v[1], b.v[0]);
        n = tmp.n;
        v = tmp.v;
        id = tmp.id;
    } else {
        Face tmp = Face(a.v[0], a.v[1], b.v[0], b.v[1]);
        n = tmp.n;
        v = tmp.v;
        id = tmp.id;
    }
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Face::~Face() {}

/**
 * @description: Set face name
 * @param {type}
 * @return {type}
 */
void Face::setName(std::string name) { name = name; }

/**
 * @description: The constructor
 * @param {type}
 * @return {type}
 */
Face::Face() {
    n = Lvector(0, 0, 0);
    id = -1;
}

/**
 * @description: Copy constructor
 * @param {type}
 * @return {type}
 */
Face::Face(Face &rhs) {
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
    n = rhs.n;
    id = rhs.id;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Face &Face::operator=(const Face &rhs) {
    this->n = rhs.n;
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
    this->id = rhs.id;
    return *this;
}
Face &Face::operator=(Face &rhs) {
    this->n = rhs.n;
    this->id = rhs.id;
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
    return *this;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Face::Face(const Face &rhs) {
    v.clear();
    for (int i = 0; i < rhs.v.size(); i++) {
        v.push_back(rhs.v[i]);
    }
    n = rhs.n;
    this->id = rhs.id;
}

/**
 * @description: Returns the point description of the surface
 * @param {type}
 * @return {type}
 */
std::string Face::getPointsString() {
    std::string str;
    for (int i = 0; i < v.size(); i++) {
        str += v[i].getXYZ();
    }
    return str;
}

/**
 * @description: You get the mean points of the faces
 * @param {type}
 * @return {type}
 */
Point3D Face::getMidFacePoint() {
    Point3D midPoint(0, 0, 0);
    int number = 0;
    for (int i = 0; i < v.size(); i++) {
        midPoint += v[i];
        number++;
    }
    midPoint /= (double)number;
    return midPoint;
}

/**
 * @description: Determine if the faces are equal
 * @param {type}
 * @return {type}
 */
bool Face::operator==(Face &rhs) {
    for (int i = 0; i < this->v.size(); i++) {
        if (std::find(rhs.v.begin(), rhs.v.end(), this->v[i]) == rhs.v.end()) {
            return false;
        }
    }
    return true;
}

/**
 * @description: You get the area of a surface
 *               Take the area of two triangles for a quadrilateral
 *               For the time being only tetrahedral
 * @param {type}
 * @return {type}
 */
double Face::getArea() {
    if (v.size() != 4) {
        std::cout << "[ERROR] face's v number is not 4\n";
        return 0;
    }
    double sumArea = 0;
    sumArea += calcAreaOfThreePoints(v[0], v[1], v[2]);
    sumArea += calcAreaOfThreePoints(v[0], v[3], v[2]);
    return sumArea;
}

/**
 * @description: 猜测可能是因为相互包含导致的问题
 * @param {type}
 * @return {type}
 */
double Face::calcAreaOfThreePoints(Point3D &a, Point3D &b, Point3D &c) {
    //应用海伦公式   S=1/4sqrt[(a+b+c)(a+b-c)(a+c-b)(b+c-a)]
    double lenA =
        std::sqrt(std::pow(b.x - c.x, 2) + std::pow(b.y - c.y, 2) + std::pow(b.z - c.z, 2)); // b - c 两点的坐标
    double lenB =
        std::sqrt(std::pow(a.x - c.x, 2) + std::pow(a.y - c.y, 2) + std::pow(a.z - c.z, 2)); // a - c 两点的坐标
    double lenC =
        std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2) + std::pow(b.z - a.z, 2)); // a - b 两点的坐标
    double Area = 1.0 / 4.0 *
                  std::sqrt((lenA + lenB + lenC) * (lenA + lenB - lenC) * (lenA + lenC - lenB) * (lenB + lenC - lenA));
    return Area;
}

/**
 * @description: I'm going to give you a bunch of points, and then I'm going to get points that aren't on the surface
 * @param {type}
 * @return {type}
 */
std::vector<Point3D> Face::getOtherPointFromAFace(std::vector<Point3D> &rhs) {
    std::unordered_map<std::string, bool> um;
    for (int i = 0; i < rhs.size(); i++) {
        um[rhs[i].getXYZ()] = true;
    }
    std::vector<Point3D> rlt;
    for (int i = 0; i < v.size(); i++) {
        if (um[v[i].getXYZ()] == false) {
            rlt.push_back(v[i]);
        }
    }
    return rlt;
}

/**
 * @description: Calculate the Angle between the two vectors
 * @param {type}
 * @return {type}
 */
double Face::angleTwoVector(Lvector &a, Lvector &b) {
    double cosTheta = (a * b) / (a.norm() * b.norm());
    if (cosTheta > 1) {
        cosTheta = 1;
    }
    if (cosTheta < -1) {
        cosTheta = -1;
    }
    double theta = std::acos(cosTheta);
    theta = theta / PI * 180.0;
    return theta;
}

/**
 * @description: Determine if the point is within the range of the surface
 * @param {type}
 * @return {type}
 */
bool Face::isInTheFace(Point3D &a) {
    genFrame();
    if (a.x - ESP > bigPoint.x || a.y - ESP > bigPoint.y || a.z - ESP > bigPoint.z) {
        return false;
    }
    if (a.x + ESP < smallPoint.x || a.y + ESP < smallPoint.y || a.z + ESP < smallPoint.z) {
        return false;
    }
    // The sum of the angles around point A is 360 degrees
    Lvector t1(a, v[0]);
    Lvector t2(a, v[1]);
    Lvector t3(a, v[2]);
    Lvector t4(a, v[3]);
    double angle1 = std::fabs(angleTwoVector(t1, t2)) + std::fabs(angleTwoVector(t2, t3)) +
                    std::fabs(angleTwoVector(t3, t4)) + std::fabs(angleTwoVector(t4, t1));
    if (std::fabs(angle1 - 360) > 5) {
        return false;
    }
    return true;
}

void Face::genFrame() {
    smallPoint = Point3D(MAXDATA, MAXDATA, MAXDATA);
    bigPoint = Point3D(MINDATA, MINDATA, MINDATA);
    for (int i = 0; i < v.size(); i++) {
        if (v[i].x < smallPoint.x) {
            smallPoint.x = v[i].x;
        }
        if (v[i].y < smallPoint.y) {
            smallPoint.y = v[i].y;
        }
        if (v[i].z < smallPoint.z) {
            smallPoint.z = v[i].z;
        }
        // ---
        if (v[i].x > bigPoint.x) {
            bigPoint.x = v[i].x;
        }
        if (v[i].y > bigPoint.y) {
            bigPoint.y = v[i].y;
        }
        if (v[i].z > bigPoint.z) {
            bigPoint.z = v[i].z;
        }
    }
}