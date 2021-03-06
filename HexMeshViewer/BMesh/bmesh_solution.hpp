/*
 * @Author: your name
 * @Date: 2020-10-03 21:10:24
 * @LastEditTime: 2021-03-16 16:14:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/solution.hpp
 */
#ifndef __BMESH_SOLUTION_HPP__
#define __BMESH_SOLUTION_HPP__
#include "common.hpp"
#include "debug.hpp"
#include "local_vector.hpp"
#include "node.hpp"
#include "string.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include "quat.hpp"
#include <map>
#include "BMesh/rmf.h"
#include "BMesh/interpolationCurve.h"

/**
 * thinking
 * 1. To build the skeleton structure, we have now used id to build the entire skeleton represented by topological
 * relations and adjacency matrix (OK)
 * 2. Generate node hexahedron(OK)
 * 3. Adjust the node hexahedron, Angle(OK)
 * 4. It produces an edge hexahedron(OK)
 * 4.1 Reconstructing the connection relationship(OK)
 * 4.2 Observe the adjacency node for each node and confirm whether there is a situation of large facing small face(OK)
 * 4.3 If there is a large face to a small face, the large face is subdivided according to the side of the small face
 * that is not the large face 4.4 Reconstruction of connection()
 * 5. The output mesh(OK)
 */
namespace BMesh {

class Solution {
public:
    Solution();
    ~Solution();

public:
    void refineJN();
    void setTriOffPath(std::string& path);
    void clearSkel();
    bool readSkel(std::string filename);
    bool readSkelFromCuboid(std::string filename);
    bool readSkelFromCube(std::string filename);
    bool genGraph();
    bool genNodeHex();
    bool genIntersectionPointInHex();
    bool reNewHexPointId();
    bool writeMesh(std::string filename);
    bool writeVtk(std::string filename);
    bool changeRotation();
    bool genlinkPoints();
    bool genlinkNode();
    bool checkNodeFaceExistBigFaceSmallFace();
    bool reNewTopology(Node *oldSplitNode, Node *newSplitNode);
    bool editNodeAngle(Node* v, double angleX, double angleY, double angleZ);
    bool editNodeAngleByQuat(Node* v, quat& inq);
    bool editNodeAngle(Node* root, double angleX, double angleY, double angleZ, Point3D &center);
    bool connectNodes(Node* a, Node* b);
    bool disconnectNodes(Node* a, Node* b);
    bool deleteNodeTopology(Node *node);
    bool editNode(Node *root, Point3D &p, std::vector<double> &lwh);
    Node *selectWhichNode(Point3D &p);
    bool getALine(Node* root, std::vector<Node*> &retNode);
    bool addNode(Node *root, Point3D &p, double angleX, double angleY, double angleZ, double length, double width, double height);
    bool delNode(Node *root);
    void pipeline();
    void pipeline1();
    void pipeline2();
    void pipeline3();
    void pipelineReCon();
    void simpleWithCgal();
    bool intreP();
    void JoinTwoNode(Node* a, Node* b);
    void saveSkel(std::string name);
    void deleteNodeTraceAllNode();
    Node* spiltX(Node* r);
    Node* spiltY(Node* r);
    Node* spiltZ(Node* r);

    bool changeToPolylines();
    void dfs(Node* a, std::map<Node*, bool>& visited, std::map<Node*, int>& toDfs, std::vector<Node*>& path);
    bool useRMF();
    void poly2json();
    void exportJsonForCurveP(std::vector<std::vector<BMesh::Point3D>>& skel);

public:
    void BFSTraverse(Node *a, Node *asplit, Node *asun);
    void BFS(Node *a, Node *asplit);
    // just for find a line
    void BFSFindALine(Node* root, std::vector<Node *> & ret);

public:
    std::string triOffPath;
    std::queue<Node *> Q;
    std::unordered_map<int, bool> visited;
    std::vector<Node *> v;               // Stores a skeleton node topology diagram. as a pure record topology diagram
    std::vector<std::vector<int>> graph; // adjacent matrix
    std::vector<std::pair<Point3D, Point3D>> linkPoints; // point to point
    // std::vector<Hex> h;
    std::unordered_map<std::string, int> m; // Remove duplicate points
    std::unordered_map<int, std::string> mm;
    std::unordered_map<Node*, Frame> frames;
    std::vector<std::vector<BMesh::Point3D>> curvePointss;
    std::vector<Frame> Us;
};
} // namespace BMesh
#endif