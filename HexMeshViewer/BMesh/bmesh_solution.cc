/*
 * @Author: your name
 * @Date: 2020-10-03 21:10:38
 * @LastEditTime: 2021-04-13 17:12:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/src/solution.cc
 */
#include "bmesh_solution.hpp"
#include <map>
#include <algorithm>
#include "simpleWithCgal.h"
using namespace BMesh;
/**
 * @description: constructor
 * @param {type}
 * @return {type}
 */
Solution::Solution() {}

/**
 * @description: destructor
 * @param {type}
 * @return {type}
 */
Solution::~Solution() {
    for (int i = 0; i < v.size(); i++) {
        delete v[i];
        // std::cout << "释放内存成功" << std::endl;
    }
}

void Solution::clearSkel() {
    //for (int i = 0; i < v.size(); i++) {
        //delete v[i];
        // std::cout << "释放内存成功" << std::endl;
    //}
    v.clear();
}

/**
 * @description: Read the SKEL skeleton function and build the link relationship of Node V
 * @param {type}
 * @return {type}
 */
bool Solution::readSkel(std::string filename) {
    std::ifstream iff = std::ifstream(filename.c_str(), std::ios::in);
    if (!iff.good()) {
        std::cout << "[ERROR] could not open file " << filename << " for reading!" << std::endl;
        iff.close();
        return false;
    }
    std::string lineStr;
    common::getStrLine(iff, lineStr);
    iff.close();
    if (lineStr == "ID Cx Cy Cz RADIUS #NEIGHBORS NEIGHBORS_LIST") {
        return readSkelFromCube(filename);
    }else if(lineStr == "ID Cx Cy Cz Length Width Height #NEIGHBORS NEIGHBORS_LIST"){
        return readSkelFromCuboid(filename);
    }else {
        return false;
    }

    // if (lineStr != "ID Cx Cy Cz RADIUS #NEIGHBORS NEIGHBORS_LIST") {
    //     std::cout << "[ERROR] file may not support. want .skel" << std::endl;
    //     return false;
    // }
    // common::getStrLine(iff, lineStr);
    // std::stringstream sstr;
    // sstr << lineStr;
    // int number;
    // sstr >> number;
    // Point3D tmp;
    // // gen node
    // std::vector<std::vector<double>> coreInfo;
    // for (int i = 0; i < number; i++) {
    //     common::getStrLine(iff, lineStr);
    //     sstr << lineStr;
    //     std::vector<double> t;
    //     double tmp;
    //     char *p;
    //     p = strtok((char *)lineStr.c_str(), " ");
    //     while (p != NULL) {
    //         tmp = atof(p);
    //         t.push_back(tmp);
    //         p = strtok(NULL, " ");
    //     }
    //     coreInfo.push_back(t);
    //     v.push_back(new Node(t[1], t[2], t[3], t[4], BN));
    // }
    // // Establish a connection based on the ID in the file
    // for (int i = 0; i < number; i++) {
    //     int subLinkNum = coreInfo[i][5];
    //     // std::cout << "[DEBUG] sublinknum " << subLinkNum << std::endl;
    //     for (int j = 0; j < subLinkNum; j++) {
    //         v[i]->addChild(v[(int)coreInfo[i][6 + j]]);
    //     }
    // }
    // iff.close();
    
    return true;
}

/**
 * @description: Generate a graph with id and generate a graph with only one side
 * @param {type}
 * @return {type}
 */
bool Solution::genGraph() {
    graph.resize(v.size());
    for (int i = 0; i < v.size(); i++) {
        graph[i].resize(v.size());
    }
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            graph[i][j] = 0;
        }
    }
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i]->children.size(); j++) {
            if (v[i]->children[j]->id > v[i]->id) {
                graph[i][v[i]->children[j]->id] = 1;
            }
        }
    }
    DBG_D("graph");
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v.size(); j++) {
            DBG_S(graph[i][j]);
            DBG_S(" ");
        }
        std::cout << "\n";
    }
    return true;
}


/**
 * @description: readSkelFromCuboid
 * @param {string} filename
 * @return {*}
 */
bool Solution::readSkelFromCuboid(std::string filename){
    std::ifstream iff = std::ifstream(filename.c_str(), std::ios::in);
    if (!iff.good()) {
        std::cout << "[ERROR] could not open file " << filename << " for reading!" << std::endl;
        iff.close();
        return false;
    }
    std::string lineStr;
    common::getStrLine(iff, lineStr);
    common::getStrLine(iff, lineStr);
    std::stringstream sstr;
    sstr << lineStr;
    int number;
    sstr >> number;
    Point3D tmp;
    // gen node
    std::vector<std::vector<double>> coreInfo;
    for (int i = 0; i < number; i++) {
        common::getStrLine(iff, lineStr);
        sstr << lineStr;
        std::vector<double> t;
        double tmp;
        char *p;
        p = strtok((char *)lineStr.c_str(), " ");
        while (p != NULL) {
            tmp = atof(p);
            t.push_back(tmp);
            p = strtok(NULL, " ");
        }
        coreInfo.push_back(t);
        v.push_back(new Node(t[1], t[2], t[3], t[4], t[5], t[6], BN));
    }
    // Establish a connection based on the ID in the file
    for (int i = 0; i < number; i++) {
        int subLinkNum = coreInfo[i][7];
        // std::cout << "[DEBUG] sublinknum " << subLinkNum << std::endl;
        for (int j = 0; j < subLinkNum; j++) {
            v[i]->addChild(v[(int)coreInfo[i][8 + j]]);
        }
    }
    iff.close();
    genNodeHex();
    return true;
}

/**
 * @description: readSkelFromCube
 * @param {string} filename
 * @return {*}
 */
bool Solution::readSkelFromCube(std::string filename){
    std::ifstream iff = std::ifstream(filename.c_str(), std::ios::in);
    if (!iff.good()) {
        std::cout << "[ERROR] could not open file " << filename << " for reading!" << std::endl;
        iff.close();
        return false;
    }
    std::string lineStr;
    common::getStrLine(iff, lineStr);
    common::getStrLine(iff, lineStr);
    std::stringstream sstr;
    sstr << lineStr;
    int number;
    sstr >> number;
    Point3D tmp;
    // gen node
    std::vector<std::vector<double>> coreInfo;
    for (int i = 0; i < number; i++) {
        common::getStrLine(iff, lineStr);
        sstr << lineStr;
        std::vector<double> t;
        double tmp;
        char *p;
        p = strtok((char *)lineStr.c_str(), " ");
        while (p != NULL) {
            tmp = atof(p);
            t.push_back(tmp);
            p = strtok(NULL, " ");
        }
        coreInfo.push_back(t);
        double length = t[4];

        v.push_back(new Node(t[1], t[2], t[3], length, length, length, BN));
    }
    // Establish a connection based on the ID in the file
    for (int i = 0; i < number; i++) {
        int subLinkNum = coreInfo[i][5];
        // std::cout << "[DEBUG] sublinknum " << subLinkNum << std::endl;
        for (int j = 0; j < subLinkNum; j++) {
            v[i]->addChild(v[(int)coreInfo[i][6 + j]]);
        }
    }
    iff.close();
    genNodeHex();
    return true;
}


/**
 * @description: Generate node hexahedron
 * @param {type}
 * @return {type}
 */
bool Solution::genNodeHex() {
    for (int i = 0; i < v.size(); i++) {
        v[i]->root = new Hex(v[i]->getPosition(), v[i]->getLengthWidthHeight());
    }
    //changeRotation();
    return true;
}

/**
 * @description: Write the body grid
 * Now, a dot in hex is not unique.The output node is fine.
 * @param {type}
 * @return {type}
 */
bool Solution::writeMesh(std::string filename) {
    std::ofstream off = std::ofstream(filename.c_str(), std::ios::out);
    if (!off.good()) {
        std::cout << "[Error] Could not open file " << filename << " for writing!" << std::endl;
        off.close();
        return false;
    }
    // Write header
    off << "MeshVersionFormatted 1" << std::endl;
    off << "Dimension\n3" << std::endl;
    off << "Vertices" << std::endl;
    reNewHexPointId();
    off << m.size() << std::endl;

    // write vertices
    for (int i = 1; i <= m.size(); i++) {
        off << mm[i];
    }
    off << "Hexahedra\n";

    off << v.size() << std::endl;
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < 8; j++) {
            if (j == 7) {
                off << m[v[i]->root->p[j].getXYZ()] << std::endl;
            } else {
                off << m[v[i]->root->p[j].getXYZ()] << " ";
            }
        }
    }
    off << "End\n";
    off.close();
    return true;
}

