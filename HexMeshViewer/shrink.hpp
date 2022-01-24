#pragma once

#include "triangle.hpp"
#include <string>
class Shrink {
public:
	bool readSurfaceOFF(std::string filename);
	bool readVtk(std::string filename);
};
