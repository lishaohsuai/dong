#include "BaseComplex.h"

BaseComplex::BaseComplex(std::set<VertexHandle, compare_OVM> complex_ves,
	std::set<FaceHandle, compare_OVM> complex_fs,
	std::set<EdgeHandle, compare_OVM> complex_seds,
	std::set<CellHandle, compare_OVM> complex_hexcells) {
	this->complex_ves = complex_ves;
	this->complex_hexcells = complex_hexcells;
	this->complex_fs = complex_fs;
	this->complex_seds = complex_seds;

}

bool BaseComplex::insertVertex(VertexHandle ve, bool is_inner) {
	if (is_inner) {
		return this->complex_ves_inner.insert(ve).second;
	}
	else {
		return this->complex_ves.insert(ve).second;
	}

}
bool BaseComplex::insertFace(FaceHandle f) {
	return this->complex_fs.insert(f).second;
}
bool BaseComplex::insertEdge(EdgeHandle ed) {
	return this->complex_seds.insert(ed).second;
}
bool BaseComplex::insertCellHandle(CellHandle ce) {
	return this->complex_hexcells.insert(ce).second;
}
void BaseComplex::setIdx(const int& x) {
	this->idx = x;
}
const int& BaseComplex::Idx() const {
	return this->idx;

}

//**************************************
//filler

int BaseComplexSetFiller::SFaceSetSeeking_Simple(const std::vector<unsigned char>& intEProp_sigular)
{
	uint64_t count_n = 0;
	
	for (uint64_t e_iter = 0; e_iter != mesh->edges_.size(); ++e_iter) {
		if (intEProp_sigular[e_iter] == 0) continue;
		std::cout << e_iter << "---";
		const std::set<FaceHandle,compare_OVM> set_neighbor_face = mesh->neighbor_e[e_iter];
		std::cout <<"e_id:"<<e_iter<<"--"<<set_neighbor_face.size()<<"\n";
		
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_iter = set_neighbor_face.begin() ; e_f_iter != set_neighbor_face.end(); ++e_f_iter)
		{
			count_n += GetSFaceExtend_Simple(EdgeHandle(e_iter),*e_f_iter, intEProp_sigular);
		}

	}
	return count_n;
}

void BaseComplexSetFiller::EraseSpace()
{
	cell_temp_set.swap(std::set<CellHandle, compare_OVM>());
	f_all_set.swap(std::set<FaceHandle, compare_OVM>());
	
}


