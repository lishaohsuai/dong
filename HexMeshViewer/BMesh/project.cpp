#include "project.h"
#include "point.hpp"
#include <sstream>
#include "common.hpp"
#include "fixFlip.hpp"
void Project::readTriMesh(std::string triFile) {
	std::ifstream iff = std::ifstream(triFile.c_str(), std::ios::in);
	if (!iff.good())
	{
		std::cout << "[ERROR] cound not open file " << triFile << " for reading!" << std::endl;
		iff.close();
		return;
	}
	std::string line;
	getline(iff, line);
	std::string los("OFF");
	if (line != los) {
		std::cout << "[ERROR] format not supported " << std::endl;
		std::cout << "los " << los << std::endl;
		std::cout << "line " << line << std::endl;
		return;
	}
	getline(iff, line);
	std::stringstream sstr;
	sstr.str(line);
	int pointNum;
	int faceNum;
	sstr >> pointNum;
	sstr >> faceNum;
	sstr.clear();
	points.clear();
	for (int i = 0; i < pointNum; i++) {
		getline(iff, line);
		sstr.str(line);
		double b1, b2, b3;
		sstr >> b1;
		sstr >> b2;
		sstr >> b3;
		points.push_back(BMesh::Point3D( b1, b2, b3 ));
		sstr.clear();
	}
	int tmp;
	int pv1;
	int pv2;
	int pv3;
	sstr.clear();
	for (int i = 0; i < faceNum; i++) {
		getline(iff, line);
		sstr.str(line);
		sstr >> tmp;
		if (tmp != 3) {
			std::cout << "[ERROR] format not supported_" << std::endl;
			return;
		}
		sstr >> pv1;
		sstr >> pv2;
		sstr >> pv3;
		BMesh::Lvector a(points[pv1], points[pv2]);
		BMesh::Lvector b(points[pv1], points[pv3]);
		BMesh::Lvector c = (a % b).normalize();
		BMesh::Triangle t(points[pv1], points[pv2], points[pv3]);
		tv.push_back(t);
		triangleVector.push_back(c);
		sstr.clear();
	}
	iff.close();
}


/**
 * @description: 投影法, 还没有删除, 有多个投影区域的错误
 * @param {set<int> &s1, std::set<int>} &s2
 * @param {int} a1
 * @param {int} a2
 * @return 是否修复了, 或者除了一种情况两个三角形都是true的情况
 */
