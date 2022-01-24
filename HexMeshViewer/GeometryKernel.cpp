#include "GeometryKernel.h"
#include "math.h"
#include <unordered_set>

VertexHandle HexV3fMesh::add_vertex(const V3f& _p) {
	//from 1
	std::vector<V3f>::iterator v_id;
	if (quick_v.count(_p) == 0) { // check the key is exist
		quick_v[_p] = dis_v++;
		vertices_.push_back(_p);
		neighbor_v.push_back(std::set<EdgeHandle, compare_OVM>());
		neighbor_v_f.push_back(std::set<FaceHandle, compare_OVM>());
		neighbor_v_c.push_back(std::set<CellHandle, compare_OVM>());
		return VertexHandle(dis_v - 1);
	}
	else {
		return VertexHandle(quick_v[_p]);
	}	
}

void HexV3fMesh::cal_bdnorm(std::vector<FaceHandle>& fh)
{
	normal_boundary_face_v.swap(std::vector<V3f>());
	//std::unordered_set<int> ff{ 7474 ,6132, 6128, 6131, 6130, 10836, 10875, 10877 ,10943 ,10960 ,1490, 7455 ,7453 ,7451, 7526 ,1462,8113 ,3790, 4061, 8576 ,8632 };
	std::unordered_set<int> ff;/*{ 16508 ,16511,19177 };*/
	//std::unordered_set<int> ff{ 14207 ,14202 ,14435 ,17965 ,223 ,16236 ,269, 116 ,44, 1190 ,19458 ,14209 ,14204 ,15545, 10484 ,11613 ,5123 ,8339 ,8338,8335, 19476, 5324, 951, 1004 };
	for (int i = 0; i < fh.size(); ++i) {
	ff.insert(fh[i]);
	}
	for (int bdn_num = 0; bdn_num < boundary_face.size(); bdn_num++)
	{
	V3f n;
	n = cal_norm(boundary_face[bdn_num]);
	if (ff.find(boundary_face[bdn_num]) != ff.end()) n = n * -1;
	normal_boundary_face_v.push_back(n);
	}
}


VertexHandle HexV3fMesh::add_vertex_new(const V3f& _p)
{
	vertices_.push_back(_p);
	neighbor_v.push_back(std::set<EdgeHandle, compare_OVM>());
	neighbor_v_f.push_back(std::set<FaceHandle, compare_OVM>());
	neighbor_v_c.push_back(std::set<CellHandle, compare_OVM>());
	return VertexHandle(vertices_.size() - 1);
}

FaceHandle HexV3fMesh::add_face(const std::vector<VertexHandle>& v_ve) {
	//add face and oppo edge
	Face e=Face();
	e.vs_f = v_ve;
	faces_.push_back(e);
	//link face index
	return FaceHandle(faces_.size()-1);
}

