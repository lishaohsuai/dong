#include "BaseComplexSimplify.h"


BaseComplexSimplify::BaseComplexSimplify(HexV3fMesh * mesh, std::set<FaceHandle, compare_OVM> f_all, std::set<BaseComplex, compare_BaseComplex> baseComplexSet)
{
	this->mesh = mesh;
	this->f_all_set = f_all;
	this->bc_set = baseComplexSet;
}

void BaseComplexSimplify::ExtendBlock2One(HexV3fMesh* re_meshing_result) {

	//简化后的点
	std::vector<VertexHandle> remeshing_ves;

	for (std::set<BaseComplex, compare_BaseComplex>::iterator bc_iter = bc_set.begin(); bc_iter != bc_set.end(); ++bc_iter) {
		//获取每个bc的角点
		const BaseComplex &item1 = const_cast<BaseComplex&>(*bc_iter);
		BaseComplex  &real_bc = const_cast<BaseComplex&>(item1);
		std::map<unsigned char, VertexHandle> map = this->CheckIndexInCorner(real_bc);
		
		//排序map 按unsigned char
		if (map.size() != 8) {
			//出错
			std::cout << "error";
		}
		//记录点重排后次序 cell-ves
		std::vector<VertexHandle> cell_ves(8);
		for (std::map<unsigned char, VertexHandle>::iterator map_iter = map.begin();
			map_iter != map.end(); ++map_iter) {
			std::vector<VertexHandle>::iterator iter_diff;
			if ((iter_diff=std::find(remeshing_ves.begin(),remeshing_ves.end(),(*map_iter).second)) != remeshing_ves.end()) {
				//已存在
				cell_ves.push_back(VertexHandle(iter_diff-remeshing_ves.begin()));
			}
			else {
				//不存在 加入remeshing_ves 和 cell_ves
				remeshing_ves.push_back((*map_iter).second);
				re_meshing_result->add_vertex(mesh->vertices_[(*map_iter).second]);
				cell_ves.push_back(VertexHandle(cell_ves.size()));
			}

			re_meshing_result->add_cell(cell_ves);
		}

	}

}

//不在corner位置返回-1 否则定位0-7
std::map<unsigned char, VertexHandle> BaseComplexSimplify::CheckIndexInCorner(BaseComplex &bc) {
	
	std::map<unsigned char, VertexHandle> map_v_corner;
	//判断边界点上是否存在点在三个邻接表面上
	std::set<VertexHandle,compare_OVM>& set_bdy_v = bc.getVes();
	std::set<FaceHandle,compare_OVM>& set_bdy_face = bc.getFaceSet();
	std::set<CellHandle, compare_OVM>& set_cell = bc.getCellSet();
	int index = 0;
	for (std::set<VertexHandle, compare_OVM>::iterator v_iter = set_bdy_v.begin(); v_iter != set_bdy_v.end(); ++v_iter) {
		std::set<FaceHandle, compare_OVM> set_v_face = mesh->neighbor_v_f[*v_iter];
		std::vector<FaceHandle> count_face;

		std::set_intersection(set_v_face.begin(), set_v_face.end(), set_bdy_face.begin(), set_bdy_face.end(),
			std::inserter(count_face,count_face.begin()),compare_OVM());

		if (count_face.size() != 3) {
			continue;
		}
		//std::sort(count_face.begin() , count_face.end(),compare_OVM());

		//存在 判断点所在的单元 定位相对位置
		std::set<CellHandle, compare_OVM> set_cell_linked = mesh->neighbor_v_c[*v_iter];
		for (std::set<CellHandle, compare_OVM>::iterator iter_set_cell = set_cell_linked.begin();
			iter_set_cell != set_cell_linked.end(); ++iter_set_cell) {
			//看单元是否在bc内部
			if (std::find(set_cell.begin(), set_cell.end(), *iter_set_cell) != set_cell.end()) {
				std::vector<VertexHandle> v_vector = mesh->cells_[*iter_set_cell].vertex_;
				//std::find(v_vector.begin(), v_vector.end(), *v_iter) - v_vector.begin()
				map_v_corner.insert(std::make_pair(index++, *v_iter));
				break;
			}
		}
		if (map_v_corner.size()>=8)
		{
			break;
		}
	}
	//排序
	return map_v_corner;
}
