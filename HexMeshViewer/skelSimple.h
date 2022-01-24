#pragma once

#include "GeometryKernel.h"
#include "bmesh/geometry.hpp"
class OurSkelNode {
public:
	OurSkelNode() {};
	OurSkelNode(int _index, V3f _pos, double _radius, bool _isvalid) :index(_index), pos(_pos), radius(_radius), is_valid(_isvalid) {};
public:
	int index;
	V3f pos;
	double radius;// 空间位置和节点半径
	bool is_valid; // true代表存在，false代表删除，删除代表Indegree为0
	int Indegree;// 有几个邻居节点
	std::vector<int> neighbor;
	int newIdx = -1;
	double angle = 0.0; // 每一个度为2的顶点的夹角
};

class OurSkel {
public:
	//包含点、线，点的邻接关系
	std::vector<OurSkelNode> SkelAll;
	// 绘图用，缩放模型
	V3f center;
	std::string filename;
	double r;// 空间位置和节点半径
public:
	OurSkel() {};
	OurSkel& operator=(const OurSkel& skel) {
		this->SkelAll.assign(skel.SkelAll.begin(), skel.SkelAll.end());
		this->center = skel.center;
		this->r = skel.r;
		return *this;
	}
	void CalBoundingBox() {
		//包围盒的两个顶点
		//V3f pmin(DBL_MAX, DBL_MAX, DBL_MAX), pmax(DBL_MIN, DBL_MIN, DBL_MIN);
		double average_r = 0.0;
		double cen_x = 0, cen_y = 0, cen_z = 0;
		double max = 0;
		double max_x = -9999, max_y = -9999, max_z = -9999;
		double min_x = 9999, min_y = 9999, min_z = 9999;
		int num = SkelAll.size();
		for (auto it = SkelAll.begin(); it != SkelAll.end(); it++)
		{
			cen_x += (*it).pos.x;
			cen_y += (*it).pos.y;
			cen_z += (*it).pos.z;
			num++;
		}
		center.x = cen_x / num;
		center.y = cen_y / num;
		center.z = cen_z / num;
		for (auto it = SkelAll.begin(); it != SkelAll.end(); it++)
		{
			//double maxr = V3f((*it).x - center.x, (*it).y - center.y, (*it).z - center.z).norm();
			double maxr = ((*it).pos - center).norm();
			average_r += maxr;
			if (maxr > max)
				max = maxr;
			if ((*it).pos.x > max_x) max_x = (*it).pos.x;
			if ((*it).pos.x < min_x) min_x = (*it).pos.x;
			if ((*it).pos.y > max_y) max_y = (*it).pos.y;
			if ((*it).pos.y < min_y) min_y = (*it).pos.y;
			if ((*it).pos.z > max_z) max_z = (*it).pos.z;
			if ((*it).pos.z < min_z) min_z = (*it).pos.z;
		}
		average_r /= num;
		r = max;
		V3f bbMax = V3f(max_x, max_y, max_z);
		V3f bbMin = V3f(min_x, min_y, min_z);
	}

	/**
	 * @description: only can delete degree  equal to 2
	 *               return 1 success
	 *						0 failure
	 * @param {int} NodeIdx
	 * @return {*}
	 */
	int DeleteNode_Skel(int NodeIdx) {
		if (SkelAll[NodeIdx].Indegree != 2) {
			return 0;
		}
		else {
			int former, later;
			former = SkelAll[NodeIdx].neighbor[0];
			later = SkelAll[NodeIdx].neighbor[1];
			// test nodeIdx distance to line(former later) is smaller then nodeIdex's (radius / 0.75 * 0.15) 
			
			V3f formerToLater = SkelAll[later].pos - SkelAll[former].pos;
			V3f formerTonodeIdx = SkelAll[NodeIdx].pos - SkelAll[former].pos;
			BMesh::Lvector a(formerToLater.x, formerToLater.y, formerToLater.z);
			BMesh::Lvector b(formerTonodeIdx.x, formerTonodeIdx.y, formerTonodeIdx.z);
			double angle = BMesh::Geometry::angleTwoVector(a, b);
			double disIdxToLine = formerTonodeIdx.norm() * sin(angle / 180.0);
			if (disIdxToLine > (SkelAll[NodeIdx].radius / 0.75 * 0.20)) {
				return 0;
			}
			
			for (int i = 0; i < SkelAll[former].neighbor.size(); i++) {
				if (SkelAll[former].neighbor[i] == NodeIdx) {
					SkelAll[former].neighbor[i] = later;
				}
			}
			for (int i = 0; i < SkelAll[later].neighbor.size(); i++) {
				if (SkelAll[later].neighbor[i] == NodeIdx) {
					SkelAll[later].neighbor[i] = former;
				}
			}
			SkelAll[NodeIdx].is_valid = false;
			return 1;
		}
	}

