#ifndef BASE_COMPLEX_H
#define BASE_COMPLEX_H

#include "ComHeader.h"
#include "SingularObject.h"


#define INVALID_ORIENTATION 6

/*
BaseComplex������֧�ֵ��Ż�����
*/
class BaseComplex {
private:
	//�㼯 (�߽�)
	std::set<VertexHandle, compare_OVM> complex_ves;
	//�㼯 ���ڲ���
	std::set<VertexHandle, compare_OVM> complex_ves_inner;
	//�Ա߽�������
	std::set<FaceHandle, compare_OVM> complex_fs;
	//�߽�߼�
	std::set<EdgeHandle, compare_OVM> complex_seds;
	//��Ԫ��
	std::set<CellHandle, compare_OVM> complex_hexcells;

	int idx = 0;
public:
	//���ƺ���
	//����=
	BaseComplex& operator=(const BaseComplex&base_complex) {
		this->complex_hexcells.swap(std::set<CellHandle, compare_OVM>());
		this->complex_fs.swap(std::set<FaceHandle, compare_OVM>());
		this->complex_seds.swap(std::set<EdgeHandle, compare_OVM>());
		this->complex_ves.swap(std::set<VertexHandle, compare_OVM>());
		this->complex_ves_inner.swap(std::set<VertexHandle, compare_OVM>());

		idx = base_complex.idx;
		

		this->complex_hexcells.insert(base_complex.complex_hexcells.begin(), base_complex.complex_hexcells.end());
		this->complex_fs.insert(base_complex.complex_fs.begin(), base_complex.complex_fs.end());
		this->complex_seds.insert(base_complex.complex_seds.begin(), base_complex.complex_seds.end());
		this->complex_ves.insert(base_complex.complex_ves.begin(), base_complex.complex_ves.end());
		this->complex_ves_inner.insert(base_complex.complex_ves_inner.begin(), base_complex.complex_ves_inner.end());
		return *this;
	}

	BaseComplex(const BaseComplex& base_complex) {
		this->complex_hexcells.swap(std::set<CellHandle, compare_OVM>());
		this->complex_fs.swap(std::set<FaceHandle, compare_OVM>());
		this->complex_seds.swap(std::set<EdgeHandle, compare_OVM>());
		this->complex_ves.swap(std::set<VertexHandle, compare_OVM>());
		this->complex_ves_inner.swap(std::set<VertexHandle, compare_OVM>());

		idx = base_complex.idx;
	

		this->complex_hexcells.insert(base_complex.complex_hexcells.begin(), base_complex.complex_hexcells.end());
		this->complex_fs.insert(base_complex.complex_fs.begin(), base_complex.complex_fs.end());
		this->complex_seds.insert(base_complex.complex_seds.begin(), base_complex.complex_seds.end());
		this->complex_ves.insert(base_complex.complex_ves.begin(), base_complex.complex_ves.end());
		this->complex_ves_inner.insert(base_complex.complex_ves_inner.begin(), base_complex.complex_ves_inner.end());
	}
	BaseComplex() {};
	BaseComplex(std::set<VertexHandle, compare_OVM> complex_ves,
		std::set<FaceHandle, compare_OVM> complex_fs,
		std::set<EdgeHandle, compare_OVM> complex_seds,
		std::set<CellHandle, compare_OVM> complex_hexcells);

	void setIdx(const int& x);
	const int& Idx()const;
	bool insertVertex(VertexHandle ve, bool is_inner);
	bool insertFace(FaceHandle f);
	bool insertEdge(EdgeHandle ed);
	bool insertCellHandle(CellHandle ce);

	std::set<FaceHandle, compare_OVM> getFaceSet() {
		return complex_fs;
	}
	std::set<CellHandle, compare_OVM> getCellSet() {
		return complex_hexcells;
	}
	std::set<EdgeHandle, compare_OVM> getSedSet() {
		return complex_seds;
	}
	std::set<VertexHandle, compare_OVM> getVes() {
		return complex_ves;
	}
	std::set<VertexHandle, compare_OVM> getVesInner() {
		return complex_ves_inner;
	}

	bool find_cell(CellHandle cell) {
		if (complex_hexcells.find(cell) == complex_hexcells.end()) {
			return false;
		}
		return true;
	}
	bool find_face(FaceHandle face) {
		if (complex_fs.find(face) == complex_fs.end()) {
			return false;
		}
		return true;
	}
	
};
struct compare_BaseComplex
{
	bool operator()(const BaseComplex &a1, const BaseComplex &b1) const
	{
		return a1.Idx()<b1.Idx();
	}
};

//���μ���м���
class IntermediateBCResult {
private:
	std::set<VertexHandle,compare_OVM> ibc_vec_bdy_v;
	std::vector<EdgeHandle> ibc_vec_edge;
	std::vector<CellHandle> ibc_vec_cell;
	std::vector<bool> b_vec_bdy_edge;
	
	std::vector<FaceHandle> ibc_vec_bdy_face;
public:
	bool is_fined_local_se;
	std::vector<EdgeHandle> edge_feature;
	std::vector<EdgeHandle> edge_singular_addin;
	//Ϊ��block�����ڽӹ�ϵ
	std::vector<std::vector<FaceHandle>> e_neibor;
	
	HexV3fMesh *mesh;

	IntermediateBCResult(HexV3fMesh *_mesh ,std::vector<CellHandle>& _ibc_vec_cell) :
		ibc_vec_cell(_ibc_vec_cell), is_fined_local_se(false),mesh(_mesh) {};
	//���߽����� ��ȡsingular
	bool DetecteFeature(const std::vector<FaceHandle>& vec_de_face);
	
