/*
 * @Author: your name
 * @Date: 2020-10-05 09:44:23
 * @LastEditTime: 2021-03-18 10:21:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/hex.cc
 */
#include "hex.hpp"
using namespace BMesh;
/**
 * @description: length width height
 * @param {type}
 * @return {type}
 */
Hex::Hex(Point3D _p, const std::vector<double> &lwh){
    position = _p;
    count++;
    if(lwh.size() != 3) {
        std::cout << "[ERROR] " << "lwh's length is not equal 3\n";
    }
    length = lwh[0];
    width = lwh[1];
    height = lwh[2];
    id = count;
    angleX = angleY = angleZ = 0;
    genEightPoint();
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
}

void Hex::useFrameToChangePoint(Frame& f) {
    if (!(width == height && height == length))
        return;
    double halfWidth = this->width / 2.0;
    double halfLength = this->length / 2.0;
    double halfHeight = this->height / 2.0;
    
    p[0] = (f.s * (-halfWidth) + f.r * (-halfWidth) + f.t *(halfWidth)) + this->position;
    p[1] = (f.s * (halfWidth) + f.r * (-halfWidth) + f.t * (halfWidth)) + this->position;
    p[2] = (f.s * (halfWidth)+ f.r * (halfWidth)+f.t * (halfWidth)) + this->position;
    p[3] = (f.s * (-halfWidth)+f.r * (halfWidth)+f.t * (halfWidth)) + this->position;
    p[4] = (f.s * (-halfWidth) + f.r * (-halfWidth)+f.t * (-halfWidth)) + this->position;
    p[5] = (f.s * (halfWidth) + f.r * (-halfWidth) + f.t * (-halfWidth)) + this->position;
    p[6] = (f.s * (halfWidth)+f.r * (halfWidth) + f.t * (-halfWidth)) + this->position;
    p[7] = (f.s * (-halfWidth)+f.r * (+halfWidth)+f.t * (-halfWidth)) + this->position;
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
}

Hex::Hex(std::vector<Point3D> v) {
    if (v.size() != 8) {
        std::cout << "[ERROR] 8points are not Satisfy" << std::endl;
        return;
    }
    for (int i = 0; i < 8; i++) {
        p[i] = v[i];
    }
    position = centerPoint();
    length = Geometry::distanceTwoPoint(p[5], p[6]);
    width = Geometry::distanceTwoPoint(p[4], p[5]);
    height = Geometry::distanceTwoPoint(p[1], p[5]);
    angleX = angleY = angleZ = 0;
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
}

Hex::Hex(Face &_a, Face &_b) {
    Point3D midPoint(0, 0, 0);
    int number = 0;
    for (int i = 0; i < _a.v.size(); i++) {
        midPoint += _a.v[i];
        number++;
    }
    for (int i = 0; i < _b.v.size(); i++) {
        midPoint += _b.v[i];
        number++;
    }
    midPoint /= (number * 1.0);
    position = midPoint;
    genEightPointUseFace(_a, _b);
    length = Geometry::distanceTwoPoint(p[5], p[6]);
    width = Geometry::distanceTwoPoint(p[4], p[5]);
    height = Geometry::distanceTwoPoint(p[1], p[5]);
    angleX = angleY = angleZ = 0;
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
}
/**
 * @description: The destructor
 * @param {type}
 * @return {type}
 */
Hex::~Hex() {}


double Hex::getHeight() {
    return height;
}
double Hex::getWidth() {
    return width;
}
double Hex::getLength() {
    return length;
}


/**
 * @description: Compute the function of the center point
 * @param {type}
 * @return {type}
 */
Point3D Hex::centerPoint() {
    Point3D sumPoint(0, 0, 0);
    for (int i = 0; i < 8; i++) {
        sumPoint += p[i];
    }
    Point3D rlt = sumPoint / 8.0;
    return rlt;
}

double Hex::getRotateX(){
    return angleX;
}
double Hex::getRotateY(){
    return angleY;
}
double Hex::getRotateZ(){
    return angleZ;
}