int BaseComplexSetFiller::GetSFaceExtend_Simple(const EdgeHandle& e, const FaceHandle& f, const std::vector<unsigned char> &intEProp_sigular)
{

	EdgeHandle now_e_handle = e;
	EdgeHandle last_e_handle = e;
	FaceHandle now_f_handle = f;
	
	FaceHandle hf_h_temp[4];

	std::vector<CellHandle> cell_vec;

	int count = 0;
	int com = 0;
	do
	{
		++com;
		last_e_handle = now_e_handle;
		
		//找对边
		const std::vector<EdgeHandle> set_e = mesh->faces_[now_f_handle].edges_;
		for (uint16_t i = 0; i < 4; ++i) {
			if (mesh->non_intersect(set_e[i], now_e_handle)) {
				now_e_handle = set_e[i];
				break;
			}
			if (i == 3) {
				std::cout << "error";
			}
		}
		
		
		//检查是否到边界或者奇异边
		if (mesh->is_bdy(last_e_handle)) { //起始边在边界上

			if (mesh->is_bdy(now_e_handle)) { //对边在边界上 
				return count;
			}
			else if (!intEProp_sigular[now_e_handle]) {//对边不在边界上 且不是奇异边
				
				f_all_set.insert(now_f_handle);

				//跳到下个邻接半面

				std::set<CellHandle, compare_OVM> set_cells_  = mesh->neighbor_f[now_f_handle];
				std::set<FaceHandle, compare_OVM> set_4_faces_ = mesh->neighbor_e[now_e_handle];
				std::set<FaceHandle, compare_OVM> set_except_faces_;
				for (std::set<CellHandle, compare_OVM>::iterator cell_iter = set_cells_.begin();
					cell_iter != set_cells_.end(); ++cell_iter) {

					std::vector<FaceHandle> f_vec = mesh->cells_[*cell_iter].faces_;
					for (int i = 0; i < f_vec.size(); ++i) {
						set_except_faces_.insert(f_vec[i]);
					}
				}
				for (std::set<FaceHandle, compare_OVM>::iterator faces_4_it = set_4_faces_.begin();
					faces_4_it != set_4_faces_.end(); ++faces_4_it) {
					if (set_except_faces_.find(*faces_4_it) == set_except_faces_.end()) {
						now_f_handle = *faces_4_it;
						break;
					}
				}
				++count;
			}
			else {//对边不在边界上 是奇异边
				
				f_all_set.insert(now_f_handle);

				++count;
				return count;
			}

		}
		else { //起始边不在边界上
			if (mesh->is_bdy(now_e_handle) || (intEProp_sigular[now_e_handle] != 0)) { //对边在边界上或是奇异边
				f_all_set.insert(now_f_handle);

				++count;
				return count;
			}
			else {//对边不在边界上 也不是奇异边
				f_all_set.insert(now_f_handle);

				//跳到下个邻接半面

				std::set<CellHandle, compare_OVM> set_cells_ = mesh->neighbor_f[now_f_handle];
				std::set<FaceHandle, compare_OVM> set_4_faces_ = mesh->neighbor_e[now_e_handle];
				std::set<FaceHandle, compare_OVM> set_except_faces_;
				for (std::set<CellHandle, compare_OVM>::iterator cell_iter = set_cells_.begin();
					cell_iter != set_cells_.end(); ++cell_iter) {

					std::vector<FaceHandle> f_vec = mesh->cells_[*cell_iter].faces_;
					for (int i = 0; i < f_vec.size(); ++i) {
						set_except_faces_.insert(f_vec[i]);
					}
					
				}
				for (std::set<FaceHandle, compare_OVM>::iterator faces_4_it = set_4_faces_.begin();
					faces_4_it != set_4_faces_.end(); ++faces_4_it) {
					if (set_except_faces_.find(*faces_4_it) == set_except_faces_.end()) {
						now_f_handle = *faces_4_it;
						break;
					}
				}
				
				++count;
			}
		}


	} while (!mesh->is_bdy(now_f_handle));
	return count;
}
//改成bfs方式
void BaseComplexSetFiller::HexCellFlooding_BFS(std::vector<bool>& boolCellChecked, const CellHandle& initHandle) {
	//广度优先栈
	std::stack<CellHandle> stack_cell;
	stack_cell.push(initHandle);
	

	//广度优先填充
	while (stack_cell.size() != 0) {
		CellHandle cell_now = stack_cell.top();
		stack_cell.pop();
		if (boolCellChecked[cell_now]) continue;
		boolCellChecked[cell_now] = true;
		cell_temp_set.insert(cell_now);
		std::vector<FaceHandle> vec_face = mesh->cells_[cell_now].faces_;
		

		for (unsigned char ori = 0; ori < 6; ++ori) {
			FaceHandle face_handle = vec_face[ori];
			if (mesh->is_bdy(face_handle) || (f_all_set.find(face_handle) != f_all_set.end())) continue;
			std::set<CellHandle, compare_OVM> set_cell = mesh->neighbor_f[face_handle];
			CellHandle cell_next = (*set_cell.begin() == cell_now) ? *set_cell.rbegin() : *set_cell.begin();
			if (boolCellChecked[cell_next])continue;
			stack_cell.push(cell_next);
		}
	}
	
}
void BaseComplexSetFiller::HexCellFlooding_Recursive(const unsigned char from_oriention, std::vector<bool>& boolCellChecked, const CellHandle& initHandle) {

	CellHandle now_cell_handle;
	FaceHandle now_face_handle, tmp_face_handle;
	//初始的遍历方向
	unsigned char now_oriention = 0;
	now_cell_handle = initHandle;

	if (!initHandle) {
		now_cell_handle = CellHandle(0);
	}

	//提前过滤
	if (boolCellChecked[now_cell_handle]) {
		return;
	}

	cell_temp_set.insert(now_cell_handle);
	boolCellChecked[now_cell_handle] = true;
	//使用初始种子开始填充
	do
	{
		//跳过来向
		if (now_oriention == from_oriention) continue;
		tmp_face_handle = mesh->cells_[now_cell_handle].faces_[now_oriention];
		
		if (!mesh->is_bdy(tmp_face_handle) &&
			(f_all_set.end() == f_all_set.find(tmp_face_handle))
		) {//未达到边界或者分割面
			//当前加入集合
			std::set<CellHandle, compare_OVM> set_cell_neihbor = mesh->neighbor_f[tmp_face_handle];
			
			CellHandle temp_cell =  (*set_cell_neihbor.begin() == now_cell_handle)? *set_cell_neihbor.rbegin(): *set_cell_neihbor.begin();
			if (boolCellChecked[temp_cell]) continue;
			//判断下当前面所在位置
			unsigned char temp_ori = std::find(mesh->cells_[temp_cell].faces_.begin(), mesh->cells_[temp_cell].faces_.end(),tmp_face_handle)
				- mesh->cells_[temp_cell].faces_.begin();
			HexCellFlooding_Recursive(temp_ori, boolCellChecked, temp_cell);
		}

		
	} while ((++now_oriention) <= 5);
}
//TODO:加上对角点数目的检测
std::vector<EdgeHandle> BaseComplexSetFiller::OneBaseComplexFilling(std::set<BaseComplex, compare_BaseComplex>& baseComplexSet, const CellHandle& initHandle) {
	int count_cell = 0;
	CellHandle initCell;
	//添加六面体单元属性
	std::vector<bool> boolCellChecked(mesh->cells_.size(),false);

	int i = 0;
	//提前把边界面上点筛选出来
	std::set<VertexHandle, compare_OVM> surface_vex_set;
	for (std::set<FaceHandle, compare_OVM>::iterator f_iter = f_all_set.begin(); f_iter != f_all_set.end(); ++f_iter) {
		std::vector<VertexHandle> vector_v = mesh->faces_[*f_iter].vs_f;
		for (std::vector<VertexHandle>::iterator v_iter = vector_v.begin(); v_iter != vector_v.end(); ++v_iter) {
			surface_vex_set.insert(*v_iter);
		}
	}
	std::vector<FaceHandle> f_all_vec;
	f_all_vec.assign(f_all_set.begin() , f_all_set.end());

	std::vector<EdgeHandle> edge_addin;

	
	//开始种子填充
	while (count_cell<mesh->cells_.size()) {
		//寻找未遍历的初始种子
		for (uint64_t c_iter = 0; c_iter != mesh->cells_.size(); ++c_iter) {
			if (!boolCellChecked[c_iter]) {
				initCell = CellHandle(c_iter);
				break;
			}
		}

		this->HexCellFlooding_Recursive(INVALID_ORIENTATION,boolCellChecked, initCell);
		BaseComplex baseComplex;
		baseComplex.setIdx(++i);
		InsertObject2BaseComplex(baseComplex, surface_vex_set);
		//检测该BC的角点
		
		//添加新加入的奇异边
		DetectSingularityInBC(baseComplex, f_all_vec, edge_addin);
		
		baseComplexSet.insert(baseComplex);
		count_cell += cell_temp_set.size();
		//清空暂存
		std::cout << " {" << (int)cell_temp_set.size() << "} " << "\n";
		cell_temp_set.swap(std::set<CellHandle, compare_OVM>());
	}
	std::cout << " " << count_cell;
	return edge_addin;
}
bool BaseComplexSetFiller::DetectSingularityInBC(BaseComplex& bc,std::vector<FaceHandle>& face_sep_vec, std::vector<EdgeHandle>& edge_addin) {//分割面
	std::set<CellHandle, compare_OVM> set_cell = bc.getCellSet();
	std::vector<CellHandle> vec_cell;
	std::vector<FaceHandle> vec_face;
	vec_cell.assign(set_cell.begin(),set_cell.end());
	
	IntermediateBCResult intermediate(mesh,vec_cell);
	intermediate.DetecteFeature(face_sep_vec);
	std::vector<VertexHandle> vec_v = intermediate.ExtractCornerVertex();
	if (vec_v.size() == 0) {//如果不是类六面体 则添加新的奇异边
		//edge_addin.assign(intermediate.edge_feature.begin() ,intermediate.edge_feature.end());
		//后插个新提取的奇异边集合
		edge_addin.insert(edge_addin.end(), intermediate.edge_singular_addin.begin(), intermediate.edge_singular_addin.end());
		return false;
	}
	else {
		if (intermediate.edge_singular_addin.size() != 0) {
			edge_addin.insert(edge_addin.end(), intermediate.edge_singular_addin.begin(), intermediate.edge_singular_addin.end());
		}
		//修改角点
		return true;
	}
}

