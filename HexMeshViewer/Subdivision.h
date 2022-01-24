#pragma once

#include "BaseComplex.h"
#include "GeometricTools\wm4bsplinerectangle.h"
#include "BSplineSolid.h"

/**
 * @description: This class completes the split operation
 */
class Split
{
public:
	HexV3fMesh *mesh;
	HexV3fMesh *newmesh;
	std::vector<V3f> v_crease;	
	std::vector<double> material_distribution;
public:
	Split() {};
	~Split() {};
	Split(HexV3fMesh *_mesh, HexV3fMesh *_newmesh) :mesh(_mesh), newmesh(_newmesh) {};
	Split(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<V3f> _v_crease) :mesh(_mesh), newmesh(_newmesh),v_crease(_v_crease){};
	Split(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<double> _material_distribution) :mesh(_mesh), newmesh(_newmesh) 
	, material_distribution(_material_distribution){};
	HexV3fMesh& Spliting();
	//HexV3fMesh& Spliting1();
	HexV3fMesh& Spliting_crease();
};


/**
 * @description: This class completes the average operation
 */
class Average
{
public:
	HexV3fMesh *mesh;
	std::vector<V3f> v_crease;
	std::vector<V3f> v_c_crease;
	std::vector<VertexHandle> v_singular;
	std::vector<V3f> v3f_singular;
	Average() {};
	~Average() {};
	Average(HexV3fMesh *_mesh) :mesh(_mesh) {};
	Average(HexV3fMesh *_mesh, std::vector<VertexHandle> _v_singular) :mesh(_mesh),v_singular(_v_singular) {};
	Average(HexV3fMesh *_mesh, std::vector<V3f> _v3f_singular) :mesh(_mesh), v3f_singular(_v3f_singular){};
	Average(HexV3fMesh *_mesh, std::vector<V3f> _v_crease, std::vector<V3f> _v_c_crease) :mesh(_mesh), v_crease(_v_crease), v_c_crease(_v_c_crease){};
public:
	HexV3fMesh& Averaging();//��ͨϸ��
	HexV3fMesh& Averaging_creases();//�����ۺۣ��д���
	HexV3fMesh& Averaging_singular();//���������
	HexV3fMesh& Averaging_test();//���ڲ���
};

class Subdivision //�����ò�ֵϸ��ֱ�ӽ���ϸ�ֲ���
{
public:
	HexV3fMesh *mesh;
	HexV3fMesh *newmesh;
	const double w = 1.0 / 16;
	Subdivision(HexV3fMesh *_mesh, HexV3fMesh *_newmesh) :mesh(_mesh), newmesh(_newmesh) {};
private:
	bool check_face(FaceHandle _f);//�ж��Ƿ�������ϸ��
	bool check_edge(EdgeHandle _e);
	V3f cal_cell_mid(const int cell_num);//�ֲ��������ܷ�������
	V3f cal_face_mid(const int cell_num, const int face_num);
	V3f cal_edge_mid(const int cell_num, const int ver1, const int ver2);
public:
	HexV3fMesh& Subdivising();
};

class CCSubdivision//������CCϸ��
{
public:
	HexV3fMesh *mesh;
	HexV3fMesh *newmesh;
	//std::vector<bool> e_cs;//�ۺ۱�
	std::vector<bool> v_cs;//�ۺ۵�
	std::vector<EdgeHandle> sharpedge;
	CCSubdivision(HexV3fMesh *_mesh, HexV3fMesh *_newmesh) :mesh(_mesh), newmesh(_newmesh) {};
	CCSubdivision(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<bool> _v_cs) :
		mesh(_mesh), newmesh(_newmesh), v_cs(_v_cs){};
	CCSubdivision(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<EdgeHandle> _sharpedge) :
		mesh(_mesh), newmesh(_newmesh), sharpedge(_sharpedge) {};
	CCSubdivision(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<double> _material_distribution)
		:mesh(_mesh), newmesh(_newmesh) , material_distribution(_material_distribution){};
protected:
	V3f cal_cell_mid(const int cell_num);
	V3f cal_cell_midv(CellHandle cell_id);

	V3f cal_face_mid(const int cell_num, const int face_num);
	V3f cal_face_mid(FaceHandle face_id);
	V3f cal_face_midv(FaceHandle face_id);