	/**
	 * @description: change radius
	 *               return 1 success
	 *						0 failure
	 * @param {int} NodeIdx
	 * @return {*}
	 */
	int changeRadius(int NodeIdx) {
		for (int i = 0; i < SkelAll[NodeIdx].neighbor.size(); i++) {
			double dis = (SkelAll[NodeIdx].pos - SkelAll[SkelAll[NodeIdx].neighbor[i]].pos).norm();
			if (dis < ((SkelAll[NodeIdx].radius + SkelAll[SkelAll[NodeIdx].neighbor[i]].radius) * 2)) {
				SkelAll[NodeIdx].radius = dis * 0.25;
				SkelAll[SkelAll[NodeIdx].neighbor[i]].radius = dis * 0.25;
			}
		}
		return 1;
	}
	//计算所有度为2的点的角度
	void CalAllAngle() {
		for (int i = 0; i < SkelAll.size(); i++) {
			if (SkelAll[i].neighbor.size() == 2 && SkelAll[i].is_valid) {
				int former, later; 
				former = SkelAll[i].neighbor[0];
				while (!SkelAll[former].is_valid) {
					former = SkelAll[former].neighbor[0];
				}
				later = SkelAll[i].neighbor[1];
				while (!SkelAll[later].is_valid) {
					later = SkelAll[later].neighbor[1];
				}
				
				auto lvec = SkelAll[former].pos - SkelAll[i].pos;
				auto rvec = SkelAll[later].pos - SkelAll[i].pos;
				double angle = (180 * acos((lvec * rvec) / (lvec.norm() * rvec.norm()))) / (3.141592658);
				//cos120 = -0.5
				//double angle = 1;
				SkelAll[i].angle = angle;
			}
		}
	}
	void OPDelete() {
		// 从最大的地方开始删除
		// 现在想5度5度做分解来删
		// 先del 175 170 165 160 155 150 145 140 135 130 125 120
		// 最好先写一个功能，把每个点的角度事先存储起来，并且能够更新
		int level = 1; // 表示删除的区间范围
		// 例如我要160度以上的不要，那我就175 170 165 160做这几次的阈值， 180 - 5 * level
		static int levelTop = 3;
		
		while (level < levelTop) {
			CalAllAngle();
			for (int i = 0; i < SkelAll.size(); i++) {
				if (SkelAll[i].is_valid && SkelAll[i].angle >= 180 - 5 * level) {
					DeleteNode_Skel(i);
				}
			}
			level++;
		}
		// 新增操作, 如果两个结点有重叠, 那么将半径设定为距离的一半的0.75倍
		for (int i = 0; i < SkelAll.size(); i++) {
			if (SkelAll[i].is_valid) {
				changeRadius(i);
			}
		}
		levelTop += 3;
	}

	void writeSimpleSkel() {
		OurSkel newskel;
		int tIndex = 0;
		std::vector<int> vv;
		std::map<int, int> vm;
		for (int i = 0; i < SkelAll.size(); i++) {
			if (SkelAll[i].is_valid) {
				vm[i] = tIndex;
				tIndex++;
				vv.push_back(i);
			}
		}
		std::ofstream output2(filename);
		output2 << "ID Cx Cy Cz RADIUS #NEIGHBORS NEIGHBORS_LIST\n";
		output2 << tIndex << "\n";
		for (int i = 0; i < tIndex; ++i) {
			std::vector<int> nn;
			for (auto it : SkelAll[vv[i]].neighbor) {
				if (SkelAll[it].is_valid) {
					nn.push_back(vm[it]);
				}
			}
			output2 << i << " " << SkelAll[vv[i]].pos.x << " " << SkelAll[vv[i]].pos.y << " " << SkelAll[vv[i]].pos.z << " "
				<< SkelAll[vv[i]].radius << " " << nn.size();
			for (auto iter : nn) {
				output2 << " " << iter;
			}
			output2 << "\n";
		}
		output2.close();
	}

};