void Project::projectVolumeToOff() {
	hMesh->frameface.clear();
	hMesh->frame.clear();
	hMesh->choseframe(); // 边界边
	hMesh->choseframeface(); // 边界面
	std::map<V3f, int> v; // 所有的边界面的顶点
	uint64_t n_vertices = 1;
	for (auto it : hMesh->frameface) {
		for (auto& iit : it) {
			if (v[iit] == 0) {
				v[iit] = n_vertices;
				n_vertices++;
			}
		}
	}
	// 将边界cell的点投影到外面的面上分成类讨论
	// 1. 如果一个cell所有的面都是边界面, 那么我们就以cell的中心然后开始投影这个cell的所有顶点
	for (int i = 0; i < hMesh->cells_.size(); i++) {
		std::vector<bool> cellCheck;
		int trueNum = 0;
		for (int j = 0; j < 6; j++) {
			if (hMesh->is_bdy(hMesh->cells_[i].faces_[j])) {
				cellCheck.push_back(true);
				trueNum++;
			}
			else {
				cellCheck.push_back(false);
			}
		}
		if (trueNum == 5) {
			// 端点结构需要将四个顶点投影到外围
			int faceConnectNum = -1;
			for (int k = 0; k < cellCheck.size(); k++) {
				if (cellCheck[k] == false) {
					faceConnectNum = k;
					break;
				}
			}
			// 首先将连接的那个面的中心点得到
			V3f face_mid;
			face_mid = (hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[0]]
				+ hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[1]]
				+ hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[2]]
				+ hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[3]]) * 1.0 / 4 * 1.0;
			// 面的第一个控制
			V3f tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[0]];
			BMesh::Rays l(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					std::cout << "相交了\n";
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[0]] = temp;
					break;
				}
			}
			// 面的第二个控制
			tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[1]];
			l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[1]] = temp;
					break;
				}
			}
			// 面的第三个控制
			tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[2]];
			l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[2]] = temp;
					break;
				}
			}
			// 面的第四个控制
			tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[3]];
			l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[3]] = temp;
					break;
				}
			}
			// 插入单元的内心
			V3f cell_mid;
			for (int j = 0; j < 8; j++) {
				cell_mid += hMesh->vertices_[hMesh->cells_[i].vertex_[j]];
			}
			cell_mid = cell_mid * 1.0 / 8 * 1.0;
			int faceOppoNum = -1;
			if (faceConnectNum % 2 == 0) {
				faceOppoNum = faceConnectNum + 1;
			}
			else {
				faceOppoNum = faceConnectNum - 1;
			}
			// 面的对面的第一个控制
			tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[0]];
			l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[0]] = temp;
					break;
				}
			}
			// 面的第二个控制
			tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[1]];
			l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[1]] = temp;
					break;
				}
			}
			// 面的第三个控制
			tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[2]];
			l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[2]] = temp;
					break;
				}
			}
			// 面的第四个控制
			tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[3]];
			l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
			for (int index = 0; index < tv.size(); index++) {
				BMesh::Point3D p;
				if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
					V3f temp(p.x, p.y, p.z);
					hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[3]] = temp;
					break;
				}
			}
		}
		if (trueNum == 4) {
			// 分成相对的还是相邻的判断, 这两个有连接面
			int faceConnectNum1 = -1;
			int faceConnectNum2 = -1;
			int k;
			for (k = 0; k < cellCheck.size(); k++) {
				if (cellCheck[k] == false) {
					faceConnectNum1 = k;
					k++;
					break;
				}
			}
			for (; k < cellCheck.size(); k++) {
				if (cellCheck[k] == false) {
					faceConnectNum2 = k;
					break;
				}
			}
			std::map<int, int> m;
			m[0] = 1;
			m[2] = 3;
			m[4] = 5; // 一个面的向对面
			if (m[faceConnectNum1] == faceConnectNum2) {
				// 进入相对面的模式判断
				int faceConnectNum = faceConnectNum1;
				// 首先将连接的那个面的中心点得到
				V3f face_mid;
				face_mid = (hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[0]]
					+ hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[1]]
					+ hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[2]]
					+ hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[3]]) * 1.0 / 4 * 1.0;
				// 面的第一个控制
				V3f tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[0]];
				BMesh::Rays l(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						std::cout << "相交了\n";
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[0]] = temp;
						break;
					}
				}
				// 面的第二个控制
				tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[1]];
				l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[1]] = temp;
						break;
					}
				}
				// 面的第三个控制
				tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[2]];
				l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[2]] = temp;
						break;
					}
				}
				// 面的第四个控制
				tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[3]];
				l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceConnectNum]].vs_f[3]] = temp;
						break;
					}
				}
				// 插入单元的内心
				V3f cell_mid;
				for (int j = 0; j < 8; j++) {
					cell_mid += hMesh->vertices_[hMesh->cells_[i].vertex_[j]];
				}
				cell_mid = cell_mid * 1.0 / 8 * 1.0;
				int faceOppoNum = -1;
				if (faceConnectNum % 2 == 0) {
					faceOppoNum = faceConnectNum + 1;
				}
				else {
					faceOppoNum = faceConnectNum - 1;
				}
				// 面的对面的第一个控制
				tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[0]];
				l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[0]] = temp;
						break;
					}
				}
				// 面的第二个控制
				tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[1]];
				l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[1]] = temp;
						break;
					}
				}
				// 面的第三个控制
				tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[2]];
				l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[2]] = temp;
						break;
					}
				}
				// 面的第四个控制
				tmp = hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[3]];
				l = BMesh::Rays(BMesh::Point3D(face_mid.x, face_mid.y, face_mid.z), BMesh::Point3D(tmp.x, tmp.y, tmp.z));
				for (int index = 0; index < tv.size(); index++) {
					BMesh::Point3D p;
					if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
						V3f temp(p.x, p.y, p.z);
						hMesh->vertices_[hMesh->faces_[hMesh->cells_[i].faces_[faceOppoNum]].vs_f[3]] = temp;
						break;
					}
				}
			}
		}
	}
}

