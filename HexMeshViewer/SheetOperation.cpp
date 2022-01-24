#include "SheetOperation.h"

void BCHexahedron::PaddingSurface(std::vector<FaceHandle>& faces_surface, FaceHandle init_face)
{
	std::set<EdgeHandle, compare_OVM> edges_feature;
	
	for (uint8_t i = 0; i < 12; ++i) {
		for (uint32_t j = 0; j < vec_vec_feature_edge[i].size(); ++j) {
			edges_feature.insert((vec_vec_feature_edge[i])[j]);
		}
	}
	//四个方向填充
	std::stack<FaceHandle> stack_face;
	stack_face.push(init_face);
	
	//BFS
	faces_surface.push_back(stack_face.top());
	while (stack_face.size() != 0) {
		FaceHandle face_now = stack_face.top();
		stack_face.pop();
		std::vector<EdgeHandle> edges_ = _mesh->faces_[face_now].edges_;
		for (uint8_t i = 0; i < 4; ++i) {
			EdgeHandle edge = edges_[i];
			if (edges_feature.find(edge) != edges_feature.end()) continue;
			std::set<FaceHandle, compare_OVM> setf = _mesh->neighbor_e[edge];
			for (std::set<FaceHandle, compare_OVM>::iterator iter = setf.begin();
				iter != setf.end(); ++iter) {
				if (std::find(vec_surface_face.begin(), vec_surface_face.end(), *iter) != vec_surface_face.end()) {
					if (std::find(faces_surface.begin(), faces_surface.end(), *iter) == faces_surface.end()) {
						stack_face.push(*iter);
						faces_surface.push_back(stack_face.top());
					}
				}
			}
		}
	}

	//输出一下 天啦 又错了
	/*for (std::vector<FaceHandle>::iterator iter = faces_surface.begin(); iter != faces_surface.end(); ++iter) {
		Face f = _mesh->faces_[*iter];
		std::cout << "face:\t" << (int)(*iter).idx() << "\tver:\n\t" <<
			(int)f.vs_f[0] << ":(" << _mesh->vertices_[f.vs_f[0]].x << "," << _mesh->vertices_[f.vs_f[0]].y << "," << _mesh->vertices_[f.vs_f[0]].z << ")\n\t" <<
			(int)f.vs_f[1] << ":(" << _mesh->vertices_[f.vs_f[1]].x << "," << _mesh->vertices_[f.vs_f[1]].y << "," << _mesh->vertices_[f.vs_f[1]].z << ")\n\t" <<
			(int)f.vs_f[2] << ":(" << _mesh->vertices_[f.vs_f[2]].x << "," << _mesh->vertices_[f.vs_f[2]].y << "," << _mesh->vertices_[f.vs_f[2]].z << ")\n\t" <<
			(int)f.vs_f[3] << ":(" << _mesh->vertices_[f.vs_f[3]].x << "," << _mesh->vertices_[f.vs_f[3]].y << "," << _mesh->vertices_[f.vs_f[3]].z << ")\n";
	}*/
}

