/*
 * @Author: your name
 * @Date: 2020-10-03 21:15:43
 * @LastEditTime: 2020-12-03 17:04:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/geometry.cc
 */
#include "geometry.hpp"
#include "debug.hpp"
using namespace BMesh;
/**
 * @description: The constructor
 * @param {none}
 * @return {none}
 */
Geometry::Geometry() {}

/**
 * @description: The destructor
 * @param {type}
 * @return {type}
 */
Geometry::~Geometry() {}

/**
 * @description: 静态函数 求线面的交点
 * @link:
 * https://baike.baidu.com/item/%E7%BA%BF%E9%9D%A2%E4%BA%A4%E7%82%B9/23119069?fr=aladdin
 * @param {type}
 * @return {bool} 判断是否相交
 */
bool Geometry::intersectionLine3DFace3D(const Line &l, const Face &f, Point3D &p) {
    if (std::fabs(l.l * f.n) < ESP) {
        return false; // 说明直线和面平行暂不考虑
    }
    Lvector local = Lvector(l.v[0], f.v[0]);
    double d = local * f.n / (l.l * f.n);
    p = l.l * d + l.v[0];
    // 判断交点p是否在面内
    if (!intersectionPointInAFace(f, p)) {
        return false;
    }
    // 判断交点p是否在线上
    if (!intersectionPointInALine(l, p)) {
        return false;
    }
    return true;
}

/**
 * @description: 静态函数 求射线 & 三角形面的交点
 * @link:
 * https://baike.baidu.com/item/%E7%BA%BF%E9%9D%A2%E4%BA%A4%E7%82%B9/23119069?fr=aladdin
 * @param {type}
 * @return {bool} 判断是否相交
 */
bool Geometry::intersectionRays3DTriangle(const Rays& l, const Triangle& t, Point3D& p) {
    if (std::fabs(l.l * t.n) < ESP) {
        return false; // 说明直线和面平行暂不考虑
    }
    Lvector local = Lvector(l.v[0], t.v[0]);
    double d = local * t.n / (l.l * t.n);
    p = l.l * d + l.v[0];
    // 判断交点p是否在面内
    if (!intersectionPointInATriangle(t, p)) {
        return false;
    }
    // 判断交点p是否在线上
    if (!intersectionPointInARays(l, p)) {
        return false;
    }
    return true;
}

bool Geometry::intersectionLine3DTriangle(const Line& l, const Triangle& t, Point3D& p) {
    if (std::fabs(l.l * t.n) < ESP) {
        return false; // 说明直线和面平行暂不考虑
    }
    Lvector local = Lvector(l.v[0], t.v[0]);
    double d = local * t.n / (l.l * t.n);
    p = l.l * d + l.v[0];
    // 判断交点p是否在面内
    if (!intersectionPointInATriangle(t, p)) {
        return false;
    }
    // 判断交点p是否在线上
    if (!intersectionPointInALineNotSeg(l, p)) {
        return false;
    }
    return true;
}

/**
 * @description: input face and the points of the line intersection of the face,then split the face
 * @param {type}
 * @return {type}
 * @TODO : 先使用一种方向进行切割
 * 0. 生成局部坐标
 * 1. 决定切割方向 使用任意两个点的向量在 a / b 方向的投影
 * 比较大小选择大的进行切分
 * 2. 然后进行局部坐标排序
 */

