#pragma once
#include <iostream>
#include <string>
#include "GeometryKernel.h"
#include <vector>
#include "triangle.hpp"
#include "geometry.hpp"
#include <limits>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#ifdef _DEBUG
#pragma comment(lib, "OpenMeshCored.lib")//这种链接库的方式似乎更好
#pragma comment(lib, "OpenMeshToolsd.lib")
#else
#pragma comment(lib, "OpenMeshCore.lib")
#pragma comment(lib, "OpenMeshTools.lib")
#endif

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;
class Project{
public:
	std::string triFile;
	std::vector<BMesh::Triangle> tv;
	std::vector<BMesh::Lvector> triangleVector;
	std::vector<BMesh::Point3D> points;
	HexV3fMesh *hMesh;
	BMesh::Point3D center;
	double radius;
	BMesh::Point3D boxMinPoint;
	BMesh::Point3D boxMaxPoint;
	std::vector<std::pair<V3f, V3f>> projectLines;
	std::vector<std::pair<V3f, V3f>> projectLines2;
	MyMesh triMesh;
	std::map<int, int> m;// 将在四边形边界的顶点id映射为新的三角网格的id
	std::map<int, int> mm; // 将三角网格的id映射为四边形网格的id

	//Project(std::string triFile_, HexV3fMesh* hMesh_) {
	//	triFile = triFile_;
	//	hMesh = hMesh_;
	//	readTriMesh(triFile);
	//}
public:
	void readTriMesh(std::string triFile);
	void projectVolumeToOff(); 
	void projectVolumeToOff_sec();
	void projectVolumeToOffFindClosePoint(); // not to do really
	void dense();// 三角网格变密
	void movePoint();
	bool surfaceQuad2Tri();
	

public:
	// 绘图相关
	void calCenter();
};