CellHandle HexV3fMesh::add_cell(const std::vector<VertexHandle>& v_ve) {
	//add edge and link relation v-e
	std::vector<EdgeHandle> e_handle(12);
	for (uint16_t i = 0; i < 12; ++i) {
		Edge temp_e(v_ve[EDGE_LINK_V_INDEX[i * 2]],
					v_ve[EDGE_LINK_V_INDEX[i * 2 + 1]]);
		std::vector<Edge>::iterator it;
		// TODO:maybe can change it later
		// std::cout << temp_e.getStr1() << std::endl;
		if (unEmap[temp_e.getStr1()] || unEmap[temp_e.getStr2()]) {
			e_handle[i] = EdgeHandle(unEdgeIndexMap[temp_e.getStr1()]);
		}
		else {
			//don't exist
			edges_.push_back(temp_e);
			unEmap[temp_e.getStr1()] = true;
			unEmap[temp_e.getStr2()] = true;
			unEdgeIndexMap[temp_e.getStr1()] = EdgeHandle(edges_.size() - 1);
			unEdgeIndexMap[temp_e.getStr2()] = EdgeHandle(edges_.size() - 1);
			neighbor_e.push_back(std::set<FaceHandle, compare_OVM>());
			e_handle[i] = EdgeHandle(edges_.size() - 1);
			neighbor_v[temp_e.from_h].insert(e_handle[i]);
			neighbor_v[temp_e.to_h].insert(e_handle[i]);
		}
#if 0
		if ((it = std::find(edges_.begin(), edges_.end(), temp_e)) != edges_.end()) {
			//has exist
			e_handle[i] = EdgeHandle(it - edges_.begin());
		}
		else {
			//don't exist
			edges_.push_back(temp_e);
			neighbor_e.push_back(std::set<FaceHandle, compare_OVM>());
			e_handle[i] = EdgeHandle(edges_.size()-1);
			neighbor_v[temp_e.from_h].insert(e_handle[i]);
			neighbor_v[temp_e.to_h].insert(e_handle[i]);
		}
#endif 
	}
	//add face and link relation f-e
	std::vector<FaceHandle> f_handle(6);
	for (uint16_t i = 0; i < 6; ++i) {
		std::vector<EdgeHandle> edge_vec = {
			e_handle[FACE_LINK_E_INDEX[i * 4]],
			e_handle[FACE_LINK_E_INDEX[i * 4 + 1]],
			e_handle[FACE_LINK_E_INDEX[i * 4 + 2]],
			e_handle[FACE_LINK_E_INDEX[i * 4 + 3]]
		};
		std::vector<VertexHandle> v_vec = {
			v_ve[FACE_LINK_V_INDEX[i * 4]],
			v_ve[FACE_LINK_V_INDEX[i * 4 + 1]],
			v_ve[FACE_LINK_V_INDEX[i * 4 + 2]],
			v_ve[FACE_LINK_V_INDEX[i * 4 + 3]]
		};
		Face face(edge_vec, v_vec);
		std::vector<Face>::iterator it;

		if (unFmap[face.getStr()]) {
			//f_handle[i] = FaceHandle(it - faces_.begin());
			f_handle[i] = FaceHandle(unFaceIndexMap[face.getStr()]);
		}
		else {
			//don't exist
			faces_.push_back(face);
			unFmap[face.getStr()] = true;
			unFaceIndexMap[face.getStr()] = FaceHandle(faces_.size() - 1);
			neighbor_f.push_back(std::set<CellHandle, compare_OVM>());
			f_handle[i] = FaceHandle(faces_.size() - 1);
			for (uint16_t arr_i = 0; arr_i < 4; ++arr_i) {
				neighbor_e[edge_vec[arr_i]].insert(f_handle[i]);
				neighbor_v_f[v_vec[arr_i]].insert(f_handle[i]);
			}
		}
#if 0
		if ((it = std::find(faces_.begin(), faces_.end(), face)) != faces_.end()) {
			f_handle[i] = FaceHandle(it - faces_.begin());
		}
		else {
			//don't exist
			faces_.push_back(face);
			neighbor_f.push_back(std::set<CellHandle, compare_OVM>());
			f_handle[i] = FaceHandle(faces_.size()-1);
			for (uint16_t arr_i = 0; arr_i < 4; ++arr_i) {
				neighbor_e[edge_vec[arr_i]].insert(f_handle[i]);
				neighbor_v_f[v_vec[arr_i]].insert(f_handle[i]);
			}
		}
#endif
	}
	//add cell
	cells_.push_back(Cell(f_handle, v_ve, e_handle));
	//f-c link relationship
	for (uint16_t arr_i = 0; arr_i < 6; ++arr_i) {
		neighbor_f[f_handle[arr_i]].insert(CellHandle(cells_.size()-1));
	}
	//v_c link relationship
	for (uint16_t arr_i = 0; arr_i < 8; ++arr_i) {
		this->neighbor_v_c[v_ve[arr_i]].insert(CellHandle(cells_.size() - 1));
	}
	return CellHandle(cells_.size() - 1);
}

CellHandle HexV3fMesh::add_cell(const std::vector<FaceHandle>& f_ve) {
	//add cell

	//add link relationship and f-cell
	return CellHandle();
}

void HexV3fMesh::add_mesh(HexV3fMesh * mesh)
{
	for (int i = 0; i < mesh->cells_.size(); ++i)
	{
		std::vector<VertexHandle> temp_ver(8);
		V3f temp_cen;
		for (int j = 0; j < mesh->cells_[i].vertex_.size(); ++j)
		{
			temp_ver[j] = add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[j]]);
			temp_cen = temp_cen + mesh->vertices_[mesh->cells_[i].vertex_[j]];
		}
		temp_cen = temp_cen / 8;
		double min = 999;
		for (int i = 0; i < cell_center.size(); ++i)
		{
			double dis = distance(temp_cen, cell_center[i]);
			if (dis < min)
				min = dis;
		}
		if (min > 0.05)
			add_cell(temp_ver);
		/*if(find(cell_center.begin(), cell_center.end(), temp_cen) == cell_center.end())
			add_cell(temp_ver);*/
	}

}

void HexV3fMesh::examine_crease()
{
	uint16_t vec_num = vertices_.size();
	for (uint16_t i = 0; i < vec_num; i++)
	{
		if (is_bdy(VertexHandle(i)))
		{
			if (neighbor_v[VertexHandle(i)].size() == 3)
			{
				v_c_crease.push_back(vertices_[VertexHandle(i)]);
			}
			if (neighbor_v[VertexHandle(i)].size() >= 4)
			{
				v_crease.push_back(vertices_[VertexHandle(i)]);
			}
		}
	}
}

bool HexV3fMesh::is_singular(EdgeHandle _e)
{
	int count = neighbor_e[_e].size();
	if (is_bdy(_e))
	{
		if (count == REGULAR_EDGE_BDY_N)
			return false;
		else
			return true;
	}
	else
	{
		if (count == REGULAR_EDGE_INNER_N)
			return false;
		else
			return true;
	}
}

bool HexV3fMesh::is_singular(VertexHandle _v)
{
	int count = neighbor_v[_v].size();
	if (is_bdy(_v))
	{
		if (count == REGULAR_VERTEX_BDY_N)
			return false;
		else
			return true;
	}
	else
	{
		if (count == REGULAR_VERTEX_INNER_N)
			return false;
		else
			return true;
	}
}

