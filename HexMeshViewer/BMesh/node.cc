/*
 * @Author: your name
 * @Date: 2020-10-05 13:25:41
 * @LastEditTime: 2021-03-18 10:13:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/node.cc
 */
#include "node.hpp"
using namespace BMesh;
/**
 * @description: The destructor
 * @param {type}
 * @return {type}
 */
Node::~Node() {
    if (root) {
        delete root;
    }
}

///**
// * @description: constructor
// * @param {type}
// * @return {type}
// */
//Node::Node(double _x, double _y, double _z, double _r, enum NODETYPE nt) {
//    double l = _r * 2;
//    double w = l;
//    double h = l;
//    Node(_x, _y, _z, l, w, h, nt);
//}


/**
 * @description: constructor
 * @param {*}
 * @return {*}
 */
Node::Node(double _x, double _y, double _z, double _l, double _w, double _h, enum NODETYPE nt){
    position = Point3D(_x, _y, _z);
    length = _l;
    width = _w;
    height = _h;
    id = count;
    count++;
    this->nt = nt;
}

/**
 * @description:
 * @param {type}
 * @return {type}
 */

Node::Node(Point3D &p, enum NODETYPE nt) {
    position = p;
    length = -1;
    width = -1;
    height = -1;
    id = count;
    count++;
    this->nt = nt;
}

Node::Node() {
    position = Point3D(0, 0, 0);
    length = -1;
    width = -1;
    height = -1;
    id = count;
    count++;
    this->nt = SN;
}
/**
 * @description: Increase the child, that is, store the pointer to the child
 * @param {type}
 * @return {type}
 */
bool Node::addChild(Node *n) {
    if (this == n) {
        std::cout << "[ERROR] can't add himself.\n";
        return false; 
    }
    if (children.empty()) {
        children.push_back(n);
        return true;
    }
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == n) {
            // std::cout << "[ERROR] child is already in childrean, not to do this !!" << std::endl;
            return false;
        }
    }
    children.push_back(n);
    return true;
}

/**
 * @description: node id init
 * @param {type}
 * @return {type}
 */
int Node::count = 0;


/**
 * @description: node id init
 * @param {type}
 * @return {type}
 */
void Node::rotate(std::vector<double> &alphaBetaGamma, Point3D& center){
    Point3D tmp = this->position - center;
    Point3D out;
    common::rotationThreeAngle(alphaBetaGamma[0], alphaBetaGamma[1], alphaBetaGamma[2], tmp, out);
    this->position = out + center;
    this->root->rotate(alphaBetaGamma, center);
}
void Node::rotate(quat& inq, Point3D& center) {
    Point3D tmp = this->position - center;
    Point3D out;
    // common::rotationThreeAngle(alphaBetaGamma[0], alphaBetaGamma[1], alphaBetaGamma[2], tmp, out);
    out = common::rotateByQuat(inq, tmp);
    this->position = out + center;
    this->root->rotate(inq, center);
}


/**
 * @description:
 * @param {type}
 * @return {type}
 */
Point3D Node::getPosition() { return position; }
// double Node::getRadius() { return radius; }
std::vector<double> Node::getLengthWidthHeight(){
    std::vector<double> rlt;
    rlt.push_back(length);
    rlt.push_back(width);
    rlt.push_back(height);
    return rlt;
}


/**
 * @description: del node
 * @param {type}
 * @return {type}
 */
bool Node::deleteChild(Node *n) {
    if (children.empty()) {
        return false;
    }
    for (auto it = children.begin(); it != children.end(); it++) {
        if ((*it) == n) {
            // std::cout << "[DEBUG] Address matching to start deleting nodes " << std::endl;
            children.erase(it);
            break;
        }
    }
    return true;
}

/**
 * @description: Set node type
 * @param {type}
 * @return {type}
 */
void Node::setNodeType(enum NODETYPE y) { nt = y; }

/**
 * @description: Get node type
 * @param {type}
 * @return {type}
 */
enum NODETYPE Node::getNodeType() { return nt; };

/**
 * @description: edit it  r != -1 是一个条件
 * @param {type}
 * @return {type}
 */
bool Node::editNode(double _x, double _y, double _z, std::vector<double> lwh) {
    position = Point3D(_x, _y, _z);
    if (lwh.size() != 0) {
        length = lwh[0];
        width = lwh[1];
        height = lwh[2];
    }
    // edit shadow cell
    root->editHex(position, this->length, this->width, this->height);
    return true;
}

/**
 * @description: set length width height
 * @param {type}
 * @return {type}
 */
void Node::setLengthWidthHeight(const std::vector<double>& lwh) {
    if (lwh.size() != 3) {
        std::cerr << "[ERROR] length width height is not equal 3\n";
    }
    length = lwh[0];
    width = lwh[1];
    height = lwh[2];
    root->length = lwh[0];
    root->width = lwh[1];
    root->height = lwh[2];
}