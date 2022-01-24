#pragma once

#include "HexTopologyKernel.h"
#include "GeometricTools\Wm4Vector3.h"
#include <unordered_map>
#include "ComHeader.h"

class V3f
{
public:
	V3f() {
		x = y = z = 0;
	}
	V3f(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	V3f(Wm4::Vector3<double> _v) : x(_v.X()), y(_v.Y()), z(_v.Z()) {}
	inline bool operator==(const V3f& _v) const { // �ں������const ��ʾ����������������������ʲô��
		if (_v.x == x && _v.y == y && _v.z == z)
			return true;
		else
			return false;
	}
	inline V3f operator%(const V3f& _rhs) const //�������
	{
		return
			V3f(y * _rhs.z - z * _rhs.y,
				z * _rhs.x - x * _rhs.z,
				x * _rhs.y - y * _rhs.x);
	}
	inline V3f operator+(const V3f& _rhs) const {
		return V3f(x + _rhs.x, y + _rhs.y, z + _rhs.z);
	}
	inline V3f & operator+=(const V3f& _rhs) {
		this->x += _rhs.x;
		this->y += _rhs.y;
		this->z += _rhs.z;
		return *this;
	}
	inline V3f operator-(const V3f& _rhs) const {
		return V3f(x - _rhs.x, y - _rhs.y, z - _rhs.z);
	}
	inline double operator*(const V3f& _rhs) const {
		return x * _rhs.x + y * _rhs.y +z * _rhs.z;
	}
	inline V3f operator/(const double normal) const {
		return V3f(x / normal, y / normal, z / normal);
	}
	inline V3f operator/=(const double normal) {
		this->x /= normal;
		this->y /= normal;
		this->z /= normal;
		return *this;
	}
	inline V3f operator*=(const double normal) {
		this->x *= normal;
		this->y *= normal;
		this->z *= normal;
		return *this;
	}
	inline V3f operator*(const double normal) const {
		return V3f(x * normal, y * normal, z * normal);
	}
	inline bool operator<(const V3f& _v) const
	{
		if (fabs(x - _v.x) > PRECISION) {
			return x < _v.x;
		}
		else {
			if (fabs(y - _v.y) > PRECISION)
				return y < _v.y;
			else
			{
				if (fabs(z - _v.z) > PRECISION) {
					return z < _v.z;
				}
				else {
					return false;
				}
			}
		}
	}
	double x, y, z;
	bool is_valid = true;
	int lastid; // ����MLCAϸ��
	int bezierid = 0;//������Bezier��ıȽ�
public:

	inline double norm() {
		return sqrt(x*x + y * y + z * z);
	}

	inline double sqrnorm() {
		return sqrt(sqrt(x*x + y * y + z * z));
	}

	inline V3f normalize() {
		double norm = this->norm();
		return V3f(x / norm, y / norm, z / norm);
	}
};

inline V3f opx(const V3f& _rhs) {
	return V3f(_rhs.x, -_rhs.y, _rhs.z);
}

inline V3f opy(const V3f& _rhs) {
	return V3f(-_rhs.x, _rhs.y, _rhs.z);
}

inline V3f op0(const V3f& _rhs) {
	return V3f(-_rhs.x, -_rhs.y, _rhs.z);
}

inline V3f timeNumber(const V3f& _rhs, double n) {
	return V3f(n * _rhs.x, n * _rhs.y, n * _rhs.z);
}

inline double distance(V3f &_v1, V3f _v2)
{
	double dis = sqrt((_v1.x - _v2.x)*(_v1.x - _v2.x) + (_v1.y - _v2.y)*(_v1.y - _v2.y)
		+ (_v1.z - _v2.z)*(_v1.z - _v2.z));
	return dis;
}



/**************************************************
@brief   : ��ϣ����
@author  : none
@input   ��none
@output  ��none
@time    : none
**************************************************/
namespace std
{
	template<> struct hash<V3f>
	{
		size_t operator()(const V3f &v)const
		{
			return hash<double>()(v.x) ^ hash<double>()(v.y) ^ hash<double>()(v.z);
		}
	};
}


class Edge {
	friend class HexV3fMesh;
public:
	VertexHandle from_h, to_h;
	bool is_valid = true;
	Edge() {}
	Edge(const VertexHandle _from_h, const VertexHandle _to_h) :from_h(_from_h), to_h(_to_h) {}
	Edge& operator=(const Edge& edge) {
		this->from_h = edge.from_h;
		this->to_h = edge.to_h;
		return *this;
	}
	~Edge() {}
	inline bool operator==(const Edge& _e) const {
		if ((_e.from_h == from_h) && (_e.to_h == to_h)) {
			return true;
		}
		else if ((_e.to_h == from_h) && (_e.from_h == to_h)) {
			return true;
		}
		return false;
	}
	inline std::string getStr1() {
		return std::to_string(from_h) + "-" + std::to_string(to_h);
	}
	inline std::string getStr2() {
		return std::to_string(to_h) + "-" + std::to_string(from_h);
	}
};

class Face {
	friend class HexV3fMesh;
public:
	std::vector<EdgeHandle> edges_;
	std::vector<VertexHandle> vs_f;// vs_f����Ķ��㣿
	bool is_valid = true;
	Face() {};
	explicit Face(const std::vector<EdgeHandle>& _edges, std::vector<VertexHandle> vs_) :edges_(_edges), vs_f(vs_) {
		std::sort(edges_.begin(), edges_.end(), compare_OVM());
		//std::sort(vs_f.begin(), vs_f.end(), compare_OVM());
	}
	Face& operator=(const Face& f) {
		this->edges_.assign(f.edges_.begin(), f.edges_.end());
		this->vs_f.assign(f.vs_f.begin(), f.vs_f.end());
		return *this;
	}
	~Face() {}
	inline bool operator==(const Face& _f) const {
		//�Ŀ��ٵ�
		if (_f.vs_f[0].idx() + _f.vs_f[1].idx() + _f.vs_f[2].idx() + _f.vs_f[3].idx() !=
			vs_f[0].idx() + vs_f[1].idx() + vs_f[2].idx() + vs_f[3].idx())
			return false;
		std::set<VertexHandle, compare_OVM> vs_f_c;
		for (uint16_t i = 0; i < 4; ++i) {
			vs_f_c.insert(_f.vs_f[i]);
		}
		for (uint16_t i = 0; i < 4; ++i) {
			vs_f_c.insert(vs_f[i]);
		}
		if (vs_f_c.size() == 4) return true;
		return false;
	}
	