/**
 * @description: Regenerate the id of the hexahedron
 * @param {type}
 * @return {type}
 */
bool Solution::reNewHexPointId() {
    int count = 0;
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < 8; j++) {
            if (m[v[i]->root->p[j].getXYZ()] == 0) {
                v[i]->root->p[j].id = ++count;
                m[v[i]->root->p[j].getXYZ()] = v[i]->root->p[j].id;
                mm[v[i]->root->p[j].id] = v[i]->root->p[j].getXYZ();
            }
        }
    }
    return true;
}

/**
 * @description: Change the rotation Angle of Hex
 * @param {type}
 * @return {type}
 */
bool Solution::editNodeAngle(Node* v, double angleX, double angleY, double angleZ) {
    if (v == NULL) {
        return false;
    }
    std::vector<double> alphaBetaGamma;
    alphaBetaGamma.push_back(angleX);
    alphaBetaGamma.push_back(angleY);
    alphaBetaGamma.push_back(angleZ);
    v->root->rotate(alphaBetaGamma);
    return true;
}

/**
 * @description: Change the rotation Angle of Hex to center point
 * @param {type}
 * @return {type}
 */
bool Solution::editNodeAngle(Node* root, double angleX, double angleY, double angleZ, Point3D &center) {
    if (root == NULL) {
        return false;
    }
    std::vector<double> alphaBetaGamma;
    alphaBetaGamma.push_back(angleX);
    alphaBetaGamma.push_back(angleY);
    alphaBetaGamma.push_back(angleZ);
    root->rotate(alphaBetaGamma, center);
    
    return true;
}

bool Solution::editNodeAngleByQuat(Node* v, quat &inq) {
    if (v == NULL) {
        return false;
    }
    glm::vec3 ou = eulerAngles(inq);
    //std::vector<double> alphaBetaGamma;
    //alphaBetaGamma.push_back(angleX);
    //alphaBetaGamma.push_back(angleY);
    //alphaBetaGamma.push_back(angleZ);
    v->rotate(inq, v->position);
    return true;
}


bool Solution::useRMF() {
    int ccs = 0;
    int testVa = 0;
    
    for (int i = 0; i < curvePointss.size(); i++) {
        Rmf s;
        BMesh::Point3D a = curvePointss[i][0];
        BMesh::Point3D b = curvePointss[i].back();
        int a1 = 0;
        int b1 = 0;
        for (int j = 0; j < v.size(); j++) {
            if (v[j]->position == a) {
                a1 = j;
                break;
            }
        }
        for (int j = 0; j < v.size(); j++) {
            if (v[j]->position == b) {
                b1 = j;
                break;
            }
        }
        if ((v[a1]->children.size() > v[b1]->children.size()) && v[b1]->children.size() == 1) {
            Frame c = frames[v[a1]];
            s.c = c;
            for (int j = 0; j < curvePointss[i].size(); j++) {
                s.in_points.push_back(BMesh::Lvector(curvePointss[i][j].x, curvePointss[i][j].y, curvePointss[i][j].z));
            }
            s.Run();
            // TEST point
            /*
            if(ccs == 0) 
            {
            
            for (int i = 0; i < 1; i++) {
                std::ofstream out("testEdge1.txt", std::ios::app);
                s.U[i].t = s.U[i].t * 0.1;
                s.U[i].r = s.U[i].r * 0.1;
                s.U[i].s = s.U[i].s * 0.1;
                out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                out << (s.U[i].t + s.U[i].center).x << " " << (s.U[i].t + s.U[i].center).y << " " << (s.U[i].t + s.U[i].center).z << "\n";
                out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                out << (s.U[i].r + s.U[i].center).x << " " << (s.U[i].r + s.U[i].center).y << " " << (s.U[i].r + s.U[i].center).z << "\n";
                out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                out << (s.U[i].s + s.U[i].center).x << " " << (s.U[i].s + s.U[i].center).y << " " << (s.U[i].s + s.U[i].center).z << "\n";
                out.close();
            }
            std::ofstream out("testEdge2.txt", std::ios::app);
            c.s = c.s * 0.2;
            c.r = c.r * 0.2;
            c.t = c.t * 0.2;
            out << c.center.x << " " << c.center.y << " " << c.center.z << " \n";
            out << (c.t + c.center).x << " " << (c.t + c.center).y << " " << (c.t + c.center).z << "\n";
            out << c.center.x << " " << c.center.y << " " << c.center.z << " \n";
            out << (c.r + c.center).x << " " << (c.r + c.center).y << " " << (c.r + c.center).z << "\n";
            out << c.center.x << " " << c.center.y << " " << c.center.z << " \n";
            out << (c.s + c.center).x << " " << (c.s + c.center).y << " " << (c.s + c.center).z << "\n";
            out.close();
            } 
            ccs++;
            */

            for (int k = 0; k < s.U.size(); k++) {
                Us.push_back(s.U[k]);
            }


        }
        else if ((v[a1]->children.size() < v[b1]->children.size()) && v[a1]->children.size() == 1) {
            Frame c = frames[v[b1]];
            s.c = c;
            for (int j = 0; j < curvePointss[i].size(); j++) {
                s.in_points.push_back(BMesh::Lvector(curvePointss[i][j].x, curvePointss[i][j].y, curvePointss[i][j].z));
            }
            std::reverse(s.in_points.begin(), s.in_points.end());
            s.Run();
            /*
            if (ccs == 0)
            {
                for (int i = 0; i < s.U.size(); i++) {
                    std::ofstream out("testEdge1.txt", std::ios::app);
                    s.U[i].t = s.U[i].t * 0.1;
                    s.U[i].r = s.U[i].r * 0.1;
                    s.U[i].s = s.U[i].s * 0.1;
                    out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                    out << (s.U[i].t + s.U[i].center).x << " " << (s.U[i].t + s.U[i].center).y << " " << (s.U[i].t + s.U[i].center).z << "\n";
                    out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                    out << (s.U[i].r + s.U[i].center).x << " " << (s.U[i].r + s.U[i].center).y << " " << (s.U[i].r + s.U[i].center).z << "\n";
                    out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                    out << (s.U[i].s + s.U[i].center).x << " " << (s.U[i].s + s.U[i].center).y << " " << (s.U[i].s + s.U[i].center).z << "\n";
                    out.close();
                }
                std::ofstream out("testEdge2.txt", std::ios::app);
                c.s = c.s * 0.2;
                c.r = c.r * 0.2;
                c.t = c.t * 0.2;
                out << s.U[0].center.x << " " << s.U[0].center.y << " " << s.U[0].center.z << " \n";
                out << (c.t + s.U[0].center).x << " " << (c.t + s.U[0].center).y << " " << (c.t + s.U[0].center).z << "\n";
                out << s.U[0].center.x << " " << s.U[0].center.y << " " << s.U[0].center.z << " \n";
                out << (c.r + s.U[0].center).x << " " << (c.r + s.U[0].center).y << " " << (c.r + s.U[0].center).z << "\n";
                out << s.U[0].center.x << " " << s.U[0].center.y << " " << s.U[0].center.z << " \n";
                out << (c.s + s.U[0].center).x << " " << (c.s + s.U[0].center).y << " " << (c.s + s.U[0].center).z << "\n";
                out.close();
            }
            ccs++;
            */
            for (int k = 0; k < s.U.size(); k++) {
                Us.push_back(s.U[k]);
            }
        }
        else if ((v[a1]->children.size() < v[b1]->children.size())) {
            Frame c = frames[v[b1]];
            s.c = c;
            for (int j = 0; j < curvePointss[i].size(); j++) {
                s.in_points.push_back(BMesh::Lvector(curvePointss[i][j].x, curvePointss[i][j].y, curvePointss[i][j].z));
            }
            std::reverse(s.in_points.begin(), s.in_points.end());
            s.Run();
            /*
            if (ccs == 0)
            {
                for (int i = 0; i < s.U.size(); i++) {
                    std::ofstream out("testEdge1.txt", std::ios::app);
                    s.U[i].t = s.U[i].t * 0.1;
                    s.U[i].r = s.U[i].r * 0.1;
                    s.U[i].s = s.U[i].s * 0.1;
                    out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                    out << (s.U[i].t + s.U[i].center).x << " " << (s.U[i].t + s.U[i].center).y << " " << (s.U[i].t + s.U[i].center).z << "\n";
                    out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                    out << (s.U[i].r + s.U[i].center).x << " " << (s.U[i].r + s.U[i].center).y << " " << (s.U[i].r + s.U[i].center).z << "\n";
                    out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                    out << (s.U[i].s + s.U[i].center).x << " " << (s.U[i].s + s.U[i].center).y << " " << (s.U[i].s + s.U[i].center).z << "\n";
                    out.close();
                }
                std::ofstream out("testEdge2.txt", std::ios::app);
                c.s = c.s * 0.2;
                c.r = c.r * 0.2;
                c.t = c.t * 0.2;
                out << s.U[0].center.x << " " << s.U[0].center.y << " " << s.U[0].center.z << " \n";
                out << (c.t + s.U[0].center).x << " " << (c.t + s.U[0].center).y << " " << (c.t + s.U[0].center).z << "\n";
                out << s.U[0].center.x << " " << s.U[0].center.y << " " << s.U[0].center.z << " \n";
                out << (c.r + s.U[0].center).x << " " << (c.r + s.U[0].center).y << " " << (c.r + s.U[0].center).z << "\n";
                out << s.U[0].center.x << " " << s.U[0].center.y << " " << s.U[0].center.z << " \n";
                out << (c.s + s.U[0].center).x << " " << (c.s + s.U[0].center).y << " " << (c.s + s.U[0].center).z << "\n";
                out.close();
            }
            ccs++;
            */
            for (int k = 0; k < s.U.size(); k++) {
                Us.push_back(s.U[k]);
            }
        }   
        else    if ((v[a1]->children.size() >= v[b1]->children.size())) {
            Frame c = frames[v[a1]];
            s.c = c;
            for (int j = 0; j < curvePointss[i].size(); j++) {
                s.in_points.push_back(BMesh::Lvector(curvePointss[i][j].x, curvePointss[i][j].y, curvePointss[i][j].z));
            }
            s.Run();
            // TEST point
            /*
            if(ccs == 0)
            {

            for (int i = 0; i < 1; i++) {
                std::ofstream out("testEdge1.txt", std::ios::app);
                s.U[i].t = s.U[i].t * 0.1;
                s.U[i].r = s.U[i].r * 0.1;
                s.U[i].s = s.U[i].s * 0.1;
                out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                out << (s.U[i].t + s.U[i].center).x << " " << (s.U[i].t + s.U[i].center).y << " " << (s.U[i].t + s.U[i].center).z << "\n";
                out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                out << (s.U[i].r + s.U[i].center).x << " " << (s.U[i].r + s.U[i].center).y << " " << (s.U[i].r + s.U[i].center).z << "\n";
                out << s.U[i].center.x << " " << s.U[i].center.y << " " << s.U[i].center.z << "\n";
                out << (s.U[i].s + s.U[i].center).x << " " << (s.U[i].s + s.U[i].center).y << " " << (s.U[i].s + s.U[i].center).z << "\n";
                out.close();
            }
            std::ofstream out("testEdge2.txt", std::ios::app);
            c.s = c.s * 0.2;
            c.r = c.r * 0.2;
            c.t = c.t * 0.2;
            out << c.center.x << " " << c.center.y << " " << c.center.z << " \n";
            out << (c.t + c.center).x << " " << (c.t + c.center).y << " " << (c.t + c.center).z << "\n";
            out << c.center.x << " " << c.center.y << " " << c.center.z << " \n";
            out << (c.r + c.center).x << " " << (c.r + c.center).y << " " << (c.r + c.center).z << "\n";
            out << c.center.x << " " << c.center.y << " " << c.center.z << " \n";
            out << (c.s + c.center).x << " " << (c.s + c.center).y << " " << (c.s + c.center).z << "\n";
            out.close();
            }
            ccs++;
            */

            for (int k = 0; k < s.U.size(); k++) {
                Us.push_back(s.U[k]);
            }


        }
    }

    for (int j = 0; j < v.size(); j++) {
        double minDistance = INT_MAX;
        int minIndex = -1;
        if (v[j]->children.size() >= 3) {
            continue;
        }
        for (int k = 0; k < Us.size(); k++) {

            if (minDistance > BMesh::Lvector(v[j]->position, Us[k].center).norm()) {
                minDistance = BMesh::Lvector(v[j]->position, Us[k].center).norm();
                minIndex = k;
            }
        }
        v[j]->root->useFrameToChangePoint(Us[minIndex]);
        testVa++;
    }
    std::cout << "[DEBUG] " << testVa << std::endl;
    return true;
}


