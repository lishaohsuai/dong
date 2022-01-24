
#include "solutionSubDiv.h"

/**************************************************
@brief   : 对于mlca的解决方案  将网格加载进来然后处理一下
@author  : lee
@input   ：mesh
@output  ：none
@time    : none
**************************************************/
void SolutionSubDiv::split(HexV3fMesh &mesh) {
	std::cout << "[DEBUG] split " << std::endl;
	mlcaSubdivision = new MLCASubdivision();
	mlcaSubdivision->Spliting(mesh);
}


/**************************************************
@brief   : 将两个动作分开
@author  : lee
@input   ：none
@output  ：none
@time    : none
**************************************************/
void SolutionSubDiv::average(HexV3fMesh &mesh) {
	std::cout << "[DEBUG] average " << std::endl;
	mlcaSubdivision = new MLCASubdivision();
	mlcaSubdivision->Averaging(mesh);
}

