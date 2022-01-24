#ifndef __SOLUTIONSUBDIV_H__
#define __SOLUTIONSUBDIV_H__
#include "HexMeshViewer.h"
#include "mlca_subdivision.h"
#include "string.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>

/**
 * ˼·��
 * 1. �����Ǽܽṹ�壬�����Ѿ�ʹ��id�����������Ǽ������˹�ϵ���ڽӾ�������ʾ(OK)
 * 2. ���ɽڵ�������(OK)
 * 3. �����ڵ�������, �Ƕ�(OK)
 * 4. ���ɱ�������(OK)
 * 4.1 �ع����ӹ�ϵ(OK)
 * 4.2 ��ÿ���ڵ�۲��ڽӽڵ�, ȷ���Ƿ���ڴ����С������()
 * 4.3 ������ڴ����С������, ���������С�治�ڴ���ı߽����ʷ�
 * 4.4 �ؽ����ӹ�ϵ()
 * 5. ���mesh(OK)
 *
 */

class SolutionSubDiv {
public:
	// �������������������Ĭ�ϵ�
	SolutionSubDiv() {};
	~SolutionSubDiv() {};
public:
	// MLCA��������
	void split(HexV3fMesh &mesh);
	void average(HexV3fMesh &mesh);
private:
	MLCASubdivision *mlcaSubdivision;
};

#endif
