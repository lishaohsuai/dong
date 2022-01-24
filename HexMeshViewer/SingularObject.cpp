#include "SingularObject.h"
SingularObject::SingularObject() {

}
SingularObject::SingularObject(HexV3fMesh* in_mesh) {

	mesh = in_mesh;
	boolVProp_singular.resize(in_mesh->vertices_.size(),false);
	intEProp_singular.resize(in_mesh->edges_.size(),0);
	B_READY = true;
}
SingularObject::SingularObject(HexV3fMesh* in_mesh, std::vector<EdgeHandle>& addin_vec) {
	mesh = in_mesh;
	boolVProp_singular.resize(in_mesh->vertices_.size(), false);
	intEProp_singular.resize(in_mesh->edges_.size(), 0);
	//中间迭代过程加入的奇异边标记
	for (std::vector<EdgeHandle>::iterator vec_iter = addin_vec.begin();
		vec_iter != addin_vec.end(); ++vec_iter) {
		intEProp_singular[*vec_iter] = UNREGULAR_EDGE_ADDIN_N;
	}
	
	B_READY = true;
}
SingularObject::~SingularObject() {

};
void SingularObject::FindSingularObject_simple() {
	if (!B_READY) return;

	uint64_t count_of_sin = 0;

	unsigned char valence; 
	for (uint64_t ve_it = 0; ve_it != mesh->vertices_.size(); ve_it++) {
		
		valence = mesh->is_bdy(VertexHandle(ve_it))? REGULAR_VERTEX_BDY_N:REGULAR_VERTEX_INNER_N;
		
		if (mesh->neighbor_v[ve_it].size() != valence) {
			//奇异点
			vector_singular_vertex_handles.push_back(VertexHandle(ve_it));
			//添加属性记录
			boolVProp_singular[ve_it] = true;
			
		}
	}


	//奇异边
	for (uint64_t e_iter = 0; e_iter != mesh->edges_.size(); ++e_iter) {
		
		bool checkGoto = false;
		int count = 0;

		count = mesh->neighbor_e[e_iter].size();

		//起始度数不为0的直接跳过 为上次迭代引入的局部奇异边
		if (intEProp_singular[e_iter] == UNREGULAR_EDGE_ADDIN_N) {
			continue;
		}
		if (mesh->is_bdy(EdgeHandle(e_iter))) {
			if (count != REGULAR_EDGE_BDY_N) {
				intEProp_singular[e_iter] = count;
				vector_singular_edge_handles.push_back(EdgeHandle(e_iter));
				std::cout << "select:" << count << "-bdy-" << e_iter << std::endl;
				++count_of_sin;
			}
			else {
				std::cout << "unselect:" << count << "-bdy-" << e_iter << std::endl;
			}
		}
		else {
			if (count != REGULAR_EDGE_INNER_N) {
				intEProp_singular[e_iter] = count;
				vector_singular_edge_handles.push_back(EdgeHandle(e_iter));
				std::cout << "select:" << count << "-ine-" << e_iter << std::endl;
				++count_of_sin;
			}
			else {
				std::cout << "unselect:" << count << "-ine-" << e_iter << std::endl;
			}
		}
	}
	std::cout << "count_select:" << count_of_sin << std::endl;
}


//初始化网格
bool SingularObject::InitMesh() {
	return true;
}
