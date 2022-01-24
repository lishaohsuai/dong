#pragma once
/**
 * @description: Contains general references
 */
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

/**
 * @description: Contains general data structure related references
 */
#include <algorithm>
#include <cctype>
#include <vector>
#include <array>
#include <set>
#include <queue>
#include <iterator>
#include <map>
#include <stack>
#include <cmath>


// CC 体细分可能造成误差, 生成新的顶点 0.001
#define PRECISION 1.e-8
// old
// #define PRECISION 1.e-7

#if !defined(M_PI)
#define M_PI 3.1415926535897932
#endif
enum
{
	WIRE_FRAME = 0, SOLID_FLAT, FLAT_LINES, ALL_LINES, ALL_POINTS,
	ALL_POLYHEDRALS, SINGULARITY, VOLUME_TEXTURE, N_DRAW_MODES, ORIV,
	ADD_BB, ADD_ORIBB, OV_V, SE_V, SX, BEISU, BEIV, BEI_FRAME,
	DAT1, DAT2, DAT3, DAT4, DAT5, DAT5_1,
	Isoline, DatPoint, DatPoint2, Interior_Point, Edge_Point,
	Corner_Point, Error, Error1, ErrorXJL, View_singular,
	TWOPOINT, ERRORV, TEST, SOLIDFLAT_AND_PICKPOINT, PICKPOINT_RESULT,
	SOLIDFLAT_AND_PICKEDGE, Isoline1, Isoline2, SOLIDFLAT_AND_PICKEDFACE,
	SOLIDFLAT_AND_PICKEDCELL, BEISUDAT, SHOWTWOMESH, SHOWTWOMESH1,
	SINGULARFACE, ADDFRAME, SOLIDFLAT_FRAME, BEISUDAT1,
	ADDFRAME1, SPHERE, ERRORFRAME, TISU, FRAMETISU, QUAD,
	QUADPICKV, QUADV, TWOQUAD, TWOQUADFRAME, CURVE,
	DATONLY, BSVECTOR, VH, VFRAME, ALL_LINES1, SKEL, SELECT_CELL, SELECT_CELLS, BLENDSKEL, Skeleton, TRISKELPRO, TESTLINES
};
// mouse modes
enum
{
	DEFAULT = 0, PICK_POINT, PICK_EDGE, PICK_FACE, DELETE_FACE,
	PICK_CELL, DELETE_CELL, DRAWBEISUR, DRAWBEISUR1, QUADPICK,
	PICK_POINT1, PICKDATONLY, PICK_CELL_SKEL, PICK_CELLS_SKEL, SET_CELL_SKEL, MOVE_CELL_SKEL, ROTATE_CELL, PICK_POINT_SKEL
};

