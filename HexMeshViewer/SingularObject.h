#pragma once

#include "ComHeader.h"
#include "GeometryKernel.h"

class SingularEdge;

class SingularObject {
private:
	HexV3fMesh *mesh;
	//提取的奇异点、边列表
	std::vector<VertexHandle> vector_singular_vertex_handles;
	std::vector<EdgeHandle> vector_singular_edge_handles;

	std::vector<bool> boolVProp_singular;

	std::vector<unsigned char> intEProp_singular;

	//对应边类型
	enum E_TYPE;
	std::vector<E_TYPE> e_type_vec;

	//对应点类型
	enum V_TYPE;
	std::vector<V_TYPE> v_type_vec;

	bool B_READY = false;
public:
	enum E_TYPE
	{
		regular_e = 1,
		bdy_se = 2,
		inner_se = 3,
		feature_e = 4,
		feature_bdy_se = 7
	};
	enum V_TYPE
	{
		regular_v = 1,
		bdy_sv = 2,
		inner_sv = 3,
		feature_v = 4,
		feature_bdy_sv = 7
	};
	SingularObject();
	SingularObject(HexV3fMesh* in_mesh);
	SingularObject(HexV3fMesh* in_mesh, std::vector<EdgeHandle>& addin_vec);

	~SingularObject();

	const std::vector<EdgeHandle>& getEdgeVector() {
		return vector_singular_edge_handles;
	}
	const std::vector<VertexHandle>& getSingularVector() {
		return vector_singular_vertex_handles;
	}
	const std::vector<unsigned char>& getIntEProp_singular() {
		return intEProp_singular;
	}
	const std::vector<E_TYPE>& getEdgeType() {
		return e_type_vec;
	}
	const std::vector<V_TYPE>& getVerticeType() {
		return v_type_vec;
	}
	void FindSingularObject_simple();
protected:

	//初始化网格
	bool InitMesh();

};


////奇异边操作类
//class SingularEdge {
//private:
//	VertexHandle endPoint_1;
//	VertexHandle endPoint_2;
//
//	bool is_filled_complish = false;
//	bool is_entirely_in_bdy = false;
//	bool is_loopped = false;
//public:
//	//按边顺序插入 
//	std::vector<VertexHandle> ver_vector;
//	std::vector<EdgeHandle> edge_vector;
//	HexV3fMesh* mesh;
//
//	//复制函数
//	SingularEdge(const SingularEdge& se) {
//		this->endPoint_1 = se.endPoint_1;
//		this->endPoint_2 = se.endPoint_2;
//
//		this->is_entirely_in_bdy = se.is_entirely_in_bdy;
//		this->is_filled_complish = se.is_filled_complish;
//		this->mesh = se.mesh;
//
//		ver_vector.assign(se.ver_vector.begin(), se.ver_vector.end());
//		edge_vector.assign(se.edge_vector.begin(), se.edge_vector.end());
//		is_filled_complish = true;
//		is_loopped = se.is_loopped;
//	}
//
//	SingularEdge & operator=(SingularEdge &se) {
//		se.endPoint_1 = this->endPoint_1;
//		se.endPoint_2 = this->endPoint_2;
//
//		se.is_entirely_in_bdy = this->is_entirely_in_bdy;
//		se.is_filled_complish = this->is_filled_complish;
//		se.mesh = this->mesh;
//
//		se.ver_vector.assign(ver_vector.begin(), ver_vector.end());
//		se.edge_vector.assign(edge_vector.begin(), edge_vector.end());
//		is_filled_complish = true;
//		is_loopped = se.is_loopped;
//		return *this;
//	}
//
//	SingularEdge() {
//		this->endPoint_1 = mesh->InvalidVertexHandle;
//		this->endPoint_2 = mesh->InvalidVertexHandle;
//		mesh = NULL;
//		is_filled_complish = false;
//
//	}
//
//	SingularEdge(HexV3fMesh *mesh, std::vector<VertexHandle> ver_vector, std::vector<EdgeHandle> edge_vector, VertexHandle endv1, VertexHandle endv2, bool is_loop) {
//		if (mesh == NULL) {
//			this->is_filled_complish = false;
//
//			return;
//		}
//		this->mesh = mesh;
//		this->endPoint_1 = endv1;
//		this->endPoint_2 = endv2;
//		if ((ver_vector.size() == 0) || (edge_vector.size() == 0)) {
//			this->is_filled_complish = false;
//			this->endPoint_1 = mesh->InvalidVertexHandle;
//			this->endPoint_2 = mesh->InvalidVertexHandle;
//			return;
//		}
//
//		this->ver_vector.swap(ver_vector);
//		this->edge_vector.swap(edge_vector);
//		is_loopped = is_loop;
//		//填充边集合
//		is_filled_complish = true;
//		//校验
//		Get_is_filled_complish(true);
//	}
//
//	bool Get_is_loopped() {
//		return is_loopped;
//	}
//
//	void Set_is_loopped(bool is_loop) {
//		is_loopped = is_loop;
//	}
//
//	VertexHandle Get_end_point(bool is_v1) {
//		return is_v1 ? endPoint_1 : endPoint_2;
//	}
//
//
//	void Set_is_filled_complish(bool is) {
//		is_entirely_in_bdy = is;
//	}
//
//	bool Get_is_filled_complish(bool need_checked) {
//		if (need_checked) {
//			for (std::vector<EdgeHandle>::iterator edge_iter = edge_vector.begin(); edge_iter != edge_vector.end(); ++edge_iter) {
//				//校验一次
//				OpenVolumeMesh::OpenVolumeMeshEdge edge = mesh->edge(*edge_iter);
//
//				if (!mesh->is_boundary(edge.from_vertex()) || !mesh->is_boundary(edge.to_vertex())) {
//					is_entirely_in_bdy = false;
//					return false;
//				}
//			}
//			is_entirely_in_bdy = true;
//			return true;
//		}
//		return is_entirely_in_bdy;
//	}
//	/*
//	bool is_filled_complish = false;
//	bool is_entirely_in_bdy = false;
//	getter setter
//	*/
//	bool Get_is_entireely_in_bdy() {
//		return this->is_entirely_in_bdy;
//	}
//};