/**
 * @description: Change the rotation Angle of Hex
 * @param {type}
 * @return {type}
 */
bool Solution::changeRotation() {
//    omp_set_num_threads(4);
//#pragma omp parallel
//    {
//#pragma omp for
    bool check = false;
        for (int i = 0; i < v.size(); i++) {
            // Get the vector of each node that v is connected to
            if (v[i]->children.size() < 3  )
                continue;
            check = true;
            std::vector<Lvector> allVec;
            for (int j = 0; j < v[i]->children.size(); j++) {
                allVec.push_back(Lvector(v[i]->root->position, v[i]->children[j]->root->position).normalize()); //
            }
            std::vector<double> alphaBetaGamma;
            Frame rlt = common::optimization(allVec, alphaBetaGamma, v[i]->getLengthWidthHeight());
            frames[v[i]] = rlt;
            // std::cout << "[DEBUG angle]" << alphaBetaGamma[0] << " " <<
            // alphaBetaGamma[1] << " " << alphaBetaGamma[2] << std::endl;
            v[i]->root->rsetRotate(alphaBetaGamma, v[i]->root->centerPoint());
            // TEST Point
            /*
            std::ofstream ofs("testEDGEtt.txt");
            for (int k = 0; k < 12; k++) {
                ofs << v[i]->root->l[k].v[0].x << " " << v[i]->root->l[k].v[0].y << " " << v[i]->root->l[k].v[0].z << " \n";
                ofs << v[i]->root->l[k].v[1].x << " " << v[i]->root->l[k].v[1].y << " " << v[i]->root->l[k].v[1].z << " \n";
            }
            for (int k = 0; k < 8; k++) {
                std::cout << "[DEBUG] " << v[i]->root->p[k].getXYZ();
            }
            ofs << "0" << " " << "0" << " " <<"0" << "\n";
            ofs << rlt.r.x << " " << rlt.r.y << " " << rlt.r.z << "\n";
            ofs << "0" << " " << "0" << " " << "0" << "\n";
            ofs << rlt.s.x << " " << rlt.s.y << " " << rlt.s.z << "\n";
            ofs << "0" << " " << "0" << " " << "0" << "\n";
            ofs << rlt.t.x << " " << rlt.t.y << " " << rlt.t.z << "\n";
            ofs.close();
            */
        }
        if (!check) {
            std::vector<Lvector> allVec;
            for (int j = 0; j < v[0]->children.size(); j++) {
                allVec.push_back(Lvector(v[0]->root->position, v[0]->children[j]->root->position).normalize()); //
            }
            std::vector<double> alphaBetaGamma;
            Frame rlt = common::optimization(allVec, alphaBetaGamma, v[0]->getLengthWidthHeight());
            frames[v[0]] = rlt;
            // std::cout << "[DEBUG angle]" << alphaBetaGamma[0] << " " <<
            // alphaBetaGamma[1] << " " << alphaBetaGamma[2] << std::endl;
            v[0]->root->rsetRotate(alphaBetaGamma, v[0]->root->centerPoint());
    }
    //}
    return true;
}

/**
 * @description: The junction point of the connected skeleton of the node is obtained
 * @param {type}
 * @return {type}
 */
bool Solution::genIntersectionPointInHex() {
    for (int i = 0; i < v.size(); i++) {
        // Get the vector of each node that v is connected to
        std::vector<Line> allLine;
        for (int j = 0; j < v[i]->children.size(); j++) {
            allLine.push_back(Line(v[i]->root->position, v[i]->children[j]->root->position));
        }
        v[i]->root->genIntersectionPointWithLine(allLine);
    }
    return true;
}

/**
 * @description: not use
 * @param {type}
 * @return {type}
 */
bool Solution::genlinkNode() { return true; }