/**
 * @description: Eight points are generated from two faces TODO
 * @param {type}
 * @return {type}
 */
void Hex::genEightPointUseFace(Face &a, Face &b) {
    Point3D aMid = a.getMidFacePoint();
    Point3D bMid = b.getMidFacePoint();
    double tmp[8];
    tmp[0] =
        Geometry::angleFourPoint(a.v[0], b.v[0], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[1], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[2], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[3], aMid, bMid);
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[0])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[1]))) {
        tmp[0] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[0])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[3]))) {
        tmp[0] += 200; // Penalty coefficient
    }

    tmp[1] =
        Geometry::angleFourPoint(a.v[0], b.v[1], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[0], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[3], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[2], aMid, bMid);

    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[1])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[0]))) {
        tmp[1] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[1])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[2]))) {
        tmp[1] += 200; // Penalty coefficient
    }

    tmp[2] =
        Geometry::angleFourPoint(a.v[0], b.v[1], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[2], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[3], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[0], aMid, bMid);

    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[1])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[2]))) {
        tmp[2] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[1])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[0]))) {
        tmp[2] += 200; // Penalty coefficient
    }

    tmp[3] =
        Geometry::angleFourPoint(a.v[0], b.v[2], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[1], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[0], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[3], aMid, bMid);

    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[2])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[1]))) {
        tmp[3] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[2])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[3]))) {
        tmp[3] += 200; // Penalty coefficient
    }

    tmp[4] =
        Geometry::angleFourPoint(a.v[0], b.v[2], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[3], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[0], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[1], aMid, bMid);

    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[2])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[3]))) {
        tmp[4] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[2])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[1]))) {
        tmp[4] += 200; // Penalty coefficient
    }

    tmp[5] =
        Geometry::angleFourPoint(a.v[0], b.v[3], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[2], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[1], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[0], aMid, bMid);

    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[3])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[2]))) {
        tmp[5] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[3])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[0]))) {
        tmp[5] += 200; // Penalty coefficient
    }

    tmp[6] =
        Geometry::angleFourPoint(a.v[0], b.v[3], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[0], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[1], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[2], aMid, bMid);

    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[3])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[0]))) {
        tmp[6] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[3])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[2]))) {
        tmp[6] += 200; // Penalty coefficient
    }

    tmp[7] =
        Geometry::angleFourPoint(a.v[0], b.v[0], aMid, bMid) + Geometry::angleFourPoint(a.v[1], b.v[3], aMid, bMid) +
        Geometry::angleFourPoint(a.v[2], b.v[2], aMid, bMid) + Geometry::angleFourPoint(a.v[3], b.v[1], aMid, bMid);

    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[0])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[1]), Lvector(a.v[0], b.v[3]))) {
        tmp[7] += 200; // Penalty coefficient
    }
    if (Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[0])) < Geometry::angleTwoVector(Lvector(a.v[0], a.v[3]), Lvector(a.v[0], b.v[1]))) {
        tmp[7] += 200; // Penalty coefficient
    }

    int minIndx = INT_MAX;
    int minValue = INT_MAX;
    p[0] = a.v[0];
    p[1] = a.v[1];
    p[2] = a.v[2];
    p[3] = a.v[3];
    for (int i = 0; i < 8; i++) {
        if (minValue > tmp[i]) {
            minValue = tmp[i];
            minIndx = i;
        }
    }
    switch (minIndx) {
    case 0:
        p[4] = b.v[0];
        p[5] = b.v[1];
        p[6] = b.v[2];
        p[7] = b.v[3];
        break;
    case 1:
        p[4] = b.v[1];
        p[5] = b.v[0];
        p[6] = b.v[3];
        p[7] = b.v[2];
        break;
    case 2:
        p[4] = b.v[1];
        p[5] = b.v[2];
        p[6] = b.v[3];
        p[7] = b.v[0];
        break;
    case 3:
        p[4] = b.v[2];
        p[5] = b.v[1];
        p[6] = b.v[0];
        p[7] = b.v[3];
        break;
    case 4:
        p[4] = b.v[2];
        p[5] = b.v[3];
        p[6] = b.v[0];
        p[7] = b.v[1];
        break;
    case 5:
        p[4] = b.v[3];
        p[5] = b.v[2];
        p[6] = b.v[1];
        p[7] = b.v[0];
        break;
    case 6:
        p[4] = b.v[3];
        p[5] = b.v[0];
        p[6] = b.v[1];
        p[7] = b.v[2];
        break;
    case 7:
        p[4] = b.v[0];
        p[5] = b.v[3];
        p[6] = b.v[2];
        p[7] = b.v[1];
        break;
    default:
        break;
    }
}