//填充每个分割块
void BaseComplexSetFiller::InsertObject2BaseComplex(BaseComplex& baseComplex, std::set<VertexHandle, compare_OVM>& surface_vex_set) {

	std::set<VertexHandle, compare_OVM> v_all_set;

	for (std::set<CellHandle, compare_OVM>::iterator bc_iter = cell_temp_set.begin();
		bc_iter != cell_temp_set.end();
		++bc_iter) {

		baseComplex.insertCellHandle(*bc_iter);
		//插入全点集
		std::vector<VertexHandle> vec_cell_v = mesh->cells_[*bc_iter].vertex_;
		for (std::vector<VertexHandle>::iterator cell_v_iter = vec_cell_v.begin(); cell_v_iter != vec_cell_v.end(); ++cell_v_iter) {
			v_all_set.insert(*cell_v_iter);
		}
	}
	//cao 这都有错```` 有毒吧
	
	std::set<FaceHandle, compare_OVM> face_set;
	std::vector<FaceHandle> face_all;
	std::set<CellHandle,compare_OVM> cell_set = baseComplex.getCellSet();
	for (std::set<CellHandle, compare_OVM>::iterator iter = cell_set.begin(); iter!=cell_set.end(); ++iter) {
		std::copy(mesh->cells_[*iter].faces_.begin(), mesh->cells_[*iter].faces_.end(), std::back_inserter(face_all));
	}
	//去重
	std::sort(face_all.begin(), face_all.end(), compare_OVM());
	uint16_t count = 0;
	for (uint64_t f = 0; f < face_all.size(); ++f) {

		if ((count = std::count(face_all.begin(), face_all.end(), face_all[f])) == 1) {
			face_set.insert(face_all[f]);
		}
		else {
			while (--count) ++f;
		}
	}
	for (std::set<FaceHandle, compare_OVM>::iterator iter = face_set.begin(); iter != face_set.end(); ++iter) {
		baseComplex.insertFace(*iter);
	}


	
	//	//插入全点集
	//	std::vector<VertexHandle> vec_cell_v = mesh->cells_[*bc_iter].vertex_;
	//	for (std::vector<VertexHandle>::iterator cell_v_iter = vec_cell_v.begin(); cell_v_iter != vec_cell_v.end(); ++cell_v_iter) {
	//		v_all_set.insert(*cell_v_iter);
	//	}

	//	std::vector<FaceHandle> f_vec = mesh->cells_[*bc_iter].faces_;
	//	for (std::vector<FaceHandle>::iterator f_iter = f_vec.begin(); f_iter != f_vec.end(); ++f_iter) {
	//		//对边界半面的插入
	//		if (mesh->is_bdy(*f_iter) || (f_all_set.find(*f_iter) != f_all_set.end())) {//可找到 即在分割面或边界上
	//			baseComplex.insertFace(*f_iter);
	//		}

	//	}

	//}
	//对整个两种点集的填充
	std::set<FaceHandle, compare_OVM> f_temp = baseComplex.getFaceSet();
	//填充点集
	for (std::set<FaceHandle, compare_OVM>::iterator f_iter = f_temp.begin(); f_iter != f_temp.end(); ++f_iter) {
		std::vector<VertexHandle> set_v_face = mesh->faces_[*f_iter].vs_f;
		for (std::vector<VertexHandle>::iterator f_v_iter = set_v_face.begin(); f_v_iter != set_v_face.end(); ++f_v_iter) {
			baseComplex.insertVertex(*f_v_iter, false);
		}
	}

	std::set<VertexHandle, compare_OVM> ve_bdy_temp = baseComplex.getVes();//获取边界点集
	//遍历筛下内部点集
	for (std::set<VertexHandle, compare_OVM>::iterator expect_v_iter = v_all_set.begin(); expect_v_iter != v_all_set.end(); ++expect_v_iter) {
		//找不到
		if (ve_bdy_temp.find(*expect_v_iter)==ve_bdy_temp.end()) {
			baseComplex.insertVertex(*expect_v_iter, true);
		}
	}
	std::cout << " {" << (int)baseComplex.getVesInner().size() << "} ";
	//清理下空间
	v_all_set.clear();
	f_temp.clear();
	ve_bdy_temp.clear();

}