	inline std::string getStr() {
		std::set<VertexHandle, compare_OVM> vs_f_c;
		for (uint16_t i = 0; i < 4; ++i) {
			vs_f_c.insert(vs_f[i]);
		}
		auto it = vs_f_c.begin();
		return std::to_string(*it) + "-" + std::to_string(*(++it)) + "-" + std::to_string(*(++it))
			+ "-" + std::to_string(*(++it));
	}
};

class Cell {
	friend class HexV3fMesh;
public:
	//0-5 xf~zb
	std::vector<FaceHandle> faces_;
	//0-7 position
	std::vector<VertexHandle> vertex_;
	std::vector<EdgeHandle> edges_;
	bool is_valid = true;
	Cell() {};
	explicit Cell(const std::vector<FaceHandle>& _faces, const std::vector<VertexHandle>& _vs) :faces_(_faces), vertex_(_vs) {
		//std::sort(vertex_.begin(), vertex_.end(), compare_OVM());
	}
	explicit Cell(const std::vector<FaceHandle>& _faces, const std::vector<VertexHandle>& _vs,
		const std::vector<EdgeHandle>& _es) :faces_(_faces), vertex_(_vs), edges_(_es) {}
	Cell& operator=(const Cell& cell) {
		this->faces_.assign(cell.faces_.begin(), cell.faces_.end()); // assign ��Ӧ��Ӧ�������¸�ֵ
		this->vertex_.assign(cell.vertex_.begin(), cell.vertex_.end());
		this->edges_.assign(cell.edges_.begin(), cell.edges_.end());
		return *this;
	}
	~Cell() {}
	inline bool operator==(const Cell& cell) const {
		std::set<VertexHandle, compare_OVM> vs_c_c;
		for (uint16_t i = 0; i < 8; ++i) {
			vs_c_c.insert(cell.vertex_[i]);
		}
		for (uint16_t i = 0; i < 4; ++i) { // maybe here 8 is more better
			vs_c_c.insert(vertex_[i]);
		}
		if (vs_c_c.size() == 8) return true;
		return false;
	}
};


/**
 * @description: QuadV3fMesh
 * @param {*}
 * @return {*}
 */
class QuadV3fMesh
{
public:
	std::vector<V3f> vertices_;
	std::vector<Edge> edges_;
	std::vector<Face> faces_;

	std::vector<bool> vdraw;
	//ϸ��ʹ��
	std::vector<bool> v_cs;//�ۺ۵�

	//�ڽӹ�ϵ
	//---���ڽӱ�
	std::vector<std::set<EdgeHandle, compare_OVM>> neighbor_v;
	//---���ڽ���
	std::vector<std::set<FaceHandle, compare_OVM>> neighbor_v_f;
	//---���ڽ���
	std::vector<std::set<FaceHandle, compare_OVM>> neighbor_e;

