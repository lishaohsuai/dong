/*
 * @Author: your name
 * @Date: 2020-10-05 20:58:37
 * @LastEditTime: 2021-01-29 23:02:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /my_skel/header/line.hpp
 */
#ifndef __RAYS_HPP__
#define __RAYS_HPP__
#include "local_vector.hpp"
#include <algorithm>
#include <vector>
namespace BMesh {
    class Rays {
    public:
        Rays(Point3D a, Point3D b); 
        ~Rays();
        Rays();
        Rays(Rays& rhs);
        Rays(const Rays& rhs);

    public:
        Rays& operator=(const Rays& rhs);
        Rays& operator=(Rays& rhs);
        bool operator==(Rays& rhs);
        bool operator!=(Rays& rhs);
        bool isPointInRays(const Point3D& p);

    public:
        Lvector l;              // line direction vec
        std::vector<Point3D> v; // point in line
    };
} // namespace BMesh
#endif