BCHexahedron::BCHexahedron(HexV3fMesh * _mesh, std::vector<BaseComplex>&  bc)
{
	this->_mesh = _mesh;

	sheet_bc_set = bc;
	//1.获取边界面 和 边界 单元
	
	//(边界面)
	for (uint32_t n = 0; n < bc.size(); ++n) {
		std::set<CellHandle, compare_OVM>& set_cell_bc = bc[n].getCellSet();
		std::set<FaceHandle, compare_OVM>& set_face_bc = bc[n].getFaceSet();
		for (std::set<CellHandle, compare_OVM>::iterator bc_cell_iter = set_cell_bc.begin();
			bc_cell_iter != set_cell_bc.end(); ++bc_cell_iter) {
			this->vec_cell.insert(*bc_cell_iter);
		}
		
		vec_surface_face.insert(vec_surface_face.end() , set_face_bc.begin() , set_face_bc.end());

	}
	std::sort(vec_surface_face.begin(), vec_surface_face.end(),compare_OVM());
	std::set<uint32_t> vec_need_remove;
	for (uint32_t f_i = 0; f_i < vec_surface_face.size()-1; ++f_i) {
		while (vec_surface_face[f_i + 1]==vec_surface_face[f_i]) {
			if (f_i == vec_surface_face.size() - 2) {
				break;
			}
			else {
				vec_need_remove.insert(f_i);
				vec_need_remove.insert(f_i + 1);
			}
			++f_i;
		}
		
	}
	//从尾向前删除
	for (std::set<uint32_t>::reverse_iterator uiter = vec_need_remove.rbegin();
		uiter != vec_need_remove.rend(); ++uiter) {
		vec_surface_face.erase(vec_surface_face.begin()+*uiter);
	}
	//(边界点)
	for (std::vector<FaceHandle>::iterator iter = vec_surface_face.begin(); iter!=vec_surface_face.end(); ++iter) {
		std::vector<VertexHandle> vs_f  = _mesh->faces_[*iter].vs_f;
		for (uint8_t i = 0; i < vs_f.size(); ++i) {
			set_surface_v.insert(vs_f[i]);
		}
	}

	//(边界标识)
	
	for (uint32_t surface_i = 0; surface_i < vec_surface_face.size(); ++surface_i) {
		std::set<CellHandle, compare_OVM> _2cell_set = _mesh->neighbor_f[vec_surface_face[surface_i]];
		for (std::set<CellHandle, compare_OVM>::iterator iter = _2cell_set.begin(); iter != _2cell_set.end(); ++iter) {
			if (vec_cell.find(*iter) != vec_cell.end()) {
				map_is_inner.insert(std::make_pair(*iter,false));
			}
			
		}
	}
	for (std::set<CellHandle, compare_OVM>::iterator cell_iter = vec_cell.begin(); cell_iter != vec_cell.end(); ++cell_iter) {
		if (map_is_inner.find(*cell_iter) == map_is_inner.end()) {
			map_is_inner.insert(std::make_pair(*cell_iter, true));
		}

	}

	is_init = true;
}