	V3f cal_edge_mid(const int cell_num, const int ver1, const int ver2);
	V3f cal_edge_mid_1(const int cell_num, const int ver1, const int ver2);
	V3f cal_edge_mid(EdgeHandle edge_id);
	V3f cal_edge_midv(EdgeHandle edge_id);

	V3f cal_new_vertex(const int cell_num, const int ver_num);
	V3f cal_new_vertex_1(const int cell_num, const int ver_num);
	V3f cal_new_vertex(VertexHandle ver_id);
protected:
	V3f cal_new_csv(const int cell_num, const int ver_num);
	V3f cal_new_csv_se(const int cell_num, const int ver_num);
	V3f cal_es_mid(const int cell_num, const int ver1, const int ver2);
	V3f cal_es_mid_se(const int cell_num, const int ver1, const int ver2);
	EdgeHandle check_edge(const int cell_num, const int ver1, const int ver2);
public:
	void check_cse()
	{
		//int tmp = 0;
		//e_cs.resize(mesh->edges_.size(), false);
		//for (int i = 0;i < mesh->edges_.size();i++)
		//{
		//	if (v_cs[mesh->edges_[i].from_h] == true && v_cs[mesh->edges_[i].to_h] == true)
		//	{
		//		tmp++;
		//		e_cs[i] = true;
		//	}
		//		//e_cs[i] = true;
		//}
		//mesh->e_cs = e_cs;
		mesh->v_cs = v_cs;
	}
	bool is_cs(VertexHandle _v)
	{
		return v_cs[_v];
	}
	bool is_cs(EdgeHandle _e)
	{
		if (v_cs[mesh->edges_[_e].from_h] == true && v_cs[mesh->edges_[_e].to_h] == true)
			return true;
		else
			return false;
		//return e_cs[_e];
	}
	bool is_cs_se(VertexHandle _v)
	{
		for (int i = 0; i < sharpedge.size(); ++i)
		{
			if (_v == mesh->edges_[sharpedge[i]].from_h || _v == mesh->edges_[sharpedge[i]].to_h)
				return true;
		}
		return false;
	}
	bool is_cs_se(EdgeHandle _e)
	{
		if (std::find(sharpedge.begin(), sharpedge.end(), _e) == sharpedge.end())
			return false;
		else
			return true;
	}
public:
	HexV3fMesh& CCSubdivising();
	HexV3fMesh& CCSubdivising_1();
	HexV3fMesh& CCSubdivising_new();//�����ظ��жϵ��Ƿ��ظ�
	HexV3fMesh& Padding(); // padding
	HexV3fMesh& Money(); // ��27�ȷ�
	HexV3fMesh& Money_right(); // ����money
	HexV3fMesh& CCSubWithCrease();//��CCϸ���������ۺ�
	HexV3fMesh& CCSubWithCrease_sharpedge();//��CCϸ���������ۺ�
	HexV3fMesh& FirstCCSubdivising();//���ڵ�һ��ϸ�� ��ǵ� ������Bezier��ıȽ�
	HexV3fMesh& SecondCCSubdivising();//��������ϸ�� ��ǵ� ������Bezier��ıȽ�
public:
	std::vector<double> material_distribution;
	//std::vector<std::vector<double>> material_distribution_mutires;//���Ϸֲ�
};

