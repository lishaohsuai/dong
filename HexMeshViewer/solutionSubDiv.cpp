
#include "solutionSubDiv.h"

/**************************************************
@brief   : ����mlca�Ľ������  ��������ؽ���Ȼ����һ��
@author  : lee
@input   ��mesh
@output  ��none
@time    : none
**************************************************/
void SolutionSubDiv::split(HexV3fMesh &mesh) {
	std::cout << "[DEBUG] split " << std::endl;
	mlcaSubdivision = new MLCASubdivision();
	mlcaSubdivision->Spliting(mesh);
}


/**************************************************
@brief   : �����������ֿ�
@author  : lee
@input   ��none
@output  ��none
@time    : none
**************************************************/
void SolutionSubDiv::average(HexV3fMesh &mesh) {
	std::cout << "[DEBUG] average " << std::endl;
	mlcaSubdivision = new MLCASubdivision();
	mlcaSubdivision->Averaging(mesh);
}

