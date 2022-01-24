#ifndef BASECOMPLEX_SIMPLIFY_H
#define BASECOMPLEX_SIMPLIFY_H
#include "ComHeader.h"
#include "BaseComplex.h"

static const unsigned char U_RELOCATED_CELL_OUT_INDEX[8][3] = {
	{ 0,3,4 },
	{ 0,2,4 },
	{ 0,2,5 },
	{ 0,3,5 },
	{ 1,3,4 },
	{ 1,3,5 },
	{ 1,2,5 },
	{ 1,2,4 }
};
//0-7��Ӧ�ڲ�����
static const unsigned char U_RELOCATED_CELL_INDEX[8][3] = {
	{ 1,2,5 },
	{ 1,3,5 },
	{ 1,3,4 },
	{ 1,2,4 },
	{ 0,2,5 },
	{ 0,2,4 },
	{ 0,3,4 },
	{ 0,3,5 }
};


class BaseComplexSimplify {

private:
	HexV3fMesh * mesh;

	//�ָ��漯��
	std::set<FaceHandle, compare_OVM> f_all_set;

	//�ָ��bc����
	std::set<BaseComplex, compare_BaseComplex> bc_set;
public:
	BaseComplexSimplify(HexV3fMesh * mesh, std::set<FaceHandle, compare_OVM> f_all, std::set<BaseComplex, compare_BaseComplex> baseComplexSet);
	void ExtendBlock2One(HexV3fMesh* re_meshing_result);
	//����cornerλ�÷���-1 ����λ0-7
	std::map<unsigned char, VertexHandle> CheckIndexInCorner(BaseComplex &bc);

	
};
#endif // !BASECOMPLEX_SIMPLIFY_H



