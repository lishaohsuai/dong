#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>
#include "ui_HexMeshViewer.h"
#include "Subdivision.h"
#include "QGLViewerWidget.h"
#include "Sort.h"
#include "FunctionBSpline.h"
#include "BezierSolid.h"
#include "Transformation.h"
#include <chrono>
#include "ComHeader.h"
#include <QtWidgets/qdialog.h>
#include "solutionSubDiv.h"
#include "solutionSkel.h"

using namespace chrono;
class SolutionSubDiv;
class SolutionSkel;

class HexMeshViewer : public QMainWindow
{
	Q_OBJECT

public:
	HexMeshViewer(QWidget *parent = Q_NULLPTR);

private:
	Ui::HexMeshViewerClass ui;
	bool isneed = false;
	HexV3fMesh* mesh;
	std::tr1::shared_ptr<HexV3fMesh> mesh1;
	std::tr1::shared_ptr<HexV3fMesh> simpmesh;
	std::vector<HexV3fMesh> vmesh;
	void cal_bdy(std::vector<HexV3fMesh> &_dat);
	void cal_cen(std::vector<HexV3fMesh> &_dat);
	std::vector<std::vector<V3f>> cal_norm(std::vector<HexV3fMesh> &_dat);
	V3f DATcenter;
	double DATminEdgeLen;
	double DATmaxEdgeLen;
	double DATr;
	V3f DATbbMin; // boudning box
	V3f DATbbMax;
	std::vector<V3f> DATcellcen;
	std::vector<std::vector<bool>> DATbdy;
	std::vector<std::vector<bool>> DATedgever;
	std::vector<V3f> twonpoint;
	V3f cal_vector_center(std::vector<V3f>);
	double cal_vector_r(std::vector<V3f>, V3f _center);
	std::vector<std::vector<std::vector<std::vector<V3f>>>> SolidData_;
	std::vector<simpsort> sensitivity1;
	std::vector<cube4sort> sort_gap;

private:
	BSplineSolid m_solids;
	std::vector<std::vector<std::vector<std::vector<V3f>>>> SolidData;
	HexV3fMesh constructBezierSolid(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color);
	HexV3fMesh constructBezierSolid(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color, int n);
	HexV3fMesh constructBezierSolid1(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color);
	HexV3fMesh constructBezierSolid1(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color, int n);
	HexV3fMesh constructBezierSolid2(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color,
									 std::vector<std::vector<CellHandle>> &tempcell, std::vector<V3f> &tempcen);
	HexV3fMesh constructBezierSolid3(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color,
									 std::vector<std::vector<CellHandle>> &tempcell, std::vector<V3f> &tempcen);
	HexV3fMesh constructBezierSolid4(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color,
									 std::vector<std::vector<CellHandle>> &tempcell, std::vector<V3f> &tempcen);
	void simsor();
	void simsor1();
	int simp_time = 1;
	std::vector<double> material_distribution;
	std::vector<double> material_distribution1;
	std::vector<double> material_distribution_check;
	std::vector<V3f> glo_cen;
	std::vector<int> glo_cen_id;
	std::vector<std::vector<VertexHandle>> in_out_side;
	std::vector<std::vector<V3f>> in_out_sidev3f;
	std::vector<VertexHandle> pickvertex;
	std::vector<EdgeHandle> pickedge;
	std::vector<EdgeHandle> pickedge1;
	std::vector<FaceHandle> pickface;
	std::vector<CellHandle> pickcell;
	std::vector<VertexHandle> csv;
	std::vector<V3f> csv3f;

public:
	std::vector<bool> checkcell;
	std::vector<int> pickdrawface;

public:
	std::vector<double> material_distribution_mutires;
	std::vector<V3f> vn;
	std::vector<double> linecheck;

public:
	QuadV3fMesh *quad;
	std::vector<std::vector<V3f>> curve_v;

public:
	std::vector<std::vector<std::vector<V3f>>> bei_surv;
	std::vector<std::vector<V3f>> nor_bei_surv;
	bool checkbei = false;
	std::vector<V3f> edgemid;

public:
	std::vector<beziersurface> bf;
	std::vector<mvh> vH;
	std::vector<frame1> vframe;
	double scales = 1;
	double atans = 0;
	double xmove = 0, ymove = 0, zmove = 0;
	void updatedrawx();
	void updatedrawz();
	void updatedraw2();
	void updatedraw3();
	void renewParam();

public:
	HexV3fMesh *mesh_1;
	std::vector<double> m_color1;
	HexV3fMesh *mesh_2;
	std::vector<double> m_color2;
	int times = 0;
	HexV3fMesh mesh1_;
	HexV3fMesh mesh2_;

public:
	int drawModeArg;
	// lee
	SolutionSubDiv *solutionSubDiv;
	SolutionSkel *solutionSkel;
	OurSkel *outskel;
	QMenu *menuLee;
	QAction *actMLCA;
	QAction *actMLCA_AVG;
	QAction *actOpenSkel;
	QAction *actExportSkel;
	QAction *actBlend;
	QAction *actWhirl;
	QAction *actHelp;
	QAction *actClearSkel;
	QAction *actPIA;
	QAction *actFaceNormChange;
	QAction *actReadOpenComplexSkel;
	QAction *actsplitComplexSkel;
	QAction *actSimpleComplexSkel;
	