bool cmpX(Point2D &a, Point2D &b) { return a.x < b.x; }
bool cmpY(Point2D &a, Point2D &b) { return a.y < b.y; }
bool Geometry::splitFaceUseIntersectionPoints(const Face &f, const std::vector<Point3D> &p,
                                                     std::vector<std::pair<Point3D, Face>> &out) {
    out.clear();
    if (p.size() < 1) {
        return false;
    }
    if (p.size() == 1) {
        // Face test = f;
        // std::cout << "[DEBUG in]\n"
        //           << test.getPointsString();
        out.push_back(std::make_pair(p[0], f));
        return true;
    }
    // gen The basis vector,v[1] is the origin point.
    Lvector a(f.v[1], f.v[0]);
    a.normalize();
    Lvector b(f.v[1], f.v[2]);
    b.normalize();
    std::vector<Point2D> p2(p.size());
    std::vector<Lvector> l;
    for (int i = 0; i < p.size(); i++) {
        l.push_back(Lvector(f.v[1], p[i]));
    }
    double sumX = 0;
    double sumY = 0;
    for (int i = 0; i < p.size(); i++) {
        p2[i].id = i;
        p2[i].setX(std::fabs(aVecProjbVecLength(l[i], a)));
        p2[i].setY(std::fabs(aVecProjbVecLength(l[i], b)));
    }
    // Look to see if there are any similarities, and if there are, remove them first
    std::sort(p2.begin(), p2.end(), cmpX);
    std::vector<Point2D> unique_p2;
    unique_p2.push_back(p2[0]);
    for (int i = 1; i < p2.size(); i++) {
        if (p2[i - 1] == p2[i]) {
            continue;
        } else {
            unique_p2.push_back(p2[i]);
        }
    }
    if (unique_p2.size() == 1) {
        out.push_back(std::make_pair(p[0], f));
        return true;
    }
    std::sort(unique_p2.begin(), unique_p2.end(), cmpX);
    for (int i = 0; i < unique_p2.size() - 1; i++) {
        sumX += unique_p2[i + 1].x - unique_p2[i].x;
    }
    std::sort(unique_p2.begin(), unique_p2.end(), cmpY);
    for (int i = 0; i < unique_p2.size() - 1; i++) {
        sumY += unique_p2[i + 1].y - unique_p2[i].y;
    }
    // 进行`方向上`的去重如果有重复的话递归调用 此函数
    std::unordered_map<double, std::vector<Point3D>> mma; // 绑定点
    if (sumX >= sumY) {
        // I'm going to partition it with a parallel vectors
        std::sort(unique_p2.begin(), unique_p2.end(), cmpX);

        std::vector<Point2D> unique_p2_x;
        unique_p2_x.push_back(unique_p2[0]);
        for (int i = 1; i < unique_p2.size(); i++) {
            if (std::to_string(unique_p2[i - 1].x) == std::to_string(unique_p2[i].x)) {
                if (mma[unique_p2[i].x].size() > 0) {
                    mma[unique_p2[i].x].push_back(p[unique_p2[i].id]);
                } else {
                    mma[unique_p2[i].x].push_back(p[unique_p2[i - 1].id]);
                    mma[unique_p2[i].x].push_back(p[unique_p2[i].id]);
                }
            } else {
                unique_p2_x.push_back(unique_p2[i]);
            }
        }
        Point3D alpha;
        Point3D alphaa;
        Point3D beta;
        Point3D betaa;
        for (int i = 0; i < unique_p2_x.size() - 1; i++) {
            // You get the extension of the midpoint and the intersection of the two sides
            double len = (unique_p2_x[i].x + unique_p2_x[i + 1].x) / 2.0;
            alpha = a * len + f.v[1];
            beta = a * len + f.v[2];
            if (i == 0) {
                out.push_back(std::make_pair(p[unique_p2_x[i].id], Face(f.v[1], f.v[2], beta, alpha)));
                alphaa = alpha;
                betaa = beta;
            } else {
                out.push_back(std::make_pair(p[unique_p2_x[i].id], Face(alphaa, betaa, beta, alpha)));
                alphaa = alpha;
                betaa = beta;
            }
        }
        out.push_back(std::make_pair(p[unique_p2_x[unique_p2_x.size() - 1].id], Face(alphaa, betaa, f.v[3], f.v[0])));
    } else {
        // I'm going to partition it with b parallel vector
        std::sort(unique_p2.begin(), unique_p2.end(), cmpY);
        std::vector<Point2D> unique_p2_y;
        unique_p2_y.push_back(unique_p2[0]);
        for (int i = 1; i < unique_p2.size(); i++) {
            if (std::to_string(unique_p2[i - 1].y) == std::to_string(unique_p2[i].y)) {
                if (mma[unique_p2[i].y].size() > 0) {
                    mma[unique_p2[i].y].push_back(p[unique_p2[i].id]);
                } else {
                    mma[unique_p2[i].y].push_back(p[unique_p2[i - 1].id]);
                    mma[unique_p2[i].y].push_back(p[unique_p2[i].id]);
                }
            } else {
                unique_p2_y.push_back(unique_p2[i]);
            }
        }
        Point3D alpha;
        Point3D alphaa;
        Point3D beta;
        Point3D betaa;
        for (int i = 0; i < unique_p2_y.size() - 1; i++) {
            // You get the extension of the midpoint and the intersection of the two sides
            double len = (unique_p2_y[i].y + unique_p2_y[i + 1].y) / 2.0;
            alpha = b * len + f.v[0];
            beta = b * len + f.v[1];
            if (i == 0) {
                out.push_back(std::make_pair(p[unique_p2_y[i].id], Face(f.v[0], f.v[1], beta, alpha)));
                alphaa = alpha;
                betaa = beta;
            } else {
                out.push_back(std::make_pair(p[unique_p2_y[i].id], Face(alphaa, betaa, beta, alpha)));
                alphaa = alpha;
                betaa = beta;
            }
        }
        out.push_back(std::make_pair(p[unique_p2_y[unique_p2_y.size() - 1].id], Face(alphaa, betaa, f.v[2], f.v[3])));
    }
    // The recursive call gets the result
    if (mma.size() > 0) {
        std::vector<Point3D> recursionPoint3d;
        for (auto it = mma.begin(); it != mma.end(); it++) {
            recursionPoint3d.clear();
            for (int i = 0; i < it->second.size(); i++) {
                recursionPoint3d.push_back(it->second[i]);
            }
            Face t;
            bool check = false;
            for (auto iit = out.begin(); iit != out.end(); iit++) {
                if (iit->first == recursionPoint3d[0]) {
                    t = iit->second;
                    out.erase(iit);
                    check = true;
                    break;
                }
            }
            if (!check) {
                continue;
            }
            std::vector<std::pair<Point3D, Face>> out1;
            if (Geometry::splitFaceUseIntersectionPoints(t, recursionPoint3d, out1)) {
                for (int i = 0; i < out1.size(); i++) {
                    out.push_back(out1[i]);
                }
            } else {
                std::cout << "[ERROR] Something unexpected has come up" << std::endl;
            }
        }
    }
    return true;
}

