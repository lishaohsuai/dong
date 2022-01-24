#pragma once

#include "ComHeader.h"
#include "Subdivision.h"
class newV3f
{
public:
	newV3f(V3f _v, VertexHandle _vid) :v(_v), vid(_vid) {};
	inline bool operator<(const newV3f& _v3f) const
	{
		return vid < _v3f.vid;
	}
	V3f v;
	VertexHandle vid;
	double error;
};

class Sort
{
public:
	Sort(HexV3fMesh *_mesh, HexV3fMesh *_mesh1) :mesh(_mesh), mesh1(_mesh1) {};
	Sort(HexV3fMesh *_mesh, HexV3fMesh *_mesh1, std::vector<double> _m_color1, std::vector<double> _m_color2) 
		:mesh(_mesh), mesh1(_mesh1), m_color1(_m_color1), m_color2(_m_color2){};
	std::vector<newV3f> sm;
	//std::vector<VertexHandle> smid;
	std::vector<newV3f> sm1;
	std::vector<V3f> drawvbig;//��Ҫ�����ĵ㣬���д����ĵ�
	std::vector<VertexHandle> drawvbigid;
	std::vector<V3f> drawvsmall;//С���ĵ�
	//std::vector<VertexHandle> sm1id;
	std::set<newV3f> color;
	std::vector<double> colorverr;
	void sorting();//����λ
	void comparing();//�Ƚ�ѡ��
	void comparing1();//�Ƚ�ѡ��
	double max;
	double min;
	std::vector<double> m_color1;
	std::vector<double> m_color2;
	double averr;
	double relerr;
public:
	void sortmesh();
	void sortmesh1();
	HexV3fMesh *mesh;
	HexV3fMesh *mesh1;
};