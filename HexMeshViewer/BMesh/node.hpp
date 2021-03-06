/*
 * @Author: your name
 * @Date: 2020-10-05 13:25:30
 * @LastEditTime: 2021-03-17 14:23:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/node.hpp
 */
#ifndef __NODE_HPP__
#define __NODE_HPP__
#include "hex.hpp"
#include "point.hpp"
#include <vector>
namespace BMesh {
class Hex;
// Simple storage graph, skeleton graph
enum NODETYPE { BN, JN, SN, TN, DN }; // BN:The initial node;JN: edge node; TN: temp; DN: should be Delete; SN ? spiltNode
class Node {
public:
    ~Node();
    // explicit Node(double _x, double _y, double _z, double _r, enum NODETYPE nt);
    explicit Node(double _x, double _y, double _z, double _l, double _w, double _h, enum NODETYPE nt);
    explicit Node(Point3D &p, enum NODETYPE nt);
    Node();
    bool addChild(Node *);
    Point3D getPosition();
    std::vector<double> getLengthWidthHeight();
    double getLWHradius() { return (length + width + height) / 6; }
    bool deleteChild(Node *);
    bool editNode(double _x, double _y, double _z, std::vector<double> lwh);
    void setLengthWidthHeight(const std::vector<double> &lwh);
    void setNodeType(enum NODETYPE y);
    void rotate(std::vector<double> &alphaBetaGamma, Point3D& center);
    void rotate(quat& inq, Point3D& center);
    enum NODETYPE getNodeType();

public:
    std::vector<Node *> children;
    Hex *root; // It is also best to mount a hexahedron for each node
    Point3D position;
    double length;
    double width;
    double height;
    int id;
    static int count;

private:
    enum NODETYPE nt;
};
} // namespace BMesh

#endif