/**
 * @description: The Angle between the two vectors
 * @param {type}
 * @return {type}
 */
double Geometry::angleTwoVector(Lvector &a, Lvector &b) {
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
 * @description: The length of a projection of one vector onto another
 * @param {type}
 * @return {type}
 */
double Geometry::aVecProjbVecLength(Lvector &a, Lvector &b) {
    double angle = angleTwoVector(a, b);
    double radian = angle / 180.0 * PI;
    double length = a.norm() * std::cos(radian);
    return length;
}

/**
 * @description: Determine if the intersection is on the line segment,simple
 * @param {type}
 * @return {type}
 */
bool Geometry::intersectionPointInALine(const Line &l, const Point3D &p) {
    double a = INT_MAX;
    double b = -a;
    Point3D minPoint(a, a, a);
    Point3D maxPoint(b, b, b);
    for (int i = 0; i < l.v.size(); i++) {
        if (l.v[i].x <= minPoint.x) {
            minPoint.x = l.v[i].x;
        }
        if (l.v[i].y <= minPoint.y) {
            minPoint.y = l.v[i].y;
        }
        if (l.v[i].z <= minPoint.z) {
            minPoint.z = l.v[i].z;
        }
        // --
        if (l.v[i].x >= maxPoint.x) {
            maxPoint.x = l.v[i].x;
        }
        if (l.v[i].y >= maxPoint.y) {
            maxPoint.y = l.v[i].y;
        }
        if (l.v[i].z >= maxPoint.z) {
            maxPoint.z = l.v[i].z;
        }
    }
    bool check = true;
    if ((p.x - ESP) > maxPoint.x || (p.x + ESP) < minPoint.x) {
        check = false;
    }
    if ((p.y - ESP) > maxPoint.y || (p.y + ESP) < minPoint.y) {
        check = false;
    }
    if ((p.z - ESP) > maxPoint.z || (p.z + ESP) < minPoint.z) {
        check = false;
    }
    return check;
}

bool Geometry::intersectionPointInALineNotSeg(const Line& l, const Point3D& p) {
    BMesh::Lvector t(p, l.l * 10 + p);
    t.normalize();
    BMesh::Lvector c(l.v[0], l.v[1]);
    c.normalize();
    if (fabs(fabs(t * c) - 1.0) < ESP) {
        return true;
    }
    return false;
}

/**
 * @description: Determine if the intersection point is on the surface
 * @param {type}
 * @return {type}
 */
//bool Geometry::intersectionPointInAFace(const Face &f, const Point3D &p) {
//    double a = INT_MAX;
//    double b = -a;
//    Point3D minPoint(a, a, a);
//    Point3D maxPoint(b, b, b);
//    for (int i = 0; i < f.v.size(); i++) {
//        if (f.v[i].x <= minPoint.x) {
//            minPoint.x = f.v[i].x;
//        }
//        if (f.v[i].y <= minPoint.y) {
//            minPoint.y = f.v[i].y;
//        }
//        if (f.v[i].z <= minPoint.z) {
//            minPoint.z = f.v[i].z;
//        }
//        // --
//        if (f.v[i].x >= maxPoint.x) {
//            maxPoint.x = f.v[i].x;
//        }
//        if (f.v[i].y >= maxPoint.y) {
//            maxPoint.y = f.v[i].y;
//        }
//        if (f.v[i].z >= maxPoint.z) {
//            maxPoint.z = f.v[i].z;
//        }
//    }
//    bool check = true;
//    if ((p.x - ESP) > maxPoint.x || (p.x + ESP) < minPoint.x) {
//        check = false;
//    }
//    if ((p.y - ESP) > maxPoint.y || (p.y + ESP) < minPoint.y) {
//        check = false;
//    }
//    if ((p.z - ESP) > maxPoint.z || (p.z + ESP) < minPoint.z) {
//        check = false;
//    }
//    return check;
//}

bool Geometry::intersectionPointInAFace(const Face& f, const Point3D& p) {
    double sumAngle = 0;
    bool check = false;
    for (int i = 0; i < f.v.size(); i++) {
        if (i == 0) {
            sumAngle += angleTwoVector(Lvector(p, f.v[0]), Lvector(p, f.v[3]));
        }
        else {
            sumAngle += angleTwoVector(Lvector(p, f.v[i]), Lvector(p, f.v[i-1]));
        }
    }
    double angleEPS = 2;
    if (fabs(sumAngle - 360) < angleEPS) {
        check = true;
    }
    return check;
}


/**
 * @description: Determine if the intersection is on the rays segment,simple
 * @param {type}
 * @return {type}
 */
bool Geometry::intersectionPointInARays(const Rays& l, const Point3D& p) {
    Rays t = l;
    return t.isPointInRays(p);
}
bool Geometry::intersectionPointInATriangle(const Triangle& t, const Point3D& p) {
    Triangle c = t;
    return c.isInTheFace(p);
}

double Geometry::angleFourPoint(Point3D &a, Point3D &b, Point3D &c, Point3D &d) {
    Lvector f(a, b);
    Lvector e(c, d);
    return std::fabs(Geometry::angleTwoVector(f, e));
}

/**
 * @description:
 * @param A: Input node; AChild: node to be deleted by the child of A; AddChild: The node to be added
 * @return {type}
 */
bool Geometry::changeLink(Node *a, Node *aChild, Node *addChild) {
    bool check = true;
    // The two nodes delete each other
    if (!a->deleteChild(aChild)) {
        std::cout << "[ERROR] delete node failed" << std::endl;
        check = false;
    }
    if (!aChild->deleteChild(a)) {
        std::cout << "[ERROR] delete node failed" << std::endl;
        check = false;
    }
    // Two nodes add an object
    if (!a->addChild(addChild)) {
        std::cout << "[ERROR] Increase node failed, already exists" << std::endl;
        check = false;
    }
    if (!aChild->addChild(addChild)) {
        std::cout << "[ERROR] Increase node failed, already exists" << std::endl;
        check = false;
    }
    // Object to add two nodes
    if (!addChild->addChild(a)) {
        std::cout << "[ERROR] Increase node failed, already exists" << std::endl;
        check = false;
    }
    if (!addChild->addChild(aChild)) {
        std::cout << "[ERROR] Increase node failed, already exists" << std::endl;
        check = false;
    }
    return check;
}

/**
 * @description: Whether the faces of hexahedron A contain faces of hexahedron B
 * 1. A's face contain B's face
 * 2. A's face === B's face
 * 3. A's face has no relationship with B's face
 * 4. B's face contain A's face
 * 5. ERROR Something strange has happened
 * 6. Face of A and face of B are interlaced
 * @param {type}
 * @return {type}
 */
unsigned int Geometry::isAHexFaceContainBHexFace(Hex *a, Hex *b, std::vector<Point3D> &diffPoint,
                                                        std::vector<Point3D> &samePoint, int &ai, int &bi) {
    std::vector<Point3D> localDiffPoint;
    std::vector<Point3D> localSamePoint;
    int lai;
    int lbi;
    int rlt = -1;
    struct local {
        std::vector<Point3D> localDiffPoint;
        std::vector<Point3D> localSamePoint;
        int lai;
        int lbi;
        int rlt;
    };
    std::vector<struct local> lv;
#ifdef __OPENMP__
    omp_set_num_threads(16);
	#pragma omp parallel for
#endif
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            localSamePoint.clear();
            localDiffPoint.clear();
            rlt = isAFaceContainBFace(a->f[i], b->f[j], localSamePoint);
            if (rlt == 1) {
                lai = i;
                lbi = j;
                DBG_D("a contain b face id %d %d\n", lai, lbi);
                localDiffPoint = b->f[j].getOtherPointFromAFace(localSamePoint);
                struct local t;
                t.rlt = 1;
                t.lai = lai;
                t.lbi = lbi;
                t.localSamePoint = localSamePoint;
                t.localDiffPoint = localDiffPoint;
                lv.push_back(t);
                // return rlt;
            } else if (rlt == 2) {
                lai = i;
                lbi = j;
                struct local t;
                t.rlt = 2;
                lv.push_back(t);
            } else if (rlt == 4) {
                lai = i;
                lbi = j;
                localDiffPoint = a->f[i].getOtherPointFromAFace(localSamePoint);
                DBG_D("b contain a face id %d %d\n", lbi, lai);
                struct local t;
                t.lai = lai;
                t.lbi = lbi;
                t.localSamePoint = localSamePoint;
                t.localDiffPoint = localDiffPoint;
                t.rlt = 4;
                lv.push_back(t);
            } else if (rlt == 6) {
                lai = i;
                lbi = j;
                localDiffPoint = a->f[i].getOtherPointFromAFace(localSamePoint);
                std::vector<Point3D> otherDiffPoints = b->f[j].getOtherPointFromAFace(localSamePoint);
                for (int t = 0; t < otherDiffPoints.size(); t++) {
                    localDiffPoint.push_back(otherDiffPoints[t]);
                }
                DBG_D("a interlace b face id %d %d\n", lai, lbi);
                struct local t;
                t.lai = lai;
                t.lbi = lbi;
                t.localSamePoint = localSamePoint;
                t.localDiffPoint = localDiffPoint;
                t.rlt = 6;
                lv.push_back(t);
            }
        }
    }
    for (int i = 0; i < lv.size(); i++) {
        if (lv[i].rlt == 2) {
            return lv[i].rlt;
        }
    }
    for (int i = 0; i < lv.size(); i++) {
        if (lv[i].rlt == 4) {
            ai = lv[i].lai;
            bi = lv[i].lbi;
            samePoint = lv[i].localSamePoint;
            diffPoint = lv[i].localDiffPoint;
            return lv[i].rlt;
        } else if (lv[i].rlt == 1) {
            ai = lv[i].lai;
            bi = lv[i].lbi;
            samePoint = lv[i].localSamePoint;
            diffPoint = lv[i].localDiffPoint;
            return lv[i].rlt;
        }
    }
    for (int i = 0; i < lv.size(); i++) {
        if (lv[i].rlt == 6) {
            ai = lv[i].lai;
            bi = lv[i].lbi;
            samePoint = lv[i].localSamePoint;
            diffPoint = lv[i].localDiffPoint;
            return lv[i].rlt;
        }
    }
    return rlt;
}