	VertexHandle add_vertex(const V3f& _p) {
		std::vector<V3f>::iterator v_id;
		if ((v_id = std::find(vertices_.begin(), vertices_.end(), _p)) == vertices_.end()) {
			vertices_.push_back(_p);
			neighbor_v.push_back(std::set<EdgeHandle, compare_OVM>());
			neighbor_v_f.push_back(std::set<FaceHandle, compare_OVM>());
			return VertexHandle(vertices_.size() - 1);
		}
		else {
			return VertexHandle(abs(std::distance(vertices_.begin(), v_id)));
		}
	}

	FaceHandle add_face(const std::vector<VertexHandle>& v_ve)
	{
		//����� ���ڽӹ�ϵ v-e
		std::vector<EdgeHandle> e_handle(4);
		std::vector<Edge> tempe{ Edge(v_ve[0],v_ve[1]),Edge(v_ve[1],v_ve[2]),Edge(v_ve[2],v_ve[3]),Edge(v_ve[3],v_ve[0]) };
		for (uint16_t i = 0; i < 4; ++i) {
			Edge temp_e = tempe[i];
			std::vector<Edge>::iterator it;
			if ((it = std::find(edges_.begin(), edges_.end(), temp_e)) != edges_.end()) {
				//�Ѵ���
				e_handle[i] = EdgeHandle(it - edges_.begin());
			}
			else {
				//������ʱ
				edges_.push_back(temp_e);
				neighbor_e.push_back(std::set<FaceHandle, compare_OVM>());
				e_handle[i] = EdgeHandle(edges_.size() - 1);
				neighbor_v[temp_e.from_h].insert(e_handle[i]);
				neighbor_v[temp_e.to_h].insert(e_handle[i]);
			}
		}
		//������
		faces_.push_back(Face(e_handle, v_ve));
		//f-e�ڽӹ�ϵ
		for (uint16_t arr_i = 0; arr_i < 4; ++arr_i) {
			neighbor_e[e_handle[arr_i]].insert(FaceHandle(faces_.size() - 1));
		}
		//v_f�ڽӹ�ϵ
		for (uint16_t arr_i = 0; arr_i < 4; ++arr_i) {
			neighbor_v_f[v_ve[arr_i]].insert(FaceHandle(faces_.size() - 1));
		}
		return FaceHandle(faces_.size() - 1);
	}
public:
	QuadV3fMesh() {};
	~QuadV3fMesh() {};
	std::vector<V3f> normal;//������
	std::vector<EdgeHandle> drawedge;
	void checkdrawedge()
	{
		for (int i = 0; i < edges_.size(); ++i) {
			if (vdraw[edges_[i].from_h] && vdraw[edges_[i].to_h]) {
				drawedge.push_back(EdgeHandle(i));
			}
		}
	}
	void cal_norm()
	{
		for (int j = 0; j < faces_.size(); j++)
		{
			V3f a; V3f b; V3f n;
			a = vertices_[faces_[j].vs_f[1]] - vertices_[faces_[j].vs_f[0]];
			b = vertices_[faces_[j].vs_f[3]] - vertices_[faces_[j].vs_f[0]];
			n = a % b;
			V3f c = normal[j];
			double temp = n.x*c.x + n.y*c.y + n.z*c.z;
			if (temp > 0)
				n = n * -1;
			n = n.normalize();
			normal[j] = n * -1;
		}
	}
	void changefe()
	{
		for (int i = 0; i < faces_.size(); ++i)
		{
			std::vector<EdgeHandle> right(4);
			std::vector<EdgeHandle> fe = faces_[i].edges_;
			for (int j = 0; j < 4; ++j)
			{
				if ((edges_[fe[j]].from_h == faces_[i].vs_f[0] && edges_[fe[j]].to_h == faces_[i].vs_f[1])
					|| (edges_[fe[j]].from_h == faces_[i].vs_f[1] && edges_[fe[j]].to_h == faces_[i].vs_f[0]))
				{
					right[0] = fe[j];
					break;
				}
			}
			for (int j = 0; j < 4; ++j)
			{
				if ((edges_[fe[j]].from_h == faces_[i].vs_f[1] && edges_[fe[j]].to_h == faces_[i].vs_f[2])
					|| (edges_[fe[j]].from_h == faces_[i].vs_f[2] && edges_[fe[j]].to_h == faces_[i].vs_f[1]))
				{
					right[1] = fe[j];
					break;
				}
			}
			for (int j = 0; j < 4; ++j)
			{
				if ((edges_[fe[j]].from_h == faces_[i].vs_f[2] && edges_[fe[j]].to_h == faces_[i].vs_f[3])
					|| (edges_[fe[j]].from_h == faces_[i].vs_f[3] && edges_[fe[j]].to_h == faces_[i].vs_f[2]))
				{
					right[2] = fe[j];
					break;
				}
			}
			for (int j = 0; j < 4; ++j)
			{
				if ((edges_[fe[j]].from_h == faces_[i].vs_f[3] && edges_[fe[j]].to_h == faces_[i].vs_f[0])
					|| (edges_[fe[j]].from_h == faces_[i].vs_f[0] && edges_[fe[j]].to_h == faces_[i].vs_f[3]))
				{
					right[3] = fe[j];
					break;
				}
			}
			faces_[i].edges_ = right;
		}
	}
};

class HexV3fMesh {
public:
	//base elements
	std::map<V3f, int> quick_v;
	int dis_v = 0;
	std::vector<V3f> vertices_;
	std::vector<Edge> edges_;
	std::unordered_map<std::string, bool> unEmap;
	std::unordered_map<std::string, int> unEdgeIndexMap;
	std::vector<Face> faces_;
	std::unordered_map<std::string, bool> unFmap;
	std::unordered_map<std::string, int> unFaceIndexMap;
	std::vector<Cell> cells_;