	std::vector<VertexHandle> ExtractCornerVertex();
	~IntermediateBCResult() {
		/*ibc_vec_bdy_v.swap(std::set<VertexHandle, compare_OVM>()) ;
		ibc_vec_edge.swap(std::vector<EdgeHandle>());
		ibc_vec_bdy_face.swap(std::vector<FaceHandle>());
		ibc_vec_cell.swap(std::vector<CellHandle>());
		b_vec_bdy_edge.swap(std::vector<bool>());

		ibc_vec_bdy_face.swap(std::vector<FaceHandle>());
		edge_feature.swap(std::vector<EdgeHandle>());*/
	}
};
static void writeBC2Mesh(BaseComplex &basecomplex ,HexV3fMesh* mesh, HexV3fMesh* newmesh) {
	std::set<CellHandle, compare_OVM> set_cell = basecomplex.getCellSet();
	std::set<VertexHandle, compare_OVM> set_v;
	
	//��������һ��``` ���д�
	for (std::set<CellHandle, compare_OVM>::iterator iter = set_cell.begin(); iter != set_cell.end(); ++iter) {
		for (uint8_t index_v = 0; index_v < 8; ++index_v) {
			set_v.insert(mesh->cells_[*iter].vertex_[index_v]);
		}
	}

	for (std::set<VertexHandle, compare_OVM>::iterator it = set_v.begin(); it != set_v.end(); ++it) {
		newmesh->add_vertex(mesh->vertices_[*it]);
	}
	
	for (std::set<CellHandle, compare_OVM>::iterator it = set_cell.begin(); it != set_cell.end(); ++it) {
		std::vector<VertexHandle> add_v_set(8);
		std::vector<VertexHandle> vec_v = mesh->cells_[*it].vertex_;
		std::set<VertexHandle, compare_OVM>::iterator iter_v;
		for (int i = 0; i < 8; ++i) {
			iter_v = set_v.find(vec_v[i]);
			
			if (iter_v == set_v.end()) {
				std::cout << "error\n";
				return;
			}
			add_v_set[i] = VertexHandle(std::distance(set_v.begin(), iter_v));
		}
		newmesh->add_cell(add_v_set);
	}
}

static void writeFaceSet2Mesh(std::set<FaceHandle,compare_OVM> &face_vec, HexV3fMesh* mesh, HexV3fMesh* newmesh) {
	std::vector<VertexHandle> result_v;
	for (std::set<FaceHandle,compare_OVM>::iterator iter = face_vec.begin(); iter != face_vec.end(); ++iter) {
		std::vector<VertexHandle> set_v = mesh->faces_[(*iter)].vs_f;
		for (int i = 0; i < 4; ++i) {
			if (std::find(result_v.begin(), result_v.end(), set_v[i]) == result_v.end()) {
				result_v.push_back(set_v[i]);
			}
		}
		
	}
	for (std::vector<VertexHandle>::iterator it = result_v.begin(); it != result_v.end(); ++it) {
		newmesh->add_vertex(mesh->vertices_[*it]);
	}
	
	for (std::set<FaceHandle, compare_OVM>::iterator it = face_vec.begin(); it != face_vec.end(); ++it) {
		
		std::vector<VertexHandle> add_v_set(4);
		std::vector<VertexHandle> vec_v = mesh->faces_[*it].vs_f;
		std::vector<VertexHandle>::iterator iter_v;
		for (int i = 0; i < 4; ++i) {
			iter_v = std::find(result_v.begin(), result_v.end(), vec_v[i]);
			add_v_set[i] = VertexHandle(iter_v - result_v.begin());
		}
		newmesh->add_face(add_v_set);
	}
}

/*
�ò�����Ҫ����
1���Էָ����̰�Ĳ���
2���Էָ���Ϊ�߽��������OVM��HEX֧�� ʵ���ڸ��������ϵķ������
3���������Ĳ��ֺ�����ṹ����Ϊ���ɲ�������Ľṹ����
*/
class BaseComplexSetFiller {
private:
	HexV3fMesh *mesh;

	//��ʱ��cell���ϴ洢 
	std::set<CellHandle, compare_OVM> cell_temp_set;

	//�޷������չ�����
	int GetSFaceExtend_Simple(const EdgeHandle& he, const FaceHandle& hf,const std::vector<unsigned char> &intEProp_sigular);

public:
	
	//�ָ��漯��
	std::set<FaceHandle, compare_OVM> f_all_set;

	//�����߽�ķָ��漯��


	BaseComplexSetFiller(HexV3fMesh* hexMesh) {
		mesh = hexMesh;
	};
	

	int SFaceSetSeeking_Simple(const std::vector<unsigned char>& intEProp_sigular);

	//��մ洢
	void EraseSpace();

	//�����������
	void HexCellFlooding_BFS(std::vector<bool>& boolCellChecked, const CellHandle& initHandle = CellHandle());
	void HexCellFlooding_Recursive(const unsigned char from_oriention, std::vector<bool>& boolCellChecked, const CellHandle& initHandle = CellHandle());

	//�ڼ��׶η��ִ��ڶ���8�ǵ�����������
	std::vector<EdgeHandle> OneBaseComplexFilling( std::set<BaseComplex, compare_BaseComplex>& baseComplexSet, const CellHandle& initHandle = CellHandle());

	bool DetectSingularityInBC(BaseComplex& bc, std::vector<FaceHandle>& face_sep_vec, std::vector<EdgeHandle>& edge_addin);
	void InsertObject2BaseComplex(BaseComplex& baseComplex, std::set<VertexHandle, compare_OVM>& surface_vex_set);
};


#endif // !BASE_COMPLEX_H
