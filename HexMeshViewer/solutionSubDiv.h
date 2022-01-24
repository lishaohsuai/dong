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
 * 思路：
 * 1. 构建骨架结构体，现在已经使用id来构建整个骨架有拓扑关系和邻接矩阵来表示(OK)
 * 2. 生成节点六面体(OK)
 * 3. 调整节点六面体, 角度(OK)
 * 4. 生成边六面体(OK)
 * 4.1 重构连接关系(OK)
 * 4.2 对每个节点观察邻接节点, 确认是否存在大面对小面的情况()
 * 4.3 如果存在大面对小面的情况, 将大面根据小面不在大面的边进行剖分
 * 4.4 重建连接关系()
 * 5. 输出mesh(OK)
 *
 */

class SolutionSubDiv {
public:
	// 构造和析构函数都采用默认的
	SolutionSubDiv() {};
	~SolutionSubDiv() {};
public:
	// MLCA两个操作
	void split(HexV3fMesh &mesh);
	void average(HexV3fMesh &mesh);
private:
	MLCASubdivision *mlcaSubdivision;
};

#endif