/**
 * @description: I'm going to generate a pair of intersections
 * @param {type}
 * @return {type}
 */
bool Solution::genlinkPoints() {
    int tmpVNum = v.size();
    std::vector<std::pair<Node *, Node *>> tmpBN;
    std::vector<Node *> tmpJN;

    for (int i = 0; i < tmpVNum; i++) {
        for (int j = 0; j < v[i]->children.size(); j++) {
            Line aLine;
            if (v[i]->children[j]->id > v[i]->id &&
                (v[i]->children[j]->getNodeType() == NODETYPE::BN && v[i]->getNodeType() == NODETYPE::BN)) {
                
                aLine = Line(v[i]->root->position, v[i]->children[j]->root->position);
                Point3D a = v[i]->root->genIntersectionPointWithALine(aLine);
                Point3D b = v[i]->children[j]->root->genIntersectionPointWithALine(aLine);
                if(a == b) { // neighbor
                    continue;
                }
                linkPoints.push_back(std::make_pair(a, b));
                // gen JN
                Face out1;
                Face out2;
                if (!v[i]->root->getSubFace(a, out1)) {
                    std::cout << "[ERROR] can't find point face 1, NODETYPE " << v[i]->getNodeType() << std::endl;
                    continue;
                }
                if (!v[i]->children[j]->root->getSubFace(b, out2)) {
                    std::cout << "[ERROR] can't find point face 2, NODETYPE " << b.getXYZ() << std::endl;
                    continue;
                }
                if (v[i]->root->getSubFace(a, out1) && v[i]->children[j]->root->getSubFace(b, out2)) {
                    Point3D midPoint(0, 0, 0);
                    midPoint = out1.getMidFacePoint();
                    Point3D outp = out2.getMidFacePoint();
                    midPoint += outp;
                    // std::cout << "[DEBUG out1] " << out1.getPointsString();
                    v.push_back(new Node(midPoint / 2.0, JN));
                    v[v.size() - 1]->root = new Hex(out1, out2); // 因为是根据面的中心点构成的在下方会引入误差
                    tmpBN.push_back(std::make_pair(v[i], v[i]->children[j]));
                    tmpJN.push_back(v[v.size() - 1]);
                }
            }
        }
    }
    // Change all connection relationships here
    for (int i = 0; i < tmpBN.size(); i++) {
        if (!Geometry::changeLink(tmpBN[i].first, tmpBN[i].second, tmpJN[i])) {
            std::cout << "[ERROR] Failed to change node connection relationship!!" << std::endl;
        }
    }
    return true;
}

Node* Solution::spiltX(Node* r){
    Point3D tmpP = Point3D(0, 0, 0);
    v.push_back(new Node(tmpP, BN));
    v[v.size() - 1]->root = new Hex();
    v[v.size() - 1]->setLengthWidthHeight(r->getLengthWidthHeight());
    *(v[v.size() - 1]->root) = *(r->root);
    r->root->p[1] = (r->root->p[1] + r->root->p[0]) / 2;
    r->root->p[2] = (r->root->p[2] + r->root->p[3]) / 2;
    r->root->p[5] = (r->root->p[5] + r->root->p[4]) / 2;
    r->root->p[6] = (r->root->p[6] + r->root->p[7]) / 2;
    v[v.size() - 1]->root->p[0] = r->root->p[1];
    v[v.size() - 1]->root->p[3] = r->root->p[2];
    v[v.size() - 1]->root->p[4] = r->root->p[5];
    v[v.size() - 1]->root->p[7] = r->root->p[6];
    v[v.size() - 1]->root->position = v[v.size() - 1]->root->centerPoint();
    r->root->position = r->root->centerPoint();
    v[v.size() - 1]->position = v[v.size() - 1]->root->position;
    r->position = r->root->position;
    

    r->root->genLWH();
    r->setLengthWidthHeight(r->root->getLengthWidthHeight());
    r->root->genTwelveLine();
    r->root->genSixFace();
    r->root->genLineOppoLines();
    v[v.size() - 1]->root->genTwelveLine();
    v[v.size() - 1]->root->genSixFace();
    v[v.size() - 1]->root->genLineOppoLines();
    v[v.size() - 1]->root->genLWH();
    v[v.size() - 1]->setLengthWidthHeight(v[v.size() - 1]->root->getLengthWidthHeight());
    // Rebuild the link relationship
    for (int k = 0; k < r->children.size(); k++) {
        v[v.size() - 1]->addChild(r->children[k]);
    }
    v[v.size() - 1]->addChild(r);
    // v[v.size()-1]->deleteChild(v[i]->children[j]);
    r->addChild(v[v.size() - 1]);
    reNewTopology(r, v[v.size() - 1]);
    //deleteNodeTopology(r);
    //deleteNodeTopology(v[v.size() - 1]);
    return v[v.size() - 1];
}

/**
 * @description: 
 * @param {*}
 * @return {*}
 */
Node* Solution::spiltY(Node* r){
    Point3D tmpP = Point3D(0, 0, 0);
    v.push_back(new Node(tmpP, BN));
    v[v.size() - 1]->root = new Hex();
    v[v.size() - 1]->setLengthWidthHeight(r->getLengthWidthHeight());
    *(v[v.size() - 1]->root) = *(r->root);
    
    r->root->p[2] = (r->root->p[1] + r->root->p[2]) / 2;
    r->root->p[3] = (r->root->p[0] + r->root->p[3]) / 2;
    r->root->p[6] = (r->root->p[5] + r->root->p[6]) / 2;
    r->root->p[7] = (r->root->p[4] + r->root->p[7]) / 2;
    v[v.size() - 1]->root->p[1] = r->root->p[2];
    v[v.size() - 1]->root->p[0] = r->root->p[3];
    v[v.size() - 1]->root->p[5] = r->root->p[6];
    v[v.size() - 1]->root->p[4] = r->root->p[7];
    v[v.size() - 1]->root->position = v[v.size() - 1]->root->centerPoint();
    r->root->position = r->root->centerPoint();
    v[v.size() - 1]->position = v[v.size() - 1]->root->position;
    r->position = r->root->position;
    r->root->genLWH();
    r->setLengthWidthHeight(r->root->getLengthWidthHeight());
    r->root->genTwelveLine();
    r->root->genSixFace();
    r->root->genLineOppoLines();
    v[v.size() - 1]->root->genTwelveLine();
    v[v.size() - 1]->root->genSixFace();
    v[v.size() - 1]->root->genLineOppoLines();
    v[v.size() - 1]->root->genLWH();
    v[v.size() - 1]->setLengthWidthHeight(v[v.size() - 1]->root->getLengthWidthHeight());
    // Rebuild the link relationship
    for (int k = 0; k < r->children.size(); k++) {
        v[v.size() - 1]->addChild(r->children[k]);
    }
    v[v.size() - 1]->addChild(r);
    // v[v.size()-1]->deleteChild(v[i]->children[j]);
    r->addChild(v[v.size() - 1]);
    reNewTopology(r, v[v.size() - 1]);
    //deleteNodeTopology(r);
    //deleteNodeTopology(v[v.size() - 1]);
    return v[v.size() - 1];
}


/**
 * @description: 
 * @param {*}
 * @return {*}
 */
Node* Solution::spiltZ(Node* r){
    Point3D tmpP = Point3D(0, 0, 0);
    v.push_back(new Node(tmpP, BN));
    v[v.size() - 1]->root = new Hex();
    v[v.size() - 1]->setLengthWidthHeight(r->getLengthWidthHeight());
    *(v[v.size() - 1]->root) = *(r->root);
    
    r->root->p[0] = (r->root->p[0] + r->root->p[4]) / 2;
    r->root->p[1] = (r->root->p[1] + r->root->p[5]) / 2;
    r->root->p[2] = (r->root->p[2] + r->root->p[6]) / 2;
    r->root->p[3] = (r->root->p[3] + r->root->p[7]) / 2;
    v[v.size() - 1]->root->p[4] = r->root->p[0];
    v[v.size() - 1]->root->p[5] = r->root->p[1];
    v[v.size() - 1]->root->p[6] = r->root->p[2];
    v[v.size() - 1]->root->p[7] = r->root->p[3];
    v[v.size() - 1]->root->position = v[v.size() - 1]->root->centerPoint();
    r->root->position = r->root->centerPoint();
    v[v.size() - 1]->position = v[v.size() - 1]->root->position;
    r->position = r->root->position;
    r->root->genLWH();
    r->setLengthWidthHeight(r->root->getLengthWidthHeight());
    r->root->genTwelveLine();
    r->root->genSixFace();
    r->root->genLineOppoLines();
    v[v.size() - 1]->root->genTwelveLine();
    v[v.size() - 1]->root->genSixFace();
    v[v.size() - 1]->root->genLineOppoLines();
    v[v.size() - 1]->root->genLWH();
    v[v.size() - 1]->setLengthWidthHeight(v[v.size() - 1]->root->getLengthWidthHeight());
    // Rebuild the link relationship
    for (int k = 0; k < r->children.size(); k++) {
        v[v.size() - 1]->addChild(r->children[k]);
    }
    v[v.size() - 1]->addChild(r);
    // v[v.size()-1]->deleteChild(v[i]->children[j]);
    r->addChild(v[v.size() - 1]);
    reNewTopology(r, v[v.size() - 1]);
    //deleteNodeTopology(r);
    //deleteNodeTopology(v[v.size() - 1]);
    return v[v.size() - 1];
}