	std::vector<bool> v_bdy;

	//for subdivide
	std::vector<V3f> v_c_crease;//crease corner vertex
	std::vector<V3f> v_crease;//crease vertex
	std::vector<V3f> v3f_singular;//singular point
	std::vector<bool> v_cs;//crease point

	//link relationship
	//---vertex link edge
	std::vector<std::set<EdgeHandle, compare_OVM>> neighbor_v;
	//---vertex link face
	std::vector<std::set<FaceHandle, compare_OVM>> neighbor_v_f;
	//---vertex link cell
	std::vector<std::set<CellHandle, compare_OVM>> neighbor_v_c;
	//---edge link face
	std::vector<std::set<FaceHandle, compare_OVM>> neighbor_e;
	//---face link cell
	std::vector<std::set<CellHandle, compare_OVM>> neighbor_f;
	//---edge link cell
	std::set<CellHandle> neighbor_e_c(EdgeHandle _e) {
		std::set<CellHandle> temp;
		std::set<FaceHandle, compare_OVM> ef = neighbor_e[_e];
		for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
		{
			std::set<CellHandle, compare_OVM> efc = neighbor_f[*ef_it];
			for (std::set<CellHandle, compare_OVM>::iterator efc_it = efc.begin(); efc_it != efc.end(); efc_it++)
			{
				temp.insert(*efc_it);
			}
		}
		return temp;
	}
	//cell link cell
	std::set<CellHandle> neighbor_c_c(CellHandle _c) {
		std::set<CellHandle> temp;
		int cf_size = cells_[_c].faces_.size();
		for (int i = 0; i < cf_size; i++)
		{
			std::set<CellHandle, compare_OVM> cfc = neighbor_f[cells_[_c].faces_[i]];
			for (std::set<CellHandle, compare_OVM>::iterator cfc_it = cfc.begin(); cfc_it != cfc.end(); cfc_it++)
			{
				if (*cfc_it != _c)
				{
					temp.insert(*cfc_it);
					break;
				}
			}
		}
		return temp;
	}
	//---vertex link boundary edge
	std::set<EdgeHandle> neighbor_v_bdy(VertexHandle _v) {
		std::set<EdgeHandle> v_e_bdy;
		std::set<EdgeHandle, compare_OVM> v_e = neighbor_v[_v];
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (is_bdy(*v_e_it))
				v_e_bdy.insert(*v_e_it);
		}
		return v_e_bdy;
	}
	//---edge link boundary face
	std::set<FaceHandle> neighbor_e_bdy(EdgeHandle _e) {
		std::set<FaceHandle> e_f_bdy;
		std::set<FaceHandle, compare_OVM> e_f = neighbor_e[_e];
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
		{
			if (is_bdy(*e_f_it))
				e_f_bdy.insert(*e_f_it);
		}
		return e_f_bdy;
	}
	//---cell ring adjacent cell
	std::set<CellHandle> neighbor_c_circle_c(CellHandle _c) {
		std::set<CellHandle> temp;
		std::vector<FaceHandle> c_f = cells_[_c].faces_;
		for (auto c_f_it = c_f.begin(); c_f_it != c_f.end(); c_f_it++)
		{
			std::set<CellHandle, compare_OVM> c_f_c = neighbor_f[*c_f_it];
			for (auto c_f_c_it = c_f_c.begin(); c_f_c_it != c_f_c.end(); c_f_c_it++)
				if (*c_f_c_it != _c)
					temp.insert(*c_f_c_it);
		}
		return temp;
	}

