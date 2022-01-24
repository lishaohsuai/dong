#pragma once

#include "ComHeader.h"
#include "GeometryKernel.h"
class BezierSolid
{
public:
	BezierSolid(std::vector<std::vector<std::vector<V3f>>> _SolidData) :SolidData(_SolidData) {};
	V3f P(double u, double v, double w);
	V3f P1(double u, double v, double w);
private:
	std::vector<std::vector<std::vector<V3f>>> SolidData;
	V3f p(double r, std::vector<V3f> data);
	V3f p1(double r, std::vector<V3f> data);
};