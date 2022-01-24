#ifndef __SOLUTIONSKEL_H__
#define __SOLUTIONSKEL_H__
#include <string>
#include "bmesh_solution.hpp"
#include "HexMeshViewer.h"
#include <QObject>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

typedef enum Estate{
	haveNotGenJN = 0,
	haveGenJN,
}STATE;

class SolutionSkel : public QObject {
	Q_OBJECT

public:
	SolutionSkel();
	~SolutionSkel();
public:
	bool readSkel(std::string filename);
	bool genNodeHex();
	void genViewHex(HexV3fMesh &mesh);
	void setMesh(HexV3fMesh *mesh_);
	BMesh::Node *selectWhichNode(V3f &p);
	BMesh::Node *selectWhichNode_(BMesh::Point3D &p);
	BMesh::Node *getRoot();
	vector<BMesh::Node*> getNodeVec();
	void rotateCubeByMouse(V3f, double);
	void saveSkel(std::string name);
	void rotateCube(int direction, int plus);
	void moveCube(int direction, int plus);
	bool connectNodes(BMesh::Node* a, BMesh::Node* b);
	bool disconnectNodes(BMesh::Node* a, BMesh::Node* b);
	void clearSkel();
	void clearJNCube();
	void getALine(std::vector<BMesh::Node*>& ret);
	void spiltX();
	void spiltY();
	void spiltZ();
	void xExpand();
	void yExpand();
	void zExpand();
	void xCrop();
	void yCrop();
	void zCrop();
	glm::quat SolutionSkel::inAngleAxis_(glm::vec3 RotationAxis, double RotationAngle);
	bool changeNodeType();
	bool changeSnap(const BMesh::Point3D&pOld, const BMesh::Point3D &pNew, BMesh::Point3D &out);
	double bigStringToZero(double p, int count);

public slots:
	void reditNode(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height);
	void rscaleNode(double deltaR, int type);
	void rdeltaEditNode(double x, double y, double z, int type);
	void reDrawCubeR();
	void raddNode(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height);
	void rdelNode();
	void pipeline();
	void pipeline2();
	void rsetStepDouble(double);
	void changeAngleAuto();
	void generateLinkBody();
	void topologicalSplit();
	void refineJN();

	void changeToPolylines();
	void simpleWithCgal();
	
	
public:
	BMesh::Node * selectNode;
	std::vector<BMesh::Node *> selectNodes;
	BMesh::Node* mirrorNode;
	BMesh::Point3D positionForSnif;
	BMesh::Point3D snifDelta;
	HexV3fMesh *mesh;
	STATE state;
	bool bdragMode;
	bool bshowTriMeshChanged;
	bool bRotateByMouse;
	bool bVertexSnap;
	bool bmirrorMode;
public:
	BMesh::Solution * solutionOldBMesh;
	double step;
};


#endif