/**
 * @description: Eight points are generated according to the radius and coordinates
 * @param {}
 * @return {type}
 */
void Hex::genEightPoint() {
    double halfWidth = this->width / 2.0;
    double halfLength = this->length / 2.0;
    double halfHeight = this->height / 2.0;
    p[0] = Point3D(this->position.x - halfWidth, this->position.y - halfLength, this->position.z + halfHeight);
    p[1] = Point3D(this->position.x + halfWidth, this->position.y - halfLength, this->position.z + halfHeight);
    p[2] = Point3D(this->position.x + halfWidth, this->position.y + halfLength, this->position.z + halfHeight);
    p[3] = Point3D(this->position.x - halfWidth, this->position.y + halfLength, this->position.z + halfHeight);
    p[4] = Point3D(this->position.x - halfWidth, this->position.y - halfLength, this->position.z - halfHeight);
    p[5] = Point3D(this->position.x + halfWidth, this->position.y - halfLength, this->position.z - halfHeight);
    p[6] = Point3D(this->position.x + halfWidth, this->position.y + halfLength, this->position.z - halfHeight);
    p[7] = Point3D(this->position.x - halfWidth, this->position.y + halfLength, this->position.z - halfHeight);
    //change 序列
}
/**
 * @description: Static variable initialization
 * @param {type}
 * @return {type}
 */
int Hex::count = 0;

/**
 * @description: Rotate to the center point
 * @param {type}
 * @return {type}
 */
void Hex::rotate(std::vector<double> &alphaBetaGamma) {
    // before rotate should reset position
    // for (int i = 0; i < 8; i++) {
    //    Point3D tmp = this->p[i] - this->position;
    //    Point3D out;
    //    common::rotationThreeAngle(angleX * -1, angleY * -1, angleZ * -1, tmp, out);
    //    this->p[i] = out + this->position;
    // }
    
    for (int i = 0; i < 8; i++) {
        Point3D tmp = this->p[i] - this->position;
        Point3D out;
        common::rotationThreeAngle(alphaBetaGamma[0], alphaBetaGamma[1], alphaBetaGamma[2], tmp, out);
        this->p[i] = out + this->position;
    }
    angleX += alphaBetaGamma[0];
    angleY += alphaBetaGamma[1];
    angleZ += alphaBetaGamma[2];
    angleX = setAngle(angleX);
    angleY = setAngle(angleY);
    angleZ = setAngle(angleZ);
    genSixFace();    // 更新面
    genTwelveLine(); // 更新边
}


void Hex::rotate_one(std::vector<double> &alphaBetaGamma) {
    // before rotate should reset position
    // for (int i = 0; i < 8; i++) {
    //    Point3D tmp = this->p[i] - this->position;
    //    Point3D out;
    //    common::rotationThreeAngle(angleX * -1, angleY * -1, angleZ * -1, tmp, out);
    //    this->p[i] = out + this->position;
    // }
    
    for (int i = 0; i < 8; i++) {
        Point3D tmp = this->p[i] - this->position;
        Point3D out;
        common::rotationThreeAngle(alphaBetaGamma[0], alphaBetaGamma[1], alphaBetaGamma[2], tmp, out);
        this->p[i] = out + this->position;
    }
    Point3D tt(0, 0, 0);
    for (int i=0; i<8; i++){
        tt += this->p[i];
    }
    tt /= 8;
    this->position = tt;
    // angleX += alphaBetaGamma[0];
    // angleY += alphaBetaGamma[1];
    // angleZ += alphaBetaGamma[2];
    // angleX = setAngle(angleX);
    // angleY = setAngle(angleY);
    // angleZ = setAngle(angleZ);
    genSixFace();    // 更新面
    genTwelveLine(); // 更新边
}

