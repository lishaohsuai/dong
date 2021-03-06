/*
 * @Author: your name
 * @Date: 2020-10-05 20:19:46
 * @LastEditTime: 2020-12-02 09:31:12
 * @LastEditors: Please set LastEditors
 * @Description: 桩类 进行代码测试
 * @FilePath: /my_skel/header/test.hpp
 */
#ifndef __TEST_HPP__
#define __TEST_HPP__
#include "bmesh_solution.hpp"
#include "common.hpp"
#include "geometry.hpp"
#include <iostream>
#include <unordered_map>
namespace BMesh {
namespace Unitest_common {
void main_common();
void main_genNodeHex(std::string filename);
void main_point();
void main_splitFace();
void main_intersection();
void main_test();
void main_testNode();
void main_testFace();
void main_testFaceContain();
void main_hexFaceSplit();
void main_interlaceFace();
void main_interlaceHex();
void main_printf();
BMesh::Point3D sub_point();
BMesh::Point3D sub1_point();
} // namespace Unitest_common
} // namespace BMesh
#endif