bool IntermediateBCResult::DetecteFeature(const std::vector<FaceHandle>& vec_de_face)//分割面
{
	std::set<FaceHandle, compare_OVM> face_set;
	std::set<EdgeHandle, compare_OVM> edge_set;
	for (uint32_t cell_index = 0; cell_index < this->ibc_vec_cell.size();++cell_index) {
		for (uint8_t cell_f_index = 0; cell_f_index < 6; ++cell_f_index) {
			FaceHandle face_temp = mesh->cells_[ibc_vec_cell[cell_index]].faces_[cell_f_index];
			//加入
			if (face_set.insert(face_temp).second) {
				for (uint8_t i = 0; i < 4; ++i) {
					EdgeHandle edge_temp = mesh->faces_[face_temp].edges_[i];
					if (std::find(ibc_vec_edge.begin(), ibc_vec_edge.end(), edge_temp) == ibc_vec_edge.end()) {
						ibc_vec_edge.push_back(edge_temp);
					}
				}
			}
		}
	}
	b_vec_bdy_edge.assign(ibc_vec_edge.size(), false);
	//两次交集 插入 ibc_vec_bdy_face
	std::set_intersection(face_set.begin(), face_set.end(), vec_de_face.begin(), vec_de_face.end()
		, std::back_inserter(ibc_vec_bdy_face), compare_OVM());

	for (std::set<FaceHandle, compare_OVM>::iterator iter = face_set.begin();
		iter != face_set.end(); ++iter) {
		if (mesh->is_bdy(*iter)) {
			ibc_vec_bdy_face.push_back(*iter);//分割面加边界面
		}
	}

	
	//校验重复
	uint64_t origin_size = ibc_vec_bdy_face.size();
	std::unique(ibc_vec_bdy_face.begin() , ibc_vec_bdy_face.end() , compare_OVM()); 
	if (origin_size != ibc_vec_bdy_face.size()) return false;

	//加入边界边标示
	for (std::vector<FaceHandle>::iterator face_iter = ibc_vec_bdy_face.begin();
		face_iter != ibc_vec_bdy_face.end(); ++face_iter) {
		std::vector<EdgeHandle> edge_vec = mesh->faces_[*face_iter].edges_;
		std::vector<VertexHandle> v_ec = mesh->faces_[*face_iter].vs_f;
		for (uint8_t i = 0; i < 4; ++i) {
			//改标示
			std::vector<EdgeHandle>::size_type diff = std::find(ibc_vec_edge.begin() , ibc_vec_edge.end(),edge_vec[i]) - ibc_vec_edge.begin();
			b_vec_bdy_edge[diff] = true;
			ibc_vec_bdy_v.insert(v_ec[i]);
		}
	}
	
	
	//边界线记录
	for (std::vector<EdgeHandle>::iterator edge_iter = ibc_vec_edge.begin();
		edge_iter != ibc_vec_edge.end(); ++edge_iter) {
		std::vector<FaceHandle> set_face_n;
		std::set<FaceHandle, compare_OVM> set = mesh->neighbor_e[*edge_iter];
		std::set_intersection(set.begin(), set.end(), face_set.begin(), face_set.end(), std::inserter(set_face_n, set_face_n.begin()));
		if (b_vec_bdy_edge[(edge_iter- ibc_vec_edge.begin())]) {//如果是边界边
			//记录邻接关系和
			if (set_face_n.size() == 2) {
				edge_feature.push_back(*edge_iter);
			}
			else {
				if (set_face_n.size() != REGULAR_EDGE_BDY_N) {
					edge_singular_addin.push_back(*edge_iter);
				}
			}
			
			e_neibor.push_back(set_face_n);
		}
		else {
			//记录邻接关系
			if (set_face_n.size() != REGULAR_EDGE_INNER_N) {
				edge_singular_addin.push_back(*edge_iter);
			}
			e_neibor.push_back(set_face_n);
		}
	}

	return true;
}