	std::set<FaceHandle> neighbor_f_f(int, FaceHandle _f) {
		std::set<FaceHandle> temp;
		for (int i = 0; i < 4; i++) {
			EdgeHandle e = faces_[_f].edges_[i];
			std::set<FaceHandle, compare_OVM> e_f = neighbor_e[e];
			for (auto e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
			{
				if (is_bdy(1, *e_f_it) && *e_f_it != _f)
					temp.insert(*e_f_it);
			}
		}
		return temp;
	}

	std::set<FaceHandle> neighbor_f_f(FaceHandle _f) {
		std::set<FaceHandle> temp;
		for (int i = 0; i < 4; i++) {
			EdgeHandle e = faces_[_f].edges_[i];
			std::set<FaceHandle, compare_OVM> e_f = neighbor_e[e];
			for (auto e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++) {
				if (is_bdy(*e_f_it) && *e_f_it != _f)
					temp.insert(*e_f_it);
			}
		}
		return temp;
	}

	//Qt draw image
	std::vector<std::vector<V3f>> boundary_face_v;	//boundary face vertex
	std::vector<FaceHandle> boundary_face;//boundary face
	std::vector<std::vector<int>> boundary_face_VID;//boundary face vertex id
	std::vector<V3f> normal_boundary_face_v;//boundary face normal
	std::vector<CellHandle> boundary_cellid;//boundary cell id
	std::vector<V3f> oriv;// original model vertex
	V3f center;// center point
	double r;// radius
	double average_r;//average radius
	std::vector<V3f> cell_center;//every cell center
	std::vector<double> cell_r;//every cell radius
	double minEdgeLen;
	double maxEdgeLen;
	V3f bbMin;//boudning box
	V3f bbMax;

	HexV3fMesh() {}
	~HexV3fMesh() {}
	HexV3fMesh(const HexV3fMesh& _mesh) {
		this->vertices_ = _mesh.vertices_;
		this->edges_ = _mesh.edges_;
		this->faces_ = _mesh.faces_;
		this->cells_ = _mesh.cells_;

		this->v_bdy = _mesh.v_bdy;
		this->neighbor_v = _mesh.neighbor_v;
		this->neighbor_v_c = _mesh.neighbor_v_c;
		this->neighbor_v_f = _mesh.neighbor_v_f;
		this->neighbor_e = _mesh.neighbor_e;
		this->neighbor_f = _mesh.neighbor_f;
	}
	HexV3fMesh& HexV3fMesh::operator=(const HexV3fMesh& _mesh) {
		this->vertices_ = _mesh.vertices_;
		this->edges_ = _mesh.edges_;
		this->faces_ = _mesh.faces_;
		this->cells_ = _mesh.cells_;

		this->v_bdy = _mesh.v_bdy;
		this->neighbor_v = _mesh.neighbor_v;
		this->neighbor_v_c = _mesh.neighbor_v_c;
		this->neighbor_v_f = _mesh.neighbor_v_f;
		this->neighbor_e = _mesh.neighbor_e;
		this->neighbor_f = _mesh.neighbor_f;

		//ò�����ûд
		return *this;
	}
	static const VertexHandle   InvalidVertexHandle;
	static const EdgeHandle     InvalidEdgeHandle;
	static const FaceHandle     InvalidFaceHandle;
	static const CellHandle     InvalidCellHandle;

	VertexHandle add_vertex(const V3f& _p);
	VertexHandle add_vertex_new(const V3f& _p);

	FaceHandle add_face(const std::vector<VertexHandle>& v_ve);
	CellHandle add_cell(const std::vector<VertexHandle>& v_ve);

	CellHandle add_cell(const std::vector<FaceHandle>& f_ve);

	void add_mesh(HexV3fMesh *mesh);

	double cal_angle(EdgeHandle e1, EdgeHandle e2, VertexHandle v)//������vΪ�е����߹��ɵļнǵ�ֵ
	{
		V3f n1, n2;
		if (edges_[e1].from_h == v)
			n1 = vertices_[edges_[e1].to_h] - vertices_[v];
		else
			n1 = vertices_[edges_[e1].from_h] - vertices_[v];
		if (edges_[e2].from_h == v)
			n2 = vertices_[edges_[e2].to_h] - vertices_[v];
		else
			n2 = vertices_[edges_[e2].from_h] - vertices_[v];
		double temp = (n1.x * n2.x + n1.y * n2.y + n1.z * n2.z) / (n1.norm() * n2.norm());
		return acos(temp);
	}

	void examine_bdy() {
		v_bdy.resize(vertices_.size(), false);
		for (uint64_t i = 0; i < neighbor_f.size(); ++i) {
			if (neighbor_f[i].size() == 1) {
				for (uint16_t i_v = 0; i_v < 4; ++i_v) {
					v_bdy[faces_[i].vs_f[i_v]] = true;
				}
			}
		}
	}

	void examine_bdy(int)
	{
		v_bdy.resize(vertices_.size(), false);
		for (int i = 0; i < vertices_.size() - 1; ++i)
		{
			v_bdy[i] = true;
		}
	}

	//examine crease
	void examine_cs() {
		v_cs.resize(vertices_.size(), false);
	}

	bool is_cs(const VertexHandle& v_)
	{
		return v_cs[v_];
	}

	bool is_cs(const EdgeHandle& e_) {
		if (v_cs[edges_[e_].from_h] == true && v_cs[edges_[e_].to_h] == true)
			return true;
		else
			return false;
	}

	bool is_bdy(const VertexHandle& v_) {
		return v_bdy[v_];
	}
	
	bool is_bdy(const EdgeHandle& e_) {
		std::set<FaceHandle, compare_OVM> ef = neighbor_e[e_];
		for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
		{
			if (is_bdy(*ef_it))
				return true;
		}
		return false;
	}

	bool is_bdy(int, const EdgeHandle& e_) {
		if (is_bdy(edges_[e_].from_h) && is_bdy(edges_[e_].to_h))
			return true;
		else
			return false;
	}

	bool is_bdy(const FaceHandle& f_) {
		return neighbor_f[f_].size() == 1;
	}
	bool is_bdy(int, const FaceHandle& f_) {
		for (int i = 0; i < faces_[f_].vs_f.size(); ++i)
		{
			if (!is_bdy(faces_[f_].vs_f[i]))
				return false;
		}
		return true;
	}
	bool is_bdy(const FaceHandle& f_, int) { //����DAT
		std::vector<VertexHandle> fv = faces_[f_].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			if (v_bdy[*fv_it] == false)
				return false;
		}
		return true;
	}
	bool is_bdy(const CellHandle& c_) {
		for (uint16_t i = 0; i < 6; ++i) {
			if (is_bdy(cells_[c_].faces_[i])) {
				return true;
			}
		}
		return false;
	}

