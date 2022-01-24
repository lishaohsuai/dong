#ifndef SHEET_OPERATION_H
#define SHEET_OPERATION_H
#include "ComHeader.h"
#include "BaseComplexSimplify.h"

#define ARRAY_SIZE( ARRAY ) (sizeof (ARRAY) / sizeof (ARRAY[0]))

class BCHexahedron {
private :
	bool is_init = false;
	

	void PaddingSurface(std::vector<FaceHandle>& faces_surface, FaceHandle init_face);
public :
	HexV3fMesh *_mesh;
	std::set<CellHandle,compare_OVM> vec_cell;

	//bc边界面
	std::vector<FaceHandle> vec_surface_face;

	//bc边界点
	std::set<VertexHandle,compare_OVM> set_surface_v;

	//单元是否在内部(单元是否存在边界面)
	std::map<CellHandle, bool, compare_OVM> map_is_inner ;

	//边界面 0-6面上
	std::vector<std::vector<FaceHandle>> vev_vec_surface;

	//12个边界特征边
	std::vector<std::vector<EdgeHandle>> vec_vec_feature_edge;

	//8个角点
	std::vector<VertexHandle> vec_corner_v;
	//8个角点单元(对应角点)
	std::vector<CellHandle> vec_corner_cell; 
	//角点单元
	std::set<CellHandle, compare_OVM> set_corner_cell;

	//对偶面
	std::vector<FaceHandle> temp_vec_dual_face;

	//包含的BaseComplex 
	std::vector<BaseComplex> sheet_bc_set;



	BCHexahedron() {
		
	}
	BCHexahedron(const BCHexahedron& bch) {
		vec_cell = bch.vec_cell;
		vec_surface_face = bch.vec_surface_face;
		set_surface_v = bch.set_surface_v;
		map_is_inner = bch.map_is_inner;
		vev_vec_surface = bch.vev_vec_surface;
		vec_vec_feature_edge = bch.vec_vec_feature_edge;
		vec_corner_v = bch.vec_corner_v;
		vec_corner_cell = bch.vec_corner_cell;
		temp_vec_dual_face = bch.temp_vec_dual_face;
		sheet_bc_set = bch.sheet_bc_set;

	}
	BCHexahedron& operator =(const BCHexahedron& bch) {
		vec_cell = bch.vec_cell;
		vec_surface_face = bch.vec_surface_face;
		set_surface_v = bch.set_surface_v;
		map_is_inner = bch.map_is_inner;
		vev_vec_surface = bch.vev_vec_surface;
		vec_vec_feature_edge = bch.vec_vec_feature_edge;
		vec_corner_v = bch.vec_corner_v;
		vec_corner_cell = bch.vec_corner_cell;
		temp_vec_dual_face = bch.temp_vec_dual_face;
		sheet_bc_set = bch.sheet_bc_set;
	}
	BCHexahedron(HexV3fMesh *_mesh, std::vector<BaseComplex>&  bc);

	bool ExtracteSurfuceFeature();

	//根据特定方向提取对偶面 return 对偶面在边界上交线集合
	//ori_index 提取方向 正交面
	virtual void setDualFace(uint8_t init_edge_index, uint8_t ori_index_1, uint8_t ori_index_2) {};
};
class BCSheet : public BCHexahedron {
private:

	//w方向 surface points pair
	//std::map<VertexHandle,std::pair<VertexHandle, VertexHandle>> dual_v_vpairs;

	//dual face 2 v
	//std::map<FaceHandle, std::pair<FaceHandle,FaceHandle>> dual_face_fpairs;

	//dual face 2 cell
	std::map<FaceHandle, std::vector<CellHandle>> face2cells;

	//对其中各个在表面上面的分布 所在bc 左-右
	std::map<FaceHandle, std::pair<uint32_t, bool>> surface_2part;

	//各个bc的分割 left->right
	std::map<uint32_t, std::pair<std::vector<CellHandle>, std::vector<CellHandle>>> bc_2part;

	//dual face位置(dual face左侧第一个cell)
	int32_t index_dual_face;
public:
	
	BCSheet() {

	}
	BCSheet(HexV3fMesh* mesh, std::vector<BaseComplex>&  bc, EdgeHandle init_edge) :BCHexahedron(mesh, bc) {
		this->ExtracteSurfuceFeature();
		//提取对偶面
		this->setDualFace(init_edge);
	}

	BCSheet(const BCSheet& bcs):BCHexahedron(bcs) {
		
		face2cells = bcs.face2cells;
		surface_2part = bcs.surface_2part;
		bc_2part = bcs.bc_2part;
		index_dual_face = bcs.index_dual_face;
	}
	BCSheet& operator =(const BCSheet& bcs) {
		BCHexahedron::operator=(bcs);
		face2cells = bcs.face2cells;
		surface_2part = bcs.surface_2part;
		bc_2part = bcs.bc_2part;
		index_dual_face = bcs.index_dual_face;
	}
	//塌陷操作
	bool Collapse2DualFace(std::vector<EdgeHandle>& initEdge);

	
	void setDualFace(EdgeHandle init_edge);
};



//该类负责对整个网格中的sheet提取和sheet邻接关系判断
class SheetOrChordHandler {
public :
	HexV3fMesh *mesh;
	std::set<BaseComplex, compare_BaseComplex> set_bc_now;
	std::vector<BCSheet> vec_sheet;
	std::map<uint32_t, bool> bc_is_checked;
	SheetOrChordHandler() {

	}
	SheetOrChordHandler(HexV3fMesh *mesh,std::set<BaseComplex,compare_BaseComplex>& set_bc) {
		this->mesh = mesh;
		set_bc_now = set_bc;
		for (std::set<BaseComplex, compare_BaseComplex>::iterator iter = set_bc.begin(); iter != set_bc.end(); ++iter) {
			bc_is_checked.insert(std::make_pair((*iter).Idx(),false));
		}
	}
	//选取sheet层
	BCSheet SelectOneSheet();

	bool CollapseOneSheet(BCSheet& bc_sheet);

};
class BCChord : public BCHexahedron {
private:
	//计数
	uint16_t w, h;

	//起始线
	std::vector<EdgeHandle> vec_init_edges;

	std::vector<std::pair<FaceHandle, FaceHandle>> vec_pair_f;

	//对角线 对称点对
	std::map<VertexHandle, std::pair<VertexHandle, VertexHandle>> map_pair_o;

	//提取对角面(参数未定 确定方向)
	void extractDiagonalFaces();
public:
	BCHexahedron *bc;
	HexV3fMesh *mesh;
	BCChord() {

	}
	BCChord(HexV3fMesh* mesh, BCHexahedron& bc);

	//塌陷操作
	bool Collapse2DiagonalFace(std::vector<EdgeHandle>& initEdge);

	void setDualFace(uint8_t init_edge_index, uint8_t ori_index_1, uint8_t ori_index_2);
};
#endif // !SHEET_OPERATION_H