/**
 * @description: Whether Face A contains Face B
 * 1. A contain B
 * 2. A == B
 * 3. A has no relationship with b
 * 4. B contain A
 * 5. ERROR Something strange has happened
 * 6. A and B intersect and split A
 * @param {type}
 * @return {type} If a containment relationship outputs a line，假定现在都是三边相等。 一定是有两个点是相等的
 */
unsigned int Geometry::isAFaceContainBFace(Face &a, Face &b, std::vector<Point3D> &samePoint) {
    // It is not necessary to determine whether the normal vectors of a face are equal, because some faces are distorted
    if (!Geometry::vecIsSameLike(a.n, b.n)) {
        return 3;
    }
    // So let's see if A is equal to B
    samePoint.clear();
    if (a == b) {
        return 2;
    }
    int samePointNum = 0;
    std::vector<Point3D> diffPoint;
    for (int i = 0; i < a.v.size(); i++) {
        for (int j = 0; j < b.v.size(); j++) {
            if (a.v[i] == b.v[j]) {
                samePointNum++;
                samePoint.push_back(a.v[i]);
                break;
            }
        }
    }

    if (samePointNum == 0 || samePointNum == 3) {
        return 3;
    }
    // If a face is normally equal, we can determine whose area is larger
    if (samePointNum == 2) {
        double sumAreaA = a.getArea();
        double sumAreaB = b.getArea();
        if (sumAreaA >= sumAreaB && samePoint.size() == 2) {
            Point3D tmp = b.getMidFacePoint();
            if (a.isInTheFace(tmp)) {
                return 1;
            }
            // return 1;// tmp
            return 3;
        }
        if (sumAreaB >= sumAreaA && samePoint.size() == 2) {
            Point3D tmp = a.getMidFacePoint();
            if (b.isInTheFace(tmp)) {
                return 4;
            }
            // return 4;// tmp
            return 3;
        }
    } else if (samePointNum == 1) {
        // intersect judge
        if (isFacetoFace(a, b)) {
            // std::cout << "[DEBUG] === a ===\n";
            // for(int i=0; i<a.v.size(); i++){
            //     std::cout << a.v[i].getXYZ();
            // }
            // std::cout << "[DEBUG] === b ===\n";
            // for(int i=0; i<b.v.size(); i++){
            //     std::cout << b.v[i].getXYZ();
            // }
            return 6;
        }
        return 3;
    }
    for (int i = 0; i < a.v.size(); i++) {
        std::cout << a.v[i].getXYZ();
    }
    for (int i = 0; i < b.v.size(); i++) {
        std::cout << b.v[i].getXYZ();
    }
    std::cout << "[ERROR] An unexpected relationship has arisen" << std::endl;
    return 5;
}