	bool non_intersect(const EdgeHandle& e1, const EdgeHandle& e2) {
		Edge edge_1 = this->edges_[e1];
		Edge edge_2 = this->edges_[e2];
		if (
			((edge_1.from_h != edge_2.from_h) && (edge_1.from_h != edge_2.to_h)) &&
			((edge_1.to_h != edge_2.from_h) && (edge_1.to_h != edge_2.to_h))
			) {
			return true;
		}
		return false;
	}

	V3f baryCenter(const CellHandle& cellHandle) {
		V3f v(0.00f, 0.00f, 0.00f);
		std::vector<VertexHandle> set_v = this->cells_[cellHandle].vertex_;
		for (uint16_t i = 0; i < 8; ++i) {
			v.x += this->vertices_[set_v[i]].x / 8;
			v.y += this->vertices_[set_v[i]].y / 8;
			v.z += this->vertices_[set_v[i]].z / 8;
		}
		return v;
	}

	//find in cell,edge's two face is not face 
	FaceHandle adjacent_face_in_cell(const CellHandle& cellHandle, const EdgeHandle& edgeHandle, const FaceHandle& faceHandle) {
		std::vector<FaceHandle> faces = this->cells_[cellHandle].faces_;

		for (uint16_t i = 0; i < 6; ++i) {
			if (faces[i] == faceHandle) continue;
			std::vector<EdgeHandle> e_vec = this->faces_[faces[i]].edges_;
			if (std::count(e_vec.begin(), e_vec.end(), edgeHandle) != 0) {
				return faces[i];
			}
		}
		return InvalidFaceHandle;
	}

	uint8_t get_orientaion(FaceHandle face, CellHandle cell) {
		return std::distance(this->cells_[cell].faces_.begin(),
			std::find(this->cells_[cell].faces_.begin(), this->cells_[cell].faces_.end(), face));
	}

	FaceHandle get_opposite_face_in_cell(FaceHandle face, CellHandle cell) {
		uint8_t ori = get_orientaion(face, cell);
		return cells_[cell].faces_[ori % 2 ? ori - 1 : ori + 1];
	}