/**
 * @description: 三角网格变密集
 * @param {set<int> &s1, std::set<int>} &s2
 * @param {int} a1
 * @param {int} a2
 * @return 是否修复了, 或者除了一种情况两个三角形都是true的情况
 */
void Project::dense() {
	std::vector<BMesh::Triangle> rlt;
	std::vector<BMesh::Triangle> tmp;
	for (int i = 0; i < tv.size(); i++) {
		tmp = tv[i].oneToThree();
		for (auto it : tmp) {
			rlt.push_back(it);
		}
		points.push_back(tv[i].getMidFacePoint());
	}
	tv.clear();
	tv.assign(rlt.begin(), rlt.end());
}

/**
 * @description: 投影体网格点到最近点
 * @param {set<int> &s1, std::set<int>} &s2
 * @param {int} a1
 * @param {int} a2
 * @return 是否修复了, 或者除了一种情况两个三角形都是true的情况
 */
void Project::projectVolumeToOffFindClosePoint() {
	dense();
	dense();
	dense();
	for (int i = 0; i < hMesh->vertices_.size(); i++) {
		if (hMesh->is_bdy(VertexHandle(i))) {
			V3f tmp = hMesh->vertices_[i];
			BMesh::Point3D temp(tmp.x, tmp.y, tmp.z);
			double minDis = INT_MAX;
			BMesh::Point3D minPoint;
			for (auto it : points) {
				double itd = BMesh::common::distance(it, temp);
				if (minDis > itd) {
					minDis = itd;
					minPoint = it;
				}
			}
			V3f tmp1(minPoint.x, minPoint.y, minPoint.z);
			projectLines.push_back(std::make_pair(hMesh->vertices_[i], tmp1));
		}
	}
}


void Project::movePoint() {
	int index = 0;
	// for TEST 
	for (int i = 0; i < hMesh->vertices_.size(); i++) {
		if (hMesh->is_bdy(VertexHandle(i))) {
			hMesh->vertices_[i] = projectLines[index++].second;
		}
	}
	
}