bool HexV3fMesh::is_f_in_c(FaceHandle _f, CellHandle _c)
{
	std::vector<FaceHandle> c_f = cells_[_c].faces_;
	for (std::vector<FaceHandle>::iterator c_f_it = c_f.begin();c_f_it != c_f.end();c_f_it++)
	{
		if (_f == *c_f_it)
			return true;
	}
	return false;
}

bool HexV3fMesh::is_e_in_f(EdgeHandle _e, FaceHandle _f)
{
	std::vector<EdgeHandle> f_e = faces_[_f].edges_;
	for (std::vector<EdgeHandle>::iterator f_e_it = f_e.begin();f_e_it != f_e.end();f_e_it++)
	{
		if (_e == *f_e_it)
			return true;
	}
	return false;
}

bool HexV3fMesh::is_e_in_c(EdgeHandle _e, CellHandle _c)
{
	std::vector<FaceHandle> c_f = cells_[_c].faces_;
	for (std::vector<FaceHandle>::iterator c_f_it = c_f.begin();c_f_it != c_f.end();c_f_it++)
	{
		if (is_e_in_f(_e, *c_f_it))
			return true;
	}
	return false;
}

bool HexV3fMesh::is_v_in_c(VertexHandle v, CellHandle c)
{
	for (int i = 0; i < cells_[c].vertex_.size(); ++i)
	{
		if (v == cells_[c].vertex_[i])
			return true;
	}
	return false;
}

bool HexV3fMesh::is_two_v_in_same_edge(VertexHandle v1, VertexHandle v2)
{
	for (int i = 0; i < edges_.size(); ++i)
	{
		if (v1 == edges_[i].from_h&&v2 == edges_[i].to_h)
			return true;
		else if (v1 == edges_[i].to_h&&v2 == edges_[i].from_h)
			return true;
	}
	return false;
}

void HexV3fMesh::examine_singular()
{
	v3f_singular.swap(std::vector<V3f>());
	uint16_t vec_num = vertices_.size();
	for (uint16_t i = 0; i < vec_num; i++)
	{
		std::set<EdgeHandle, compare_OVM> e_v = neighbor_v[VertexHandle(i)];
		std::vector<EdgeHandle> bd_e_v;
		if (is_bdy(VertexHandle(i)))
		{
			if (neighbor_v_c[VertexHandle(i)].size() >= 4)//�߽ǵ㲻���������
			{
				for (std::set<EdgeHandle, compare_OVM>::iterator e_v_it = e_v.begin();e_v_it != e_v.end();e_v_it++)
				{
					if (is_bdy(*e_v_it))
						bd_e_v.push_back(*e_v_it);
				}
				if (bd_e_v.size() != 4)
				{
					//V3f v = vertices_[VertexHandle(i)];
					v3f_singular.push_back(vertices_[VertexHandle(i)]);
				}
			}
			else
				continue;
		/*	if (neighbor_v[VertexHandle(i)].size() >= 6)
			{
				V3f v = vertices_[VertexHandle(i)];
				v3f_singular.push_back(vertices_[VertexHandle(i)]);
			}*/
		}
		else
		{
			if (neighbor_v[VertexHandle(i)].size() != 6)
			{
				//V3f v = vertices_[VertexHandle(i)];
				v3f_singular.push_back(vertices_[VertexHandle(i)]);
			}
		}
	}
}

int HexV3fMesh::val(VertexHandle v1, VertexHandle v2, bool _is_bdy)
{
	if (_is_bdy == true)
	{
		std::set<FaceHandle, compare_OVM> v1_c = neighbor_v_f[v1];
		std::set<FaceHandle, compare_OVM> v1_c_bdy;
		for (auto v1_c_it = v1_c.begin();v1_c_it != v1_c.end();v1_c_it++)
		{
			if (is_bdy(*v1_c_it))
				v1_c_bdy.insert(*v1_c_it);
		}
		std::set<FaceHandle, compare_OVM> v2_c = neighbor_v_f[v2];
		std::set<FaceHandle, compare_OVM> v2_c_bdy;
		for (auto v2_c_it = v2_c.begin();v2_c_it != v2_c.end();v2_c_it++)
		{
			if (is_bdy(*v2_c_it))
				v2_c_bdy.insert(*v2_c_it);
		}
		std::vector<FaceHandle> vc;
		std::set_intersection(v1_c_bdy.begin(), v1_c_bdy.end(), v2_c_bdy.begin(), v2_c_bdy.end(), std::inserter(vc, vc.begin()));
		//std::cout << "vc.size():" << vc.size() << std::endl;
		return vc.size();
	}
	else
	{
		std::set<CellHandle, compare_OVM> v1_c = neighbor_v_c[v1];
		std::set<CellHandle, compare_OVM> v2_c = neighbor_v_c[v2];
		std::vector<CellHandle> vc;
		std::set_intersection(v1_c.begin(), v1_c.end(), v2_c.begin(), v2_c.end(), std::inserter(vc, vc.begin()));
		//std::cout << "vc.size():" << vc.size() << std::endl;
		return vc.size();
	}
	
}

