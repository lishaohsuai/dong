//#pragma once
#ifndef CGALMCFSKEL_H
#define CGALMCFSKEL_H
#include "ComHeader.h"
#include "GeometryKernel.h"
#include "skelSimple.h"
class E2I {
public:
	E2I() :begin(0), end(0) {};
	E2I(int _b, int _e) :begin(_b), end(_e) {}
public:
	int begin, end;
};

class CgalMcfskel
{
public:
	CgalMcfskel() {};
	int Run();
	
public:
	std::string NowProcessPath;
	std::string outProcessPath;
	OurSkel SkelResult;
};

#endif