bool BCHexahedron::ExtracteSurfuceFeature()
{
	//if (!is_init) return false;

	//角点 角点单元确定
	//(寻找邻接三个边界面的点)
	//vec_corner_v.resize(8);
	for (std::set<VertexHandle, compare_OVM>::iterator iter = set_surface_v.begin(); iter!=set_surface_v.end(); ++iter) {
		std::set<FaceHandle, compare_OVM> set_face = _mesh->neighbor_v_f[*iter];
		uint8_t count_existing = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator f_iter = set_face.begin(); f_iter != set_face.end(); ++f_iter) {
			if (std::find(vec_surface_face.begin(), vec_surface_face.end(), *f_iter) != vec_surface_face.end()) {
				++count_existing;
			}
		}
		if (count_existing == 3) {
			vec_corner_v.push_back(*iter);
		}
	}
	if (vec_corner_v.size() != 8) {
		return false;
	}
	//(添加边界单元)
	vec_corner_cell.resize(8);
	std::vector<VertexHandle>::iterator iter_v;
	for (std::map<CellHandle, bool, compare_OVM>::iterator iter = map_is_inner.begin(); iter != map_is_inner.end(); ++iter) {
		if ((*iter).second)continue;
		std::vector<VertexHandle> vec = _mesh->cells_[(*iter).first].vertex_;
		for (uint8_t i = 0; i < vec.size(); ++i) {
			if ((iter_v = std::find(vec_corner_v.begin(), vec_corner_v.end(), vec[i])) != vec_corner_v.end()) {
				vec_corner_cell[iter_v - vec_corner_v.begin()]  = ((*iter).first);
				set_corner_cell.insert((*iter).first);
				//break;
			}
		}
	}
	

	//提取边界特征

	std::set<EdgeHandle, compare_OVM> edge_feature_temp; 

	uint8_t it_n = 0;


	for (std::vector<VertexHandle>::iterator it_v = vec_corner_v.begin(); it_v!=vec_corner_v.end(); ++it_v,++it_n) {
		
		
		VertexHandle now_vertex = *it_v;
		std::vector<EdgeHandle> vec_3_edge = _mesh->get_3edge_in_corner(now_vertex,vec_corner_cell[it_n]);
		
		
		//先找三个邻接边
		for (uint8_t i_e = 0; i_e < 3; ++i_e) {
			std::vector<EdgeHandle> edge_addin;
			EdgeHandle now_edge = vec_3_edge[i_e];
			if (edge_feature_temp.find(now_edge) != edge_feature_temp.end()) {
				continue;
			}
			CellHandle now_cell = vec_corner_cell[it_n];
			VertexHandle now_vertex_feature = now_vertex;
			while(true){
				now_vertex_feature = (now_vertex_feature == _mesh->edges_[now_edge].from_h) ?
					_mesh->edges_[now_edge].to_h : _mesh->edges_[now_edge].from_h;
				//把当前的边存入
				edge_addin.push_back(now_edge);

				//检查一下now_vertex是否到边界
				if (std::find(vec_corner_v.begin(), vec_corner_v.end(), now_vertex_feature) != vec_corner_v.end()) {
					//TODO
					break;
				}
				
				std::vector<EdgeHandle> vec_3_edge_now_v = _mesh->get_3edge_in_corner(now_vertex_feature, now_cell);

				//找下条边
				std::set<CellHandle, compare_OVM> temp_neibor_cell = _mesh->neighbor_v_c[now_vertex_feature];
				std::vector<CellHandle> intersection_vec;
				std::set_intersection(temp_neibor_cell.begin(),temp_neibor_cell.end(),
					vec_cell.begin() , vec_cell.end() , std::back_inserter(intersection_vec));
				uint8_t i_c;
				for (i_c = 0; i_c < intersection_vec.size(); ++i_c) {
					if (intersection_vec[i_c] != now_cell) {
						now_cell = intersection_vec[i_c];
						break;
					}
				}
				//其实```根本不会出现
				if (i_c == intersection_vec.size()) {
					//不存在下一个单元
					//TODO:
					break;
				}
				std::vector<EdgeHandle> vec_3_edge_temp = _mesh->get_3edge_in_corner(now_vertex_feature,now_cell);
				
				for (uint8_t i_ = 0; i_ < 3; i_++)
				{
					if (std::find(vec_3_edge_now_v.begin(), vec_3_edge_now_v.end(), vec_3_edge_temp[i_]) == vec_3_edge_now_v.end()) {
						now_edge = vec_3_edge_temp[i_];
						break;
					}
				}


			}
			if (edge_addin.size() != 0) {
				vec_vec_feature_edge.push_back(edge_addin);
				for (uint64_t i = 0; i < edge_addin.size(); ++i)
					edge_feature_temp.insert(edge_addin[i]);
			}
		}
		
	}

	//加入六个面集合 (加个球````)
	//按照对面形式加入

	//由一角点出发 找三个邻接面 
	std::vector<FaceHandle> face_vec = _mesh->get_3face_in_corner(vec_corner_v[0],vec_corner_cell[0]);

	//先把该三个面找全
	for (uint8_t i = 0; i < 6; i += 2) {
		std::vector<FaceHandle> temp_face_vec;
		PaddingSurface(temp_face_vec, face_vec[i / 2]);
		vev_vec_surface.push_back(temp_face_vec);
		
		//顺便把平行面填充了
		std::vector<FaceHandle> temp_face_oppo;
		for (uint8_t i_face_o = 0; i_face_o < temp_face_vec.size(); ++i_face_o) {
			FaceHandle now_face_ori = temp_face_vec[0];
			CellHandle now_cell_ori;
			if (vec_cell.find(*(_mesh->neighbor_f[now_face_ori].begin())) != vec_cell.end()) {
				now_cell_ori = *(_mesh->neighbor_f[now_face_ori].begin());
			}
			else {
				now_cell_ori = *(_mesh->neighbor_f[now_face_ori].rbegin());
			}
			now_face_ori = _mesh->get_opposite_face_in_cell(now_face_ori, now_cell_ori);
			while (std::find(vec_surface_face.begin(), vec_surface_face.end(), now_face_ori) == vec_surface_face.end()) {
				now_cell_ori = (now_cell_ori == *_mesh->neighbor_f[now_face_ori].begin()) ?
					*_mesh->neighbor_f[now_face_ori].rbegin() : *_mesh->neighbor_f[now_face_ori].begin();
				now_face_ori = _mesh->get_opposite_face_in_cell(now_face_ori,now_cell_ori);
			}
			temp_face_oppo.push_back(now_face_ori);
		}
		vev_vec_surface.push_back(temp_face_oppo);
	}
	

	return true;
}