class InterpolateCC : public CCSubdivision //����CCϸ�ֵĲ�ֵϸ��
{
public:
	InterpolateCC(HexV3fMesh *_mesh, HexV3fMesh *_newmesh) :CCSubdivision(_mesh,_newmesh){};
	InterpolateCC(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, HexV3fMesh *_tempmesh) :CCSubdivision(_mesh, _newmesh),tempmesh(_tempmesh) {};
	void PBfirstep();//����CCϸ�ֵõ�Ҫ�õĵ����firstver
	void NBfirstep();//��ÿ���������
	void MLCAfirstep();
private:
	/*-----------Push-Back��-----------*/
	std::vector<V3f> firstver;//��һ���õ��ĵ�
	std::vector<V3f> detver;//��һ���õ��ĵ���ԭ����Ĳ�ֵ
	/*std::vector<V3f> firstedge;//��һ���õ��ıߵ�
	std::vector<V3f> detedge;//��һ���õ��������ԭ���ߵ�Ĳ�ֵ
	std::vector<V3f> firstface;//��һ���õ������
	std::vector<V3f> detface;//��һ���õ��������ԭ�����Ĳ�ֵ
	std::vector<V3f> oriv;//ԭʼģ�͵ĵ�*/
	std::vector<V3f> ev;
	std::vector<V3f> fv;
	std::vector<V3f> cv;
	std::vector<V3f> vv;
	/*const double le = 0.05;
	const double uf = 0.05;
	const double dc = 0.05;*/
	const double le = 0.25;
	const double uf = 0.125;
	const double dc = 0.0625;
	const double sv = 0.5;
	/*const double le = 0.75;
	const double uf = 0.625;
	const double dc = 0.5;*/
	/*const double le = 0.5;
	const double uf = 0.5;
	const double dc = 0.25;*/
	V3f cal_CIPB_ev(const int cell_num, const int ver1, const int ver2);
	V3f cal_CIPB_ev(EdgeHandle edge_id);
	V3f cal_CIPB_fv(const int cell_num, const int face_num);
	V3f cal_CIPB_fv(FaceHandle face_id);
	V3f cal_CIPB_cv(const int cell_num);
	V3f cal_CIPB_cv(CellHandle cell_id);
	V3f cal_CIPB_nv(const int cell_num, const int ver_num);
	V3f cal_CIPB_nv(VertexHandle ver_id);
	V3f cal_CIPB_fnv(VertexHandle ver_id);
	/*-----------Normal-Based��-----------*/
	std::vector<V3f> normalver;//����ÿ���������
	const double we = 0.25;
	const double vf = 0.125;
	V3f cal_CINB_ev(const int cell_num, const int ver1, const int ver2);
	V3f cal_CINB_ev(EdgeHandle edge_id);
	V3f cal_CINB_fv(const int cell_num, const int face_num);
	V3f cal_CINB_fv(FaceHandle face_id);
	V3f cal_CINB_nv(const int cell_num, const int ver_num);
	/*-----------MLCA����-----------*/
	HexV3fMesh *tempmesh;
	void Spliting();
	//void Averaging();
	/*std::vector<V3f> new_vv;
	std::vector<V3f> new_ev;
	std::vector<V3f> new_fv;
	std::vector<V3f> new_cv;*/
	std::vector<V3f> mlcadet;
	std::vector<VertexHandle> vid;
	std::vector<EdgeHandle> eid;
	std::vector<FaceHandle> fid;
	std::vector<CellHandle> cid;
	V3f cal_MLCA_ev(const int cell_num, const int ver1, const int ver2);
	V3f cal_MLCA_ev(EdgeHandle edge_id);
	V3f cal_MLCA_fv(const int cell_num, const int face_num);
	V3f cal_MLCA_fv(FaceHandle face_id);
	V3f cal_MLCA_cv(const int cell_num);
	V3f cal_MLCA_cv(CellHandle cell_id);
	V3f cal_MLCA_nv(const int cell_num, const int ver_num);
	V3f cal_mlca_vertex(VertexHandle ver_id);
public:
	HexV3fMesh& CCInterpolatingPB();
	HexV3fMesh& CCInterpolatingPB1();
	HexV3fMesh& CCInterpolatingNB();
	HexV3fMesh& CCInterpolatingMLCA();
};