/**
 * @description: To see if there are faces at each node, the connections are large to small
 * @param {type}
 * @return {type}
 */
bool Solution::checkNodeFaceExistBigFaceSmallFace() {
    // Traversing each node, traversing each node's corresponding neighbor node
    std::vector<Point3D> diffPoint;
    std::vector<Point3D> samePoint;
    int ai = -1;
    int bi = -1;
    int tmpvN = v.size();
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i]->children.size(); j++) {
            // geometry Check whether there is a big face and a small face. If there is a big face and a small face,
            // divide the big face and rebuild the link relationship std::cout << "[DEBUG] id " << v[i]->id << " " <<
            // v[i]->children[j]->id << std::endl;
            unsigned int rlt =
                Geometry::isAHexFaceContainBHexFace(v[i]->root, v[i]->children[j]->root, diffPoint, samePoint, ai, bi);
            if (rlt == 1) {
                // Face A contains face B and divides the hexahedron of A
                Point3D tmpP = Point3D(0, 0, 0);
                v.push_back(new Node(tmpP, SN));
                v[v.size() - 1]->root = new Hex();
                if (Geometry::splitHexUseALine(*(v[i]->root), *(v[v.size() - 1]->root), diffPoint, samePoint, ai)) {
                    DBG_D("can split it\n");
                } else {
                    DBG_D("[-] can't split it\n");
                }
                // Rebuild the link relationship
                for (int k = 0; k < v[i]->children.size(); k++) {
                    v[v.size() - 1]->addChild(v[i]->children[k]);
                }
                v[v.size() - 1]->addChild(v[i]);
                // v[v.size()-1]->deleteChild(v[i]->children[j]);
                v[i]->addChild(v[v.size() - 1]);
                reNewTopology(v[i], v[v.size() - 1]);

                BFSTraverse(v[i], v[v.size() - 1], v[i]->children[j]);
                i = 0; // Re-traverse
                j = 0;
            } else if (rlt == 4) {
                // Face B contains face A, and the hexahedron of B is divided
                Point3D tmpP = Point3D(0, 0, 0);
                v.push_back(new Node(tmpP, SN));
                v[v.size() - 1]->root = new Hex();
                if (Geometry::splitHexUseALine(*(v[i]->children[j]->root), *(v[v.size() - 1]->root), diffPoint,
                                               samePoint, bi)) {
                    DBG_D("can split it\n");
                } else {
                    DBG_D("[-] can't split it\n");
                }
                // Rebuild the link relationship
                for (int k = 0; k < v[i]->children[j]->children.size(); k++) {
                    v[v.size() - 1]->addChild(v[i]->children[j]->children[k]);
                }

                // v[v.size()-1]->deleteChild(v[i]);
                v[i]->children[j]->addChild(v[v.size() - 1]);
                // add A and B can have relation with C's children
                // for(int t=0; t<v[i]->children.size(); t++){
                //     v[v.size()-1]->addChild(v[i]->children[t]);
                // }
                reNewTopology(v[i]->children[j], v[v.size() - 1]);

                BFSTraverse(v[i]->children[j], v[v.size() - 1], v[i]);
                i = 0; // Re-traverse
                j = 0;
            } else if (rlt == 3) {
                // v[i]->deleteChild(v[i]->children[j]);
                // v[i]->children[j]->deleteChild(v[i]);
            } else if (rlt == 6) {
                // Side A intersects with side B to split the hexahedron of B
                Point3D tmpP = Point3D(0, 0, 0);
                v.push_back(new Node(tmpP, SN));
                v[v.size() - 1]->root = new Hex();
                int rlt = 0;
                if (Geometry::splitHexUseALine(*(v[i]->children[j]->root), *(v[v.size() - 1]->root), *(v[i]->root), diffPoint,
                                               samePoint, ai, bi, rlt)) {
                    DBG_D("can split it\n");
                } else {
                    DBG_D("[-] can't split it\n");
                }
                if (rlt == 0) {
                    for (int k = 0; k < v[i]->children[j]->children.size(); k++) {
                        v[v.size() - 1]->addChild(v[i]->children[j]->children[k]);
                    }

                    // v[v.size()-1]->deleteChild(v[i]);
                    v[i]->children[j]->addChild(v[v.size() - 1]);
                    // add A and B can have relation with C's children
                    // for(int t=0; t<v[i]->children.size(); t++){
                    //     v[v.size()-1]->addChild(v[i]->children[t]);
                    // }
                    reNewTopology(v[i]->children[j], v[v.size() - 1]);

                    BFSTraverse(v[i]->children[j], v[v.size() - 1], v[i]);
                    i = 0; // Re-traverse
                    j = 0;
                }
                else {
                    // Rebuild the link relationship
                    for (int k = 0; k < v[i]->children.size(); k++) {
                        v[v.size() - 1]->addChild(v[i]->children[k]);
                    }
                    v[v.size() - 1]->addChild(v[i]);
                    // v[v.size()-1]->deleteChild(v[i]->children[j]);
                    v[i]->addChild(v[v.size() - 1]);
                    reNewTopology(v[i], v[v.size() - 1]);

                    BFSTraverse(v[i], v[v.size() - 1], v[i]->children[j]);
                    i = 0; // Re-traverse
                    j = 0;
                }
                
            }
        }
    }
    return true;
}

/**
 * @description: Breadth first traversal
 * @param {type}
 * @return {type}
 */
void Solution::BFSTraverse(Node *a, Node *asplit, Node *asun) {
    int tmpVn = v.size();
    visited.clear();
    std::queue<Node *> empty;
    std::swap(empty, Q);
    visited[a->id] = true;
    // visited[asun->id] = true;
    visited[asplit->id] = true;
    BFS(a, asplit);
    DBG_D("BFS %d %d %d\n", a->id, asplit->id, asun->id);
}

/**
 * @description: BFS core function
 * @param v is id
 * @return {type}
 */