bool BCSheet::Collapse2DualFace(std::vector<EdgeHandle>& initEdge)
{
	return false;
}

void BCSheet::setDualFace(EdgeHandle init_edge)
{
	//先遍历该边所在的边界特征边
	//0-11
	uint8_t choossen_feature_index = 12;
	for (choossen_feature_index = 0; choossen_feature_index < 11; ++choossen_feature_index) {
		if (std::find(vec_vec_feature_edge[choossen_feature_index].begin(), vec_vec_feature_edge[choossen_feature_index].end(),
			init_edge) != vec_vec_feature_edge[choossen_feature_index].end()) {
			break;
		}
	}
	

	//选取近似中点
	uint32_t mid_point = 0;
	mid_point = vec_vec_feature_edge[choossen_feature_index].size();
	//特殊情况 只有宽度1 只填充左侧
	//TODO:(最有可能出错```)
	if (mid_point == 1) {
		//在左侧面上找个起始面
		
		VertexHandle v_init = _mesh->edges_[init_edge].from_h;
		CellHandle cell_init = vec_corner_cell[std::find(vec_corner_v.begin(),vec_corner_v.end(),v_init)-vec_corner_v.begin()];
		FaceHandle f_init ;
		
		std::vector<FaceHandle> face_corner = _mesh->get_3face_in_corner(v_init,cell_init);
		std::vector<FaceHandle> face_2 = _mesh->get_2face_edge(init_edge,cell_init);
		for (uint16_t i = 0; i < face_corner.size(); ++i) {
			if (std::find(face_2.begin(), face_2.end(), face_corner[i]) != face_2.end()) {
				f_init = face_corner[i];
				break;
			}
		}
		this->index_dual_face = -1;
		uint16_t ori_face = 6;
		for (uint16_t i = 0; i < 6; ++i) {
			if (std::find(vev_vec_surface[i].begin(), vev_vec_surface[i].end(), f_init) != vev_vec_surface[i].end()) {
				ori_face = i;
			}
		}
		for (uint32_t face_i = 0; face_i < vev_vec_surface[ori_face].size(); ++face_i) {
			std::set<CellHandle, compare_OVM> set_neibor2 = _mesh->neighbor_f[vev_vec_surface[ori_face][face_i]];
			std::vector<CellHandle> cells_left_to_right;
			if (vec_cell.find(*set_neibor2.begin()) != vec_cell.end()) {
				cells_left_to_right.push_back(*set_neibor2.begin());
			}
			else {
				cells_left_to_right.push_back(*set_neibor2.rbegin());
			}

			face2cells.insert(std::make_pair(vev_vec_surface[ori_face][face_i],cells_left_to_right));
			for (std::vector<BaseComplex>::iterator iter = sheet_bc_set.begin(); iter != sheet_bc_set.end(); ++iter) {
				if ((*iter).find_cell(cells_left_to_right[0])) {
					surface_2part.insert(std::make_pair(vev_vec_surface[ori_face][face_i],std::make_pair((*iter).Idx(),false)));
					FaceHandle opposite = _mesh->get_opposite_face_in_cell(vev_vec_surface[ori_face][face_i], cells_left_to_right[0]);
					surface_2part.insert(std::make_pair(opposite, std::make_pair((*iter).Idx(), true)));
					break;
				}
			}

		}
		for (std::vector<BaseComplex>::iterator iter = sheet_bc_set.begin(); iter != sheet_bc_set.end(); ++iter) {
			std::set<CellHandle,compare_OVM> set = (*iter).getCellSet();
			std::vector <CellHandle> right;
			std::copy(set.begin(), set.end(), std::back_inserter(right));
			bc_2part.insert(std::make_pair((*iter).Idx(),std::make_pair(std::vector<CellHandle>(), right)));
		}


		////对其中各个basecomplex面在表面上的分布 左-右
		//std::map<FaceHandle, std::pair<BaseComplex*, bool>> surface_2part;

		////各个bc的分割 left->right
		//std::map<uint64_t, std::pair<std::vector<CellHandle>, std::vector<CellHandle>>> bc_2part;

		return;
		
	}
	//两侧分0-(mid_point/2-1) (mid_point)-(len-1)

	mid_point = mid_point / 2 - 1;
	//按照一定的顺序由内向外遍历
	//左侧向右侧进行
	this->index_dual_face = mid_point;
	//确定左右表面面位置

	EdgeHandle edge_start = vec_vec_feature_edge[choossen_feature_index][0];
	VertexHandle init_v;
	FaceHandle init_f;
	std::vector<VertexHandle>::iterator v_iter;
	if ((v_iter = std::find(vec_corner_v.begin(), vec_corner_v.end(), _mesh->edges_[edge_start].from_h)) != vec_corner_v.end()) {
		init_v = _mesh->edges_[edge_start].from_h;
	}
	else {
		init_v = _mesh->edges_[edge_start].to_h;
	}
	CellHandle init_cell = vec_corner_cell[v_iter - vec_corner_v.begin()];
	std::vector<FaceHandle> face_corner = _mesh->get_3face_in_corner(init_v, init_cell);
	std::vector<FaceHandle> face_2 = _mesh->get_2face_edge(init_edge, init_cell);
	for (uint16_t i = 0; i < face_corner.size(); ++i) {
		if (std::find(face_2.begin(), face_2.end(), face_corner[i]) != face_2.end()) {
			init_f = face_corner[i];
			break;
		}
	}
	
	uint16_t ori_face = 6;
	for (uint16_t i = 0; i < 6; ++i) {
		if (std::find(vev_vec_surface[i].begin(), vev_vec_surface[i].end(), init_f) != vev_vec_surface[i].end()) {
			ori_face = i;
		}
	}
	
	//分两半
	std::vector<CellHandle> left_cell;
	std::vector<CellHandle> right_cell;

	for (uint32_t i_face_ori = 0; i_face_ori < vev_vec_surface[ori_face].size(); ++i_face_ori) {
		CellHandle last_cell;
		FaceHandle last_face = vev_vec_surface[i_face_ori][0];
		FaceHandle dual_face;
		
		std::set<CellHandle, compare_OVM> cell2_set = _mesh->neighbor_f[last_face];
		last_cell =
			(vec_cell.find(*cell2_set.begin()) != vec_cell.end()) ?
			*cell2_set.begin() : *cell2_set.rbegin();

		//加入face pair
		std::vector<BaseComplex>::iterator iter;
		for (iter = sheet_bc_set.begin(); iter != sheet_bc_set.end(); ++iter) {
			if ((*iter).find_face(last_face)) {
				surface_2part.insert(std::make_pair(last_face,std::make_pair((*iter).Idx(),false)));
				break;
			}
		}


		std::vector<CellHandle> temp_addin(vec_vec_feature_edge[choossen_feature_index].size());
		temp_addin.push_back(last_cell);
		left_cell.push_back(last_cell);
		for (uint32_t left = 1; left < vec_vec_feature_edge[choossen_feature_index].size(); ++left) {
			last_face = _mesh->get_opposite_face_in_cell(last_face, last_cell);
			last_cell =
				(last_cell != *_mesh->neighbor_f[last_face].begin()) ?
				*_mesh->neighbor_f[last_face].rbegin(): *_mesh->neighbor_f[last_face].begin();
			if (left == index_dual_face) {
				dual_face = last_face;
			}
			temp_addin.push_back(last_cell);
			if (left <= mid_point) {
				left_cell.push_back(last_cell);
			}
			else {
				right_cell.push_back(last_cell);
			}
		}
		//加入面对
		surface_2part.insert(std::make_pair(last_face, std::make_pair((*iter).Idx(), true)));

		face2cells.insert(std::make_pair(dual_face,temp_addin));
		temp_vec_dual_face.push_back(dual_face);
	}
	

	for (std::vector<BaseComplex>::iterator iter = sheet_bc_set.begin(); iter != sheet_bc_set.end(); ++iter) {
		//求交集
		std::vector<CellHandle> vec_left;
		std::vector<CellHandle> vec_right;

		std::set<CellHandle,compare_OVM>& set = (*iter).getCellSet();
		//left
		std::set_intersection(left_cell.begin() , left_cell.end() , set.begin() , set.end(),vec_left.begin());

		//right
		std::set_intersection(right_cell.begin(), right_cell.end(), set.begin(), set.end(), vec_right.begin());

		bc_2part.insert(std::make_pair((*iter).Idx(),std::make_pair(vec_left,vec_right)));
	}
	

	return ;
}


