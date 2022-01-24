#pragma once
#include <QWidget>
#include <QApplication>
#include <QtWidgets>
#include <QButtonGroup>
#include <QRadioButton>
#include "singalFather.h"

class CuttingWidget :public QWidget
{
	Q_OBJECT
public:
	explicit CuttingWidget(QWidget *parent = 0);
	~CuttingWidget() {};
private:
	QTabWidget* tabWidget;

	//QCheckBox* draw_main_scene;
	//Show Volume Meshm
	//QLabel* leftRight; QCheckBox* leftRightShow;
	//QLabel* upDown; QCheckBox* upDownShow;
	//QLabel* backFront; QCheckBox* backFrontShow;

	QLabel* xSlider; QSlider* XSlider;
	QLabel* ySlider; QSlider* YSlider;
	QLabel* zSlider; QSlider* ZSlider;
	QLabel* leftLabel;

	QCheckBox* draw_inv_tet;
	QGroupBox* basic_draw_Box;
	QPushButton* save_show_mesh;
	QPushButton* save_mesh_graph;
	QPushButton* save_hex_mesh_graph;
	QPushButton* construct_vertex_texture;

	QWidget* showVolumeMesh;
	QWidget* showBMesh;
	QWidget* showReCon;
	QScrollArea *viewVM;
	QScrollArea* viewBMesh;
	QScrollArea* viewReCon; // tuo pu reconstruction

	// lee
	QGroupBox* basic_BMesh_Box;
	QLabel* xPosition; QLineEdit* XPosition;
	QLabel* xAngle; QLineEdit* XAngle;
	QLabel* yPosition; QLineEdit* YPosition;
	QLabel* yAngle; QLineEdit* YAngle;
	QLabel* zPosition; QLineEdit* ZPosition;
	QLabel* zAngle; QLineEdit* ZAngle;
	QLabel* length; QLineEdit* Length;
	QLabel* width; QLineEdit* Width;
	QLabel* height; QLineEdit* Height;
	QPushButton* setStep;
	QLineEdit* step;
	QPushButton* addCube;
	QPushButton* editCube;
	QPushButton* deleteCube;
	QPushButton* selectCube;
	QPushButton* boom;
	QPushButton* boom1;
	QPushButton* boom2;
	QPushButton* boom3;
	QPushButton* boom4; // refine just like chinese money in old age
	QPushButton* refine;

	QPushButton* spiltX;
	QPushButton* spiltY;
	QPushButton* spiltZ;
	
	QPushButton* addCubeWithMouse;
	QPushButton* disconnectNode;
	QPushButton* connectNode;
	QPushButton* selectMutiNodes;
	QPushButton* selectALine;
	QPushButton* xyView;
	QPushButton* xyView_;
	QPushButton* yzView;
	QPushButton* yzView_;
	QPushButton* xzView;
	QPushButton* xzView_;
	QPushButton* xExpand;
	QPushButton* yExpand;
	QPushButton* zExpand;
	QPushButton* xCrop;
	QPushButton* yCrop;
	QPushButton* zCrop;
	
	QCheckBox* axisShow;
	QCheckBox* groundShow;
	QCheckBox* skelShow;
	QCheckBox* mirrorShow;
	QCheckBox* mirrorMode;
	QCheckBox* dragMode;
	QCheckBox* showTriMesh;
	QCheckBox* showProjectLines;
	QCheckBox* roateByMouse;
	QButtonGroup *drawMode;
	QRadioButton *drawSkelMode;
	QRadioButton *drawSelectSkelMode;
	QRadioButton *drawSelectSkelsMode;
	QCheckBox* vertexSnapMode;

	QGroupBox* basic_Topo_Box;
	QPushButton* readTestLine;
	QPushButton* actSimpleWithCgal;
	QPushButton* actFittingCurve; // In fact, it is Polygon
	QPushButton* actFittingCurves;
	QPushButton* actRMF;
	QPushButton* pipeReCon;
	

signals:
	void xslidderChanged(int value);
	void yslidderChanged(int value);
	void zslidderChanged(int value);
	void construct_vertex_texture_signal();

public slots:
	
	void addCubeSignal();
	void deleteCubeSignal();
	void editCubeSignal();
	void selectCubeSignal();
	void selectMutiNodesSignal();
	void connectNodeSignal();
	void disconnectNodeSignal();
	void reNewDataSignal(double x, double y, double z, double angleX, double angleY, double angleZ, double length, double width, double height);
	void genMesh();
	void addCubeWithMouseSignal();
	void setStepSignal();
	void changeAngleAutoSignal();
	void generateLinkBodySignal();
	void topoSplitSignal();
	void refineSignal();
	void selectALineSignal();
	void xyViewSignal();
	void yzViewSignal();
	void xzViewSignal();
	void xyView_Signal();
	void yzView_Signal();
	void xzView_Signal();
	void axisShowChanged(int);
	void groundShowChanged(int);
	void skelShowChanged(int);
	void mirrorShowChanged(int);
	void mirrorModeChanged(int);
	void dragModeChanged(int);
	void showTriMeshChanged(int);
	void showProjectLinesChanged(int);
	void drawModeChange();
	void spiltXSignal();
	void spiltYSignal();
	void spiltZSignal();
	void xExpandSignal();
	void yExpandSignal();
	void zExpandSignal();
	void xCropSignal();
	void yCropSignal();
	void zCropSignal();
	void roateByMouseChange(int arg);
	void vertexSnapModeChange(int arg);
	void boom4Signal();

	void simpleWithCgalSignal();
	void readTestLineSignal();
	void fittingCurveSignal();
	void fittingCurvesSignal();
	void actRMFSignal();
	void pipeline2();
	
private:
	void initDialog();
	void createWidget();
	void createLayout();
	void createSVMWidget();
public:
	/*void sendslot()
	{
		emit construct_vertex_texture_signal();
	}*/
};
