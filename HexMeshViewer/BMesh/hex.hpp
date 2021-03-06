/*
 * @Author: your name
 * @Date: 2020-10-05 09:36:14
 * @LastEditTime: 2021-04-12 20:22:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/hex.hpp
 */
#ifndef __HEX_HPP__
#define __HEX_HPP__
#include "common.hpp"
#include "face.hpp"
#include "geometry.hpp"
#include "line.hpp"
#include "point.hpp"

/*
* 
          0 ------------- 3
        1 ------------- 2 |          |Z(t)
        | |             | |          |
        | |             | |          |___________Y(s)
        | 4 ------------|-7      _-
        5------------- 6      _-X(r)
        56 is length
        45 is width
        15 is height
        face (0 1 2 3) 0 up
        face (4 7 6 5) 1 down The normal vector is pointing out
        face (0 4 5 1) 2 left
        face (7 3 2 6) 3 right
        face (2 1 5 6) 4 front
        face (0 3 7 4) 5 back
        face pair
                 0 - 1
                 2 - 3
                 4 - 5
        line pair
                0: 2 4 (0 1)  (2 3) (4 5)
                1: 3 5 (1 2)  (3 0) (5 6)
                2: 0 6 (2 3)  (0 1) (6 7)
                3: 1 7 (3 0)  (1 2) (7 4)
                4: 0 6 (4 5)  (0 1) (6 7)
                5: 1 7
                6: 2 4
                7: 3 5
                8: 9 11
                9: 10 8
                10: 11 9
                11: 10 8

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
        ==========
        Where are the intersections stored? I'm going to use dot ID to tell which side it is,
        Whether the traversal line plane intersects or not, according to the order of traversal, is the attribute of the
   point in the edge or corner.
*/
namespace BMesh {
class Face;
class Hex {
public:
    Hex(Point3D _p, const std::vector<double> &lwh);
    Hex(Face &_a, Face &_b);
    Hex(std::vector<Point3D> v);
    Hex();
    ~Hex();

public:
    void genEightPoint();
    void genLWH();
    void genEightPointUseFace(Face &a, Face &b);
    void rotate(std::vector<double> &alphaBetaGamma);
    void rotate_one(std::vector<double> &alphaBetaGamma);
	void rotate(double angleX, double angleY, double angleZ);
    void rotate(std::vector<double> &alphaBetaGamma, Point3D& center);
    void rotate(quat& inq, Point3D& center);
    void rsetRotate(std::vector<double> &alphaBetaGamma, Point3D& center);
    double getRotateX();
    double getRotateY();
    double getRotateZ();
    double getLength();
    double getWidth();
    double getHeight();
    std::vector<double> getLengthWidthHeight();
    double setAngle(double angle);
    void genSixFace();
    void genTwelveLine();
    void genIntersectionPointWithLine(std::vector<Line> &l);
    void editHex(Point3D _p, double _length, double _width, double _height);
    Point3D centerPoint();
    Point3D genIntersectionPointWithALine(Line &l);
    bool getSubFace(Point3D &p, Face &f);
    std::vector<Line> getOppositeLine(Line &l);
    void genLineOppoLines();
    Hex &operator=(Hex &rhs);
    Hex &operator=(const Hex &rhs);
    void useFrameToChangePoint(Frame& f);

public:
    Point3D p[8];
    Point3D position;
    Face f[6];
    Line l[12];
    std::vector<Point3D> intersectionPoints;
    std::vector<std::pair<Point3D, Face>> subFace;
    std::unordered_map<int, std::pair<int, int>> lineOppoLines;
    double length;
    double width;
    double height;
    double id;
    double angleX, angleY, angleZ;
    static int count;
};
} // namespace BMesh
#endif