int HexV3fMesh::val(VertexHandle v , bool _is_bdy)
{
	if (_is_bdy == true)
	{
		std::set<FaceHandle, compare_OVM> v_c = neighbor_v_f[v];
		std::set<FaceHandle, compare_OVM> v_c_bdy;
		for (auto v_c_it = v_c.begin();v_c_it != v_c.end();v_c_it++)
		{
			if (is_bdy(*v_c_it))
				v_c_bdy.insert(*v_c_it);
		}
		return v_c_bdy.size();
	}
	else
	{
		std::set<CellHandle, compare_OVM> v_c = neighbor_v_c[v];
		return v_c.size();
	}
}

int HexV3fMesh::dim(VertexHandle v1, VertexHandle v2)
{
	std::set<EdgeHandle, compare_OVM> v1_e = neighbor_v[v1];
	//std::set<EdgeHandle, compare_OVM> v2_e = neighbor_v[v2];
	std::set<VertexHandle, compare_OVM> v1_e_v;
	std::set<FaceHandle, compare_OVM> v1_f = neighbor_v_f[v1];
	//std::set<FaceHandle, compare_OVM> v2_f = neighbor_v_f[v2];
	std::set<VertexHandle, compare_OVM> v1_f_v;
	std::set<CellHandle, compare_OVM> v1_c = neighbor_v_c[v1];
	//std::set<CellHandle, compare_OVM> v2_c = neighbor_v_c[v2];
	std::set<VertexHandle, compare_OVM> v1_c_v;
	for (std::set<EdgeHandle, compare_OVM>::iterator it1 = v1_e.begin(); it1 != v1_e.end();it1++)
	{
		v1_e_v.insert(edges_[*it1].from_h);
		v1_e_v.insert(edges_[*it1].to_h);
	}
	for (std::set<FaceHandle, compare_OVM>::iterator it2 = v1_f.begin(); it2 != v1_f.end();it2++)
	{
		std::vector<VertexHandle> vf = faces_[*it2].vs_f;
		for (std::vector<VertexHandle>::iterator it = vf.begin();it != vf.end();it++)
		{
			v1_f_v.insert(*it);
		}
	}
	for (std::set<CellHandle, compare_OVM>::iterator it3 = v1_c.begin();it3 != v1_c.end();it3++)
	{
		std::vector<VertexHandle> vc = cells_[*it3].vertex_;
		for (std::vector<VertexHandle>::iterator it = vc.begin();it != vc.end();it++)
		{
			v1_c_v.insert(*it);
		}
	}
	if (v1 == v2)
		return 0;
	else if (std::find(v1_e_v.begin(), v1_e_v.end(), v2) != v1_e_v.end())
		return 1;
	else if (std::find(v1_f_v.begin(), v1_f_v.end(), v2) != v1_f_v.end())
		return 2;
	else if (std::find(v1_c_v.begin(), v1_c_v.end(), v2) != v1_c_v.end())
		return 3;
	else
	{
		std::cout << "error" << std::endl;
		return 9;
	}
}

void HexV3fMesh::examine_bdfv()
{
	boundary_face_v.swap(std::vector<std::vector<V3f>>());
	boundary_face.swap(std::vector<FaceHandle>());
	boundary_face_VID.swap(std::vector<std::vector<int>>());
	//boundary_face.clear();
	//boundary_face_v.clear();//ÿһ�μ���ǰ�������һ�α�����ֵ����Ȼ��ͼ����
	for (int bdfv_num = 0;bdfv_num < faces_.size();bdfv_num++)
	{
		if (is_bdy(FaceHandle(bdfv_num)))
		{
			std::vector<V3f> bdfv_1;
			std::vector<int> bdfv_id;
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[0]]);
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[1]]);
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[2]]);
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[3]]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[0]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[1]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[2]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[3]);
			boundary_face_v.push_back(bdfv_1);
			boundary_face.push_back(FaceHandle(bdfv_num));
			boundary_face_VID.push_back(bdfv_id);
		 /* V3f a;V3f b;V3f n;V3f temp;
			a.x = vertices_[faces_[bdfv_num].vs_f[1]].x - vertices_[faces_[bdfv_num].vs_f[0]].x;
			a.y = vertices_[faces_[bdfv_num].vs_f[1]].y - vertices_[faces_[bdfv_num].vs_f[0]].y;
			a.z = vertices_[faces_[bdfv_num].vs_f[1]].z - vertices_[faces_[bdfv_num].vs_f[0]].z;
			b.x = vertices_[faces_[bdfv_num].vs_f[3]].x - vertices_[faces_[bdfv_num].vs_f[0]].x;
			b.y = vertices_[faces_[bdfv_num].vs_f[3]].y - vertices_[faces_[bdfv_num].vs_f[0]].y;
			b.z = vertices_[faces_[bdfv_num].vs_f[3]].z - vertices_[faces_[bdfv_num].vs_f[0]].z;
			n.x = a.y*b.z - a.z*b.y;
			n.y = -(a.x*b.z - a.z*b.x);
			n.z = a.x*b.y - a.y*b.x;
			temp.x =  vertices_[faces_[bdfv_num].vs_f[0]].x - center.x;
			temp.y =  vertices_[faces_[bdfv_num].vs_f[0]].y - center.y;
			temp.z =  vertices_[faces_[bdfv_num].vs_f[0]].z - center.z;
			double cos;
			cos = (n.x*temp.x + n.y*temp.y + n.z*temp.z) / (n.norm()*temp.norm());
			if (cos >= 0)
			{
				normal_boundary_face_v.push_back(n);
			}
			else
			{
				normal_boundary_face_v.push_back(V3f((n.x),(n.y),(n.z)));
			}*/
		}
	}
}