	QAction *actFromVolumeToOff;
	QAction *actChangeNodeType;
	QAction *actScaledJacobian;
	QAction *actProject;
	
	QAction *actExportBroSkel; // 师弟的文件输出
	QAction *actFixFlip;
	QAction *actShrinking;
	QAction* actShowProjectLine;
	QAction* actOpenOff;// outer triangle mesh 
	QAction* actPickProjPoints;
	QAction* actSkelMove;
	QTimer *timer;

	Project* projectSkel;

public slots:
	void subdivision();
	void average();
	void openMeshFile();
	void readXJ();
	void combine();
	void readSkel();
	void selectCell();
	void selectMutiCells();
	void rconnectNodes();
	void editCell();
	void delCell();
	void drawNormalSkel();
	void reNewDataSignal();
	void genCoarseMeshFinishMesh();
	void addCellUseMouseSignal();
	void addCellUseMouseImplSignal();
	void exportSkel();
	void blendSkel();
	void rotate();
	void rupdateGL();
	void helpString();
	void rBoom4();
	void rdisconnectNodes();
	void clearSkel();
	void rxRotateCell(int);
	void ryRotateCell(int);
	void rzRotateCell(int);
	void rupMoveCell(int);
	void rdownMoveCell(int);
	void rfrontMoveCell(int);
	void rbackMoveCell(int);
	void rleftMoveCell(int);
	void rrightMoveCell(int);
	void PIA();
	void rselectALine();
	void rupdateSelectMutiNodes();
	void rsetxyView();
	void rsetyzView();
	void rsetxzView();
	void rsetxy_View();
	void rsetyz_View();
	void rsetxz_View();
	void rmirrorModeChanged(int);
	void rshowTriMeshChanged(int);
	void rshowProjectLinesChanged(int);
	void raxisShowChanged(int);
	void rgroundShowChanged(int);
	void rskelShowChanged(int);
	void rmirrorShowChanged(int);
	void rdragModeChanged(int);
	void rnewNodeOK();
	void rdrawModeChanged(int);
	void faceNormChange();
	void readOpenComplexSkel();
	void simpleComplexSkel();
	void simpleWithCgal();
	void rspiltX();
	void rspiltY();
	void rspiltZ();
	void rxExpand();
	void ryExpand();
	void rzExpand();
	void rxCrop();
	void ryCrop();
	void rzCrop();
	void rRotateByMouseChanged(int);
	void rRotateCubeByMouse(V3f, double);
	void fromVolumeToOff();
	void changeNodeType();
	void scaledJacobian();
	void rVertexSnapModeChanged(int arg);
	void rMouseRelease();
	void project();
	void exportBroSkel();
	void fixFlip();
	void openOff();
	void shrinking();
	void showProjectLine();
	void pickProjPoints();
	void skelMove();
	void splitComplexSkel();

	void rreadTestLineSignal();
	void rchangeToPolylines();
	void rfittingCurvesSignal();
	void ractRMFSignal();
	void rshowSkel();
	void rBoom5();

signals:
	void leeUpdate();

public:
	HexV3fMesh* newmesh_local;
	HexV3fMesh& spiltChineseMoney(HexV3fMesh& mesh);
	void createActions(void);
	void createMenus(void);
	void drawMode();
	void exportJsonForCurveP(vector<vector<BMesh::Point3D>> &skel);
};
