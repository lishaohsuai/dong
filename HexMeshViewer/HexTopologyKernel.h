#pragma once

#include "ComHeader.h"

// Regular Boundary edge degree
static const unsigned char REGULAR_EDGE_BDY_N = 3;
// Regular inner edge degree
static const unsigned char REGULAR_EDGE_INNER_N = 4;
// regular inner vertex inner degree
static const unsigned char REGULAR_VERTEX_INNER_N = 6;
// regular Boundary vertex degree
static const unsigned char REGULAR_VERTEX_BDY_N = 5;

//no meaning, just for sign the intermediate iteration addin unreaular edge
static const unsigned char UNREGULAR_EDGE_ADDIN_N = 10;

/*
vertexId
		  0------------ 4
		1 ----------- 5 |        |Z
		||            | |        |
		||            | |        |___________Y
		|3 -----------|-7      _-
		2------------ 6      _-X
edgeId
	 (0, 1) 0
	 (1, 5) 1
	 (5, 4) 2
	 (4, 0) 3
	 (3, 2) 4
	 (3, 7) 5
	 (7, 6) 6
	 (6, 2) 7
	 (1, 2) 8
	 (6, 5) 9
	 (3, 0) 10
	 (4, 7) 11
*/

const uint16_t FACE_LINK_V_INDEX[6 * 4] = {
	0,4,5,1, // up
	2,6,7,3, // down
	1,5,6,2, // front
	3,7,4,0, // back
	4,7,6,5, // right
	1,0,3,2  // left
};

const uint16_t EDGE_LINK_V_INDEX[12*2] = {
	0,1, 1,5, 5,4, 4,0, // up
	2,3, 3,7, 7,6, 6,2, // down
	1,2, 6,5, 3,0, 4,7  // mid
};

const uint16_t FACE_LINK_E_INDEX[6 * 4] = {
	0,1,2,3, // (0, 1) 0  
	4,5,6,7,
	8,7,9,1,
	10,3,11,5,
	2,9,6,11,
	0,10,4,8
};

const uint16_t EDGE_LINK_FACE[12][2] = { 
	{0, 5}, // 0 
	{0, 2}, // 1
	{0, 4}, // 2
	{0, 3}, // 3
	{1, 5}, // 4
	{1, 3}, // 5
	{1, 4}, // 6
	{1, 2}, // 7
	{2, 5}, // 8
	{2, 4}, // 9
	{3, 5}, // 10
	{3, 4}  // 11
};


const uint16_t EDGE_LINK_VERTICAL_FACE[12][2] = {
	{2, 3}, // 0
	{4, 5}, // 1
	{2, 3}, // 2
	{4, 5}, // 3
	{2, 3}, // 4
	{4, 5}, // 5
	{2, 3}, // 6
	{4, 5}, // 7
	{0, 1}, // 8
	{0, 1}, // 9
	{0, 1}, // 10
	{0, 1}  // 11
};

const uint16_t VERTEX_LINK_EDGE[8][3] = {
	{0, 3, 10}, // 0
	{0, 1, 8}, // 1
	{4, 7, 8}, // 2
	{4, 5, 10}, // 3
	{2, 3, 11}, // 4
	{1, 2, 9}, // 5
	{6, 7, 9}, // 6
	{5, 6, 11} // 7
};

class MeshHandle {
public:
	explicit MeshHandle(int _idx) : idx_(_idx) {};
	MeshHandle& operator=(int _idx) {
		idx_ = _idx;
		return *this;
	}

	MeshHandle& operator=(const MeshHandle& _idx) {
		idx_ = _idx.idx_;
		return *this;
	}

	inline bool is_valid() const { return idx_ != -1; }
	inline bool operator<(const MeshHandle& _idx) const { return (this->idx_ < _idx.idx_); }
	inline bool operator<(int _idx) const { return idx_ < _idx; }
	inline bool operator>(const MeshHandle& _idx) const { return (this->idx_ > _idx.idx_); }
	inline bool operator>(int _idx) const { return idx_ > _idx; }
	inline bool operator==(const MeshHandle& _h) const { return _h.idx_ == this->idx_; }
	inline bool operator!=(const MeshHandle& _h) const { return _h.idx_ != this->idx_; }
	inline const int& idx() const { return idx_; }
	void idx(const int& _idx) { idx_ = _idx; }
	inline operator int() const { return idx_; }
	void reset() { idx_ = -1; }

private:
	int idx_;
};

	
struct compare_OVM
{
	bool operator()(const MeshHandle &a, const MeshHandle &b) const{
		return a.idx()<b.idx();
	}
};

//============================================================================================
//============================= vertex edge face cell ========================================

class VertexHandle : public MeshHandle { 
public: 
	explicit VertexHandle(int _idx = -1) : MeshHandle(_idx) {} // ��ֹ��ʽ���캯��
};

class EdgeHandle : public MeshHandle {
public: 
	explicit EdgeHandle(int _idx = -1) : MeshHandle(_idx) {}
};

class FaceHandle : public MeshHandle { 
public: 
	explicit FaceHandle(int _idx = -1) : MeshHandle(_idx) {} 
};

class CellHandle : public MeshHandle { 
public: 
	explicit CellHandle(int _idx = -1) : MeshHandle(_idx) {} 
};

//============================================================================================