void HexV3fMesh::examine_bdfv(int)
{
	boundary_face_v.swap(std::vector<std::vector<V3f>>());
	boundary_face.swap(std::vector<FaceHandle>());
	boundary_face_VID.swap(std::vector<std::vector<int>>());
	for (int bdfv_num = 0; bdfv_num < faces_.size(); bdfv_num++)
	{
		if (is_bdy(1,FaceHandle(bdfv_num)))
		{
			std::vector<V3f> bdfv_1;
			std::vector<int> bdfv_id;
			
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[0]]);
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[1]]);
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[2]]);
			bdfv_1.push_back(vertices_[faces_[bdfv_num].vs_f[3]]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[0]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[1]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[2]);
			bdfv_id.push_back(faces_[bdfv_num].vs_f[3]);
			boundary_face_v.push_back(bdfv_1);
			boundary_face.push_back(FaceHandle(bdfv_num));
			boundary_face_VID.push_back(bdfv_id);
		}
	}
}

void HexV3fMesh::cal_bdnorm()
{
	normal_boundary_face_v.swap(std::vector<V3f>());
	for (int bdn_num = 0;bdn_num < boundary_face.size();bdn_num++)
	{
		V3f n;
		n = cal_norm(boundary_face[bdn_num]);
		normal_boundary_face_v.push_back(n);
	}
}

void HexV3fMesh::cal_bdnorm(std::vector<V3f> &vn)
{
	normal_boundary_face_v.swap(std::vector<V3f>());
	for (int bdn_num = 0; bdn_num < boundary_face.size(); bdn_num++)
	{
		V3f n;
		std::set<CellHandle, compare_OVM> fc = neighbor_f[boundary_face[bdn_num]];
		CellHandle c = *fc.begin();
		n = vn[c];
		normal_boundary_face_v.push_back(n);
	}
}

V3f HexV3fMesh::cal_norm(FaceHandle f)
{
	V3f a; V3f b; V3f n;
	std::vector<VertexHandle> fv = faces_[f].vs_f;
	a = vertices_[fv[1]] - vertices_[fv[0]];
	b = vertices_[fv[3]] - vertices_[fv[0]];
	n = a % b;
	n = n.normalize();
	std::set<CellHandle, compare_OVM> fc = neighbor_f[f];
	CellHandle fcc = *(fc.begin());
	std::vector<VertexHandle> fcv = cells_[fcc].vertex_;
	V3f fc_center;
	for (std::vector<VertexHandle>::iterator fcv_it = fcv.begin();fcv_it != fcv.end();fcv_it++)
	{
		fc_center = fc_center + vertices_[*fcv_it];
	}
	fc_center = fc_center / 8;
	V3f c = fc_center - vertices_[fv[0]];
	double temp = n.x*c.x + n.y*c.y + n.z*c.z;
	if (temp > 0)
		n = n * -1;
	return n;
}



/**************************************************
@brief   : calc max radius of the mesh
		   calc center point of the mesh
		   calc bounding box
@author  : none
@input   ��none
@output  ��none
@time    : none
**************************************************/
void HexV3fMesh::cal_cen(){
	double cen_x = 0, cen_y = 0, cen_z = 0;
	double max = 0;
	double max_x = std::numeric_limits<double>::lowest(), max_y = std::numeric_limits<double>::lowest(), max_z = std::numeric_limits<double>::lowest();
	double min_x = std::numeric_limits<double>::max(), min_y = std::numeric_limits<double>::max(), min_z = std::numeric_limits<double>::max();
	int num = vertices_.size();
	for (std::vector<V3f>::iterator it = vertices_.begin(); it != vertices_.end(); it++){
		cen_x += (*it).x;
		cen_y += (*it).y;
		cen_z += (*it).z;
		num++;
	}
	center.x = cen_x / num;
	center.y = cen_y / num;
	center.z = cen_z / num;
	for (std::vector<V3f>::iterator it = vertices_.begin(); it != vertices_.end(); it++) {
		double maxr = ((*it) - center).norm();
		average_r += maxr;
		if (maxr > max) {
			max = maxr;
		}
		if ((*it).x > max_x) max_x = (*it).x;
		if ((*it).x < min_x) min_x = (*it).x;
		if ((*it).y > max_y) max_y = (*it).y;
		if ((*it).y < min_y) min_y = (*it).y;
		if ((*it).z > max_z) max_z = (*it).z;
		if ((*it).z < min_z) min_z = (*it).z;
	}
	average_r /= num;
	r = max; // calc max radius of the mesh
	// calc bounding box
	bbMax = V3f(max_x, max_y, max_z);
	bbMin = V3f(min_x, min_y, min_z);
}

