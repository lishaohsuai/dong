#ifndef __MLCA_SUBDIVISION_H__
#define __MLCA_SUBDIVISION_H__

#include "BaseComplex.h"
#include "GeometricTools\wm4bsplinerectangle.h"
#include "BSplineSolid.h"

class MLCASubdivision
{
public:
	MLCASubdivision() {};
	HexV3fMesh& Spliting(HexV3fMesh &mesh);
	HexV3fMesh& Averaging(HexV3fMesh &mesh);
protected:
	std::vector<V3f> new_vv;
	std::vector<V3f> new_ev;
};
#endif