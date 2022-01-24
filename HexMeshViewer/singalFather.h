#ifndef __SINGALFATHER_H__
#define __SINGALFATHER_H__
#include "node.hpp"
#include <QObject>
#include "GeometryKernel.h"
class SingalFather : public QObject {
	Q_OBJECT
public:
	static SingalFather *getInstance();
signals:
	void spipeline2();
	void sshowSkelSingal();
	void sactRMFSignal();
	void sfittingCurvesSignal();
	void schangeToPolylines();
	void ssimpleWithCgalSignal();
	void sreadTestLineSignal();
	void sBoom4();
	void sMouseRelease();
	void sRotateCubeByMouse(V3f, double);
	void sRotateByMouse(int);
	void sVertexSnapMode(int);
	void sspiltX();
	void sspiltY();
	void sspiltZ();
	void sxExpand();
	void syExpand();
	void szExpand();
	void sxCrop();
	void syCrop();
	void szCrop();
	void sdrawModeChanged(int);
	void saxisShowChanged(int);
	void sgroundShowChanged(int);
	void sskelShowChanged(int);
	void smirrorShowChanged(int);
	void smirrorModeChanged(int);
	void sdragModeChanged(int);
	void sshowTriMeshChanged(int);
	void sshowProjectLinesChanged(int);
	void snewNodeOK();
	void sselectALine();
	void ssetStepDouble(double);
	void shelp();
	void sselectMutiNodes();
	void supdateSelectMutiNodes();
	void ssetxyView();
	void ssetyzView();
	void ssetxzView();
	void ssetxy_View();
	void ssetyz_View();
	void ssetxz_View();
	void sconnectNodes();
	void sdisconnectNodes();
	void schangeAngleAutoSignal();
	void sgenerateLinkBodySignal();
	void stopoSplitSignal();
	void srefineSignal();
	void sscaleCell(double deltaR, int type);
	void seditCell(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height);
	void sxRotateCell(int);
	void syRotateCell(int);
	void szRotateCell(int);
	void supMoveCell(int);
	void sdownMoveCell(int);
	void sleftMoveCell(int);
	void srightMoveCell(int);
	void sfrontMoveCell(int);
	void sbackMoveCell(int);
	void sdeltaEditCell(double x, double y, double z, int type); // 0: x 1: y 2:z 3:o
	void supdateGL();
	void reDrawCube();
	void sdrawNormalSkel();
	void saddNode(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height);
	void sdelNode();
	void sdelCell();
	void sselectCube();
	void reNewData();
	void getData(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height);
	void genCoarseMesh();
	void genCoarseMeshFinish();
	void addCubeUseMouse();
	void addCubeUseMouseImpl();


private:
	static SingalFather *instance;
	SingalFather();
};

#endif