void HexV3fMesh::cal_cen1()
{
	double max = 0;
	double max_x = -999, max_y = -999, max_z = -999;
	double min_x = 999, min_y = 999, min_z = 999;

	V3f centers(0, 0, 0);
	int num = 0;
	for (int i = 0; i < cells_.size(); ++i)
	{
		for (int j = 0; j < cells_[i].vertex_.size(); ++j)
		{
			centers = centers + vertices_[cells_[i].vertex_[j]];
			num++;
		}
	}
	center = centers / num;

	for (int i = 0; i < cells_.size(); ++i)
	{
		for (int j = 0; j < cells_[i].vertex_.size(); ++j)
		{
			V3f v = vertices_[cells_[i].vertex_[j]];
			double maxr = (v - center).norm();
			average_r += maxr;
			if (maxr > max)
				max = maxr;
			if (v.x > max_x) max_x = v.x;
			if (v.x < min_x) min_x = v.x;
			if (v.y > max_y) max_y = v.y;
			if (v.y < min_y) min_y = v.y;
			if (v.z > max_z) max_z = v.z;
			if (v.z < min_z) min_z = v.z;
		}
	}
	average_r /= num;
	r = max;
	bbMax = V3f(max_x, max_y, max_z);
	bbMin = V3f(min_x, min_y, min_z);

	for (std::vector<V3f>::iterator it = vertices_.begin(); it != vertices_.end(); it++)
	{
		double maxr = ((*it) - center).norm();
		average_r += maxr;
		if (maxr > max)
			max = maxr;
		if ((*it).x > max_x) max_x = (*it).x;
		if ((*it).x < min_x) min_x = (*it).x;
		if ((*it).y > max_y) max_y = (*it).y;
		if ((*it).y < min_y) min_y = (*it).y;
		if ((*it).z > max_z) max_z = (*it).z;
		if ((*it).z < min_z) min_z = (*it).z;
	}
}

void HexV3fMesh::cal_cell_cen()
{
	cell_center.swap(std::vector<V3f>());
	//cell_center.clear();//ÿһ�μ���ǰ�������һ�α�����ֵ����Ȼ��ͼ����
	for (std::vector<Cell>::iterator c_it = cells_.begin();c_it != cells_.end();c_it++)
	{
		V3f center;
		int num = (*c_it).vertex_.size();
		for (std::vector<VertexHandle>::iterator v_it = (*c_it).vertex_.begin();v_it != (*c_it).vertex_.end();v_it++)
		{
			center.x += vertices_[*v_it].x;
			center.y += vertices_[*v_it].y;
			center.z += vertices_[*v_it].z;
		}
		center.x /= num;
		center.y /= num;
		center.z /= num;
		cell_center.push_back(center);
	}
	for (int i = 0; i < cells_.size(); i++)
	{
		double temp_r = 0;
		for (int j = 0; j < cells_[i].vertex_.size(); j++)
		{
			V3f temp_v = vertices_[cells_[i].vertex_[j]];
			double temp1 = pow(cell_center[i].x - temp_v.x, 2);
			temp_r += sqrt(pow(cell_center[i].x - temp_v.x, 2) + pow(cell_center[i].y - temp_v.y, 2) +
				pow(cell_center[i].z - temp_v.z, 2));
		}
		temp_r /= cells_[i].vertex_.size();
		cell_r.push_back(temp_r);
	}
}

void HexV3fMesh::cal_mami_ed()
{
	double max = 0, min = 999;
	for (std::vector<Edge>::iterator e_it = edges_.begin();e_it != edges_.end();e_it++)
	{
		/*double dis = V3f(vertices_[(*e_it).from_h].x - vertices_[(*e_it).to_h].x,
			vertices_[(*e_it).from_h].y - vertices_[(*e_it).to_h].y,
			vertices_[(*e_it).from_h].z - vertices_[(*e_it).to_h].z).norm();*/
		double dis = (vertices_[(*e_it).from_h] - vertices_[(*e_it).to_h]).norm();
		if (dis > max)
			max = dis;
		if (dis < min)
			min = dis;
	}
	minEdgeLen = min;
	maxEdgeLen = max;
}

//std::vector<CellHandle> HexV3fMesh::incident_cell(FaceHandle _f)
//{
//	std::vector<CellHandle> cc;
//	int c_num = 0;
//	for (std::vector<Cell>::iterator c_it = cells_.begin();c_it != cells_.end(); c_it++)
//	{
//		std::vector<FaceHandle> tempf = (*c_it).faces_;
//		if (find(tempf.begin(), tempf.end(), _f) != tempf.end())
//		{
//			cc.push_back(CellHandle(c_num));
//		}
//		c_num++;
//	}
//	return cc;
//}