void Solution::BFS(Node *a, Node *asplit) {
    DBG_D("visit Node: %d %d\n", (a->id), asplit->id);
    Q.push(a);
    Q.push(asplit);
    while (!Q.empty()) {
        Node *a = Q.front();
        Q.pop();
        for (int i = 0; i < a->children.size(); i++) {
            if (visited[a->children[i]->id] == false) {
                // manage
                std::vector<Point3D> diffPoint;
                std::vector<Point3D> samePoint;
                int ai = -1;
                int bi = -1;
                unsigned int rlt =
                    Geometry::isAHexFaceContainBHexFace(a->root, a->children[i]->root, diffPoint, samePoint, ai, bi);
                if (rlt == 1) {
                    visited[a->children[i]->id] = true;
                } else if (rlt == 4) {
                    // Face B contains face A, and the hexahedron of B is divided
                    Q.push(a->children[i]);

                    Point3D tmpP = Point3D(0, 0, 0);
                    v.push_back(new Node(tmpP, SN));
                    v[v.size() - 1]->root = new Hex();
                    if (Geometry::splitHexUseALine(*(a->children[i]->root), *(v[v.size() - 1]->root), diffPoint,
                                                   samePoint, bi)) {
                        DBG_D("can split it\n");
                    } else {
                        DBG_D("[-] can't split it\n");
                    }
                    // Rebuild the link relationship
                    for (int k = 0; k < a->children[i]->children.size(); k++) {
                        v[v.size() - 1]->addChild(a->children[i]->children[k]);
                    }
                    v[v.size() - 1]->addChild(a->children[i]);
                    // v[vNum-1]->deleteChild(a);
                    a->children[i]->addChild(v[v.size() - 1]);

                    // for(int t=0; t<a->children.size(); t++){
                    //     v[v.size()-1]->addChild(a->children[t]);
                    // }
                    reNewTopology(a->children[i], v[v.size() - 1]);

                    visited[v[v.size() - 1]->id] = true; // 不希望分裂的再次分裂
                    visited[a->children[i]->id] = true;
                } else if (rlt == 3) {

                    // v[i]->deleteChild(v[i]->children[j]);
                    // v[i]->children[j]->deleteChild(v[i]);
                } else if (rlt == 2) {
                    visited[a->children[i]->id] = true;
                } else if (rlt == 6) {
                    // Q.push(a->children[i]);
                    Point3D tmpP = Point3D(0, 0, 0);
                    v.push_back(new Node(tmpP, SN));
                    v[v.size() - 1]->root = new Hex();
                    int rlt = 2;
                    if (Geometry::splitHexUseALine(*(a->children[i]->root), *(v[v.size() - 1]->root), *(a->root), diffPoint,
                                                   samePoint, ai, bi, rlt)) {
                        DBG_D("can split it\n");
                    } else {
                        DBG_D("[-] can't split it\n");
                    }
                    // Rebuild the link relationship
                    for (int k = 0; k < a->children[i]->children.size(); k++) {
                        v[v.size() - 1]->addChild(a->children[i]->children[k]);
                    }
                    v[v.size() - 1]->addChild(a->children[i]);
                    // v[vNum-1]->deleteChild(a);
                    a->children[i]->addChild(v[v.size() - 1]);

                    // for(int t=0; t<a->children.size(); t++){
                    //     v[v.size()-1]->addChild(a->children[t]);
                    // }
                    reNewTopology(a->children[i], v[v.size() - 1]);

                    visited[v[v.size() - 1]->id] = true; // 不希望分裂的再次分裂
                    visited[a->children[i]->id] = true;
                }
            }
        }
    }
    DBG_D("BFS over\n");
}

/**
 * @description: reNewTopology let origin split node's children connect the new split node
 * @param v is id
 * @return {type}
 */
bool Solution::reNewTopology(Node *oldSplitNode, Node *newSplitNode) {
    for (int i = 0; i < oldSplitNode->children.size(); i++) {
        if (oldSplitNode->id != newSplitNode->id) {
            oldSplitNode->children[i]->addChild(newSplitNode);
            newSplitNode->addChild(oldSplitNode->children[i]);
        }
    }
    return true;
}

/**
 * @description: connect a and b
 * @param v is id
 * @return {type}
 */
bool Solution::connectNodes(Node* a, Node* b) {
    a->addChild(b);
    b->addChild(a);
    return true;
}

/**
 * @description: disconnect a and b
 * @param v is id
 * @return {type}
 */
bool Solution::disconnectNodes(Node* a, Node* b) {
    a->deleteChild(b);
    b->deleteChild(a);
    return true;
}


/**
 * @description: Write VTK files
 * @param {*}
 * @return {*}
 */
bool Solution::writeVtk(std::string filename) {
    std::ofstream off = std::ofstream(filename.c_str(), std::ios::out);
    if (!off.good()) {
        std::cout << "[Error] Could not open file " << filename << " for writing!" << std::endl;
        off.close();
        return false;
    }
    // Write header
    off << "# vtk DataFile Version 2.0" << std::endl;
    off << "mesh vtk data - converted from .off" << std::endl;
    off << "ASCII" << std::endl;
    off << "DATASET UNSTRUCTURED_GRID" << std::endl;
    reNewHexPointId();
    off << "POINTS " << m.size() << " double" << std::endl;
    // write vertices
    for (int i = 1; i <= m.size(); i++) {
        off << mm[i];
    }
    off << "CELLS " << v.size() << " " << v.size() * 9 << std::endl;
    for (int i = 0; i < v.size(); i++) {
        off << "8 ";
        for (int j = 0; j < 8; j++) {
            if (j == 7) {
                off << m[v[i]->root->p[j].getXYZ()] - 1 << std::endl;
            } else {
                off << m[v[i]->root->p[j].getXYZ()] - 1 << " ";
            }
        }
    }
    off << "CELL_TYPES " << v.size() << std::endl;
    for (int i = 0; i < v.size(); i++) {
        off << "12\n";
    }
    off.close();
    return true;
}

/**
 * @description: editNode
 * @param v is id
 * @return {type}
 */
bool Solution::editNode(Node *root, Point3D &p, std::vector<double> &lwh) {
    root->editNode(p.x, p.y, p.z, lwh);
    return true;
}

/**
 * @description: choose Node
 * @param v is id
 * @return {type}
 */
Node *Solution::selectWhichNode(Point3D &p) {
    double minDistance = 999;
    Node *rlt = nullptr;
    for (int i = 0; i < v.size(); i++) {
        double tmpDis = Geometry::distanceTwoPoint(v[i]->position, p);
        if (minDistance > tmpDis) {
            minDistance = tmpDis;
            rlt = v[i];
        }
    }
    return rlt;
}

/**
 * @description: addNode
 * @param
 * @return {type}
 */
bool Solution::addNode(Node *root, Point3D &p, double angleX, double angleY, double angleZ, double length, double width, double height) {
    v.push_back(new Node(p.x, p.y, p.z, length, width, height, BN));
    // establish a connection
    root->addChild(v[v.size() - 1]);
    v[v.size() - 1]->addChild(root);
    std::vector<double> alphaBetaGamma;
    alphaBetaGamma.push_back(angleX);
    alphaBetaGamma.push_back(angleY);
    alphaBetaGamma.push_back(angleZ);
    // gen Hex
    v[v.size() - 1]->root = new Hex(v[v.size() - 1]->getPosition(), v[v.size() - 1]->getLengthWidthHeight());
    v[v.size() - 1]->root->rotate(alphaBetaGamma);
    // redraw
    return true;
}

/**
 * @description: delNode
 * @param
 * @return {type}
 */
bool Solution::delNode(Node *root) {
    if (root == nullptr) {
        return false;
    }
    for (int i = 0; i < root->children.size(); i++) {
        root->children[i]->deleteChild(root);
    }
    // del hex
    delete root->root;
    // del node
    for (auto it = v.begin(); it != v.end(); it++) {
        if (*it == root) {
            it = v.erase(it);
            std::cout << "[DEBUG] erase success!" << std::endl;
            break;
        }
    }
    return true;
}


void Solution::pipelineReCon() {
    if (triOffPath == "")
        return;
    simpleWithCgal();
    changeToPolylines();
    poly2json();
    if (!intreP()) {
        changeToPolylines();
        poly2json();
        intreP();
    }
    changeRotation();
    
    useRMF();
}


void Solution::setTriOffPath(std::string& path) {
    triOffPath = path;
}

void Solution::simpleWithCgal() {
    CgalMcfskel tt;
    tt.NowProcessPath = triOffPath;
    tt.outProcessPath = "simpleWithCgal.skel";
    tt.Run();
    clearSkel();
    this->readSkel("simpleWithCgal.skel");
}

void Solution::pipeline() {
    changeRotation();
    genIntersectionPointInHex();
    genlinkPoints();
    checkNodeFaceExistBigFaceSmallFace();
    writeMesh("coarse.mesh");
}

void Solution::pipeline1() {
    
    changeToPolylines();
    poly2json();
    InterpolateCurve a;
    a.genPoints("interPCurveSkel.json");
    curvePointss = a.curvePointss;
    //clearSkel();
    //this->readSkel("result.skel");
    changeRotation();
    useRMF();
}
void Solution::pipeline2() {
    genIntersectionPointInHex();
    genlinkPoints();
}
void Solution::pipeline3() {
    checkNodeFaceExistBigFaceSmallFace();
    writeMesh("coarse.mesh");
}