std::vector<double> Hex::getLengthWidthHeight() {
    std::vector<double> lwh;
    lwh.push_back(length);
    lwh.push_back(width);
    lwh.push_back(height);
    return lwh;
}

void Hex::genLWH(){
    length = common::distance(p[5], p[6]);
    width = common::distance(p[4], p[5]);
    height = common::distance(p[1], p[5]);
}


/**
 * @description: rotate to the point center (polymorphic)
 * @param {*}
 * @return {*}
 */
void Hex::rsetRotate(std::vector<double> &alphaBetaGamma, Point3D& center) {
    // before rotate should reset position
    //for (int i = 0; i < 8; i++) {
    //   Point3D tmp = this->p[i] - this->position;
    //   Point3D out;
    //   common::rotationThreeAngle(angleX * -1, angleY * -1, angleZ * -1, tmp, out);
    //   this->p[i] = out + this->position;
    //}
    genEightPoint();
    angleX = setAngle(0);
    angleY = setAngle(0);
    angleZ = setAngle(0);
    
    for (int i = 0; i < 8; i++) {
        Point3D tmp = this->p[i] - this->position;
        Point3D out;
        common::rotationThreeAngle(alphaBetaGamma[0], alphaBetaGamma[1], alphaBetaGamma[2], tmp, out);
        this->p[i] = out + this->position;
    }
    Point3D tt(0, 0, 0);
    for (int i=0; i<8; i++){
        tt += this->p[i];
    }
    tt /= 8;
    this->position = tt;
    angleX += alphaBetaGamma[0];
    angleY += alphaBetaGamma[1];
    angleZ += alphaBetaGamma[2];
    angleX = setAngle(angleX);
    angleY = setAngle(angleY);
    angleZ = setAngle(angleZ);
    genSixFace();    // 更新面
    genTwelveLine(); // 更新边
}

/**
 * @description: rotate to the point center (polymorphic)
 * @param {*}
 * @return {*}
 */
void Hex::rotate(std::vector<double> &alphaBetaGamma, Point3D& center) {
    // before rotate should reset position
    // for (int i = 0; i < 8; i++) {
    //    Point3D tmp = this->p[i] - this->position;
    //    Point3D out;
    //    common::rotationThreeAngle(angleX * -1, angleY * -1, angleZ * -1, tmp, out);
    //    this->p[i] = out + this->position;
    // }
    
    for (int i = 0; i < 8; i++) {
        Point3D tmp = this->p[i] - center;
        Point3D out;
        common::rotationThreeAngle(alphaBetaGamma[0], alphaBetaGamma[1], alphaBetaGamma[2], tmp, out);
        this->p[i] = out + center;
    }
    Point3D tt(0, 0, 0);
    for (int i=0; i<8; i++){
        tt += this->p[i];
    }
    tt /= 8;
    this->position = tt;
    // angleX += alphaBetaGamma[0];
    // angleY += alphaBetaGamma[1];
    // angleZ += alphaBetaGamma[2];
    // angleX = setAngle(angleX);
    // angleY = setAngle(angleY);
    // angleZ = setAngle(angleZ);
    genSixFace();    // 更新面
    genTwelveLine(); // 更新边
}