void BCChord::extractDiagonalFaces()
{
}

BCChord::BCChord(HexV3fMesh * mesh, BCHexahedron & bc)
{
}

bool BCChord::Collapse2DiagonalFace(std::vector<EdgeHandle>& initEdge)
{
	return false;
}

void BCChord::setDualFace(uint8_t init_edge_index, uint8_t ori_index_1, uint8_t ori_index_2)
{
	return ;
}

BCSheet SheetOrChordHandler::SelectOneSheet()
{
	//随机选择一个未遍历的bc
	uint32_t index_bc_init = set_bc_now.size();
	std::set<BaseComplex, compare_BaseComplex>::iterator iter;

CHOOSE_ANOTHER:
	bool find = false;
	do
	{
		//index_bc_init = (rand() % (index_bc_init));
		//随机个毛线··
		
		
		//uint32_t p = 0;
		for (iter = set_bc_now.begin();
			(iter != set_bc_now.end()); ++iter) {
			const BaseComplex &item1 = const_cast<BaseComplex&>(*iter);
			BaseComplex  &real_bc = const_cast<BaseComplex&>(item1);
			std::set<FaceHandle,compare_OVM> complex_fs = real_bc.getFaceSet();
			for (std::set<FaceHandle, compare_OVM>::iterator iter = complex_fs.begin();
				iter != complex_fs.end(); ++iter) {
				if (mesh->is_bdy(*iter)) {
					find = true;
					break;
				}
			}
			if (find)break;
		}

		
	} while (bc_is_checked[(*iter).Idx()]&&!find);
	find = false;
	BaseComplex bc = *iter;
	std::vector<BaseComplex> bc_vec_init;
	bc_vec_init.push_back(bc);


	//选择该bc上一个边界特征边为起点
	BCHexahedron bch_init(mesh, bc_vec_init);
	bch_init.ExtracteSurfuceFeature();
	EdgeHandle init_edge;

	//选择在边界上的边界特征边
	uint8_t i_e_12;
	for (i_e_12 = 0; i_e_12 < 12; ++i_e_12) {
		if (mesh->is_bdy(bch_init.vec_vec_feature_edge[i_e_12][0])) {
			init_edge = bch_init.vec_vec_feature_edge[i_e_12][0];
			break;
		}
	}
	if (i_e_12 == 12) {
		goto CHOOSE_ANOTHER;
	}
	

	//遍历所有的平行边 邻接cell所在bc 加入sheet
	
	//设置属性 进行BFS 加入cell

	std::stack<EdgeHandle> stack_edge_paralle;
	
	std::vector<bool> edge_is_checked(mesh->edges_.size(),false);
	std::set<CellHandle, compare_OVM> sheet;
	//和feature edge 求交 
	std::set<EdgeHandle, compare_OVM> sheet_edge;
	uint64_t temp_cell_size = 0;
	stack_edge_paralle.push(init_edge);
	
	while (stack_edge_paralle.size()!=0){
		EdgeHandle temp_e = stack_edge_paralle.top();
		sheet_edge.insert(temp_e);
		stack_edge_paralle.pop();
		std::set<FaceHandle, compare_OVM> face_set = mesh->neighbor_e[temp_e];
		
		for (std::set<FaceHandle, compare_OVM>::iterator iter_face = face_set.begin();
			iter_face != face_set.end(); ++iter_face) {
			sheet.insert(*mesh->neighbor_f[*iter_face].begin());
			//为sheet插入新加入的cell
			if (sheet.size() > temp_cell_size) {
				++temp_cell_size;
				std::vector<EdgeHandle> edges_4 = mesh->get_parallel_edges(temp_e,*sheet.rbegin());
				for (uint8_t i = 0; i < edges_4.size(); ++i) {
					if (!edge_is_checked[edges_4[i]]) {
						stack_edge_paralle.push(edges_4[i]);
						
						edge_is_checked[edges_4[i]] = true;
					}
				}

			}
			//加入edge
			if (mesh->neighbor_f[*iter_face].size() == 2) {
				sheet.insert(*mesh->neighbor_f[*iter_face].rbegin());
				if (sheet.size() > temp_cell_size) {
					++temp_cell_size;
					std::vector<EdgeHandle> edges_4 = mesh->get_parallel_edges(temp_e, *sheet.rbegin());
					for (uint8_t i = 0; i < edges_4.size(); ++i) {
						if (!edge_is_checked[edges_4[i]]) {
							stack_edge_paralle.push(edges_4[i]);
							edge_is_checked[edges_4[i]] = true;
						}
					}

				}
			}
		}
		
	}

	//更改init_edge
	std::set<EdgeHandle,compare_OVM>::iterator itere;
	for (itere = sheet_edge.begin(); itere != sheet_edge.end(); ++itere) {
		if (!mesh->is_bdy(*itere)) continue;
		std::set<FaceHandle, compare_OVM> face_set = mesh->neighbor_e[*itere];
		std::set<EdgeHandle, compare_OVM> edge4;
		std::vector<EdgeHandle> edges;
		for (std::set<FaceHandle, compare_OVM>::iterator f_iter = face_set.begin(); f_iter != face_set.end(); ++f_iter) {
			edge4.insert(mesh->get_opposite_edge_in_face(*itere,*f_iter));
		}
		std::set_intersection(edge4.begin(),edge4.end(), sheet_edge.begin(),sheet_edge.end(),edges.begin());
		if (edges.size() == 2) {
			init_edge = *itere;
			break;
		}
	}
	if (itere == sheet_edge.end()) {
		std::cout << "error";
	}

	
	std::vector<BaseComplex> bc_addin_sheet;
	for (std::set<BaseComplex, compare_BaseComplex>::iterator iter_bc = set_bc_now.begin();
		iter_bc != set_bc_now.end(); ++iter_bc) {
		const BaseComplex &item1 = const_cast<BaseComplex&>(*iter_bc);
		BaseComplex  &real_bc = const_cast<BaseComplex&>(item1);
		std::set<CellHandle, compare_OVM>& cell_in_bc = real_bc.getCellSet();

		std::set<CellHandle, compare_OVM>::iterator iter_cell_in_bc = cell_in_bc.begin();
		std::set<CellHandle, compare_OVM>::iterator iter_sheet = sheet.begin();

		while ((iter_cell_in_bc != cell_in_bc.end()) &&
			(iter_sheet != sheet.end())) {
			if ((*iter_cell_in_bc) > (*iter_sheet)) ++iter_sheet;
			else if ((*iter_cell_in_bc) < (*iter_sheet)) ++iter_cell_in_bc;
			else {
				bc_addin_sheet.push_back(real_bc);
				break;
			}
		}
	}

	BCSheet bch(mesh, bc_addin_sheet,init_edge);
	//bch.ExtracteSurfuceFeature();
	//bch.setDualFace(init_edge);
	//提取返回
	return bch;
}

bool SheetOrChordHandler::CollapseOneSheet(BCSheet & bc_sheet)
{
	//寻找周边bc

	//合并bc

	//删去旧的bc 添加新的bc
	return false;
}