	EdgeHandle get_opposite_edge_in_face(EdgeHandle edge, FaceHandle face) {
		const std::vector<EdgeHandle>& edge_vec = faces_[face].edges_;
		for (uint8_t i = 0; i < 4; ++i) {
			if (
				((edges_[edge_vec[i]].from_h != edges_[edge].from_h) && (edges_[edge_vec[i]].to_h != edges_[edge].to_h))
				&& ((edges_[edge_vec[i]].from_h != edges_[edge].to_h) && (edges_[edge_vec[i]].to_h != edges_[edge].from_h))
				)return edge_vec[i];
		}
		return EdgeHandle(-1);
	}

	std::vector<EdgeHandle> get_3edge_in_corner(VertexHandle v, CellHandle cell) {

		std::vector<EdgeHandle> edge_vec;
		std::set<EdgeHandle, compare_OVM> edge_cell;
		for (uint8_t i = 0; i < 6; ++i) {
			std::vector<EdgeHandle> edges = faces_[cells_[cell].faces_[i]].edges_;
			for (uint8_t j = 0; j < 4; ++j) {
				edge_cell.insert(edges[j]);
			}
		}
		for (std::set<EdgeHandle, compare_OVM>::iterator iter = edge_cell.begin(); iter != edge_cell.end(); ++iter) {
			Edge edge = edges_[*iter];
			if ((edge.from_h == v) || (edge.to_h == v)) {
				edge_vec.push_back(*iter);
			}
		}
		return edge_vec;
	}

	std::vector<FaceHandle> get_3face_in_corner(VertexHandle v, CellHandle cell) {
		std::vector<FaceHandle> face;
		for (uint8_t i = 0; i < 6; ++i) {
			Face f = faces_[cells_[cell].faces_[i]];
			if (std::find(f.vs_f.begin(), f.vs_f.end(), v) != f.vs_f.end()) {
				face.push_back(cells_[cell].faces_[i]);
			}
		}
		return face;
	}

	std::vector<FaceHandle> get_2face_edge(EdgeHandle edge, CellHandle cell) {
		std::vector<FaceHandle> _2face;
		std::vector<FaceHandle> c_f = cells_[cell].faces_;
		for (std::vector<FaceHandle>::iterator c_f_it = c_f.begin(); c_f_it != c_f.end(); c_f_it++)
		{
			if (is_e_in_f(edge, *c_f_it))
				_2face.push_back(*c_f_it);
		}
		return _2face;
	}
	std::vector<EdgeHandle> get_parallel_edges(EdgeHandle edge, CellHandle cell) {
		std::vector<EdgeHandle> _4edge;
		std::vector<FaceHandle> face_vec = get_2face_edge(edge, cell);
		face_vec.push_back(get_opposite_face_in_cell(face_vec[0], cell));
		face_vec.push_back(get_opposite_face_in_cell(face_vec[1], cell));

		std::set<EdgeHandle, compare_OVM> set;
		uint8_t count_temp = 0;
		for (uint8_t i = 0; i < face_vec.size(); ++i) {
			Face face = this->faces_[face_vec[i]];
			for (uint8_t j = 0; j < 4; ++j) {
				set.insert(face.edges_[j]);
				if (set.size() == count_temp) { //ƽ�еı߶�Ҫ��������
					_4edge.push_back(face.edges_[j]);
				}
				else {
					++count_temp;
				}
			}
		}
		return _4edge;
	}

	//寻找这个面中的对角点
	VertexHandle find_op_vertex(VertexHandle _v, FaceHandle _f);


	//判断是否为奇异边奇异点
	bool is_singular(EdgeHandle _e);
	bool is_singular(VertexHandle _v);
	//判断面是否在块内
	bool is_f_in_c(FaceHandle _f, CellHandle _c);
	//判断边是否在面内
	bool is_e_in_f(EdgeHandle _e, FaceHandle _f);
	bool is_e_in_c(EdgeHandle _e, CellHandle _c);
	bool is_v_in_c(VertexHandle v, CellHandle c);
	//判断两条边是否在同一面内
	bool is_two_e_in_same_face(EdgeHandle e1, EdgeHandle e2);
	//判断两条边是否在同一块内
	bool is_two_e_in_same_cell(EdgeHandle e1, EdgeHandle e2);
	//判断两个面是否在同一快内
	bool is_two_f_in_same_cell(FaceHandle f1, FaceHandle f2);
	bool is_two_v_in_same_edge(VertexHandle v1, VertexHandle v2);
	//合并点
	bool merge_v();
	//对其中单元的删除合并
	CellHandle delete_cell(CellHandle cellHandle);