void Hex::rotate(quat& inq, Point3D& center) {
    for (int i = 0; i < 8; i++) {
        Point3D tmp = this->p[i] - center;
        Point3D out;
        out = common::rotateByQuat(inq, tmp);
        // common::rotationThreeAngle(alphaBetaGamma[0], alphaBetaGamma[1], alphaBetaGamma[2], tmp, out);
        this->p[i] = out + center;
    }
    Point3D tt(0, 0, 0);
    for (int i = 0; i < 8; i++) {
        tt += this->p[i];
    }
    tt /= 8;
    this->position = tt;
    genSixFace();    // 更新面
    genTwelveLine(); // 更新边
}

/**
 * @description: setAngle
 * @param {type}
 * @return {type}
 */
double Hex::setAngle(double angle){
    if(angle > 360){
        angle = angle - 360;
    }
    if(angle < 0) {
        angle = angle + 360;
    }
    return angle;
}

/**
 * @description: Rotate to the center point
 * @param {type}
 * @return {type}
 */
void Hex::rotate(double angleX, double angleY, double angleZ) {
	std::vector<double> lv;
	lv.push_back(angleX);
	lv.push_back(angleY);
	lv.push_back(angleZ);
	rotate(lv);
}

/**
 * @description: edit hex
 * @param {type}
 * @return {type}
 */
void Hex::editHex(Point3D _p, double _length, double _width, double _height) {
    position = _p;
    length = _length;
    width = _width;
    height = _height;
    genEightPoint();
    std::vector<double> lv;
	lv.push_back(angleX);
	lv.push_back(angleY);
	lv.push_back(angleZ);
	rotate_one(lv);
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
}

/**
 * @description: gen six face
 * @param {type}
 * @return {type}
        face (0 1 2 3) 0 up
        face (4 7 6 5) 1 down normal vec toward outer
        face (0 4 5 1) 2 left
        face (7 3 2 6) 3 right
        face (2 1 4 5) 4 front
        face (0 3 7 4) 5 back
 */
void Hex::genSixFace() {
    f[0] = Face(p[0], p[1], p[2], p[3]);
    f[1] = Face(p[4], p[7], p[6], p[5]);
    f[2] = Face(p[0], p[4], p[5], p[1]);
    f[3] = Face(p[7], p[3], p[2], p[6]);
    f[4] = Face(p[2], p[1], p[5], p[6]);
    f[5] = Face(p[0], p[3], p[7], p[4]);
}

/**
 * @description: gen 12 edges
 * @param {type}
 * @return {type}
 *
    line (0 1) 0
    line (1 2) 1
    line (2 3) 2
    line (3 0) 3
    ---
    line (4 5) 4
    line (5 6) 5
    line (6 7) 6
    line (7 4) 7
    ---
    line (0 4) 8
    line (1 5) 9
    line (2 6) 10
    line (3 7) 11
 */

void Hex::genTwelveLine() {
    l[0] = Line(p[0], p[1]);
    l[1] = Line(p[1], p[2]);
    l[2] = Line(p[2], p[3]);
    l[3] = Line(p[3], p[0]);

    l[4] = Line(p[4], p[5]);
    l[5] = Line(p[5], p[6]);
    l[6] = Line(p[6], p[7]);
    l[7] = Line(p[7], p[4]);

    l[8] = Line(p[0], p[4]);
    l[9] = Line(p[1], p[5]);
    l[10] = Line(p[2], p[6]);
    l[11] = Line(p[3], p[7]);
}

/**
 * @description: Get the intersections point
 * @param {type}
 * @return {type}
 */
