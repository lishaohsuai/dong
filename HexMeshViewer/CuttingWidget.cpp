#include "CuttingWidget.h"

CuttingWidget::CuttingWidget(QWidget *parent) : QWidget(parent) {
	initDialog();
}

void CuttingWidget::initDialog() {
	createWidget();
	createLayout();
}

void CuttingWidget::createWidget() {
	createSVMWidget();
}

void CuttingWidget::createLayout() {
	tabWidget = new QTabWidget();

	tabWidget->addTab(viewVM, "SVM");
	tabWidget->addTab(viewBMesh, "BMesh");
	tabWidget->addTab(viewReCon, "ReCon");
	//tabWidget->addTab();
	tabWidget->setStyleSheet("QTabBar::tab { width: 150px; }");
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(tabWidget);

	setLayout(layout);
}

void CuttingWidget::createSVMWidget() {
	//draw_main_scene = new QCheckBox("Draw Main Scene");
	//draw_main_scene->setCheckable(true); draw_main_scene->setChecked(true);
	//construct the show Volume mesh dialog
	//leftRightShow = new QCheckBox("Show Right Part");
	//leftRightShow->setCheckState(Qt::Checked);
	//upDownShow = new QCheckBox("Show Down Part");
	//upDownShow->setCheckState(Qt::Checked);
	//backFrontShow = new QCheckBox("Show Back Part");
	//backFrontShow->setCheckState(Qt::Checked);

	xSlider = new QLabel("X : ");
	XSlider = new QSlider(Qt::Horizontal);
	XSlider->setMinimum(0);
	XSlider->setMaximum(100);
	XSlider->setValue(0);
	xSlider->setBuddy(XSlider);

	ySlider = new QLabel("Y : ");
	YSlider = new QSlider(Qt::Horizontal);
	YSlider->setMinimum(0);
	YSlider->setMaximum(100);
	YSlider->setValue(0);
	ySlider->setBuddy(YSlider);

	zSlider = new QLabel("Z : ");
	ZSlider = new QSlider(Qt::Horizontal);
	ZSlider->setMinimum(0);
	ZSlider->setMaximum(100);
	ZSlider->setValue(0);
	zSlider->setBuddy(ZSlider);

	//draw_inv_tet = new QCheckBox("Draw Flipped Tet");
	//draw_inv_tet->setCheckable(true); draw_inv_tet->setChecked(false);

	//save_show_mesh = new QPushButton("Save Show Mesh");
	//save_mesh_graph = new QPushButton("Save Mesh Graph");
	//save_hex_mesh_graph = new QPushButton("Save Hex Mesh Graph");
	construct_vertex_texture = new QPushButton("Generate Vertex Texture");

	int svm_index = 0;
	QGridLayout* layout1 = new QGridLayout();
	//layout1->addWidget(draw_main_scene, svm_index, 0, 1, 2); ++svm_index;
	//layout1->addWidget(leftRightShow, svm_index, 0, 1, 2); ++svm_index;
	//layout1->addWidget(upDownShow, svm_index, 0, 1, 2); ++svm_index;
	//layout1->addWidget(backFrontShow, svm_index, 0, 1, 2); ++svm_index;
	layout1->addWidget(xSlider, svm_index, 0, 1, 1);
	layout1->addWidget(XSlider, svm_index, 1, 1, 1); ++svm_index;
	layout1->addWidget(ySlider, svm_index, 0, 1, 1);
	layout1->addWidget(YSlider, svm_index, 1, 1, 1); ++svm_index;
	layout1->addWidget(zSlider, svm_index, 0, 1, 1);
	layout1->addWidget(ZSlider, svm_index, 1, 1, 1); ++svm_index;
	//layout1->addWidget(draw_inv_tet, svm_index, 0, 1, 2); ++svm_index;
	//layout1->addWidget(save_show_mesh, svm_index, 0, 1, 2); ++svm_index;
	//layout1->addWidget(save_mesh_graph, svm_index, 0, 1, 2); ++svm_index;
	//layout1->addWidget(save_hex_mesh_graph, svm_index, 0, 1, 2); ++svm_index;
	layout1->addWidget(construct_vertex_texture, svm_index, 0, 1, 2); ++svm_index;

	basic_draw_Box = new QGroupBox("Basic Drawing");
	basic_draw_Box->setLayout(layout1);

	int bmesh_index = 0;
	basic_BMesh_Box = new QGroupBox("BMesh");
	QGridLayout* layoutBMesh = new QGridLayout();

	int topo_index = 0;
	basic_Topo_Box = new QGroupBox("Topo");
	QGridLayout* layoutTopo = new QGridLayout();

	xPosition = new QLabel("X pos:");
	yPosition = new QLabel("Y pos:");
	zPosition = new QLabel("Z pos:");

	length = new QLabel("length:");
	width = new QLabel("width");
	height = new QLabel("height");

	xAngle = new QLabel("angleX:");
	yAngle = new QLabel("angleY:");
	zAngle = new QLabel("angleZ:");

	addCube = new QPushButton("Add_Edit");
	deleteCube = new QPushButton("Delete");
	editCube = new QPushButton("Edit");
	selectCube = new QPushButton("Select");
	selectMutiNodes = new QPushButton("SelectMuti");
	addCubeWithMouse = new QPushButton("add_Mouse");
	boom = new QPushButton("GenCoarseMesh");
	boom1 = new QPushButton("ChangeAngleAuto");
	boom2 = new QPushButton("genLinkBody");
	boom3 = new QPushButton("topoSplit");
	boom4 = new QPushButton("Singu gen");
	spiltX = new QPushButton("spiltX");
	spiltY = new QPushButton("spiltY");
	spiltZ = new QPushButton("spiltZ");
	xExpand = new QPushButton("xExpand");
	yExpand = new QPushButton("yExpand");
	zExpand = new QPushButton("zExpand");
	xCrop = new QPushButton("xCrop");
	yCrop = new QPushButton("yCrop");
	zCrop = new QPushButton("zCrop");
	refine = new QPushButton("refine");
	connectNode = new QPushButton("connectNode");
	disconnectNode = new QPushButton("disconnectNode");
	setStep = new QPushButton("SetStep");
	selectALine = new QPushButton("selectALine");
	xyView = new QPushButton("xyView");
	xyView_ = new QPushButton("xyView_");
	yzView = new QPushButton("yzView");
	yzView_ = new QPushButton("yzView_");
	xzView = new QPushButton("xzView");
	xzView_ = new QPushButton("xzView_");

	XPosition = new QLineEdit();
	YPosition = new QLineEdit();
	ZPosition = new QLineEdit();
	Length = new QLineEdit();
	Width = new QLineEdit();
	Height = new QLineEdit();
	XAngle = new QLineEdit();
	YAngle = new QLineEdit();
	ZAngle = new QLineEdit();
	step = new QLineEdit();
	axisShow = new QCheckBox("showAxis"); 
	axisShow->setCheckable(true); axisShow->setChecked(false);
	axisShow->setCheckState(Qt::Unchecked);
	groundShow = new QCheckBox("showGround");
	groundShow->setCheckable(true); groundShow->setChecked(false);
	groundShow->setCheckState(Qt::Unchecked);
	skelShow = new QCheckBox("showSkel");
	skelShow->setCheckable(true); skelShow->setChecked(false);
	skelShow->setCheckState(Qt::Unchecked);
	mirrorShow = new QCheckBox("showMirror");
	mirrorShow->setCheckable(true); mirrorShow->setChecked(false);
	mirrorShow->setCheckState(Qt::Unchecked);
	mirrorMode = new QCheckBox("mirrorMode");
	mirrorMode->setCheckable(true); mirrorMode->setChecked(false);
	mirrorMode->setCheckState(Qt::Unchecked);
	dragMode = new QCheckBox("dragMode");
	dragMode->setCheckable(true); dragMode->setChecked(false);
	dragMode->setCheckState(Qt::Unchecked);

	showTriMesh = new QCheckBox("showTriMesh");
	showTriMesh->setCheckable(true); showTriMesh->setChecked(false);
	showTriMesh->setCheckState(Qt::Unchecked);

	showProjectLines = new QCheckBox("showProLines");
	showProjectLines->setCheckable(true); showProjectLines->setChecked(false);
	showProjectLines->setCheckState(Qt::Unchecked);

	drawSkelMode = new QRadioButton("skel");
	drawSelectSkelMode = new QRadioButton("selectSkel");
	drawSelectSkelsMode = new QRadioButton("selectSkels");
	vertexSnapMode = new QCheckBox("vertexSnap");
	vertexSnapMode->setCheckable(true); vertexSnapMode->setChecked(false);
	vertexSnapMode->setCheckState(Qt::Unchecked);
	drawMode = new QButtonGroup(this);
	drawMode->addButton(drawSkelMode, 0);
	drawMode->addButton(drawSelectSkelMode, 1);
	drawMode->addButton(drawSelectSkelsMode, 2);
	drawSkelMode->setChecked(true);
	roateByMouse = new QCheckBox("rotateByMouse");
	roateByMouse->setCheckable(true); roateByMouse->setChecked(false);
	roateByMouse->setCheckState(Qt::Unchecked);


	layoutBMesh->addWidget(xPosition, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(yPosition, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(zPosition, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(XPosition, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(YPosition, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(ZPosition, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(xAngle, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(yAngle, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(zAngle, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(XAngle, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(YAngle, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(ZAngle, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(length, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(width, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(height, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(Length, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(Width, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(Height, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(setStep, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(step, bmesh_index, 1, 1, 1); bmesh_index++;

	layoutBMesh->addWidget(addCube, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(addCubeWithMouse, bmesh_index, 1, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(connectNode, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(disconnectNode, bmesh_index, 1, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(deleteCube, bmesh_index, 0, 1, 2); bmesh_index++;
	layoutBMesh->addWidget(editCube, bmesh_index, 0, 1, 2); bmesh_index++;
	layoutBMesh->addWidget(selectCube, bmesh_index, 0, 1, 1); 
	layoutBMesh->addWidget(selectMutiNodes, bmesh_index, 1, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(boom, bmesh_index, 0, 1, 2); bmesh_index++;
	layoutBMesh->addWidget(boom1, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(boom2, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(boom3, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(spiltX, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(spiltY, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(spiltZ, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(xExpand, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(yExpand, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(zExpand, bmesh_index, 2, 1, 1); bmesh_index++;
	
	layoutBMesh->addWidget(xCrop, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(yCrop, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(zCrop, bmesh_index, 2, 1, 1); bmesh_index++;

	layoutBMesh->addWidget(refine, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(roateByMouse, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(vertexSnapMode, bmesh_index, 2, 1, 1);bmesh_index++;
	layoutBMesh->addWidget(selectALine, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(boom4, bmesh_index, 1, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(xyView, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(yzView, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(xzView, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(xyView_, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(yzView_, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(xzView_, bmesh_index, 2, 1, 1); bmesh_index++;
	layoutBMesh->addWidget(axisShow, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(groundShow, bmesh_index, 1, 1, 1); 
	layoutBMesh->addWidget(skelShow, bmesh_index, 2, 1, 1); bmesh_index++; 
	layoutBMesh->addWidget(mirrorShow, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(mirrorMode, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(dragMode, bmesh_index, 2, 1, 1); bmesh_index++; 
	layoutBMesh->addWidget(showTriMesh, bmesh_index, 0, 1, 1); 
	layoutBMesh->addWidget(showProjectLines, bmesh_index, 1, 1, 1); bmesh_index++;

	layoutBMesh->addWidget(drawSkelMode, bmesh_index, 0, 1, 1);
	layoutBMesh->addWidget(drawSelectSkelMode, bmesh_index, 1, 1, 1);
	layoutBMesh->addWidget(drawSelectSkelsMode, bmesh_index, 2, 1, 1);  bmesh_index++;
	basic_BMesh_Box->setLayout(layoutBMesh);


	readTestLine = new QPushButton("readTestLineskel");
	actSimpleWithCgal = new QPushButton("SimpleWithCgal");
	actFittingCurve = new QPushButton("changeToPolylines");
	actFittingCurves = new QPushButton("actFittingCurves");
	actRMF = new QPushButton("rmf");
	pipeReCon = new QPushButton("pipeReCon");
	layoutTopo->addWidget(actSimpleWithCgal, topo_index, 0, 1, 1);
	layoutTopo->addWidget(readTestLine, topo_index, 1, 1, 1);
	layoutTopo->addWidget(actFittingCurve, topo_index, 2, 1, 1); topo_index++;
	layoutTopo->addWidget(actFittingCurves, topo_index, 0, 1, 1);
	layoutTopo->addWidget(actRMF, topo_index, 1, 1, 1);
	layoutTopo->addWidget(pipeReCon, topo_index, 2, 1, 1);
	basic_Topo_Box->setLayout(layoutTopo);

	/*choose_point_surface = new QRadioButton("choose point on surface");
	choose_point_surface->setChecked(true);
	choose_point_other = new QRadioButton("choose point on other");
	choose_point_other->setChecked(false);
	choose_point_group = new QButtonGroup();
	choose_point_group->addButton(choose_point_surface, MeshField::POINT_ON_SURFACE);
	choose_point_group->addButton(choose_point_other, MeshField::POINT_ON_OTHER);
	extend_start = new QLabel("Extend Start");
	start_Slider = new QSlider(Qt::Horizontal);
	start_Slider->setMinimum(0);
	start_Slider->setMaximum(100);
	start_Slider->setValue(0);
	extend_start->setBuddy(start_Slider);

	extend_end = new QLabel("Extend End");
	end_Slider = new QSlider(Qt::Horizontal);
	end_Slider->setMinimum(0);
	end_Slider->setMaximum(100);
	end_Slider->setValue(100);
	extend_end->setBuddy(end_Slider);

	angle_phi = new QLabel("phi : ");
	phi_Slider = new QSlider(Qt::Horizontal);
	phi_Slider->setMinimum(0);
	phi_Slider->setMaximum(100);
	phi_Slider->setValue(0.0);
	angle_phi->setBuddy(phi_Slider);

	angle_alpha = new QLabel("alpha : ");
	alpha_Slider = new QSlider(Qt::Horizontal);
	alpha_Slider->setMinimum(0.0);
	alpha_Slider->setMaximum(100.0);
	alpha_Slider->setValue(0.0);
	angle_alpha->setBuddy(alpha_Slider);

	find_exist_end = new QPushButton("find exist end");
	draw_active_lines = new QCheckBox("draw active lines");
	draw_active_lines->setCheckable(true); draw_active_lines->setChecked(false);

	int i_l_index = 0;
	QGridLayout* insert_line_layout = new QGridLayout();
	insert_line_layout->addWidget(choose_point_surface, i_l_index, 0, 1,2); i_l_index += 1;
	insert_line_layout->addWidget(choose_point_other, i_l_index, 0, 1,2); i_l_index += 1;
	insert_line_layout->addWidget(extend_start, i_l_index, 0, 1,1);
	insert_line_layout->addWidget(start_Slider, i_l_index, 1, 1,1);i_l_index += 1;
	insert_line_layout->addWidget(extend_end, i_l_index, 0, 1,1);
	insert_line_layout->addWidget(end_Slider, i_l_index, 1, 1,1); i_l_index += 1;
	insert_line_layout->addWidget(angle_phi, i_l_index, 0, 1,1);
	insert_line_layout->addWidget(phi_Slider, i_l_index, 1, 1,1); i_l_index += 1;
	insert_line_layout->addWidget(angle_alpha, i_l_index, 0, 1,1);
	insert_line_layout->addWidget(alpha_Slider, i_l_index, 1, 1,1); i_l_index += 1;
	insert_line_layout->addWidget(find_exist_end, i_l_index, 0, 1, 2); i_l_index += 1;
	insert_line_layout->addWidget(draw_active_lines, i_l_index, 0, 1, 2); i_l_index += 1;

	insert_lines = new QGroupBox();
	insert_lines->setLayout(insert_line_layout);*/

	int main_index = 0;
	QGridLayout* main_layout = new QGridLayout();
	main_layout->addWidget(basic_draw_Box, main_index, 0, svm_index, 1); main_index += svm_index;
	//main_layout->addWidget(insert_lines,main_index, 0, i_l_index, 1); main_index += svm_index;
	//main_layout->addWidget(leftLabel, main_index, 0, 15, 1);
	int BMesh_index = 0;
	QGridLayout* BMesh_layout = new QGridLayout();
	BMesh_layout->addWidget(basic_BMesh_Box, BMesh_index, 0, bmesh_index, 1); BMesh_index += bmesh_index;

	int Topo_index = 0;
	QGridLayout* Topo_layout = new QGridLayout();
	Topo_layout->addWidget(basic_Topo_Box, Topo_index, 0, topo_index, 1); Topo_index += topo_index;

	showVolumeMesh = new QWidget();
	showVolumeMesh->setLayout(main_layout);
	viewVM = new QScrollArea;
	viewVM->setFocusPolicy(Qt::NoFocus);
	viewVM->setFrameStyle(QFrame::NoFrame);
	viewVM->setWidget(showVolumeMesh);
	viewVM->setWidgetResizable(true);

	showBMesh = new QWidget();
	showBMesh->setLayout(BMesh_layout);
	viewBMesh = new QScrollArea;
	viewBMesh->setFocusPolicy(Qt::NoFocus);
	viewBMesh->setFrameStyle(QFrame::NoFrame);
	viewBMesh->setWidget(showBMesh);
	viewBMesh->setWidgetResizable(true);

	showReCon = new QWidget();
	showReCon->setLayout(Topo_layout);
	viewReCon = new QScrollArea;
	viewReCon->setFocusPolicy(Qt::NoFocus);
	viewReCon->setFrameStyle(QFrame::NoFrame);
	viewReCon->setWidget(showReCon);
	viewReCon->setWidgetResizable(true);

	//some slots and signals
	// connect(draw_main_scene, SIGNAL(stateChanged(int)), SIGNAL(draw_main_scene_signal(int)));
    //connect(leftRightShow, SIGNAL(stateChanged(int)), SIGNAL(leftrightChanged(int)));
	//connect(upDownShow, SIGNAL(stateChanged(int)), SIGNAL(updownChanged(int)));
	//connect(backFrontShow, SIGNAL(stateChanged(int)), SIGNAL(backfrontChanged(int)));

	connect(XSlider, SIGNAL(valueChanged(int)), SIGNAL(xslidderChanged(int)));
	connect(YSlider, SIGNAL(valueChanged(int)), SIGNAL(yslidderChanged(int)));
	connect(ZSlider, SIGNAL(valueChanged(int)), SIGNAL(zslidderChanged(int)));

	//connect(draw_inv_tet, SIGNAL(stateChanged(int)), SIGNAL(set_draw_inv_tet_signal(int)));
	//connect(save_show_mesh, SIGNAL(clicked()), SIGNAL(save_show_mesh_signal()));
	//connect(save_mesh_graph, SIGNAL(clicked()), SIGNAL(save_mesh_graph_signal()));
	//connect(save_hex_mesh_graph, SIGNAL(clicked()), SIGNAL(save_mesh_graph_hex_signal()));
	connect(construct_vertex_texture, SIGNAL(clicked()), SIGNAL(construct_vertex_texture_signal()));

	connect(addCube, SIGNAL(clicked()), this, SLOT(addCubeSignal()));
	connect(deleteCube, SIGNAL(clicked()), this, SLOT(deleteCubeSignal()));
	connect(connectNode, SIGNAL(clicked()), this, SLOT(connectNodeSignal()));
	connect(disconnectNode, SIGNAL(clicked()), this, SLOT(disconnectNodeSignal()));
	connect(selectMutiNodes, SIGNAL(clicked()), this, SLOT(selectMutiNodesSignal()));
	connect(disconnectNode, SIGNAL(clicked()), this, SLOT(disconnectNodeSignal()));
	connect(editCube, SIGNAL(clicked()), this, SLOT(editCubeSignal()));
	connect(selectCube, SIGNAL(clicked()), this, SLOT(selectCubeSignal()));
	connect(addCubeWithMouse, SIGNAL(clicked()), this, SLOT(addCubeWithMouseSignal()));
	connect(setStep, SIGNAL(clicked()), this, SLOT(setStepSignal()));

	connect(SingalFather::getInstance(), SIGNAL(getData(double, double, double, double, double, double, double, double, double)), 
		this, SLOT(reNewDataSignal(double, double, double, double, double, double, double, double, double)));
	connect(boom, SIGNAL(clicked()), this, SLOT(genMesh()));
	connect(boom1, SIGNAL(clicked()), this, SLOT(changeAngleAutoSignal()));
	connect(boom2, SIGNAL(clicked()), this, SLOT(generateLinkBodySignal()));
	connect(boom3, SIGNAL(clicked()), this, SLOT(topoSplitSignal()));
	connect(refine, SIGNAL(clicked()), this, SLOT(refineSignal()));
	connect(selectALine, SIGNAL(clicked()), this, SLOT(selectALineSignal()));

	connect(xyView, SIGNAL(clicked()), this, SLOT(xyViewSignal()));
	connect(xyView_, SIGNAL(clicked()), this, SLOT(xyView_Signal()));
	connect(yzView, SIGNAL(clicked()), this, SLOT(yzViewSignal()));
	connect(yzView_, SIGNAL(clicked()), this, SLOT(yzView_Signal()));
	connect(xzView, SIGNAL(clicked()), this, SLOT(xzViewSignal()));
	connect(xzView_, SIGNAL(clicked()), this, SLOT(xzView_Signal()));

	connect(axisShow, SIGNAL(stateChanged(int)), this, SLOT(axisShowChanged(int)));
	connect(groundShow, SIGNAL(stateChanged(int)), this, SLOT(groundShowChanged(int)));
	connect(skelShow, SIGNAL(stateChanged(int)), this, SLOT(skelShowChanged(int)));
	connect(mirrorShow, SIGNAL(stateChanged(int)), this, SLOT(mirrorShowChanged(int)));
	connect(mirrorMode, SIGNAL(stateChanged(int)), this, SLOT(mirrorModeChanged(int)));
	connect(dragMode, SIGNAL(stateChanged(int)), this, SLOT(dragModeChanged(int)));
	
	connect(showTriMesh, SIGNAL(stateChanged(int)), this, SLOT(showTriMeshChanged(int)));
	connect(showProjectLines, SIGNAL(stateChanged(int)), this, SLOT(showProjectLinesChanged(int)));


	connect(drawSkelMode, SIGNAL(clicked()), this, SLOT(drawModeChange()));
	connect(drawSelectSkelMode, SIGNAL(clicked()), this, SLOT(drawModeChange()));
	connect(drawSelectSkelsMode, SIGNAL(clicked()), this, SLOT(drawModeChange()));

	connect(spiltX, SIGNAL(clicked()), this, SLOT(spiltXSignal()));
	connect(spiltY, SIGNAL(clicked()), this, SLOT(spiltYSignal()));
	connect(spiltZ, SIGNAL(clicked()), this, SLOT(spiltZSignal()));

	connect(xExpand, SIGNAL(clicked()), this, SLOT(xExpandSignal()));
	connect(yExpand, SIGNAL(clicked()), this, SLOT(yExpandSignal()));
	connect(zExpand, SIGNAL(clicked()), this, SLOT(zExpandSignal()));

	connect(xCrop, SIGNAL(clicked()), this, SLOT(xCropSignal()));
	connect(yCrop, SIGNAL(clicked()), this, SLOT(yCropSignal()));
	connect(zCrop, SIGNAL(clicked()), this, SLOT(zCropSignal()));

	connect(roateByMouse, SIGNAL(stateChanged(int)), this, SLOT(roateByMouseChange(int)));
	connect(vertexSnapMode, SIGNAL(stateChanged(int)), this, SLOT(vertexSnapModeChange(int)));
	connect(boom4, SIGNAL(clicked()), this, SLOT(boom4Signal()));

	connect(actSimpleWithCgal, SIGNAL(clicked()), this, SLOT(simpleWithCgalSignal()));
	connect(readTestLine, SIGNAL(clicked()), this, SLOT(readTestLineSignal()));
	connect(actFittingCurve, SIGNAL(clicked()), this, SLOT(fittingCurveSignal()));
	connect(actFittingCurves, SIGNAL(clicked()), this, SLOT(fittingCurvesSignal()));
	connect(actRMF, SIGNAL(clicked()), this, SLOT(actRMFSignal()));
	connect(pipeReCon, SIGNAL(clicked()), this, SLOT(pipeline2()));
}

void CuttingWidget::pipeline2() {
	SingalFather::getInstance()->spipeline2();
}

void CuttingWidget::actRMFSignal() {
	SingalFather::getInstance()->sactRMFSignal();
}

void CuttingWidget::fittingCurvesSignal() {
	SingalFather::getInstance()->sfittingCurvesSignal();
}


void CuttingWidget::fittingCurveSignal() {
	// 输出分段文件
	SingalFather::getInstance()->schangeToPolylines();
}

void CuttingWidget::simpleWithCgalSignal() {
	SingalFather::getInstance()->ssimpleWithCgalSignal();
}

void CuttingWidget::readTestLineSignal() {
	SingalFather::getInstance()->sreadTestLineSignal();
}


void CuttingWidget::boom4Signal(){
	SingalFather::getInstance()->sBoom4();
}

void CuttingWidget::vertexSnapModeChange(int arg) {
	SingalFather::getInstance()->sVertexSnapMode(arg);
}

void CuttingWidget::roateByMouseChange(int arg) {
	SingalFather::getInstance()->sRotateByMouse(arg);
}

void CuttingWidget::xCropSignal() {
	SingalFather::getInstance()->sxCrop();
}
void CuttingWidget::yCropSignal() {
	SingalFather::getInstance()->syCrop();
}
void CuttingWidget::zCropSignal() {
	SingalFather::getInstance()->szCrop();
}


void CuttingWidget::xExpandSignal() {
	SingalFather::getInstance()->sxExpand();
}
void CuttingWidget::yExpandSignal() {
	SingalFather::getInstance()->syExpand();
}
void CuttingWidget::zExpandSignal() {
	SingalFather::getInstance()->szExpand();
}

void CuttingWidget::spiltXSignal() {
	SingalFather::getInstance()->sspiltX();
}

void CuttingWidget::spiltYSignal() {
	SingalFather::getInstance()->sspiltY();
}

void CuttingWidget::spiltZSignal() {
	SingalFather::getInstance()->sspiltZ();
}

// void CuttingWidget::sp

void CuttingWidget::drawModeChange() {
    switch(drawMode->checkedId()) {
       	case 0:
	   	   	SingalFather::getInstance()->sdrawModeChanged(0);
           break;
       	case 1:
           SingalFather::getInstance()->sdrawModeChanged(1);
           break;
       	case 2:
           SingalFather::getInstance()->sdrawModeChanged(2);
           break;
		default:
			break;
    }
}

void CuttingWidget::axisShowChanged(int arg) {
	SingalFather::getInstance()->saxisShowChanged(arg);
}

void CuttingWidget::groundShowChanged(int arg) {
	SingalFather::getInstance()->sgroundShowChanged(arg);
}

void CuttingWidget::skelShowChanged(int arg) {
	SingalFather::getInstance()->sskelShowChanged(arg);
}

void CuttingWidget::mirrorShowChanged(int arg) {
	SingalFather::getInstance()->smirrorShowChanged(arg);
}

void CuttingWidget::mirrorModeChanged(int arg) {
	SingalFather::getInstance()->smirrorModeChanged(arg);
}

void CuttingWidget::dragModeChanged(int arg) {
	SingalFather::getInstance()->sdragModeChanged(arg);
}

void CuttingWidget::showTriMeshChanged(int arg) {
	SingalFather::getInstance()->sshowTriMeshChanged(arg);
}

void CuttingWidget::showProjectLinesChanged(int arg) {
	SingalFather::getInstance()->sshowProjectLinesChanged(arg);
}

void CuttingWidget::setStepSignal(){
	double stepDou = step->text().toDouble();
	SingalFather::getInstance()->ssetStepDouble(stepDou);
}

void CuttingWidget::addCubeSignal() {
	double x = XPosition->text().toDouble();
	double y = YPosition->text().toDouble();
	double z = ZPosition->text().toDouble();
	double angleX = XAngle->text().toDouble();
	double angleY = YAngle->text().toDouble();
	double angleZ = ZAngle->text().toDouble();

	double length = Length->text().toDouble();
	double width = Width->text().toDouble();
	double height = Height->text().toDouble();
	SingalFather::getInstance()->saddNode(x, y, z, angleX, angleY, angleZ, length, width, height);
}

void CuttingWidget::changeAngleAutoSignal() {
	SingalFather::getInstance()->schangeAngleAutoSignal();
}

void CuttingWidget::generateLinkBodySignal() {
	SingalFather::getInstance()->sgenerateLinkBodySignal();
}

void CuttingWidget::deleteCubeSignal() {
	SingalFather::getInstance()->sdelCell();
}

void CuttingWidget::topoSplitSignal() {
	SingalFather::getInstance()->stopoSplitSignal();
}

void CuttingWidget::refineSignal() {
	SingalFather::getInstance()->srefineSignal();
}

void CuttingWidget::editCubeSignal() {
	double x = XPosition->text().toDouble();
	double y = YPosition->text().toDouble();
	double z = ZPosition->text().toDouble();
	double angleX = XAngle->text().toDouble();
	double angleY = YAngle->text().toDouble();
	double angleZ = ZAngle->text().toDouble();
	double length = Length->text().toDouble();
	double width = Width->text().toDouble();
	double height = Height->text().toDouble();
	SingalFather::getInstance()->seditCell(x, y, z, angleX, angleY, angleZ, length, width, height);
}

void CuttingWidget::selectALineSignal() {
	SingalFather::getInstance()->sselectALine();
}

void CuttingWidget::selectCubeSignal() {
	SingalFather::getInstance()->sselectCube();
}

void CuttingWidget::genMesh() {
	SingalFather::getInstance()->genCoarseMesh();
}

void CuttingWidget::reNewDataSignal(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height) {
	XPosition->setText(QString::number(x));
	YPosition->setText(QString::number(y));
	ZPosition->setText(QString::number(z));
	XAngle->setText(QString::number(angleX));
	YAngle->setText(QString::number(angleY));
	ZAngle->setText(QString::number(angleZ));
	Length->setText(QString::number(length));
	Width->setText(QString::number(width));
	Height->setText(QString::number(height));
}

void CuttingWidget::addCubeWithMouseSignal() {
	SingalFather::getInstance()->addCubeUseMouse();
}

void CuttingWidget::connectNodeSignal() {
	SingalFather::getInstance()->sconnectNodes();
}

void CuttingWidget::disconnectNodeSignal() {
	SingalFather::getInstance()->sdisconnectNodes();
}

void CuttingWidget::selectMutiNodesSignal() {
	SingalFather::getInstance()->sselectMutiNodes();
}

void CuttingWidget::xyViewSignal(){
	SingalFather::getInstance()->ssetxyView();
}

void CuttingWidget::yzViewSignal() {
	SingalFather::getInstance()->ssetyzView();
}

void CuttingWidget::xzViewSignal() {
	SingalFather::getInstance()->ssetxzView();
}

void CuttingWidget::xyView_Signal(){
	SingalFather::getInstance()->ssetxy_View();
}

void CuttingWidget::yzView_Signal() {
	SingalFather::getInstance()->ssetyz_View();
}

void CuttingWidget::xzView_Signal() {
	SingalFather::getInstance()->ssetxz_View();
}

//void CuttingWidget::xslidderChanged(int _t1)
//{
//	void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
//	QMetaObject::activate(this, &staticMetaObject, 4, _a);
//}
//
//void CuttingWidget::yslidderChanged(int _t1)
//{
//	void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
//	QMetaObject::activate(this, &staticMetaObject, 5, _a);
//}
//
//void CuttingWidget::zslidderChanged(int _t1)
//{
//	void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
//	QMetaObject::activate(this, &staticMetaObject, 6, _a);
//}