	//细分时使用
	void examine_crease();// 判断折痕点
	int val(VertexHandle v1, VertexHandle v2, bool _is_bdy);//包含v1，v2单元的数量
	int val(VertexHandle v, bool _is_bdy);//包含v1单元的数量
	int dim(VertexHandle v1, VertexHandle v2);//包含v1,v2的最小侧面的维数
	void examine_singular();

	//Qt画图时用
	void examine_bdfv();//寻找边界面
	void examine_bdfv(int);//寻找边界面
	void cal_cen();//计算中心点
	void cal_cen1();//计算中心点
	void cal_cell_cen();//分开写 不然读取速度太慢
	void cal_bdnorm();//计算边界面的法向量
	void cal_bdnorm(std::vector<V3f>& vn);
	void cal_bdnorm(std::vector<FaceHandle> &fh);
	void cal_mami_ed();//计算最长边与最短边
	V3f cal_norm(FaceHandle f);//计算一个面的法向量
	//void cal_bb();//�����Χ��
	//std::vector<CellHandle> incident_cell(FaceHandle _f);
	void cal_oriv()
	{
		int ver_size = vertices_.size();
		for (int i = 0; i < ver_size; i++)
		{
			if (vertices_[i].is_valid == true)
				oriv.push_back(vertices_[i]);
		}
	}
	double cal_edge_length(EdgeHandle i)
	{
		V3f v1 = vertices_[edges_[i].from_h];
		V3f v2 = vertices_[edges_[i].to_h];
		return (v1 - v2).norm();
	}
	V3f cal_edge_mid(EdgeHandle i)
	{
		V3f v1 = vertices_[edges_[i].from_h];
		V3f v2 = vertices_[edges_[i].to_h];
		return (v1 + v2) / 2;
	}
	V3f cal_face_center(FaceHandle f)
	{
		V3f v(0, 0, 0);
		for (int i = 0; i < 4; i++)
		{
			v = v + vertices_[faces_[f].vs_f[i]];
		}
		v = v / 4;
		return v;
	}
	V3f cal_edge_center(EdgeHandle i)
	{
		V3f v1 = vertices_[edges_[i].from_h];
		V3f v2 = vertices_[edges_[i].to_h];
		return (v1 + v2) / 2;
	}
public:
	std::vector<int> singularface;
	void checksingualrface();
	std::vector<std::pair<V3f, V3f>> frame; 
	std::vector<std::vector<V3f>> frameface; // GU: The outer surface
	std::vector<V3f> framefacenrom; // GU: the outerFace's norm
	void choseframe()
	{
		for (int i = 0; i < edges_.size(); ++i)
		{
			if (is_bdy(EdgeHandle(i)))
			{
				std::set<FaceHandle, compare_OVM> e_f = neighbor_e[i];
				if (e_f.size() == 2)
				{
					V3f v1 = vertices_[edges_[i].from_h];
					V3f v2 = vertices_[edges_[i].to_h];
					frame.push_back(std::pair<V3f, V3f>(v1, v2));
				}
			}
		}
	}
	void choseframeface()
	{
		for (int i = 0; i < faces_.size(); ++i)
		{
			if (is_bdy(FaceHandle(i)))
			{
				std::vector<V3f> temp;
				temp.push_back(vertices_[faces_[i].vs_f[0]]);
				temp.push_back(vertices_[faces_[i].vs_f[1]]);
				temp.push_back(vertices_[faces_[i].vs_f[2]]);
				temp.push_back(vertices_[faces_[i].vs_f[3]]);
				frameface.push_back(temp);
				V3f tempnorm = cal_norm(FaceHandle(i));
				framefacenrom.push_back(tempnorm);
			}
		}
	}
public:
	void smooth_mesh();
	QuadV3fMesh change2Quad();
};

struct beziersurface
{
	std::vector<FaceHandle> biaojiface;
	std::vector<std::vector<V3f>> bei_surv;
	std::vector<std::vector<std::vector<V3f>>> bei_surv_dat;
	std::vector<std::vector<V3f>> nor_bei_surv_dat;
	std::vector<V3f> nor_bei_surv;
	std::vector<V3f> bei_surv_center;
	std::vector<std::vector<V3f>> edgeline;
	std::vector<int> singularface;
	int tempdraw_num;
};

struct mvh
{
	std::vector<std::vector<V3f>> boundary_face_v;
	std::vector<V3f> normal_boundary_face_v;
};

struct frame1
{
	std::vector<std::vector<std::vector<V3f>>> bei_surv_datframe;
	std::vector<std::vector<V3f>> nor_beiframe;
};