void Solution::refineJN() {
    for (int i = 0; i < v.size(); i++) {
        if (v[i]->getNodeType() == BMesh::JN) {
            double r1 = v[i]->children[0]->getLWHradius();
            double r2 = v[i]->children[1]->getLWHradius();
            double len1 = BMesh::Geometry::distanceTwoPoint(v[i]->children[0]->getPosition(),
                v[i]->children[1]->getPosition());
            double len2 = len1 - r1 - r2;
            double minData = std::min(len2 / (2 * r1), len2 / (2 * r2));
            double maxR = -1;
            if (r2 >= r1) {
                maxR = r2;
            }
            else {
                maxR = r1;
            }

            if (minData > 2) {
                // should split
                int splitNum = minData;
                // split face point
                std::vector<BMesh::Point3D> a;
                std::vector<BMesh::Point3D> b;
                std::vector<BMesh::Point3D> c;
                std::vector<BMesh::Point3D> d;
                BMesh::Node* an = v[i]->children[0];
                BMesh::Node* bn = v[i]->children[1];
                {
                    BMesh::Point3D aa = v[i]->root->l[8].v[0];
                    BMesh::Point3D bb = v[i]->root->l[8].v[1];
                    BMesh::Lvector ll(aa, bb);
                    double len = ll.norm();
                    double splitLen = len / (double)splitNum;
                    BMesh::Point3D tmpA = ll.getNormalize() * splitLen + aa;
                    a.push_back(aa);
                    a.push_back(tmpA);
                    for (int t = 1; t < splitNum - 1; t++) {
                        BMesh::Point3D tmpAA = ll.getNormalize() * splitLen + a.back();
                        a.push_back(tmpAA);
                    }
                    a.push_back(bb);
                }
                {
                    BMesh::Point3D aa = v[i]->root->l[9].v[0];
                    BMesh::Point3D bb = v[i]->root->l[9].v[1];
                    BMesh::Lvector ll(aa, bb);
                    double len = ll.norm();
                    double splitLen = len / (double)splitNum;
                    BMesh::Point3D tmpA = ll.getNormalize() * splitLen + aa;
                    b.push_back(aa);
                    b.push_back(tmpA);
                    for (int t = 1; t < splitNum - 1; t++) {
                        BMesh::Point3D tmpAA = ll.getNormalize() * splitLen + b.back();
                        b.push_back(tmpAA);
                    }
                    b.push_back(bb);
                }
                {
                    BMesh::Point3D aa = v[i]->root->l[10].v[0];
                    BMesh::Point3D bb = v[i]->root->l[10].v[1];
                    BMesh::Lvector ll(aa, bb);
                    double len = ll.norm();
                    double splitLen = len / (double)splitNum;
                    BMesh::Point3D tmpA = ll.getNormalize() * splitLen + aa;
                    c.push_back(aa);
                    c.push_back(tmpA);
                    for (int t = 1; t < splitNum - 1; t++) {
                        BMesh::Point3D tmpAA = ll.getNormalize() * splitLen + c.back();
                        c.push_back(tmpAA);
                    }
                    c.push_back(bb);
                }
                {
                    BMesh::Point3D aa = v[i]->root->l[11].v[0];
                    BMesh::Point3D bb = v[i]->root->l[11].v[1];
                    BMesh::Lvector ll(aa, bb);
                    double len = ll.norm();
                    double splitLen = len / (double)splitNum;
                    BMesh::Point3D tmpA = ll.getNormalize() * splitLen + aa;
                    d.push_back(aa);
                    d.push_back(tmpA);
                    for (int t = 1; t < splitNum - 1; t++) {
                        BMesh::Point3D tmpAA = ll.getNormalize() * splitLen + d.back();
                        d.push_back(tmpAA);
                    }
                    d.push_back(bb);
                }
                {
                    BMesh::Face f1(a[0], b[0], c[0], d[0]);
                    BMesh::Face f2(a[1], b[1], c[1], d[1]);
                    BMesh::Point3D midPoint(0, 0, 0);
                    midPoint = f1.getMidFacePoint();
                    midPoint += f2.getMidFacePoint();
                    v.push_back(new BMesh::Node(midPoint / 2.0, BMesh::TN));
                    v[v.size() - 1]->root = new BMesh::Hex(f1, f2); // 因为是根据面的中心点构成的在下方会引入误差
                    v[v.size() - 1]->addChild(an);
                    an->deleteChild(v[i]);
                    an->addChild(v[v.size() - 1]);
                }
                for (int t = 1; t < a.size() - 2; t++) {
                    BMesh::Face f1(a[t], b[t], c[t], d[t]);
                    BMesh::Face f2(a[t + 1], b[t + 1], c[t + 1], d[t + 1]);
                    BMesh::Point3D midPoint(0, 0, 0);
                    midPoint = f1.getMidFacePoint();
                    midPoint += f2.getMidFacePoint();
                    v.push_back(new BMesh::Node(midPoint / 2.0, BMesh::TN));
                    v[v.size() - 1]->root = new BMesh::Hex(f1, f2); // 因为是根据面的中心点构成的在下方会引入误差
                    v[v.size() - 1]->addChild(v[v.size() - 2]);
                    v[v.size() - 2]->addChild(v[v.size() - 1]);
                }
                {
                    BMesh::Face f1(a[a.size() - 2], b[a.size() - 2], c[a.size() - 2], d[a.size() - 2]);
                    BMesh::Face f2(a[a.size() - 1], b[a.size() - 1], c[a.size() - 1], d[a.size() - 1]);
                    BMesh::Point3D midPoint(0, 0, 0);
                    midPoint = f1.getMidFacePoint();
                    midPoint += f2.getMidFacePoint();
                    v.push_back(new BMesh::Node(midPoint / 2.0, BMesh::TN));
                    v[v.size() - 1]->root = new BMesh::Hex(f1, f2); // 因为是根据面的中心点构成的在下方会引入误差
                    v[v.size() - 1]->addChild(bn);
                    v[v.size() - 1]->addChild(v[v.size() - 2]);
                    v[v.size() - 2]->addChild(v[v.size() - 1]);
                    bn->deleteChild(v[i]);
                    bn->addChild(v[v.size() - 1]);
                }
                v[i]->setNodeType(BMesh::DN);
            }

        }
    }
    for (auto it = v.begin(); it != v.end();) {
        if ((*it)->getNodeType() == BMesh::DN) {
            it = v.erase(it);
        }
        else {
            if ((*it)->getNodeType() == BMesh::TN) {
                (*it)->setNodeType(BMesh::JN);
            }
            it++;
        }
    }
}

void Solution::saveSkel(std::string name) {
    std::ofstream off = std::ofstream(name.c_str(), std::ios::out);
    if (!off.good()) {
        std::cout << "[Error] Could not open file " << name << " for writing!" << std::endl;
        off.close();
        return;
    }
    // Write header
    off << "ID Cx Cy Cz Length Width Height #NEIGHBORS NEIGHBORS_LIST" << std::endl;
    off << v.size() << std::endl;
    // renew id
    for (int i = 0; i < v.size(); i++) {
        v[i]->id = i;
    }
    for (int i = 0; i < v.size(); i++) {
        off << v[i]->id << " " << v[i]->getPosition().x << " " << v[i]->getPosition().y << " " << v[i]->getPosition().z
            << " " << v[i]->root->getLengthWidthHeight()[0] << " " << v[i]->root->getLengthWidthHeight()[1] << " " << v[i]->root->getLengthWidthHeight()[2] << " " << v[i]->children.size();
        for (int j = 0; j < v[i]->children.size(); j++) {
            off << " " << v[i]->children[j]->id;
        }
        off << std::endl;
    }
    off.close();
}

/**
 * @description: node have a lot of children, some is bad, so delete it
 * @param {*}
 * @return {*}
 */
bool Solution::deleteNodeTopology(Node *node) {
    std::vector<Point3D> diffPoint;
    std::vector<Point3D> samePoint;
    int ai = -1;
    int bi = -1;
    for (int i = 0; i < node->children.size();) {
        unsigned int rlt =
            Geometry::isAHexFaceContainBHexFace(node->root, node->children[i]->root, diffPoint, samePoint, ai, bi);
        if (rlt != 2) { // != 2
            node->children[i]->deleteChild(node);
            node->deleteChild(node->children[i]);
            i = 0;
        } else {
            i++;
        }
    }
    return true;
}

/**
 * @description: delete bad child
 * @param {*}
 * @return {*}
 */
void Solution::deleteNodeTraceAllNode() {
    for (int i = 0; i < v.size(); i++) {
        deleteNodeTopology(v[i]);
    }
}

/**
 * @description: from a node get a line
 * @param {*}
 * @return {*}
 */