VertexHandle HexV3fMesh::find_op_vertex(VertexHandle _v, FaceHandle _f)
{
	std::vector<EdgeHandle> fe = faces_[_f].edges_;
	std::set<EdgeHandle, compare_OVM> ve = neighbor_v[_v];
	std::vector<VertexHandle> temp;
	temp.push_back(_v);
	for (std::vector<EdgeHandle>::iterator fe_it = fe.begin();fe_it != fe.end();fe_it++)
	{
		if (find(ve.begin(), ve.end(), *fe_it) != ve.end())
		{
			if (edges_[*fe_it].from_h == _v)
				temp.push_back(edges_[*fe_it].to_h);
			else
				temp.push_back(edges_[*fe_it].from_h);
		}
	}
	std::vector<VertexHandle> fv = faces_[_f].vs_f;
	for (std::vector<VertexHandle>::iterator fv_it = fv.begin();fv_it != fv.end();fv_it++)
	{
		if (find(temp.begin(), temp.end(), *fv_it) == temp.end())
			return (*fv_it);
	}
}

bool HexV3fMesh::is_two_e_in_same_face(EdgeHandle e1, EdgeHandle e2)
{
	std::set<FaceHandle> e1_f = neighbor_e_bdy(e1);
	std::set<FaceHandle> e2_f = neighbor_e_bdy(e2);
	for (std::set<FaceHandle, compare_OVM>::iterator e1_f_it = e1_f.begin();e1_f_it != e1_f.end();e1_f_it++)
	{
		for (std::set<FaceHandle, compare_OVM>::iterator e2_f_it = e2_f.begin();e2_f_it != e2_f.end();e2_f_it++)
		{
			if (*e1_f_it == *e2_f_it)
				return true;
		}
	}
	return false;
}

bool HexV3fMesh::is_two_e_in_same_cell(EdgeHandle e1, EdgeHandle e2)
{
	std::set<CellHandle> e1_c = neighbor_e_c(e1);
	std::set<CellHandle> e2_c = neighbor_e_c(e2);
	for (std::set<CellHandle>::iterator e1_c_it = e1_c.begin();e1_c_it != e1_c.end();e1_c_it++)
	{
		for (std::set<CellHandle>::iterator e2_c_it = e2_c.begin();e2_c_it != e2_c.end();e2_c_it++)
		{
			if (*e1_c_it == *e2_c_it)
				return true;
		}
	}
	return false;
}

bool HexV3fMesh::is_two_f_in_same_cell(FaceHandle f1, FaceHandle f2)
{
	std::set<CellHandle, compare_OVM> f_c1 = neighbor_f[f1];
	std::set<CellHandle, compare_OVM> f_c2 = neighbor_f[f2];
	for (std::set<CellHandle, compare_OVM>::iterator f_c1_it = f_c1.begin();f_c1_it != f_c1.end();f_c1_it++)
	{
		for (std::set<CellHandle, compare_OVM>::iterator f_c2_it = f_c2.begin();f_c2_it != f_c2.end();f_c2_it++)
		{
			if (*f_c1_it == *f_c2_it)
				return true;
		}
	}
	return false;
}

CellHandle HexV3fMesh::delete_cell(CellHandle cellHandle)
{
	//��������ɾ��Ԫ��

	//��ɾ���ڽӹ�ϵ ��ɾ��Ԫ��
	std::vector<FaceHandle> f_vec = cells_[cellHandle].faces_;
	std::set<EdgeHandle, compare_OVM> e_set;
	std::set<FaceHandle, compare_OVM> f_delete_set;
	std::set<EdgeHandle, compare_OVM> e_delete_set;
	std::set<VertexHandle, compare_OVM> v_delete_set;
	for each (FaceHandle f in f_vec) {
		//���ڽ�
		neighbor_f[f].erase(cellHandle);
		for each (EdgeHandle e in faces_[f].edges_) {
			e_set.insert(e);

		}
		if (neighbor_f[f].size() == 0) {
			f_delete_set.insert(f);
			//ɾ��v-f
			for each (VertexHandle v in faces_[f].vs_f) {
				neighbor_v_f[v].erase(f);
			}
		}
	}

	//���ڽ�
	for each (EdgeHandle e in e_set) {
		//neighbor_e_c[e].erase(cellHandle);
		//TODO:��������� doublet
		if (neighbor_e[e].size() == 2) {
			e_delete_set.insert(e);
		}
		for each (FaceHandle f in f_delete_set) {
			neighbor_e[e].erase(f);
		}

	}
	//���ڽ�ɾ��
	for each(VertexHandle v in cells_[cellHandle].vertex_) {
		neighbor_v_c[v].erase(cellHandle);

		std::set<EdgeHandle, compare_OVM>& edge_set = neighbor_v[v];

		//ֻ�нǵ����ɾ��
		if (edge_set.size() == 3) {
			v_delete_set.insert(v);
		}

		for (std::set<EdgeHandle, compare_OVM>::iterator edge_iter = edge_set.begin(); edge_iter != edge_set.end();) {
			if (e_delete_set.find(*edge_iter) != e_delete_set.end()) {
				edge_set.erase(edge_iter++);
			}
			else {
				++edge_iter;
			}
		}

	}

	//ɾ��Ԫ�� c->f->e->v
	cells_[cellHandle].is_valid = false;

	for each (FaceHandle f in f_delete_set)
	{
		faces_[f].is_valid = false;
	}
	for each(EdgeHandle e in e_delete_set) {
		edges_[e].is_valid = false;
	}
	for each (VertexHandle v in v_delete_set)
	{
		vertices_[v].is_valid = false;
	}
	//TODO:���ܻ����ط��ȽϺ� ̫��ʱ ����
	examine_bdy();
	return cellHandle;
}