/**
 * @description: Calculate an area based on three points
 * @param {type}
 * @return {type}
 */
double Geometry::calcAreaOfThreePoints(Point3D &a, Point3D &b, Point3D &c) {
    // Apply Helen's formula  S=1/4sqrt[(a+b+c)(a+b-c)(a+c-b)(b+c-a)]
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
 * @description: Splitting a hexahedron use a line
 * @param a Input-Output
 *        b Output
 *        diffpoint Two different points from A
 *        samePoint Same two points
 *        Which side of A
 * @return {type}
 */
bool Geometry::splitHexUseALine(Hex &a, Hex &b, std::vector<Point3D> &diffPoint, std::vector<Point3D> &samePoint,
                                       int ai) {
    int t = ai % 2;
    int aOppositeFaceId = -1;
    if (t == 0) {
        aOppositeFaceId = ai + 1;
    } else {
        aOppositeFaceId = ai - 1;
    }
    // get four points on the surface
    Face af = a.f[ai];
    Face aof = a.f[aOppositeFaceId];
    Face result[4]; // 1 2 / 3 4
    // produces two subfaces
    // Go through all four edges to see where the intersection is on that edge
    std::vector<Point3D> otherSamePoint;
    otherSamePoint = af.getOtherPointFromAFace(samePoint);
    result[0] = Face(Line(samePoint[0], samePoint[1]), Line(diffPoint[0], diffPoint[1]));
    result[1] = Face(Line(otherSamePoint[0], otherSamePoint[1]), Line(diffPoint[0], diffPoint[1]));
    Line sameLine = Line(samePoint[0], samePoint[1]);
    Line otherSameLine = Line(otherSamePoint[0], otherSamePoint[1]);
    std::vector<Line> aol1 = a.getOppositeLine(sameLine);
    Line aol11;
    Line aol21;
    for (int i = 0; i < aol1.size(); i++) {
        if (aol1[i] != otherSameLine) {
            aol11 = aol1[i];
            break;
        }
    }
    std::vector<Line> aol2 = a.getOppositeLine(otherSameLine);
    for (int i = 0; i < aol2.size(); i++) {
        if (aol2[i] != sameLine) {
            aol21 = aol2[i];
            break;
        }
    }
    // use samepoint to diffpoint vector to gen oppo's diffpoint
    double len1 = 100000;
    double lenf1 = 100000;
    for (int i = 0; i < diffPoint.size(); i++) {
        if (len1 > distanceTwoPoint(samePoint[0], diffPoint[i])) {
            len1 = distanceTwoPoint(samePoint[0], diffPoint[i]);
        }
    }
    for (int i = 0; i < otherSamePoint.size(); i++) {
        if (lenf1 > distanceTwoPoint(samePoint[0], otherSamePoint[i])) {
            lenf1 = distanceTwoPoint(samePoint[0], otherSamePoint[i]);
        }
    }
    double len2 = 100000;
    double lenf2 = 100000;
    for (int i = 0; i < diffPoint.size(); i++) {
        if (len2 > distanceTwoPoint(samePoint[1], diffPoint[i])) {
            len2 = distanceTwoPoint(samePoint[1], diffPoint[i]);
        }
    }
    for (int i = 0; i < otherSamePoint.size(); i++) {
        if (lenf2 > distanceTwoPoint(samePoint[1], otherSamePoint[i])) {
            lenf2 = distanceTwoPoint(samePoint[1], otherSamePoint[i]);
        }
    }
    double scale1 = len1 / lenf1;
    double scale2 = len2 / lenf2;
    std::vector<Point3D> oSamePoint;
    std::vector<Point3D> oOtherSamePoint;
    for (int i = 0; i < aol11.v.size(); i++) {
        oSamePoint.push_back(aol11.v[i]);
    }
    for (int i = 0; i < aol21.v.size(); i++) {
        oOtherSamePoint.push_back(aol21.v[i]);
    }
    if (distanceTwoPoint(samePoint[0], oSamePoint[0]) > distanceTwoPoint(samePoint[0], oSamePoint[1])) {
        std::swap(oSamePoint[0], oSamePoint[1]);
    }
    len1 = 100000;
    lenf1 = 100000;
    len2 = 100000;
    lenf2 = 100000;
    std::vector<Point3D> oDiffPoint;
    std::pair<Point3D, Point3D> oDFather1;
    std::pair<Point3D, Point3D> oDFather2;
    for (int i = 0; i < otherSamePoint.size(); i++) {
        if (lenf1 > distanceTwoPoint(oSamePoint[0], oOtherSamePoint[i])) {
            lenf1 = distanceTwoPoint(oSamePoint[0], oOtherSamePoint[i]);
            oDFather1 = std::make_pair(oSamePoint[0], oOtherSamePoint[i]);
        }
    }
    for (int i = 0; i < otherSamePoint.size(); i++) {
        if (lenf2 > distanceTwoPoint(oSamePoint[1], oOtherSamePoint[i])) {
            lenf2 = distanceTwoPoint(oSamePoint[1], oOtherSamePoint[i]);
            oDFather2 = std::make_pair(oSamePoint[1], oOtherSamePoint[i]);
        }
    }
    len1 = lenf1 * scale1;
    len2 = lenf2 * scale2;
    Lvector od1 = Lvector(oDFather1.first, oDFather1.second).normalize();
    Lvector od2 = Lvector(oDFather2.first, oDFather2.second).normalize();
    oDiffPoint.push_back(od1 * len1 + oSamePoint[0]);
    oDiffPoint.push_back(od2 * len2 + oSamePoint[1]);
    result[2] = Face(Line(oSamePoint[0], oSamePoint[1]), Line(oDiffPoint[0], oDiffPoint[1]));
    result[3] = Face(Line(oOtherSamePoint[0], oOtherSamePoint[1]), Line(oDiffPoint[0], oDiffPoint[1]));
    a = Hex(result[0], result[2]);
    b = Hex(result[1], result[3]);
    return true;
}

/**
 * @description: polymorphism interlace
 * @param {*}
 * @return {*}
 */
bool Geometry::splitHexUseALine(Hex &a, Hex &b, Hex& c, std::vector<Point3D> &diffPoint, std::vector<Point3D> &samePoint,
                                       int ai, int bi, int &rlt) {
    std::vector<Point3D> diffPointA;
    std::vector<Point3D> diffPointB;
    if (diffPoint.size() != 6) {
        std::cout << "[ERROOOOR] different points' number is not equal 6" << std::endl;
        return false;
    }
    if (samePoint.size() != 1) {
        std::cout << "[ERROOOOR] samepoints' number is not equal 1" << std::endl;
        return false;
    }
    for (int i = 0; i < 3; i++) {
        diffPointA.push_back(diffPoint[i]);
    }
    for (int i = 3; i < 6; i++) {
        diffPointB.push_back(diffPoint[i]);
    }
    std::vector<Lvector> al;
    DBG_D("al \n");

    for (int i = 0; i < diffPointA.size(); i++) {
        al.push_back(Lvector(samePoint[0], diffPointA[i]));
        Lvector s = Lvector(samePoint[0], diffPointA[i]);
        DBG_S(s.v[0].getXYZ());
        DBG_S(s.v[1].getXYZ());
    }
    std::vector<Lvector> bl;
    DBG_D("bl \n");
    for (int i = 0; i < diffPointB.size(); i++) {
        bl.push_back(Lvector(samePoint[0], diffPointB[i]));
        Lvector s = Lvector(samePoint[0], diffPointB[i]);
        DBG_S(s.v[0].getXYZ());
        DBG_S(s.v[1].getXYZ());
    }
    std::vector<std::pair<Lvector, Lvector>> abl;
    for (int i = 0; i < al.size(); i++) {
        for (int j = 0; j < bl.size(); j++) {
            if (al[i].getNormalize() == bl[j].getNormalize()) {
                abl.push_back(std::make_pair(al[i], bl[j]));
                break;
            }
        }
    }
    // interlace one big one small
    if (abl.size() == 3) {
        double angle1 = angleTwoVector(abl[0].first, abl[1].first);
        double angle2 = angleTwoVector(abl[0].first, abl[2].first);
        double angle3 = angleTwoVector(abl[1].first, abl[2].first);
        /*double area1 = (abl[0].first % abl[1].first).norm();
        double area2 = (abl[0].first % abl[2].first).norm();
        double area3 = (abl[1].first % abl[2].first).norm();*/
        if (angle1 > angle2 && angle1 > angle3) {
            abl.erase(abl.begin() + 2);
        }
        if (angle2 > angle1 && angle2 > angle3) {
            abl.erase(abl.begin() + 1);
        }
        if (angle3 > angle1 && angle3 > angle2) {
            abl.erase(abl.begin());
        }
    }
    //if (abl.size() == 3) {
    //    Point3D o = abl[0].first + abl[1].second.v[1];
    //    diffPoint.clear();
    //    diffPoint.push_back(abl[0].first.v[1]);
    //    diffPoint.push_back(o);
    //    samePoint.push_back(abl[1].second.v[1]);
    //    return splitHexUseALine(a, b, diffPoint, samePoint, bi);
    //}
    if (abl.size() != 2) {
        std::cout << "[ERROR] Intersecting edges are not two edges that have a common vector" << std::endl;
        return false;
    }
    bool check1 = abl[0].first.norm() > abl[0].second.norm();
    bool check2 = abl[1].first.norm() > abl[1].second.norm();
    if (check1 != check2) {
        if (!check1) {
            Point3D o = abl[0].first + abl[1].second.v[1];
            diffPoint.clear();
            diffPoint.push_back(abl[0].first.v[1]);
            diffPoint.push_back(o);
            samePoint.push_back(abl[1].second.v[1]);
            rlt = 0;
            return splitHexUseALine(a, b, diffPoint, samePoint, bi);
        } else {
            Point3D o = abl[1].first + abl[0].second.v[1];
            diffPoint.clear();
            diffPoint.push_back(abl[1].first.v[1]);
            diffPoint.push_back(o);
            samePoint.push_back(abl[0].second.v[1]);
            rlt = 0;
            return splitHexUseALine(a, b, diffPoint, samePoint, bi);
        }
    } else {
        if (rlt == 2) { // stop split
            return false;
        }
        if (check1) {
            Point3D o = abl[1].first + abl[0].second.v[1]; // 长向量 + 短向量的点
            diffPoint.clear();
            diffPoint.push_back(abl[0].second.v[1]); // 短向量的点
            diffPoint.push_back(o);
            samePoint.push_back(abl[1].first.v[1]); // 长向量的点
            rlt = 1;
            return splitHexUseALine(c, b, diffPoint, samePoint, ai);
        }
        else {
            Point3D o = abl[0].second + abl[1].first.v[1]; // 长向量 + 短向量的点
            diffPoint.clear();
            diffPoint.push_back(abl[1].first.v[1]);
            diffPoint.push_back(o);
            samePoint.push_back(abl[0].second.v[1]);
            rlt = 0;
            return splitHexUseALine(a, b, diffPoint, samePoint, bi);
        }
        
        // std::cout << "[ERROR] I can't handle this situation for the time being" << std::endl;
        // return false;
    }

    return true;
}

/**
 * @description: Calculate the distance between two points
 * @param {type}
 * @return {type}
 */
double Geometry::distanceTwoPoint(Point3D &a, Point3D &b) {
    Lvector t = Lvector(a, b);
    double rlt = t.norm();
    return rlt;
}

/**
 * @description: normal is near?
 * @param {type}
 * @return {type}
 */
bool Geometry::vecIsSameLike(Lvector &a, Lvector &b) {
    a.normalize();
    b.normalize();
    Lvector c;
    if (a * b >= 0) {
        c = a - b;
    } else {
        c = a + b;
    }
    const double ESP = 0.001;
    if (std::fabs(c.x) < ESP && std::fabs(c.y) < ESP && std::fabs(c.z) < ESP) {
        return true;
    }
    return false;
}

/**
 * @description: for same point have two normal vector are same
 * @param {*}
 * @return {*}
 */
bool Geometry::isFacetoFace(Face &a, Face &b) {
    std::vector<Point3D> tmp;
    int ai = 0;
    int bj = 0;
    for (int i = 0; i < a.v.size(); i++) {
        for (int j = 0; j < b.v.size(); j++) {
            if (a.v[i] == b.v[j]) {
                ai = i;
                bj = j;
                tmp.push_back(a.v[i]);
                break;
            }
        }
    }
    if (tmp.size() != 1) {
        return false;
    }
    // vectors compare
    std::vector<Lvector> al;
    std::vector<Lvector> bl;
    if (ai == 0) {
        al.push_back(Lvector(a.v[ai], a.v[ai + 1]));
        al.push_back(Lvector(a.v[ai], a.v[3]));
    } else if (ai == 3) {
        al.push_back(Lvector(a.v[ai], a.v[0]));
        al.push_back(Lvector(a.v[ai], a.v[ai - 1]));
    } else {
        al.push_back(Lvector(a.v[ai], a.v[ai + 1]));
        al.push_back(Lvector(a.v[ai], a.v[ai - 1]));
    }

    if (bj == 0) {
        bl.push_back(Lvector(b.v[bj], b.v[bj + 1]));
        bl.push_back(Lvector(b.v[bj], b.v[3]));
    } else if (bj == 3) {
        bl.push_back(Lvector(b.v[bj], b.v[0]));
        bl.push_back(Lvector(b.v[bj], b.v[bj - 1]));
    } else {
        bl.push_back(Lvector(b.v[bj], b.v[bj + 1]));
        bl.push_back(Lvector(b.v[bj], b.v[bj - 1]));
    }
    int sameNormalVectorNum = 0;
    for (int i = 0; i < al.size(); i++) {
        for (int j = 0; j < bl.size(); j++) {
            if (al[i].normalize() == bl[j].normalize()) {
                sameNormalVectorNum++;
                break;
            }
        }
    }
    if (sameNormalVectorNum == 2) {
        return true;
    }
    return false;
}