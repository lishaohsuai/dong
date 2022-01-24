#pragma once
#include "GeometryKernel.h"
#include <iostream>
#include<unordered_map>

class PIA_FOR_CC_SOLID {
public:
	HexV3fMesh *mesh;
	HexV3fMesh *firstmesh;
	HexV3fMesh *oldmesh;
	double err_now;
	PIA_FOR_CC_SOLID(HexV3fMesh *_firstmesh,HexV3fMesh *_mesh) :firstmesh(_firstmesh),mesh(_mesh),err_now(0.0) {};
public:
	void PIA_FOR_CC_SOLID_ONCE();
	void PIA_FOR_HIGH_ORDERED_MESH();
};

class PartPIA {
public:
	HexV3fMesh* mesh;                                       // ����������
	std::vector<VertexHandle> MovablePart;                  // ���ƶ��Ĳ��֣�
	std::vector<V3f> TargetPoint;                           // ��Ҫ��ֵ���ĵ�
	std::vector<EdgeHandle> SharpEdge;                      // �����
	double CurrentError;                                    // ��ǰ���
	PartPIA(HexV3fMesh* _mesh, std::vector<V3f>& _TargetPoint, std::vector<EdgeHandle>& _SharpEdge) ;
	void GetTargetPoint(std::vector<V3f>& _TargetPoint);
	void PartUpDate();
	void GetMovablePart();
};


class SurfacePIA {
public:
	HexV3fMesh* mesh;                                       // ����������
	std::unordered_map<int, int>FixedPart;                  // �̶��Ĳ��֣�
	std::vector<VertexHandle> MovablePart;                  // ���ƶ��ĵ�
	std::vector<V3f> TargetPoint;                           // ��Ҫ��ֵ���ĵ�
	std::vector<EdgeHandle> SharpEdge;                      // �����
	std::unordered_map<int, int> SharpPoints;               // ������ϵĵ㣬ʹ�ù�ϣӳ����ڲ���
	double CurrentError;                                    // ��ǰ���
	SurfacePIA(HexV3fMesh* _mesh, std::vector<V3f>& _TargetPoint, std::vector<EdgeHandle>& _FixedEdge);
	void GetFixedPart(std::vector<EdgeHandle>& _FixedEdge);
	void GetSharpEdge(std::vector<EdgeHandle>& _SharpEdge);
	void GetSharpPoints();
	void GetMovablePart();
	void GetTargetPoint(std::vector<V3f>& _TargetPoint);
	void SurfaceUpDate();
};