std::vector<VertexHandle> IntermediateBCResult::ExtractCornerVertex()
{
	//对特征边进行遍历，其上的点是否存在角点
	std::vector<VertexHandle> corner_vs;
	//不能只算边界特征边 边界上的局部奇异边也要算上
	std::set<VertexHandle, compare_OVM> ver_all_checking;
	for(std::vector<EdgeHandle>::iterator e_iter = this->edge_feature.begin(); e_iter != this->edge_feature.end(); ++e_iter){
		ver_all_checking.insert(mesh->edges_[*e_iter].from_h);
		ver_all_checking.insert(mesh->edges_[*e_iter].to_h);
	}
	for (std::vector<EdgeHandle>::iterator e_iter = this->edge_singular_addin.begin(); e_iter != this->edge_singular_addin.end(); ++e_iter) {
		if (b_vec_bdy_edge[std::find(ibc_vec_edge.begin(), ibc_vec_edge.end(), *e_iter) - ibc_vec_edge.begin()]) {
			ver_all_checking.insert(mesh->edges_[*e_iter].from_h);
			ver_all_checking.insert(mesh->edges_[*e_iter].to_h);
		}
	}
	for (std::set<VertexHandle, compare_OVM>::iterator iter = ver_all_checking.begin(); iter != ver_all_checking.end(); ++iter) {
		

		std::set<CellHandle, compare_OVM> set_cell = mesh->neighbor_v_c[*iter];
		uint8_t count_cell_in_bc = 0;
		for (std::set<CellHandle, compare_OVM>::iterator cell_iter = set_cell.begin(); cell_iter != set_cell.end(); ++cell_iter) {
			if (std::find(ibc_vec_cell.begin(), ibc_vec_cell.end(), *cell_iter) != ibc_vec_cell.end())
				++count_cell_in_bc;
		}
		if (count_cell_in_bc == 1) {
			if (std::find(corner_vs.begin(), corner_vs.end(), *iter) == corner_vs.end()) {
				corner_vs.push_back(*iter);
			}
		}
	}
	if (corner_vs.size() == 8) {
		return corner_vs;
	}
	else {
		return std::vector<VertexHandle>();
	}
}

