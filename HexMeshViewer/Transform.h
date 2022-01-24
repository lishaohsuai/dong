#pragma once

#include "GeometryKernel.h"
#include<fstream>
class Transform {
public:
	Transform(HexV3fMesh* _HexModel) : HexModel(_HexModel) {};
	void Quad2Triangle();
public:
	HexV3fMesh* HexModel;
	std::string OutputPath;
};