void HexV3fMesh::checksingualrface()
{
	int face_num = faces_.size();
	int bdy_face = 0;
	for (int i = 0; i < face_num; i++)//����Ϊ��λ����
	{
		if (is_bdy(1, FaceHandle(i)))
		{
			++bdy_face;
		}
	}
	singularface.resize(bdy_face, 0);
	bdy_face = 0;
	for (int i = 0; i < face_num; i++)//����Ϊ��λ����
	{
		if (is_bdy(1, FaceHandle(i)))
		{
			std::vector<VertexHandle> fv = faces_[i].vs_f;
			int sin_num = 0;
			for (int j = 0; j < fv.size(); ++j)
			{
				std::set<EdgeHandle, compare_OVM> fve = neighbor_v[fv[j]];
				int fve_num = 0;
				for (auto fve_it = fve.begin(); fve_it != fve.end(); ++fve_it)
				{
					if (is_bdy(1, *fve_it))
						fve_num++;
				}
				if (fve_num != 4)
					sin_num++;
			}
			if (sin_num == 1)
				singularface[bdy_face] = 1;
			else if(sin_num > 1)
				singularface[bdy_face] = 2;
			++bdy_face;
		}
	}	
}

void HexV3fMesh::smooth_mesh()
{
	std::vector<V3f> newV = vertices_;
	for (int vi = 0; vi < vertices_.size(); ++vi)
	{
		V3f tempV(0, 0, 0);
		int tempn = 0;
		if (is_bdy(VertexHandle(vi)))
		{
			std::set<EdgeHandle, compare_OVM> ve = neighbor_v[vi];
			if (ve.size() == 0)
			{
				continue;
			}
			for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
			{
				if (is_bdy(*ve_it))
				{
					if (edges_[*ve_it].from_h == VertexHandle(vi))
					{
						tempV = tempV + vertices_[edges_[*ve_it].to_h];
						tempn++;
					}
					else
					{
						tempV = tempV + vertices_[edges_[*ve_it].from_h];
						tempn++;
					}
				}
			}
			if (!is_cs(VertexHandle(vi)))
			{
				tempV = tempV / tempn;
				newV[vi] = tempV;
			}	
		}
		/*else
		{
			std::set<EdgeHandle, compare_OVM> ve = neighbor_v[vi];
			if (ve.size() == 0)
			{
				continue;
			}
			for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
			{
				if (edges_[*ve_it].from_h == VertexHandle(vi))
				{
					tempV = tempV + vertices_[edges_[*ve_it].to_h];
					tempn++;
				}
				else
				{
					tempV = tempV + vertices_[edges_[*ve_it].from_h];
					tempn++;
				}
			}
			tempV = tempV / tempn;
			newV[vi] = tempV;
		}*/
	}
	//vertices_ = newV;
	////�ڲ�����Ҫ������һ��
	//for (int vi = 0; vi < vertices_.size(); ++vi)
	//{
	//	V3f tempV(0, 0, 0);
	//	int tempn = 0;
	//	if (!is_bdy(VertexHandle(vi)))
	//	{
	//		std::set<EdgeHandle, compare_OVM> ve = neighbor_v[vi];
	//		if (ve.size() == 0)
	//		{
	//			continue;
	//		}
	//		for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	//		{
	//			if (edges_[*ve_it].from_h == VertexHandle(vi))
	//			{
	//				tempV = tempV + vertices_[edges_[*ve_it].to_h];
	//				tempn++;
	//			}
	//			else
	//			{
	//				tempV = tempV + vertices_[edges_[*ve_it].from_h];
	//				tempn++;
	//			}
	//		}
	//		tempV = tempV / tempn;
	//		newV[vi] = tempV;
	//	}
	//}
	vertices_ = newV;
}

QuadV3fMesh HexV3fMesh::change2Quad()
{
	QuadV3fMesh *quad = new QuadV3fMesh();
	std::vector<int> check(vertices_.size(), -1);
	for (int i = 0; i < vertices_.size(); ++i)
	{
		if (is_bdy(VertexHandle(i)))
		{
			VertexHandle v = quad->add_vertex(vertices_[i]);
			check[i] = v.idx();
		}
	}
	for (int i = 0; i < faces_.size(); ++i)
	{
		if (is_bdy(FaceHandle(i)))
		{
			std::vector<VertexHandle> fv(4);
			for (int j = 0; j < 4; ++j)
			{
				fv[j] = VertexHandle(check[faces_[i].vs_f[j]]);
			}
			quad->add_face(fv);
		}
	}
	quad->normal = normal_boundary_face_v;

	return *quad;
}