void Hex::genIntersectionPointWithLine(std::vector<Line> &l) {
    // Generate all the intersections points
    for (int j = 0; j < l.size(); j++) {
        for (int i = 0; i < 6; i++) {
            Point3D p;
            if (Geometry::intersectionLine3DFace3D(l[j], f[i], p)) {
                p.id = i;
                if (p.x == -0.17802900000000002)
                    std::cout << " wait \n";
                intersectionPoints.push_back(p);
                break;
            }
        }
    }
    // Subplanes are generated from all the intersections
    for (int i = 0; i < 6; i++) {
        std::vector<Point3D> tmpP;
        for (int j = 0; j < intersectionPoints.size(); j++) {
            if (intersectionPoints[j].id == i) {
                tmpP.push_back(intersectionPoints[j]);
            }
        }
        std::vector<std::pair<Point3D, Face>> tmpPa;
        if (Geometry::splitFaceUseIntersectionPoints(f[i], tmpP, tmpPa)) {
            for (int k = 0; k < tmpPa.size(); k++) {
                subFace.push_back(tmpPa[k]);
            }
        }
    }
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */

Point3D Hex::genIntersectionPointWithALine(Line &l) {
    Point3D rlt;
    for (int i = 0; i < 6; i++) {
        Point3D p;
        if (Geometry::intersectionLine3DFace3D(l, f[i], p)) {
            rlt = p;
            break;
        }
    }
    return rlt;
}

/**
 * @description: Get a subsurface
 * @param {type}
 * @return {type}
 */
bool Hex::getSubFace(Point3D &p, Face &out) {
    // std::cout << "[ppp]" << p.getXYZ();
    for (int i = 0; i < subFace.size(); i++) {
        // std::cout << "[subF] " << subFace[i].first.getXYZ();
        // std::cout << "[sub] " << (subFace[i].first - p).getXYZ();
        if (subFace[i].first == p) {
            out = subFace[i].second;
            return true;
        }
    }
    return false;
}

/**
 * @description: Get two opposite lines from one line
 * @param {type}
 * @return {type}
 */
std::vector<Line> Hex::getOppositeLine(Line &l) {
    std::vector<Line> rlt;
    for (int i = 0; i < 12; i++) {
        if (this->l[i] == l) {
            rlt.push_back(this->l[lineOppoLines[i].first]);
            rlt.push_back(this->l[lineOppoLines[i].second]);
            return rlt;
        }
    }
    return rlt;
}

/**
 * @description: 根据line的id的生成对边id的集合
 * @param {type}
 * @return {type}
 */
void Hex::genLineOppoLines() {
    std::vector<int> tmp;
    lineOppoLines[0] = std::make_pair(2, 4);
    lineOppoLines[1] = std::make_pair(3, 5);
    lineOppoLines[2] = std::make_pair(0, 6);
    lineOppoLines[3] = std::make_pair(1, 7);
    lineOppoLines[4] = std::make_pair(0, 6);
    lineOppoLines[5] = std::make_pair(1, 7);
    lineOppoLines[6] = std::make_pair(2, 4);
    lineOppoLines[7] = std::make_pair(3, 5);
    lineOppoLines[8] = std::make_pair(9, 11);
    lineOppoLines[9] = std::make_pair(10, 8);
    lineOppoLines[10] = std::make_pair(11, 9);
    lineOppoLines[11] = std::make_pair(10, 8);
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */
Hex::Hex() {
    position = Point3D(0, 0, 0);
    count++;
    width = 0.1;
    height = 0.1;
    length = 0.1;
    id = count;
    angleX = angleY = angleZ = 0;
    genEightPoint();
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
}


/**
 * @description:
 * @param {type}
 * @return {type}
 */
Hex &Hex::operator=(Hex &rhs) {
    position = rhs.position;
    width = rhs.width;
    height = rhs.height;
    length = rhs.length;
    angleX = rhs.angleX;
    angleY = rhs.angleY;
    angleZ = rhs.angleZ;
    for (int i = 0; i < 8; i++) {
        p[i] = rhs.p[i];
    }
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
    return *this;
}

Hex &Hex::operator=(const Hex &rhs) {
    position = rhs.position;
    width = rhs.width;
    height = rhs.height;
    length = rhs.length;
    angleX = rhs.angleX;
    angleY = rhs.angleY;
    angleZ = rhs.angleZ;
    for (int i = 0; i < 8; i++) {
        p[i] = rhs.p[i];
    }
    genTwelveLine();
    genSixFace();
    genLineOppoLines();
    return *this;
}