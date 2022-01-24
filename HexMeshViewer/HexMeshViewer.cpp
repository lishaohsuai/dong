#include "HexMeshViewer.h"
#include <qfiledialog.h>
#include <FileHandler.h>
#include "singalFather.h"
#include "simpleWithCgal.h"
#include "PIA.h"
#include <math.h>
#include <Eigen/Dense>
#include <algorithm>
#include "Transform.h"
#include "fixFlip.hpp"
#include "project.h"
#include "triangleMesh.hpp"
#include "interpolationCurve.h"

HexMeshViewer::HexMeshViewer(QWidget *parent)
	: QMainWindow(parent), mesh(new HexV3fMesh())
{
	ui.setupUi(this);
	this->resize(1800, 1080); // set App size
	createActions();
	createMenus();
	solutionSubDiv = new SolutionSubDiv();
	solutionSkel = new SolutionSkel(); 
	projectSkel = new Project();

	
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssimpleWithCgalSignal()), this, SLOT(simpleWithCgal()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sactRMFSignal()), this, SLOT(ractRMFSignal()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sreadTestLineSignal()), this, SLOT(rreadTestLineSignal()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sfittingCurvesSignal()), this, SLOT(rfittingCurvesSignal()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(schangeToPolylines()), this, SLOT(rchangeToPolylines()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sshowSkelSingal()), this, SLOT(rshowSkel()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(spipeline2()), this, SLOT(rBoom5()));

	QObject::connect(SingalFather::getInstance(), SIGNAL(supMoveCell(int)), this, SLOT(rupMoveCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdownMoveCell(int)), this, SLOT(rdownMoveCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sleftMoveCell(int)), this, SLOT(rleftMoveCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(srightMoveCell(int)), this, SLOT(rrightMoveCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sfrontMoveCell(int)), this, SLOT(rfrontMoveCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sbackMoveCell(int)), this, SLOT(rbackMoveCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetStepDouble(double)), solutionSkel, SLOT(rsetStepDouble(double)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetxyView()), this, SLOT(rsetxyView()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetyzView()), this, SLOT(rsetyzView()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetxzView()), this, SLOT(rsetxzView()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetxy_View()), this, SLOT(rsetxy_View()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetyz_View()), this, SLOT(rsetyz_View()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetxz_View()), this, SLOT(rsetxz_View()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdrawModeChanged(int)), this, SLOT(rdrawModeChanged(int)));
	
	QObject::connect(SingalFather::getInstance(), SIGNAL(sBoom4()), this, SLOT(rBoom4()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(saxisShowChanged(int)), this, SLOT(raxisShowChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sgroundShowChanged(int)), this, SLOT(rgroundShowChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sskelShowChanged(int)), this, SLOT(rskelShowChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(smirrorShowChanged(int)), this, SLOT(rmirrorShowChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(smirrorModeChanged(int)), this, SLOT(rmirrorModeChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdragModeChanged(int)), this, SLOT(rdragModeChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sshowTriMeshChanged(int)), this, SLOT(rshowTriMeshChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sshowProjectLinesChanged(int)), this, SLOT(rshowProjectLinesChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(snewNodeOK()), this, SLOT(rnewNodeOK()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sRotateByMouse(int)), this, SLOT(rRotateByMouseChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sVertexSnapMode(int)), this, SLOT(rVertexSnapModeChanged(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sRotateCubeByMouse(V3f, double)), this, SLOT(rRotateCubeByMouse(V3f, double)));

	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetyzView()), this, SLOT(rsetyzView()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(ssetxzView()), this, SLOT(rsetxzView()));

	QObject::connect(SingalFather::getInstance(), SIGNAL(sspiltX()), this, SLOT(rspiltX()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sspiltY()), this, SLOT(rspiltY()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sspiltZ()), this, SLOT(rspiltZ()));

	QObject::connect(SingalFather::getInstance(), SIGNAL(sxExpand()), this, SLOT(rxExpand()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(syExpand()), this, SLOT(ryExpand()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(szExpand()), this, SLOT(rzExpand()));

	QObject::connect(SingalFather::getInstance(), SIGNAL(sxCrop()), this, SLOT(rxCrop()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(syCrop()), this, SLOT(ryCrop()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(szCrop()), this, SLOT(rzCrop()));
	
	QObject::connect(SingalFather::getInstance(), SIGNAL(schangeAngleAutoSignal()), solutionSkel, SLOT(changeAngleAuto()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sgenerateLinkBodySignal()), solutionSkel, SLOT(generateLinkBody()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(stopoSplitSignal()), solutionSkel, SLOT(topologicalSplit()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(srefineSignal()), solutionSkel, SLOT(refineJN()));

	QObject::connect(SingalFather::getInstance(), SIGNAL(sxRotateCell(int)), this, SLOT(rxRotateCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(syRotateCell(int)), this, SLOT(ryRotateCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(szRotateCell(int)), this, SLOT(rzRotateCell(int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sconnectNodes()), this, SLOT(rconnectNodes()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdisconnectNodes()), this, SLOT(rdisconnectNodes()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(shelp()), this, SLOT(helpString()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sscaleCell(double, int)), solutionSkel, SLOT(rscaleNode(double, int))); // 生成的这个有啥用呀奇怪
	QObject::connect(SingalFather::getInstance(), SIGNAL(seditCell(double, double, double, double, double, double, double, double, double)), solutionSkel, SLOT(reditNode(double, double, double, double, double, double, double, double, double))); // 生成的这个有啥用呀奇怪
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdeltaEditCell(double, double, double, int)), solutionSkel, SLOT(rdeltaEditNode(double, double, double, int)));
	QObject::connect(SingalFather::getInstance(), SIGNAL(reDrawCube()), solutionSkel, SLOT(reDrawCubeR()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdrawNormalSkel()), this, SLOT(drawNormalSkel()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(saddNode(double, double, double, double, double, double, double, double, double)), 
					 solutionSkel, SLOT(raddNode(double, double, double, double, double, double, double, double, double))); // add Node signal
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdelCell()), this, SLOT(delCell()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sdelNode()), solutionSkel, SLOT(rdelNode()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(sselectCube()), this, SLOT(selectCell())); // select cube
	QObject::connect(SingalFather::getInstance(), SIGNAL(sselectMutiNodes()), this, SLOT(selectMutiCells()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(reNewData()), this, SLOT(reNewDataSignal()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(addCubeUseMouse()), this, SLOT(addCellUseMouseSignal()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(addCubeUseMouseImpl()), this, SLOT(addCellUseMouseImplSignal()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(genCoarseMesh()), solutionSkel, SLOT(pipeline()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(genCoarseMeshFinish()), this, SLOT(genCoarseMeshFinishMesh()));
	QObject::connect(SingalFather::getInstance(), SIGNAL(supdateGL()), this, SLOT(rupdateGL())); // update one cell

	QObject::connect(SingalFather::getInstance(), SIGNAL(sselectALine()), this, SLOT(rselectALine())); // update one cell
	
	QObject::connect(SingalFather::getInstance(), SIGNAL(supdateSelectMutiNodes()), this, SLOT(rupdateSelectMutiNodes())); // update one cell
	QObject::connect(SingalFather::getInstance(), SIGNAL(sMouseRelease()), this, SLOT(rMouseRelease())); // update one cell



	ui.actionOpen->setIcon(QIcon("Images/Open.png"));
	ui.actionOpen->setStatusTip(("Open a mesh file"));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openMeshFile()));

	ui.actionReadXJ->setIcon(QIcon("Images/Open.png"));
	ui.actionReadXJ->setStatusTip(("Open a mesh file"));
	connect(ui.actionReadXJ, SIGNAL(triggered()), this, SLOT(readXJ()));


	ui.actionMesh->setStatusTip(("Select Mesh"));
	connect(ui.actionMesh, &QAction::triggered,
		[=]() {
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->setmesh(mesh);
			ui.widget->setdraw_mode_(FLAT_LINES);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
			ui.widget->setoriv(mesh->oriv);
		});

	ui.actionMesh1->setStatusTip(("Select Mesh1"));
	connect(ui.actionMesh1, &QAction::triggered,
		[=]() {
			std::vector<int> temp_num(mesh->cells_.size(), 0);
			for (int i = 0; i < mesh->cells_.size(); i++)
			{
				temp_num[i] = (mesh->neighbor_c_c(CellHandle(i))).size();
			}
			ui.widget->setbffv(mesh1->boundary_face_v, mesh1->normal_boundary_face_v, mesh1->boundary_face, mesh1->boundary_face_VID);
			ui.widget->setmesh(mesh1);
			ui.widget->setdraw_mode_(FLAT_LINES);
			ui.widget->set_scene_pos(mesh1->center, mesh1->r);
			ui.widget->seted(mesh1->maxEdgeLen, mesh1->minEdgeLen);
			ui.widget->setbb(mesh1->bbMax, mesh1->bbMin);
			ui.widget->setoribb(mesh1->bbMax, mesh1->bbMin);
			ui.widget->setoriv(mesh1->oriv);
		});

	ui.actionSort_Two_Mesh->setStatusTip(("Sort Two Mesh"));
	connect(ui.actionSort_Two_Mesh, &QAction::triggered,
		[=]() {
			Sort stm(mesh, mesh1.get());
			stm.sorting();
			stm.comparing();
			ui.widget->setdrawv_big_small(stm.drawvbig, stm.drawvsmall, stm.drawvbigid);
			ui.widget->seterrmax_min(stm.max, stm.min);
			qDebug() << "max: " << stm.max << " min: " << stm.min;
			ui.widget->set_color(stm.color, stm.colorverr);
		});

	ui.actionCombine->setStatusTip(("Combine mltiple operates"));
	connect(ui.actionCombine, SIGNAL(triggered()), this, SLOT(combine()));

	ui.actionAdd_HexMesh->setIcon(QIcon("Images/Open.png"));
	ui.actionAdd_HexMesh->setStatusTip(("Add HexMesh to a HexMesh"));
	connect(ui.actionAdd_HexMesh, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "VTK(*.mesh)");
			HexV3fMesh *tempmesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.read2HexMeshObj(tempmesh, true, true, true);
			qDebug() << tempmesh->cells_.size();
			tempmesh->cal_cen();
			tempmesh->cal_cell_cen();
			tempmesh->examine_bdy();
			tempmesh->examine_cs();
			tempmesh->cal_cen1();
			tempmesh->examine_bdfv();
			tempmesh->cal_cell_cen();
			tempmesh->cal_bdnorm();
			tempmesh->cal_mami_ed();
			tempmesh->cal_oriv();
			vH.push_back({ tempmesh->boundary_face_v, tempmesh->normal_boundary_face_v });
			ui.widget->setvH(vH);
			ui.widget->setdraw_mode_(VH);
		});

	ui.actionOpenVTK_Tri->setIcon(QIcon("Images/Open.png"));
	ui.actionOpenVTK_Tri->setStatusTip(("Open a VTK file"));
	connect(ui.actionOpenVTK_Tri, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Vtk(*.vtk)");
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readVtk2HexMeshObj1(mesh);
			mesh->examine_bdy();
			mesh->examine_cs();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			mesh->cal_oriv();
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->setmesh(mesh);
			ui.widget->setdraw_mode_(FLAT_LINES);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
			ui.widget->setoriv(mesh->oriv);
			update();
		});

	ui.actionOpenOBJ->setIcon(QIcon("Images/Open.png"));
	ui.actionOpenOBJ->setStatusTip(("Open a OBJ file"));
	connect(ui.actionOpenOBJ, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Obj(*.obj)");
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			std::vector<V3f>().swap(vn);
			file.readObj2HexMesh(mesh, vn);
			mesh->examine_bdy(1);
			mesh->examine_cs();
			mesh->cal_cen();
			mesh->examine_bdfv(1);
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			mesh->cal_oriv();
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->setmesh(mesh);
			ui.widget->setdraw_mode_(FLAT_LINES);
			if (isneed == false){
				ui.widget->set_scene_pos(mesh->center, mesh->r);
				ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
				ui.widget->setbb(mesh->bbMax, mesh->bbMin);
				ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
				isneed = true;
			}
			ui.widget->setoriv(mesh->oriv);
			update();
		});

	ui.actionOpenVtk_smooth->setIcon(QIcon("Images/Open.png"));
	ui.actionOpenVtk_smooth->setStatusTip(("Open a OBJ file"));
	connect(ui.actionOpenVtk_smooth, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "VTK(*.vtk)");
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			std::vector<V3f>().swap(vn);
			//file.readVtk2HexMeshObj_smooth(mesh, vn);
			file.readVtk2HexMeshObj_smooth1(mesh, vn);
			mesh->examine_bdy(1);
			mesh->examine_cs();
			mesh->cal_cen();
			mesh->examine_bdfv(1);
			mesh->cal_cell_cen();
			mesh->cal_bdnorm(vn);
			mesh->cal_mami_ed();
			mesh->cal_oriv();
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->setmesh(mesh);
			ui.widget->setdraw_mode_(FLAT_LINES);
			if (isneed == false)
			{
				ui.widget->set_scene_pos(mesh->center, mesh->r);
				ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
				ui.widget->setbb(mesh->bbMax, mesh->bbMin);
				ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
				isneed = true;
			}
			ui.widget->setoriv(mesh->oriv);
			ui.widget->pickdrawface.swap(std::vector<int>());
			update();
		});

	ui.actionOpenBezier->setIcon(QIcon("Images/Open.png"));
	ui.actionOpenBezier->setStatusTip(("Open a Bezier file"));
	connect(ui.actionOpenBezier, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "TXT(*.txt)");
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			std::vector<std::vector<double>> m_bdycoef;
			file.readbezier(m_solids, m_bdycoef);
			std::vector<double> m_color;
			std::cout << "Start constructBezierSolid" << std::endl;
			std::vector<std::vector<CellHandle>> tempcell;
			std::vector<V3f> tempcen;
			*mesh = constructBezierSolid2(m_bdycoef, m_color, tempcell, tempcen);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			//ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionOpenBezier1->setIcon(QIcon("Images/Open.png"));
	ui.actionOpenBezier1->setStatusTip(("Open a Bezier file"));
	connect(ui.actionOpenBezier1, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "TXT(*.txt)");
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			std::vector<std::vector<double>> m_bdycoef;
			file.readbezier(m_solids, m_bdycoef);
			std::vector<double> m_color;
			std::cout << "Start constructBezierSolid" << std::endl;
			std::vector<std::vector<CellHandle>> tempcell;
			std::vector<V3f> tempcen;
			*mesh = constructBezierSolid3(m_bdycoef, m_color, tempcell, tempcen);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			//ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionOpenBezier2->setIcon(QIcon("Images/Open.png"));
	ui.actionOpenBezier2->setStatusTip(("Open a Bezier file"));
	connect(ui.actionOpenBezier2, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "TXT(*.txt)");
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			std::vector<std::vector<double>> m_bdycoef;
			file.readbezier(m_solids, m_bdycoef);
			std::vector<double> m_color;
			std::cout << "Start constructBezierSolid" << std::endl;
			std::vector<std::vector<CellHandle>> tempcell;
			std::vector<V3f> tempcen;
			*mesh = constructBezierSolid4(m_bdycoef, m_color, tempcell, tempcen);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			//ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionOpenBezierNew->setIcon(QIcon("Images/Open.png"));
	ui.actionOpenBezierNew->setStatusTip(("Open a Bezier file"));
	connect(ui.actionOpenBezierNew, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "MESH(*.mesh)");
			//mesh = new HexV3fMesh();
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<double> m_color;
			file.readbeziernew(m_color, mesh);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionSave->setIcon(QIcon("Images/Save.png"));
	ui.actionSave->setStatusTip(("Save the mesh to file"));
	connect(ui.actionSave, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getSaveFileName(this, 
				tr("Save mesh file"), 
				tr(".."), 
				tr("Mesh (*.mesh);;"
				"Vtk (*.vtk);;"
				"All File (*)"));
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				filenew.writeFile(name, mesh);
			}
		});


	ui.actionSaveDAT->setIcon(QIcon("Images/Save.png"));
	ui.actionSaveDAT->setStatusTip(("Save the mesh to DAT file"));
	connect(ui.actionSaveDAT, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "Dat(*.dat)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				filenew.write2DatFile(name, vmesh);
			}
		});

	ui.actionSaveAXL->setIcon(QIcon("Images/Save.png"));
	ui.actionSaveAXL->setStatusTip(("Save the mesh to AXL file"));
	connect(ui.actionSaveAXL, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "Axl(*.axl)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				ui.widget->set_axl_write(SolidData_, name);
				ui.widget->write2AXLFile();
			}
		});

	ui.actionWIRE_FRAME->setIcon(QIcon("Images/wire.png"));
	ui.actionWIRE_FRAME->setStatusTip(("Using wireFrame showing method"));
	connect(ui.actionWIRE_FRAME, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(WIRE_FRAME);
			update();
		});

	ui.actionFLAT_LINES->setIcon(QIcon("Images/flatlines.png"));
	ui.actionFLAT_LINES->setStatusTip(("Using flatLines showing method"));
	connect(ui.actionFLAT_LINES, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(FLAT_LINES);
			update();
		});

	ui.actionSOLID_FLAT->setIcon(QIcon("Images/flat.png"));
	ui.actionSOLID_FLAT->setStatusTip(("Using Solid Falt showing method"));
	connect(ui.actionSOLID_FLAT, &QAction::triggered,
		[=]() {
			//ui.widget->mesh->cal_bdnorm();
			ui.widget->setdraw_mode_(SOLID_FLAT);
			update();
		});

	ui.actionSOLIDFLAT_FRAME->setIcon(QIcon("Images/flat.png"));
	ui.actionSOLIDFLAT_FRAME->setStatusTip(("Using Solid Falt showing method"));
	connect(ui.actionSOLIDFLAT_FRAME, &QAction::triggered,
		[=]() {
			HexV3fMesh *meshtest = new HexV3fMesh();
			*meshtest = *mesh;
			meshtest->examine_bdy();
			meshtest->choseframe();
			meshtest->choseframeface();
			ui.widget->setframe(meshtest->frame, meshtest->frameface, meshtest->framefacenrom);
			ui.widget->setdraw_mode_(SOLIDFLAT_FRAME);
			update();
		});

	ui.actionALL_LINES->setIcon(QIcon("Images/all_lines.png"));
	ui.actionALL_LINES->setStatusTip(("Show All Lines"));
	connect(ui.actionALL_LINES, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ALL_LINES);
			update();
		});

	ui.actionALL_LINES1->setIcon(QIcon("Images/all_lines.png"));
	ui.actionALL_LINES1->setStatusTip(("Show All Lines"));
	connect(ui.actionALL_LINES1, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ALL_LINES1);
			update();
		});

	ui.actionALL_POLY->setIcon(QIcon("Images/all_poly.png"));
	ui.actionALL_POLY->setStatusTip(("Using AllLines showing method"));
	connect(ui.actionALL_POLY, &QAction::triggered,
		[=]() {
			//mesh->cal_cell_cen();
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->setdraw_mode_(ALL_POLYHEDRALS);
			update();
		});

	ui.actionALL_POINTS->setIcon(QIcon("Images/points.png"));
	ui.actionALL_POINTS->setStatusTip(("Show All Points"));
	connect(ui.actionALL_POINTS, &QAction::triggered,
		[=]() {
			//mesh->cal_cell_cen();
			//ui.widget->setcell_center(mesh->cell_center);
			ui.widget->setdraw_mode_(ALL_POINTS);
			update();
		});

	ui.actionVOLUME_TEXTURE->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionVOLUME_TEXTURE->setStatusTip(("Show Volume Texture"));
	connect(ui.actionVOLUME_TEXTURE, &QAction::triggered,
		[=]() {
			//mesh->cal_cell_cen();
			//ui.widget->setcell_center(mesh->cell_center);
			ui.widget->setdraw_mode_(VOLUME_TEXTURE);
			update();
		});

	ui.actionDAT1->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionDAT1->setStatusTip(("Show DAT1"));
	connect(ui.actionDAT1, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(DAT1);
			update();
		});

	ui.actionDAT2->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionDAT2->setStatusTip(("Show DAT2"));
	connect(ui.actionDAT2, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(DAT2);
			update();
		});

	ui.actionDAT3->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionDAT3->setStatusTip(("Show DAT3"));
	connect(ui.actionDAT3, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(DAT3);
			update();
		});

	ui.actionDAT4_2->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionDAT4_2->setStatusTip(("Show DAT4"));
	connect(ui.actionDAT4_2, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(DAT4);
			update();
		});

	ui.actionDAT5->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionDAT5->setStatusTip(("Show DAT4"));
	connect(ui.actionDAT5, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(DAT5);
			update();
		});

	ui.actionDAT5_1->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionDAT5_1->setStatusTip(("Show DAT5_1"));
	connect(ui.actionDAT5_1, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(DAT5_1);
			update();
		});

	ui.actionIsoline->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionIsoline->setStatusTip(("Show Isoline"));
	connect(ui.actionIsoline, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(Isoline);
			update();
		});

	ui.actionIsoline1->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionIsoline1->setStatusTip(("Show Isoline1"));
	connect(ui.actionIsoline1, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(Isoline1);
			update();
		});

	ui.actionIsoline2->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionIsoline2->setStatusTip(("Show Isoline2"));
	connect(ui.actionIsoline2, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(Isoline2);
			update();
		});

	ui.actionDATonly->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionDATonly->setStatusTip(("Show DATonly"));
	connect(ui.actionDATonly, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(DATONLY);
			update();
		});

	ui.actionError->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionError->setStatusTip(("Show Error"));
	connect(ui.actionError, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionError1->setIcon(QIcon("Images/texture_3D.png"));
	ui.actionError1->setStatusTip(("Show Error"));
	connect(ui.actionError1, &QAction::triggered,
		[=]() {
			//ui.widget->addbffv(mesh1->boundary_face_v, mesh1->normal_boundary_face_v, mesh1->boundary_face);
			ui.widget->setbffv1(mesh1->boundary_face_v, mesh1->normal_boundary_face_v, mesh1->boundary_face, mesh1->boundary_face_VID);
			ui.widget->setmesh1(mesh1);
			ui.widget->seted1(mesh1->maxEdgeLen, mesh1->minEdgeLen);
			ui.widget->setbb1(mesh1->bbMax, mesh1->bbMin);
			ui.widget->setdraw_mode_(Error1);
			update();
		});

	ui.actionSubdvision->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSubdvision->setStatusTip(("Subdivide The Mesh"));
	connect(ui.actionSubdvision, &QAction::triggered,
		[=]() {
			std::vector<V3f> v3f_singular;
			HexV3fMesh *newmesh = new HexV3fMesh();
			Split split(mesh, newmesh, material_distribution);
			*mesh = split.Spliting();
			material_distribution = split.material_distribution;
			mesh->examine_bdy();
			mesh->examine_singular();
			Average average(mesh);
			*mesh = average.Averaging();

			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionSplit->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSplit->setStatusTip(("Split The Mesh"));
	connect(ui.actionSplit, &QAction::triggered,
		[=]() {
			std::vector<V3f> v3f_singular;
			HexV3fMesh *newmesh = new HexV3fMesh();
			Split split(mesh, newmesh, material_distribution);
			*mesh = split.Spliting();
			material_distribution = split.material_distribution;
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			//ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			//ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			//ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionSplit1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSplit1->setStatusTip(("Split1 The Mesh"));
	connect(ui.actionSplit1, &QAction::triggered,
		[=]() {
		});

	ui.actionSubdvision2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSubdvision2->setStatusTip(("Subdivide The Mesh With Method Interpolatory"));
	connect(ui.actionSubdvision2, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			mesh->examine_bdy();
			Subdivision sub(mesh, newmesh);
			*mesh = sub.Subdivising();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionSubdvision3->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSubdvision3->setStatusTip(("Subdivide The Mesh With Method CC"));
	connect(ui.actionSubdvision3, &QAction::triggered,
		[=]() {
			auto start = system_clock::now();

			HexV3fMesh *newmesh = new HexV3fMesh();
			mesh->examine_bdy();
			CCSubdivision sub(mesh, newmesh, material_distribution);
			*mesh = sub.CCSubdivising();
			//*mesh = sub.CCSubdivising();
			//*mesh = sub.CCSubdivising();
			//*mesh = sub.CCSubdivising();
			auto end = system_clock::now();
			auto duration = duration_cast<microseconds>(end - start);
			std::cout << "������"
				<< double(duration.count()) * microseconds::period::num / microseconds::period::den
				<< "��" << std::endl;
			material_distribution = sub.material_distribution;
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			//ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			//ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			//ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionCCSub_New->setStatusTip(("Subdivide The Mesh With Method CC"));
	connect(ui.actionCCSub_New, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			mesh->examine_bdy();
			CCSubdivision sub(mesh, newmesh);
			*mesh = sub.CCSubdivising_new();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionAdd_CC->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAdd_CC->setStatusTip(("Subdivide The Mesh1 With Method CC"));
	connect(ui.actionAdd_CC, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			mesh1->examine_bdy();
			CCSubdivision sub(mesh1.get(), newmesh, material_distribution1);
			*mesh1 = sub.CCSubdivising();
			mesh1->examine_bdy();
			mesh1->cal_cen();
			mesh1->examine_bdfv();
			mesh1->cal_cell_cen();
			mesh1->cal_bdnorm();
			mesh1->cal_mami_ed();
			update();
		});

	ui.actionSubdvision4->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSubdvision4->setStatusTip(("Subdivide The Mesh With Method CC And Crease"));
	connect(ui.actionSubdvision4, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			//for (int i = 0; i < mesh->v_cs.size(); ++i)
			//{
			//	if(mesh->v_cs[i]==true)
			//}

			//CCSubdivision sub(mesh, newmesh, mesh->v_cs);
			CCSubdivision sub(mesh, newmesh, ui.widget->pickedge_e);
			//*mesh = sub.CCSubWithCrease();
			*mesh = sub.CCSubWithCrease_sharpedge();
			//sub.check_cse();
			ui.widget->pickedge_e = sub.sharpedge;

			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);

			/*std::vector<EdgeHandle> newpickedge;
		for (int i = 0; i < mesh->edges_.size(); ++i)
		{
			if (mesh->v_cs[mesh->edges_[i].from_h] == true && mesh->v_cs[mesh->edges_[i].to_h] == true)
				newpickedge.push_back(EdgeHandle(i));
		}
		pickedge = newpickedge;
		ui.widget->pickedge_e = pickedge;*/
			std::cout << pickedge.size() << std::endl;
			update();
		});

	ui.actionSubdvision5->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSubdvision5->setStatusTip(("Subdivide The Mesh With Method Push-Back Operation Based CC "));
	connect(ui.actionSubdvision5, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();

			InterpolateCC ICsub(mesh, newmesh);
			ICsub.PBfirstep();
			*mesh = ICsub.CCInterpolatingPB();

			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionSubdvision6->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSubdvision6->setStatusTip(("Subdivide The Mesh With Method Normal-Based Methbod Based CC "));
	connect(ui.actionSubdvision6, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();

			InterpolateCC ICsub(mesh, newmesh);
			ICsub.NBfirstep();
			*mesh = ICsub.CCInterpolatingNB();

			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionSubdvision7->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSubdvision7->setStatusTip(("Subdivide The Mesh With Method Normal-Based Methbod Based CC "));
	connect(ui.actionSubdvision7, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			HexV3fMesh *tempmesh = new HexV3fMesh();
			InterpolateCC MLCAsub(mesh, newmesh, tempmesh);
			MLCAsub.PBfirstep();
			MLCAsub.MLCAfirstep();
			*mesh = MLCAsub.CCInterpolatingMLCA();

			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionApproximationCC->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionApproximationCC->setStatusTip(("Create Bezier to approximate CC"));
	connect(ui.actionApproximationCC, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			AppCC.approximationing();
			AppCC.cal_nor_sur();
			ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
			ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
			ui.widget->setdraw_mode_(BEISU);
			update();
		});

	ui.actionConstructBezier->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructBezier->setStatusTip(("Create Bezier"));
	connect(ui.actionConstructBezier, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			AppCC.approximationing();
			AppCC.cal_nor_sur();
			AppCC.constructBezier();
			AppCC.cal_nor_beisur();
			ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
			ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
			ui.widget->setdraw_mode_(BEISU);
			update();
		});

	ui.actionBeibdyDAT->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionBeibdyDAT->setStatusTip(("Create Bezier"));
	connect(ui.actionBeibdyDAT, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, vn);
			AppCC.approximationing(1);
			AppCC.constructBezier(1);
			//AppCC.cal_nor_beisur(1);//���ڸ����㷨����
			//AppCC.cal_nor_beisur1(1);//����ͨ�����ĵ���㷨��
			AppCC.cal_nor_beisur(0.1); //��������ͬ,ͨ����������ķ�����㷨��
			//AppCC.cal_nor_beisur1();
			AppCC.cal_beisurv_center(1);
			mesh->checksingualrface();
			ui.widget->setbiaojiface(AppCC.biaojiface);
			ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
			ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
			ui.widget->setedgeline(AppCC.edgeline);
			ui.widget->setsingularface(mesh->singularface);
			//ui.widget->setdraw_mode_(SINGULARFACE);
			ui.widget->setdraw_mode_(SHOWTWOMESH1);
			update();
		});

	ui.actionBeibdyDAT1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionBeibdyDAT1->setStatusTip(("Create Bezier1"));
	connect(ui.actionBeibdyDAT1, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			AppCC.approximationing();
			AppCC.constructBezier(1);
			AppCC.cal_nor_beisur(0.1);
			AppCC.cal_beisurv_center(1);
			mesh->checksingualrface();
			ui.widget->setbiaojiface(AppCC.biaojiface);
			ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
			ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
			ui.widget->setedgeline(AppCC.edgeline);
			ui.widget->setsingularface(mesh->singularface);
			ui.widget->setdraw_mode_(BEISUDAT1);
			update();
		});

	ui.actionAppCCSolid->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAppCCSolid->setStatusTip(("Create Control Points for Solid"));
	connect(ui.actionAppCCSolid, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			*mesh = AppCC.CCapproximation();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionConstructBS->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructBS->setStatusTip(("Create Bezier Solid"));
	connect(ui.actionConstructBS, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
			AppCC.CCapproximation1();
			*mesh = AppCC.constructBezierSolid();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionAppCCSolidNoS->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAppCCSolidNoS->setStatusTip(("Create Control Points for Solid with no Singluar"));
	connect(ui.actionAppCCSolidNoS, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			*mesh = AppCC.CCapproximation2();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionConstructBSNoS->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructBSNoS->setStatusTip(("Create Bezier Solid with no Singluar"));
	connect(ui.actionConstructBSNoS, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			AppCC.CCapproximation3();
			*mesh = AppCC.constructBezierSolid();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionCompare->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionCompare->setStatusTip(("Compare two method"));
	connect(ui.actionCompare, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			AppCC.cal_limitccver();
			mesh->oriv = AppCC.ccver;
			ui.widget->setoriv(mesh->oriv);
		});

	ui.actionAppCCSolidDAT->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAppCCSolidDAT->setStatusTip(("Create Control Points for Solid DAT"));
	connect(ui.actionAppCCSolidDAT, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			vmesh = AppCC.CCapproximationDAT();
			cal_bdy(vmesh);
			cal_cen(vmesh);
			std::vector<std::vector<V3f>> datnorm = cal_norm(vmesh);
			ui.widget->setvmesh(vmesh, datnorm);
			ui.widget->set_scene_pos(DATcenter, DATr);
			ui.widget->seted(DATmaxEdgeLen, DATminEdgeLen);
			ui.widget->setbb(DATbbMax, DATbbMin);
			ui.widget->setdatcellcen(DATcellcen);
			ui.widget->setdatbdy(DATbdy, DATedgever);
			ui.widget->setdraw_mode_(DAT5);
			update();
		});

	ui.actionConstructBSDAT->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructBSDAT->setStatusTip(("Create Bezier Solid DAT"));
	connect(ui.actionConstructBSDAT, &QAction::triggered,
		[=]() {
			auto start = system_clock::now();

			ui.widget->pickedge_e.clear();
			/*for (int i = 0; i < mesh->edges_.size(); ++i)
		{
			if (mesh->is_bdy(EdgeHandle(i)))
			{
				V3f v = mesh->cal_edge_mid(EdgeHandle(i));
				double min = 999;
				for (int i = 0; i < edgemid.size(); ++i)
				{
					double dis = distance(v, edgemid[i]);
					if (dis < min)
						min = dis;
				}
				if (min < 1.0e-5)
					ui.widget->pickedge_e.push_back(EdgeHandle(i));
			}
		}*/
			ui.widget->pickedge_e = pickedge1;
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
			AppCC.CCapproximation1();
			SolidData_ = AppCC.SolidData;
			vmesh = AppCC.constructBezierSolidDAT();

			auto end = system_clock::now();
			auto duration = duration_cast<microseconds>(end - start);
			std::cout << "������"
				<< double(duration.count()) * microseconds::period::num / microseconds::period::den
				<< "��" << std::endl;
			cal_bdy(vmesh);
			cal_cen(vmesh);
			std::vector<std::vector<V3f>> datnorm = cal_norm(vmesh);
			ui.widget->setedgeline(AppCC.line_quick);
			ui.widget->setvmesh(vmesh, datnorm);
			ui.widget->set_scene_pos(DATcenter, DATr);
			ui.widget->seted(DATmaxEdgeLen, DATminEdgeLen);
			ui.widget->setbb(DATbbMax, DATbbMin);
			ui.widget->setdatcellcen(DATcellcen);
			ui.widget->setdatbdy(DATbdy, DATedgever);
			ui.widget->setisoline(AppCC.isoline);
			ui.widget->setdraw_mode_(Isoline1);
			update();
		});

	ui.actionAppCCSolidDAT_1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAppCCSolidDAT_1->setStatusTip(("Create Bezier Solid DAT"));
	connect(ui.actionAppCCSolidDAT_1, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			AppCC.CCapproximation_1();
			SolidData_ = AppCC.SolidData;
			vmesh = AppCC.constructBezierSolidDAT();
			cal_bdy(vmesh);
			cal_cen(vmesh);
			std::vector<std::vector<V3f>> datnorm = cal_norm(vmesh);
			ui.widget->setvmesh(vmesh, datnorm);
			ui.widget->set_scene_pos(DATcenter, DATr);
			ui.widget->seted(DATmaxEdgeLen, DATminEdgeLen);
			ui.widget->setbb(DATbbMax, DATbbMin);
			ui.widget->setdatcellcen(DATcellcen);
			ui.widget->setdatbdy(DATbdy, DATedgever);
			ui.widget->setisoline(AppCC.isoline);
			ui.widget->setdraw_mode_(Isoline);
			update();
		});

	ui.actionAppCCsolidNSDAT->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAppCCsolidNSDAT->setStatusTip(("Create Control Points for Solid DAT no Singluar"));
	connect(ui.actionAppCCsolidNSDAT, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			vmesh = AppCC.CCapproximationDAT1();
			cal_bdy(vmesh);
			cal_cen(vmesh);
			std::vector<std::vector<V3f>> datnorm = cal_norm(vmesh);
			ui.widget->setvmesh(vmesh, datnorm);
			ui.widget->set_scene_pos(DATcenter, DATr);
			ui.widget->seted(DATmaxEdgeLen, DATminEdgeLen);
			ui.widget->setbb(DATbbMax, DATbbMin);
			ui.widget->setdatcellcen(DATcellcen);
			ui.widget->setdatbdy(DATbdy, DATedgever);
			ui.widget->setdraw_mode_(DAT1);
			update();
		});

	ui.actionConsturctBSNCDAT->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConsturctBSNCDAT->setStatusTip(("Create Bezier Solid DAT no Singluar"));
	connect(ui.actionConsturctBSNCDAT, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			AppCC.CCapproximation3();
			vmesh = AppCC.constructBezierSolidDAT();
			cal_bdy(vmesh);
			cal_cen(vmesh);
			std::vector<std::vector<V3f>> datnorm = cal_norm(vmesh);
			ui.widget->setvmesh(vmesh, datnorm);
			ui.widget->set_scene_pos(DATcenter, DATr);
			ui.widget->seted(DATmaxEdgeLen, DATminEdgeLen);
			ui.widget->setbb(DATbbMax, DATbbMin);
			ui.widget->setdatcellcen(DATcellcen);
			ui.widget->setdatbdy(DATbdy, DATedgever);
			ui.widget->setisoline(AppCC.isoline);
			ui.widget->setdraw_mode_(DAT2);
			update();
		});

	ui.actionClear_ALL->setIcon(QIcon("Images/ClearMesh.png"));
	ui.actionClear_ALL->setStatusTip(("Clear all Mesh"));
	connect(ui.actionClear_ALL, &QAction::triggered,
		[=]() {
			ui.widget->clear_mesh();
			delete mesh;
			mesh = new HexV3fMesh();
			isneed = false;
			update();
		});

	ui.actionPICKFACE->setIcon(QIcon("Images/ClearMesh.png"));
	ui.actionPICKFACE->setStatusTip(("Clear all Mesh"));
	connect(ui.actionPICKFACE, &QAction::triggered,
		[=]() {
			for (int i = 0; i < mesh->faces_.size(); ++i)
			{
				if (mesh->is_bdy(FaceHandle(i)))
				{
					V3f fv = mesh->cal_face_center(FaceHandle(i));
					if (fv.y == 0.3)
					{

						ui.widget->pickface.push_back(FaceHandle(i));
					}
				}
			}
			for (int i = 0; i < mesh->faces_.size(); ++i)
			{
				if (mesh->is_bdy(FaceHandle(i)))
				{
					V3f fv = mesh->cal_face_center(FaceHandle(i));
					if (fv.y < 0)
					{
						ui.widget->pickface.push_back(FaceHandle(i));
					}
				}
			}
		});

	ui.actionClear_edge->setIcon(QIcon("Images/ClearMesh.png"));
	ui.actionClear_edge->setStatusTip(("Clear all Mesh"));
	connect(ui.actionClear_edge, &QAction::triggered,
		[=]() {
			//ui.widget->pickedge_e.clear();
			ui.widget->pickedge_e.pop_back();
			update();
		});

	ui.actionAdd_Mesh->setStatusTip(("Add a New Mesh"));
	connect(ui.actionAdd_Mesh, &QAction::triggered,
		[=]() {
			mesh = new HexV3fMesh();

			/*V3f center(4.97576e-05,0.000187515,-4.13636e-05);
		double r = 0.013;
		V3f v1(center.x - r, center.y - r, center.z - r);
		V3f v2(center.x + r, center.y - r, center.z - r);
		V3f v3(center.x + r, center.y + r, center.z - r);
		V3f v4(center.x - r, center.y + r, center.z - r);
		V3f v5(center.x - r, center.y - r, center.z + r);
		V3f v6(center.x + r, center.y - r, center.z + r);
		V3f v7(center.x + r, center.y + r, center.z + r);
		V3f v8(center.x - r, center.y + r, center.z + r);*/
		/*V3f v1(0.938469 * 2, 4.66274 , 2.49 * 2);
	V3f v2(0.938469 * 2, 4.66274 , 2.21 * 2);
	V3f v3(0.938246 * 2, 4.33922 , 2.21 * 2);
	V3f v4(0.938246 * 2, 4.33922 , 2.49 * 2);
	V3f v5(2.06169 * 2, 4.66284 , 0.49 * 2);
	V3f v6(2.06169 * 2, 4.66284 , 0.21 * 2);
	V3f v7(2.05954 * 2, 4.33606 , 0.21 * 2);
	V3f v8(2.05954 * 2, 4.33606 , 0.49 * 2);*/
	/*V3f v1(0, 0, 0);
V3f v2(3, 0, 0);
V3f v3(3, 3, 0);
V3f v4(0, 3, 0);
V3f v5(0, 0, 3);
V3f v6(3, 0, 3);
V3f v7(3, 3, 3);
V3f v8(0, 3, 3);

V3f v9(1, 1, 1);
V3f v10(2, 1, 1);
V3f v11(2, 2, 1);
V3f v12(1, 2, 1);
V3f v13(1, 1, 2);
V3f v14(2, 1, 2);
V3f v15(2, 2, 2);
V3f v16(1, 2, 2);*/
			V3f v1(0, 620, -245);
			V3f v2(0, 620, 345);
			V3f v3(0, -70, 345);
			V3f v4(0, -70, -245);
			V3f v5(-40, 620, -245);
			V3f v6(-40, 620, 345);
			V3f v7(-40, -70, 345);
			V3f v8(-40, -70, -245);
			std::vector<VertexHandle> c1;
			c1.push_back(mesh->add_vertex(v1));
			c1.push_back(mesh->add_vertex(v2));
			c1.push_back(mesh->add_vertex(v3));
			c1.push_back(mesh->add_vertex(v4));
			c1.push_back(mesh->add_vertex(v5));
			c1.push_back(mesh->add_vertex(v6));
			c1.push_back(mesh->add_vertex(v7));
			c1.push_back(mesh->add_vertex(v8));
			mesh->add_cell(c1);
			/*std::vector<VertexHandle> c2;
		c2.push_back(mesh->add_vertex(v2));
		c2.push_back(mesh->add_vertex(v3));
		c2.push_back(mesh->add_vertex(v7));
		c2.push_back(mesh->add_vertex(v6));
		c2.push_back(mesh->add_vertex(v10));
		c2.push_back(mesh->add_vertex(v11));
		c2.push_back(mesh->add_vertex(v15));
		c2.push_back(mesh->add_vertex(v14));
		mesh->add_cell(c2);
		std::vector<VertexHandle> c3;
		c3.push_back(mesh->add_vertex(v5));
		c3.push_back(mesh->add_vertex(v6));
		c3.push_back(mesh->add_vertex(v7));
		c3.push_back(mesh->add_vertex(v8));
		c3.push_back(mesh->add_vertex(v13));
		c3.push_back(mesh->add_vertex(v14));
		c3.push_back(mesh->add_vertex(v15));
		c3.push_back(mesh->add_vertex(v16));
		mesh->add_cell(c3);
		std::vector<VertexHandle> c4;
		c4.push_back(mesh->add_vertex(v1));
		c4.push_back(mesh->add_vertex(v5));
		c4.push_back(mesh->add_vertex(v8));
		c4.push_back(mesh->add_vertex(v4));
		c4.push_back(mesh->add_vertex(v9));
		c4.push_back(mesh->add_vertex(v13));
		c4.push_back(mesh->add_vertex(v16));
		c4.push_back(mesh->add_vertex(v12));
		mesh->add_cell(c4);
		std::vector<VertexHandle> c5;
		c5.push_back(mesh->add_vertex(v9));
		c5.push_back(mesh->add_vertex(v10));
		c5.push_back(mesh->add_vertex(v11));
		c5.push_back(mesh->add_vertex(v12));
		c5.push_back(mesh->add_vertex(v13));
		c5.push_back(mesh->add_vertex(v14));
		c5.push_back(mesh->add_vertex(v15));
		c5.push_back(mesh->add_vertex(v16));
		mesh->add_cell(c5);
		std::vector<VertexHandle> c6;
		c6.push_back(mesh->add_vertex(v1));
		c6.push_back(mesh->add_vertex(v2));
		c6.push_back(mesh->add_vertex(v6));
		c6.push_back(mesh->add_vertex(v5));
		c6.push_back(mesh->add_vertex(v9));
		c6.push_back(mesh->add_vertex(v10));
		c6.push_back(mesh->add_vertex(v14));
		c6.push_back(mesh->add_vertex(v13));
		mesh->add_cell(c6);
		std::vector<VertexHandle> c7;
		c7.push_back(mesh->add_vertex(v12));
		c7.push_back(mesh->add_vertex(v11));
		c7.push_back(mesh->add_vertex(v15));
		c7.push_back(mesh->add_vertex(v16));
		c7.push_back(mesh->add_vertex(v4));
		c7.push_back(mesh->add_vertex(v3));
		c7.push_back(mesh->add_vertex(v7));
		c7.push_back(mesh->add_vertex(v8));
		mesh->add_cell(c7);*/

			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setdraw_mode_(FLAT_LINES);
			update();
		});

	ui.actionShow_Oriv->setStatusTip(("Show Original Vertex"));
	connect(ui.actionShow_Oriv, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ORIV);
			update();
		});

	ui.actionShow_BB->setStatusTip(("Show Boundary Box"));
	connect(ui.actionShow_BB, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ADD_BB);
			update();
		});

	ui.actionShow_OriBB->setStatusTip(("Show Original Boundary Box"));
	connect(ui.actionShow_OriBB, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ADD_ORIBB);
			update();
		});

	ui.actionShow_OvandV->setStatusTip(("Show Original Vertex And Vertex"));
	connect(ui.actionShow_OvandV, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(OV_V);
			update();
		});

	ui.actionShow_OvandV->setStatusTip(("Show Singular Edge And Vertex"));
	connect(ui.actionShow_SE_and_SV, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(SE_V);
			update();
		});

	ui.actionShi_xiong->setStatusTip(("Show Singular Edge And Vertex"));
	connect(ui.actionShi_xiong, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(SX);
			update();
		});

	ui.actionShow_BeiV->setStatusTip(("Show Vertex of Bezier"));
	connect(ui.actionShow_BeiV, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(BEIV);
			update();
		});

	ui.actionShow_Beiflatline->setStatusTip(("Show Flatline of Bezier"));
	connect(ui.actionShow_Beiflatline, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(BEISU);
			update();
		});

	ui.actionShow_Beiframe->setStatusTip(("Show Frameline of Bezier"));
	connect(ui.actionShow_Beiframe, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(BEI_FRAME);
			update();
		});

	ui.actionDatPoint->setStatusTip(("Show DatPoint1"));
	connect(ui.actionDatPoint, &QAction::triggered,
		[=]() {
			mesh = new HexV3fMesh();
			V3f v1_0(0, 0, 0);
			V3f v1_1(1, 0, 0);
			V3f v1_4(0, 1, 0);
			V3f v1_5(1, 1, 0);
			V3f v2_0(0, 0, 1);
			V3f v2_1(1, 0, 1);
			V3f v2_4(0, 1, 1);
			V3f v2_5(1, 1, 1);
			std::vector<VertexHandle> c101;
			c101.push_back(mesh->add_vertex(v2_0));
			c101.push_back(mesh->add_vertex(v2_4));
			c101.push_back(mesh->add_vertex(v2_5));
			c101.push_back(mesh->add_vertex(v2_1));
			c101.push_back(mesh->add_vertex(v1_0));
			c101.push_back(mesh->add_vertex(v1_4));
			c101.push_back(mesh->add_vertex(v1_5));
			c101.push_back(mesh->add_vertex(v1_1));
			mesh->add_cell(c101);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setdraw_mode_(DatPoint);
			update();
		});

	ui.actionDatPoint2->setStatusTip(("Show DatPoint2"));
	connect(ui.actionDatPoint2, &QAction::triggered,
		[=]() {
			mesh = new HexV3fMesh();
			V3f v1_0(0, 0, 0);
			V3f v1_1(1, 0, 0);
			V3f v1_2(2, 0, 0);
			V3f v1_3(3, 0, 0);
			V3f v1_4(0, 1, 0);
			V3f v1_5(1, 1, 0);
			V3f v1_6(2, 1, 0);
			V3f v1_7(3, 1, 0);
			V3f v1_8(0, 2, 0);
			V3f v1_9(1, 2, 0);
			V3f v1_10(2, 2, 0);
			V3f v1_11(3, 2, 0);
			V3f v1_12(0, 3, 0);
			V3f v1_13(1, 3, 0);
			V3f v1_14(2, 3, 0);
			V3f v1_15(3, 3, 0);

			V3f v2_0(0, 0, 1);
			V3f v2_1(1, 0, 1);
			V3f v2_2(2, 0, 1);
			V3f v2_3(3, 0, 1);
			V3f v2_4(0, 1, 1);
			V3f v2_5(1, 1, 1);
			V3f v2_6(2, 1, 1);
			V3f v2_7(3, 1, 1);
			V3f v2_8(0, 2, 1);
			V3f v2_9(1, 2, 1);
			V3f v2_10(2, 2, 1);
			V3f v2_11(3, 2, 1);
			V3f v2_12(0, 3, 1);
			V3f v2_13(1, 3, 1);
			V3f v2_14(2, 3, 1);
			V3f v2_15(3, 3, 1);

			V3f v3_0(0, 0, 2);
			V3f v3_1(1, 0, 2);
			V3f v3_2(2, 0, 2);
			V3f v3_3(3, 0, 2);
			V3f v3_4(0, 1, 2);
			V3f v3_5(1, 1, 2);
			V3f v3_6(2, 1, 2);
			V3f v3_7(3, 1, 2);
			V3f v3_8(0, 2, 2);
			V3f v3_9(1, 2, 2);
			V3f v3_10(2, 2, 2);
			V3f v3_11(3, 2, 2);
			V3f v3_12(0, 3, 2);
			V3f v3_13(1, 3, 2);
			V3f v3_14(2, 3, 2);
			V3f v3_15(3, 3, 2);

			V3f v4_0(0, 0, 3);
			V3f v4_1(1, 0, 3);
			V3f v4_2(2, 0, 3);
			V3f v4_3(3, 0, 3);
			V3f v4_4(0, 1, 3);
			V3f v4_5(1, 1, 3);
			V3f v4_6(2, 1, 3);
			V3f v4_7(3, 1, 3);
			V3f v4_8(0, 2, 3);
			V3f v4_9(1, 2, 3);
			V3f v4_10(2, 2, 3);
			V3f v4_11(3, 2, 3);
			V3f v4_12(0, 3, 3);
			V3f v4_13(1, 3, 3);
			V3f v4_14(2, 3, 3);
			V3f v4_15(3, 3, 3);

			std::vector<VertexHandle> c101;
			c101.push_back(mesh->add_vertex(v2_0));
			c101.push_back(mesh->add_vertex(v2_4));
			c101.push_back(mesh->add_vertex(v2_5));
			c101.push_back(mesh->add_vertex(v2_1));
			c101.push_back(mesh->add_vertex(v1_0));
			c101.push_back(mesh->add_vertex(v1_4));
			c101.push_back(mesh->add_vertex(v1_5));
			c101.push_back(mesh->add_vertex(v1_1));
			mesh->add_cell(c101);
			std::vector<VertexHandle> c102;
			c102.push_back(mesh->add_vertex(v2_1));
			c102.push_back(mesh->add_vertex(v2_5));
			c102.push_back(mesh->add_vertex(v2_6));
			c102.push_back(mesh->add_vertex(v2_2));
			c102.push_back(mesh->add_vertex(v1_1));
			c102.push_back(mesh->add_vertex(v1_5));
			c102.push_back(mesh->add_vertex(v1_6));
			c102.push_back(mesh->add_vertex(v1_2));
			mesh->add_cell(c102);
			std::vector<VertexHandle> c103;
			c103.push_back(mesh->add_vertex(v2_2));
			c103.push_back(mesh->add_vertex(v2_6));
			c103.push_back(mesh->add_vertex(v2_7));
			c103.push_back(mesh->add_vertex(v2_3));
			c103.push_back(mesh->add_vertex(v1_2));
			c103.push_back(mesh->add_vertex(v1_6));
			c103.push_back(mesh->add_vertex(v1_7));
			c103.push_back(mesh->add_vertex(v1_3));
			mesh->add_cell(c103);
			std::vector<VertexHandle> c104;
			c104.push_back(mesh->add_vertex(v2_4));
			c104.push_back(mesh->add_vertex(v2_8));
			c104.push_back(mesh->add_vertex(v2_9));
			c104.push_back(mesh->add_vertex(v2_5));
			c104.push_back(mesh->add_vertex(v1_4));
			c104.push_back(mesh->add_vertex(v1_8));
			c104.push_back(mesh->add_vertex(v1_9));
			c104.push_back(mesh->add_vertex(v1_5));
			mesh->add_cell(c104);
			std::vector<VertexHandle> c105;
			c105.push_back(mesh->add_vertex(v2_5));
			c105.push_back(mesh->add_vertex(v2_9));
			c105.push_back(mesh->add_vertex(v2_10));
			c105.push_back(mesh->add_vertex(v2_6));
			c105.push_back(mesh->add_vertex(v1_5));
			c105.push_back(mesh->add_vertex(v1_9));
			c105.push_back(mesh->add_vertex(v1_10));
			c105.push_back(mesh->add_vertex(v1_6));
			mesh->add_cell(c105);
			std::vector<VertexHandle> c106;
			c106.push_back(mesh->add_vertex(v2_6));
			c106.push_back(mesh->add_vertex(v2_10));
			c106.push_back(mesh->add_vertex(v2_11));
			c106.push_back(mesh->add_vertex(v2_7));
			c106.push_back(mesh->add_vertex(v1_6));
			c106.push_back(mesh->add_vertex(v1_10));
			c106.push_back(mesh->add_vertex(v1_11));
			c106.push_back(mesh->add_vertex(v1_7));
			mesh->add_cell(c106);
			std::vector<VertexHandle> c107;
			c107.push_back(mesh->add_vertex(v2_8));
			c107.push_back(mesh->add_vertex(v2_12));
			c107.push_back(mesh->add_vertex(v2_13));
			c107.push_back(mesh->add_vertex(v2_9));
			c107.push_back(mesh->add_vertex(v1_8));
			c107.push_back(mesh->add_vertex(v1_12));
			c107.push_back(mesh->add_vertex(v1_13));
			c107.push_back(mesh->add_vertex(v1_9));
			mesh->add_cell(c107);
			std::vector<VertexHandle> c108;
			c108.push_back(mesh->add_vertex(v2_9));
			c108.push_back(mesh->add_vertex(v2_13));
			c108.push_back(mesh->add_vertex(v2_14));
			c108.push_back(mesh->add_vertex(v2_10));
			c108.push_back(mesh->add_vertex(v1_9));
			c108.push_back(mesh->add_vertex(v1_13));
			c108.push_back(mesh->add_vertex(v1_14));
			c108.push_back(mesh->add_vertex(v1_10));
			mesh->add_cell(c108);
			std::vector<VertexHandle> c109;
			c109.push_back(mesh->add_vertex(v2_10));
			c109.push_back(mesh->add_vertex(v2_14));
			c109.push_back(mesh->add_vertex(v2_15));
			c109.push_back(mesh->add_vertex(v2_11));
			c109.push_back(mesh->add_vertex(v1_10));
			c109.push_back(mesh->add_vertex(v1_14));
			c109.push_back(mesh->add_vertex(v1_15));
			c109.push_back(mesh->add_vertex(v1_11));
			mesh->add_cell(c109);

			std::vector<VertexHandle> c201;
			c201.push_back(mesh->add_vertex(v3_0));
			c201.push_back(mesh->add_vertex(v3_4));
			c201.push_back(mesh->add_vertex(v3_5));
			c201.push_back(mesh->add_vertex(v3_1));
			c201.push_back(mesh->add_vertex(v2_0));
			c201.push_back(mesh->add_vertex(v2_4));
			c201.push_back(mesh->add_vertex(v2_5));
			c201.push_back(mesh->add_vertex(v2_1));
			mesh->add_cell(c201);
			std::vector<VertexHandle> c202;
			c202.push_back(mesh->add_vertex(v3_1));
			c202.push_back(mesh->add_vertex(v3_5));
			c202.push_back(mesh->add_vertex(v3_6));
			c202.push_back(mesh->add_vertex(v3_2));
			c202.push_back(mesh->add_vertex(v2_1));
			c202.push_back(mesh->add_vertex(v2_5));
			c202.push_back(mesh->add_vertex(v2_6));
			c202.push_back(mesh->add_vertex(v2_2));
			mesh->add_cell(c202);
			std::vector<VertexHandle> c203;
			c203.push_back(mesh->add_vertex(v3_2));
			c203.push_back(mesh->add_vertex(v3_6));
			c203.push_back(mesh->add_vertex(v3_7));
			c203.push_back(mesh->add_vertex(v3_3));
			c203.push_back(mesh->add_vertex(v2_2));
			c203.push_back(mesh->add_vertex(v2_6));
			c203.push_back(mesh->add_vertex(v2_7));
			c203.push_back(mesh->add_vertex(v2_3));
			mesh->add_cell(c203);
			std::vector<VertexHandle> c204;
			c204.push_back(mesh->add_vertex(v3_4));
			c204.push_back(mesh->add_vertex(v3_8));
			c204.push_back(mesh->add_vertex(v3_9));
			c204.push_back(mesh->add_vertex(v3_5));
			c204.push_back(mesh->add_vertex(v2_4));
			c204.push_back(mesh->add_vertex(v2_8));
			c204.push_back(mesh->add_vertex(v2_9));
			c204.push_back(mesh->add_vertex(v2_5));
			mesh->add_cell(c204);
			std::vector<VertexHandle> c205;
			c205.push_back(mesh->add_vertex(v3_5));
			c205.push_back(mesh->add_vertex(v3_9));
			c205.push_back(mesh->add_vertex(v3_10));
			c205.push_back(mesh->add_vertex(v3_6));
			c205.push_back(mesh->add_vertex(v2_5));
			c205.push_back(mesh->add_vertex(v2_9));
			c205.push_back(mesh->add_vertex(v2_10));
			c205.push_back(mesh->add_vertex(v2_6));
			mesh->add_cell(c205);
			std::vector<VertexHandle> c206;
			c206.push_back(mesh->add_vertex(v3_6));
			c206.push_back(mesh->add_vertex(v3_10));
			c206.push_back(mesh->add_vertex(v3_11));
			c206.push_back(mesh->add_vertex(v3_7));
			c206.push_back(mesh->add_vertex(v2_6));
			c206.push_back(mesh->add_vertex(v2_10));
			c206.push_back(mesh->add_vertex(v2_11));
			c206.push_back(mesh->add_vertex(v2_7));
			mesh->add_cell(c206);
			std::vector<VertexHandle> c207;
			c207.push_back(mesh->add_vertex(v3_8));
			c207.push_back(mesh->add_vertex(v3_12));
			c207.push_back(mesh->add_vertex(v3_13));
			c207.push_back(mesh->add_vertex(v3_9));
			c207.push_back(mesh->add_vertex(v2_8));
			c207.push_back(mesh->add_vertex(v2_12));
			c207.push_back(mesh->add_vertex(v2_13));
			c207.push_back(mesh->add_vertex(v2_9));
			mesh->add_cell(c207);
			std::vector<VertexHandle> c208;
			c208.push_back(mesh->add_vertex(v3_9));
			c208.push_back(mesh->add_vertex(v3_13));
			c208.push_back(mesh->add_vertex(v3_14));
			c208.push_back(mesh->add_vertex(v3_10));
			c208.push_back(mesh->add_vertex(v2_9));
			c208.push_back(mesh->add_vertex(v2_13));
			c208.push_back(mesh->add_vertex(v2_14));
			c208.push_back(mesh->add_vertex(v2_10));
			mesh->add_cell(c208);
			std::vector<VertexHandle> c209;
			c209.push_back(mesh->add_vertex(v3_10));
			c209.push_back(mesh->add_vertex(v3_14));
			c209.push_back(mesh->add_vertex(v3_15));
			c209.push_back(mesh->add_vertex(v3_11));
			c209.push_back(mesh->add_vertex(v2_10));
			c209.push_back(mesh->add_vertex(v2_14));
			c209.push_back(mesh->add_vertex(v2_15));
			c209.push_back(mesh->add_vertex(v2_11));
			mesh->add_cell(c209);

			std::vector<VertexHandle> c301;
			c301.push_back(mesh->add_vertex(v4_0));
			c301.push_back(mesh->add_vertex(v4_4));
			c301.push_back(mesh->add_vertex(v4_5));
			c301.push_back(mesh->add_vertex(v4_1));
			c301.push_back(mesh->add_vertex(v3_0));
			c301.push_back(mesh->add_vertex(v3_4));
			c301.push_back(mesh->add_vertex(v3_5));
			c301.push_back(mesh->add_vertex(v3_1));
			mesh->add_cell(c301);
			std::vector<VertexHandle> c302;
			c302.push_back(mesh->add_vertex(v4_1));
			c302.push_back(mesh->add_vertex(v4_5));
			c302.push_back(mesh->add_vertex(v4_6));
			c302.push_back(mesh->add_vertex(v4_2));
			c302.push_back(mesh->add_vertex(v3_1));
			c302.push_back(mesh->add_vertex(v3_5));
			c302.push_back(mesh->add_vertex(v3_6));
			c302.push_back(mesh->add_vertex(v3_2));
			mesh->add_cell(c302);
			std::vector<VertexHandle> c303;
			c303.push_back(mesh->add_vertex(v4_2));
			c303.push_back(mesh->add_vertex(v4_6));
			c303.push_back(mesh->add_vertex(v4_7));
			c303.push_back(mesh->add_vertex(v4_3));
			c303.push_back(mesh->add_vertex(v3_2));
			c303.push_back(mesh->add_vertex(v3_6));
			c303.push_back(mesh->add_vertex(v3_7));
			c303.push_back(mesh->add_vertex(v3_3));
			mesh->add_cell(c303);
			std::vector<VertexHandle> c304;
			c304.push_back(mesh->add_vertex(v4_4));
			c304.push_back(mesh->add_vertex(v4_8));
			c304.push_back(mesh->add_vertex(v4_9));
			c304.push_back(mesh->add_vertex(v4_5));
			c304.push_back(mesh->add_vertex(v3_4));
			c304.push_back(mesh->add_vertex(v3_8));
			c304.push_back(mesh->add_vertex(v3_9));
			c304.push_back(mesh->add_vertex(v3_5));
			mesh->add_cell(c304);
			std::vector<VertexHandle> c305;
			c305.push_back(mesh->add_vertex(v4_5));
			c305.push_back(mesh->add_vertex(v4_9));
			c305.push_back(mesh->add_vertex(v4_10));
			c305.push_back(mesh->add_vertex(v4_6));
			c305.push_back(mesh->add_vertex(v3_5));
			c305.push_back(mesh->add_vertex(v3_9));
			c305.push_back(mesh->add_vertex(v3_10));
			c305.push_back(mesh->add_vertex(v3_6));
			mesh->add_cell(c305);
			std::vector<VertexHandle> c306;
			c306.push_back(mesh->add_vertex(v4_6));
			c306.push_back(mesh->add_vertex(v4_10));
			c306.push_back(mesh->add_vertex(v4_11));
			c306.push_back(mesh->add_vertex(v4_7));
			c306.push_back(mesh->add_vertex(v3_6));
			c306.push_back(mesh->add_vertex(v3_10));
			c306.push_back(mesh->add_vertex(v3_11));
			c306.push_back(mesh->add_vertex(v3_7));
			mesh->add_cell(c306);
			std::vector<VertexHandle> c307;
			c307.push_back(mesh->add_vertex(v4_8));
			c307.push_back(mesh->add_vertex(v4_12));
			c307.push_back(mesh->add_vertex(v4_13));
			c307.push_back(mesh->add_vertex(v4_9));
			c307.push_back(mesh->add_vertex(v3_8));
			c307.push_back(mesh->add_vertex(v3_12));
			c307.push_back(mesh->add_vertex(v3_13));
			c307.push_back(mesh->add_vertex(v3_9));
			mesh->add_cell(c307);
			std::vector<VertexHandle> c308;
			c308.push_back(mesh->add_vertex(v4_9));
			c308.push_back(mesh->add_vertex(v4_13));
			c308.push_back(mesh->add_vertex(v4_14));
			c308.push_back(mesh->add_vertex(v4_10));
			c308.push_back(mesh->add_vertex(v3_9));
			c308.push_back(mesh->add_vertex(v3_13));
			c308.push_back(mesh->add_vertex(v3_14));
			c308.push_back(mesh->add_vertex(v3_10));
			mesh->add_cell(c308);
			std::vector<VertexHandle> c309;
			c309.push_back(mesh->add_vertex(v4_10));
			c309.push_back(mesh->add_vertex(v4_14));
			c309.push_back(mesh->add_vertex(v4_15));
			c309.push_back(mesh->add_vertex(v4_11));
			c309.push_back(mesh->add_vertex(v3_10));
			c309.push_back(mesh->add_vertex(v3_14));
			c309.push_back(mesh->add_vertex(v3_15));
			c309.push_back(mesh->add_vertex(v3_11));
			mesh->add_cell(c309);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->draw_mesh_all_pointsDatpoint2();
			ui.widget->setdraw_mode_(DatPoint2);
			update();
		});

	ui.actionInterior_Point->setStatusTip(("Show Interior Point"));
	connect(ui.actionInterior_Point, &QAction::triggered,
		[=]() {
			mesh = new HexV3fMesh();
			V3f v1_0(0, 0, 0);
			V3f v1_1(1, 0, 0);
			V3f v1_4(0, 1, 0);
			V3f v1_5(1, 1, 0);
			V3f v2_0(0, 0, 1);
			V3f v2_1(1, 0, 1);
			V3f v2_4(0, 1, 1);
			V3f v2_5(1, 1, 1);
			V3f v3_0(0, 0, 2);
			V3f v3_1(1, 0, 2);
			V3f v3_4(0, 1, 2);
			V3f v3_5(1, 1, 2);
			std::vector<VertexHandle> c101;
			c101.push_back(mesh->add_vertex(v2_0));
			c101.push_back(mesh->add_vertex(v2_4));
			c101.push_back(mesh->add_vertex(v2_5));
			c101.push_back(mesh->add_vertex(v2_1));
			c101.push_back(mesh->add_vertex(v1_0));
			c101.push_back(mesh->add_vertex(v1_4));
			c101.push_back(mesh->add_vertex(v1_5));
			c101.push_back(mesh->add_vertex(v1_1));
			mesh->add_cell(c101);
			std::vector<VertexHandle> c201;
			c201.push_back(mesh->add_vertex(v3_0));
			c201.push_back(mesh->add_vertex(v3_4));
			c201.push_back(mesh->add_vertex(v3_5));
			c201.push_back(mesh->add_vertex(v3_1));
			c201.push_back(mesh->add_vertex(v2_0));
			c201.push_back(mesh->add_vertex(v2_4));
			c201.push_back(mesh->add_vertex(v2_5));
			c201.push_back(mesh->add_vertex(v2_1));
			mesh->add_cell(c201);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setdraw_mode_(Interior_Point);
			update();
		});

	ui.actionEdge_Point->setStatusTip(("Show Edge Point"));
	connect(ui.actionEdge_Point, &QAction::triggered,
		[=]() {
			mesh = new HexV3fMesh();
			V3f v1_0(0, 0, 0);
			V3f v1_1(1, 0, 0);
			V3f v1_2(0.5, 0.5, 0);
			V3f v1_3(1, 0.5, 0);
			V3f v1_4(0, 1, 0);
			V3f v1_5(0.5, 1, 0);
			V3f v1_6(1, 1, 0);
			V3f v2_0(0, 0, 0.5);
			V3f v2_1(1, 0, 0.5);
			V3f v2_2(0.5, 0.5, 0.5);
			V3f v2_3(1, 0.5, 0.5);
			V3f v2_4(0, 1, 0.5);
			V3f v2_5(0.5, 1, 0.5);
			V3f v2_6(1, 1, 0.5);
			std::vector<VertexHandle> c101;
			c101.push_back(mesh->add_vertex(v2_0));
			c101.push_back(mesh->add_vertex(v2_4));
			c101.push_back(mesh->add_vertex(v2_5));
			c101.push_back(mesh->add_vertex(v2_2));
			c101.push_back(mesh->add_vertex(v1_0));
			c101.push_back(mesh->add_vertex(v1_4));
			c101.push_back(mesh->add_vertex(v1_5));
			c101.push_back(mesh->add_vertex(v1_2));
			mesh->add_cell(c101);
			std::vector<VertexHandle> c102;
			c102.push_back(mesh->add_vertex(v2_2));
			c102.push_back(mesh->add_vertex(v2_5));
			c102.push_back(mesh->add_vertex(v2_6));
			c102.push_back(mesh->add_vertex(v2_3));
			c102.push_back(mesh->add_vertex(v1_2));
			c102.push_back(mesh->add_vertex(v1_5));
			c102.push_back(mesh->add_vertex(v1_6));
			c102.push_back(mesh->add_vertex(v1_3));
			mesh->add_cell(c102);
			std::vector<VertexHandle> c103;
			c103.push_back(mesh->add_vertex(v2_0));
			c103.push_back(mesh->add_vertex(v2_2));
			c103.push_back(mesh->add_vertex(v2_3));
			c103.push_back(mesh->add_vertex(v2_1));
			c103.push_back(mesh->add_vertex(v1_0));
			c103.push_back(mesh->add_vertex(v1_2));
			c103.push_back(mesh->add_vertex(v1_3));
			c103.push_back(mesh->add_vertex(v1_1));
			mesh->add_cell(c103);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setdraw_mode_(Edge_Point);
			update();
		});

	ui.actionCorner_Point->setStatusTip(("Show Corner Point"));
	connect(ui.actionCorner_Point, &QAction::triggered,
		[=]() {
			mesh = new HexV3fMesh();
			V3f v1_0(0, 0, 0);
			V3f v1_1(1, 0, 0);
			V3f v1_2(2, 0, 0);
			V3f v1_3(0, 1, 0);
			V3f v1_4(1, 1, 0);
			V3f v1_5(0, 2, 0);
			V3f v1_6(2, 2, 0);
			V3f v2_0(0, 0, 1);
			V3f v2_1(1, 0, 1);
			V3f v2_3(0, 1, 1);
			V3f v2_4(1, 1, 1);
			V3f v3_0(0, 0, 2);
			V3f v3_1(2, 0, 2);
			V3f v3_2(0, 2, 2);
			V3f v3_3(2, 2, 2);
			std::vector<VertexHandle> c101;
			c101.push_back(mesh->add_vertex(v2_0));
			c101.push_back(mesh->add_vertex(v2_3));
			c101.push_back(mesh->add_vertex(v2_4));
			c101.push_back(mesh->add_vertex(v2_1));
			c101.push_back(mesh->add_vertex(v1_0));
			c101.push_back(mesh->add_vertex(v1_3));
			c101.push_back(mesh->add_vertex(v1_4));
			c101.push_back(mesh->add_vertex(v1_1));
			mesh->add_cell(c101);
			std::vector<VertexHandle> c102;
			c102.push_back(mesh->add_vertex(v3_0));
			c102.push_back(mesh->add_vertex(v3_2));
			c102.push_back(mesh->add_vertex(v3_3));
			c102.push_back(mesh->add_vertex(v3_1));
			c102.push_back(mesh->add_vertex(v2_0));
			c102.push_back(mesh->add_vertex(v2_3));
			c102.push_back(mesh->add_vertex(v2_4));
			c102.push_back(mesh->add_vertex(v2_1));
			mesh->add_cell(c102);
			std::vector<VertexHandle> c103;
			c103.push_back(mesh->add_vertex(v2_1));
			c103.push_back(mesh->add_vertex(v2_4));
			c103.push_back(mesh->add_vertex(v3_3));
			c103.push_back(mesh->add_vertex(v3_1));
			c103.push_back(mesh->add_vertex(v1_1));
			c103.push_back(mesh->add_vertex(v1_4));
			c103.push_back(mesh->add_vertex(v1_6));
			c103.push_back(mesh->add_vertex(v1_2));
			mesh->add_cell(c103);
			std::vector<VertexHandle> c104;
			c104.push_back(mesh->add_vertex(v2_3));
			c104.push_back(mesh->add_vertex(v3_2));
			c104.push_back(mesh->add_vertex(v3_3));
			c104.push_back(mesh->add_vertex(v2_4));
			c104.push_back(mesh->add_vertex(v1_3));
			c104.push_back(mesh->add_vertex(v1_5));
			c104.push_back(mesh->add_vertex(v1_6));
			c104.push_back(mesh->add_vertex(v1_4));
			mesh->add_cell(c104);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setdraw_mode_(Corner_Point);
			update();
		});

	ui.actionView_singular->setStatusTip(("View singular"));
	connect(ui.actionView_singular, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(View_singular);
			update();
		});

	ui.actionSelect_one_point->setStatusTip(("Select a point"));
	connect(ui.actionSelect_one_point, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			V3f v = AppCC.selectapoint();
			ui.widget->setselectpoint(v);
		});

	ui.actionInitialModel->setStatusTip(("Initial Model"));
	connect(ui.actionInitialModel, &QAction::triggered,
		[=]() {
			twonpoint.swap(std::vector<V3f>());
			V3f v1(0, 0, 0);
			V3f v2(1, 0, 0);
			V3f v3(1, 1, 0);
			V3f v4(0, 1, 0);
			twonpoint.push_back(v1);
			twonpoint.push_back(v2);
			twonpoint.push_back(v3);
			twonpoint.push_back(v4);
			V3f center = cal_vector_center(twonpoint);
			double r = cal_vector_r(twonpoint, center);
			ui.widget->settwonpoint(twonpoint);
			ui.widget->set_scene_pos(center, r);
			ui.widget->setdraw_mode_(TWOPOINT);
		});

	ui.action4_Point->setStatusTip(("4 Point"));
	connect(ui.action4_Point, &QAction::triggered,
		[=]() {
			int twonpoint_num = twonpoint.size();
			std::vector<V3f> newtwopoint(2 * twonpoint_num, V3f(0, 0, 0));
			for (int i = 0; i < newtwopoint.size(); i++)
			{
				if (i % 2 == 0)
					newtwopoint[i] = twonpoint[i / 2];
				else
				{
					int j = (i - 1) / 2;
					newtwopoint[i] = twonpoint[j] * 9 / 16 + twonpoint[(j + 1) % twonpoint_num] * 9 / 16 -
						twonpoint[(j - 1 + twonpoint_num) % twonpoint_num] / 16 - twonpoint[(j + 2) % twonpoint_num] / 16;
				}
			}
			twonpoint = newtwopoint;
			V3f center = cal_vector_center(twonpoint);
			double r = cal_vector_r(twonpoint, center);
			ui.widget->settwonpoint(twonpoint);
			ui.widget->set_scene_pos(center, r);
			ui.widget->setdraw_mode_(TWOPOINT);
		});

	ui.action6_Point->setStatusTip(("6 Point"));
	connect(ui.action6_Point, &QAction::triggered,
		[=]() {
			int twonpoint_num = twonpoint.size();
			std::vector<V3f> newtwopoint(2 * twonpoint_num, V3f(0, 0, 0));
			for (int i = 0; i < newtwopoint.size(); i++)
			{
				if (i % 2 == 0)
					newtwopoint[i] = twonpoint[i / 2];
				else
				{
					int j = (i - 1) / 2;
					newtwopoint[i] = twonpoint[j] * 150 / 256 + twonpoint[(j + 1) % twonpoint_num] * 150 / 256 -
						twonpoint[(j - 1 + twonpoint_num) % twonpoint_num] * 25 / 256 + twonpoint[(j - 2 + twonpoint_num) % twonpoint_num] * 3 / 256 - twonpoint[(j + 2) % twonpoint_num] * 25 / 256 + twonpoint[(j + 3) % twonpoint_num] * 3 / 256;
				}
			}
			twonpoint = newtwopoint;
			V3f center = cal_vector_center(twonpoint);
			double r = cal_vector_r(twonpoint, center);
			ui.widget->settwonpoint(twonpoint);
			ui.widget->set_scene_pos(center, r);
			ui.widget->setdraw_mode_(TWOPOINT);
		});

	ui.action2n_Point_Line->setStatusTip(("2n Point Line"));
	connect(ui.action2n_Point_Line, &QAction::triggered,
		[=]() {
			int twonpoint_num = twonpoint.size();
			std::vector<V3f> newtwopoint(2 * twonpoint_num, V3f(0, 0, 0));
			int iteration_num = 10;
			std::vector<std::vector<V3f>> linepoint(iteration_num + 1, std::vector<V3f>(twonpoint_num, V3f(0, 0, 0)));
			std::vector<std::vector<V3f>> D(iteration_num + 1, std::vector<V3f>(twonpoint_num, V3f(0, 0, 0)));
			for (int i = 1; i <= iteration_num; i++)
			{
				if (i == 1)
				{
					for (int line_num = 0; line_num < twonpoint_num; line_num++)
					{
						linepoint[i][line_num] = (twonpoint[line_num] + twonpoint[(line_num + 1) % twonpoint_num]) / 2;
						D[i][line_num] = linepoint[i][line_num] - linepoint[i - 1][line_num];
					}
				}
				else
				{
					for (int line_num = 0; line_num < twonpoint_num; line_num++)
					{
						linepoint[i][line_num] = linepoint[i - 1][line_num] + (D[i - 1][line_num] * 2 - D[i - 1][(line_num - 1 + twonpoint_num) % twonpoint_num] - D[i - 1][(line_num + 1) % twonpoint_num]) * ((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
						D[i][line_num] = linepoint[i][line_num] - linepoint[i - 1][line_num];
					}
				}
			}
			for (int i = 0; i < 2 * twonpoint_num; i++)
			{
				if (i % 2 == 0)
					newtwopoint[i] = twonpoint[i / 2];
				else
					newtwopoint[i] = linepoint[iteration_num][i / 2];
			}
			twonpoint = newtwopoint;
			V3f center = cal_vector_center(twonpoint);
			double r = cal_vector_r(twonpoint, center);
			ui.widget->settwonpoint(twonpoint);
			ui.widget->set_scene_pos(center, r);
			ui.widget->setdraw_mode_(TWOPOINT);
		});

	ui.action2n_Point->setStatusTip(("2n Point Solid"));
	connect(ui.action2n_Point, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			TwoNSubdivision Twonsub(mesh, newmesh, 4, mesh->oriv);
			*mesh = Twonsub.TwoNSubdiving();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionDelete_Cell->setStatusTip(("delete cell"));
	connect(ui.actionDelete_Cell, &QAction::triggered,
		[=]() {
			mesh->examine_bdy();

			mesh->cal_cen();
			mesh->cal_cell_cen();

			/*double maxz = -999, minz = 999;
		for (int i = 0; i < mesh->vertices_.size(); ++i)
		{
			V3f v = mesh->vertices_[i];
			if (maxz < v.z)
				maxz = v.z;
			if (minz > v.z)
				minz = v.z;
		}
		double delete_z = (maxz + minz) / 2;
		std::vector<CellHandle> dc;
		for (int i = 0; i < mesh->cells_.size(); ++i)
		{
			if (mesh->cell_center[i].z < delete_z)
				dc.push_back(CellHandle(i));
		}
		for (std::vector<CellHandle>::iterator dc_it = dc.begin();dc_it != dc.end();dc_it++)
		{
			mesh->delete_cell(*dc_it);
		}*/
			std::vector<CellHandle> dc = ui.widget->pickcell;
			/*for (int i = 0; i < mesh->cells_.size(); ++i)
		{
			if(std::find(dc.begin(), dc.end(), CellHandle(i)) == dc.end())
				mesh->delete_cell(CellHandle(i));
		}*/
			for (std::vector<CellHandle>::iterator dc_it = dc.begin(); dc_it != dc.end(); dc_it++)
			{
				mesh->delete_cell(*dc_it);
			}
			ui.widget->pickcell.swap(std::vector<CellHandle>());

			/*std::vector<CellHandle> dc;
		for (int i = 0; i < mesh->cells_.size(); ++i)
		{
			V3f v = mesh->cell_center[i];
			if (v.x < 4)
				dc.push_back(CellHandle(i));
		}
		for (std::vector<CellHandle>::iterator dc_it = dc.begin(); dc_it != dc.end(); dc_it++)
		{
			mesh->delete_cell(*dc_it);
		}*/
		/*std::vector<FaceHandle> pf = ui.widget->pickface;
	for (int i = 0; i < pf.size(); ++i)
	{
		std::set<CellHandle, compare_OVM> pfc = mesh->neighbor_f[pf[i]];
		for(auto it = pfc.begin();it!= pfc.end(); ++it)
			mesh->delete_cell(*it);
	}*/
			FileHandler filenew = FileHandler();
			std::string name("delete.mesh");
			filenew.writeFile(name, mesh);
			//mesh = new HexV3fMesh();
			mesh = new HexV3fMesh();
			FileHandler file("delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionDelete_Cell1->setStatusTip(("delete cell"));
	connect(ui.actionDelete_Cell1, &QAction::triggered,
		[=]() {
			mesh->examine_bdy();

			mesh->cal_cen();
			mesh->cal_cell_cen();

			int cell_num = mesh->cells_.size();
			//std::vector<CellHandle> dc;
			//std::vector<CellHandle> dc1;
			std::set<int> dcc;
			//for (int i = 0;i < cell_num;i++)
			//{
			//	if (mesh->is_bdy(CellHandle(i)))
			//	{
			//		if (mesh->neighbor_c_c(CellHandle(i)).size() <= 2)
			//			dc.push_back(CellHandle(i));
			//	}
			//}
			//for (std::vector<CellHandle>::iterator dc_it = dc.begin();dc_it != dc.end();dc_it++)
			//{
			//	mesh->delete_cell(*dc_it);
			//}
			////���ж�һ��
			//for (int i = 0; i < cell_num; i++)
			//{
			//	if (mesh->is_bdy(CellHandle(i)))
			//	{
			//		if (mesh->neighbor_c_c(CellHandle(i)).size() <= 1)
			//			dc1.push_back(CellHandle(i));
			//	}
			//}
			//for (std::vector<CellHandle>::iterator dc1_it = dc1.begin(); dc1_it != dc1.end(); dc1_it++)
			//{
			//	mesh->delete_cell(*dc1_it);
			//}
			//for (int i = 0; i < cell_num; i++)
			//{
			//	V3f center_temp1 = mesh->cell_center[i];
			//	if (center_temp1.z > 1.33 && center_temp1.z < 1.82)
			//	{
			//		bool check = false;
			//		for (int j = 0; j < cell_num; j++)
			//		{
			//			/*if (j == i)
			//				continue;*/
			//			if (center_temp1.z > 2.52 && center_temp1.z < 3.01)
			//			{
			//				V3f center_temp2 = mesh->cell_center[j];
			//				double dis = sqrt((center_temp1.x - center_temp2.x)*(center_temp1.x - center_temp2.x) +
			//					(center_temp1.y - center_temp2.y)*(center_temp1.y - center_temp2.y));
			//				if (dis < 1e-3)
			//				{
			//					check = true;
			//					break;
			//				}
			//			}
			//		}
			//		if (!check)
			//			dcc.insert(i);
			//	}
			//}
			/*for (int i = 0; i < cell_num; i++)
		{
			V3f center_temp1 = mesh->cell_center[i];
			if (center_temp1.y > 2.25&&center_temp1.y<2.75&&center_temp1.z>0.2857&&center_temp1.z < 0.7142)
				dcc.insert(CellHandle(i));
		}
		std::cout << "dcc size: " << dcc.size() << std::endl;*/
			for (int i = 0; i < cell_num; i++)
			{
				V3f center_temp1 = mesh->cell_center[i];
				if (center_temp1.x > 0.357142 && center_temp1.x < 4.64286)
				{
					if (center_temp1.z > 3 && center_temp1.z < 3.7)
						dcc.insert(CellHandle(i));
					else if (center_temp1.z > 1.82 && center_temp1.z < 2.52)
						dcc.insert(CellHandle(i));
					else if (center_temp1.z > 0.63 && center_temp1.z < 1.33)
						dcc.insert(CellHandle(i));
					else if (center_temp1.z < 0.35)
						dcc.insert(CellHandle(i));
					else if (center_temp1.z > 4.13)
						dcc.insert(CellHandle(i));
				}
			}
			for (std::set<int>::iterator dc_it = dcc.begin(); dc_it != dcc.end(); dc_it++)
			{
				mesh->delete_cell(CellHandle(*dc_it));
			}
			/*double maxz = -999, minz = 999;
		for (int i = 0; i < mesh->vertices_.size(); ++i)
		{
			V3f v = mesh->vertices_[i];
			if (maxz < v.z)
				maxz = v.z;
			if (minz > v.z)
				minz = v.z;
		}
		double delete_z = (maxz + minz) / 2;
		std::vector<CellHandle> dc;
		for (int i = 0; i < mesh->cells_.size(); ++i)
		{
			if (mesh->cell_center[i].z < delete_z)
				dc.push_back(CellHandle(i));
		}
		for (std::vector<CellHandle>::iterator dc_it = dc.begin();dc_it != dc.end();dc_it++)
		{
			mesh->delete_cell(*dc_it);
		}*/

			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, mesh);
			//mesh = new HexV3fMesh();
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionCCSub_1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionCCSub_1->setStatusTip(("Subdivide The Mesh With Method CC_1"));
	connect(ui.actionCCSub_1, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			mesh->examine_bdy();
			CCSubdivision sub(mesh, newmesh);
			*mesh = sub.CCSubdivising_1();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionConstructModel->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructModel->setStatusTip(("Construct BSplineSolid Model"));
	connect(ui.actionConstructModel, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			//ApproximationCC AppCC1(mesh, newmesh);
			//for (int cell_i = 0; cell_i < mesh->cells_.size(); cell_i++)
			//{
			//	for (int i = 0; i < mesh->cells_[cell_i].vertex_.size(); i++)
			//	{
			//		if (mesh->neighbor_v[mesh->cells_[cell_i].vertex_[i]].size() == 3)
			//		{
			//			V3f temp = AppCC1.solid_corner_point(mesh->cells_[cell_i].vertex_[i]);
			//			glo_cen.push_back(temp);
			//			//glo_cen.push_back(mesh->vertices_[mesh->cells_[cell_i].vertex_[i]]);
			//		}
			//	}
			//}
			mesh->cal_cen();
			//HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			simpmesh.reset(new HexV3fMesh());
			*simpmesh = *mesh;
			AppCC.construct_bss();
			m_solids = AppCC.bss;
			m_solids.Global_BSplineSolid.examine_bdy();
			SolidData = AppCC.SolidData;
			*mesh = m_solids.Global_BSplineSolid;
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			/*vmesh = m_solids.Local_BSplineSolid;
		cal_bdy(vmesh);
		cal_cen(vmesh);
		ui.widget->setvmesh(vmesh);
		ui.widget->set_scene_pos(DATcenter, DATr);
		ui.widget->seted(DATmaxEdgeLen, DATminEdgeLen);
		ui.widget->setbb(DATbbMax, DATbbMin);
		ui.widget->setdatcellcen(DATcellcen);
		ui.widget->setdatbdy(DATbdy, DATedgever);*/
			update();
		});

	ui.actionPreparePDE->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPreparePDE->setStatusTip(("Construct BSplineSolid Model"));
	connect(ui.actionPreparePDE, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, pickedge1);
			AppCC.CCapproximation1();
			SolidData = AppCC.SolidData;
		});

	ui.actionConstructModel_1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructModel_1->setStatusTip(("Construct BSplineSolid Model"));
	connect(ui.actionConstructModel_1, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			simpmesh.reset(new HexV3fMesh());
			*simpmesh = *mesh;
			AppCC.construct_bss1();
			m_solids = AppCC.bss;
			SolidData = AppCC.SolidData;
			*mesh = m_solids.Global_BSplineSolid;
			m_solids.Global_BSplineSolid.examine_bdy();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			/*vmesh = m_solids.Local_BSplineSolid;
		cal_bdy(vmesh);
		cal_cen(vmesh);
		ui.widget->setvmesh(vmesh);
		ui.widget->set_scene_pos(DATcenter, DATr);
		ui.widget->seted(DATmaxEdgeLen, DATminEdgeLen);
		ui.widget->setbb(DATbbMax, DATbbMin);
		ui.widget->setdatcellcen(DATcellcen);
		ui.widget->setdatbdy(DATbdy, DATedgever);*/
			update();
		});

	ui.actionConstructModel_1_new->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructModel_1_new->setStatusTip(("Construct BSplineSolid Model"));
	connect(ui.actionConstructModel_1_new, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh);
			simpmesh.reset(new HexV3fMesh());
			*simpmesh = *mesh;
			AppCC.construct_bss1_new();
			m_solids = AppCC.bss;
		});

	ui.actionConstructModel_new->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionConstructModel_new->setStatusTip(("Construct BSplineSolid Model"));
	connect(ui.actionConstructModel_new, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			/*ApproximationCC AppCC1(mesh, newmesh);
		for (int cell_i = 0; cell_i < mesh->cells_.size(); cell_i++)
		{
			for (int i = 0; i < mesh->cells_[cell_i].vertex_.size(); i++)
			{
				if (mesh->neighbor_v[mesh->cells_[cell_i].vertex_[i]].size() == 3)
				{
					V3f temp = AppCC1.solid_corner_point(mesh->cells_[cell_i].vertex_[i]);
					glo_cen.push_back(temp);
				}
			}
		}*/
			mesh->examine_bdy();
			/*-----�����ĸ���-----*/
			//ApproximationCC AppCC1(mesh, newmesh);
			//for (int cell_i = 0; cell_i < mesh->cells_.size(); cell_i++)
			//{
			//	for (int i = 0; i < mesh->cells_[cell_i].vertex_.size(); i++)
			//	{
			//		if (mesh->vertices_[mesh->cells_[cell_i].vertex_[i]] == V3f(0,0,0) || mesh->vertices_[mesh->cells_[cell_i].vertex_[i]] == V3f(3, 0, 0)
			//			|| mesh->vertices_[mesh->cells_[cell_i].vertex_[i]] == V3f(3, 3, 0)|| mesh->vertices_[mesh->cells_[cell_i].vertex_[i]] == V3f(0, 3, 0))
			//		{
			//			V3f temp = AppCC1.solid_corner_point(mesh->cells_[cell_i].vertex_[i]);
			//			glo_cen.push_back(temp);
			//			//glo_cen_id.push_back(cell_i);
			//		}
			//	}
			//}
			mesh->cal_cen();
			//ui.widget->pickedge_e.swap(std::vector<EdgeHandle>());
			//for (int i = 0; i < mesh->edges_.size(); ++i)
			//{
			//	if (mesh->is_bdy(EdgeHandle(i)))
			//	{
			//		V3f v = mesh->cal_edge_mid(EdgeHandle(i));
			//		/*double min = 999;
			//		for (int i = 0; i < edgemid.size(); ++i)
			//		{
			//			double dis = distance(v, edgemid[i]);
			//			if (dis < min)
			//				min = dis;
			//		}
			//		if (min < 1.0e-5)
			//			ui.widget->pickedge_e.push_back(EdgeHandle(i));*/
			//		if (v.x == 0 && mesh->neighbor_e[i].size() == 2)
			//			pickedge1.push_back(EdgeHandle(i));
			//	}
			//}
			ApproximationCC AppCC(mesh, newmesh, pickedge1);
			simpmesh.reset(new HexV3fMesh());
			*simpmesh = *mesh;
			AppCC.construct_bss_new();
			m_solids = AppCC.bss;
		});

	ui.actionBSplineSolidDAT->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionBSplineSolidDAT->setStatusTip(("Create Bezier Solid"));
	connect(ui.actionBSplineSolidDAT, &QAction::triggered,
		[=]() {
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, SolidData);
			*mesh = AppCC.constructBezierSolid();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionCalPDE->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionCalPDE->setStatusTip(("Construct BSplineSolid Model"));
	connect(ui.actionCalPDE, &QAction::triggered,
		[=]() {
			FunctionBSpline fb(m_solids);
			std::vector<BdyBase> bdybaseid;
			/*-----------------����նȾ�����Ҷ���-----------------*/
			std::vector<std::vector<double>> m_bdycoef;
			m_bdycoef.resize(m_solids.Local_BSplineSolid.size());
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_BSplineSolid[i].vertices_.size(), 0);
			}
			/*V3f m_glo_cen = simpmesh->center;
		int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
		for (int i = 0; i< m_solids.Local_BSplineSolid.size(); i++)
		{
			for (int j = 0; j < m_solids.Local_BSplineSolid[i].vertices_.size(); j++)
			{
				V3f temp_v = m_solids.Local_BSplineSolid[i].vertices_[j];
				double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
					+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = j;
					mindis_cellid = i;
				}
			}
		}
		m_bdycoef[mindis_cellid][mindis_id] = 10;
		bdybaseid.push_back(BdyBase(mindis_id, m_solids.CP_Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));*/

		//for (int cen_i = 0; cen_i < glo_cen.size(); cen_i++)
		//{
		//	V3f m_glo_cen = glo_cen[cen_i];
		//	int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
		//	for (int j = 0; j < m_solids.Local_BSplineSolid[cen_i].vertices_.size(); j++)
		//	{
		//		V3f temp_v = m_solids.Local_BSplineSolid[cen_i].vertices_[j];
		//		double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
		//			+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
		//		if (dis < min_dis)
		//		{
		//			min_dis = dis;
		//			mindis_id = j;
		//			mindis_cellid = cen_i;
		//		}
		//	}
		//	m_bdycoef[mindis_cellid][mindis_id] = 0;
		//	bdybaseid.push_back(BdyBase(mindis_id, m_solids.CP_Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
		//	/*for (int j = 8; j <= 15; j++)
		//	{
		//		m_bdycoef[0][j] = 10;
		//		bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[0][j], 0));
		//	}*/
		//}
		/*for (int j = 8; j <= 15; j++)
	{
		m_bdycoef[0][j] = 10;
		bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[0][j], 0));
	}*/
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					if (m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]))
					{
						bool check = true;
						for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
						{
							if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
							{
								check = false;
								break;
							}
						}
						if (check == true)
							bdybaseid.push_back(BdyBase(v_id, m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
					}
				}
			}
			/*for (int cen_i = 0; cen_i < glo_cen.size(); cen_i++)
		{
			V3f m_glo_cen = glo_cen[cen_i];
			int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				for (int j = 0; j < m_solids.Local_BSplineSolid[i].vertices_.size(); j++)
				{
					V3f temp_v = m_solids.Local_BSplineSolid[i].vertices_[j];
					double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
						+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
					if (dis < min_dis)
					{
						min_dis = dis;
						mindis_id = j;
						mindis_cellid = i;
					}
				}
			}
			m_bdycoef[mindis_cellid][mindis_id] = 0;
			bdybaseid.push_back(BdyBase(mindis_id, m_solids.CP_Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
		}*/

		/*in_out_side = ui.widget->pickpoint_vtoline;
	for (int i = 0; i < in_out_side.size(); i++)
	{
		std::vector<VertexHandle> temp_in_out = in_out_side[i];
		for (int j = 0; j < temp_in_out.size(); j++)
		{
			int global_id = temp_in_out[j];
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				bool ocheck = false;
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					if (m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
						&& m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id] == global_id)
					{
						bool check = true;
						for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
						{
							if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
							{
								check = false;
								break;
							}
						}
						if (check == true)
						{
							if (i <= 4)
								m_bdycoef[Local_BSplineSolid_id][v_id] = 10;
							else
								m_bdycoef[Local_BSplineSolid_id][v_id] = 0;
							bdybaseid.push_back(BdyBase(v_id, m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
							ocheck = true;
							break;
						}
					}
				}
				if (ocheck == true)
					break;
			}
		}
	}*/

			Matrix m_bigmatrix;
			std::vector<double> m_bigrhs;
			fb.Global_MatrixAndRhs(m_bigmatrix, m_bigrhs);
			/*-----------------�����նȾ�����Ҷ���-----------------*/
			//std::vector<BdyBase> bdybaseid;
			//bdybaseid.push_back(BdyBase(m_solids.Control_Point_id[0][1][1][1], m_solids.CP_Local_to_Global[0][m_solids.Control_Point_id[0][1][1][1]], 0));
			//m_solids.Global_BSplineSolid.examine_bdy();
			//for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			//{
			//	for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
			//	{
			//		//if (m_solids.Local_BSplineSolid[Local_BSplineSolid_id].neighbor_v[v_id].size() <= 5)
			//		if (m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]))
			//		{
			//			bool check = true;
			//			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
			//			{
			//				if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
			//				{
			//					check = false;
			//					break;
			//				}
			//			}
			//			if(check == true)
			//				bdybaseid.push_back(BdyBase(v_id,m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
			//		}
			//	}
			//}
			int m_GlobalSize = m_solids.Global_BSplineSolid.vertices_.size();
			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
			{
				int m_baseid = (*bdybaseid_id).local_id;
				int m_solidid = (*bdybaseid_id).solid_id;
				int m_globalid = (*bdybaseid_id).global_id;
				for (int j = 0; j < m_GlobalSize; j++)
				{
					m_bigrhs[j] = m_bigrhs[j] - m_bdycoef[m_solidid][m_baseid] * m_bigmatrix[j][m_globalid];
				}
				for (int j = 0; j < m_GlobalSize; j++)
				{
					m_bigmatrix[j][m_globalid] = 0;
					m_bigmatrix[m_globalid][j] = 0;
				}
				m_bigmatrix[m_globalid][m_globalid] = 1;
				m_bigrhs[m_globalid] = m_bdycoef[m_solidid][m_baseid];
			}
			/*-----------------������ͷ�����-----------------*/
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_bigmatrix, m_bigrhs, m_coef);
			//m_coef[mindis_id] = 10;
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					m_bdycoef[Local_BSplineSolid_id][v_id] = m_coef[m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]];
				}
			}
			/*-----------------����ֵ��-----------------*/
			/*std::vector<std::vector<double>> m_sol_local, m_sol_local1;
		m_sol_local.resize(m_solids.Local_BSplineSolid.size());
		for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
		{
			m_sol_local[i].resize(m_solids.Local_BSplineSolid[i].vertices_.size(), 0);
		}
		m_sol_local1 = m_sol_local;
		std::vector<double> m_sol_global(m_coef.size(),0);
		for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
		{
			int m_uptnum = m_solids.Local_Node[i][0].size() - m_solids.Local_Order[i][0] - 1;
			int m_vptnum = m_solids.Local_Node[i][1].size() - m_solids.Local_Order[i][1] - 1;
			int m_wptnum = m_solids.Local_Node[i][2].size() - m_solids.Local_Order[i][2] - 1;
			for (int u = 0; u < m_uptnum; u++)
			{
				for (int v = 0; v < m_vptnum; v++)
				{
					for (int w = 0; w < m_wptnum; w++)
					{
						double uu = u * 1.0 / (m_uptnum - 1);
						double vv = v * 1.0 / (m_vptnum - 1);
						double ww = w * 1.0 / (m_wptnum - 1);
						double m_uder[2], m_vder[2], m_wder[2];
						double m_approxsol = 0;
						V3f vertex = m_solids.Geometry(i, uu, vv, ww);
						double exat = m_solids.ExactSol(vertex.x, vertex.y, vertex.z);
						m_sol_local1[i][m_solids.Control_Point_id[i][w][v][u]] = exat;
						for (int ii = 0; ii < m_uptnum; ii++)
						{
							m_solids.DersOneBasisFun(i,0, ii, uu, 1, m_uder);
							for (int jj = 0; jj < m_vptnum; jj++)
							{
								m_solids.DersOneBasisFun(i, 1, jj, vv, 1, m_vder);
								for (int kk = 0; kk < m_wptnum; kk++)
								{
									m_solids.DersOneBasisFun(i, 2, kk, ww, 1, m_wder);
									m_approxsol += m_bdycoef[i][m_solids.Control_Point_id[i][kk][jj][ii]] * m_uder[0] * m_vder[0]* m_wder[0];
								}
							}
						}
						m_sol_local[i][m_solids.Control_Point_id[i][w][v][u]] = m_approxsol;
					}
				}
			}
		}
		for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
		{
			for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
			{
				m_sol_global[m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]] = m_sol_local[Local_BSplineSolid_id][v_id];
			}
		}*/
			std::vector<double> m_color;
			if (times == 0)
			{
				*mesh = constructBezierSolid(m_bdycoef, m_color, 1);
				m_color1 = m_color;
				mesh_1 = new HexV3fMesh();
				*mesh_1 = *mesh;
				times++;
			}
			else
			{
				*mesh = constructBezierSolid(m_bdycoef, m_color, 16);
				m_color2 = m_color;
				mesh_2 = new HexV3fMesh();
				*mesh_2 = *mesh;
			}
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionActionCalPDE1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionActionCalPDE1->setStatusTip(("Construct BSplineSolid Model"));
	connect(ui.actionActionCalPDE1, &QAction::triggered,
		[=]() {
			Sort stm(mesh_1, mesh_2, m_color1, m_color2);
			//Sort stm(&mesh1_, &mesh2_, m_color1, m_color2);
			stm.sorting();
			stm.comparing1();
			std::cout << std::endl
				<< "max: " << stm.max << " min: " << stm.min << " average: "
				<< stm.averr << " releate average: " << stm.relerr << std::endl;
			mesh_1->examine_bdy();
			mesh_1->cal_cen();
			mesh_1->examine_bdfv();
			mesh_1->cal_cell_cen();
			mesh_1->cal_bdnorm();
			mesh_1->cal_mami_ed();
			ui.widget->setmesh(mesh_1);
			ui.widget->setbffv(mesh_1->boundary_face_v, mesh_1->normal_boundary_face_v, mesh_1->boundary_face, mesh_1->boundary_face_VID);
			ui.widget->set_scene_pos(mesh_1->center, mesh_1->r);
			ui.widget->setcell_center(mesh_1->cell_center);
			ui.widget->seted(mesh_1->maxEdgeLen, mesh_1->minEdgeLen);
			ui.widget->setbb(mesh_1->bbMax, mesh_1->bbMin);
			/*mesh1_.examine_bdy();
		mesh1_.cal_cen();
		mesh1_.examine_bdfv();
		mesh1_.cal_cell_cen();
		mesh1_.cal_bdnorm();
		mesh1_.cal_mami_ed();
		ui.widget->setmesh(&mesh1_);
		ui.widget->setbffv(mesh1_.boundary_face_v, mesh1_.normal_boundary_face_v, mesh1_.boundary_face, mesh1_.boundary_face_VID);
		ui.widget->set_scene_pos(mesh1_.center, mesh1_.r);
		ui.widget->setcell_center(mesh1_.cell_center);
		ui.widget->seted(mesh1_.maxEdgeLen, mesh1_.minEdgeLen);
		ui.widget->setbb(mesh1_.bbMax, mesh1_.bbMin);*/
			ui.widget->set_color(stm.colorverr);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionExactSol->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionExactSol->setStatusTip(("Exact Solution"));
	connect(ui.actionExactSol, &QAction::triggered,
		[=]() {
			std::vector<double> m_coef;
			int num = m_solids.Global_BSplineSolid.vertices_.size();
			m_coef.resize(num, 0);
			for (int i = 0; i < num; i++)
			{
				//if (!m_solids.Global_BSplineSolid.is_bdy(VertexHandle(i)))
				//{
				double x = m_solids.Global_BSplineSolid.vertices_[i].x;
				double y = m_solids.Global_BSplineSolid.vertices_[i].y;
				double z = m_solids.Global_BSplineSolid.vertices_[i].z;
				m_coef[i] = x * (1 - x) * y * (1 - y) * (1 + z) * (1 - z);
				//}
			}
			ui.widget->set_color(m_coef);
			ui.widget->setdraw_mode_(Error);
		});

	ui.actionERRORV->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionERRORV->setStatusTip(("ERRORV"));
	connect(ui.actionERRORV, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ERRORV);
		});

	ui.actionTESTDraw->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionTESTDraw->setStatusTip(("TEST"));
	connect(ui.actionTESTDraw, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(TEST);
		});

	ui.actionSIMP->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSIMP->setStatusTip(("SIMP"));
	connect(ui.actionSIMP, &QAction::triggered,
		[=]() {
			/*--------------------��ȡ�Ĳ��Ϸֲ�--------------------*/
			material_distribution.resize(m_solids.Local_BSplineSolid.size(), 1); //��ʼ��ȫ������Ϊ1
			FunctionBSpline fb(m_solids, material_distribution);
			/*-----------------����նȾ�����Ҷ���-----------------*/
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			m_bdycoef.resize(m_solids.Local_BSplineSolid.size());
			m_solids.Global_BSplineSolid.examine_bdy();
			//bool checkbdy = false;
			//for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			//{
			//	m_bdycoef[i].resize(m_solids.Local_BSplineSolid[i].vertices_.size(), 0);
			//	if (simpmesh->is_bdy(CellHandle(i)))
			//	{
			//		if (checkbdy == false)
			//		{
			//			int local_facenum = m_solids.Local_BSplineSolid[i].faces_.size();
			//			for (int j = 0; j < local_facenum; j++)
			//			{
			//				int bdy_fnum = 0;
			//				bool checkface = false;
			//				std::vector<VertexHandle> local_f_v = m_solids.Local_BSplineSolid[i].faces_[j].vs_f;
			//				for (std::vector<VertexHandle>::iterator local_f_v_it = local_f_v.begin(); local_f_v_it != local_f_v.end();
			//					local_f_v_it++)
			//				{
			//					if (m_solids.Global_BSplineSolid.is_bdy(VertexHandle(m_solids.CP_Local_to_Global[i][*local_f_v_it])))
			//					{
			//						bdybaseid.push_back(BdyBase(*local_f_v_it, m_solids.CP_Local_to_Global[i][*local_f_v_it], i));
			//						//m_bdycoef[i][*local_f_v_it] = 1;
			//						checkface = true;
			//					}
			//					else
			//						break;
			//				}
			//				if (checkface == true)
			//				{
			//					checkbdy = true;
			//					break;
			//				}
			//			}
			//		}
			//	}
			//}
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_BSplineSolid[i].vertices_.size(), 0);
				//m_bdycoef[i][m_solids.Control_Point_id[i][1][1][1]] = 1;
			}
			/*V3f m_glo_cen = V3f(0.5, 0.5, 1);
		int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
		for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
		{
			for (int j = 0; j < m_solids.Local_BSplineSolid[i].vertices_.size(); j++)
			{
				V3f temp_v = m_solids.Local_BSplineSolid[i].vertices_[j];
				if (temp_v.z == 1)
				{
					if (temp_v.x >= 0.25 && temp_v.x <= 0.75)
					{
						if (temp_v.y >= 0.25 && temp_v.y <= 0.75)
							bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[i][j],i));
					}
				}
			}
		}*/
			std::vector<int> dis_id;
			V3f m_glo_cen = V3f(0.5, 0.5, 0.5);
			int mindis_id = 0;
			double min_dis = 999;
			int mindis_cellid = 0;
			for (int i = 0; i < m_solids.Global_BSplineSolid.vertices_.size(); i++) //Ѱ�����ĵ�
			{
				V3f temp_v = m_solids.Global_BSplineSolid.vertices_[i];
				/*if (temp_v.z == 1)
			{
				if (temp_v.x >=  0.25 && temp_v.x <= 0.75)
				{
					if (temp_v.y >= 0.25 && temp_v.y <= 0.75)
						dis_id.push_back(i);
				}
			}*/
				double dis = sqrt((temp_v.x - m_glo_cen.x) * (temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y) * (temp_v.y - m_glo_cen.y) + (temp_v.z - m_glo_cen.z) * (temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = i;
				}
			}
			Matrix m_bigmatrix;
			std::vector<double> m_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> local_bigrhs;
			fb.Global_MatrixAndRhs(m_bigmatrix, m_bigrhs, local_matrix, local_bigrhs);
			/*-----------------�����նȾ�����Ҷ���-----------------*/
			//std::vector<BdyBase> bdybaseid;
			//bdybaseid.push_back(BdyBase(m_solids.Control_Point_id[0][1][1][1], m_solids.CP_Local_to_Global[0][m_solids.Control_Point_id[0][1][1][1]], 0));
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					if (m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]))
					{
						bool check = true;
						for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
						{
							if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
							{
								check = false;
								break;
							}
						}
						if (check == true)
							bdybaseid.push_back(BdyBase(v_id, m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
					}
				}
			}
			int m_GlobalSize = m_solids.Global_BSplineSolid.vertices_.size();
			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
			{
				int m_baseid = (*bdybaseid_id).local_id;
				int m_solidid = (*bdybaseid_id).solid_id;
				int m_globalid = (*bdybaseid_id).global_id;
				for (int j = 0; j < m_GlobalSize; j++)
				{
					m_bigrhs[j] = m_bigrhs[j] - m_bdycoef[m_solidid][m_baseid] * m_bigmatrix[j][m_globalid];
				}
				for (int j = 0; j < m_GlobalSize; j++)
				{
					m_bigmatrix[j][m_globalid] = 0;
					m_bigmatrix[m_globalid][j] = 0;
				}
				m_bigmatrix[m_globalid][m_globalid] = 1;
				m_bigrhs[m_globalid] = m_bdycoef[m_solidid][m_baseid];
			}
			/*-----------------������ͷ�����-----------------*/
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_bigmatrix, m_bigrhs, m_coef);
			/*for (int i = 0; i < dis_id.size(); i++)
		{
			m_coef[dis_id[i]] = 0;
		}*/
			m_coef[mindis_id] = 10;
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					m_bdycoef[Local_BSplineSolid_id][v_id] = m_coef[m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]];
				}
			}
			/*-----------------�����ȷ���-----------------*/
			std::vector<simpsort> sensitivity;
			sensitivity.resize(m_solids.Local_BSplineSolid.size());
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				double temp = pow(material_distribution[i], fb.p - 1);
				temp = fb.uTKu(m_bdycoef[i], local_matrix[i]);
				double temp_senitiv = fb.p / 2 * pow(material_distribution[i], fb.p - 1) * fb.uTKu(m_bdycoef[i], local_matrix[i]);
				//temp_senitiv = temp_senitiv * 100000000;
				sensitivity[i].senitiv = temp_senitiv;
				//sensitivity[i].senitiv = 2 * fb.PTu(local_bigrhs[i], m_bdycoef[i]) - fb.uTKu(m_bdycoef[i], local_matrix[i]);
				sensitivity[i].cell_id = i;
				qDebug() << sensitivity[i].senitiv;
			}
			sensitivity1 = sensitivity;
			simsor();
			int delete_num = 0.5 * m_solids.Local_BSplineSolid.size();
			/*int cast = delete_num % 6;
		if (cast != 0)
		{
			delete_num = delete_num - cast;
		}*/
			for (int i = 0; i < delete_num; i++)
			{
				simpmesh->delete_cell(CellHandle(sensitivity1[i].cell_id));
			}
			/*-----------------����ֵ��-----------------*/
			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, simpmesh.get());
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			std::vector<double> m_color;
			//*mesh = constructBezierSolid(m_bdycoef, m_color);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			//ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionSIMP1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSIMP1->setStatusTip(("SIMP1"));
	connect(ui.actionSIMP1, &QAction::triggered,
		[=]() {
			m_solids.Global_BSplineSolid.examine_bdy(); //��ʼ���߽���Ϣ
			m_solids.Global_BSplineSolid.cal_cen();
			/*--------------------��ȡ�Ĳ��Ϸֲ�--------------------*/
			material_distribution.resize(m_solids.Local_BSplineSolid.size(), 1); //��ʼ��ȫ������Ϊ1
			FunctionBSpline fb(m_solids, material_distribution);
			/*-----------------����նȾ�����Ҷ���-----------------*/
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			std::map<int, int> base_check; //���ÿ�������������������±�
			m_bdycoef.resize(m_solids.Local_BSplineSolid.size());
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_BSplineSolid[i].vertices_.size(), 0);
			}
			V3f m_glo_cen = V3f(0.5, 0.5, 0.5);
			int mindis_id = 0;
			double min_dis = 999;
			int mindis_cellid = 0;
			std::vector<int> dis_id;
			/*for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
		{
			for (int j = 0; j < m_solids.Local_BSplineSolid[i].vertices_.size(); j++)
			{
				V3f temp_v = m_solids.Local_BSplineSolid[i].vertices_[j];
				double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
					+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = j;
					mindis_cellid = i;
				}
			}
		}*/
		//for (int i = 0; i < m_solids.Global_BSplineSolid.vertices_.size(); i++)//Ѱ�����ĵ�
		//{
		//	V3f temp_v = m_solids.Global_BSplineSolid.vertices_[i];
		//	double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
		//		+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
		//	if (dis < min_dis)
		//	{
		//		min_dis = dis;
		//		mindis_id = i;
		//	}
		//}
			Matrix m_bigmatrix;
			std::vector<double> m_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> local_bigrhs;
			fb.Global_MatrixAndRhs1(m_bigmatrix, m_bigrhs, local_matrix, local_bigrhs, base_check);
			/*-----------------�����նȾ�����Ҷ���-----------------*/
			/*int m_GlobalSize = base_check.size();
		int local_mindis_id = base_check[mindis_id];
		for (int j = 0; j < m_GlobalSize; j++)
		{
			m_bigrhs[j] = m_bigrhs[j] - 10 * m_bigmatrix[j][local_mindis_id];
		}
		for (int j = 0; j < m_GlobalSize; j++)
		{
			m_bigmatrix[j][local_mindis_id] = 0;
			m_bigmatrix[local_mindis_id][j] = 0;
		}
		m_bigmatrix[local_mindis_id][local_mindis_id] = 1;
		m_bigrhs[local_mindis_id] = 10;*/
		/*-----------------������ͷ�����-----------------*/
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_bigmatrix, m_bigrhs, m_coef);
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					int temp = m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id];
					if (base_check.find(temp) != base_check.end())
						m_bdycoef[Local_BSplineSolid_id][v_id] = m_coef[base_check[temp]];
					else
						m_bdycoef[Local_BSplineSolid_id][v_id] = 0;
				}
			}
			/*-----------------�����ȷ���-----------------*/
			std::vector<simpsort> sensitivity;
			sensitivity.resize(m_solids.Local_BSplineSolid.size());
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				//double temp = pow(material_distribution[i], fb.p - 1);
				//temp = fb.uTKu(m_bdycoef[i], local_matrix[i]);
				double temp_senitiv = fb.p / 2 * pow(material_distribution[i], fb.p - 1) * fb.uTKu(m_bdycoef[i], local_matrix[i]);
				//temp_senitiv = temp_senitiv * 100000000;
				sensitivity[i].senitiv = temp_senitiv;
				//sensitivity[i].senitiv = 2 * fb.PTu(local_bigrhs[i], m_bdycoef[i]) - fb.uTKu(m_bdycoef[i], local_matrix[i]);
				sensitivity[i].cell_id = i;
				qDebug() << sensitivity[i].senitiv;
			}
			sensitivity1 = sensitivity;
			simsor();
			int delete_num = 0.5 * m_solids.Local_BSplineSolid.size();
			for (int i = 0; i < delete_num; i++)
			{
				simpmesh->delete_cell(CellHandle(sensitivity1[i].cell_id));
			}
			/*-----------------����ֵ��-----------------*/
			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, simpmesh.get());
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			std::vector<double> m_color;
			//*mesh = constructBezierSolid(m_bdycoef, m_color);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			update();
		});

	ui.actionNewSIMP->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionNewSIMP->setStatusTip(("New SIMP"));
	connect(ui.actionNewSIMP, &QAction::triggered,
		[=]() {
			m_solids.Global_BSplineSolid.examine_bdy(); //��ʼ���߽���Ϣ
			m_solids.Global_BSplineSolid.cal_cen();
			/*--------------------��ȡ�Ĳ��Ϸֲ�--------------------*/
			material_distribution_check = material_distribution;
			FunctionBSpline fb(m_solids, material_distribution);
			/*-----------------����նȾ�����Ҷ���-----------------*/
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			std::map<int, int> base_check; //���ÿ�������������������±�
			m_bdycoef.resize(m_solids.Local_BSplineSolid.size());
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_BSplineSolid[i].vertices_.size(), 0);
			}
			V3f m_glo_cen = V3f(0.5, 0.5, 0.5);
			int mindis_id = 0;
			double min_dis = 999;
			int mindis_cellid = 0;
			std::vector<int> dis_id;
			for (int i = 0; i < m_solids.Global_BSplineSolid.vertices_.size(); i++) //Ѱ�����ĵ�
			{
				V3f temp_v = m_solids.Global_BSplineSolid.vertices_[i];
				double dis = sqrt((temp_v.x - m_glo_cen.x) * (temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y) * (temp_v.y - m_glo_cen.y) + (temp_v.z - m_glo_cen.z) * (temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = i;
				}
			}
			Matrix m_bigmatrix;
			std::vector<double> m_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> local_bigrhs;
			fb.Global_MatrixAndRhs1(m_bigmatrix, m_bigrhs, local_matrix, local_bigrhs, base_check);
			/*-----------------�����նȾ�����Ҷ���-----------------*/
			int m_GlobalSize = base_check.size();
			int local_mindis_id = base_check[mindis_id];
			for (int j = 0; j < m_GlobalSize; j++)
			{
				m_bigrhs[j] = m_bigrhs[j] - 10 * m_bigmatrix[j][local_mindis_id];
			}
			for (int j = 0; j < m_GlobalSize; j++)
			{
				m_bigmatrix[j][local_mindis_id] = 0;
				m_bigmatrix[local_mindis_id][j] = 0;
			}
			m_bigmatrix[local_mindis_id][local_mindis_id] = 1;
			m_bigrhs[local_mindis_id] = 10;
			/*-----------------������ͷ�����-----------------*/
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_bigmatrix, m_bigrhs, m_coef);
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					int temp = m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id];
					if (base_check.find(temp) != base_check.end())
						m_bdycoef[Local_BSplineSolid_id][v_id] = m_coef[base_check[temp]];
					else
						m_bdycoef[Local_BSplineSolid_id][v_id] = 0;
				}
			}
			/*-----------------�����ȷ���-----------------*/
			std::vector<simpsort> sensitivity;
			sensitivity.resize(m_solids.Local_BSplineSolid.size());
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				double temp_senitiv = fb.p / 2 * pow(material_distribution[i], fb.p - 1) * fb.uTKu(m_bdycoef[i], local_matrix[i]);
				sensitivity[i].senitiv = temp_senitiv;
				;
				sensitivity[i].cell_id = i;
				qDebug() << sensitivity[i].senitiv;
			}
			sensitivity1 = sensitivity;
			simsor();
			int delete_num = (0.16 * (simp_time++)) * m_solids.Local_BSplineSolid.size();
			for (int i = 0; i < delete_num; i++)
			{
				material_distribution[sensitivity1[i].cell_id] = 0.0001;
				//simpmesh->delete_cell(CellHandle(sensitivity1[i].cell_id));
			}
			/*-----------------����ֵ��-----------------*/
			/*FileHandler filenew = FileHandler();
		std::string name("E://delete.mesh");
		filenew.writeFile1(name, simpmesh);
		mesh = new HexV3fMesh();
		FileHandler file("E://delete.mesh");
		file.read2HexMeshObj(mesh, true, true, true);*/
			std::vector<double> m_color;
			//*mesh = constructBezierSolid(m_bdycoef, m_color);
			*mesh = *simpmesh;
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			update();
		});

	ui.actionDelete->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionDelete->setStatusTip(("Delete SIMP"));
	connect(ui.actionDelete, &QAction::triggered,
		[=]() {
			int delete_num = m_solids.Local_BSplineSolid.size();
			int j = 0;
			for (int i = 0; i < delete_num; i++)
			{
				qDebug() << material_distribution[i];
				if (material_distribution[i] != 1 /*&& material_distribution_check[i] != 1*/)
				{
					j++;
					simpmesh->delete_cell(CellHandle(i));
				}
			}
			qDebug() << delete_num;
			qDebug() << j;
			/*-----------------����ֵ��-----------------*/
			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, simpmesh.get());
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			std::vector<double> m_color;
			//*mesh = constructBezierSolid(m_bdycoef, m_color);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			update();
		});

	ui.actionSIMP_new->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSIMP_new->setStatusTip(("SIMP new"));
	connect(ui.actionSIMP_new, &QAction::triggered,
		[=]() {
			/*--------------------��ȡ�Ĳ��Ϸֲ�--------------------*/
			material_distribution.resize(m_solids.Local_V3f.size(), 1); //��ʼ��ȫ������Ϊ1
			FunctionBSpline fb(m_solids, material_distribution);
			/*-----------------����նȾ�����Ҷ���-----------------*/
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			std::map<int, int> base_check; //���ÿ�������������������±�
			m_bdycoef.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_V3f[i].size(), 0);
			}
			V3f m_glo_cen = V3f(0.5, 0.5, 0.5);
			int mindis_id = 0;
			double min_dis = 999;
			int mindis_cellid = 0;
			/*for (int i = 0; i < m_solids.Local_V3f.size(); i++)
		{
			for (int j = 0; j < m_solids.Local_V3f[i].size(); j++)
			{
				V3f temp_v = m_solids.Local_V3f[i][j];
				double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
					+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = j;
					mindis_cellid = i;
				}
			}
		}
		m_bdycoef[mindis_cellid][mindis_id] = 10;
		bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));*/

			in_out_side = ui.widget->pickpoint_vtoline;
			for (int i = 0; i < in_out_side.size(); i++)
			{
				std::vector<VertexHandle> temp_in_out = in_out_side[i];
				for (int j = 0; j < temp_in_out.size(); j++)
				{
					int global_id = temp_in_out[j];
					for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
					{
						bool ocheck = false;
						for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
						{
							if (m_solids.local_bdy[Local_V3f_id][v_id] && m_solids.Local_to_Global[Local_V3f_id][v_id] == global_id)
							{
								bool check = true;
								for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
								{
									if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
									{
										check = false;
										break;
									}
								}
								if (check == true)
								{
									if (i <= 4)
										m_bdycoef[Local_V3f_id][v_id] = 10;
									else
										m_bdycoef[Local_V3f_id][v_id] = 0;
									bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
									ocheck = true;
									break;
								}
							}
						}
						if (ocheck == true)
							break;
					}
				}
			}
			Matrix m_bigmatrix;
			std::vector<double> m_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> local_bigrhs;
			fb.Global_MatrixAndRhs_new(m_bigmatrix, m_bigrhs, local_matrix);
			/*-----------------�����նȾ�����Ҷ���-----------------*/
			/*int m_GlobalSize = base_check.size();
		int local_mindis_id = base_check[m_solids.Local_to_Global[mindis_cellid][mindis_id]];
		for (int j = 0; j < m_GlobalSize; j++)
		{
			m_bigrhs[j] = m_bigrhs[j] - 10 * m_bigmatrix[j][local_mindis_id];
		}
		for (int j = 0; j < m_GlobalSize; j++)
		{
			m_bigmatrix[j][local_mindis_id] = 0;
			m_bigmatrix[local_mindis_id][j] = 0;
		}
		m_bigmatrix[local_mindis_id][local_mindis_id] = 1;
		m_bigrhs[local_mindis_id] = 10;*/
		/*for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
	{
		for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
		{
			if (m_solids.local_bdy[Local_V3f_id][v_id])
			{
				bool check = true;
				for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
				{
					if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
					{
						check = false;
						break;
					}
				}
				if (check == true)
					bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
			}
		}
	}*/

			int m_GlobalSize = m_solids.Global_size;
			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
			{
				int m_baseid = (*bdybaseid_id).local_id;
				int m_solidid = (*bdybaseid_id).solid_id;
				int m_globalid = (*bdybaseid_id).global_id;
				for (int j = 0; j < m_GlobalSize; j++)
				{
					m_bigrhs[j] = m_bigrhs[j] - m_bdycoef[m_solidid][m_baseid] * m_bigmatrix[j][m_globalid];
				}
				for (int j = 0; j < m_GlobalSize; j++)
				{
					m_bigmatrix[j][m_globalid] = 0;
					m_bigmatrix[m_globalid][j] = 0;
				}
				m_bigmatrix[m_globalid][m_globalid] = 1;
				m_bigrhs[m_globalid] = m_bdycoef[m_solidid][m_baseid];
			}
			/*-----------------������ͷ�����-----------------*/
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_bigmatrix, m_bigrhs, m_coef);
			for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
				{
					/*int temp = m_solids.Local_to_Global[Local_V3f_id][v_id];
				if (base_check.find(temp) != base_check.end())
					m_bdycoef[Local_V3f_id][v_id] = m_coef[base_check[temp]];
				else
					m_bdycoef[Local_V3f_id][v_id] = 0;*/
					m_bdycoef[Local_V3f_id][v_id] = m_coef[m_solids.Local_to_Global[Local_V3f_id][v_id]];
				}
			}
			/*-----------------�����ȷ���-----------------*/
			std::vector<simpsort> sensitivity;
			sensitivity.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				double temp_senitiv = fb.p / 2 * pow(material_distribution[i], fb.p - 1) * fb.uTKu(m_bdycoef[i], local_matrix[i]);
				sensitivity[i].senitiv = temp_senitiv;
				sensitivity[i].cell_id = i;
				qDebug() << sensitivity[i].senitiv;
			}
			sensitivity1 = sensitivity;
			simsor();
			int delete_num = 0.5 * m_solids.Local_V3f.size();
			for (int i = 0; i < delete_num; i++)
			{
				simpmesh->delete_cell(CellHandle(sensitivity1[i].cell_id));
			}
			/*-----------------����ֵ��-----------------*/
			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, simpmesh.get());
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			std::vector<double> m_color;
			//*mesh = constructBezierSolid(m_bdycoef, m_color);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			update();
		});

	ui.actionNewSIMP_new->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionNewSIMP_new->setStatusTip(("NewSIMP new"));
	connect(ui.actionNewSIMP_new, &QAction::triggered,
		[=]() {
			int iteration_time = 5;
			material_distribution.resize(mesh->cells_.size(), 1);
			std::vector<double> m_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> local_bigrhs;
			std::vector<double> sensitivity_pre(m_solids.Local_V3f.size(), 0);
			for (int iteration_num = 0; iteration_num < iteration_time; iteration_num++)
			{
				/*--------------------��ȡ�Ĳ��Ϸֲ�--------------------*/
				FunctionBSpline fb(m_solids, material_distribution);
				/*-----------------����նȾ�����Ҷ���-----------------*/
				std::vector<std::vector<double>> m_bdycoef;
				std::vector<BdyBase> bdybaseid;
				std::map<int, int> base_check; //���ÿ�������������������±�
				m_bdycoef.resize(m_solids.Local_V3f.size());
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					m_bdycoef[i].resize(m_solids.Local_V3f[i].size(), 0);
				}
				V3f m_glo_cen = V3f(0.5, 0.5, 0.5);
				int mindis_id = 0;
				double min_dis = 999;
				int mindis_cellid = 0;
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					for (int j = 0; j < m_solids.Local_V3f[i].size(); j++)
					{
						V3f temp_v = m_solids.Local_V3f[i][j];
						double dis = sqrt((temp_v.x - m_glo_cen.x) * (temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y) * (temp_v.y - m_glo_cen.y) + (temp_v.z - m_glo_cen.z) * (temp_v.z - m_glo_cen.z));
						if (dis < min_dis)
						{
							min_dis = dis;
							mindis_id = j;
							mindis_cellid = i;
						}
					}
				}
				m_bdycoef[mindis_cellid][mindis_id] = 10;
				bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
				Matrix m_bigmatrix;
				/*std::vector<double> m_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector< std::vector<double>> local_bigrhs;*/
				if (iteration_num == 0)
					fb.Global_MatrixAndRhs_new(m_bigmatrix, m_bigrhs, local_matrix);
				else
					fb.Global_MatrixAndRhs_new(m_bigmatrix, local_matrix);
				/*-----------------�����նȾ�����Ҷ���-----------------*/
				for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
				{
					for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
					{
						if (m_solids.local_bdy[Local_V3f_id][v_id])
						{
							bool check = true;
							for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
							{
								if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
								{
									check = false;
									break;
								}
							}
							if (check == true)
								bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
						}
					}
				}
				int m_GlobalSize = m_solids.Global_size;
				for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
				{
					int m_baseid = (*bdybaseid_id).local_id;
					int m_solidid = (*bdybaseid_id).solid_id;
					int m_globalid = (*bdybaseid_id).global_id;
					for (int j = 0; j < m_GlobalSize; j++)
					{
						m_bigrhs[j] = m_bigrhs[j] - m_bdycoef[m_solidid][m_baseid] * m_bigmatrix[j][m_globalid];
					}
					for (int j = 0; j < m_GlobalSize; j++)
					{
						m_bigmatrix[j][m_globalid] = 0;
						m_bigmatrix[m_globalid][j] = 0;
					}
					m_bigmatrix[m_globalid][m_globalid] = 1;
					m_bigrhs[m_globalid] = m_bdycoef[m_solidid][m_baseid];
				}
				/*-----------------������ͷ�����-----------------*/
				std::vector<double> m_coef;
				fb.SolveLinearEquation(-2, m_bigmatrix, m_bigrhs, m_coef);
				for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
				{
					for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
					{
						m_bdycoef[Local_V3f_id][v_id] = m_coef[m_solids.Local_to_Global[Local_V3f_id][v_id]];
					}
				}
				/*-----------------�����ȷ���-----------------*/
				std::vector<simpsort> sensitivity(m_solids.Local_V3f.size());
				std::vector<double> temp_sen(m_solids.Local_V3f.size());
				for (int i = 0; i < m_solids.Local_V3f.size(); i++) //�ȼ����ÿ�����������
				{
					temp_sen[i] = fb.p / 2 * pow(material_distribution[i], fb.p - 1) * fb.uTKu(m_bdycoef[i], local_matrix[i]);
				}
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					std::set<CellHandle> c_c = mesh->neighbor_c_c(CellHandle(i));
					double temp_senitiv = temp_sen[i];
					for (std::set<CellHandle>::iterator c_c_it = c_c.begin(); c_c_it != c_c.end(); c_c_it++)
					{
						temp_senitiv += temp_sen[*c_c_it];
					}
					if (iteration_num == 0) //��һ�εĻ������Լ�
						sensitivity[i].senitiv = temp_senitiv;
					else
						sensitivity[i].senitiv = (temp_senitiv + sensitivity_pre[i]) / 2;
					sensitivity_pre[i] = sensitivity[i].senitiv;
					sensitivity[i].cell_id = i;
				}
				sensitivity1 = sensitivity;
				simsor();
				//int delete_num = (1 - pow(0.5, 1.0 / iteration_time))*m_solids.Local_V3f.size()*(iteration_num + 1);
				int delete_num = (0.16 / iteration_time) * (iteration_num + 1) * m_solids.Local_V3f.size();
				for (int i = 0; i < material_distribution.size(); i++)
				{
					material_distribution[i] = 1;
				}
				for (int i = 0; i < delete_num; i++)
				{
					material_distribution[sensitivity1[i].cell_id] = 0.0001;
				}
			}
		});

	ui.actionPick_Point->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPick_Point->setStatusTip(("Pick Point"));
	connect(ui.actionPick_Point, &QAction::triggered,
		[=]() {
			mesh->v_cs.swap(std::vector<bool>(mesh->vertices_.size(), false));
			ui.widget->setmouse_event(PICK_POINT);
			ui.widget->setdraw_mode_(SOLIDFLAT_AND_PICKPOINT);
		});

	ui.actionPick_Point_2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPick_Point_2->setStatusTip(("Pick Point"));
	connect(ui.actionPick_Point_2, &QAction::triggered,
		[=]() {
			quad->v_cs.swap(std::vector<bool>(quad->vertices_.size(), false));
			ui.widget->setmouse_event(PICK_POINT1);
			ui.widget->setdraw_mode_(QUADPICKV);
		});

	ui.actionPick_Edge->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPick_Edge->setStatusTip(("Pick Edge"));
	connect(ui.actionPick_Edge, &QAction::triggered,
		[=]() {
			ui.widget->setmouse_event(PICK_EDGE);
			ui.widget->setdraw_mode_(SOLIDFLAT_AND_PICKEDGE);
		});

	ui.actionPick_Face->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPick_Face->setStatusTip(("Pick Face"));
	connect(ui.actionPick_Face, &QAction::triggered,
		[=]() {
			ui.widget->setmouse_event(PICK_FACE);
			ui.widget->setdraw_mode_(SOLIDFLAT_AND_PICKEDFACE);
		});

	ui.actionDelete_Face->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionDelete_Face->setStatusTip(("Delete Picked Face"));
	connect(ui.actionDelete_Face, &QAction::triggered,
		[=]() {
			ui.widget->setmouse_event(DELETE_FACE);
			ui.widget->setdraw_mode_(SOLIDFLAT_AND_PICKEDFACE);
		});

	ui.actionPick_Cell->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPick_Cell->setStatusTip(("Pick Cell"));
	connect(ui.actionPick_Cell, &QAction::triggered,
		[=]() {
			ui.widget->setmouse_event(PICK_CELL);
			ui.widget->setdraw_mode_(SOLIDFLAT_AND_PICKEDCELL);
		});

	ui.actionDelete_Cell_2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionDelete_Cell_2->setStatusTip(("Delete Picked Cell"));
	connect(ui.actionDelete_Cell_2, &QAction::triggered,
		[=]() {
			ui.widget->setmouse_event(DELETE_CELL);
			ui.widget->setdraw_mode_(SOLIDFLAT_AND_PICKEDCELL);
		});

	ui.actionDrawBeisur->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionDrawBeisur->setStatusTip(("Draw Bei Sur"));
	connect(ui.actionDrawBeisur, &QAction::triggered,
		[=]() {
			ui.widget->setmouse_event(DRAWBEISUR);
			//ui.widget->setdraw_mode_(BEISUDAT);
		});

	ui.actionDeleteBeisur->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionDeleteBeisur->setStatusTip(("Delete Bei Sur"));
	connect(ui.actionDeleteBeisur, &QAction::triggered,
		[=]() {
			ui.widget->setmouse_event(DRAWBEISUR1);
			//ui.widget->setdraw_mode_(BEISUDAT);
		});

	ui.actionOutputDraw->setIcon(QIcon("Images/Save.png"));
	ui.actionOutputDraw->setStatusTip(("Output Picked Draw"));
	connect(ui.actionOutputDraw, &QAction::triggered,
		[=]() {
			pickdrawface = ui.widget->pickdrawface;
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "PRF(*.prf)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				filenew.writepickdrawface(name, pickdrawface);
			}
		});

	ui.actionReadDraw->setIcon(QIcon("Images/Open.png"));
	ui.actionReadDraw->setStatusTip(("Read Draw Face"));
	connect(ui.actionReadDraw, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "PRF(*.prf)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readpickdrawface(pickdrawface);
			qDebug() << "pickdrawface size" << pickdrawface.size();
			ui.widget->updatedrawface(pickdrawface);
			update();
		});

	ui.actionOpDraw->setIcon(QIcon("Images/Open.png"));
	ui.actionOpDraw->setStatusTip(("Read Draw Face"));
	connect(ui.actionOpDraw, &QAction::triggered,
		[=]() {
			ui.widget->opupdatedrawface();
			update();
		});

	ui.actionOutput_PickPoint->setIcon(QIcon("Images/Save.png"));
	ui.actionOutput_PickPoint->setStatusTip(("Output Picked Point"));
	connect(ui.actionOutput_PickPoint, &QAction::triggered,
		[=]() {
			//in_out_side = ui.widget->pickpoint_vtoline;
			std::vector<VertexHandle> pickpoint = ui.widget->pickpoint_v;
			std::vector<V3f> pickv3f = ui.widget->pickpoint_v3f;
			in_out_side.push_back(pickpoint);
			in_out_sidev3f.push_back(pickv3f);
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "PPV(*.ppv)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				//filenew.writepickedpoint(name, in_out_side);
				filenew.writepickedpoint(name, in_out_sidev3f);
			}
		});

	ui.actionRead_PickPoint->setIcon(QIcon("Images/Open.png"));
	ui.actionRead_PickPoint->setStatusTip(("Read Picked Point"));
	connect(ui.actionRead_PickPoint, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "PPV(*.ppv)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readpickedpoint(in_out_sidev3f);
		});

	ui.actionOutput_PickedEdge->setIcon(QIcon("Images/Save.png"));
	ui.actionOutput_PickedEdge->setStatusTip(("Output Picked Edge"));
	connect(ui.actionOutput_PickedEdge, &QAction::triggered,
		[=]() {
			/*for (int i = 0; i < mesh->edges_.size(); ++i)
		{
			if (mesh->neighbor_e[i].size() == 2)
				pickedge.push_back(EdgeHandle(i));
		}
		ui.widget->pickedge_e = pickedge;*/
			pickedge = ui.widget->pickedge_e;
			//std::cout << "ui.widget->pickedge_e size: " << ui.widget->pickedge_e.size() << std::endl;
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "PPE(*.ppe)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				//filenew.writepickededge(name, pickedge, mesh);
				filenew.writepickededge(name, pickedge);
			}
		});

	ui.actionRead_PickedEdge->setIcon(QIcon("Images/Open.png"));
	ui.actionRead_PickedEdge->setStatusTip(("Read Picked Edge"));
	connect(ui.actionRead_PickedEdge, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "PPE(*.ppe)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			edgemid.clear();
			pickedge.clear();
			pickedge1.clear();
			//file.readpickededge(edgemid);
			file.readpickededge(pickedge1);
			ui.widget->pickedge_e = pickedge1;
			std::cout << edgemid.size() << std::endl;
			mesh->v_cs.resize(mesh->vertices_.size(), false);
			//std::vector<EdgeHandle> edgesharp;
			/*for (int i = 0; i < mesh->edges_.size(); ++i)
		{
			if (mesh->is_bdy(EdgeHandle(i)))
			{
				V3f v = mesh->cal_edge_center(EdgeHandle(i));
				double min = 999;
				for (int i = 0; i < edgemid.size(); ++i)
				{
					double dis = distance(v, edgemid[i]);
					if (dis < min)
						min = dis;
				}
				if (min < 1.0e-5)
				{
					edgesharp.push_back(EdgeHandle(i));
					mesh->v_cs[mesh->edges_[i].from_h] = true;
					mesh->v_cs[mesh->edges_[i].to_h] = true;
				}
			}
		}*/
			for (int i = 0; i < pickedge1.size(); ++i)
			{
				mesh->v_cs[mesh->edges_[pickedge1[i]].from_h] = true;
				mesh->v_cs[mesh->edges_[pickedge1[i]].to_h] = true;
			}

			/*mesh->v_cs.swap(std::vector<bool>(mesh->vertices_.size(), false));
		for (int i = 0; i < mesh->edges_.size(); ++i)
		{
			if (mesh->is_bdy(EdgeHandle(i)))
			{
				V3f v = mesh->cal_edge_mid(EdgeHandle(i));
				double min = 999;
				for (int i = 0; i < edgemid.size(); ++i)
				{
					double dis = distance(v, edgemid[i]);
					if (dis < min)
						min = dis;
				}
				if (min < 1.0e-5)
				{
					mesh->v_cs[mesh->edges_[i].from_h] = true;
					mesh->v_cs[mesh->edges_[i].to_h] = true;
					ui.widget->pickedge_e.push_back(EdgeHandle(i));
				}
			}
		}*/

		//ui.widget->pickedge_e = edgesharp;
			std::cout << ui.widget->pickedge_e.size() << std::endl;
		});

	ui.actionRead_PickedEdge1->setIcon(QIcon("Images/Open.png"));
	ui.actionRead_PickedEdge1->setStatusTip(("Read Picked Edge"));
	connect(ui.actionRead_PickedEdge1, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "PPE(*.ppe)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			edgemid.clear();
			pickedge.clear();
			pickedge1.clear();
			file.readpickededge(edgemid);
			//file.readpickededge(pickedge1);
			//ui.widget->pickedge_e = pickedge1;
			std::cout << edgemid.size() << std::endl;
			mesh->v_cs.resize(mesh->vertices_.size(), false);
			std::vector<EdgeHandle> edgesharp;
			for (int i = 0; i < mesh->edges_.size(); ++i)
			{
				if (mesh->is_bdy(EdgeHandle(i)))
				{
					V3f v = mesh->cal_edge_center(EdgeHandle(i));
					double min = 999;
					for (int i = 0; i < edgemid.size(); ++i)
					{
						double dis = distance(v, edgemid[i]);
						if (dis < min)
							min = dis;
					}
					if (min < 1.0e-5)
					{
						edgesharp.push_back(EdgeHandle(i));
						mesh->v_cs[mesh->edges_[i].from_h] = true;
						mesh->v_cs[mesh->edges_[i].to_h] = true;
					}
				}
			}
			/*for (int i = 0; i < pickedge1.size(); ++i)
		{
			mesh->v_cs[mesh->edges_[pickedge1[i]].from_h] = true;
			mesh->v_cs[mesh->edges_[pickedge1[i]].to_h] = true;
		}*/

		/*mesh->v_cs.swap(std::vector<bool>(mesh->vertices_.size(), false));
	for (int i = 0; i < mesh->edges_.size(); ++i)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			V3f v = mesh->cal_edge_mid(EdgeHandle(i));
			double min = 999;
			for (int i = 0; i < edgemid.size(); ++i)
			{
				double dis = distance(v, edgemid[i]);
				if (dis < min)
					min = dis;
			}
			if (min < 1.0e-5)
			{
				mesh->v_cs[mesh->edges_[i].from_h] = true;
				mesh->v_cs[mesh->edges_[i].to_h] = true;
				ui.widget->pickedge_e.push_back(EdgeHandle(i));
			}
		}
	}*/

			ui.widget->pickedge_e = edgesharp;
			std::cout << ui.widget->pickedge_e.size() << std::endl;
		});

	ui.actionOutput_PickedFace->setIcon(QIcon("Images/Save.png"));
	ui.actionOutput_PickedFace->setStatusTip(("Output Picked Face"));
	connect(ui.actionOutput_PickedFace, &QAction::triggered,
		[=]() {
			pickface = ui.widget->pickface;
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "PPF(*.ppf)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				filenew.writepickedface(name, pickface);
			}
		});

	ui.actionRead_PickedFace->setIcon(QIcon("Images/Open.png"));
	ui.actionRead_PickedFace->setStatusTip(("Read Picked Face"));
	connect(ui.actionRead_PickedFace, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "PPF(*.ppf)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readpickedface(pickface);
		});

	ui.actionOutputCSV->setIcon(QIcon("Images/Save.png"));
	ui.actionOutputCSV->setStatusTip(("Output CSV"));
	connect(ui.actionOutputCSV, &QAction::triggered,
		[=]() {
			csv.swap(std::vector<VertexHandle>());
			for (int i = 0; i < mesh->v_cs.size(); ++i)
			{
				if (mesh->v_cs[i] == true)
					csv.push_back(VertexHandle(i));
			}
			//csv = ui.widget->pickpoint_v;
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "CSV(*.csv)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				//filenew.writecsv(name, csv, mesh);
				filenew.writecsv(name, csv);
			}
		});

	ui.actionReadCSV->setIcon(QIcon("Images/Open.png"));
	ui.actionReadCSV->setStatusTip(("Read CSV"));
	connect(ui.actionReadCSV, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "CSV(*.csv)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readcsv(csv);
			mesh->v_cs.resize(mesh->vertices_.size(), false);
			for (int i = 0; i < csv.size(); ++i)
			{
				mesh->v_cs[csv[i]] = true;
			}
			ui.widget->pickpoint_v = csv;
			//int count = 0;
			//file.readcsv(csv, csv3f);
			//mesh->v_cs.swap(std::vector<bool>(mesh->vertices_.size(), false));
			//for (int i = 0; i < mesh->vertices_.size(); ++i)
			//{
			//	V3f v = mesh->vertices_[i];
			//	double min = 999;
			//	for (int j = 0; j < csv3f.size(); ++j)
			//	{
			//		V3f tempv = csv3f[j];
			//		double dis = distance(v, tempv);
			//		if (dis < min)
			//			min = dis;
			//	}
			//	if (min < 0.0001)
			//	{
			//		mesh->v_cs[i] = true;
			//		ui.widget->pickpoint_v.push_back(VertexHandle(i));
			//		count++;

			//	}
			//	/*if (std::find(csv3f.begin(), csv3f.end(), v) != csv3f.end())
			//	{
			//		mesh->v_cs[i] = true;
			//		ui.widget->pickpoint_v.push_back(VertexHandle(i));
			//		count++;
			//	}*/
			//
			//}
			//std::cout << "csv3f size:" << count << std::endl;
			/*quad->v_cs.swap(std::vector<bool>(quad->vertices_.size(), false));
		for (int i = 0; i < quad->vertices_.size(); ++i)
		{
			V3f v = quad->vertices_[i];
			if (std::find(csv3f.begin(), csv3f.end(), v) != csv3f.end())
				quad->v_cs[i] = true;
		}*/
		});

	ui.actionOutput_PickedAll->setIcon(QIcon("Images/Save.png"));
	ui.actionOutput_PickedAll->setStatusTip(("Output Picked All"));
	connect(ui.actionOutput_PickedAll, &QAction::triggered,
		[=]() {
			pickvertex = ui.widget->pickpoint_v;
			pickedge = ui.widget->pickedge_e;
			pickface = ui.widget->pickface;
			pickcell = ui.widget->pickcell;
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "PPA(*.ppa)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				filenew.writepickedall(name, pickvertex, pickedge, pickface, pickcell);
			}
		});

	ui.actionOutput_MAXMIN->setIcon(QIcon("Images/Save.png"));
	ui.actionOutput_MAXMIN->setStatusTip(("Output error max and min"));
	connect(ui.actionOutput_MAXMIN, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "MAMI(*.MAMI)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				filenew.writemaxmin(name, ui.widget->errmax, ui.widget->errmin);
			}
		});

	ui.actionRead_PickedAll->setIcon(QIcon("Images/Open.png"));
	ui.actionRead_PickedAll->setStatusTip(("Read Picked All"));
	connect(ui.actionRead_PickedAll, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "PPA(*.ppa)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readpickedall(pickvertex, pickedge, pickface, pickcell);
			ui.widget->pickpoint_v = pickvertex;
			ui.widget->pickedge_e = pickedge;
			ui.widget->pickface = pickface;
			ui.widget->pickcell = pickcell;
			qDebug() << "pickvertex size: " << pickvertex.size();
			qDebug() << "pickedge size: " << pickedge.size();
			qDebug() << "pickface size: " << pickface.size();
			qDebug() << "pickcell size: " << pickcell.size();
		});

	ui.actionPDESparse->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPDESparse->setStatusTip(("Cal PDE Sparse"));
	connect(ui.actionPDESparse, &QAction::triggered,
		[=]() {
			FunctionBSpline fb(m_solids);
			std::vector<BdyBase> bdybaseid;
			/*-----------------����նȾ�����Ҷ���-----------------*/
			std::vector<std::vector<double>> m_bdycoef;
			m_bdycoef.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_V3f[i].size(), 0);
			}

			/*V3f m_glo_cen = mesh->center;
		int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
		for (int i = 0; i< m_solids.Local_V3f.size(); i++)
		{
			for (int j = 0; j < m_solids.Local_V3f[i].size(); j++)
			{
				V3f temp_v = m_solids.Local_V3f[i][j];
				double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
					+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = j;
					mindis_cellid = i;
				}
			}
		}
		m_bdycoef[mindis_cellid][mindis_id] = 10;
		bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));*/

			for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
				{
					if (m_solids.local_bdy[Local_V3f_id][v_id])
					{
						bool check = true;
						for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
						{
							if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
							{
								check = false;
								break;
							}
						}
						if (check == true)
							bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
					}
				}
			}

			/*for (int j = 8; j <= 15; j++)
		{
			m_bdycoef[0][j] = 10;
			bdybaseid.push_back(BdyBase(j, m_solids.Local_to_Global[0][j], 0));
		}*/
		/*for (int cen_i = 0; cen_i < glo_cen.size(); cen_i++)
	{
		V3f m_glo_cen = glo_cen[cen_i];
		int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
		for (int i = 0; i < m_solids.Local_V3f.size(); i++)
		{
			for (int j = 0; j < m_solids.Local_V3f[i].size(); j++)
			{
				V3f temp_v = m_solids.Local_V3f[i][j];
				double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
					+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = j;
					mindis_cellid = i;
				}
			}
		}
		m_bdycoef[mindis_cellid][mindis_id] = 0;
		bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
	}*/

	/*in_out_side = ui.widget->pickpoint_vtoline;
for (int i = 0; i < in_out_side.size(); i++)
{
	std::vector<VertexHandle> temp_in_out = in_out_side[i];
	for (int j = 0; j < temp_in_out.size(); j++)
	{
		int global_id = temp_in_out[j];
		for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
		{
			bool ocheck = false;
			for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
			{
				if (m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
					&& m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id] == global_id)
				{
					bool check = true;
					for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
					{
						if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
						{
							check = false;
							break;
						}
					}
					if (check == true)
					{
						if (i <= 4)
							m_bdycoef[Local_BSplineSolid_id][v_id] = 10;
						else
							m_bdycoef[Local_BSplineSolid_id][v_id] = 0;
						bdybaseid.push_back(BdyBase(v_id, m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
						ocheck = true;
						break;
					}
				}
			}
			if (ocheck == true)
				break;
		}
	}
}*/

			std::vector<double> m_bigrhs;
			std::vector<int> rows, cols; //ϡ��洢��Ԫ��������Ϣ
			std::vector<double> elems;   //ϡ��洢��Ԫ����Ϣ
			fb.Global_MatrixAndRhs_Sparse(m_bdycoef, bdybaseid, m_bigrhs, rows, cols, elems);
			/*-----------------������ͷ�����-----------------*/
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_solids.Global_size, rows, cols, elems, m_bigrhs, m_coef);
			for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
				{
					m_bdycoef[Local_V3f_id][v_id] = m_coef[m_solids.Local_to_Global[Local_V3f_id][v_id]];
				}
			}
			std::vector<double> m_color;
			std::cout << "Start constructBezierSolid" << std::endl;
			//*mesh = constructBezierSolid1(m_bdycoef, m_color);
			if (times == 0)
			{
				*mesh = constructBezierSolid1(m_bdycoef, m_color, 2);
				mesh_1 = new HexV3fMesh();
				*mesh_1 = *mesh;
				//mesh1_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color1 = m_color;
				times++;
			}
			else if (times == 1)
			{
				*mesh = constructBezierSolid1(m_bdycoef, m_color, 4);
				mesh_2 = new HexV3fMesh();
				*mesh_2 = *mesh;
				//mesh2_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color2 = m_color;
				times++;
			}
			else if (times == 2)
			{
				*mesh = constructBezierSolid1(m_bdycoef, m_color, 2);
				mesh_2 = new HexV3fMesh();
				*mesh_2 = *mesh;
				//mesh2_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color2 = m_color;
				times++;
			}
			else
			{
				*mesh = constructBezierSolid1(m_bdycoef, m_color, 16);
				mesh_2 = new HexV3fMesh();
				*mesh_2 = *mesh;
				//mesh2_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color2 = m_color;
				times++;
			}
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			//ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionPDESparseOld->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPDESparseOld->setStatusTip(("Cal PDE Sparse old"));
	connect(ui.actionPDESparseOld, &QAction::triggered,
		[=]() {
			FunctionBSpline fb(m_solids);
			std::vector<BdyBase> bdybaseid;
			/*-----------------����նȾ�����Ҷ���-----------------*/
			std::vector<std::vector<double>> m_bdycoef;
			m_bdycoef.resize(m_solids.Local_BSplineSolid.size());
			for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_BSplineSolid[i].vertices_.size(), 0);
			}
			/*V3f m_glo_cen = mesh->center;
		int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
		for (int i = 0; i< m_solids.Local_BSplineSolid.size(); i++)
		{
			for (int j = 0; j < m_solids.Local_BSplineSolid[i].vertices_.size(); j++)
			{
				V3f temp_v = m_solids.Local_BSplineSolid[i].vertices_[j];
				double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
					+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = j;
					mindis_cellid = i;
				}
			}
		}
		m_bdycoef[mindis_cellid][mindis_id] = 10;
		bdybaseid.push_back(BdyBase(mindis_id, m_solids.CP_Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));*/
		/*for (int j = 8; j <= 15; j++)
	{
		m_bdycoef[0][j] = 10;
		bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[0][j], 0));
	}*/
	/*glo_cen.push_back(V3f(0, 0, 1.33333));
glo_cen.push_back(V3f(0, 0, 4.33333));
for (int cen_i = 0; cen_i < glo_cen.size(); cen_i++)
{
	V3f m_glo_cen = glo_cen[cen_i];
	int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
	for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
	{
		for (int j = 0; j < m_solids.Local_BSplineSolid[i].vertices_.size(); j++)
		{
			V3f temp_v = m_solids.Local_BSplineSolid[i].vertices_[j];
			double dis = sqrt((temp_v.x - m_glo_cen.x)*(temp_v.x - m_glo_cen.x) + (temp_v.y - m_glo_cen.y)*(temp_v.y - m_glo_cen.y)
				+ (temp_v.z - m_glo_cen.z)*(temp_v.z - m_glo_cen.z));
			if (dis < min_dis)
			{
				min_dis = dis;
				mindis_id = j;
				mindis_cellid = i;
			}
		}
	}
	m_bdycoef[mindis_cellid][mindis_id] = 10;
	bdybaseid.push_back(BdyBase(mindis_id, m_solids.CP_Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
}*/
//m_solids.Global_BSplineSolid.examine_bdy();
//V3f temp1(0, 0, 1.3333);
//V3f temp2(0, 0, 6);
//for (int i = 0; i < m_solids.Local_BSplineSolid.size(); i++)
//{
//	for (int j = 0; j < m_solids.Local_BSplineSolid[i].vertices_.size(); j++)
//	{
//		V3f temp_v = m_solids.Local_BSplineSolid[i].vertices_[j];
//		/*if (distance(temp_v, temp1) < 1.0e-4)
//		{
//			bool check = true;
//			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
//			{
//				if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[i][j])
//				{
//					check = false;
//					break;
//				}
//			}
//			if (check == true)
//			{
//				m_bdycoef[i][j] = 10;
//				bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[i][j], i));
//			}
//		}
//		else if (distance(temp_v, temp2) < 1.0e-4)
//		{
//			bool check = true;
//			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
//			{
//				if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[i][j])
//				{
//					check = false;
//					break;
//				}
//			}
//			if (check == true)
//			{
//				m_bdycoef[i][j] = 0;
//				bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[i][j], i));
//			}
//		}*/
//		if (fabs(temp_v.x - 0) < 1.0e-7&& fabs(temp_v.y - 0) < 1.0e-7)
//		{
//			bool check = true;
//			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
//			{
//				if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[i][j])
//				{
//					check = false;
//					break;
//				}
//			}
//			if (check == true)
//			{
//				m_bdycoef[i][j] = 10;
//				bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[i][j], i));
//			}
//		}
//		else if (m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[i][j]))
//		{
//			bool check = true;
//			for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
//			{
//				if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[i][j])
//				{
//					check = false;
//					break;
//				}
//			}
//			if (check == true)
//			{
//				m_bdycoef[i][j] = 0;
//				bdybaseid.push_back(BdyBase(j, m_solids.CP_Local_to_Global[i][j], i));
//			}
//		}
//	}
//}
//in_out_side = ui.widget->pickpoint_vtoline;
//for (int i = 0; i < in_out_side.size(); i++)
//{
//	std::vector<VertexHandle> temp_in_out = in_out_side[i];
//	for (int j = 0; j < temp_in_out.size(); j++)
//	{
//		int global_id = temp_in_out[j];
//		for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
//		{
//			bool ocheck = false;
//			for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
//			{
//				if (/*m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
//					&&*/ m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id] == global_id)
//				{
//					bool check = true;
//					for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
//					{
//						if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
//						{
//							check = false;
//							break;
//						}
//					}
//					if (check == true)
//					{
//						if (j < 1)
//							m_bdycoef[Local_BSplineSolid_id][v_id] = 10;
//						else
//							m_bdycoef[Local_BSplineSolid_id][v_id] = 0;
//						bdybaseid.push_back(BdyBase(v_id, m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
//						ocheck = true;
//						break;
//					}
//				}
//			}
//			if (ocheck == true)
//				break;
//		}
//	}
//}
//std::vector<VertexHandle> pickpoint = ui.widget->pickpoint_v;
//for (int j = 0; j < pickpoint.size(); j++)
//{
//	int global_id = pickpoint[j];
//	for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
//	{
//		bool ocheck = false;
//		for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
//		{
//			if (m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id] == global_id)
//			{
//				bool check = true;
//				for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
//				{
//					if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
//					{
//						check = false;
//						break;
//					}
//				}
//				if (check == true)
//				{
//					//if (j < 1)
//						m_bdycoef[Local_BSplineSolid_id][v_id] = 10;
//					//else
//						//m_bdycoef[Local_BSplineSolid_id][v_id] = 0;
//					bdybaseid.push_back(BdyBase(v_id, m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
//					ocheck = true;
//					break;
//				}
//			}
//		}
//		if (ocheck == true)
//			break;
//	}
//}
			m_solids.Global_BSplineSolid.examine_bdy();
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					if (m_solids.Global_BSplineSolid.is_bdy(m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]))
					{
						bool check = true;
						for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
						{
							if ((*bdybaseid_id).global_id == m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id])
							{
								check = false;
								break;
							}
						}
						if (check == true)
							bdybaseid.push_back(BdyBase(v_id, m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id], Local_BSplineSolid_id));
					}
				}
			}
			std::vector<double> m_bigrhs;
			std::vector<int> rows, cols; //ϡ��洢��Ԫ��������Ϣ
			std::vector<double> elems;   //ϡ��洢��Ԫ����Ϣ
			fb.Global_MatrixAndRhs_SparseOld(m_bdycoef, bdybaseid, m_bigrhs, rows, cols, elems);
			/*-----------------������ͷ�����-----------------*/
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_solids.Global_BSplineSolid.vertices_.size(), rows, cols, elems, m_bigrhs, m_coef);
			for (int Local_BSplineSolid_id = 0; Local_BSplineSolid_id < m_solids.Local_BSplineSolid.size(); Local_BSplineSolid_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_BSplineSolid[Local_BSplineSolid_id].vertices_.size(); v_id++)
				{
					m_bdycoef[Local_BSplineSolid_id][v_id] = m_coef[m_solids.CP_Local_to_Global[Local_BSplineSolid_id][v_id]];
				}
			}
			std::vector<double> m_color;
			if (times == 0)
			{
				*mesh = constructBezierSolid(m_bdycoef, m_color, 2);
				mesh_1 = new HexV3fMesh();
				*mesh_1 = *mesh;
				//mesh1_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color1 = m_color;
				times++;
			}
			else if (times == 1)
			{
				*mesh = constructBezierSolid(m_bdycoef, m_color, 4);
				mesh_2 = new HexV3fMesh();
				*mesh_2 = *mesh;
				//mesh2_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color2 = m_color;
				times++;
			}
			else if (times == 2)
			{
				*mesh = constructBezierSolid(m_bdycoef, m_color, 2);
				mesh_2 = new HexV3fMesh();
				*mesh_2 = *mesh;
				//mesh2_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color2 = m_color;
				times++;
			}
			else
			{
				*mesh = constructBezierSolid(m_bdycoef, m_color, 16);
				mesh_2 = new HexV3fMesh();
				*mesh_2 = *mesh;
				//mesh2_ = constructBezierSolid(m_bdycoef, m_color, 2);
				m_color2 = m_color;
				times++;
			}
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			/*if (times == 1)
		{
			mesh1_.examine_bdy();
			mesh1_.cal_cen();
			mesh1_.examine_bdfv();
			mesh1_.cal_cell_cen();
			mesh1_.cal_bdnorm();
			mesh1_.cal_mami_ed();
			ui.widget->setmesh(&mesh1_);
			ui.widget->setbffv(mesh1_.boundary_face_v, mesh1_.normal_boundary_face_v, mesh1_.boundary_face, mesh1_.boundary_face_VID);
			ui.widget->set_scene_pos(mesh1_.center, mesh1_.r);
			ui.widget->setcell_center(mesh1_.cell_center);
			ui.widget->seted(mesh1_.maxEdgeLen, mesh1_.minEdgeLen);
			ui.widget->setbb(mesh1_.bbMax, mesh1_.bbMin);
		}
		else
		{
			mesh2_.examine_bdy();
			mesh2_.cal_cen();
			mesh2_.examine_bdfv();
			mesh2_.cal_cell_cen();
			mesh2_.cal_bdnorm();
			mesh2_.cal_mami_ed();
			ui.widget->setmesh(&mesh2_);
			ui.widget->setbffv(mesh2_.boundary_face_v, mesh2_.normal_boundary_face_v, mesh2_.boundary_face, mesh2_.boundary_face_VID);
			ui.widget->set_scene_pos(mesh2_.center, mesh2_.r);
			ui.widget->setcell_center(mesh2_.cell_center);
			ui.widget->seted(mesh2_.maxEdgeLen, mesh2_.minEdgeLen);
			ui.widget->setbb(mesh2_.bbMax, mesh2_.bbMin);
		}*/
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionPDESparseSIMP->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPDESparseSIMP->setStatusTip(("PDE Sparse SIMP"));
	connect(ui.actionPDESparseSIMP, &QAction::triggered,
		[=]() {
			std::vector<double>().swap(material_distribution);
			material_distribution.resize(mesh->cells_.size(), 1);
			std::vector<double> sensitivity_pre(mesh->cells_.size(), 0);
			int iteration_time = 2;
			int iteration_num = 0;
			std::vector<std::vector<double>> local_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			std::vector<std::vector<int>> m_colids;
			std::vector<std::vector<std::pair<int, int>>> m_bdyquick;
			m_bdycoef.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_V3f[i].size(), 0);
			}
			/*-----�˽���0------*/
			// bool check = false;
			// for (int cen_i = 0; cen_i < glo_cen.size(); cen_i++)//�˽���0
			// {
			// check = false;
			// V3f m_glo_cen = glo_cen[cen_i];
			// int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
			// for (int j = 0; j < m_solids.Local_V3f[glo_cen_id[cen_i]].size(); j++)
			// {
			// V3f temp_v = m_solids.Local_V3f[glo_cen_id[cen_i]][j];
			// if(temp_v.x == m_glo_cen.x && temp_v.y == m_glo_cen.y && temp_v.z == m_glo_cen.z)
			// {
			// mindis_id = j;
			// mindis_cellid = glo_cen_id[cen_i];
			// check = true;
			// break;
			// }
			// }
			// if(check == false)
			// {
			// std::cout<<"can not find corner point"<<std::endl;
			// return 0;
			// }
			// m_bdycoef[mindis_cellid][mindis_id] = 0;
			// bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
			// }
			/*-----��������м���10------*/
			glo_cen.push_back(V3f(0, 0, 1.33333));
			glo_cen.push_back(V3f(0, 0, 4.33333));
			for (int cen_i = 0; cen_i < glo_cen.size(); cen_i++)
			{
				V3f m_glo_cen1 = glo_cen[cen_i];
				int mindis_id = 0;
				double min_dis = 999;
				int mindis_cellid = 0;
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					for (int j = 0; j < m_solids.Local_V3f[i].size(); j++)
					{
						V3f temp_v = m_solids.Local_V3f[i][j];
						double dis = sqrt((temp_v.x - m_glo_cen1.x) * (temp_v.x - m_glo_cen1.x) + (temp_v.y - m_glo_cen1.y) * (temp_v.y - m_glo_cen1.y) + (temp_v.z - m_glo_cen1.z) * (temp_v.z - m_glo_cen1.z));
						if (dis < min_dis)
						{
							min_dis = dis;
							mindis_id = j;
							mindis_cellid = i;
						}
					}
				}
				m_bdycoef[mindis_cellid][mindis_id] = 10;
				bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
			}
			/*-----��һ�м���10------*/
			/*int mindis_id = 0; double min_dis = 999; int mindis_cellid = 0;
		V3f m_glo_cen1 = mesh->center;
		for (int i = 0; i< m_solids.Local_V3f.size(); i++)
		{
			for (int j = 0; j <  m_solids.Local_V3f[i].size(); j++)
			{
				V3f temp_v = m_solids.Local_V3f[i][j];
				double dis = sqrt((temp_v.x - m_glo_cen1.x)*(temp_v.x - m_glo_cen1.x) + (temp_v.y - m_glo_cen1.y)*(temp_v.y - m_glo_cen1.y)
					+ (temp_v.z - m_glo_cen1.z)*(temp_v.z - m_glo_cen1.z));
				if (dis < min_dis)
				{
					min_dis = dis;
					mindis_id = j;
					mindis_cellid = i;
				}
			}
		}*/
		/*for (int i = 0; i< m_solids.Local_V3f.size(); i++)
	{
		for (int j = 0; j <  m_solids.Local_V3f[i].size(); j++)
		{
			V3f temp_v = m_solids.Local_V3f[i][j];
			double dis = sqrt((temp_v.x - m_glo_cen1.x)*(temp_v.x - m_glo_cen1.x) + (temp_v.y - m_glo_cen1.y)*(temp_v.y - m_glo_cen1.y)
				+ (temp_v.z - m_glo_cen1.z)*(temp_v.z - m_glo_cen1.z));
			if (sqrt(pow(dis - min_dis,2)) <= 0.0001)
			{
				m_bdycoef[i][j] = 10;
				bdybaseid.push_back(BdyBase(j, m_solids.Local_to_Global[i][j], i));
			}
		}
	}*/
	//m_bdycoef[mindis_cellid][mindis_id] = 10;
	//bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
	/*-----��Ե��0------*/
			for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
				{
					if (m_solids.local_bdy[Local_V3f_id][v_id])
					{
						bool check = true;
						for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
						{
							if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
							{
								check = false;
								break;
							}
						}
						if (check == true)
							bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
					}
				}
			}
			/*-----����Ȧ��0����Ȧ��10------*/
			// for (int i = 0; i < in_out_side.size(); i++)
			// {
			// if(i == 0 || i == 13)
			// {
			// std::vector<VertexHandle> temp_in_out = in_out_side[i];
			// for (int j = 0; j < temp_in_out.size(); j++)
			// {
			// int global_id = temp_in_out[j];
			// for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			// {
			// bool ocheck = false;
			// for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
			// {
			// if (m_solids.local_bdy[Local_V3f_id][v_id]
			// && m_solids.Local_to_Global[Local_V3f_id][v_id] == global_id)
			// {
			// bool check = true;
			// for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
			// {
			// if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
			// {
			// check = false;
			// break;
			// }
			// }
			// if (check == true)
			// {
			// if (i == 0)
			// m_bdycoef[Local_V3f_id][v_id] = 10;
			// else
			// m_bdycoef[Local_V3f_id][v_id] = 0;
			// bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
			// ocheck = true;
			// break;
			// }
			// }
			// }
			// if (ocheck == true)
			// break;
			// }
			// }
			// }
			// }
			// std::cout<<"bdybaseid size "<<bdybaseid.size()<<std::endl;
			// for(int i = 0;i<bdybaseid.size();i++)
			// {
			// V3f tempdby = m_solids.Local_V3f[bdybaseid[i].solid_id][bdybaseid[i].local_id];
			// std::cout<<tempdby.x<<" "<<tempdby.y<<" "<<tempdby.z<<std::endl;
			// }
			// return 0;
			/*-----ָ������10------*/
			/*for (int i = 0; i < in_out_side.size(); i++)
		{
			std::vector<VertexHandle> temp_in_out = in_out_side[i];
			for (int j = 0; j < temp_in_out.size(); j++)
			{
				int global_id = temp_in_out[j];
				for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
				{
					bool ocheck = false;
					for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
					{
						if ( m_solids.Local_to_Global[Local_V3f_id][v_id] == global_id)
						{
							bool check = true;
							for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
							{
								if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
								{
									check = false;
									break;
								}
							}
							if (check == true)
							{
								m_bdycoef[Local_V3f_id][v_id] = 10;
								bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
								ocheck = true;
								break;
							}
						}
					}
					if (ocheck == true)
						break;
				}
			}
		}*/
			FunctionBSpline fb(m_solids, material_distribution);
			std::cout << "Start iteration" << std::endl;
			for (iteration_num = 0; iteration_num < iteration_time; iteration_num++)
				//while (true)
			{
				std::cout << "Schedule of iteration " << iteration_num + 1 << ":" << iteration_time << std::endl;
				/*-----------------����PDE-----------------*/
				if (iteration_num != 0)
					fb.update_material_distribution(material_distribution);
				/*-----------------����նȾ�����Ҷ���-----------------*/
				std::cout << "Start constructing stiffness matrix and the right end term" << std::endl;
				std::vector<double> m_bigrhs;
				std::vector<int> rows, cols; //ϡ��洢��Ԫ��������Ϣ
				std::vector<double> elems;   //ϡ��洢��Ԫ����Ϣ
				if (iteration_num == 0)
					fb.Global_MatrixAndRhs_Sparse(m_bdycoef, bdybaseid, local_matrix, m_bigrhs,
						local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
				else
					fb.Global_MatrixAndRhs_Sparse_quick(m_bdycoef, bdybaseid, local_matrix, m_bigrhs,
						local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
				/*-----------------������ͷ�����-----------------*/
				std::cout << "Start solving linear equations" << std::endl;
				std::vector<double> m_coef;
				fb.SolveLinearEquation(-2, m_solids.Global_size, rows, cols, elems, m_bigrhs, m_coef);
				for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
				{
					for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
					{
						m_bdycoef[Local_V3f_id][v_id] = m_coef[m_solids.Local_to_Global[Local_V3f_id][v_id]];
					}
				}
				std::cout << "Solving linear equations completely" << std::endl;
				/*-----------------�����ȷ���-----------------*/
				std::cout << "Start sensitivity analysis" << std::endl;
				std::vector<simpsort> sensitivity;
				sensitivity.resize(m_solids.Local_V3f.size());
				std::vector<double> temp_sen(m_solids.Local_V3f.size());
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					if (material_distribution[i] != 1)
						temp_sen[i] = 0;
					else
						temp_sen[i] = fb.uTKu(m_bdycoef[i], local_matrix[i]) / 2;
				}
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					double rmin = 0;
					std::set<CellHandle> c_c = mesh->neighbor_c_c(CellHandle(i));
					for (std::set<CellHandle>::iterator c_c_it = c_c.begin(); c_c_it != c_c.end(); c_c_it++)
					{
						rmin = rmin + mesh->cell_r[i] + mesh->cell_r[*c_c_it];
					}
					rmin = rmin / c_c.size();
					rmin = rmin * 2;
					double weight = 0;
					double temp_senitiv = 0;
					for (int k = 0; k < m_solids.Local_V3f.size(); k++)
					{
						double rdis = sqrt(pow(mesh->cell_center[i].x - mesh->cell_center[k].x, 2) + pow(mesh->cell_center[i].y - mesh->cell_center[k].y, 2) + pow(mesh->cell_center[i].z - mesh->cell_center[k].z, 2));
						if (rdis < rmin)
						{
							weight += rmin - rdis;
							temp_senitiv += temp_sen[k] * (rmin - rdis);
						}
					}
					temp_senitiv /= weight;
					if (iteration_num == 0) //��һ�εĻ������Լ�
						sensitivity[i].senitiv = temp_senitiv;
					else
						sensitivity[i].senitiv = (temp_senitiv + sensitivity_pre[i]) / 2;
					sensitivity_pre[i] = sensitivity[i].senitiv;
					sensitivity[i].cell_id = i;
				}
				sensitivity1 = sensitivity;
				simsor();
				std::cout << "Sensitivity analysis completely" << std::endl;
				std::cout << "Start update material_distribution" << std::endl;
				//double delete_rate = 1 - pow(0.9, iteration_num + 1);
				double delete_rate = (0.5 / iteration_time) * (iteration_num + 1);
				int delete_num = delete_rate * m_solids.Local_V3f.size();
				material_distribution.clear();
				material_distribution.resize(m_solids.Local_BSplineSolid.size(), 1);
				for (int i = 0; i < delete_num; i++)
				{
					material_distribution[sensitivity1[i].cell_id] = 0.0001;
				}
				std::cout << "Update material_distribution completely" << std::endl;
				/*if (delete_rate >= 0.5)
			{
				std::cout << iteration_num << "th iteration's delete_rate: " << delete_rate << " >= " << 0.5 << " break while" << std::endl;
				break;
			}
			else
				std::cout << iteration_num << "th iteration's delete_rate: " << delete_rate << " < " << 0.5 << " continue" << std::endl;
			iteration_num++;*/
			}
			/*-----------------ɾ����-----------------*/
			std::cout << "Start delete" << std::endl;
			int delete_num = m_solids.Local_V3f.size();
			for (int i = 0; i < delete_num; i++)
			{
				if (material_distribution[i] != 1)
				{
					simpmesh->delete_cell(CellHandle(i));
				}
			}
			std::cout << "Delete completely" << std::endl;
			*mesh = *simpmesh;
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionPDESpareLiner->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPDESpareLiner->setStatusTip(("PDE SIMP liner"));
	connect(ui.actionPDESpareLiner, &QAction::triggered,
		[=]() {
			std::vector<double>().swap(material_distribution);
			material_distribution.resize(mesh->cells_.size(), 1);
			std::vector<double> sensitivity_pre(mesh->cells_.size(), 0);
			int iteration_time = 10;
			int iteration_num = 0;
			Matrix m_bigmatrix;
			std::vector<double> m_bigrhs;
			std::vector<std::vector<double>> local_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			//std::vector<std::vector<int>> m_colids;
			//std::vector<std::vector<std::pair<int, int>>> m_bdyquick;
			m_bdycoef.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_V3f[i].size() * 3, 0);
			}
			/*-----liner------*/
			for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
				{
					V3f tempv = m_solids.Local_V3f[Local_V3f_id][v_id];
					if (m_solids.local_bdy[Local_V3f_id][v_id])
					{
						if (tempv.y == 4 && tempv.x == 0)
						{
							m_bdycoef[Local_V3f_id][3 * v_id + 1] = 10; //Y����
							bool check = true;
							for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
							{
								if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
								{
									check = false;
									break;
								}
							}
							if (check == true)
								bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
						}
					}
				}
			}
			for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			{
				for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
				{
					V3f tempv = m_solids.Local_V3f[Local_V3f_id][v_id];
					if (m_solids.local_bdy[Local_V3f_id][v_id])
					{
						if (tempv.x == 9)
						{
							m_bdycoef[Local_V3f_id][3 * v_id] = 0;     //X����
							m_bdycoef[Local_V3f_id][3 * v_id + 1] = 0; //Y����
							m_bdycoef[Local_V3f_id][3 * v_id + 2] = 0; //Z����
							bool check = true;
							for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
							{
								if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
								{
									check = false;
									break;
								}
							}
							if (check == true)
								bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
						}
					}
				}
			}
			int count = 0;
			for (int i = 0; i < bdybaseid.size(); i++)
			{
				V3f tempdby = m_solids.Local_V3f[bdybaseid[i].solid_id][bdybaseid[i].local_id];
				if (tempdby.y == 4 && tempdby.x == 0)
					count++;
			}
			FunctionBSpline fb(m_solids, material_distribution);
			std::cout << "Start iteration" << std::endl;
			for (iteration_num = 0; iteration_num < iteration_time; iteration_num++)
				//while (true)
			{
				std::cout << "Schedule of iteration " << iteration_num + 1 << ":" << iteration_time << std::endl;
				/*-----------------����PDE-----------------*/
				if (iteration_num != 0)
					fb.update_material_distribution(material_distribution);
				/*-----------------����նȾ�����Ҷ���-----------------*/
				std::cout << "Start constructing stiffness matrix and the right end term" << std::endl;
				if (iteration_num == 0)
					fb.Global_MatrixAndRhs_liner(bdybaseid, m_bigmatrix, m_bigrhs, local_matrix, local_bigrhs);
				else
					fb.Global_MatrixAndRhs_liner_quick(m_bigmatrix, m_bigrhs, local_matrix, local_bigrhs);
				//std::vector<double> m_bigrhs;
				//std::vector<int> rows, cols;          //ϡ��洢��Ԫ��������Ϣ
				//std::vector<double> elems;             //ϡ��洢��Ԫ����Ϣ
				//if (iteration_num == 0)
				//	fb.Global_MatrixAndRhs_Sparse_liner(m_bdycoef, bdybaseid, local_matrix, m_bigrhs,
				//		local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
				//else
				//	fb.Global_MatrixAndRhs_Sparse_quick(m_bdycoef, bdybaseid, local_matrix, m_bigrhs,
				//		local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
				/*-----------------�����նȾ�����Ҷ���-----------------*/
				std::cout << "Start processing right item" << std::endl;
				int m_GlobalSize = m_solids.Global_size;
				int bdybase_count = 0;
				for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
				{
					std::cout << "Schedule of processing right item " << bdybase_count + 1 << ":" << bdybaseid.size() << std::endl;
					int m_baseid = (*bdybaseid_id).local_id;
					int m_solidid = (*bdybaseid_id).solid_id;
					int m_globalid = (*bdybaseid_id).global_id;
					/*if (bdybase_count < 5)
					m_bigrhs[m_globalid * 3 + 1] += (fb.lbda + 2 * fb.mu) * 2;*/
					for (int j = 0; j < m_GlobalSize; j++)
					{
						if (bdybase_count < 5)
						{
							m_bigrhs[3 * j] -= m_bdycoef[m_solidid][3 * m_baseid + 1] * m_bigmatrix[3 * j][3 * m_globalid + 1];
							m_bigrhs[3 * j + 1] -= m_bdycoef[m_solidid][3 * m_baseid + 1] * m_bigmatrix[3 * j + 1][3 * m_globalid + 1];
							m_bigrhs[3 * j + 2] -= m_bdycoef[m_solidid][3 * m_baseid + 1] * m_bigmatrix[3 * j + 2][3 * m_globalid + 1];
						}
						else
						{
							m_bigrhs[3 * j] -= m_bdycoef[m_solidid][3 * m_baseid] * m_bigmatrix[3 * j][3 * m_globalid] +
								m_bdycoef[m_solidid][3 * m_baseid + 1] * m_bigmatrix[3 * j][3 * m_globalid + 1] +
								m_bdycoef[m_solidid][3 * m_baseid + 2] * m_bigmatrix[3 * j][3 * m_globalid + 2];
							m_bigrhs[3 * j + 1] -= m_bdycoef[m_solidid][3 * m_baseid] * m_bigmatrix[3 * j + 1][3 * m_globalid] +
								m_bdycoef[m_solidid][3 * m_baseid + 1] * m_bigmatrix[3 * j + 1][3 * m_globalid + 1] +
								m_bdycoef[m_solidid][3 * m_baseid + 2] * m_bigmatrix[3 * j + 1][3 * m_globalid + 2];
							m_bigrhs[3 * j + 2] -= m_bdycoef[m_solidid][3 * m_baseid] * m_bigmatrix[3 * j + 2][3 * m_globalid] +
								m_bdycoef[m_solidid][3 * m_baseid + 1] * m_bigmatrix[3 * j + 2][3 * m_globalid + 1] +
								m_bdycoef[m_solidid][3 * m_baseid + 2] * m_bigmatrix[3 * j + 2][3 * m_globalid + 2];
						}
					}
					for (int j = 0; j < m_GlobalSize; j++)
					{
						if (bdybase_count < 5)
						{
							m_bigmatrix[j][3 * m_globalid + 1] = 0;
							m_bigmatrix[3 * m_globalid + 1][j] = 0;
						}
						else
						{
							m_bigmatrix[j][3 * m_globalid] = 0;
							m_bigmatrix[3 * m_globalid][j] = 0;
							m_bigmatrix[j][3 * m_globalid + 1] = 0;
							m_bigmatrix[3 * m_globalid + 1][j] = 0;
							m_bigmatrix[j][3 * m_globalid + 2] = 0;
							m_bigmatrix[3 * m_globalid + 2][j] = 0;
						}
					}
					if (bdybase_count < 5)
					{
						m_bigmatrix[3 * m_globalid + 1][3 * m_globalid + 1] = 1;
						m_bigrhs[3 * m_globalid + 1] = m_bdycoef[m_solidid][3 * m_baseid + 1];
					}
					else
					{
						m_bigmatrix[3 * m_globalid][3 * m_globalid] = 1;
						m_bigmatrix[3 * m_globalid][3 * m_globalid + 1] = 1;
						m_bigmatrix[3 * m_globalid][3 * m_globalid + 2] = 1;
						m_bigrhs[3 * m_globalid] = m_bdycoef[m_solidid][3 * m_baseid];
						m_bigmatrix[3 * m_globalid + 1][3 * m_globalid] = 1;
						m_bigmatrix[3 * m_globalid + 1][3 * m_globalid + 1] = 1;
						m_bigmatrix[3 * m_globalid + 1][3 * m_globalid + 2] = 1;
						m_bigrhs[3 * m_globalid + 1] = m_bdycoef[m_solidid][3 * m_baseid + 1];
						m_bigmatrix[3 * m_globalid + 2][3 * m_globalid] = 1;
						m_bigmatrix[3 * m_globalid + 2][3 * m_globalid + 1] = 1;
						m_bigmatrix[3 * m_globalid + 2][3 * m_globalid + 2] = 1;
						m_bigrhs[3 * m_globalid + 2] = m_bdycoef[m_solidid][3 * m_baseid + 2];
					}
					bdybase_count++;
				}
				std::cout << "Processing right item completely" << std::endl;
				/*-----------------������ͷ�����-----------------*/
				std::cout << "Start solving linear equations" << std::endl;
				std::vector<double> m_coef;
				//fb.SolveLinearEquation(-2, m_solids.Global_size, rows, cols, elems, m_bigrhs, m_coef);
				fb.SolveLinearEquation(-2, m_bigmatrix, m_bigrhs, m_coef);
				for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
					for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
						for (int k = 0; k < 3; k++)
							m_bdycoef[Local_V3f_id][3 * v_id + k] = m_coef[3 * m_solids.Local_to_Global[Local_V3f_id][v_id] + k];
				std::cout << "Solving linear equations completely" << std::endl;
				/*-----------------�����ȷ���-----------------*/
				std::cout << "Start sensitivity analysis" << std::endl;
				std::vector<simpsort> sensitivity;
				sensitivity.resize(m_solids.Local_V3f.size());
				std::vector<double> temp_sen(m_solids.Local_V3f.size());
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					if (material_distribution[i] != 1)
						temp_sen[i] = 0;
					else
						temp_sen[i] = fb.uTKu(m_bdycoef[i], local_matrix[i]) / 2;
				}
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					double rmin = 0;
					std::set<CellHandle> c_c = mesh->neighbor_c_c(CellHandle(i));
					for (std::set<CellHandle>::iterator c_c_it = c_c.begin(); c_c_it != c_c.end(); c_c_it++)
					{
						rmin = rmin + mesh->cell_r[i] + mesh->cell_r[*c_c_it];
					}
					rmin = rmin / c_c.size();
					rmin = rmin * 2;
					double weight = 0;
					double temp_senitiv = 0;
					for (int k = 0; k < m_solids.Local_V3f.size(); k++)
					{
						double rdis = sqrt(pow(mesh->cell_center[i].x - mesh->cell_center[k].x, 2) + pow(mesh->cell_center[i].y - mesh->cell_center[k].y, 2) + pow(mesh->cell_center[i].z - mesh->cell_center[k].z, 2));
						if (rdis < rmin)
						{
							weight += rmin - rdis;
							temp_senitiv += temp_sen[k] * (rmin - rdis);
						}
					}
					temp_senitiv /= weight;
					if (iteration_num == 0) //��һ�εĻ������Լ�
						sensitivity[i].senitiv = temp_senitiv;
					else
						sensitivity[i].senitiv = (temp_senitiv + sensitivity_pre[i]) / 2;
					sensitivity_pre[i] = sensitivity[i].senitiv;
					sensitivity[i].cell_id = i;
				}
				sensitivity1 = sensitivity;
				simsor();
				std::cout << "Sensitivity analysis completely" << std::endl;
				std::cout << "Start update material_distribution" << std::endl;
				//double delete_rate = 1 - pow(0.9, iteration_num + 1);
				double delete_rate = (0.5 / iteration_time) * (iteration_num + 1);
				int delete_num = delete_rate * m_solids.Local_V3f.size();
				material_distribution.clear();
				material_distribution.resize(m_solids.Local_BSplineSolid.size(), 1);
				for (int i = 0; i < delete_num; i++)
				{
					material_distribution[sensitivity1[i].cell_id] = 0.0001;
				}
				std::cout << "Update material_distribution completely" << std::endl;
				/*if (delete_rate >= 0.5)
			{
				std::cout << iteration_num << "th iteration's delete_rate: " << delete_rate << " >= " << 0.5 << " break while" << std::endl;
				break;
			}
			else
				std::cout << iteration_num << "th iteration's delete_rate: " << delete_rate << " < " << 0.5 << " continue" << std::endl;
			iteration_num++;*/
			}
			/*-----------------ɾ����-----------------*/
			std::cout << "Start delete" << std::endl;
			int delete_num = m_solids.Local_V3f.size();
			for (int i = 0; i < delete_num; i++)
			{
				if (material_distribution[i] != 1)
				{
					simpmesh->delete_cell(CellHandle(i));
				}
			}
			std::cout << "Delete completely" << std::endl;
			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, simpmesh.get());
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionPDESpareLiner_2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPDESpareLiner_2->setStatusTip(("PDE Sparse SIMP liner"));
	connect(ui.actionPDESpareLiner_2, &QAction::triggered,
		[=]() {
			std::vector<double>().swap(material_distribution);
			material_distribution.resize(mesh->cells_.size(), 1);
			std::vector<double> sensitivity_pre(mesh->cells_.size(), 0);
			int iteration_time = 1;
			int iteration_num = 0;
			std::vector<std::vector<double>> local_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			std::vector<std::vector<int>> m_colids;
			std::vector<std::vector<std::pair<int, int>>> m_bdyquick;
			m_bdycoef.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_V3f[i].size() * 3, 0);
			}
			/*-----���bdybaseid------*/
			qDebug() << "bdybaseid size " << bdybaseid.size();
			/*qDebug() << "bdybaseid size " << bdybaseid.size();
		for(int i = 0;i<bdybaseid.size();i++)
		{
			V3f tempdby = m_solids.Local_V3f[bdybaseid[i].solid_id][bdybaseid[i].local_id];
			qDebug() << tempdby.x << " " << tempdby.y << " " << tempdby.z;
		}*/
			FunctionBSpline fb(m_solids, material_distribution);
			qDebug() << "Start iteration";
			//for (iteration_num = 0; iteration_num < iteration_time; iteration_num++)
			while (true)
			{
				qDebug() << "Schedule of iteration " << iteration_num + 1 << ":" << iteration_time;
				/*-----------------����PDE-----------------*/
				if (iteration_num != 0)
					fb.update_material_distribution(material_distribution);
				/*-----------------����նȾ�����Ҷ���-----------------*/
				qDebug() << "Start constructing stiffness matrix and the right end term";
				std::vector<double> m_bigrhs;
				std::vector<int> rows, cols; //ϡ��洢��Ԫ��������Ϣ
				std::vector<double> elems;   //ϡ��洢��Ԫ����Ϣ
				if (iteration_num == 0)
					fb.Global_MatrixAndRhs_Sparse_liner(m_bdycoef, bdybaseid, local_matrix,
						m_bigrhs, local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
				else
					fb.Global_MatrixAndRhs_Sparse_liner_quick(m_bdycoef, bdybaseid, local_matrix, m_bigrhs,
						local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
				qDebug() << "Processing right item completely";
				/*-----------------������ͷ�����-----------------*/
				qDebug() << "Start solving linear equations";
				std::vector<double> m_coef;
				fb.SolveLinearEquation(-2, m_solids.Global_size * 3, rows, cols, elems, m_bigrhs, m_coef);
				for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
					for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
						for (int k = 0; k < 3; k++)
							m_bdycoef[Local_V3f_id][3 * v_id + k] = m_coef[3 * m_solids.Local_to_Global[Local_V3f_id][v_id] + k];
				qDebug() << "Solving linear equations completely";
				/*-----------------�����ȷ���-----------------*/
				qDebug() << "Start sensitivity analysis";
				std::vector<simpsort> sensitivity;
				sensitivity.resize(m_solids.Local_V3f.size());
				std::vector<double> temp_sen(m_solids.Local_V3f.size());
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					if (material_distribution[i] != 1)
						temp_sen[i] = 0;
					else
						temp_sen[i] = fb.uTKu(m_bdycoef[i], local_matrix[i]) / 2;
					//qDebug() << "ith sensitivity " << temp_sen[i];
				}
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					double rmin = 0;
					std::set<CellHandle> c_c = mesh->neighbor_c_c(CellHandle(i));
					for (std::set<CellHandle>::iterator c_c_it = c_c.begin(); c_c_it != c_c.end(); c_c_it++)
					{
						rmin = rmin + mesh->cell_r[i] + mesh->cell_r[*c_c_it];
					}
					rmin = rmin / c_c.size();
					rmin = rmin * 2;
					double weight = 0;
					double temp_senitiv = 0;
					for (int k = 0; k < m_solids.Local_V3f.size(); k++)
					{
						double rdis = sqrt(pow(mesh->cell_center[i].x - mesh->cell_center[k].x, 2) + pow(mesh->cell_center[i].y - mesh->cell_center[k].y, 2) + pow(mesh->cell_center[i].z - mesh->cell_center[k].z, 2));
						if (rdis < rmin)
						{
							weight += rmin - rdis;
							temp_senitiv += temp_sen[k] * (rmin - rdis);
						}
					}
					temp_senitiv /= weight;
					if (iteration_num == 0) //��һ�εĻ������Լ�
						sensitivity[i].senitiv = temp_senitiv;
					else
						sensitivity[i].senitiv = (temp_senitiv + sensitivity_pre[i]) / 2;
					sensitivity_pre[i] = sensitivity[i].senitiv;
					sensitivity[i].cell_id = i;
				}
				sensitivity1 = sensitivity;
				simsor();
				qDebug() << "Sensitivity analysis completely";
				qDebug() << "Start update material_distribution";
				double delete_rate = 1 - pow(0.98, iteration_num + 1);
				//double delete_rate = (0.5 / iteration_time)*(iteration_num + 1);
				int delete_num = delete_rate * m_solids.Local_V3f.size();
				material_distribution.clear();
				material_distribution.resize(m_solids.Local_BSplineSolid.size(), 1);
				for (int i = 0; i < delete_num; i++)
				{
					material_distribution[sensitivity1[i].cell_id] = 0.0001;
				}
				qDebug() << "Update material_distribution completely";
				if (delete_rate >= 0.5)
				{
					qDebug() << iteration_num << "th iteration's delete_rate: " << delete_rate << " >= " << 0.5 << " break while";
					break;
				}
				else
					qDebug() << iteration_num << "th iteration's delete_rate: " << delete_rate << " < " << 0.5 << " continue";
				iteration_num++;
			}
			/*-----------------ɾ����-----------------*/
			qDebug() << "Start delete";
			int delete_num = m_solids.Local_V3f.size();
			for (int i = 0; i < delete_num; i++)
			{
				if (material_distribution[i] != 1 && std::find(pickcell.begin(), pickcell.end(), CellHandle(i)) == pickcell.end())
				{
					simpmesh->delete_cell(CellHandle(i));
				}
			}
			qDebug() << "Delete completely";
			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, simpmesh.get());
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionPDELinerShow->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPDELinerShow->setStatusTip(("PDE Sparse SIMP liner"));
	connect(ui.actionPDELinerShow, &QAction::triggered,
		[=]() {
			std::vector<double>().swap(material_distribution);
			material_distribution.resize(mesh->cells_.size(), 1);
			std::vector<double> sensitivity_pre(mesh->cells_.size(), 0);
			std::vector<std::vector<double>> local_bigrhs;
			std::vector<Matrix> local_matrix;
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			std::vector<std::vector<int>> m_colids;
			std::vector<std::vector<std::pair<int, int>>> m_bdyquick;
			m_bdycoef.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_V3f[i].size() * 3, 0);
			}
			/*--------------------brige--------------------*/
			int f1n = 0, f2n = 0;
			for (int i = 0; i < mesh->faces_.size(); ++i)
			{
				if (mesh->is_bdy(FaceHandle(i)))
				{
					V3f fv = mesh->cal_face_center(FaceHandle(i));
					if (fv.y == 0.3)
					{
						f1n++;
						pickface.push_back(FaceHandle(i));
					}
				}
			}
			std::cout << "f1n: " << f1n << std::endl;
			for (int i = 0; i < mesh->faces_.size(); ++i)
			{
				if (mesh->is_bdy(FaceHandle(i)))
				{
					V3f fv = mesh->cal_face_center(FaceHandle(i));
					if (fv.y < 0)
					{
						f2n++;
						pickface.push_back(FaceHandle(i));
					}
				}
			}
			std::cout << "f2n: " << f2n << std::endl;
			std::cout << "pickface size :" << pickface.size() << std::endl;
			HexV3fMesh *newmesh1 = new HexV3fMesh();
			ApproximationCC AppCC1(mesh, newmesh1);
			std::vector<V3f> chosev;
			std::vector<VertexHandle> tempcheckv;
			std::vector<EdgeHandle> tempchecke;
			std::vector<FaceHandle> tempcheckf;
			std::vector<CellHandle> tempcheckc;
			int fvn = 0;
			for (int i = 0; i < pickface.size(); i++)
			{
				FaceHandle f = pickface[i];
				for (int v_num = 0; v_num < 4; v_num++)
				{
					chosev.push_back(AppCC1.solid_face_point(f, mesh->faces_[f].vs_f[v_num]));
					if (std::find(tempcheckv.begin(), tempcheckv.end(), mesh->faces_[f].vs_f[v_num]) == tempcheckv.end())
					{
						chosev.push_back(AppCC1.solid_corner_point(mesh->faces_[f].vs_f[v_num]));
						tempcheckv.push_back(mesh->faces_[f].vs_f[v_num]);
					}
				}
				for (int e_num = 0; e_num < 4; e_num++)
				{
					EdgeHandle e = mesh->faces_[f].edges_[e_num];
					if (std::find(tempchecke.begin(), tempchecke.end(), e) == tempchecke.end())
					{
						chosev.push_back(AppCC1.solid_edge_point(e, mesh->edges_[e].from_h));
						chosev.push_back(AppCC1.solid_edge_point(e, mesh->edges_[e].to_h));
						tempchecke.push_back(e);
					}
				}
				if (i == f1n - 1)
					fvn = chosev.size();
			}
			for (int cen_i = 0; cen_i < chosev.size(); cen_i++)
			{
				V3f m_glo_cen1 = chosev[cen_i];
				int mindis_id = 0;
				double min_dis = 999;
				int mindis_cellid = 0;
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					for (int j = 0; j < m_solids.Local_V3f[i].size(); j++)
					{
						V3f temp_v = m_solids.Local_V3f[i][j];
						double dis = distance(temp_v, m_glo_cen1);
						if (dis < min_dis)
						{
							min_dis = dis;
							mindis_id = j;
							mindis_cellid = i;
						}
					}
				}
				if (cen_i < fvn)
				{
					m_bdycoef[mindis_cellid][3 * mindis_id + 1] = -1000; //Y����
				}
				else
				{
					m_bdycoef[mindis_cellid][3 * mindis_id] = 0;     //X����
					m_bdycoef[mindis_cellid][3 * mindis_id + 1] = 0; //Y����
					m_bdycoef[mindis_cellid][3 * mindis_id + 2] = 0; //Z����
				}
				bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
			}
			/*-----���bdybaseid------*/
			qDebug() << "bdybaseid size " << bdybaseid.size();
			/*qDebug() << "bdybaseid size " << bdybaseid.size();
		for(int i = 0;i<bdybaseid.size();i++)
		{
			V3f tempdby = m_solids.Local_V3f[bdybaseid[i].solid_id][bdybaseid[i].local_id];
			qDebug() << tempdby.x << " " << tempdby.y << " " << tempdby.z;
		}*/
			FunctionBSpline fb(m_solids, material_distribution);
			fb.subnum = fvn;
			//fb.subnum = tttt;
			//std::cout << "tttt:"<<tttt << std::endl;
			fb.forceorder = 1;
			qDebug() << "Start iteration";
			/*-----------------����PDE-----------------*/
			/*-----------------����նȾ�����Ҷ���-----------------*/
			qDebug() << "Start constructing stiffness matrix and the right end term";
			std::vector<double> m_bigrhs;
			std::vector<int> rows, cols; //ϡ��洢��Ԫ��������Ϣ
			std::vector<double> elems;   //ϡ��洢��Ԫ����Ϣ
			fb.Global_MatrixAndRhs_Sparse_liner(m_bdycoef, bdybaseid, local_matrix,
				m_bigrhs, local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
			/*FileHandler file("E:\\testbdy1.bdy");
		file.readbdy(m_bdyquick);
		fb.Global_MatrixAndRhs_Sparse_liner1(m_bdycoef, bdybaseid, local_matrix,
			m_bigrhs, local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);*/
			qDebug() << "Processing right item completely";
			/*-----------------������ͷ�����-----------------*/
			qDebug() << "Start solving linear equations";
			std::vector<double> m_coef;
			fb.SolveLinearEquation(-2, m_solids.Global_size * 3, rows, cols, elems, m_bigrhs, m_coef);
			for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
				for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
					for (int k = 0; k < 3; k++)
						m_bdycoef[Local_V3f_id][3 * v_id + k] = m_coef[3 * m_solids.Local_to_Global[Local_V3f_id][v_id] + k];
			qDebug() << "Solving linear equations completely";
			std::vector<std::vector<double>> m_bdycoef_x, m_bdycoef_y, m_bdycoef_z;
			m_bdycoef_x.resize(m_solids.Local_V3f.size());
			m_bdycoef_y.resize(m_solids.Local_V3f.size());
			m_bdycoef_z.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef_x[i].resize(m_solids.Local_V3f[i].size(), 0);
				m_bdycoef_y[i].resize(m_solids.Local_V3f[i].size(), 0);
				m_bdycoef_z[i].resize(m_solids.Local_V3f[i].size(), 0);
			}
			for (int i = 0; i < m_solids.Local_V3f.size(); ++i)
			{
				for (int j = 0; j < m_solids.Local_V3f[i].size(); ++j)
				{
					m_bdycoef_x[i][j] = m_bdycoef[i][3 * j];
					m_bdycoef_y[i][j] = m_bdycoef[i][3 * j + 1];
					m_bdycoef_z[i][j] = m_bdycoef[i][3 * j + 2];
				}
			}
			std::vector<double> m_color;
			*mesh = constructBezierSolid1(m_bdycoef_y, m_color);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->set_color(m_color);
			ui.widget->setdraw_mode_(Error);
			update();
		});

	ui.actionOpen_Model->setIcon(QIcon("Images/Open.png"));
	ui.actionOpen_Model->setStatusTip(("Open a Model of God He"));
	connect(ui.actionOpen_Model, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "VTK(*.vtk)");
			mesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readmodelofGodHe(mesh);
			qDebug() << mesh->cells_.size();
			material_distribution.resize(mesh->cells_.size(), 1);
			/*for (int i = 0; i < mesh->vertices_.size(); ++i)
		{
			if (mesh->vertices_[i].y == 5)
				mesh->vertices_[i].y -= 0.06;
			else if(mesh->vertices_[i].y == 4)
				mesh->vertices_[i].y += 0.06;
		}*/
			mesh->examine_bdy();
			mesh->examine_cs();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			mesh->cal_oriv();
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->setmesh(mesh);
			ui.widget->setdraw_mode_(FLAT_LINES);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
			ui.widget->setoriv(mesh->oriv);

			mesh1.reset(new HexV3fMesh());
			file.readmodelofGodHe(mesh1.get());
			mesh1->examine_bdy();
			mesh1->examine_cs();
			mesh1->cal_cen();
			mesh1->examine_bdfv();
			mesh1->cal_cell_cen();
			mesh1->cal_bdnorm();
			mesh1->cal_mami_ed();
			mesh1->cal_oriv();
			ui.widget->setmesh1(mesh1);
			ui.widget->setbffv1(mesh1->boundary_face_v, mesh1->normal_boundary_face_v, mesh1->boundary_face, mesh1->boundary_face_VID);
		});

	ui.actionShow_two_mesh->setIcon(QIcon("Images/all_poly.png"));
	ui.actionShow_two_mesh->setStatusTip(("Show two mesh"));
	connect(ui.actionShow_two_mesh, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Mesh(*.mesh)");
			mesh1.reset(new HexV3fMesh());
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.read2HexMeshObj(mesh1.get(), true, true, true);
			mesh1->examine_bdy();
			mesh1->examine_cs();
			mesh1->cal_cen();
			mesh1->examine_bdfv();
			mesh1->cal_cell_cen();
			mesh1->cal_bdnorm();
			mesh1->cal_mami_ed();
			mesh1->cal_oriv();
			ui.widget->setbffv1(mesh1->boundary_face_v, mesh1->normal_boundary_face_v, mesh1->boundary_face, mesh1->boundary_face_VID);
			ui.widget->setmesh1(mesh1);
			ui.widget->setdraw_mode_(SHOWTWOMESH);
			update();
		});

	ui.actionShow_two_mesh_singular->setIcon(QIcon("Images/all_poly.png"));
	ui.actionShow_two_mesh_singular->setStatusTip(("Show two mesh"));
	connect(ui.actionShow_two_mesh_singular, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(SHOWTWOMESH1);
			update();
		});

	ui.actionCheckCell->setIcon(QIcon("Images/Open.png"));
	ui.actionCheckCell->setStatusTip(("Check cell"));
	connect(ui.actionCheckCell, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Mesh(*.mesh)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			mesh1.reset(new HexV3fMesh());
			file.read2HexMeshObj(mesh1.get(), true, true, true);
			mesh1->examine_bdy();
			mesh1->examine_cs();
			mesh1->cal_cen();
			mesh1->examine_bdfv();
			mesh1->cal_cell_cen();
			mesh1->cal_bdnorm();
			mesh1->cal_mami_ed();
			mesh1->cal_oriv();
			std::vector<V3f> cell_center = mesh->cell_center;
			std::vector<V3f> cell_center1 = mesh1->cell_center;
			for (int i = 0; i < cell_center.size(); ++i)
			{
				if (std::find(cell_center1.begin(), cell_center1.end(), cell_center[i]) != cell_center1.end()
					/*&& !mesh->is_bdy(CellHandle(i))*/)
					checkcell[i] = true;
			}
			ui.widget->setcheckcell(checkcell);
		});

	ui.actionSingularFace->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionSingularFace->setStatusTip(("Draw Bei Sur"));
	connect(ui.actionSingularFace, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(SINGULARFACE);
		});

	ui.actionTisu->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionTisu->setStatusTip(("Draw Tisu"));
	connect(ui.actionTisu, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(TISU);
		});

	ui.actionDrawFrame->setIcon(QIcon("Images/all_poly.png"));
	ui.actionDrawFrame->setStatusTip(("Show Frame"));
	connect(ui.actionDrawFrame, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Mesh(*.mesh)");
			HexV3fMesh *meshtest = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.read2HexMeshObj(meshtest, true, true, true);
			meshtest->examine_bdy();
			meshtest->choseframe();
			meshtest->choseframeface();
			ui.widget->setframe(meshtest->frame, meshtest->frameface, meshtest->framefacenrom);
			ui.widget->setdraw_mode_(ADDFRAME);
			update();
		});

	ui.actionFrameTisu->setIcon(QIcon("Images/all_poly.png"));
	ui.actionFrameTisu->setStatusTip(("Show Frame"));
	connect(ui.actionFrameTisu, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(FRAMETISU);
			update();
		});

	ui.actionDrameFrame1->setIcon(QIcon("Images/all_poly.png"));
	ui.actionDrameFrame1->setStatusTip(("Show Frame 1"));
	connect(ui.actionDrameFrame1, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Mesh(*.mesh)");
			HexV3fMesh *meshtest = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.read2HexMeshObj(meshtest, true, true, true);
			meshtest->examine_bdy();
			meshtest->choseframe();
			meshtest->choseframeface();
			ui.widget->setframe(meshtest->frame, meshtest->frameface, meshtest->framefacenrom);
			ui.widget->setdraw_mode_(ADDFRAME1);
			update();
		});

	ui.actionFrame->setIcon(QIcon("Images/all_poly.png"));
	ui.actionFrame->setStatusTip(("Show Frame 1"));
	connect(ui.actionFrame, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ADDFRAME1);
			update();
		});

	ui.actionFrame_2->setIcon(QIcon("Images/all_poly.png"));
	ui.actionFrame_2->setStatusTip(("Show Frame 1"));
	connect(ui.actionFrame_2, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(ADDFRAME);
			update();
		});

	ui.actionErrorAndFrame->setIcon(QIcon("Images/all_poly.png"));
	ui.actionErrorAndFrame->setStatusTip(("Show Frame 1"));
	connect(ui.actionErrorAndFrame, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Mesh(*.mesh)");
			HexV3fMesh *meshtest = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			//file.read2HexMeshObj(meshtest, true, true, true);
			file.read2HexMeshObj_new(meshtest);
			meshtest->examine_bdy();
			meshtest->choseframe();
			meshtest->choseframeface();
			ui.widget->setframe(meshtest->frame, meshtest->frameface, meshtest->framefacenrom);
			ui.widget->setdraw_mode_(ERRORFRAME);
			update();
		});

	ui.actionPDESquareLiner_Muti->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionPDESquareLiner_Muti->setStatusTip(("PDE Sparse SIMP liner mutires"));
	connect(ui.actionPDESquareLiner_Muti, &QAction::triggered,
		[=]() {
			std::vector<double>().swap(material_distribution);
			material_distribution.resize(mesh->cells_.size(), 1);

			HexV3fMesh *nextsubmesh = new HexV3fMesh();
			CCSubdivision sub(mesh, nextsubmesh, material_distribution);
			*nextsubmesh = sub.CCSubdivising();
			material_distribution_mutires = sub.material_distribution;
			nextsubmesh->examine_bdy();
			nextsubmesh->cal_cell_cen();

			std::vector<double> sensitivity_pre(nextsubmesh->cells_.size(), 0);
			int iteration_time = 20;
			int iteration_num = 0;
			std::vector<std::vector<std::vector<double>>> local_bigrhs;
			std::vector<std::vector<Matrix>> local_matrixs;
			std::vector<std::vector<double>> m_bdycoef;
			std::vector<BdyBase> bdybaseid;
			std::vector<std::vector<std::pair<int, int>>> m_bdyquick;
			std::vector<std::vector<V3f>> centers;
			std::vector<std::vector<int>> m_colids;
			m_bdycoef.resize(m_solids.Local_V3f.size());
			for (int i = 0; i < m_solids.Local_V3f.size(); i++)
			{
				m_bdycoef[i].resize(m_solids.Local_V3f[i].size() * 3, 0);
			}
			/*-----liner_simple------*/
			//for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			//{
			//	for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
			//	{
			//		V3f tempv = m_solids.Local_V3f[Local_V3f_id][v_id];
			//		if (m_solids.local_bdy[Local_V3f_id][v_id])
			//		{
			//			if (tempv.y == 4 && tempv.x == 0)
			//			{
			//				m_bdycoef[Local_V3f_id][3 * v_id + 1] = 10;//Y����
			//				bool check = true;
			//				for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
			//				{
			//					if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
			//					{
			//						check = false;
			//						break;
			//					}
			//				}
			//				if (check == true)
			//					bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
			//			}
			//		}
			//	}
			//}
			//for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
			//{
			//	for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
			//	{
			//		V3f tempv = m_solids.Local_V3f[Local_V3f_id][v_id];
			//		if (m_solids.local_bdy[Local_V3f_id][v_id])
			//		{
			//			if (tempv.x == 9)
			//			{
			//				m_bdycoef[Local_V3f_id][3 * v_id] = 0;//X����
			//				m_bdycoef[Local_V3f_id][3 * v_id + 1] = 0;//Y����
			//				m_bdycoef[Local_V3f_id][3 * v_id + 2] = 0;//Z����
			//				bool check = true;
			//				for (std::vector<BdyBase>::iterator bdybaseid_id = bdybaseid.begin(); bdybaseid_id != bdybaseid.end(); bdybaseid_id++)
			//				{
			//					if ((*bdybaseid_id).global_id == m_solids.Local_to_Global[Local_V3f_id][v_id])
			//					{
			//						check = false;
			//						break;
			//					}
			//				}
			//				if (check == true)
			//					bdybaseid.push_back(BdyBase(v_id, m_solids.Local_to_Global[Local_V3f_id][v_id], Local_V3f_id));
			//			}
			//		}
			//	}
			//}
			/*-----liner_complex------*/
			HexV3fMesh *newmesh1 = new HexV3fMesh();
			ApproximationCC AppCC1(mesh, newmesh1);
			std::vector<V3f> chosev;
			std::vector<VertexHandle> tempcheckv;
			std::vector<EdgeHandle> tempchecke;
			std::vector<FaceHandle> tempcheckf;
			std::vector<CellHandle> tempcheckc;
			pickedge = ui.widget->pickedge_e;
			for (int i = 0; i < pickedge.size(); i++)
			{
				EdgeHandle e = pickedge[i];
				chosev.push_back(AppCC1.solid_edge_point(e, mesh->edges_[e].from_h));
				chosev.push_back(AppCC1.solid_edge_point(e, mesh->edges_[e].to_h));
				if (std::find(tempcheckv.begin(), tempcheckv.end(), mesh->edges_[e].from_h) == tempcheckv.end())
				{
					chosev.push_back(AppCC1.solid_corner_point(mesh->edges_[e].from_h));
					tempcheckv.push_back(mesh->edges_[e].from_h);
				}
				if (std::find(tempcheckv.begin(), tempcheckv.end(), mesh->edges_[e].to_h) == tempcheckv.end())
				{
					chosev.push_back(AppCC1.solid_corner_point(mesh->edges_[e].to_h));
					tempcheckv.push_back(mesh->edges_[e].to_h);
				}
			}
			pickface = ui.widget->pickface;
			for (int i = 0; i < pickface.size(); i++)
			{
				std::cout << "Schedule of pickface " << i + 1 << ":" << pickface.size() << std::endl;
				FaceHandle f = pickface[i];
				for (int v_num = 0; v_num < 4; v_num++)
				{
					chosev.push_back(AppCC1.solid_face_point(f, mesh->faces_[f].vs_f[v_num]));
					if (std::find(tempcheckv.begin(), tempcheckv.end(), mesh->faces_[f].vs_f[v_num]) == tempcheckv.end())
					{
						chosev.push_back(AppCC1.solid_corner_point(mesh->faces_[f].vs_f[v_num]));
						tempcheckv.push_back(mesh->faces_[f].vs_f[v_num]);
					}
				}
				for (int e_num = 0; e_num < 4; e_num++)
				{
					EdgeHandle e = mesh->faces_[f].edges_[e_num];
					if (std::find(tempchecke.begin(), tempchecke.end(), e) == tempchecke.end())
					{
						chosev.push_back(AppCC1.solid_edge_point(e, mesh->edges_[e].from_h));
						chosev.push_back(AppCC1.solid_edge_point(e, mesh->edges_[e].to_h));
						tempchecke.push_back(e);
					}
				}
			}
			for (int cen_i = 0; cen_i < chosev.size(); cen_i++)
			{
				V3f m_glo_cen1 = chosev[cen_i];
				int mindis_id = 0;
				double min_dis = 999;
				int mindis_cellid = 0;
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					for (int j = 0; j < m_solids.Local_V3f[i].size(); j++)
					{
						V3f temp_v = m_solids.Local_V3f[i][j];
						double dis = distance(temp_v, m_glo_cen1);
						if (dis < min_dis)
						{
							min_dis = dis;
							mindis_id = j;
							mindis_cellid = i;
						}
					}
				}
				if (cen_i < 13)
					m_bdycoef[mindis_cellid][3 * mindis_id + 1] = -10; //Y����
				else
				{
					m_bdycoef[mindis_cellid][3 * mindis_id] = 0;     //X����
					m_bdycoef[mindis_cellid][3 * mindis_id + 1] = 0; //Y����
					m_bdycoef[mindis_cellid][3 * mindis_id + 2] = 0; //Z����
				}
				bdybaseid.push_back(BdyBase(mindis_id, m_solids.Local_to_Global[mindis_cellid][mindis_id], mindis_cellid));
			}
			qDebug() << "bdybaseid size " << bdybaseid.size();
			/*
			qDebug() << "bdybaseid size " << bdybaseid.size();
			for(int i = 0;i<bdybaseid.size();i++)
			{
				V3f tempdby = m_solids.Local_V3f[bdybaseid[i].solid_id][bdybaseid[i].local_id];
				qDebug() << tempdby.x << " " << tempdby.y << " " << tempdby.z;
			}
		*/
			FunctionBSpline fb(m_solids, material_distribution);
			qDebug() << "Start iteration";
			for (iteration_num = 0; iteration_num < iteration_time; iteration_num++)
				//while (true)
			{
				qDebug() << "Schedule of iteration " << iteration_num + 1 << ":" << iteration_time;
				/*-----------------����PDE-----------------*/
				//fb.material_distribution_mutires = material_distribution_mutires;
				/*-----------------����նȾ�����Ҷ���-----------------*/
				qDebug() << "Start constructing stiffness matrix and the right end term";
				std::vector<double> m_bigrhs;
				std::vector<int> rows, cols; //ϡ��洢��Ԫ��������Ϣ
				std::vector<double> elems;   //ϡ��洢��Ԫ����Ϣ
				if (iteration_num == 0)
					fb.Global_MatrixAndRhs_Sparse_liner_mutires(m_bdycoef, bdybaseid, local_matrixs,
						m_bigrhs, local_bigrhs, rows, cols, elems, m_colids, m_bdyquick, centers);
				else
					fb.Global_MatrixAndRhs_Sparse_liner_mutires_quick(m_bdycoef, bdybaseid, local_matrixs,
						m_bigrhs, local_bigrhs, rows, cols, elems, m_colids, m_bdyquick);
				qDebug() << "Processing right item completely";
				/*-----------------������ͷ�����-----------------*/
				qDebug() << "Start solving linear equations";
				std::vector<double> m_coef;
				fb.SolveLinearEquation(-2, m_solids.Global_size * 3, rows, cols, elems, m_bigrhs, m_coef);
				for (int Local_V3f_id = 0; Local_V3f_id < m_solids.Local_V3f.size(); Local_V3f_id++)
					for (int v_id = 0; v_id < m_solids.Local_V3f[Local_V3f_id].size(); v_id++)
						for (int k = 0; k < 3; k++)
							m_bdycoef[Local_V3f_id][3 * v_id + k] = m_coef[3 * m_solids.Local_to_Global[Local_V3f_id][v_id] + k];
				qDebug() << "Solving linear equations completely";
				/*-----------------�����ȷ���-----------------*/
				qDebug() << "Start sensitivity analysis";
				std::vector<simpsort> sensitivity;
				sensitivity.resize(nextsubmesh->cells_.size());
				std::vector<double> temp_sen(nextsubmesh->cells_.size());
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					for (int temsen = 0; temsen < 8; ++temsen)
					{
						//�ҵ���Ӧ����±�
						V3f tempcenter = centers[i][temsen];
						int cell_cenid;
						double mindis = 999;
						for (int cell_it = 0; cell_it < nextsubmesh->cells_.size(); ++cell_it)
						{
							V3f temp_cell_center = nextsubmesh->cell_center[cell_it];
							double dis = distance(tempcenter, temp_cell_center);
							if (dis < mindis)
							{
								mindis = dis;
								cell_cenid = cell_it;
							}
						}
						if (material_distribution_mutires[cell_cenid] != 1)
							temp_sen[cell_cenid] = 0;
						else
							temp_sen[cell_cenid] = fb.uTKu(m_bdycoef[i], local_matrixs[i][temsen]) / 2;
					}
				}
				for (int i = 0; i < nextsubmesh->cells_.size(); i++)
				{
					double rmin = 0;
					std::set<CellHandle> c_c = nextsubmesh->neighbor_c_c(CellHandle(i));
					for (std::set<CellHandle>::iterator c_c_it = c_c.begin(); c_c_it != c_c.end(); c_c_it++)
					{
						rmin = rmin + nextsubmesh->cell_r[i] + nextsubmesh->cell_r[*c_c_it];
					}
					rmin = rmin / c_c.size();
					rmin = rmin * 2;
					double weight = 0;
					double temp_senitiv = 0;
					for (int k = 0; k < nextsubmesh->cells_.size(); k++)
					{
						double rdis = distance(nextsubmesh->cell_center[i], nextsubmesh->cell_center[k]);
						if (rdis < rmin)
						{
							weight += rmin - rdis;
							temp_senitiv += temp_sen[k] * (rmin - rdis);
						}
					}
					temp_senitiv /= weight;
					if (iteration_num == 0) //��һ�εĻ������Լ�
						sensitivity[i].senitiv = temp_senitiv;
					else
						sensitivity[i].senitiv = (temp_senitiv + sensitivity_pre[i]) / 2;
					sensitivity_pre[i] = sensitivity[i].senitiv;
					sensitivity[i].cell_id = i;
				}
				sensitivity1 = sensitivity;
				simsor();
				qDebug() << "Sensitivity analysis completely";
				qDebug() << "Start update material_distribution";
				//double delete_rate = 1 - pow(0.98, iteration_num + 1);
				double delete_rate = (0.7 / iteration_time) * (iteration_num + 1);
				int delete_num = delete_rate * nextsubmesh->cells_.size();
				material_distribution_mutires.clear();
				material_distribution_mutires.resize(nextsubmesh->cells_.size(), 1);
				for (int i = 0; i < delete_num; i++)
				{
					material_distribution_mutires[sensitivity1[i].cell_id] = 0.0001;
				}
				//�������նȾ�����Ҫ������������
				std::vector<std::vector<double>> mdu(m_solids.Local_V3f.size());
				for (int i = 0; i < m_solids.Local_V3f.size(); i++)
				{
					mdu[i].resize(8);
					for (int temsen = 0; temsen < 8; ++temsen)
					{
						//�ҵ���Ӧ����±�
						V3f tempcenter = centers[i][temsen];
						int cell_cenid;
						double mindis = 999;
						for (int cell_it = 0; cell_it < nextsubmesh->cells_.size(); ++cell_it)
						{
							V3f temp_cell_center = nextsubmesh->cell_center[cell_it];
							double dis = distance(tempcenter, temp_cell_center);
							if (dis < mindis)
							{
								mindis = dis;
								cell_cenid = cell_it;
							}
						}
						mdu[i][temsen] = material_distribution_mutires[cell_cenid];
					}
				}
				fb.material_distribution_mutires = mdu;
				qDebug() << "Update material_distribution completely";
				/*if (delete_rate >= 0.5)
			{
				qDebug() << iteration_num << "th iteration's delete_rate: " << delete_rate << " >= " << 0.5 << " break while";
				break;
			}
			else
				qDebug() << iteration_num << "th iteration's delete_rate: " << delete_rate << " < " << 0.5 << " continue";
			iteration_num++;*/
			}
			/*-----------------ɾ����-----------------*/
			qDebug() << "Start delete";
			std::vector<CellHandle> delete_cell;
			for (int i = 0; i < nextsubmesh->cells_.size(); i++)
			{
				if (material_distribution_mutires[i] != 1 && std::find(pickcell.begin(), pickcell.end(), CellHandle(i)) == pickcell.end())
				{
					delete_cell.push_back(CellHandle(i));
				}
			}
			for (int i = 0; i < delete_cell.size(); ++i)
				nextsubmesh->delete_cell(delete_cell[i]);
			qDebug() << "Delete completely";
			FileHandler filenew = FileHandler();
			std::string name("E://delete.mesh");
			filenew.writeFile(name, nextsubmesh);
			mesh = new HexV3fMesh();
			FileHandler file("E://delete.mesh");
			file.read2HexMeshObj(mesh, true, true, true);
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			ui.widget->setdraw_mode_(FLAT_LINES);
			update();
		});

	ui.actionSphereDraw->setIcon(QIcon("Images/all_poly.png"));
	ui.actionSphereDraw->setStatusTip(("Show Frame 1"));
	connect(ui.actionSphereDraw, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Vtk(*.vtk)");
			HexV3fMesh *testmesh = new HexV3fMesh();
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readVtk2HexMeshObj(testmesh);
			testmesh->examine_bdy();
			testmesh->examine_cs();
			testmesh->cal_cen();
			testmesh->examine_bdfv();
			testmesh->cal_cell_cen();
			testmesh->cal_bdnorm();
			testmesh->cal_mami_ed();
			testmesh->cal_oriv();
			ui.widget->setbffv1(testmesh->boundary_face_v, testmesh->normal_boundary_face_v, testmesh->boundary_face, testmesh->boundary_face_VID);
			ui.widget->setmesh1(testmesh);
			ui.widget->setdraw_mode_(SPHERE);
			update();
		});

	ui.actionGrab->setIcon(QIcon("Images/all_poly.png"));
	ui.actionGrab->setStatusTip(("Show Frame 1"));
	connect(ui.actionGrab, &QAction::triggered,
		[=]() {
			//ui.widget->grab1();
			//QRect rect = ui.widget->geometry();
			//QPixmap p = ui.widget_3->grab(rect);
			QPixmap p = QPixmap::grabWidget(ui.widget);
			QString filePathName = "widget";
			filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
			filePathName += ".png";
			if (!p.save(filePathName, "png"))
			{
				std::cout << "save widget screen failed" << std::endl;
			}
		});

	ui.actionReadLine->setIcon(QIcon("Images/Open.png"));
	ui.actionReadLine->setStatusTip(("Read Picked All"));
	connect(ui.actionReadLine, &QAction::triggered,
		[=]() {
			linecheck.swap(std::vector<double>());
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "TXT(*.txt)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			file.readline(linecheck);
		});

	ui.actionOutputline->setIcon(QIcon("Images/Open.png"));
	ui.actionOutputline->setStatusTip(("Read Picked All"));
	connect(ui.actionOutputline, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "TXT(*.txt)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				//filenew.writeline(name, linecheck);
				filenew.writev3fline(name, mesh, ui.widget->pickpoint_v, ui.widget->pickpoint_vtoline);
			}
		});

	ui.actionSmooth->setStatusTip(("Smooth mesh"));
	connect(ui.actionSmooth, &QAction::triggered,
		[=]() {
			//mesh->v_cs.swap(std::vector<bool>(mesh->vertices_.size(), false));
			//for (int i = 0; i < mesh->vertices_.size(); ++i)
			//{
			//	V3f v = mesh->vertices_[i];
			//	/*if (std::find(csv3f.begin(), csv3f.end(), v) != csv3f.end())
			//		mesh->v_cs[i] = true;*/
			//	/*if (v.x == 0)
			//		mesh->v_cs[i] = true;*/
			//	/*if (v.y == 220 || v.x == 100 || v.x == -220 || v.y == -100)
			//		mesh->v_cs[i] = true;*/
			//	if (v.y < -0.0025)
			//		mesh->v_cs[i] = true;
			//}
			//for (int i = 0; i < mesh->edges_.size(); ++i)
			//{
			//	if (mesh->is_bdy(EdgeHandle(i)))
			//	{
			//		V3f v = mesh->cal_edge_mid(EdgeHandle(i));
			//		/*double min = 999;
			//		for (int i = 0; i < edgemid.size(); ++i)
			//		{
			//			double dis = distance(v, edgemid[i]);
			//			if (dis < min)
			//				min = dis;
			//		}
			//		if (min < 1.0e-5)
			//		{
			//			mesh->v_cs[mesh->edges_[i].from_h] = true;
			//			mesh->v_cs[mesh->edges_[i].to_h] = true;
			//		}*/
			//		if (v.x == 0 && mesh->neighbor_e[i].size() == 2)
			//		{
			//			mesh->v_cs[mesh->edges_[i].from_h] = true;
			//			mesh->v_cs[mesh->edges_[i].to_h] = true;
			//			ui.widget->pickedge_e.push_back(EdgeHandle(i));
			//		}
			//	}
			//}
			/*mesh->smooth_mesh();
			mesh->smooth_mesh();
			mesh->smooth_mesh();
			mesh->smooth_mesh();
			mesh->smooth_mesh();*/
			mesh->smooth_mesh();
			mesh->examine_bdy();
			mesh->cal_cen();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			ui.widget->setmesh(mesh);
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			//ui.widget->set_scene_pos(mesh->center, mesh->r);
			ui.widget->setcell_center(mesh->cell_center);
			ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
			ui.widget->setbb(mesh->bbMax, mesh->bbMin);
			update();
		});

	ui.actionBezierLine->setStatusTip(("Bezier Line")); // 没什么用似乎, 显示四边形的??
	connect(ui.actionBezierLine, &QAction::triggered,
		[=]() {
			quad = new QuadV3fMesh();
			*quad = mesh->change2Quad();
			quad->changefe();
			quad->vdraw.resize(quad->vertices_.size(), true);
			quad->v_cs.resize(quad->vertices_.size(), false);
			quad->checkdrawedge();
			ui.widget->setquad(quad, quad->drawedge);
			ui.widget->setdraw_mode_(QUAD);
			update();
		});

	ui.actionQuadCC_Crease->setStatusTip("Quad CC with Crease");
	connect(ui.actionQuadCC_Crease, &QAction::triggered,
		[=]() {
			QuadV3fMesh *newmesh = new QuadV3fMesh();
			QuadCC sub(quad, newmesh, quad->v_cs, quad->vdraw);
			*quad = sub.CCSubWithCrease();
			quad->changefe();
			quad->v_cs = sub.v_cs;
			quad->vdraw = sub.vdraw;
			quad->checkdrawedge();
			quad->cal_norm();
			ui.widget->setquad(quad, quad->drawedge);
			ui.widget->setdraw_mode_(QUAD);
			update();
		});

	ui.actionQuadV->setStatusTip("Quad Point");
	connect(ui.actionQuadV, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(QUADV);
			update();
		});

	ui.actionSetQuad1->setStatusTip("Set Another Quad");
	connect(ui.actionSetQuad1, &QAction::triggered,
		[=]() {
			ui.widget->setquad1(quad);
			//ui.widget->setsurv1(bei_surv, nor_bei_surv);
			isneed = false;
			update();
		});

	ui.actionSetFrame->setStatusTip("Set Frame");
	connect(ui.actionSetFrame, &QAction::triggered,
		[=]() {
			//ui.widget->setquadframe(quad);
			ui.widget->setsurvframe(bei_surv, nor_bei_surv);
			vframe.push_back({ bei_surv, nor_bei_surv });
			update();
		});

	ui.actionShowTwoQuad->setStatusTip("Show two Quad");
	connect(ui.actionShowTwoQuad, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(TWOQUAD);
			update();
		});

	ui.actionShowQuad->setStatusTip("Show two Quad");
	connect(ui.actionShowQuad, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(QUAD);
			update();
		});

	ui.actionShowFrame->setStatusTip("Show two Quad Frame");
	connect(ui.actionShowFrame, &QAction::triggered,
		[=]() {
			ui.widget->setdraw_mode_(TWOQUADFRAME);
			update();
		});

	ui.actionPick_curve->setStatusTip("Pick curve");
	connect(ui.actionPick_curve, &QAction::triggered,
		[=]() {
			std::vector<std::vector<EdgeHandle>> curve = ui.widget->curve;
			pickvertex = ui.widget->pickpoint_v;
			curve_v.clear();
			for (int i = 0; i < curve.size(); ++i)
			{
				std::vector<VertexHandle> tempv;
				tempv.push_back(pickvertex[i]);
				VertexHandle v;
				if (mesh->edges_[curve[i][0]].from_h == pickvertex[i])
					v = mesh->edges_[curve[i][0]].to_h;
				else
					v = mesh->edges_[curve[i][0]].from_h;
				for (int j = 1; j < curve[i].size(); ++j)
				{
					EdgeHandle e = curve[i][j];
					if (mesh->edges_[e].from_h == v)
					{
						tempv.push_back(mesh->edges_[e].from_h);
						v = mesh->edges_[e].to_h;
					}
					else
					{
						tempv.push_back(mesh->edges_[e].to_h);
						v = mesh->edges_[e].from_h;
					}
					/*if (std::find(tempv.begin(), tempv.end(), mesh->edges_[e].from_h) == tempv.end())
					tempv.push_back(mesh->edges_[e].from_h);
				if (std::find(tempv.begin(), tempv.end(), mesh->edges_[e].to_h) == tempv.end())
					tempv.push_back(mesh->edges_[e].to_h);*/
				}
				std::vector<V3f> tempv3f(tempv.size());
				for (int j = 0; j < tempv3f.size(); ++j)
				{
					tempv3f[j] = mesh->vertices_[tempv[j]];
				}
				int subnum = 3;
				for (int subtime = 0; subtime < subnum; ++subtime)
				{
					int v3fnum = tempv3f.size();
					std::vector<V3f> newpoint(2 * v3fnum, V3f(0, 0, 0));
					for (int newpointi = 0; newpointi < newpoint.size(); newpointi++)
					{
						if (newpointi % 2 == 0)
						{
							int id = newpointi / 2;
							newpoint[newpointi] = tempv3f[id] * 3 / 4 + tempv3f[(id + 1) % v3fnum] * 1 / 8 +
								tempv3f[(id - 1 + v3fnum) % v3fnum] * 1 / 8;
						}
						else
						{
							int id = (newpointi - 1) / 2;
							newpoint[newpointi] = (tempv3f[id] + tempv3f[(id + 1) % v3fnum]) / 2;
						}
					}
					tempv3f = newpoint;
				}
				curve_v.push_back(tempv3f);
			}
			ui.widget->setcurve_v(curve_v);
			ui.widget->setdraw_mode_(CURVE);
			update();
		});

	ui.actionOutput_curve->setIcon(QIcon("Images/Save.png"));
	ui.actionOutput_curve->setStatusTip(("Output Curve"));
	connect(ui.actionOutput_curve, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getSaveFileName(this, "save", "../", "PPV(*.ppv)");
			if (path.isEmpty() == false)
			{
				std::string name = path.toStdString();
				FileHandler filenew = FileHandler();
				filenew.writepickedpoint(name, curve_v);
			}
		});

	ui.actionRead_curve->setIcon(QIcon("Images/Open.png"));
	ui.actionRead_curve->setStatusTip(("Read Curve"));
	connect(ui.actionRead_curve, &QAction::triggered,
		[=]() {
			QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "PPV(*.ppv)");
			std::string stpath = path.toStdString();
			FileHandler file(stpath);
			curve_v.clear();
			file.readpickedpoint(curve_v);
			ui.widget->setcurve_v(curve_v);
			ui.widget->setdraw_mode_(CURVE);
			update();
		});

	ui.actionBezierBDY->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionBezierBDY->setStatusTip(("Create Bezier with Sharp"));
	connect(ui.actionBezierBDY, &QAction::triggered,
		[=]() {
			std::vector<FaceHandle> sharpface; //��������ߵ���
			std::vector<int> sharpnum;         //��������ڵڼ�������
			std::vector<std::vector<EdgeHandle>> curve = ui.widget->curve;
			for (int i = 0; i < curve.size(); ++i)
			{
				for (int j = 0; j < curve[i].size(); ++j)
				{
					EdgeHandle edge = curve[i][j];
					std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[edge];
					for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ++ef_it)
					{
						if (mesh->is_bdy(*ef_it))
						{
							sharpface.push_back(*ef_it);
							sharpnum.push_back(i);
						}
					}
				}
			}
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, curve_v, sharpface, sharpnum);
			AppCC.approximationing();
			AppCC.constructBezierSharp();
			AppCC.cal_nor_beisur(0.1);
			AppCC.cal_beisurv_center(1);
			mesh->checksingualrface();
			ui.widget->setbiaojiface(AppCC.biaojiface);
			ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
			ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
			ui.widget->setedgeline(AppCC.edgeline);
			ui.widget->setsingularface(mesh->singularface);
			ui.widget->setdraw_mode_(BEISUDAT1);
			update();
		});

	ui.actionBezierSharp->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionBezierSharp->setStatusTip(("Create Bezier1"));
	connect(ui.actionBezierSharp, &QAction::triggered,
		[=]() {
			ui.widget->pickedge_e.swap(std::vector<EdgeHandle>());
			for (int i = 0; i < mesh->edges_.size(); ++i)
			{
				if (mesh->is_bdy(EdgeHandle(i)))
				{
					V3f v = mesh->cal_edge_mid(EdgeHandle(i));
					//if ((v.y == 220 || v.x == 100 || v.x == -220 || v.y == -100) && mesh->neighbor_e[i].size() == 2)
					//ui.widget->pickedge_e.push_back(EdgeHandle(i));
					/*double min = 999;
				for (int i = 0; i < edgemid.size(); ++i)
				{
					double dis = distance(v, edgemid[i]);
					if (dis < min)
						min = dis;
				}
				if (min < 1.0e-5)
					ui.widget->pickedge_e.push_back(EdgeHandle(i));*/
					if (v.x == 0 && mesh->neighbor_e[i].size() == 2)
						ui.widget->pickedge_e.push_back(EdgeHandle(i));
				}
			}
			ui.widget->pickedge_e = pickedge1;
			std::cout << ui.widget->pickedge_e.size() << std::endl;
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
			AppCC.approximationingSharp();
			AppCC.constructBezier(1);
			AppCC.cal_nor_beisur(0.1);
			AppCC.cal_beisurv_center(1);
			mesh->checksingualrface();
			if (checkbei == false)
			{
				bei_surv = AppCC.bei_surv_dat;
				nor_bei_surv = AppCC.nor_bei_surv_dat;
				ui.widget->setbiaojiface(AppCC.biaojiface);
				ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
				ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
				ui.widget->setedgeline(AppCC.edgeline);
				ui.widget->setsingularface(mesh->singularface);
				ui.widget->setdraw_mode_(BEISUDAT1);
				checkbei = true;
			}
			else
			{
				bei_surv = AppCC.bei_surv_dat;
				nor_bei_surv = AppCC.nor_bei_surv_dat;
			}
			ui.widget->pickedge_e.clear();
			update();
		});

	ui.actionCheckbei->setStatusTip(("Create Bezier1"));
	connect(ui.actionCheckbei, &QAction::triggered,
		[=]() {
			ui.widget->pickdrawface.clear();
			ui.widget->pickdrawface_center.clear();
			ui.widget->pickedge_e.clear();
			mesh->v_cs.swap(std::vector<bool>(mesh->vertices_.size(), false));
			//for (int i = 0; i < mesh->vertices_.size(); ++i)
			//{
			//	V3f v = mesh->vertices_[i];
			//	if (std::find(csv3f.begin(), csv3f.end(), v) != csv3f.end())
			//		mesh->v_cs[i] = true;
			//	/*if (v.x == 0)
			//		mesh->v_cs[i] = true;*/
			//	/*if (v.y == 220 || v.x == 100 || v.x == -220 || v.y == -100)
			//		mesh->v_cs[i] = true;*/
			//}
			//for (int i = 0; i < mesh->edges_.size(); ++i)
			//{
			//	if (mesh->is_bdy(EdgeHandle(i)))
			//	{
			//		V3f v = mesh->cal_edge_mid(EdgeHandle(i));
			//		/*if ((v.y == 220 || v.x == 100 || v.x == -220 || v.y == -100) && mesh->neighbor_e[i].size() == 2)
			//			ui.widget->pickedge_e.push_back(EdgeHandle(i));*/
			//		/*double min = 999;
			//		for (int i = 0; i < edgemid.size(); ++i)
			//		{
			//			double dis = distance(v, edgemid[i]);
			//			if (dis < min)
			//				min = dis;
			//		}
			//		if (min < 1.0e-5)
			//		{
			//			mesh->v_cs[mesh->edges_[i].from_h] = true;
			//			mesh->v_cs[mesh->edges_[i].to_h] = true;
			//			ui.widget->pickedge_e.push_back(EdgeHandle(i));
			//		}*/
			//		if (v.x == 0 && mesh->neighbor_e[i].size() == 2)
			//		{
			//			mesh->v_cs[mesh->edges_[i].from_h] = true;
			//			mesh->v_cs[mesh->edges_[i].to_h] = true;
			//			ui.widget->pickedge_e.push_back(EdgeHandle(i));
			//		}
			//	}
			//}
			//mesh->smooth_mesh(); mesh->smooth_mesh(); mesh->smooth_mesh();
			//mesh->smooth_mesh(); mesh->smooth_mesh(); mesh->smooth_mesh();
			checkbei = false;
			//ui.widget->pickedge_e = pickedge;
			std::cout << ui.widget->pickedge_e.size() << std::endl;
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
			AppCC.approximationingSharp();
			AppCC.constructBezier(1);
			AppCC.cal_nor_beisur(0.1);
			AppCC.cal_beisurv_center(1);
			mesh->checksingualrface();
			if (checkbei == false)
			{
				bei_surv = AppCC.bei_surv_dat;
				nor_bei_surv = AppCC.nor_bei_surv_dat;
				ui.widget->setbiaojiface(AppCC.biaojiface);
				ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
				ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
				ui.widget->setedgeline(AppCC.edgeline);
				ui.widget->setsingularface(mesh->singularface);
				ui.widget->setdraw_mode_(TWOQUADFRAME);
				checkbei = true;
			}
			else
			{
				bei_surv = AppCC.bei_surv_dat;
				nor_bei_surv = AppCC.nor_bei_surv_dat;
			}
			update();
		});

	ui.actionBezierSharp1->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionBezierSharp1->setStatusTip(("Create Bezier1"));
	connect(ui.actionBezierSharp1, &QAction::triggered,
		[=]() {
			mesh->smooth_mesh();
			mesh->smooth_mesh();
			mesh->smooth_mesh();

			std::cout << ui.widget->pickedge_e.size() << std::endl;
			HexV3fMesh *newmesh = new HexV3fMesh();
			ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
			AppCC.approximationingSharp();
			AppCC.constructBezier(1);
			AppCC.cal_nor_beisur(0.1);
			AppCC.cal_beisurv_center(1);
			mesh->checksingualrface();
			beziersurface bftemp;
			bftemp.biaojiface = AppCC.biaojiface;
			bftemp.bei_surv = AppCC.bei_surv;
			bftemp.bei_surv_dat = AppCC.bei_surv_dat;
			bftemp.tempdraw_num = mesh->faces_.size();
			bftemp.nor_bei_surv = AppCC.nor_bei_surv;
			bftemp.nor_bei_surv_dat = AppCC.nor_bei_surv_dat;
			bftemp.bei_surv_center = AppCC.bei_surv_center;
			bftemp.edgeline = AppCC.edgeline;
			bftemp.singularface = mesh->singularface;
			bf.push_back(bftemp);
			ui.widget->setbiaojiface(AppCC.biaojiface);
			ui.widget->setbeisuv(AppCC.bei_surv, AppCC.bei_surv_dat, mesh->faces_.size());
			ui.widget->setbeinorsur(AppCC.nor_bei_surv, AppCC.nor_bei_surv_dat, AppCC.bei_surv_center);
			ui.widget->setedgeline(AppCC.edgeline);
			ui.widget->setsingularface(mesh->singularface);
			ui.widget->setdraw_mode_(SINGULARFACE);
			ui.widget->pickedge_e.clear();
			update();
		});

	ui.actionDraw_BSvector->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionDraw_BSvector->setStatusTip(("draw bezier sharp vector"));
	connect(ui.actionDraw_BSvector, &QAction::triggered,
		[=]() {
			ui.widget->setbf(bf);
			ui.widget->setdraw_mode_(BSVECTOR);
			update();
		});

	ui.actionDraw_framevector->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionDraw_framevector->setStatusTip(("draw bezier sharp vector"));
	connect(ui.actionDraw_framevector, &QAction::triggered,
		[=]() {
			ui.widget->setbf(bf);
			ui.widget->setvF(vframe);
			ui.widget->setdraw_mode_(VFRAME);
			update();
		});

	ui.actionTransformation->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionTransformation->setStatusTip(("Transformation"));
	connect(ui.actionTransformation, &QAction::triggered,
		[=]() {
			Transformation t(mesh);
			t.translation(-0.04116, 0, 0);
			//t.scale(2, 2, 2);
			/*------3-------*/
			/*t.rotate_y(2, atan(2.4 / 5));
		t.scale(sqrt(31) / 5, 1, 1);
		t.translation(-0.5, 0, 0.15);*/
		/*------5-------*/
		/*t.scale(1.5, 1, 1);
	t.rotate_y(1.5, atan(3.3 / 2));
	t.translation(-0.35, 0, 1.12);*/
			mesh->cal_cen();
			mesh->cal_cell_cen();
			mesh->examine_bdy();
			mesh->examine_bdy();
			mesh->examine_cs();
			mesh->cal_cen1();
			mesh->examine_bdfv();
			mesh->cal_cell_cen();
			mesh->cal_bdnorm();
			mesh->cal_mami_ed();
			mesh->cal_oriv();
			ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
			ui.widget->setmesh(mesh);
			if (isneed == false)
			{
				ui.widget->set_scene_pos(mesh->center, mesh->r);
				ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
				ui.widget->setbb(mesh->bbMax, mesh->bbMin);
				ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
				isneed = true;
			}
			ui.widget->setdraw_mode_(FLAT_LINES);
			ui.widget->setoriv(mesh->oriv);
			update();
		});

	ui.actionX->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionX->setStatusTip(("Transformation"));
	connect(ui.actionX, &QAction::triggered,
		[=]() {
			xmove += 0.1;
			updatedrawx();
		});

	ui.actionX_2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionX_2->setStatusTip(("Transformation"));
	connect(ui.actionX_2, &QAction::triggered,
		[=]() {
			xmove -= 0.1;
			updatedrawx();
		});

	ui.actionZ->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionZ->setStatusTip(("Transformation"));
	connect(ui.actionZ, &QAction::triggered,
		[=]() {
			zmove += 0.1;
			updatedrawz();
		});

	ui.actionZ_2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionZ_2->setStatusTip(("Transformation"));
	connect(ui.actionZ_2, &QAction::triggered,
		[=]() {
			zmove -= 0.1;
			updatedrawz();
		});

	ui.actionAngle->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAngle->setStatusTip(("Transformation"));
	connect(ui.actionAngle, &QAction::triggered,
		[=]() {
			atans += 0.1;
			updatedraw2();
		});

	ui.actionAngle_2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionAngle_2->setStatusTip(("Transformation"));
	connect(ui.actionAngle_2, &QAction::triggered,
		[=]() {
			atans -= 0.1;
			updatedraw2();
		});

	ui.actionScale->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionScale->setStatusTip(("Transformation"));
	connect(ui.actionScale, &QAction::triggered,
		[=]() {
			scales += 0.1;
			updatedraw3();
		});

	ui.actionScale_2->setIcon(QIcon("Images/volume_mesh.png"));
	ui.actionScale_2->setStatusTip(("Transformation"));
	connect(ui.actionScale_2, &QAction::triggered,
		[=]() {
			scales -= 0.1;
			updatedraw3();
		});

	// 快捷图标栏
	ui.mainToolBar->addAction(ui.actionOpen);
	ui.mainToolBar->addAction(ui.actionSave);
	ui.mainToolBar->addAction(ui.actionClear_ALL);
	QToolBar *toolBar = addToolBar("View");
	toolBar->addAction(ui.actionWIRE_FRAME);
	toolBar->addAction(ui.actionSOLID_FLAT);
	toolBar->addAction(ui.actionFLAT_LINES);
	toolBar->addAction(ui.actionALL_LINES);
	toolBar->addAction(ui.actionALL_LINES1);
	toolBar->addAction(ui.actionALL_POINTS);
	toolBar->addAction(ui.actionALL_POLY);
	toolBar->addAction(ui.actionVOLUME_TEXTURE);
	QToolBar *toolBar1 = addToolBar("Function");
	toolBar1->addAction(ui.actionSubdvision);
	toolBar1->addAction(ui.actionSubdvision2);
	toolBar1->addAction(ui.actionSubdvision3);
	toolBar1->addAction(ui.actionSubdvision4);
	toolBar1->addAction(ui.actionSubdvision5);
	toolBar1->addAction(ui.actionSubdvision6);

	//�и����
	connect(ui.widget_3, SIGNAL(xslidderChanged(int)), ui.widget, SLOT(xSliderShow(int)));
	connect(ui.widget_3, SIGNAL(yslidderChanged(int)), ui.widget, SLOT(ySliderShow(int)));
	connect(ui.widget_3, SIGNAL(zslidderChanged(int)), ui.widget, SLOT(zSliderShow(int)));
	//���Ӳ���
	connect(ui.widget_3, SIGNAL(construct_vertex_texture_signal()), ui.widget, SLOT(construct_vertex_texture()));

	drawModeArg = 0;
}

/**
 * @description: create Actions
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::createActions()
{
	// lee
	actMLCA = new QAction(tr("MLCA"), this);
	actMLCA->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actMLCA->setStatusTip(tr("calc mlca subdivision for hex"));

	actMLCA_AVG = new QAction(tr("MLCA_AVG"), this);
	actMLCA_AVG->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actMLCA_AVG->setStatusTip(tr("calc mlca subdivision for hex:average"));

	actOpenSkel = new QAction(tr("openskel"), this);
	actOpenSkel->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actOpenSkel->setStatusTip(tr("readskel"));

	actExportSkel = new QAction(tr("export skel"), this);
	actExportSkel->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actExportSkel->setStatusTip(tr("export skel"));

	actBlend = new QAction(tr("blend"));
	actBlend->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actBlend->setStatusTip(tr("blend skel and hex"));

	actWhirl = new QAction(tr("whirl"));
	actWhirl->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actWhirl->setStatusTip(tr("rotate the scence"));

	actHelp = new QAction(tr("help"));
	actHelp->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actHelp->setStatusTip(tr("help to how to use this software"));

	actClearSkel = new QAction(tr("clearSkel"));
	actClearSkel->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actClearSkel->setStatusTip(tr("clear the skel"));

	actPIA = new QAction(tr("PIA"));
	actPIA->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actPIA->setStatusTip(tr("PIA hole mesh"));

	actFaceNormChange = new QAction(tr("faceNormChange"));
	actFaceNormChange->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actFaceNormChange->setStatusTip(tr("Face Norm Change"));

	actReadOpenComplexSkel = new QAction(tr("ReadComplexSkel"));
	actReadOpenComplexSkel->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actReadOpenComplexSkel->setStatusTip(tr("ReadComplexSkel"));

	actSimpleComplexSkel = new QAction(tr("SimpleComplexSkel"));
	actSimpleComplexSkel->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actSimpleComplexSkel->setStatusTip(tr("SimpleComplexSkel"));

	
	actsplitComplexSkel = new QAction(tr("actsplitComplexSkel"));
	actsplitComplexSkel->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actsplitComplexSkel->setStatusTip(tr("actsplitComplexSkel"));


	actFromVolumeToOff = new QAction(tr("fromVolumeToOff"));
	actFromVolumeToOff->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actFromVolumeToOff->setStatusTip(tr("fromVolumeToOff"));

	
	actChangeNodeType = new QAction(tr("ChangeNodeType"));
	actChangeNodeType->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actChangeNodeType->setStatusTip(tr("fromVolumeToOff"));


	actScaledJacobian = new QAction(tr("CalScaledJacobian"));
	actScaledJacobian->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actScaledJacobian->setStatusTip(tr("fromVolumeToOff"));

	actProject = new QAction(tr("Project"));
	actProject->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actProject->setStatusTip(tr("actProject"));

	actExportBroSkel = new QAction(tr("export simple op"));
	actExportBroSkel->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actExportBroSkel->setStatusTip(tr("export simple op"));

	actFixFlip = new QAction(tr("fixFlip"));
	actFixFlip->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actFixFlip->setStatusTip(tr("fixFliptriangle"));

	actOpenOff = new QAction(tr("openOff"));
	actOpenOff->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actOpenOff->setStatusTip(tr("openOff"));

	actShrinking = new QAction(tr("shrinking"));
	actShrinking->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actShrinking->setStatusTip(tr("shrinking"));

	actShowProjectLine = new QAction(tr("showProjectLine"));
	actShowProjectLine->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actShowProjectLine->setStatusTip(tr("showProjectLine"));


	
	actPickProjPoints = new QAction(tr("actPickProjPoints"));
	actPickProjPoints->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actPickProjPoints->setStatusTip(tr("actPickProjPoints"));

	

	actSkelMove = new QAction(tr("actSkelMove"));
	actSkelMove->setIcon(QIcon(":/HexMeshViewer/Images/cola.png"));
	actSkelMove->setStatusTip(tr("actSkelMove"));
	connect(actShowProjectLine, SIGNAL(triggered()), this, SLOT(showProjectLine()));
	connect(actMLCA, SIGNAL(triggered()), this, SLOT(subdivision()));
	connect(actMLCA_AVG, SIGNAL(triggered()), this, SLOT(average()));
	connect(actOpenSkel, SIGNAL(triggered()), this, SLOT(readSkel()));
	connect(actExportSkel, SIGNAL(triggered()), this, SLOT(exportSkel()));
	connect(actBlend, SIGNAL(triggered()), this, SLOT(blendSkel()));
	connect(actWhirl, SIGNAL(triggered()), this, SLOT(rotate()));
	connect(actHelp, SIGNAL(triggered()), this, SLOT(helpString()));
	connect(actClearSkel, SIGNAL(triggered()), this, SLOT(clearSkel()));
	connect(actPIA, SIGNAL(triggered()), this, SLOT(PIA()));
	connect(actFaceNormChange, SIGNAL(triggered()), this, SLOT(faceNormChange()));
	connect(actReadOpenComplexSkel, SIGNAL(triggered()), this, SLOT(readOpenComplexSkel()));
	connect(actsplitComplexSkel, SIGNAL(triggered()), this, SLOT(splitComplexSkel()));
	connect(actSimpleComplexSkel, SIGNAL(triggered()), this, SLOT(simpleComplexSkel()));
	
	connect(actFromVolumeToOff, SIGNAL(triggered()), this, SLOT(fromVolumeToOff()));
	connect(actChangeNodeType, SIGNAL(triggered()), this, SLOT(changeNodeType()));
	connect(actScaledJacobian, SIGNAL(triggered()), this, SLOT(scaledJacobian()));
	connect(actProject, SIGNAL(triggered()), this, SLOT(project()));
	connect(actExportBroSkel, SIGNAL(triggered()), this, SLOT(exportBroSkel()));
	connect(actFixFlip, SIGNAL(triggered()), this, SLOT(fixFlip()));
	connect(actOpenOff, SIGNAL(triggered()), this, SLOT(openOff()));
	connect(actShrinking, SIGNAL(triggered()), this, SLOT(shrinking()));
	connect(actPickProjPoints, SIGNAL(triggered()), this, SLOT(pickProjPoints()));
	connect(actSkelMove, SIGNAL(triggered()), this, SLOT(skelMove()));
}

// first open skel, then open skin
void HexMeshViewer::showProjectLine() {
	Project *t = new Project();
	QString path = QFileDialog::getOpenFileName(this, "projectSurface", "D:/yanjiusheng/code/HexMeshViewer/SkelResult"
		, "OFF(*.off)");
	if (path.isEmpty()) {
		return;
	}
	t->readTriMesh(path.toStdString());
	t->calCenter();
	ui.widget->setTriMesh(t);
	ui.widget->set_scene_pos({ t->center.x, t->center.y, t->center.z }, t->radius);
	ui.widget->setbb({ t->boxMaxPoint.x, t->boxMaxPoint.y, t->boxMaxPoint.z }, { t->boxMinPoint.x, t->boxMinPoint.y, t->boxMinPoint.z });
	ui.widget->seted(10, 10);
	ui.widget->setdraw_mode_(TRISKELPRO);
	ui.widget->updateGL();
}


void HexMeshViewer::shrinking() {
	QString path = QFileDialog::getOpenFileName(this, "openFixTriangleMesh", "D:/yanjiusheng/code/HexMeshViewer/SkelResult"
		, "OFF(*.off)");
	FileHandler file;
	if (path.isEmpty()) {
		std::cout << "[ERROR] file is not correct !\n";
		return;
	}
	

}

void HexMeshViewer::fixFlip() {
	QString path = QFileDialog::getOpenFileName(this, "openFixTriangleMesh", "D:/yanjiusheng/code/HexMeshViewer/SkelResult"
		, "OFF(*.off)");
	FileHandler file;
	if (path.isEmpty() == false)
	{
		FixFlip f;
		f.run(path.toStdString(), path.toStdString().substr(0, path.toStdString().find(".off")) + "_fix.off");
	}
	std::cout << "write OK\n";
}

void HexMeshViewer::openOff() {
	QString path = QFileDialog::getOpenFileName(this, "openTriangleMesh", "D:/yanjiusheng/code/HexMeshViewer/SkelResult"
		, "OFF(*.off)");
	if (path.isEmpty() == false) {
		//BMesh::TriangleMesh localTriangle;
		//localTriangle.openOff(path.toStdString());
		projectSkel = new Project();
		projectSkel->readTriMesh(path.toStdString());
		ui.widget->setTriMesh(projectSkel);
	}
}

void HexMeshViewer::fromVolumeToOff() {
	FileHandler file;
	QString path = QFileDialog::getSaveFileName(this,
		tr("Save off file"),
		tr(".."),
		tr("Off (*.off);;"
			"All File (*)"));
	if (path.isEmpty() == false) {
		file.writeFile(path.toStdString(), mesh);
		FixFlip f;
		f.run(path.toStdString(), path.toStdString().substr(0, path.toStdString().find(".off")) + "_fix.off");
	}
	/*Transform transform(mesh);
	transform.OutputPath = "D:/model/test.obj";
	transform.Quad2Triangle();*/
	std::cout << "write OK\n";
}

// project cube point to a suface
void HexMeshViewer::project() {
	projectSkel = new Project();
	QString path = QFileDialog::getOpenFileName(this, "projectSurface", "D:/yanjiusheng/code/HexMeshViewer/SkelResult"
		, "OFF(*.off)");
	if (path.isEmpty()) {
		return;
	}
	projectSkel->readTriMesh(path.toStdString());
	projectSkel->hMesh = mesh;
	ui.widget->setTriMesh(projectSkel);
	//t.projectVolumeToOff();
	//projectSkel->projectVolumeToOffFindClosePoint();
	
	mesh->examine_bdy();
	mesh->cal_cen();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	ui.widget->setmesh(mesh);
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	
	projectSkel->projectVolumeToOff_sec();

	ui.widget->set_scene_pos(mesh->center, mesh->r);
	ui.widget->setcell_center(mesh->cell_center);
	ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	ui.widget->setbb(mesh->bbMax, mesh->bbMin);
	update();
}

void HexMeshViewer::skelMove() {
	if (projectSkel != nullptr) {

		projectSkel->hMesh = mesh;
		projectSkel->movePoint();

		material_distribution.resize(mesh->cells_.size(), 1);
		checkcell.resize(mesh->cells_.size(), false);
		mesh->cal_cen();
		mesh->cal_cell_cen();
		mesh->examine_bdy();
		mesh->examine_cs();
		mesh->cal_cen1();
		mesh->examine_bdfv();
		mesh->cal_cell_cen();
		mesh->cal_bdnorm();
		mesh->cal_mami_ed();
		mesh->cal_oriv();
		ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
		ui.widget->setmesh(mesh);
		ui.widget->set_scene_pos(mesh->center, mesh->r);
		qDebug() << mesh->r;
		ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
		ui.widget->setbb(mesh->bbMax, mesh->bbMin);
		ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
		isneed = true;
		ui.widget->setmouse_event(DEFAULT);
		ui.widget->setdraw_mode_(FLAT_LINES);
		ui.widget->setoriv(mesh->oriv);
		vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
		ui.widget->update();
	}
}

void HexMeshViewer::rchangeToPolylines() {
	solutionSkel->changeToPolylines();
}



void HexMeshViewer::rshowSkel() {
	ui.widget->clear_mesh();
	delete mesh;
	mesh = new HexV3fMesh();
	isneed = false;
	// let Bmesh hex to mesh
	solutionSkel->genViewHex((*mesh));
	// show it
	material_distribution.resize(mesh->cells_.size(), 1);
	checkcell.resize(mesh->cells_.size(), false);
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	ui.widget->setmesh(mesh);
	ui.widget->setRoot(solutionSkel->getRoot());
	ui.widget->setNodeVec(solutionSkel->getNodeVec());
	ui.widget->set_scene_pos(mesh->center, mesh->r);
	qDebug() << mesh->r;
	ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	ui.widget->setbb(mesh->bbMax, mesh->bbMin);
	ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
	isneed = true;
	ui.widget->setdraw_mode_(SKEL);
	ui.widget->setoriv(mesh->oriv);
	vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
}


void HexMeshViewer::scaledJacobian() {
	double ave_sj = 0; double count = 0;
	double min_sj = 2.0;
	Eigen::Matrix3d J;
	
	for (auto &it : mesh->cells_) {
		std::vector<V3f> vv;
		std::vector<double> val;
		std::vector<VertexHandle> tmpVtex;
		for (auto& iit : it.vertex_) {
			vv.push_back(mesh->vertices_[iit]); // 0 - 7
			tmpVtex.push_back(iit);
		}
		V3f lvector1 = (vv[1] - vv[0]).normalize();
		V3f lvector2 = (vv[3] - vv[0]).normalize();
		V3f lvector3 = (vv[4] - vv[0]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		double det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;


		lvector1 = (vv[2] - vv[1]).normalize();
		lvector2 = (vv[0] - vv[1]).normalize();
		lvector3 = (vv[5] - vv[1]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;


		lvector1 = (vv[3] - vv[2]).normalize();
		lvector2 = (vv[1] - vv[2]).normalize();
		lvector3 = (vv[6] - vv[2]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;

		lvector1 = (vv[0] - vv[3]).normalize();
		lvector2 = (vv[2] - vv[3]).normalize();
		lvector3 = (vv[7] - vv[3]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;

		lvector1 = (vv[7] - vv[4]).normalize();
		lvector2 = (vv[5] - vv[4]).normalize();
		lvector3 = (vv[0] - vv[4]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;

		lvector1 = (vv[4] - vv[5]).normalize();
		lvector2 = (vv[6] - vv[5]).normalize();
		lvector3 = (vv[1] - vv[5]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;

		lvector1 = (vv[5] - vv[6]).normalize();
		lvector2 = (vv[7] - vv[6]).normalize();
		lvector3 = (vv[2] - vv[6]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;


		lvector1 = (vv[6] - vv[7]).normalize();
		lvector2 = (vv[4] - vv[7]).normalize();
		lvector3 = (vv[3] - vv[7]).normalize();
		for (int i = 0; i < 3; ++i) {
			if (i == 0) {
				J(0, i) = lvector1.x; J(1, i) = lvector1.y; J(2, i) = lvector1.z;
			}
			else if (i == 1) {
				J(0, i) = lvector2.x; J(1, i) = lvector2.y; J(2, i) = lvector2.z;
			}
			else {
				J(0, i) = lvector3.x; J(1, i) = lvector3.y; J(2, i) = lvector3.z;
			}
		}
		det_val = J.determinant();
		val.push_back(det_val);
		// std::cout << "[DEBUG] " << det_val << std::endl;
		if (det_val < min_sj)
		{
			min_sj = det_val;
		}
		ave_sj += det_val; count += 1;
		auto tmp = std::min_element(val.begin(), val.end());
		std::cout << "[DEBUG] minJob " << *tmp << "\n";
		
		if (*tmp < 0) {
			std::vector<VertexHandle> tmpVtexO(tmpVtex.begin(), tmpVtex.end());
			tmpVtex[0] = tmpVtexO[4];
			tmpVtex[1] = tmpVtexO[5];
			tmpVtex[2] = tmpVtexO[6];
			tmpVtex[3] = tmpVtexO[7];
			tmpVtex[4] = tmpVtexO[0];
			tmpVtex[5] = tmpVtexO[1];
			tmpVtex[6] = tmpVtexO[2];
			tmpVtex[7] = tmpVtexO[3];
			it.vertex_.clear();
			it.vertex_.push_back(tmpVtex[0]);
			it.vertex_.push_back(tmpVtex[1]);
			it.vertex_.push_back(tmpVtex[2]);
			it.vertex_.push_back(tmpVtex[3]);
			it.vertex_.push_back(tmpVtex[4]);
			it.vertex_.push_back(tmpVtex[5]);
			it.vertex_.push_back(tmpVtex[6]);
			it.vertex_.push_back(tmpVtex[7]);
		}

	}
	printf("Average Scaled Jacobian: %f \n", ave_sj / count);
	printf("Min Scaled Jacobian: %f \n", min_sj);
}

/**
 * @description: simple complex skel
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::simpleWithCgal() {
	QString path = QFileDialog::getOpenFileName(this, "open", ""
		, "OFF(*.off)");
	CgalMcfskel tt;
	tt.NowProcessPath = path.toLocal8Bit().data();
	QString path1 = QFileDialog::getSaveFileName(this,
		tr("Save skel file"),
		tr(".."),
		tr("skel (*.skel);;"
			"All File (*)"));
	if (path1.isEmpty() == false) {
		tt.outProcessPath = path1.toStdString();
		tt.Run();
		QMessageBox::information(NULL, "Information", "off to skel is finished, Lucky Guy!",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

/**
 * @description: pipe reCon
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rBoom5() {
	QString path = QFileDialog::getOpenFileName(this, "open", ""
		, "OFF(*.off)");
	solutionSkel->solutionOldBMesh->setTriOffPath(path.toStdString());
	solutionSkel->pipeline2();
}


/**
 * @description: simple complex skel
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::simpleComplexSkel() {
	outskel->OPDelete();
	//不需要再设置模型中心
	ui.widget->setSkel(outskel);
	//
	//ui.widget->setFlag(true);
	ui.widget->setdraw_mode_(Skeleton);
	update();
	ui.widget->updateGL();
}

void HexMeshViewer::exportBroSkel() {
	QString path = QFileDialog::getSaveFileName(this, "open", "D:/"
	, "SKEL(*.skel)");
	if (path.isEmpty()) {
		return;
	}
	outskel->filename = path.toStdString();
	outskel->writeSimpleSkel();
}



/**
 * @description: read complex Skel
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::readOpenComplexSkel() {
	QString path = QFileDialog::getOpenFileName(this, "open", "D:/yanjiusheng/code/HexMeshViewer/SkelResult"
		, "skel(*.skel)");
	OurSkel *TriSkel = new OurSkel();
	outskel = new OurSkel();
	std::string stpath = path.toStdString();
	FileHandler file(stpath);
	file.readSkeltonSKEL(TriSkel);
	outskel->SkelAll.assign(TriSkel->SkelAll.begin(), TriSkel->SkelAll.end());
	ui.widget->set_scene_pos(TriSkel->center, TriSkel->r);
	ui.widget->setSkel(TriSkel);
	ui.widget->setdraw_mode_(Skeleton);
	update();
	ui.widget->updateGL();
}

/**
 * @description: split complex Skel maybe then wo interp it;
  	// Indegree = 2 then we visit it then check it
	// Indegree = 1 then we visit it then check it
	// other Indegree then we visit it once then wo plus 1 then if visit time equal his Indegree then we check it
 * @param {*}
 * @return {*}
 */

void dfs(std::vector<int>& visited, std::vector<OurSkelNode>& outskel, int index) {

}
void HexMeshViewer::splitComplexSkel() {
	// first we find indegree != 2
	if (outskel == nullptr) {
		std::cout << "we should read complexskel first \n";
		return;
	}
	std::vector<std::pair<OurSkelNode, int> > endNodes;
	for (int i = 0; i < outskel->SkelAll.size(); i++) {
		if (outskel->SkelAll[i].Indegree != 2) {
			endNodes.push_back(std::make_pair(outskel->SkelAll[i], i));
		}
	}
	std::vector<int> visited(outskel->SkelAll.size(), 0);
	sort(outskel->SkelAll.begin(), outskel->SkelAll.end(), [](const auto& a, const auto& b) {
		return a.Indegree < b.Indegree;
		});
}


/**
 * @description: select mutiNodes then update selectNodes.
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rupdateSelectMutiNodes() {
	if(ui.widget->pickcell.size() == 0) {
		return;
	}
	solutionSkel->selectNodes.clear();
	for(int index=0; index < ui.widget->pickcell.size(); index++) {
		V3f tmp(0.0, 0.0, 0.0);
		for (int i = 0; i < 8; i++) {
			tmp += mesh->vertices_[mesh->cells_[ui.widget->pickcell[index]].vertex_[i]];
		}
		tmp = tmp / 8.0;
		// check with all node position
		BMesh::Node * rlt = solutionSkel->selectWhichNode(tmp);
		if(rlt) {
			solutionSkel->selectNodes.push_back(rlt);
		}
	}
}

void HexMeshViewer::rdrawModeChanged(int arg) {
	if(arg == 0) {
		ui.widget->setdraw_mode_(SKEL);
	}else if (arg == 1) {
		ui.widget->setdraw_mode_(SELECT_CELL);
	}else if (arg == 2) {
		ui.widget->setdraw_mode_(SELECT_CELLS);
	}
	drawModeArg = arg;
	ui.widget->updateGL();
}

/**
 * @description: 
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::faceNormChange() {
	mesh->cal_cen();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm(ui.widget->pickface);
	//mesh->cal_bdnorm_smooth();
	mesh->cal_mami_ed();
	ui.widget->setmesh(mesh);
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	//ui.widget->set_scene_pos(mesh->center, mesh->r);
	ui.widget->setcell_center(mesh->cell_center);
	ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	ui.widget->setbb(mesh->bbMax, mesh->bbMin);
	ui.widget->pickface.clear();
	update();
	ui.widget->updateGL();
}



/**
 * @description: PIA
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::PIA() {
	

	FileHandler file1;
	file1.writeFileMesh("coarse_skel.mesh", mesh);
	// std::string stpath = "coarse_skel.mesh";
	// FileHandler file(stpath);
	// file.read2HexMeshObj_new(mesh);
	
	// QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/"
	// 	, "Mesh(*.mesh)");
	std::string stpath = "coarse_skel.mesh";
	FileHandler file(stpath);
	HexV3fMesh* oldmesh = new HexV3fMesh();
	file.readFile(oldmesh);
	//file.read2HexMeshObj(oldmesh, true, true, true);
	oldmesh->examine_bdy();
	oldmesh->examine_cs();
	oldmesh->cal_cen();
	oldmesh->examine_bdfv();
	oldmesh->cal_cell_cen();
	oldmesh->cal_bdnorm();
	oldmesh->cal_mami_ed();
	oldmesh->cal_oriv();

	mesh->examine_bdy();
	for (int i = 0; i < 10; ++i) {
		PIA_FOR_CC_SOLID pia(mesh, mesh);
		pia.oldmesh = oldmesh;

		//ui.textBrowser->append(QString::number(pia.err_now));
		pia.PIA_FOR_CC_SOLID_ONCE();


		mesh->examine_bdy();
		mesh->cal_cen();
		mesh->examine_bdfv();
		mesh->cal_cell_cen();
		mesh->cal_bdnorm();
		mesh->cal_mami_ed();
		//ui.textBrowser->append(QString::number(pia.err_now));
		ui.widget->setmesh(mesh);
		ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
		ui.widget->set_scene_pos(mesh->center, mesh->r);
		ui.widget->setcell_center(mesh->cell_center);
		ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
		ui.widget->setbb(mesh->bbMax, mesh->bbMin);
		update();
	}
}

void HexMeshViewer::raxisShowChanged(int arg){
	if(arg) {
		ui.widget->bshowAxis = true;
	}else {
		ui.widget->bshowAxis = false;
	}
	ui.widget->updateGL();
}

void HexMeshViewer::rgroundShowChanged(int arg){
	if(arg) {
		ui.widget->bshowGround = true;
	}else {
		ui.widget->bshowGround = false;
	}
	ui.widget->updateGL();
}

void HexMeshViewer::rskelShowChanged(int arg){
	if(arg) {
		ui.widget->bshowSKEL = true;
	}else {
		ui.widget->bshowSKEL = false;
	}
	ui.widget->updateGL();
}

void HexMeshViewer::rmirrorShowChanged(int arg) {
	if(arg) {
		ui.widget->bshowMirror = true;
	} else {
		ui.widget->bshowMirror = false;
	}
	ui.widget->updateGL();
}

/**
 * @description: 暂时不处理
 * @param {int} arg
 * @return {*}
 */
void HexMeshViewer::rmirrorModeChanged(int arg) {
	if(arg) {
		ui.widget->bmirrorMode = true;
		solutionSkel->bmirrorMode = true;
	} else {
		ui.widget->bmirrorMode = false;
		solutionSkel->bmirrorMode = false;
	}
	ui.widget->updateGL();
}

void HexMeshViewer::rdragModeChanged(int arg){
	if(arg) {
		ui.widget->bdragMode = true;
		solutionSkel->bdragMode = true;
	} else {
		ui.widget->bdragMode = false;
		solutionSkel->bdragMode = false;
	}
}

void HexMeshViewer::rshowTriMeshChanged(int arg) {
	if (arg) {
		ui.widget->bshowTriMeshChanged = true;
		//ui.widget->setdraw_mode_(SKEL);
		//solutionSkel->bdragMode = true;
	}
	else {
		ui.widget->bshowTriMeshChanged = false;
		//ui.widget->setdraw_mode_(TRISKELPRO);
		//solutionSkel->bdragMode = false;
	}
}

void HexMeshViewer::rshowProjectLinesChanged(int arg) {
	if (arg) {
		ui.widget->bshowProjectLinesChanged = true;

		//solutionSkel->bdragMode = true;
	}
	else {
		ui.widget->bshowProjectLinesChanged = false;

		//solutionSkel->bdragMode = false;
	}
}



void HexMeshViewer::rRotateByMouseChanged(int arg) {
	if(arg) {
		ui.widget->bRotateByMouse = true;
		solutionSkel->bRotateByMouse = true;
	} else {
		ui.widget->bRotateByMouse = false;
		solutionSkel->bRotateByMouse = false;
	}
}

void HexMeshViewer::rVertexSnapModeChanged(int arg) {
	if(arg) {
		ui.widget->bVertexSnap = true;
		solutionSkel->bVertexSnap = true;
	} else {
		ui.widget->bVertexSnap = false;
		solutionSkel->bVertexSnap = false;
	}
}

/**
 * @description: 清空骨架
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::clearSkel() {
	solutionSkel->clearSkel();
	ui.widget->clear_mesh();
	delete mesh;
	mesh = new HexV3fMesh();
	isneed = false;
	update();
}

/**
 * @description: show how to use this software
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::helpString() {
	QString text("<h2>Our BMesh Implementation</h2>");
	text += "<h3>Participants</h3>";
	text += "<ul>";
	text += "<li>Lee</li>";
	text += "</ul>";
	text += "<h3>Basics</h3>";
	text += "<p>";

	text += "App actions: <ul>";
	text += "<li> H : make this help appear</li>";
	text += "<li> alt + ctrl + x/y/z/o : scale skel (mirror)</li>";
	text += "<li> alt + shift + x/y/z: rotate angle</li>";
	text += "<li> shift + x/y/z: rotate angle 正转</li>";
	text += "<li> alt + x/y/z: add angle</li>";
	text += "<li> shift + w/a/s/d/j/k: move cube</li>";
	text += "<li> 选中一个节点，然后可以选中在骨架上的节点,通过 shift + wasdjk 可以控制移动大部分操作<li>";
	
	QMessageBox::information(NULL, "Help", text, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}



void HexMeshViewer::rfittingCurvesSignal() {
	InterpolateCurve a;
	a.readOff(); // read off
	a.genPoints("");
	a.simpleSkel(); 
	a.exportSkel();
	solutionSkel->solutionOldBMesh->curvePointss = a.curvePointss;
	QMessageBox::information(NULL, "Information", "finish! file == result.skel",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void HexMeshViewer::ractRMFSignal() {
	solutionSkel->solutionOldBMesh->useRMF();
	// Test Point
	
	if(projectSkel == NULL)
		projectSkel = new Project();
	ui.widget->setTriMesh(projectSkel);

	for (int i = 0; i < solutionSkel->solutionOldBMesh->curvePointss.size(); i++) {
		for (int j = 0; j < solutionSkel->solutionOldBMesh->curvePointss[i].size()-1; j++) {
			projectSkel->projectLines.push_back(std::make_pair(
				V3f(solutionSkel->solutionOldBMesh->curvePointss[i][j].x,
				solutionSkel->solutionOldBMesh->curvePointss[i][j].y,
				solutionSkel->solutionOldBMesh->curvePointss[i][j].z),
				V3f(solutionSkel->solutionOldBMesh->curvePointss[i][j + 1].x,
					solutionSkel->solutionOldBMesh->curvePointss[i][j + 1].y,
					solutionSkel->solutionOldBMesh->curvePointss[i][j + 1].z)));
		}
	}
	for (int i = 0; i < solutionSkel->solutionOldBMesh->Us.size(); i++) {
		projectSkel->projectLines.push_back(std::make_pair(
			V3f(solutionSkel->solutionOldBMesh->Us[i].center.x,
				solutionSkel->solutionOldBMesh->Us[i].center.y,
				solutionSkel->solutionOldBMesh->Us[i].center.z),
			V3f(solutionSkel->solutionOldBMesh->Us[i].center.x + solutionSkel->solutionOldBMesh->Us[i].t.x * 0.2,
				solutionSkel->solutionOldBMesh->Us[i].center.y + solutionSkel->solutionOldBMesh->Us[i].t.y * 0.2,
				solutionSkel->solutionOldBMesh->Us[i].center.z + solutionSkel->solutionOldBMesh->Us[i].t.z * 0.2)));
		projectSkel->projectLines.push_back(std::make_pair(
			V3f(solutionSkel->solutionOldBMesh->Us[i].center.x,
				solutionSkel->solutionOldBMesh->Us[i].center.y,
				solutionSkel->solutionOldBMesh->Us[i].center.z),
			V3f(solutionSkel->solutionOldBMesh->Us[i].center.x + solutionSkel->solutionOldBMesh->Us[i].r.x * 0.2,
				solutionSkel->solutionOldBMesh->Us[i].center.y + solutionSkel->solutionOldBMesh->Us[i].r.y * 0.2,
				solutionSkel->solutionOldBMesh->Us[i].center.z + solutionSkel->solutionOldBMesh->Us[i].r.z * 0.2)));
		projectSkel->projectLines.push_back(std::make_pair(
			V3f(solutionSkel->solutionOldBMesh->Us[i].center.x,
				solutionSkel->solutionOldBMesh->Us[i].center.y,
				solutionSkel->solutionOldBMesh->Us[i].center.z),
			V3f(solutionSkel->solutionOldBMesh->Us[i].center.x + solutionSkel->solutionOldBMesh->Us[i].s.x * 0.2,
				solutionSkel->solutionOldBMesh->Us[i].center.y + solutionSkel->solutionOldBMesh->Us[i].s.y * 0.2,
				solutionSkel->solutionOldBMesh->Us[i].center.z + solutionSkel->solutionOldBMesh->Us[i].s.z * 0.2)));
	}
	for (auto it : solutionSkel->solutionOldBMesh->frames) {
		projectSkel->projectLines2.push_back(std::make_pair(
			V3f(it.second.center.x,
				it.second.center.y,
				it.second.center.z),
			V3f(it.second.center.x + it.second.s.x * 0.2,
				it.second.center.y + it.second.s.y * 0.2,
				it.second.center.z + it.second.s.z * 0.2)));
		projectSkel->projectLines2.push_back(std::make_pair(
			V3f(it.second.center.x,
				it.second.center.y,
				it.second.center.z),
			V3f(it.second.center.x + it.second.r.x * 0.2,
				it.second.center.y + it.second.r.y * 0.2,
				it.second.center.z + it.second.r.z * 0.2)));
		projectSkel->projectLines2.push_back(std::make_pair(
			V3f(it.second.center.x,
				it.second.center.y,
				it.second.center.z),
			V3f(it.second.center.x + it.second.t.x * 0.2,
				it.second.center.y + it.second.t.y * 0.2,
				it.second.center.z + it.second.t.z * 0.2)));
	}
	
	SingalFather::getInstance()->sdrawNormalSkel();
}

void HexMeshViewer::rBoom4() {
	HexV3fMesh* newmesh = new HexV3fMesh();
	mesh->examine_bdy();
	CCSubdivision sub(mesh, newmesh);
	*mesh = sub.Money();
	mesh->examine_bdy();
	mesh->cal_cen();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	ui.widget->setmesh(mesh);
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	ui.widget->set_scene_pos(mesh->center, mesh->r);
	ui.widget->setcell_center(mesh->cell_center);
	ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	ui.widget->setbb(mesh->bbMax, mesh->bbMin);
	update();

	//*mesh = spiltChineseMoney(*mesh);
}
/**
 * @description: spilt one to 7 , just like chinese money
 * @param {*}
 * @return {*}
 */
HexV3fMesh& HexMeshViewer::spiltChineseMoney(HexV3fMesh& mesh)
{
	
	uint64_t cell_num = mesh.cells_.size();
	VertexHandle temp_vex_local[16];
	HexV3fMesh newmesh;
	newmesh_local = new HexV3fMesh();
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		VertexHandle temp_vex_local[16];
		std::vector<V3f>::iterator v_id;

		// 傻逼了 直接for不就好了

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex[mesh_vex_num] = newmesh.add_vertex(mesh.vertices_[mesh.cells_[i].vertex_[mesh_vex_num]]);
			temp_vex_local[mesh_vex_num] = 
				newmesh_local->add_vertex(mesh.vertices_[mesh.cells_[i].vertex_[mesh_vex_num]]);
		}
		//再插入每条边的中点 暴力插入 以免顺序错误
		//第一条边的中点
		V3f edge_mid_1;
		edge_mid_1 = (mesh.vertices_[mesh.cells_[i].vertex_[0]] + mesh.vertices_[mesh.cells_[i].vertex_[1]]) * 1.0 / 2 * 1.0;
		temp_vex[8] = newmesh.add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2;
		edge_mid_2 = (mesh.vertices_[mesh.cells_[i].vertex_[1]] + mesh.vertices_[mesh.cells_[i].vertex_[5]]) * 1.0 / 2 * 1.0;
		temp_vex[9] = newmesh.add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3;
		edge_mid_3 = (mesh.vertices_[mesh.cells_[i].vertex_[5]] + mesh.vertices_[mesh.cells_[i].vertex_[4]]) * 1.0 / 2 * 1.0;
		temp_vex[10] = newmesh.add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4;
		edge_mid_4 = (mesh.vertices_[mesh.cells_[i].vertex_[4]] + mesh.vertices_[mesh.cells_[i].vertex_[0]]) * 1.0 / 2 * 1.0;
		temp_vex[11] = newmesh.add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5;
		edge_mid_5 = (mesh.vertices_[mesh.cells_[i].vertex_[3]] + mesh.vertices_[mesh.cells_[i].vertex_[2]]) * 1.0 / 2 * 1.0;
		temp_vex[12] = newmesh.add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6;
		edge_mid_6 = (mesh.vertices_[mesh.cells_[i].vertex_[3]] + mesh.vertices_[mesh.cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
		temp_vex[13] = newmesh.add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7;
		edge_mid_7 = (mesh.vertices_[mesh.cells_[i].vertex_[6]] + mesh.vertices_[mesh.cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
		temp_vex[14] = newmesh.add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8;
		edge_mid_8 = (mesh.vertices_[mesh.cells_[i].vertex_[2]] + mesh.vertices_[mesh.cells_[i].vertex_[6]]) * 1.0 / 2 * 1.0;
		temp_vex[15] = newmesh.add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9;
		edge_mid_9 = (mesh.vertices_[mesh.cells_[i].vertex_[1]] + mesh.vertices_[mesh.cells_[i].vertex_[2]]) * 1.0 / 2 * 1.0;
		temp_vex[16] = newmesh.add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10;
		edge_mid_10 = (mesh.vertices_[mesh.cells_[i].vertex_[5]] + mesh.vertices_[mesh.cells_[i].vertex_[6]]) * 1.0 / 2 * 1.0;
		temp_vex[17] = newmesh.add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11;
		edge_mid_11 = (mesh.vertices_[mesh.cells_[i].vertex_[0]] + mesh.vertices_[mesh.cells_[i].vertex_[3]]) * 1.0 / 2 * 1.0;
		temp_vex[18] = newmesh.add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12;
		edge_mid_12 = (mesh.vertices_[mesh.cells_[i].vertex_[4]] + mesh.vertices_[mesh.cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
		temp_vex[19] = newmesh.add_vertex(edge_mid_12);

		//再插入每个面的中点
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid;
			face_mid = (mesh.vertices_[mesh.faces_[mesh.cells_[i].faces_[mesh_face_num]].vs_f[0]]
				+ mesh.vertices_[mesh.faces_[mesh.cells_[i].faces_[mesh_face_num]].vs_f[1]]
				+ mesh.vertices_[mesh.faces_[mesh.cells_[i].faces_[mesh_face_num]].vs_f[2]]
				+ mesh.vertices_[mesh.faces_[mesh.cells_[i].faces_[mesh_face_num]].vs_f[3]]) * 1.0 / 4 * 1.0;
			temp_vex[20 + mesh_face_num] = newmesh.add_vertex(face_mid);
		}

		//插入单元的内心
		V3f cell_mid;
		for (int j = 0; j < 8; j++)
		{
			cell_mid += mesh.vertices_[mesh.cells_[i].vertex_[j]];
		}
		cell_mid = cell_mid * 1.0 / 8 * 1.0;
		temp_vex[26] = newmesh.add_vertex(cell_mid);

		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(8);
		
		//第一个单元
		sub_cell[0].push_back(temp_vex[0]); sub_cell[0].push_back(temp_vex[8]);
		sub_cell[0].push_back(temp_vex[25]); sub_cell[0].push_back(temp_vex[18]);
		sub_cell[0].push_back(temp_vex[11]); sub_cell[0].push_back(temp_vex[20]);
		sub_cell[0].push_back(temp_vex[26]); sub_cell[0].push_back(temp_vex[23]);
		//第二个单元
		sub_cell[1].push_back(temp_vex[8]); sub_cell[1].push_back(temp_vex[1]);
		sub_cell[1].push_back(temp_vex[16]); sub_cell[1].push_back(temp_vex[25]);
		sub_cell[1].push_back(temp_vex[20]); sub_cell[1].push_back(temp_vex[9]);
		sub_cell[1].push_back(temp_vex[22]); sub_cell[1].push_back(temp_vex[26]);
		//第三个单元
		sub_cell[3].push_back(temp_vex[18]); sub_cell[3].push_back(temp_vex[25]);
		sub_cell[3].push_back(temp_vex[12]); sub_cell[3].push_back(temp_vex[3]);
		sub_cell[3].push_back(temp_vex[23]); sub_cell[3].push_back(temp_vex[26]);
		sub_cell[3].push_back(temp_vex[21]); sub_cell[3].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[2].push_back(temp_vex[25]); sub_cell[2].push_back(temp_vex[16]);
		sub_cell[2].push_back(temp_vex[2]); sub_cell[2].push_back(temp_vex[12]);
		sub_cell[2].push_back(temp_vex[26]); sub_cell[2].push_back(temp_vex[22]);
		sub_cell[2].push_back(temp_vex[15]); sub_cell[2].push_back(temp_vex[21]);
		//第五个单元
		sub_cell[4].push_back(temp_vex[11]); sub_cell[4].push_back(temp_vex[20]);
		sub_cell[4].push_back(temp_vex[26]); sub_cell[4].push_back(temp_vex[23]);
		sub_cell[4].push_back(temp_vex[4]); sub_cell[4].push_back(temp_vex[10]);
		sub_cell[4].push_back(temp_vex[24]); sub_cell[4].push_back(temp_vex[19]);
		//第六个单元
		sub_cell[5].push_back(temp_vex[20]); sub_cell[5].push_back(temp_vex[9]);
		sub_cell[5].push_back(temp_vex[22]); sub_cell[5].push_back(temp_vex[26]);
		sub_cell[5].push_back(temp_vex[10]); sub_cell[5].push_back(temp_vex[5]);
		sub_cell[5].push_back(temp_vex[17]); sub_cell[5].push_back(temp_vex[24]);
		//第七个单元
		sub_cell[7].push_back(temp_vex[23]); sub_cell[7].push_back(temp_vex[26]);
		sub_cell[7].push_back(temp_vex[21]); sub_cell[7].push_back(temp_vex[13]);
		sub_cell[7].push_back(temp_vex[19]); sub_cell[7].push_back(temp_vex[24]);
		sub_cell[7].push_back(temp_vex[14]); sub_cell[7].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[6].push_back(temp_vex[26]); sub_cell[6].push_back(temp_vex[22]);
		sub_cell[6].push_back(temp_vex[15]); sub_cell[6].push_back(temp_vex[21]);
		sub_cell[6].push_back(temp_vex[24]); sub_cell[6].push_back(temp_vex[17]);
		sub_cell[6].push_back(temp_vex[6]); sub_cell[6].push_back(temp_vex[14]);
		int index_local = 0;
		for (auto it : sub_cell) {
			V3f cell_mid;
			for (auto iit : it) {
				cell_mid += newmesh.vertices_[iit];
			}
			cell_mid = cell_mid * 1.0 / 8 * 1.0;
			temp_vex_local[index_local + 8] = newmesh_local->add_vertex(cell_mid);
			index_local++;
		}
		std::vector<std::vector<VertexHandle>> sub_cell_local(7);
		/*  显示cc体细分有问题
		//第一个单元
		sub_cell_local[0].push_back(temp_vex_local[0]); sub_cell_local[0].push_back(temp_vex_local[4]);
		sub_cell_local[0].push_back(temp_vex_local[5]); sub_cell_local[0].push_back(temp_vex_local[1]);
		sub_cell_local[0].push_back(temp_vex_local[8]); sub_cell_local[0].push_back(temp_vex_local[12]);
		sub_cell_local[0].push_back(temp_vex_local[13]); sub_cell_local[0].push_back(temp_vex_local[9]);
		//第二个单元
		sub_cell_local[1].push_back(temp_vex_local[2]); sub_cell_local[1].push_back(temp_vex_local[6]);
		sub_cell_local[1].push_back(temp_vex_local[7]); sub_cell_local[1].push_back(temp_vex_local[3]);
		sub_cell_local[1].push_back(temp_vex_local[10]); sub_cell_local[1].push_back(temp_vex_local[14]);
		sub_cell_local[1].push_back(temp_vex_local[15]); sub_cell_local[1].push_back(temp_vex_local[11]);
		//第三个单元
		sub_cell_local[2].push_back(temp_vex_local[1]); sub_cell_local[2].push_back(temp_vex_local[5]);
		sub_cell_local[2].push_back(temp_vex_local[6]); sub_cell_local[2].push_back(temp_vex_local[2]);
		sub_cell_local[2].push_back(temp_vex_local[9]); sub_cell_local[2].push_back(temp_vex_local[13]);
		sub_cell_local[2].push_back(temp_vex_local[14]); sub_cell_local[2].push_back(temp_vex_local[10]);
		//第四个单元
		sub_cell_local[3].push_back(temp_vex_local[3]); sub_cell_local[3].push_back(temp_vex_local[7]);
		sub_cell_local[3].push_back(temp_vex_local[4]); sub_cell_local[3].push_back(temp_vex_local[0]);
		sub_cell_local[3].push_back(temp_vex_local[11]); sub_cell_local[3].push_back(temp_vex_local[15]);
		sub_cell_local[3].push_back(temp_vex_local[12]); sub_cell_local[3].push_back(temp_vex_local[8]);
		//第五个单元
		sub_cell_local[4].push_back(temp_vex_local[4]); sub_cell_local[4].push_back(temp_vex_local[7]);
		sub_cell_local[4].push_back(temp_vex_local[6]); sub_cell_local[4].push_back(temp_vex_local[5]);
		sub_cell_local[4].push_back(temp_vex_local[12]); sub_cell_local[4].push_back(temp_vex_local[15]);
		sub_cell_local[4].push_back(temp_vex_local[14]); sub_cell_local[4].push_back(temp_vex_local[13]);
		//第6个单元
		sub_cell_local[5].push_back(temp_vex_local[1]); sub_cell_local[5].push_back(temp_vex_local[0]);
		sub_cell_local[5].push_back(temp_vex_local[3]); sub_cell_local[5].push_back(temp_vex_local[2]);
		sub_cell_local[5].push_back(temp_vex_local[9]); sub_cell_local[5].push_back(temp_vex_local[8]);
		sub_cell_local[5].push_back(temp_vex_local[11]); sub_cell_local[5].push_back(temp_vex_local[10]);
		//第7个单元
		sub_cell_local[6].push_back(temp_vex_local[8]); sub_cell_local[6].push_back(temp_vex_local[9]);
		sub_cell_local[6].push_back(temp_vex_local[10]); sub_cell_local[6].push_back(temp_vex_local[11]);
		sub_cell_local[6].push_back(temp_vex_local[12]); sub_cell_local[6].push_back(temp_vex_local[13]);
		sub_cell_local[6].push_back(temp_vex_local[14]); sub_cell_local[6].push_back(temp_vex_local[15]);
		*/
		//第一个单元 左
		sub_cell_local[0].push_back(temp_vex_local[0]); sub_cell_local[0].push_back(temp_vex_local[1]);
		sub_cell_local[0].push_back(temp_vex_local[2]); sub_cell_local[0].push_back(temp_vex_local[3]);
		sub_cell_local[0].push_back(temp_vex_local[8]); sub_cell_local[0].push_back(temp_vex_local[9]);
		sub_cell_local[0].push_back(temp_vex_local[10]); sub_cell_local[0].push_back(temp_vex_local[11]);
		//第二个单元 右
		sub_cell_local[1].push_back(temp_vex_local[12]); sub_cell_local[1].push_back(temp_vex_local[13]);
		sub_cell_local[1].push_back(temp_vex_local[14]); sub_cell_local[1].push_back(temp_vex_local[15]);
		sub_cell_local[1].push_back(temp_vex_local[4]); sub_cell_local[1].push_back(temp_vex_local[5]);
		sub_cell_local[1].push_back(temp_vex_local[6]); sub_cell_local[1].push_back(temp_vex_local[7]);
		//第三个单元 上
		sub_cell_local[2].push_back(temp_vex_local[0]); sub_cell_local[2].push_back(temp_vex_local[1]);
		sub_cell_local[2].push_back(temp_vex_local[9]); sub_cell_local[2].push_back(temp_vex_local[8]);
		sub_cell_local[2].push_back(temp_vex_local[4]); sub_cell_local[2].push_back(temp_vex_local[5]);
		sub_cell_local[2].push_back(temp_vex_local[13]); sub_cell_local[2].push_back(temp_vex_local[12]);
		//第四个单元 下
		sub_cell_local[3].push_back(temp_vex_local[11]); sub_cell_local[3].push_back(temp_vex_local[10]);
		sub_cell_local[3].push_back(temp_vex_local[2]); sub_cell_local[3].push_back(temp_vex_local[3]);
		sub_cell_local[3].push_back(temp_vex_local[15]); sub_cell_local[3].push_back(temp_vex_local[14]);
		sub_cell_local[3].push_back(temp_vex_local[6]); sub_cell_local[3].push_back(temp_vex_local[7]);
		//第五个单元 前
		sub_cell_local[4].push_back(temp_vex_local[9]); sub_cell_local[4].push_back(temp_vex_local[1]);
		sub_cell_local[4].push_back(temp_vex_local[2]); sub_cell_local[4].push_back(temp_vex_local[10]);
		sub_cell_local[4].push_back(temp_vex_local[13]); sub_cell_local[4].push_back(temp_vex_local[5]);
		sub_cell_local[4].push_back(temp_vex_local[6]); sub_cell_local[4].push_back(temp_vex_local[14]);
		//第6个单元 后
		sub_cell_local[5].push_back(temp_vex_local[0]); sub_cell_local[5].push_back(temp_vex_local[8]);
		sub_cell_local[5].push_back(temp_vex_local[11]); sub_cell_local[5].push_back(temp_vex_local[3]);
		sub_cell_local[5].push_back(temp_vex_local[4]); sub_cell_local[5].push_back(temp_vex_local[12]);
		sub_cell_local[5].push_back(temp_vex_local[15]); sub_cell_local[5].push_back(temp_vex_local[7]);
		//第7个单元 中心
		sub_cell_local[6].push_back(temp_vex_local[8]); sub_cell_local[6].push_back(temp_vex_local[9]);
		sub_cell_local[6].push_back(temp_vex_local[10]); sub_cell_local[6].push_back(temp_vex_local[11]);
		sub_cell_local[6].push_back(temp_vex_local[12]); sub_cell_local[6].push_back(temp_vex_local[13]);
		sub_cell_local[6].push_back(temp_vex_local[14]); sub_cell_local[6].push_back(temp_vex_local[15]);
		//插入单元
		//for (int j = 0; j < 8; j++) {
		//	newmesh.add_cell(sub_cell[j]);
		//}

		//插入单元
		//for (int j = 0; j < 7; j++) {
		//	newmesh_local->add_cell(sub_cell_local[j]);
		//}
		if (material_distribution[i] != 1)
		{
			material_distribution1[newmesh_local->add_cell(sub_cell[0])] = 0.0001;
			material_distribution1[newmesh_local->add_cell(sub_cell[1])] = 0.0001;
			material_distribution1[newmesh_local->add_cell(sub_cell[2])] = 0.0001;
			material_distribution1[newmesh_local->add_cell(sub_cell[3])] = 0.0001;
			material_distribution1[newmesh_local->add_cell(sub_cell[4])] = 0.0001;
			material_distribution1[newmesh_local->add_cell(sub_cell[5])] = 0.0001;
			material_distribution1[newmesh_local->add_cell(sub_cell[6])] = 0.0001;
			//std::vector<double> material_distribution_temp(8, 0.0001);
			//material_distribution_mutires.push_back(material_distribution_temp);
		}
		else
		{
			newmesh_local->add_cell(sub_cell[0]);
			newmesh_local->add_cell(sub_cell[1]);
			newmesh_local->add_cell(sub_cell[2]);
			newmesh_local->add_cell(sub_cell[3]);
			newmesh_local->add_cell(sub_cell[4]);
			newmesh_local->add_cell(sub_cell[5]);
			newmesh_local->add_cell(sub_cell[6]);
			//std::vector<double> material_distribution_temp(8, 1);
			//material_distribution_mutires.push_back(material_distribution_temp);
		}
	}
	material_distribution = material_distribution1;
	return *newmesh_local;
}


void HexMeshViewer::rsetxyView(){
	ui.widget->setxyView();
}

void HexMeshViewer::rsetyzView(){
	ui.widget->setyzView();
}

void HexMeshViewer::rsetxzView(){
	ui.widget->setxzView();
}

void HexMeshViewer::rsetxy_View(){
	ui.widget->setxy_View();
}

void HexMeshViewer::rsetyz_View(){
	ui.widget->setyz_View();
}

void HexMeshViewer::rsetxz_View(){
	ui.widget->setxz_View();
}

void HexMeshViewer::rnewNodeOK() {
	ui.widget->setmouse_event(MOVE_CELL_SKEL);
}

void HexMeshViewer::genCoarseMeshFinishMesh() {
	mesh = new HexV3fMesh();
	std::string stpath = "coarse.mesh";
	FileHandler file(stpath);
	file.read2HexMeshObj_new(mesh);
	qDebug() << mesh->cells_.size();

	material_distribution.resize(mesh->cells_.size(), 1);
	checkcell.resize(mesh->cells_.size(), false);
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	ui.widget->setmesh(mesh);
	ui.widget->set_scene_pos(mesh->center, mesh->r);
	qDebug() << mesh->r;
	ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	ui.widget->setbb(mesh->bbMax, mesh->bbMin);
	ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
	isneed = true;
	ui.widget->setmouse_event(DEFAULT);
	ui.widget->setdraw_mode_(FLAT_LINES);
	ui.widget->setoriv(mesh->oriv);
	vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
}

/**
 * @description: Add rotation animation
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rotate() {
	static int count = 0;
	count++;
	if (count%2) {
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), ui.widget, SLOT(rotation()));
		timer->start(10);
	}
	else {
		timer->stop();
	}
}

void HexMeshViewer::exportSkel() {
	QString path = QFileDialog::getSaveFileName(this, "save", "../", "SKEL(*.skel)");
	if (path.isEmpty() == false)
	{
		std::string name = path.toStdString();
		solutionSkel->saveSkel(name);
	}
}

void HexMeshViewer::blendSkel() {
	mesh->examine_bdy();
	mesh->choseframe();
	mesh->choseframeface();
	ui.widget->setframe(mesh->frame, mesh->frameface, mesh->framefacenrom);
	ui.widget->setdraw_mode_(BLENDSKEL);
	update();
}


void HexMeshViewer::addCellUseMouseSignal() {
	ui.widget->setmouse_event(SET_CELL_SKEL);
}

/**
 * @description: create Menu
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::createMenus()
{
	QMenu *menuLee = menuBar()->addMenu(tr("&Lee"));
	menuLee->addAction(actMLCA);
	menuLee->addAction(actMLCA_AVG);
	menuLee->addAction(actOpenSkel);
	menuLee->addAction(actExportSkel);
	menuLee->addAction(actBlend);
	menuLee->addAction(actWhirl);
	menuLee->addAction(actClearSkel);
	menuLee->addAction(actHelp);
	menuLee->addAction(actPIA);
	menuLee->addAction(actFaceNormChange);
	menuLee->addAction(actReadOpenComplexSkel);
	menuLee->addAction(actSimpleComplexSkel);
	//menuLee->addAction(actSimpleWithCgal);
	menuLee->addAction(actFromVolumeToOff);
	menuLee->addAction(actChangeNodeType);
	menuLee->addAction(actScaledJacobian);
	menuLee->addAction(actProject);
	menuLee->addAction(actExportBroSkel);
	menuLee->addAction(actFixFlip);
	menuLee->addAction(actOpenOff);
	menuLee->addAction(actShrinking);
	menuLee->addAction(actShowProjectLine);
	menuLee->addAction(actPickProjPoints);
	menuLee->addAction(actSkelMove);
}

/**
 * @description: spilt
 * @author: lee
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::subdivision()
{
	solutionSubDiv->split(*mesh);
	ui.widget->constructShowMesh();
	renewParam();
}

/**
 * @description: average 
 * @author: lee
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::average() {
	solutionSubDiv->average(*mesh);
	ui.widget->constructShowMesh();
}

/**
 * @description: read cgal gen file
 * @author: lee
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rreadTestLineSignal() {
	//"skel-poly.polylines.txt" 序号 + 点
	//"correspondance-poly.polylines.txt" 2 + 点 + 点
	std::string filename1 = "skel-poly.polylines.txt";
	std::ifstream iff = std::ifstream(filename1.c_str(), std::ios::in);
	if (!iff.good()) {
		std::cout << "[ERROR] could not open file " << filename1 << " for reading!" << std::endl;
		iff.close();
		return;
	}
	vector<vector<BMesh::Point3D>> skel;
	
	std::string lineStr;
	while (BMesh::common::getStrLine(iff, lineStr)) {
		std::stringstream sstr;
		sstr << lineStr;
		int number = 0;
		sstr >> number;
		vector<BMesh::Point3D> vs;
		for (int i = 0; i < number; i++) {
			double a, b, c;
			sstr >> a;
			sstr >> b;
			sstr >> c;
			BMesh::Point3D p(a, b, c);
			vs.push_back(p);
		}
		skel.push_back(vs);
	}
	iff.close();
	/*分段输出json文件*/
	exportJsonForCurveP(skel);
	/*
	std::string filename2 = "correspondance-poly.polylines.txt";
	std::ifstream iff2 = std::ifstream(filename2.c_str(), std::ios::in);
	if (!iff2.good()) {
		std::cout << "[ERROR] could not open file " << filename2 << " for reading!" << std::endl;
		iff2.close();
		return;
	}
	vector<vector<BMesh::Point3D>> lines;

	
	while (BMesh::common::getStrLine(iff2, lineStr)) {
		std::stringstream sstr;
		sstr << lineStr;
		int number = 0;
		sstr >> number;
		vector<BMesh::Point3D> vs;
		for (int i = 0; i < number; i++) {
			double a, b, c;
			sstr >> a;
			sstr >> b;
			sstr >> c;
			BMesh::Point3D p(a, b, c);
			vs.push_back(p);
		}
		lines.push_back(vs);
	}
	iff2.close();
	*/
	//ui.widget->setlines(lines);
	ui.widget->setSkel(skel);
	ui.widget->setdraw_mode_(TESTLINES);
}



/**
 * @description: export Json For Curve Interpolation
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::exportJsonForCurveP(vector<vector<BMesh::Point3D>>& skel) {
	// 冗余骨架的合并  --   还是先改成交互式删除.
	/*std::map<BMesh::Point3D, int> m;
	for (auto &it : skel) {
		for (auto& iit : it) {
			m[iit] += 1;
		}
	}
	std::vector<BMesh::Point3D> v;
	for (auto it : m) {
		if (it.second == 2) {
			v.push_back(it.first);
		}
	}*/
	std::ofstream out("interPCurveSkel.json");
	out << "{\n";
	out << "\"name\": \"interpolateCurves\",\n";
	out << "\"lines\": [ \n";
	
	for (int i = 0; i < skel.size(); i++) {
		if (skel[i].size() < 2) {
			std::cout << "[ERROR] : file skel line points less than 2\n";
			continue;
		}
		else if(skel[i].size() == 2){
			skel[i].push_back(skel[i][0]);
			skel[i].push_back(skel[i][1]);
		}
		else if (skel[i].size() == 3) {
			skel[i].push_back(skel[i][0]);
		}
		if (i == 0) {
			out << "{\n";
		}
		else {
			out << ",{\n";
		}
		out << "\"curvePointNum\": 100, \n";
		out << "\"p\": 3,\n";
		out << "\"points\": [\n";
		for (int j = 0; j < skel[i].size(); j++) {
			if (j == 0) {
				out << "[" << skel[i][j].x << "," << skel[i][j].y << "," << skel[i][j].z << "]";
			}
			else {
				out << ",[" << skel[i][j].x << "," << skel[i][j].y << "," << skel[i][j].z << "]\n";
			}
		}
		out << "]\n";
		out << "}\n";
	}
	out << "]}\n";
	out.close();
}


/**
 * @description: select Cell in SKEL mode
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::selectCell() {
	solutionSkel->selectNodes.clear();
	solutionSkel->setMesh(mesh);
	ui.widget->setmouse_event(PICK_CELL_SKEL);
}

/**
 * @description: select MutiCells in SKEL mode
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::selectMutiCells() {
	solutionSkel->setMesh(mesh);
	ui.widget->setmouse_event(PICK_CELLS_SKEL);
}
/**
 * @description: get select Node
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::delCell() {
	std::vector<CellHandle> dc = ui.widget->pickcell;

	std::cout << "[DEBUG] dcdc " << dc.size() << std::endl;
	if (dc.size() != 1) {
		return;
	}
	V3f tmp(0.0, 0.0, 0.0);
	for (int i = 0; i < 8; i++) {
		tmp += mesh->vertices_[mesh->cells_[dc[0]].vertex_[i]];
	}
	tmp = tmp / 8.0;
	// check with all node position
	BMesh::Node * rlt = solutionSkel->selectWhichNode(tmp);
	SingalFather::getInstance()->sdelNode();
}


void HexMeshViewer::changeNodeType() {
	if (solutionSkel->changeNodeType()) {
		std::cout << "node change to BN\n";
	}
}

/**
 * @description: edit Cell in SKEL mode
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::editCell() {
	std::vector<CellHandle> dc = ui.widget->pickcell;

	std::cout << "[DEBUG] dcdc " << dc.size() << std::endl;
	if (dc.size() != 1) {
		return;
	}
	V3f tmp(0.0, 0.0, 0.0);
	for (int i = 0; i < 8; i++) {
		tmp += mesh->vertices_[mesh->cells_[dc[0]].vertex_[i]];
	}
	tmp = tmp / 8.0;
	// check with all node position
	BMesh::Node * rlt = solutionSkel->selectWhichNode(tmp);
}

/**
 * @description: just update the cell not the all
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rupdateGL() {
	std::vector<CellHandle> dc = ui.widget->pickcell;
	std::cout << "[DEBUG] rupdateGL " << dc.size() << std::endl;
	// if (dc.size() != 1) {
	// 	return;
	// }
	if(dc.size() == 1) {
		for (int i = 0; i < 8; i++) {
			V3f t(solutionSkel->selectNode->root->p[i].x, solutionSkel->selectNode->root->p[i].y, solutionSkel->selectNode->root->p[i].z);
			mesh->vertices_[mesh->cells_[dc[0]].vertex_[i]] = t;
		}
	}
	else{
		for(int index = 0; index < solutionSkel->selectNodes.size(); index++){
			for (int i = 0; i < 8; i++) {
				V3f t(solutionSkel->selectNodes[index]->root->p[i].x, 
				solutionSkel->selectNodes[index]->root->p[i].y, 
				solutionSkel->selectNodes[index]->root->p[i].z);
				mesh->vertices_[mesh->cells_[dc[index]].vertex_[i]] = t;
			}
		}
	}
	
	if (solutionSkel->mirrorNode != NULL) {
		int csize = mesh->cells_.size();
		double mindis = 999; int mineid = 0;
		for (int i = 0; i < csize; i++)
		{
			V3f meshv(0, 0, 0);
			for (int cell_it = 0; cell_it < 8; cell_it++)
				meshv = meshv + mesh->vertices_[mesh->cells_[i].vertex_[cell_it]];
			meshv = meshv / 8.0;
			V3f temp_pick(solutionSkel->mirrorNode->getPosition().x, solutionSkel->mirrorNode->getPosition().y, solutionSkel->mirrorNode->getPosition().z);
			double dis = distance(meshv, temp_pick);
			if (dis < mindis)
			{
				mindis = dis;
				mineid = i;
			}
		}
		if (std::find(pickcell.begin(), pickcell.end(), CellHandle(mineid)) == pickcell.end()) // I think use set maybe better.
		{
			for (int i = 0; i < 8; i++) {
				V3f t(solutionSkel->mirrorNode->root->p[i].x, solutionSkel->mirrorNode->root->p[i].y, solutionSkel->mirrorNode->root->p[i].z);
				mesh->vertices_[mesh->cells_[CellHandle(mineid)].vertex_[i]] = t;
			}
		}
	}
	material_distribution.resize(mesh->cells_.size(), 1);
	checkcell.resize(mesh->cells_.size(), false);
	//mesh->cal_cen();
	//mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	ui.widget->setmesh(mesh);
	ui.widget->setNodeVec(solutionSkel->getNodeVec());
	ui.widget->setRoot(solutionSkel->getRoot());
	
	//ui.widget->set_scene_pos(mesh->center, mesh->r);
	qDebug() << mesh->r;
	//ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	//ui.widget->setbb(mesh->bbMax, mesh->bbMin);
	//ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
	isneed = true;
	//ui.widget->setdraw_mode_(SKEL);
	drawMode();
	//ui.widget->setoriv(mesh->oriv);
	vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
	update();
	ui.widget->update();
}

void HexMeshViewer::drawMode() {
	if (drawModeArg == 0) {
		ui.widget->setdraw_mode_(SKEL);
	}
	else if (drawModeArg == 1) {
		ui.widget->setdraw_mode_(SELECT_CELL);
	}
	else if (drawModeArg == 2) {
		ui.widget->setdraw_mode_(SELECT_CELLS);
	}
	ui.widget->updateGL();
}

void HexMeshViewer::rMouseRelease() {
	solutionSkel->snifDelta = BMesh::Point3D(0, 0, 0);
}

/**
 * @description: rselectALine let a select cube's line be selected
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rselectALine(){
	solutionSkel->setMesh(mesh);
	ui.widget->setmouse_event(PICK_CELLS_SKEL);
	ui.widget->setdraw_mode_(SELECT_CELLS);
	if(ui.widget->pickcell.size() == 1) {
		std::vector<CellHandle> dc = ui.widget->pickcell;
		V3f tmp(0.0, 0.0, 0.0);
		for (int i = 0; i < 8; i++) {
			tmp += mesh->vertices_[mesh->cells_[dc[0]].vertex_[i]];
		}
		tmp = tmp / 8.0;
		// check with all node position
		BMesh::Node * rlt = solutionSkel->selectWhichNode(tmp);
		std::vector<BMesh::Node *> ret;
		solutionSkel->getALine(ret);
		for(int i=0; i<ret.size(); i++) {
			V3f temp_pick(ret[i]->getPosition().x, ret[i]->getPosition().y, ret[i]->getPosition().z);

			int csize = mesh->cells_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < csize; i++) {
				V3f meshv(0, 0, 0);
				for (int cell_it = 0; cell_it < 8; cell_it++) {
					meshv = meshv + mesh->vertices_[mesh->cells_[i].vertex_[cell_it]];
				}
				meshv = meshv / 8;
				double dis = distance(meshv, temp_pick);
				if (dis < mindis) {
					mindis = dis;
					mineid = i;
				}
			}
			// find it
			std::vector<CellHandle>::iterator it;
			if ((it = std::find(ui.widget->pickcell.begin(), ui.widget->pickcell.end(), CellHandle(mineid))) != ui.widget->pickcell.end()) {
				// pickcell.erase(it);
			}
			else {
				ui.widget->pickcell.push_back(CellHandle(mineid));
			}
		}
	}
	ui.widget->update();
}


void HexMeshViewer::addCellUseMouseImplSignal() {
	V3f mouseP = ui.widget->mousePosition;
	SingalFather::getInstance()->saddNode(mouseP.x, mouseP.y, mouseP.z, 0, 0, 0, 1.0000123, 1.0000123, 1.0000123); // 1.0000123 is sign
	if (ui.widget->xPressed) {
		SingalFather::getInstance()->saddNode(solutionSkel->selectNode->getPosition().x + solutionSkel->selectNode->getPosition().x - mouseP.x, mouseP.y, mouseP.z, 0, 0, 0, 1.0000123, 1.0000123, 1.0000123);
	}
	if (ui.widget->yPressed) {
		SingalFather::getInstance()->saddNode(mouseP.x, solutionSkel->selectNode->getPosition().y + solutionSkel->selectNode->getPosition().y -mouseP.y, mouseP.z, 0, 0, 0, 1.0000123, 1.0000123, 1.0000123);
	}
	if (ui.widget->zPressed) {
		SingalFather::getInstance()->saddNode(mouseP.x, mouseP.y, solutionSkel->selectNode->getPosition().z + solutionSkel->selectNode->getPosition().z - mouseP.z, 0, 0, 0, 1.0000123, 1.0000123, 1.0000123);
	}
	if (ui.widget->oPressed) {
		SingalFather::getInstance()->saddNode(solutionSkel->selectNode->getPosition().x + solutionSkel->selectNode->getPosition().x - mouseP.x,
			solutionSkel->selectNode->getPosition().y + solutionSkel->selectNode->getPosition().y - mouseP.y,
			solutionSkel->selectNode->getPosition().z + solutionSkel->selectNode->getPosition().z - mouseP.z,
			0, 0, 0,
			1.0000123, 1.0000123, 1.0000123);
	}
}

void HexMeshViewer::reNewDataSignal() {
	std::vector<CellHandle> dc = ui.widget->pickcell;

	std::cout << "[DEBUG] dcdc " << dc.size() << std::endl;
	if (dc.size() != 1) {
		return;
	}
	V3f tmp(0.0, 0.0, 0.0);
	for (int i = 0; i < 8; i++) {
		tmp += mesh->vertices_[mesh->cells_[dc[0]].vertex_[i]];
	}
	tmp = tmp / 8.0;
	// check with all node position
	BMesh::Node * rlt = solutionSkel->selectWhichNode(tmp);
	SingalFather::getInstance()->getData(rlt->getPosition().x, rlt->getPosition().y, rlt->getPosition().z, 
		rlt->root->getRotateX(), rlt->root->getRotateY(), rlt->root->getRotateZ(),	
		rlt->root->getLength(), rlt->root->getWidth(), rlt->root->getHeight());
}

void HexMeshViewer::drawNormalSkel() {
	// clear mesh
	ui.widget->clear_mesh();
	mesh = new HexV3fMesh();
	isneed = false;
	update();
	// --
	HexV3fMesh* meshT = new HexV3fMesh();
	solutionSkel->genViewHex(*meshT);
	// diff ?
	if (meshT->cells_.size() == mesh->cells_.size()) {
		for (int i = 0; i < meshT->vertices_.size(); i++) {
			mesh->vertices_[i] = meshT->vertices_[i];
		}
	}
	else {
		*mesh = *meshT;
	}
	std::cout << "dafdsafdafd\n";
	// --
	static int signal_reDraw = 1;
	// --
	if (signal_reDraw) {
		material_distribution.resize(mesh->cells_.size(), 1);
		checkcell.resize(mesh->cells_.size(), false);
		mesh->cal_cen();
		mesh->cal_cell_cen();
		mesh->examine_bdy();
		mesh->examine_cs();
		mesh->cal_cen1();
		mesh->examine_bdfv();
		mesh->cal_cell_cen();
		mesh->cal_bdnorm();
		mesh->cal_mami_ed();
		mesh->cal_oriv();
		ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
		ui.widget->setmesh(mesh);
		ui.widget->setRoot(solutionSkel->getRoot());
		ui.widget->setNodeVec(solutionSkel->getNodeVec());
		ui.widget->set_scene_pos(mesh->center, mesh->r);
		qDebug() << mesh->r;
		ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
		ui.widget->setbb(mesh->bbMax, mesh->bbMin);
		ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
		isneed = true;
		//ui.widget->setdraw_mode_(SELECT_CELL);
		drawMode();
		ui.widget->setoriv(mesh->oriv);
		vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
		signal_reDraw = 0;
	}
	else {
		//std::cout << "debuggg===\n";
		material_distribution.resize(mesh->cells_.size(), 1);
		checkcell.resize(mesh->cells_.size(), false);
		//mesh->cal_cen();
		//mesh->cal_cell_cen();
		mesh->examine_bdy();
		//mesh->examine_cs();
		//mesh->cal_cen1();
		mesh->examine_bdfv();
		//mesh->cal_cell_cen();
		mesh->cal_bdnorm();
		mesh->cal_mami_ed();
		mesh->cal_oriv();
		ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
		ui.widget->setmesh(mesh);
		ui.widget->setRoot(solutionSkel->getRoot());
		ui.widget->setNodeVec(solutionSkel->getNodeVec());
		//ui.widget->set_scene_pos(mesh->center, mesh->r);
		qDebug() << mesh->r;
		//ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
		//ui.widget->setbb(mesh->bbMax, mesh->bbMin);
		//ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
		isneed = true;
		//ui.widget->setdraw_mode_(SELECT_CELL);
		drawMode();
		//ui.widget->setoriv(mesh->oriv);
		vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
		update();
	}
}

void HexMeshViewer::rxExpand() {
	solutionSkel->xExpand();
	rupdateGL();
}
void HexMeshViewer::ryExpand() {
	solutionSkel->yExpand();
	rupdateGL();
}
void HexMeshViewer::rzExpand() {
	solutionSkel->zExpand();
	rupdateGL();
}
void HexMeshViewer::rxCrop() {
	solutionSkel->xCrop();
	rupdateGL();
}
void HexMeshViewer::ryCrop() {
	solutionSkel->yCrop();
	rupdateGL();
}
void HexMeshViewer::rzCrop() {
	solutionSkel->zCrop();
	rupdateGL();
}


void HexMeshViewer::rconnectNodes() {
	std::vector<BMesh::Node*> vv;
	for (int i = 0; i < ui.widget->pickcell.size(); i++) {
		V3f meshv(0, 0, 0);
		for (int cell_it = 0; cell_it < 8; cell_it++)
			meshv = meshv + mesh->vertices_[mesh->cells_[ui.widget->pickcell[i]].vertex_[cell_it]];
		meshv = meshv / 8.0;
		BMesh::Node * node1 = solutionSkel->selectWhichNode(meshv);
		if (node1 != nullptr) {
			vv.push_back(node1);
		}
	}
	// connect vv
	for (int i = 0; i < vv.size() - 1 && vv.size(); i++) {
		for(int j=i+1; j<vv.size(); j++){
			solutionSkel->connectNodes(vv[i], vv[j]);
		}
	}
	ui.widget->setNodeVec(solutionSkel->getNodeVec());
	renewParam();
}

void HexMeshViewer::rdisconnectNodes() {
	std::vector<BMesh::Node*> vv;
	for (int i = 0; i < ui.widget->pickcell.size(); i++) {
		V3f meshv(0, 0, 0);
		for (int cell_it = 0; cell_it < 8; cell_it++)
			meshv = meshv + mesh->vertices_[mesh->cells_[ui.widget->pickcell[i]].vertex_[cell_it]];
		meshv = meshv / 8.0;
		BMesh::Node* node1 = solutionSkel->selectWhichNode(meshv);
		if (node1 != nullptr) {
			vv.push_back(node1);
		}
	}
	// connect vv
	for (int i = 0; i < vv.size() - 1 && vv.size(); i++) {
		for (int j = i + 1; j < vv.size(); j++) {
			solutionSkel->disconnectNodes(vv[i], vv[j]);
		}
	}
	ui.widget->setNodeVec(solutionSkel->getNodeVec());
	renewParam();
}

/**
 * @description: read skeleton
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::readSkel() {
	QString path = QFileDialog::getOpenFileName(this, "open",
		"D:/code/dong/Data/skel", "Skel(*.skel)");
	// clear mesh
	ui.widget->clear_mesh();
	delete mesh;
	mesh = new HexV3fMesh();
	isneed = false;
	
	/*solutionSkel = new SolutionSkel();*/
	std::string stpath = path.toStdString();
	if (!solutionSkel->readSkel(stpath)) {
		std::cout << "[DEBUG] read skel error" << std::endl;
		return;
	}
	std::cout << "[DEBUG] read skel success" << std::endl;
	// let Bmesh hex to mesh
	solutionSkel->genViewHex((*mesh));
	// show it
	material_distribution.resize(mesh->cells_.size(), 1);
	checkcell.resize(mesh->cells_.size(), false);
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	ui.widget->setmesh(mesh);
	ui.widget->setRoot(solutionSkel->getRoot());
	ui.widget->setNodeVec(solutionSkel->getNodeVec());
	ui.widget->set_scene_pos(mesh->center, mesh->r);
	qDebug() << mesh->r;
	ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	ui.widget->setbb(mesh->bbMax, mesh->bbMin);
	ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
	isneed = true;
	ui.widget->setdraw_mode_(SKEL);
	ui.widget->setoriv(mesh->oriv);
	vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
}

/**
 * @description: open mesh file and open vtk file
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::openMeshFile()
{
	QString path = QFileDialog::getOpenFileName(this,
		tr("Open file"),
		tr("D:/code/dong/Data"),
		tr("Mesh Files (*.mesh);;"
			"VTK Files (*.vtk);;"
			"All Files (*)"));
	mesh = new HexV3fMesh();
	std::string stpath = path.toStdString();
	FileHandler file(stpath);
	file.readFile(mesh);
	qDebug() << mesh->cells_.size();
	material_distribution.resize(mesh->cells_.size(), 1);
	checkcell.resize(mesh->cells_.size(), false);
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	ui.widget->setmesh(mesh);
	if (isneed == false) {
		ui.widget->set_scene_pos(mesh->center, mesh->r);
		ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
		ui.widget->setbb(mesh->bbMax, mesh->bbMin);
		ui.widget->setoribb(mesh->bbMax, mesh->bbMin);
		isneed = true;
	}
	ui.widget->setdraw_mode_(FLAT_LINES);
	ui.widget->setoriv(mesh->oriv);
	vH.push_back({ mesh->boundary_face_v, mesh->normal_boundary_face_v });
}

/**
 * @description: rotateCube
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rRotateCubeByMouse(V3f axis, double angle) {
	solutionSkel->rotateCubeByMouse(axis, angle);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}
void HexMeshViewer::rxRotateCell(int upOrDown) {
	solutionSkel->rotateCube(0, upOrDown);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}
void HexMeshViewer::ryRotateCell(int upOrDown) {
	solutionSkel->rotateCube(1, upOrDown);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}
void HexMeshViewer::rzRotateCell(int upOrDown) {
	solutionSkel->rotateCube(2, upOrDown);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}


void HexMeshViewer::rspiltX() {
	solutionSkel->spiltX();
	drawNormalSkel();
}


void HexMeshViewer::rspiltY() {
	solutionSkel->spiltY();
	drawNormalSkel();
}

void HexMeshViewer::rspiltZ() {
	solutionSkel->spiltZ();
	drawNormalSkel();
}


/**
 * @description: move cell
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::rupMoveCell(int){
	solutionSkel->moveCube(2, 1);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}
void HexMeshViewer::rdownMoveCell(int){
	solutionSkel->moveCube(2, -1);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}
void HexMeshViewer::rfrontMoveCell(int){
	solutionSkel->moveCube(1, 1);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}
void HexMeshViewer::rbackMoveCell(int){
	solutionSkel->moveCube(1, -1);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}


void HexMeshViewer::pickProjPoints() {
	ui.widget->setmouse_event(PICK_POINT_SKEL);
}

void HexMeshViewer::rleftMoveCell(int){
	solutionSkel->moveCube(0, -1);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}
void HexMeshViewer::rrightMoveCell(int){
	solutionSkel->moveCube(0, 1);
	if(solutionSkel->state == haveGenJN){
		solutionSkel->generateLinkBody();
	}else {
		rupdateGL();
	}
	if(solutionSkel->selectNode != NULL) {
		SingalFather::getInstance()->getData(solutionSkel->selectNode->getPosition().x, solutionSkel->selectNode->getPosition().y, 
		solutionSkel->selectNode->getPosition().z, solutionSkel->selectNode->root->getRotateX(), 
		solutionSkel->selectNode->root->getRotateY(), solutionSkel->selectNode->root->getRotateZ(),	
		solutionSkel->selectNode->root->getLength(), solutionSkel->selectNode->root->getWidth(), solutionSkel->selectNode->root->getHeight());
	}
}


/**
 * @description: readXJ??
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::readXJ()
{
	QString path = QFileDialog::getOpenFileName(this, "open", "E:/DatResult/", "Mesh(*.mesh)");
	std::string stpath = path.toStdString();
	FileHandler file(stpath);
	if (times == 0)
	{
		mesh_1 = new HexV3fMesh();
		file.readXJ(mesh_1, m_color1);
		times++;
		mesh_1->examine_bdy();
		mesh_1->cal_cen();
		mesh_1->examine_bdfv();
		mesh_1->cal_cell_cen();
		mesh_1->cal_bdnorm();
		mesh_1->cal_mami_ed();
		ui.widget->setmesh(mesh_1);
		ui.widget->setbffv(mesh_1->boundary_face_v, mesh_1->normal_boundary_face_v, mesh_1->boundary_face, mesh_1->boundary_face_VID);
		ui.widget->set_scene_pos(mesh_1->center, mesh_1->r);
		ui.widget->setcell_center(mesh_1->cell_center);
		ui.widget->seted(mesh_1->maxEdgeLen, mesh_1->minEdgeLen);
		ui.widget->setbb(mesh_1->bbMax, mesh_1->bbMin);
		ui.widget->set_color(m_color1);
		std::cout << "OK" << endl;
		ui.widget->setdraw_mode_(Error);
	}
	else
	{
		mesh_2 = new HexV3fMesh();
		file.readXJ(mesh_2, m_color2);
		times++;
		mesh_2->examine_bdy();
		mesh_2->cal_cen();
		mesh_2->examine_bdfv();
		mesh_2->cal_cell_cen();
		mesh_2->cal_bdnorm();
		mesh_2->cal_mami_ed();
		ui.widget->setmesh(mesh_2);
		ui.widget->setbffv(mesh_2->boundary_face_v, mesh_2->normal_boundary_face_v, mesh_2->boundary_face, mesh_2->boundary_face_VID);
		ui.widget->set_scene_pos(mesh_2->center, mesh_2->r);
		ui.widget->setcell_center(mesh_2->cell_center);
		ui.widget->seted(mesh_2->maxEdgeLen, mesh_2->minEdgeLen);
		ui.widget->setbb(mesh_2->bbMax, mesh_2->bbMin);
		ui.widget->set_color(m_color2);
		std::cout << "OK" << endl;
		ui.widget->setdraw_mode_(Error);
	}
}



/**
 * @description: actionCombine
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::combine()
{
	HexV3fMesh *newmesh = new HexV3fMesh();
	ApproximationCC AppCC(mesh, newmesh);
	AppCC.CCapproximation1();
	*mesh = AppCC.constructBezierSolid();
	mesh->examine_bdy();
	mesh->cal_cen();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	ui.widget->setmesh(mesh);
	ui.widget->setbffv(mesh->boundary_face_v, mesh->normal_boundary_face_v, mesh->boundary_face, mesh->boundary_face_VID);
	ui.widget->set_scene_pos(mesh->center, mesh->r);
	ui.widget->setcell_center(mesh->cell_center);
	ui.widget->seted(mesh->maxEdgeLen, mesh->minEdgeLen);
	ui.widget->setbb(mesh->bbMax, mesh->bbMin);

	//�ڶ������� ϸ��Mesh1
	HexV3fMesh *newmesh1 = new HexV3fMesh();
	mesh1->examine_bdy();
	CCSubdivision sub1(mesh1.get(), newmesh1);
	*mesh1 = sub1.CCSubdivising();

	HexV3fMesh *newmesh2 = new HexV3fMesh();
	mesh1->examine_bdy();
	CCSubdivision sub2(mesh1.get(), newmesh2);
	*mesh1 = sub2.CCSubdivising();

	HexV3fMesh *newmesh3 = new HexV3fMesh();
	mesh1->examine_bdy();
	CCSubdivision sub3(mesh1.get(), newmesh3);
	*mesh1 = sub3.CCSubdivising();

	HexV3fMesh *newmesh4 = new HexV3fMesh();
	mesh1->examine_bdy();
	CCSubdivision sub4(mesh1.get(), newmesh4);
	*mesh1 = sub4.CCSubdivising();

	mesh1->examine_bdy();
	mesh1->cal_cen();
	mesh1->examine_bdfv();
	mesh1->cal_cell_cen();
	mesh1->cal_bdnorm();
	mesh1->cal_mami_ed();

	//���������� �Ƚ�Mesh��Mesh1
	Sort stm(mesh, mesh1.get());
	stm.sorting();
	stm.comparing();
	ui.widget->setdrawv_big_small(stm.drawvbig, stm.drawvsmall, stm.drawvbigid);
	ui.widget->seterrmax_min(stm.max, stm.min);
	ui.widget->set_color(stm.color, stm.colorverr);

	//���ĸ����� �л��滭ģʽ
	ui.widget->setdraw_mode_(Error);
	update();
}



void HexMeshViewer::cal_cen(std::vector<HexV3fMesh> &_dat)
{
	DATcellcen.swap(std::vector<V3f>());
	double cen_x = 0, cen_y = 0, cen_z = 0;
	double max = 0;
	double max_x = -999, max_y = -999, max_z = -999;
	double min_x = 999, min_y = 999, min_z = 999;
	int datnum = _dat.size();
	int temp = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = _dat.begin(); dat_it != _dat.end(); dat_it++)
	{
		dat_it->cal_cen();
		DATcellcen.push_back(dat_it->center);
		DATcenter = DATcenter + dat_it->center;
		temp++;
	}
	DATcenter = DATcenter / datnum;
	double max1 = 0, min1 = 999;
	for (std::vector<HexV3fMesh>::iterator dat_it = _dat.begin(); dat_it != _dat.end(); dat_it++)
	{
		for (std::vector<V3f>::iterator it = dat_it->vertices_.begin(); it != dat_it->vertices_.end(); it++)
		{
			double maxr = ((*it) - DATcenter).norm();
			if (maxr > max)
				max = maxr;
			if ((*it).x > max_x)
				max_x = (*it).x;
			if ((*it).x < min_x)
				min_x = (*it).x;
			if ((*it).y > max_y)
				max_y = (*it).y;
			if ((*it).y < min_y)
				min_y = (*it).y;
			if ((*it).z > max_z)
				max_z = (*it).z;
			if ((*it).z < min_z)
				min_z = (*it).z;
		}
		for (std::vector<Edge>::iterator e_it = dat_it->edges_.begin(); e_it != dat_it->edges_.end(); e_it++)
		{
			double dis = (dat_it->vertices_[(*e_it).from_h] - dat_it->vertices_[(*e_it).to_h]).norm();
			if (dis > max1)
				max1 = dis;
			if (dis < min1)
				min1 = dis;
		}
	}
	DATr = max;
	DATbbMax = V3f(max_x, max_y, max_z);
	DATbbMin = V3f(min_x, min_y, min_z);
	DATminEdgeLen = min1;
	DATmaxEdgeLen = max1;
}

void HexMeshViewer::cal_bdy(std::vector<HexV3fMesh> &_dat)
{
	DATbdy.swap(std::vector<std::vector<bool>>());
	DATedgever.swap(std::vector<std::vector<bool>>());
	std::vector<V3f> check;
	for (std::vector<HexV3fMesh>::iterator dat_it = _dat.begin(); dat_it != _dat.end(); dat_it++)
	{
		dat_it->examine_bdy();
		std::vector<V3f> mv = dat_it->vertices_;
		for (std::vector<V3f>::iterator mv_it = mv.begin(); mv_it != mv.end(); mv_it++)
		{
			check.push_back(*mv_it);
		}
		DATbdy.push_back(dat_it->v_bdy);
		DATedgever.push_back(std::vector<bool>());
	}
	//DATedgever = DATbdy;
	int di = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = _dat.begin(); dat_it != _dat.end(); dat_it++)
	{
		std::vector<V3f> mv = dat_it->vertices_;
		int i = 0;
		for (std::vector<V3f>::iterator mv_it = mv.begin(); mv_it != mv.end(); mv_it++)
		{
			if (dat_it->is_bdy(VertexHandle(i)))
			{
				int same = std::count(check.begin(), check.end(), *mv_it);
				if (same > 1)
				{
					DATbdy[di][VertexHandle(i)] = false;

					//if(dat_it->neighbor_v[i].size() == 3)
					//    DATedgever[di][VertexHandle(i)] = true;//�õ������߿ɻ�
					//else
					//	DATedgever[di][VertexHandle(i)] = false;
				}
				//else
				//{
				//	DATedgever[di][VertexHandle(i)] = false;//�õ������߲��ɻ�
				//}
			}

			i++;
		}

		std::vector<Edge> me = dat_it->edges_;
		int mesize = me.size();
		DATedgever[di].resize(mesize, false);

		for (int i = 0; i < mesize; i++)
		{
			if (dat_it->neighbor_e[i].size() == 2)
			{
				VertexHandle fr = dat_it->edges_[i].from_h;
				VertexHandle to = dat_it->edges_[i].to_h;
				if (DATbdy[di][fr] == false && DATbdy[di][to] == false)
					DATedgever[di][i] = true;
				else
					DATedgever[di][i] = false;
			}
			else
				DATedgever[di][i] = false;
		}

		di++;
	}
}

V3f HexMeshViewer::cal_vector_center(std::vector<V3f> _v)
{
	V3f center(0, 0, 0);
	int num = _v.size();
	for (std::vector<V3f>::iterator _v_it = _v.begin(); _v_it != _v.end(); _v_it++)
	{
		center = center + (*_v_it);
	}
	center = center / num;
	return center;
}

double HexMeshViewer::cal_vector_r(std::vector<V3f> _v, V3f _center)
{
	double r;
	double max = 0;
	for (std::vector<V3f>::iterator _v_it = _v.begin(); _v_it != _v.end(); _v_it++)
	{
		r = sqrt((_center.x - (*_v_it).x) * (_center.x - (*_v_it).x) + (_center.y - (*_v_it).y) * (_center.y - (*_v_it).y) + (_center.z - (*_v_it).z) * (_center.z - (*_v_it).z));
		if (r > max)
			max = r;
	}
	return max;
}

HexV3fMesh HexMeshViewer::constructBezierSolid(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color)
{
	HexV3fMesh beziermesh;
	int solidnum = SolidData.size();
	int size = 2;
	m_color.resize((size + 1) * (size + 1) * (size + 1) * solidnum);
	for (int solid_id = 0; solid_id < solidnum; solid_id++)
	{
		int m_uptnum = m_solids.Local_Node[solid_id][0].size() - m_solids.Local_Order[solid_id][0] - 1;
		int m_vptnum = m_solids.Local_Node[solid_id][1].size() - m_solids.Local_Order[solid_id][1] - 1;
		int m_wptnum = m_solids.Local_Node[solid_id][2].size() - m_solids.Local_Order[solid_id][2] - 1;
		std::vector<std::vector<std::vector<V3f>>> bsdata = SolidData[solid_id];
		BezierSolid BS(bsdata);
		//m_color[ii].resize((size + 1)*(size + 1)*(size + 1));
		std::vector<std::vector<V3f>> bezierv(size + 1);
		std::vector<std::vector<double>> colortemp(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1) * (size + 1); j++)
			{
				double u = j % (size + 1) * (1.0 / size);
				double v = j / (size + 1) * (1.0 / size);
				double w = i % (size + 1) * (1.0 / size);
				bezierv[i].push_back(BS.P(u, v, w));
				double m_approxsol = 0;
				double m_uder[2], m_vder[2], m_wder[2];
				for (int ii = 0; ii < m_uptnum; ii++)
				{
					m_solids.DersOneBasisFun(solid_id, 0, ii, u, 1, m_uder);
					for (int jj = 0; jj < m_vptnum; jj++)
					{
						m_solids.DersOneBasisFun(solid_id, 1, jj, v, 1, m_vder);
						for (int kk = 0; kk < m_wptnum; kk++)
						{
							m_solids.DersOneBasisFun(solid_id, 2, kk, w, 1, m_wder);
							m_approxsol += m_bdycoef[solid_id][m_solids.Control_Point_id[solid_id][kk][jj][ii]] * m_uder[0] * m_vder[0] * m_wder[0];
						}
					}
				}
				colortemp[i].push_back(m_approxsol);
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size * size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				int temp = 0;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t]));
				m_color[ctemp[temp++]] = colortemp[j][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 1];
				beziermesh.add_cell(ctemp);
			}
		}
	}
	return beziermesh;
}

HexV3fMesh HexMeshViewer::constructBezierSolid(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color, int n)
{
	HexV3fMesh beziermesh;
	int solidnum = SolidData.size();
	int size = n; //Ҫ����size*size*size����
	m_color.resize((size + 1) * (size + 1) * (size + 1) * solidnum);
	for (int solid_id = 0; solid_id < solidnum; solid_id++)
	{
		int m_uptnum = m_solids.Local_Node[solid_id][0].size() - m_solids.Local_Order[solid_id][0] - 1;
		int m_vptnum = m_solids.Local_Node[solid_id][1].size() - m_solids.Local_Order[solid_id][1] - 1;
		int m_wptnum = m_solids.Local_Node[solid_id][2].size() - m_solids.Local_Order[solid_id][2] - 1;
		std::vector<std::vector<std::vector<V3f>>> bsdata = SolidData[solid_id];
		BezierSolid BS(bsdata);
		//m_color[ii].resize((size + 1)*(size + 1)*(size + 1));
		std::vector<std::vector<V3f>> bezierv(size + 1);
		std::vector<std::vector<double>> colortemp(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1) * (size + 1); j++)
			{
				double u = j % (size + 1) * (1.0 / size);
				double v = j / (size + 1) * (1.0 / size);
				double w = i % (size + 1) * (1.0 / size);
				bezierv[i].push_back(BS.P(u, v, w));
				double m_approxsol = 0;
				double m_uder[2], m_vder[2], m_wder[2];
				for (int ii = 0; ii < m_uptnum; ii++)
				{
					m_solids.DersOneBasisFun(solid_id, 0, ii, u, 1, m_uder);
					for (int jj = 0; jj < m_vptnum; jj++)
					{
						m_solids.DersOneBasisFun(solid_id, 1, jj, v, 1, m_vder);
						for (int kk = 0; kk < m_wptnum; kk++)
						{
							m_solids.DersOneBasisFun(solid_id, 2, kk, w, 1, m_wder);
							m_approxsol += m_bdycoef[solid_id][m_solids.Control_Point_id[solid_id][kk][jj][ii]] * m_uder[0] * m_vder[0] * m_wder[0];
						}
					}
				}
				colortemp[i].push_back(m_approxsol);
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size * size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				int temp = 0;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t]));
				m_color[ctemp[temp++]] = colortemp[j][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 1];
				beziermesh.add_cell(ctemp);
			}
		}
	}
	return beziermesh;
}

void HexMeshViewer::simsor()
{
	int n = sensitivity1.size();
	for (int i = 0; i < n; i++)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (sensitivity1[j].senitiv < sensitivity1[min].senitiv)
				min = j;
		}
		std::swap(sensitivity1[i], sensitivity1[min]);
	}
}




void HexMeshViewer::simsor1()
{
	int n = sort_gap.size();
	for (int i = 0; i < n; i++)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (sort_gap[j].key < sort_gap[min].key)
				min = j;
		}
		std::swap(sort_gap[i], sort_gap[min]);
	}
}

HexV3fMesh HexMeshViewer::constructBezierSolid1(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color)
{
	HexV3fMesh beziermesh;
	int solidnum = SolidData.size();
	int size = 2; //Ҫ����size*size*size����
	m_color.resize((size + 1) * (size + 1) * (size + 1) * solidnum);
	for (int solid_id = 0; solid_id < solidnum; solid_id++)
	{
		int m_uptnum = m_solids.Local_Node[solid_id][0].size() - m_solids.Local_Order[solid_id][0] - 1;
		int m_vptnum = m_solids.Local_Node[solid_id][1].size() - m_solids.Local_Order[solid_id][1] - 1;
		int m_wptnum = m_solids.Local_Node[solid_id][2].size() - m_solids.Local_Order[solid_id][2] - 1;
		std::vector<std::vector<std::vector<V3f>>> bsdata = SolidData[solid_id];
		BezierSolid BS(bsdata);
		//m_color[ii].resize((size + 1)*(size + 1)*(size + 1));
		std::vector<std::vector<V3f>> bezierv(size + 1);
		std::vector<std::vector<double>> colortemp(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1) * (size + 1); j++)
			{
				double u = j % (size + 1) * (1.0 / size);
				double v = j / (size + 1) * (1.0 / size);
				double w = i % (size + 1) * (1.0 / size);
				bezierv[i].push_back(BS.P(u, v, w));
				double m_approxsol = 0;
				double m_uder[2], m_vder[2], m_wder[2];
				for (int ii = 0; ii < m_uptnum; ii++)
				{
					m_solids.DersOneBasisFun(solid_id, 0, ii, u, 1, m_uder);
					for (int jj = 0; jj < m_vptnum; jj++)
					{
						m_solids.DersOneBasisFun(solid_id, 1, jj, v, 1, m_vder);
						for (int kk = 0; kk < m_wptnum; kk++)
						{
							m_solids.DersOneBasisFun(solid_id, 2, kk, w, 1, m_wder);
							m_approxsol += m_bdycoef[solid_id][m_solids.Control_Point[solid_id][kk][jj][ii]] * m_uder[0] * m_vder[0] * m_wder[0];
						}
					}
				}
				colortemp[i].push_back(m_approxsol);
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size * size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				int temp = 0;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t]));
				m_color[ctemp[temp++]] = colortemp[j][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 1];
				beziermesh.add_cell(ctemp);
			}
		}
	}
	return beziermesh;
}

HexV3fMesh HexMeshViewer::constructBezierSolid1(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color, int n)
{
	HexV3fMesh beziermesh;
	int solidnum = SolidData.size();
	int size = n; //Ҫ����size*size*size����
	m_color.resize((size + 1) * (size + 1) * (size + 1) * solidnum);
	for (int solid_id = 0; solid_id < solidnum; solid_id++)
	{
		int m_uptnum = m_solids.Local_Node[solid_id][0].size() - m_solids.Local_Order[solid_id][0] - 1;
		int m_vptnum = m_solids.Local_Node[solid_id][1].size() - m_solids.Local_Order[solid_id][1] - 1;
		int m_wptnum = m_solids.Local_Node[solid_id][2].size() - m_solids.Local_Order[solid_id][2] - 1;
		std::vector<std::vector<std::vector<V3f>>> bsdata = SolidData[solid_id];
		BezierSolid BS(bsdata);
		//m_color[ii].resize((size + 1)*(size + 1)*(size + 1));
		std::vector<std::vector<V3f>> bezierv(size + 1);
		std::vector<std::vector<double>> colortemp(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1) * (size + 1); j++)
			{
				double u = j % (size + 1) * (1.0 / size);
				double v = j / (size + 1) * (1.0 / size);
				double w = i % (size + 1) * (1.0 / size);
				bezierv[i].push_back(BS.P(u, v, w));
				double m_approxsol = 0;
				double m_uder[2], m_vder[2], m_wder[2];
				for (int ii = 0; ii < m_uptnum; ii++)
				{
					m_solids.DersOneBasisFun(solid_id, 0, ii, u, 1, m_uder);
					for (int jj = 0; jj < m_vptnum; jj++)
					{
						m_solids.DersOneBasisFun(solid_id, 1, jj, v, 1, m_vder);
						for (int kk = 0; kk < m_wptnum; kk++)
						{
							m_solids.DersOneBasisFun(solid_id, 2, kk, w, 1, m_wder);
							m_approxsol += m_bdycoef[solid_id][m_solids.Control_Point[solid_id][kk][jj][ii]] * m_uder[0] * m_vder[0] * m_wder[0];
						}
					}
				}
				colortemp[i].push_back(m_approxsol);
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size * size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				int temp = 0;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t]));
				m_color[ctemp[temp++]] = colortemp[j][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 1];
				beziermesh.add_cell(ctemp);
			}
		}
	}
	return beziermesh;
}

std::vector<std::vector<V3f>> HexMeshViewer::cal_norm(std::vector<HexV3fMesh> &_dat)
{
	std::vector<std::vector<V3f>> norm;
	for (int i = 0; i < _dat.size(); ++i)
	{
		std::vector<Face> datface = _dat[i].faces_;
		std::vector<V3f> tempnorm(datface.size(), V3f(0, 0, 0));
		for (int j = 0; j < datface.size(); ++j)
		{
			if (_dat[i].is_bdy(FaceHandle(j)))
			{
				V3f norm1 = _dat[i].cal_norm(FaceHandle(j));
				tempnorm[j] = norm1;
			}
		}
		norm.push_back(tempnorm);
	}
	return norm;
}

/**
 * @description: updatesimple
 * @param {*}
 * @return {*}
 */
void HexMeshViewer::renewParam()
{
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();
	update();
	ui.widget->update();
}

void HexMeshViewer::updatedrawx()
{
	Transformation t(mesh);
	t.translation(xmove, 0, 0);
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();

	mesh->smooth_mesh();
	mesh->smooth_mesh();
	mesh->smooth_mesh();
	HexV3fMesh *newmesh = new HexV3fMesh();
	ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
	AppCC.approximationingSharp();
	AppCC.constructBezier(1);
	AppCC.cal_nor_beisur(0.1);
	AppCC.cal_beisurv_center(1);
	mesh->checksingualrface();
	beziersurface bftemp;
	bftemp.biaojiface = AppCC.biaojiface;
	bftemp.bei_surv = AppCC.bei_surv;
	bftemp.bei_surv_dat = AppCC.bei_surv_dat;
	bftemp.tempdraw_num = mesh->faces_.size();
	bftemp.nor_bei_surv = AppCC.nor_bei_surv;
	bftemp.nor_bei_surv_dat = AppCC.nor_bei_surv_dat;
	bftemp.bei_surv_center = AppCC.bei_surv_center;
	bftemp.edgeline = AppCC.edgeline;
	bftemp.singularface = mesh->singularface;
	if (!bf.empty())
	{
		std::cout << "Σ�յش�";
		bf.pop_back();
	}
	bf.push_back(bftemp);
	ui.widget->setbf(bf);
	ui.widget->setdraw_mode_(BSVECTOR);
	ui.widget->pickedge_e.clear();
	update();
}

void HexMeshViewer::updatedrawz()
{
	Transformation t(mesh);
	t.translation(0, 0, zmove);
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();

	mesh->smooth_mesh();
	mesh->smooth_mesh();
	mesh->smooth_mesh();
	HexV3fMesh *newmesh = new HexV3fMesh();
	ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
	AppCC.approximationingSharp();
	AppCC.constructBezier(1);
	AppCC.cal_nor_beisur(0.1);
	AppCC.cal_beisurv_center(1);
	mesh->checksingualrface();
	beziersurface bftemp;
	bftemp.biaojiface = AppCC.biaojiface;
	bftemp.bei_surv = AppCC.bei_surv;
	bftemp.bei_surv_dat = AppCC.bei_surv_dat;
	bftemp.tempdraw_num = mesh->faces_.size();
	bftemp.nor_bei_surv = AppCC.nor_bei_surv;
	bftemp.nor_bei_surv_dat = AppCC.nor_bei_surv_dat;
	bftemp.bei_surv_center = AppCC.bei_surv_center;
	bftemp.edgeline = AppCC.edgeline;
	bftemp.singularface = mesh->singularface;
	bf.pop_back();
	bf.push_back(bftemp);
	ui.widget->setbf(bf);
	ui.widget->setdraw_mode_(BSVECTOR);
	ui.widget->pickedge_e.clear();
	update();
}

void HexMeshViewer::updatedraw2()
{
	Transformation t(mesh);
	t.rotate_y(1.5, atan(atans / 2));
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();

	mesh->smooth_mesh();
	mesh->smooth_mesh();
	mesh->smooth_mesh();
	HexV3fMesh *newmesh = new HexV3fMesh();
	ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
	AppCC.approximationingSharp();
	AppCC.constructBezier(1);
	AppCC.cal_nor_beisur(0.1);
	AppCC.cal_beisurv_center(1);
	mesh->checksingualrface();
	beziersurface bftemp;
	bftemp.biaojiface = AppCC.biaojiface;
	bftemp.bei_surv = AppCC.bei_surv;
	bftemp.bei_surv_dat = AppCC.bei_surv_dat;
	bftemp.tempdraw_num = mesh->faces_.size();
	bftemp.nor_bei_surv = AppCC.nor_bei_surv;
	bftemp.nor_bei_surv_dat = AppCC.nor_bei_surv_dat;
	bftemp.bei_surv_center = AppCC.bei_surv_center;
	bftemp.edgeline = AppCC.edgeline;
	bftemp.singularface = mesh->singularface;
	bf.pop_back();
	bf.push_back(bftemp);
	ui.widget->setbf(bf);
	ui.widget->setdraw_mode_(BSVECTOR);
	ui.widget->pickedge_e.clear();
	update();
}

void HexMeshViewer::updatedraw3()
{
	Transformation t(mesh);
	t.scale(scales, 1, 1);
	mesh->cal_cen();
	mesh->cal_cell_cen();
	mesh->examine_bdy();
	mesh->examine_bdy();
	mesh->examine_cs();
	mesh->cal_cen1();
	mesh->examine_bdfv();
	mesh->cal_cell_cen();
	mesh->cal_bdnorm();
	mesh->cal_mami_ed();
	mesh->cal_oriv();

	mesh->smooth_mesh();
	mesh->smooth_mesh();
	mesh->smooth_mesh();
	HexV3fMesh *newmesh = new HexV3fMesh();
	ApproximationCC AppCC(mesh, newmesh, ui.widget->pickedge_e);
	AppCC.approximationingSharp();
	AppCC.constructBezier(1);
	AppCC.cal_nor_beisur(0.1);
	AppCC.cal_beisurv_center(1);
	mesh->checksingualrface();
	beziersurface bftemp;
	bftemp.biaojiface = AppCC.biaojiface;
	bftemp.bei_surv = AppCC.bei_surv;
	bftemp.bei_surv_dat = AppCC.bei_surv_dat;
	bftemp.tempdraw_num = mesh->faces_.size();
	bftemp.nor_bei_surv = AppCC.nor_bei_surv;
	bftemp.nor_bei_surv_dat = AppCC.nor_bei_surv_dat;
	bftemp.bei_surv_center = AppCC.bei_surv_center;
	bftemp.edgeline = AppCC.edgeline;
	bftemp.singularface = mesh->singularface;
	bf.pop_back();
	bf.push_back(bftemp);
	ui.widget->setbf(bf);
	ui.widget->setdraw_mode_(BSVECTOR);
	ui.widget->pickedge_e.clear();
	update();
}

HexV3fMesh HexMeshViewer::constructBezierSolid2(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color,
	std::vector<std::vector<CellHandle>> &tempcell, std::vector<V3f> &tempcen)
{
	HexV3fMesh beziermesh;
	int solidnum = m_solids.Local_V3f.size();
	int size = 12; // size * size * size
	tempcell.resize(solidnum);
	tempcen.resize(solidnum);
	m_color.resize((size + 1) * (size + 1) * (size + 1) * solidnum);
	for (int solid_id = 0; solid_id < solidnum; solid_id++)
	{
		int m_uptnum = m_solids.Local_Node[solid_id][0].size() - m_solids.Local_Order[solid_id][0] - 1;
		int m_vptnum = m_solids.Local_Node[solid_id][1].size() - m_solids.Local_Order[solid_id][1] - 1;
		int m_wptnum = m_solids.Local_Node[solid_id][2].size() - m_solids.Local_Order[solid_id][2] - 1;
		std::vector<std::vector<V3f>> bezierv(size + 1);
		std::vector<std::vector<double>> colortemp(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1) * (size + 1); j++)
			{
				double u = j % (size + 1) * (1.0 / size);
				double v = j / (size + 1) * (1.0 / size);
				double w = i % (size + 1) * (1.0 / size);
				V3f tempvv = m_solids.Geometry_new(solid_id, u, v, w);

				/*if (solid_id == 0)
				{
					tempvv.x -= 0.5;
					tempvv.y -= 0.5;
				}
				else if (solid_id == 1)
				{
					tempvv.x += 0.5;
					tempvv.y += 0.5;
				}
				else if (solid_id == 2)
				{
					tempvv.x += 0.5;
					tempvv.y -= 0.5;
				}
				else if (solid_id == 3)
				{
					tempvv.x -= 0.5;
					tempvv.y += 0.5;
				}
				else if (solid_id == 4)
				{
					tempvv.z += 0.5;
				}
				else if (solid_id == 5)
				{
					tempvv.z -= 0.5;
				}*/

				//if (solid_id == 6)
				//tempvv.z = (tempvv.z + 2) / 4;
				bezierv[i].push_back(tempvv);
				double m_approxsol = 0;
				double m_uder[2], m_vder[2], m_wder[2];
				for (int ii = 0; ii < m_uptnum; ii++)
				{
					m_solids.DersOneBasisFun(solid_id, 0, ii, u, 1, m_uder);
					for (int jj = 0; jj < m_vptnum; jj++)
					{
						m_solids.DersOneBasisFun(solid_id, 1, jj, v, 1, m_vder);
						for (int kk = 0; kk < m_wptnum; kk++)
						{
							m_solids.DersOneBasisFun(solid_id, 2, kk, w, 1, m_wder);
							m_approxsol += m_bdycoef[solid_id][m_solids.Control_Point[solid_id][kk][jj][ii]] * m_uder[0] * m_vder[0] * m_wder[0];
						}
					}
				}
				colortemp[i].push_back(m_approxsol);
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size * size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				int temp = 0;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t]));
				m_color[ctemp[temp++]] = colortemp[j][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 1];
				tempcell[solid_id].push_back(beziermesh.add_cell(ctemp));
			}
		}
	}
	beziermesh.cal_cell_cen();
	for (int i = 0; i < tempcell.size(); ++i)
	{
		V3f v1;
		for (int j = 0; j < tempcell[i].size(); ++j)
			v1 = v1 + beziermesh.cell_center[tempcell[i][j]];
		v1 = v1 / tempcell[i].size();
		tempcen[i] = v1;
	}
	return beziermesh;
}

HexV3fMesh HexMeshViewer::constructBezierSolid3(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color,
	std::vector<std::vector<CellHandle>> &tempcell, std::vector<V3f> &tempcen)
{
	HexV3fMesh beziermesh;
	int solidnum = m_solids.Local_V3f.size();
	int size = 12; //
	tempcell.resize(solidnum);
	tempcen.resize(solidnum);
	m_color.resize((size + 1) * (size + 1) * (size + 1) * solidnum);
	for (int solid_id = 0; solid_id < solidnum; solid_id++)
	{
		int m_uptnum = m_solids.Local_Node[solid_id][0].size() - m_solids.Local_Order[solid_id][0] - 1;
		int m_vptnum = m_solids.Local_Node[solid_id][1].size() - m_solids.Local_Order[solid_id][1] - 1;
		int m_wptnum = m_solids.Local_Node[solid_id][2].size() - m_solids.Local_Order[solid_id][2] - 1;
		std::vector<std::vector<V3f>> bezierv(size + 1);
		std::vector<std::vector<double>> colortemp(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1) * (size + 1); j++)
			{
				double u = j % (size + 1) * (1.0 / size);
				double v = j / (size + 1) * (1.0 / size);
				double w = i % (size + 1) * (1.0 / size);
				V3f tempvv = m_solids.Geometry_new(solid_id, u, v, w);

				if (solid_id == 0)
				{
					tempvv.x -= 0.5;
					tempvv.y -= 0.5;
				}
				else if (solid_id == 1)
				{
					tempvv.x += 0.5;
					tempvv.y += 0.5;
				}
				else if (solid_id == 2)
				{
					tempvv.x += 0.5;
					tempvv.y -= 0.5;
				}
				else if (solid_id == 3)
				{
					tempvv.x -= 0.5;
					tempvv.y += 0.5;
				}
				else if (solid_id == 4)
				{
					tempvv.z += 0.5;
				}
				else if (solid_id == 5)
				{
					tempvv.z -= 0.5;
				}

				/*if (solid_id == 6)
					tempvv.z = (tempvv.z + 2) / 4;*/
				bezierv[i].push_back(tempvv);
				double m_approxsol = 0;
				double m_uder[2], m_vder[2], m_wder[2];
				for (int ii = 0; ii < m_uptnum; ii++)
				{
					m_solids.DersOneBasisFun(solid_id, 0, ii, u, 1, m_uder);
					for (int jj = 0; jj < m_vptnum; jj++)
					{
						m_solids.DersOneBasisFun(solid_id, 1, jj, v, 1, m_vder);
						for (int kk = 0; kk < m_wptnum; kk++)
						{
							m_solids.DersOneBasisFun(solid_id, 2, kk, w, 1, m_wder);
							m_approxsol += m_bdycoef[solid_id][m_solids.Control_Point[solid_id][kk][jj][ii]] * m_uder[0] * m_vder[0] * m_wder[0];
						}
					}
				}
				colortemp[i].push_back(m_approxsol);
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size * size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				int temp = 0;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t]));
				m_color[ctemp[temp++]] = colortemp[j][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 1];
				tempcell[solid_id].push_back(beziermesh.add_cell(ctemp));
			}
		}
	}
	beziermesh.cal_cell_cen();
	for (int i = 0; i < tempcell.size(); ++i)
	{
		V3f v1;
		for (int j = 0; j < tempcell[i].size(); ++j)
			v1 = v1 + beziermesh.cell_center[tempcell[i][j]];
		v1 = v1 / tempcell[i].size();
		tempcen[i] = v1;
	}
	return beziermesh;
}

HexV3fMesh HexMeshViewer::constructBezierSolid4(std::vector<std::vector<double>> m_bdycoef, std::vector<double> &m_color,
	std::vector<std::vector<CellHandle>> &tempcell, std::vector<V3f> &tempcen)
{
	HexV3fMesh beziermesh;
	int solidnum = m_solids.Local_V3f.size();
	int size = 12; //
	tempcell.resize(solidnum);
	tempcen.resize(solidnum);
	m_color.resize((size + 1) * (size + 1) * (size + 1) * solidnum);
	for (int solid_id = 0; solid_id < solidnum; solid_id++)
	{
		//if (solid_id == 0 || solid_id == 2 || solid_id == 4)
		//continue;
		int m_uptnum = m_solids.Local_Node[solid_id][0].size() - m_solids.Local_Order[solid_id][0] - 1;
		int m_vptnum = m_solids.Local_Node[solid_id][1].size() - m_solids.Local_Order[solid_id][1] - 1;
		int m_wptnum = m_solids.Local_Node[solid_id][2].size() - m_solids.Local_Order[solid_id][2] - 1;
		std::vector<std::vector<V3f>> bezierv(size + 1);
		std::vector<std::vector<double>> colortemp(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1) * (size + 1); j++)
			{
				double u = j % (size + 1) * (1.0 / size);
				double v = j / (size + 1) * (1.0 / size);
				double w = i % (size + 1) * (1.0 / size);
				V3f tempvv = m_solids.Geometry_new(solid_id, u, v, w);

				//if (solid_id == 6)
				//tempvv.z = (tempvv.z + 2) / 4;
				bezierv[i].push_back(tempvv);
				double m_approxsol = 0;
				double m_uder[2], m_vder[2], m_wder[2];
				for (int ii = 0; ii < m_uptnum; ii++)
				{
					m_solids.DersOneBasisFun(solid_id, 0, ii, u, 1, m_uder);
					for (int jj = 0; jj < m_vptnum; jj++)
					{
						m_solids.DersOneBasisFun(solid_id, 1, jj, v, 1, m_vder);
						for (int kk = 0; kk < m_wptnum; kk++)
						{
							m_solids.DersOneBasisFun(solid_id, 2, kk, w, 1, m_wder);
							m_approxsol += m_bdycoef[solid_id][m_solids.Control_Point[solid_id][kk][jj][ii]] * m_uder[0] * m_vder[0] * m_wder[0];
						}
					}
				}
				colortemp[i].push_back(m_approxsol);
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size * size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				int temp = 0;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t]));
				m_color[ctemp[temp++]] = colortemp[j][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j][t + size + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + 1];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 2];
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				m_color[ctemp[temp++]] = colortemp[j + 1][t + size + 1];
				tempcell[solid_id].push_back(beziermesh.add_cell(ctemp));
			}
		}
	}
	beziermesh.cal_cell_cen();
	for (int i = 0; i < tempcell.size(); ++i)
	{
		V3f v1;
		for (int j = 0; j < tempcell[i].size(); ++j)
			v1 = v1 + beziermesh.cell_center[tempcell[i][j]];
		v1 = v1 / tempcell[i].size();
		tempcen[i] = v1;
	}
	return beziermesh;
}