class ApproximationCC : public CCSubdivision
{
public:
	ApproximationCC(HexV3fMesh *_mesh, HexV3fMesh *_newmesh) :CCSubdivision(_mesh, _newmesh) {};
	ApproximationCC(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<std::vector<std::vector<std::vector<V3f>>>> _SolidData)
		:CCSubdivision(_mesh, _newmesh), SolidData(_SolidData){};
	ApproximationCC(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<V3f> _vn) :
		CCSubdivision(_mesh, _newmesh),vn(_vn) {};
	ApproximationCC(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<std::vector<V3f>> _curve_v, 
		std::vector<FaceHandle> _sharpface, std::vector<int> _sharpnum) :
		CCSubdivision(_mesh, _newmesh), curve_v(_curve_v), sharpface(_sharpface), sharpnum(_sharpnum){};
	ApproximationCC(HexV3fMesh *_mesh, HexV3fMesh *_newmesh, std::vector<EdgeHandle> _sharpedge) 
		:CCSubdivision(_mesh, _newmesh) , sharpedge(_sharpedge){};
	std::vector<V3f> vn;
	std::vector<std::vector<V3f>> surv;
	std::vector<V3f> nor_surv;//������
	std::vector<std::vector<std::vector<V3f>>> beiv;
	std::vector<std::vector<V3f>> bei_surv;
	std::vector<V3f> nor_bei_surv;
	std::vector<std::vector<std::vector<V3f>>> bei_surv_dat;
	std::vector<std::vector<V3f>> nor_bei_surv_dat;
	std::vector<std::vector<V3f>> edgeline;
	std::vector<V3f> bei_surv_center;
	std::vector<FaceHandle> biaojiface;
	void approximationing();
	void approximationing(int);
	void cal_nor_sur();
	void constructBezier();
	void cal_nor_beisur();
	void constructBezier(int);
	void cal_nor_beisur(int);
	void cal_nor_beisur1(int);
	void cal_nor_beisur1();
	void cal_nor_beisur(double);
	void cal_beisurv_center(int);
	std::vector<std::vector<std::vector<V3f>>> isoline;//�Ȳ���
	void constructBezierSharp();
	void approximationingSharp();
	std::vector<std::vector<V3f>> curve_v;
	std::vector<FaceHandle> sharpface;//��������ߵ���
	std::vector<int> sharpnum;//��������ڵڼ�������
	std::vector<EdgeHandle> sharpedge;
public:
	V3f cal_corner_point(VertexHandle _v);
	//V3f cal_corner_point_1(VertexHandle _v,CellHandle _c);
	V3f cal_edge_point(EdgeHandle _e, VertexHandle _v);
	//V3f cal_edge_point_1(EdgeHandle _e, VertexHandle _v, CellHandle _c);
	V3f cal_inter_point(FaceHandle _f, VertexHandle _v);

	V3f cal_corner_point2(VertexHandle _v);
	V3f cal_edge_point2(EdgeHandle _e, VertexHandle _v);
	V3f cal_inter_point2(FaceHandle _f, VertexHandle _v);

	V3f cal_inter_point(CellHandle _c, VertexHandle _v);
	V3f solid_edge_point(EdgeHandle _e, VertexHandle _v);
	V3f solid_edge_point_1(EdgeHandle _e, VertexHandle _v,CellHandle _c);
	V3f solid_face_point(FaceHandle _f, VertexHandle _v);
	V3f solid_corner_point(VertexHandle _v);
	V3f solid_corner_point_1(VertexHandle _v, CellHandle _c);
	V3f solid_edge_point(int cell_num, int edge_num, int vertex_num);
	V3f solid_edge_point_1(int cell_num, int edge_num, int vertex_num);
	V3f solid_face_point(int cell_num, int face_num, int vertex_num);
	/*������������������бȽ�*/
	V3f cal_inter_point1(FaceHandle _f, VertexHandle _v);
	V3f cal_inter_point1(CellHandle _c, VertexHandle _v);
	V3f solid_edge_point1(EdgeHandle _e, VertexHandle _v);
	V3f solid_face_point1(FaceHandle _f, VertexHandle _v);
	V3f solid_corner_point1(VertexHandle _v);
	V3f solid_edge_point1(int cell_num, int edge_num, int vertex_num);
	V3f solid_face_point1(int cell_num, int face_num, int vertex_num);
public:
	V3f cal_corner_pointsharp(VertexHandle _v);
	V3f cal_edge_pointsharp(EdgeHandle _e, VertexHandle _v);
	V3f cal_inter_pointsharp(FaceHandle _f, VertexHandle _v);
public:
	std::vector<std::vector<std::vector<std::vector<V3f>>>> SolidData;
	HexV3fMesh& CCapproximation();
	HexV3fMesh& CCapproximation1();
	HexV3fMesh& CCapproximation_1();//����ȡ��������
	HexV3fMesh constructBezierSolid();
	std::vector<HexV3fMesh>& CCapproximationDAT();
	std::vector<HexV3fMesh>&  constructBezierSolidDAT();
	std::vector<HexV3fMesh> vmesh;
	std::vector<HexV3fMesh> vBmesh;
	std::vector<V3f> ccver;//��ģ����ÿ�����ӦCCϸ�ֵļ��޵㣬���ڱȽ�
public:
	void constructBezierSolidDAT_quick();
	std::vector<std::vector<std::vector<V3f>>> vmesh_quick;
	std::vector<std::vector<V3f>> line_quick;
	//����һ�����ø÷����õ��ĵ�
	void cal_ccver();
	//����һ�����Ӧ��CCϸ�ֵļ��޵�
	void cal_limitccver();
	/*������������������бȽ�*/
	HexV3fMesh& CCapproximation2();
	HexV3fMesh& CCapproximation3();
	std::vector<HexV3fMesh>& CCapproximationDAT1();
	V3f selectapoint();
	BSplineSolid bss;
	void construct_bss();//4*4*4�����Ƶ�
	void construct_bss1();//3*3*3�����Ƶ�
	void construct_bss_new();
	void construct_bss_new_new();
	void construct_bss1_new();//�����ظ�����
	void construct_bss1_new_new();
};