void Project::calCenter() {
	radius = 0;
	center = BMesh::Point3D{ 0, 0, 0 };
	boxMaxPoint = BMesh::Point3D( std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min());
	boxMinPoint = BMesh::Point3D( std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
	for (auto it : points) {
		center += it;
	}
	center /= points.size();
	for (auto it : points) {
		if (radius < BMesh::Geometry::distanceTwoPoint(it, center)) {
			radius = BMesh::Geometry::distanceTwoPoint(it, center);
		}
		if (boxMaxPoint.x < it.x) boxMaxPoint.x = it.x;
		if (boxMaxPoint.y < it.y) boxMaxPoint.y = it.y;
		if (boxMaxPoint.z < it.z) boxMaxPoint.z = it.z;

		if (boxMinPoint.x > it.x) boxMinPoint.x = it.x;
		if (boxMinPoint.y > it.y) boxMinPoint.y = it.y;
		if (boxMinPoint.z > it.z) boxMinPoint.z = it.z;
	}
	
}


bool Project::surfaceQuad2Tri() {
	std::ofstream out("tempFile_surfaceQuad2Tri.off");
	out << "OFF\n";

	int vertexFaceBdyNum = 0;

	for (int i = 0; i < hMesh->vertices_.size(); i++) {
		if (hMesh->is_bdy(VertexHandle(i))) {
			m[vertexFaceBdyNum] = i;
			mm[i] = vertexFaceBdyNum;
			vertexFaceBdyNum++;
		}
	}
	int faceBdyNum = 2 * hMesh->boundary_face.size(); 
	out << vertexFaceBdyNum << " " << faceBdyNum << " 0\n";
	for (int i = 0; i < hMesh->vertices_.size(); i++) {
		if (hMesh->is_bdy(VertexHandle(i))) {
			out << hMesh->vertices_[i].x << " " << hMesh->vertices_[i].y << " " << hMesh->vertices_[i].z << std::endl;
		}
	}
	for (int i = 0; i < hMesh->boundary_face_VID.size(); i++) {
		out << "3 " << mm[hMesh->boundary_face_VID[i][0]] << " " << mm[hMesh->boundary_face_VID[i][1]] << " " << mm[hMesh->boundary_face_VID[i][2]] << std::endl;
		out << "3 " << mm[hMesh->boundary_face_VID[i][0]] << " " << mm[hMesh->boundary_face_VID[i][2]] << " " << mm[hMesh->boundary_face_VID[i][3]] << std::endl;
	}
	out.close();

	FixFlip f;
	string path = "tempFile_surfaceQuad2Tri.off";
	f.run(path, path.substr(0, path.find(".off")) + "_fix.off");
	
	// read mesh to output.obj
	try
	{
		if (!OpenMesh::IO::read_mesh(triMesh, "tempFile_surfaceQuad2Tri_fix.off"))
		{
			std::cerr << "Cannot write mesh to file " << "tempFile_surfaceQuad2Tri_fix.off" << std::endl;
		}
		else {
			std::cout << "[DEBUG] read mesh success, lucky Guy\n";
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
	}


	return true;
}


void Project::projectVolumeToOff_sec() {
	surfaceQuad2Tri();
	//auto it = triMesh.vertex_normals();
	triMesh.request_face_normals();
	triMesh.request_vertex_normals();
	triMesh.update_normals();
	//projectLines.resize(triMesh.n_vertices());

	for (MyMesh::VertexIter v_it = triMesh.vertices_begin(); v_it != triMesh.vertices_end(); ++v_it) {
		auto it = triMesh.calc_vertex_normal(*v_it);// 得到每个顶点的法向量
		auto point = triMesh.point(*v_it);
		auto point1 = point + it * 0.1;// 顶点加上法向量生成新顶点
		// TODO:先不考虑异常情况射线射到本身上面
		BMesh::Rays l(BMesh::Point3D(point.data()[0], point.data()[1], point.data()[2]), BMesh::Point3D(point1.data()[0], point1.data()[1], point1.data()[2]));
		BMesh::Point3D p;
		std::vector<V3f> ss;
		
		int maxDis = INT_MAX;
		BMesh::Point3D rlt;
// #pragma omp parallel for num_threads(8)
		bool get = false;
		for (int index = 0; index < tv.size(); index++) {
			if (BMesh::Geometry::intersectionRays3DTriangle(l, tv[index], p)) {
				//std::cout << "相交了\n";
				V3f temp(p.x, p.y, p.z);
				// 寻找最近点
				if (l.l * tv[index].n < 0) {
					continue;
				}
				double distance = BMesh::Geometry::distanceTwoPoint(BMesh::Point3D(point.data()[0], point.data()[1], point.data()[2]), p);
				if (maxDis > distance) {
					maxDis = distance;
					rlt = p;
					get = true;
				}
			}
		}
		if (!get) {
			BMesh::Line ls(l.v[0], l.v[1]);
			for (int index = 0; index < tv.size(); index++) {
				if (BMesh::Geometry::intersectionLine3DTriangle(ls, tv[index], p)) {
					//std::cout << "相交了\n";
					V3f temp(p.x, p.y, p.z);
					// 寻找最近点
					double distance = BMesh::Geometry::distanceTwoPoint(BMesh::Point3D(point.data()[0], point.data()[1], point.data()[2]), p);
					if (maxDis > distance) {
						maxDis = distance;
						rlt = p;
						get = true;
					}
				}
			}
		}
		int a = m[v_it->idx()];
		projectLines.push_back(std::make_pair(hMesh->vertices_[a], V3f(rlt.x, rlt.y, rlt.z)));
		//projectLines.push_back(std::make_pair(hMesh->vertices_[a], V3f(point1.data()[0], point1.data()[1], point1.data()[2])));
		//hMesh->vertices_[a] = V3f(rlt.x, rlt.y, rlt.z);
	}
}