bool Solution::getALine(Node* root, std::vector<Node*>& retNode) {
    retNode.clear();
    if (root == NULL) {
        return false;
    }
    visited.clear();
    std::queue<Node*> empty;
    std::swap(empty, Q);
    visited[root->id] = true;
    BFSFindALine(root, retNode);
    return true;
}


/**
 * @description: just to find a line
 * @param {type}
 * @return {type}
 */
void Solution::BFSFindALine(Node* root, std::vector<Node*>& retNode) {
    Q.push(root);
    while (!Q.empty()) {
        BMesh::Node* a = Q.front();
        visited[a->id] = true;
        retNode.push_back(a);
        Q.pop();
        for (int i = 0; i < a->children.size(); i++) {
            if (visited[a->children[i]->id] == false) {
                Q.push(a->children[i]);
                visited[a->children[i]->id] = true;
            }
        }
    }
}

/**
 * @description: just to find a line
 * @param {type}
 * @return {type}
 */
bool Solution::changeToPolylines() {
    std::map<Node*, int> toDfs;
    std::vector<std::pair<Node*, int>> backs;
    std::map<Node *, bool> visited;

    for (int i = 0; i < v.size(); i++) {
        if (v[i]->children.size() == 1 || v[i]->children.size() >= 3) {
            toDfs[v[i]] = v[i]->children.size();
        }
    }
    if (toDfs.size() == 0) {
        toDfs[v[0]] = 2;
    }
    struct TmpNode {
        Node* a;
        int count;
    };
    std::vector<TmpNode> toDfs2;
    for (auto it : toDfs) {
        toDfs2.push_back({ it.first, it.second });
    }
    sort(toDfs2.begin(), toDfs2.end(), [=](const auto a, const auto b) {
        return a.count < b.count;
        });
    std::vector<std::vector<Node*>> out;
    for (auto it = toDfs2.begin(); it != toDfs2.end(); it++) {
        
        while (toDfs[(*it).a] > 0) {
            std::vector<Node*> path;
            //toDfs[(*it).a]--;
            //visited[(*it).a] = true;
            //for(int i=0; i)
            //path.push_back((*it).a);
            dfs((*it).a, visited, toDfs, path);
            out.push_back(path);
        }
    }
    std::string filename1 = "skel-poly.polylines.txt";
    std::ofstream off = std::ofstream(filename1.c_str(), std::ios::out);
    for (int i = 0; i < out.size(); i++) {
        off << out[i].size();
        for (int j = 0; j < out[i].size(); j++) {
            off << " " << out[i][j]->getPosition().x << " " << out[i][j]->getPosition().y << " " << out[i][j]->getPosition().z;
        }
        off << "\n";
    }
    off.close();
    return true;
}

void Solution::dfs(Node * a, std::map<Node*, bool>& visited, std::map<Node*, int> & toDfs, std::vector<Node*> & path) {
    if (!visited[a]) {
        
        path.push_back(a);
        toDfs[a]--;
        if (toDfs[a] <= 0) {
            visited[a] = true;
        }
        if (path.size() != 1 && a->children.size() >= 3) {
            return;
        }
        for (int i = 0; i < a->children.size(); i++) {
            if ( !visited[a->children[i]] ) {
                if (path.size() == 2) {
                    if (path[0] == a->children[i]) {
                        continue;
                    }
                    dfs(a->children[i], visited, toDfs, path);
                    break;
                }
                else {
                    dfs(a->children[i], visited, toDfs, path);
                    break;
                }

            }
        }
    }
}


bool Solution::intreP() {
    InterpolateCurve a;
    a.readOff(triOffPath); // read off
    a.genPoints("interPCurveSkel.json");
    a.simpleSkel();
    a.exportSkel();
    curvePointss = a.curvePointss;
    // TEST
    this->clearSkel();
    this->readSkel("result.skel");
    // if two node too close, then join them
    std::vector<std::pair<Node *, Node *>> simple; // 1 - n , then make n to be (1 + n) / 2, then delete 1
    std::vector<std::pair<Node*, Node*>> complex; // n - n, then make n to be (n + n) /2 ,then delete n then connect n's children
    bool check = false;
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i]->children.size(); j++) {
            if (BMesh::Lvector(v[i]->position, v[i]->children[j]->position).norm() <
                    (
                        BMesh::Lvector(v[i]->root->p[0], v[i]->position).norm() + 
                        BMesh::Lvector(v[i]->children[j]->root->p[0], v[i]->children[j]->position).norm()
                    )
                ) {
                check = true;
                JoinTwoNode(v[i], v[i]->children[j]);
                i = -1;
                break;
            }
        }
    }
    if (check) {
        return false;
    }
    return true;
}

void  Solution::JoinTwoNode(Node* a, Node* b) {
    if (a->children.size() == 1) {
        BMesh::Lvector s(b->getPosition(), a->getPosition());
        b->position = (a->getPosition() + b->position) / 2.0;
        editNode(b, b->position, b->getLengthWidthHeight());
        delNode(a);
    }
    else if (b->children.size() == 1) {
        BMesh::Lvector s(b->getPosition(), a->getPosition());
        a->position = (a->getPosition() + b->position) / 2.0;
        editNode(a, a->position, a->getLengthWidthHeight());
        delNode(b);
    }
    else {
        b->position = (a->getPosition() + b->position) / 2.0;
        for (int i = 0; i < a->children.size(); i++) {
            if (a->children[i] == b) {
                //connectNodes(Node * a, Node * b);
                disconnectNodes(a, b);
                continue;
            }
            else {
                connectNodes(a->children[i], b);
                disconnectNodes(a, a->children[i]);
            }
        }
        editNode(b, b->position, b->getLengthWidthHeight());
        delNode(a);
    }
    
}

void Solution::poly2json() {
    std::string filename1 = "skel-poly.polylines.txt";
    std::ifstream iff = std::ifstream(filename1.c_str(), std::ios::in);
    if (!iff.good()) {
        std::cout << "[ERROR] could not open file " << filename1 << " for reading!" << std::endl;
        iff.close();
        return;
    }
    std::vector<std::vector<BMesh::Point3D>> skel;

    std::string lineStr;
    while (BMesh::common::getStrLine(iff, lineStr)) {
        std::stringstream sstr;
        sstr << lineStr;
        int number = 0;
        sstr >> number;
        std::vector<BMesh::Point3D> vs;
        for (int i = 0; i < number; i++) {
            double a, b, c;
            sstr >> a;
            sstr >> b;
            sstr >> c;
            BMesh::Point3D p(a, b, c);
            vs.push_back(p);
        }
        skel.push_back(vs);
    }
    iff.close();
    /*分段输出json文件*/
    exportJsonForCurveP(skel);
}


void Solution::exportJsonForCurveP(std::vector<std::vector<BMesh::Point3D>>& skel) {
    // 冗余骨架的合并  --   还是先改成交互式删除.
    /*std::map<BMesh::Point3D, int> m;
    for (auto &it : skel) {
        for (auto& iit : it) {
            m[iit] += 1;
        }
    }
    std::vector<BMesh::Point3D> v;
    for (auto it : m) {
        if (it.second == 2) {
            v.push_back(it.first);
        }
    }*/
    std::ofstream out("interPCurveSkel.json");
    out << "{\n";
    out << "\"name\": \"interpolateCurves\",\n";
    out << "\"lines\": [ \n";

    for (int i = 0; i < skel.size(); i++) {
        if (skel[i].size() < 2) {
            std::cout << "[ERROR] : file skel line points less than 2\n";
            continue;
        }
        else if (skel[i].size() == 2) {
            skel[i].push_back(skel[i][0]);
            skel[i].push_back(skel[i][1]);
        }
        else if (skel[i].size() == 3) {
            skel[i].push_back(skel[i][0]);
        }
        if (i == 0) {
            out << "{\n";
        }
        else {
            out << ",{\n";
        }
        out << "\"curvePointNum\": 100, \n";
        out << "\"p\": 3,\n";
        out << "\"points\": [\n";
        for (int j = 0; j < skel[i].size(); j++) {
            if (j == 0) {
                out << "[" << skel[i][j].x << "," << skel[i][j].y << "," << skel[i][j].z << "]";
            }
            else {
                out << ",[" << skel[i][j].x << "," << skel[i][j].y << "," << skel[i][j].z << "]\n";
            }
        }
        out << "]\n";
        out << "}\n";
    }
    out << "]}\n";
    out.close();
}