class TwoNSubdivision: public CCSubdivision
{
public:
	TwoNSubdivision(HexV3fMesh *_mesh, HexV3fMesh *_newmesh) :CCSubdivision(_mesh, _newmesh) {};
	TwoNSubdivision(HexV3fMesh *_mesh, HexV3fMesh *_newmesh,int _iteration_num, std::vector<V3f> _oriv) 
		:CCSubdivision(_mesh, _newmesh), iteration_num(_iteration_num), oriv(_oriv){};
private:
	int iteration_num;
	std::vector<std::vector<V3f>> edgepoint;
	void cal_edgepoint_withn();
	V3f cal_facepoint_withn(FaceHandle _f);
	V3f cal_facepoint_withn(FaceHandle _f,int n);
	void cal_facepoint_withn_solid();
	std::vector<std::vector<V3f>> facepointsolid;
	std::vector<V3f> select_face_line(FaceHandle _f, EdgeHandle _e);//��һ����������бߵ�ȫ����������
	std::vector<V3f> oriv;//�����ж���Щ���ǳ�ʼ��
	V3f cal_cellpoint_withn(CellHandle _c);
	std::vector<V3f> select_cell_line(CellHandle _c, FaceHandle _f);//��һ��������������ȫ����������
private:
	V3f cal_twon_edge_point(const int cell_num, const int ver1, const int ver2);
	V3f cal_twon_face_point(const int cell_num, const int face_num);
	V3f cal_twon_new_vertex(const int cell_num, const int ver_num);
public:
	HexV3fMesh& TwoNSubdiving();
};

class QuadCC
{
public:
	QuadV3fMesh *mesh;
	QuadV3fMesh *newmesh;
	std::vector<bool> v_cs;//�ۺ۵�
	std::vector<bool> vdraw;
	QuadCC(QuadV3fMesh *_mesh, QuadV3fMesh *_newmesh) :mesh(_mesh), newmesh(_newmesh) {};
	QuadCC(QuadV3fMesh *_mesh, QuadV3fMesh *_newmesh, std::vector<bool> _v_cs, std::vector<bool> _vdraw) :
		mesh(_mesh), newmesh(_newmesh), v_cs(_v_cs) , vdraw(_vdraw) {};
protected:
	V3f cal_face_mid(FaceHandle face_id);
	V3f cal_face_midv(FaceHandle face_id);

	V3f cal_edge_mid(EdgeHandle edge_id);
	V3f cal_edge_midv(EdgeHandle edge_id);

	V3f cal_new_vertex(VertexHandle ver_id);
protected:
	V3f cal_new_csv(VertexHandle ver_id);
	V3f cal_es_mid(EdgeHandle edge_id);
	bool is_cs(VertexHandle _v)
	{
		return v_cs[_v];
	}
	bool is_cs(EdgeHandle _e)
	{
		if (v_cs[mesh->edges_[_e].from_h] == true && v_cs[mesh->edges_[_e].to_h] == true)
			return true;
		else
			return false;
	}
public:
	QuadV3fMesh& CCSubWithCrease();
};

