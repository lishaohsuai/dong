#pragma once

#include <QtOpenGL/QtOpenGL> 
#include <QtOpenGL/QGLWidget>
#include <QWidget>
#include <Subdivision.h>
#include <memory>
#include <ThirdParty\colorramp\HSVcolormap.h>
#include "Sort.h"
#include <qdebug.h>
#include "node.hpp"
#include "singalFather.h"
#include "ComHeader.h"
#include "skelSimple.h"
#include "project.h"

class QGLViewerWidget : public QGLWidget
{
	Q_OBJECT

public:
	// skel
	void setRoot(BMesh::Node *_root);
	void setNodeVec(std::vector<BMesh::Node *> &v);
	void drawSkel();
	void DrawSkel(); // draw complex skel
	void setSkel(OurSkel *ourSkel);
	
	void BFSTraverse(BMesh::Node *root);
	void BFS(BMesh::Node *root);
	void drawAxis();
	void drawHorizon();
	void drawXOZ();
	void helpString() const;
	void setxyView();
	void setyzView();
	void setxzView();
	void setxy_View();
	void setyz_View();
	void setxz_View();
	std::unordered_map<int, bool> visited;
	std::queue<BMesh::Node*> Q;
	BMesh::Node *root;
	std::vector<BMesh::Node*> nodeVec;
	double depth;
	bool ctlPressed;// scale
	bool xPressed; // x Axisymmetric
	bool yPressed; // y Axisymmetric
	bool zPressed; // z Axisymmetric
	bool oPressed; // o Axisymmetric
	bool addAndDrag; // darg
	V3f mousePosition;
	bool bshowAxis;
	bool bshowGround;
	bool bshowSKEL;
	bool bshowMirror;
	bool bdragMode;
	bool bshowTriMeshChanged;
	bool bshowProjectLinesChanged;
	bool bRotateByMouse;
	bool bVertexSnap;
	bool bmirrorMode;
	double px; double py; double pz;
	OurSkel *myskel;
	Project* proj;

	std::vector<std::vector<BMesh::Point3D>> skels;
	std::vector<std::vector<BMesh::Point3D>> lines;
public:
	QGLViewerWidget(QWidget* _parent = 0);
	QGLViewerWidget(QGLFormat& _fmt, QWidget* _parent = 0);
	QGLViewerWidget(HexV3fMesh* _mesh, QWidget* _parent = 0);
	virtual ~QGLViewerWidget();

	void draw_tri_mesh_solidflat() const;
	void draw_project_lines() const;
	void drawTestLines() const;
	void setlines(std::vector<std::vector<BMesh::Point3D>>& s) {
		lines = s;
	}
	void setSkel(std::vector<std::vector<BMesh::Point3D>>& s) {
		skels = s;
	}
	void setmesh(std::tr1::shared_ptr<HexV3fMesh> _mesh)
	{
		//mesh = _mesh;
	}
	void setmesh(HexV3fMesh* _mesh)
	{
		mesh = _mesh;
	}
	void setTriMesh(Project* p) {
		proj = p;
	}
	void setdraw_mode_(int _draw_mode)
	{
		draw_mode_ = _draw_mode;
		updateGL();
	}
	void setcell_center(std::vector<V3f> _cell_center)
	{
		cell_center.swap(std::vector<V3f>());
		cell_center = _cell_center;
	}
	void seted(double _max, double _min)
	{
		minEdgeLen = _min;
		maxEdgeLen = _max;
	}
	void setbb(V3f max, V3f min)
	{
		bbMax = max;
		bbMin = min;
	}
	void setoribb(V3f max, V3f min)
	{
		oribbMax = max;
		oribbMin = min;
	}
	void setbffv(std::vector<std::vector<V3f>> _boundary_face_v, std::vector<V3f> _normal_boundary_face_v
		, std::vector<FaceHandle> _boundary_face, std::vector<std::vector<int>> _boundary_face_VID)
	{
		boundary_face_v.swap(std::vector<std::vector<V3f>>());
		normal_boundary_face_v.swap(std::vector<V3f>());
		boundary_face.swap(std::vector<FaceHandle>());
		boundary_face_VID.swap(std::vector<std::vector<int>>());
		boundary_face = _boundary_face;
		boundary_face_v = _boundary_face_v;
		normal_boundary_face_v = _normal_boundary_face_v;
		boundary_face_VID = _boundary_face_VID;
	}
	/*void addbffv(std::vector<std::vector<V3f>> _boundary_face_v, std::vector<V3f> _normal_boundary_face_v
		, std::vector<FaceHandle> _boundary_face)
	{
		boundary_face.insert(boundary_face.end(), _boundary_face.begin(), _boundary_face.end());
		boundary_face_v.insert(boundary_face_v.end(), _boundary_face_v.begin(), _boundary_face_v.end());
		normal_boundary_face_v.insert(normal_boundary_face_v.end(), _normal_boundary_face_v.begin(), _normal_boundary_face_v.end());
	}*/
	void setoriv(std::vector<V3f> _oriv)
	{
		oriv.swap(std::vector<V3f>());
		oriv = _oriv;
	}
	void setbeisuv(std::vector<std::vector<V3f>> _surv, std::vector<std::vector<std::vector<V3f>>> _bei_surv_dat,
		int tempdraw_num)
	{
		beisurv.swap(std::vector<std::vector<V3f>>());
		bei_surv_dat.swap(std::vector<std::vector<std::vector<V3f>>>());
		beisurv = _surv;
		bei_surv_dat = _bei_surv_dat;
		int temp = bei_surv_dat.size();
		datdrawnum.resize(temp);
		for (int i = 0; i < temp; i++)
		{
			datdrawnum[i] = i;
		}
		tempdraw.resize(tempdraw_num, -1);
	}
	void setbeinorsur(std::vector<V3f> _bei_nor_sur, std::vector<std::vector<V3f>> _nor_bei_surv_dat,
		std::vector<V3f> _bei_surv_center)
	{
		bei_nor_sur.swap(std::vector<V3f>());
		nor_bei_surv_dat.swap(std::vector<std::vector<V3f>>());
		bei_surv_center.swap(std::vector<V3f>());
		bei_nor_sur = _bei_nor_sur;
		nor_bei_surv_dat = _nor_bei_surv_dat;
		bei_surv_center = _bei_surv_center;
	}
	void setedgeline(std::vector<std::vector<V3f>> _edgeline)
	{
		edgeline.swap(std::vector<std::vector<V3f>>());
		edgeline = _edgeline;
	}
	void setsingularface(std::vector<int> _singularface)
	{
		singularface.swap(std::vector<int>());
		singularface = _singularface;
	}
	void setvmesh(std::vector<HexV3fMesh> _vmesh, std::vector<std::vector<V3f>> _datnorm)
	{
		vmesh.swap(std::vector<HexV3fMesh>());
		vsavemesh.swap(std::vector<HexV3fMesh>());
		datnorm.swap(std::vector<std::vector<V3f>>());
		vmesh = _vmesh;
		vsavemesh = _vmesh;
		int temp = vsavemesh.size();
		datdrawnum.resize(temp);
		for (int i = 0; i < temp; i++)
		{
			datdrawnum[i] = i;
		}
		tempdraw.resize(vmesh.size(), -1);
		datnorm = _datnorm;
	}
	void setdatcellcen(std::vector<V3f> _datcellcen)
	{
		datcellcen.swap(std::vector<V3f>());
		datcellcen = _datcellcen;
	}
	void setdatbdy(std::vector<std::vector<bool>> _DATbdy, std::vector<std::vector<bool>> _DATedgever)
	{
		DATbdy.swap(std::vector<std::vector<bool>>());
		DATbdy = _DATbdy;
		DATedgever = _DATedgever;
	}
	void setisoline(std::vector<std::vector<std::vector<V3f>>> _isoline)
	{
		isoline.swap(std::vector<std::vector<std::vector<V3f>>>());
		isoline = _isoline;
	}
	void setdrawv_big_small(std::vector<V3f> _drawvbig, std::vector<V3f> _drawvsmall, std::vector<VertexHandle> _drawvbigid)
	{
		drawvbig.swap(std::vector<V3f>());
		drawvsmall.swap(std::vector<V3f>());
		drawvbigid.swap(std::vector<VertexHandle>());
		drawvbig = _drawvbig;
		drawvsmall = _drawvsmall;
		drawvbigid = _drawvbigid;
	}
	void seterrmax_min(double _errmax, double _errmin)
	{
		errmax = _errmax;
		errmin = _errmin;
	}
	void set_color(std::set<newV3f> _color, std::vector<double> _colorverr)
	{
		colorverr.swap(std::vector<double>());
		color.swap(std::set<newV3f>());
		color = _color;
		colorverr = _colorverr;
	}
	void set_color(std::vector<double> _colorverr)
	{
		colorverr.swap(std::vector<double>());
		colorverr.resize(_colorverr.size());
		for (int i = 0; i < _colorverr.size(); ++i)
		{
			colorverr[i] = fabs(_colorverr[i]);
			//colorverr[i] = -_colorverr[i];
		}
	}
	// �趨��ǵ�
	void setselectpoint(V3f _selectpoint)
	{
		selectpoint = _selectpoint;
	}
	void setcheckcell(std::vector<bool> _checkcell)
	{
		checkcell.swap(std::vector<bool>());
		checkcell = _checkcell;
	}
	void setbiaojiface(std::vector<FaceHandle> _biaojiface)
	{
		std::vector<FaceHandle>().swap(biaojiface);
		biaojiface = _biaojiface;
	}
	HexV3fMesh *mesh;

	//std::tr1::shared_ptr<HexV3fMesh> mesh;
	//std::tr1::shared_ptr<HexV3fMesh> mesh1(new HexV3fMesh());
	//HexV3fMesh *mesh1 = new HexV3fMesh();//���������Ļ
	std::vector<V3f> cell_center;//ÿ���������
	std::vector<std::vector<V3f>> boundary_face_v;
	std::vector<FaceHandle> boundary_face;
	std::vector<std::vector<int>> boundary_face_VID;
	std::vector<V3f> normal_boundary_face_v;
	std::vector<FaceHandle> cutting_face;
	std::vector<std::vector<V3f>> cutting_face_v;
	std::vector< std::vector<int> > cutting_face_VID;
	std::vector<V3f> normal_cutting_face_v;
	std::vector<V3f> vertex_texture;//����
	std::vector<std::vector<V3f>> beisurv;//����������Ŀ��Ƶ�
	std::vector<V3f> bei_nor_sur;
	std::vector<std::vector<std::vector<V3f>>> bei_surv_dat;
	std::vector<std::vector<V3f>> nor_bei_surv_dat;
	std::vector<std::vector<V3f>> edgeline;
	std::vector<V3f> bei_surv_center;
	std::vector<FaceHandle> biaojiface;
	std::vector<int> singularface;
	std::vector<bool> checkcell;
	std::vector<HexV3fMesh> vmesh;//��DAT��ʽ���ڻ滭
	std::vector<HexV3fMesh> vsavemesh;//��DAT��ʽ���ڴ���
	std::vector<HexV3fMesh> othermesh;//���и����DAT
	std::vector<int> tempdraw;//�̶���ɫ ÿ����ɫ���ظ�
	std::vector<int> tempdraw1;
	std::vector<int> pickdrawface;
	std::vector<V3f> pickdrawface_center;
	std::vector<std::vector<V3f>> datnorm;
	void updatedrawface(std::vector<int> &_pickdrawface)
	{
		pickdrawface = _pickdrawface;
		qDebug() << "pickdrawface size: " << pickdrawface.size();
		for (int i = 0; i < tempdraw.size(); ++i)
		{
			if (std::find(_pickdrawface.begin(), _pickdrawface.end(), i) != _pickdrawface.end())
				tempdraw[i] = 0;
			else
				tempdraw[i] = tempdraw1[i];
		}
		updateGL();
	}
	bool ischeckop = true;
	void opupdatedrawface()
	{
		if (ischeckop == true)
		{
			for (int i = 0; i < tempdraw.size(); ++i)
			{
				if (std::find(pickdrawface.begin(), pickdrawface.end(), i) == pickdrawface.end())
					tempdraw[i] = 0;
				else
					tempdraw[i] = tempdraw1[i];
			}
			ischeckop = false;
		}
		else
		{
			for (int i = 0; i < tempdraw.size(); ++i)
			{
				if (std::find(pickdrawface.begin(), pickdrawface.end(), i) == pickdrawface.end())
					tempdraw[i] = tempdraw1[i];
				else
					tempdraw[i] = 0;
			}
			ischeckop = true;
		}
		updateGL();
	}
	bool colorcheck = false;//�Ƿ��Ѿ������Ļ���
	std::vector<int> othermeshID;
	std::vector<V3f> datcellcen;
	std::vector<int> datdrawnum;
	std::vector<std::vector<bool>> DATbdy;
	std::vector<std::vector<bool>> DATedgever;
	std::vector<std::vector<std::vector<V3f>>> isoline;//�Ȳ���
	std::vector<std::vector<V3f>> line_quick;
	std::vector<V3f> drawvbig;//��Ҫ�����ĵ㣬���д����ĵ�
	std::vector<V3f> drawvsmall;//С���ĵ�
	std::vector<VertexHandle> drawvbigid;
	double errmax;
	double errmin;
	std::set<newV3f> color;
	std::vector<double> colorverr;
	V3f selectpoint;
	/*mesh1*/
	std::tr1::shared_ptr<HexV3fMesh> mesh1;
	std::vector<V3f> cell_center1;//ÿ���������
	std::vector<std::vector<V3f>> boundary_face_v1;
	std::vector<FaceHandle> boundary_face1;
	std::vector<std::vector<int>> boundary_face_VID1;
	std::vector<V3f> normal_boundary_face_v1;
	std::vector<std::vector<V3f>> cutting_face_v1;
	std::vector< std::vector<int> > cutting_face_VID1;
	std::vector<V3f> normal_cutting_face_v1;
	double minEdgeLen1;
	double maxEdgeLen1;
	V3f bbMin1;//boudning box
	V3f bbMax1;
	std::vector<bool> vertex_flag1;//�������ж��и�ʱ�Ƿ񻭳�
	std::vector<bool> ValidCell1;
	std::vector<bool> ValidFace1;
	void setmesh1(std::tr1::shared_ptr<HexV3fMesh> _mesh)
	{
		mesh1 = _mesh;
	}
	void setmesh1(HexV3fMesh* _mesh)
	{
		mesh1.reset(_mesh);
	}
	void setbffv1(std::vector<std::vector<V3f>> _boundary_face_v, std::vector<V3f> _normal_boundary_face_v
		, std::vector<FaceHandle> _boundary_face, std::vector<std::vector<int>> _boundary_face_VID)
	{
		boundary_face_v1.swap(std::vector<std::vector<V3f>>());
		normal_boundary_face_v1.swap(std::vector<V3f>());
		boundary_face1.swap(std::vector<FaceHandle>());
		boundary_face_VID1.swap(std::vector<std::vector<int>>());
		boundary_face1 = _boundary_face;
		boundary_face_v1 = _boundary_face_v;
		normal_boundary_face_v1 = _normal_boundary_face_v;
		boundary_face_VID1 = _boundary_face_VID;
	}
	void seted1(double _max, double _min)
	{
		minEdgeLen1 = _min;
		maxEdgeLen1 = _max;
	}
	void setbb1(V3f max, V3f min)
	{
		bbMax1 = max;
		bbMin1 = min;
	}
	bool datisbdy(int hexnum, FaceHandle f)
	{
		std::vector<VertexHandle> fv = vsavemesh[hexnum].faces_[f].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			if (DATbdy[hexnum][*fv_it] == true)
				return true;
		}
		return false;
	}
	bool datisbdy(int hexnum, EdgeHandle e)
	{
		/*VertexHandle fr = vsavemesh[hexnum].edges_[e].from_h;
		VertexHandle to = vsavemesh[hexnum].edges_[e].to_h;
		if (DATedgever[hexnum][fr] == true && DATedgever[hexnum][to] == true)
			return true;
		else
			return false;*/
		return DATedgever[hexnum][e];
	}

	enum { BOUNDART = 9, CUTTING };//�߽��滹���и���
	std::vector<V3f> oriv;
	std::vector<std::vector<double>> bezier_color;//���ڴ洢dat�滭��ʽ��rgbֵ ��������ļ�
	std::string name;//�������axl�ļ��ĵ�ַ
	std::vector<std::vector<std::vector<std::vector<V3f>>>> SolidData;//����bezier�Ŀ��Ƶ���Ϣ
	void set_axl_write(std::vector<std::vector<std::vector<std::vector<V3f>>>> _SolidData, std::string _name)
	{
		SolidData = _SolidData;
		name = _name;
	}
	void write2AXLFile();
protected:
	// Qt mouse events
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void wheelEvent(QWheelEvent*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
	GLdouble projection_matrix_[16];
	GLdouble modelview_matrix_[16];
	GLdouble modelview_matrix_xy[16];
	GLdouble modelview_matrix_yz[16];
	GLdouble modelview_matrix_xz[16];

	GLdouble modelview_matrix_xy_[16];
	GLdouble modelview_matrix_yz_[16];
	GLdouble modelview_matrix_xz_[16];
	virtual void draw_scene(int drawmode);
	int draw_mode_;
	int mouse_mode_;
	V3f  center_;
	double  radius_;
	bool map_to_sphere(const QPoint& _point, V3f& _result);
	QPoint  last_point_2D_;
	V3f  last_point_3D_;
	bool    last_point_ok_;
public:
	void draw_mesh_wireframe() const;
	void draw_mesh_wireframe1() const;
	void draw_mesh_wireframe_bei() const;
	//void draw_mesh_hidden_lines() const;
	void draw_mesh_solidflat() const;
	void draw_mesh_solidflattisu() const;
	void draw_mesh_solidflat_bei() const;
	void draw_mesh_solidflatDAT();
	void draw_mesh_solidflatIsoline();
	void draw_mesh_solidflatdatonly();
	void draw_mesh_solidflatIsoline_notdraw();
	void draw_mesh_all_lines() const;
	void draw_mesh_all_lines2() const;
	void draw_mesh_all_lines3() const;
	void draw_mesh_all_polysDAT();
	void draw_mesh_all_polys() const;
	void draw_mesh_all_polys_linesDAT1();
	void draw_mesh_all_polys_linesDAT2();
	void draw_mesh_all_polys_linesDATquick();
	void draw_mesh_all_polys_linesIsoline();
	void draw_mesh_all_polys_lines() const;
	void draw_mesh_singularity();
	void draw_mesh_all_points();
	void draw_mesh_all_points_bei();
	void draw_mesh_texture();
	void draw_oriv();
	void draw_oribb();
	void draw_bb();
	void draw_sv();
	void draw_mesh_all_lines1() const;
	void draw_mesh_all_points1();
	void draw_mesh_solidflat1() const;
	void draw_mesh_solidflatDatpoint();
	void draw_mesh_wireframeDatpoint();
	void draw_mesh_all_pointsDatpoint1();
	void draw_mesh_all_pointsDatpoint2();
	void draw_mesh_all_pointsInterior();
	void draw_mesh_solidflatInterior();
	void draw_mesh_all_pointsEdge();
	void draw_mesh_solidflatEdge();
	void draw_mesh_wireframeEdge();
	void draw_mesh_all_pointsCorner();
	void draw_mesh_solidflatTran();
	void draw_errorv();
	void draw_mesh_solidflatTran1();
	void draw_mesh_compare();
	void draw_mesh_compare_new();
	void draw_mesh_compare_newXJL();
	void drawtest();
	HSVColor setcolor;
	void draw_mesh_wireframe_view_singular();
	void draw_mesh_all_points_view_singular();
	void draw_mesh_errorv();
	void draw_mesh_test();
	void draw_beisurDATface();
	void draw_beisurDATface1();
	void draw_beisurDATline();
	void draw_beisurDATline1();
	void draw_beisurDATsingularface();
	void draw_small_triangle(V3f v);//������
	void draw_small_diamond(V3f v);//����
	void draw_small_square(V3f v);//������
	void draw_small_circle(V3f v);//Բ��
	void draw_small_circle1(V3f v);//Բ��
	void draw_small_line();
	void draw_small_face();
	void draw_small_face1();
	void drawString(const char* str);
	void drawzimu();
	void drawzimu1();
	void drawotherline();
	V3f cal_f_v(FaceHandle f, VertexHandle v);
	V3f cal_f_v1(FaceHandle f, VertexHandle v);
	void draw_mesh_all_points2();
	void drawfaceline();
	void drawfaceface();
	void draw_small_triangle1(V3f v);//������
	void draw_small_diamond1(V3f v);//����
	void draw_small_square1(V3f v);//������
	void draw_small_circle2(V3f v);//Բ��
public:
	std::vector<V3f> trianglev;
	std::vector<V3f> diamondv;
	std::vector<V3f> squarev;
	std::vector<V3f> circlev;
	bool small_check = false;

	// rotate the scene (around its center) and update modelview matrix
	void rotate(const V3f& _axis, double _angle);
private:
	// initialize OpenGL states (triggered by Qt)
	void initializeGL();
	// draw the scene (triggered by Qt)
	void paintGL();
	// handle resize events (triggered by Qt)
	void resizeGL(int w, int h);
	void setDefaultMaterial(void);
	void setDefaultLight(void);
	// translate the scene and update modelview matrix
	void translate(const V3f& _trans);
	
	
	
	void update_projection_matrix();
	void translation(QPoint p);
	void translationCell(QPoint p);
	void rotation(QPoint p);
	void init(void);
public:
	void set_scene_pos(const V3f& _center, double _radius);
	/* view the whole scene: the eye point is moved far enough from the
	center so that the whole scene is visible. */
	void view_all();
	float fovy() const { return 45.0f; }
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void clear_mesh()
	{
		//mesh = mesh1;
		//mesh.reset(new HexV3fMesh());
		mesh = new HexV3fMesh();
		cell_center.swap(std::vector<V3f>());
		boundary_face_v.swap(std::vector<std::vector<V3f>>());
		normal_boundary_face_v.swap(std::vector<V3f>());
		cutting_face_v.swap(std::vector<std::vector<V3f>>());
		normal_cutting_face_v.swap(std::vector<V3f>());
		boundary_face.swap(std::vector<FaceHandle>());
		boundary_face_VID.swap(std::vector<std::vector<int>>());
		cutting_face_VID.swap(std::vector<std::vector<int>>());
		vertex_texture.swap(std::vector<V3f>());//����
		beisurv.swap(std::vector<std::vector<V3f>>());//����������Ŀ��Ƶ�
		bei_nor_sur.swap(std::vector<V3f>());
		vmesh.swap(std::vector<HexV3fMesh>());//��DAT��ʽ���ڻ滭
		vsavemesh.swap(std::vector<HexV3fMesh>());//��DAT��ʽ���ڴ���
		othermesh.swap(std::vector<HexV3fMesh>());//���и����DAT
		othermeshID.swap(std::vector<int>());
		datcellcen.swap(std::vector<V3f>());
		datdrawnum.swap(std::vector<int>());
		DATbdy.swap(std::vector<std::vector<bool>>());
		DATedgever.swap(std::vector<std::vector<bool>>());
		isoline.swap(std::vector<std::vector<std::vector<V3f>>>());//�Ȳ���
		beisurv.swap(std::vector<std::vector<V3f>>());
		bei_surv_dat.swap(std::vector<std::vector<std::vector<V3f>>>());
		std::vector<FaceHandle>().swap(biaojiface);
		bei_nor_sur.swap(std::vector<V3f>());
		nor_bei_surv_dat.swap(std::vector<std::vector<V3f>>());
		bei_surv_center.swap(std::vector<V3f>());
		edgeline.swap(std::vector<std::vector<V3f>>());
		singularface.swap(std::vector<int>());
		colorcheck = false;
		std::vector<int>().swap(tempdraw);//�̶���ɫ ÿ����ɫ���ظ�
		std::vector<int>().swap(tempdraw1);
		updateGL();
	}
	void constructShowMesh();
protected:
	void constructBoundaryMesh();
	void constructShowMeshFaceCenter();
	bool showRight; double x_per;
	bool showUp;    double y_per;
	bool showBack;  double z_per;
	int Verterx_Texture_ID;
	std::vector<bool> vertex_flag;//�������ж��и�ʱ�Ƿ񻭳�
	std::vector<bool> ValidCell;
	std::vector<bool> ValidFace;
	double minEdgeLen;
	double maxEdgeLen;
	V3f bbMin;//boudning box
	V3f bbMax;
	V3f oribbMin;//boudning box
	V3f oribbMax;
public slots:
	void xSliderShow(int value);
	void ySliderShow(int value);
	void zSliderShow(int value);
	void construct_vertex_texture();
	void rotation();
public://2n-Pointר��
	std::vector<V3f> twonpoint;
	void settwonpoint(std::vector<V3f> _twonpoint)
	{
		twonpoint = _twonpoint;
	}
	void drawtwonpoint();
	void drawtwonpointlines();
public://�����Ļ��ȡ����
	void pick_point(int x, int y, double p[3]);
	void pick_point_get_depth(int x, int y, double p[3], double& d);
	void pick_point_with_depth(int x, int y, double p[3], double d);
	std::vector<VertexHandle> pickpoint_v;
	std::vector<VertexHandle> pickpoint_vtemp;
	std::vector<V3f> pickpoint_v3f;
	//std::vector<V3f> pickpoint_v3ftemp;
	//std::vector<std::vector<V3f>> pickpoint_v3fline;
	//int v3fnum = 0;
	std::vector<std::vector<VertexHandle>> pickpoint_vtoline;
	std::vector<EdgeHandle> pickedge_e;
	std::vector<FaceHandle> pickface;
	std::vector<CellHandle> pickcell;
	std::vector<std::vector<EdgeHandle>> curve;

	int mouse_event = DEFAULT;
	void setmouse_event(int _mouse_event)
	{
		mouse_event = _mouse_event;
	}
	void drawpickpoint();
	void drawpickpointtoline();
	void drawlinepoint();
	void drawpickedge();
	void vtoline();
	void drawpickedface();
	void drawpickedcell();
public:
	std::vector<std::pair<V3f, V3f>> frame;
	std::vector<std::vector<V3f>> frameface;
	std::vector<V3f> framefacenrom;
	void setframe(std::vector<std::pair<V3f, V3f>> _frame, std::vector<std::vector<V3f>> _frameface,
		std::vector<V3f> _framefacenrom)
	{
		frame.swap(std::vector<std::pair<V3f, V3f>>());
		frameface.swap(std::vector<std::vector<V3f>>());
		framefacenrom.swap(std::vector<V3f>());
		frame = _frame;
		frameface = _frameface;
		framefacenrom = _framefacenrom;
	}
	void drawframe();
	void drawframeface();
public:
	void draw_mesh_solidflat_sphere() const;
public:
	void grab1();
public:
	QuadV3fMesh *quad;
	QuadV3fMesh *quad1;
	QuadV3fMesh *quadframe;
	std::vector<EdgeHandle> drawedge;
	void setquad(QuadV3fMesh *_quad, std::vector<EdgeHandle> _drawedge)
	{
		quad = _quad;
		drawedge = _drawedge;
	}
	void setquad1(QuadV3fMesh *_quad1)
	{
		quad1 = _quad1;
	}
	void setquadframe(QuadV3fMesh *_quadframe)
	{
		quadframe = _quadframe;
	}
	void draw_quad();
	void draw_quad1();
	void draw_quadlines();
	void draw_quadlines1();
	void draw_quadpickv();
	void draw_quadv();
	void drawframequad();
	std::vector<VertexHandle> quadpick;
public:
	std::vector<std::vector<V3f>> curve_v;
	void setcurve_v(std::vector<std::vector<V3f>> &_curve_v)
	{
		curve_v = _curve_v;
	}
	void drawcurvepoint();
	void drawcurve();
public:
	std::vector<std::vector<std::vector<V3f>>> bei_surv_dat1;
	std::vector<std::vector<V3f>> nor_bei1;
	std::vector<std::vector<std::vector<V3f>>> bei_surv_datframe;
	std::vector<std::vector<V3f>> nor_beiframe;
	void setsurv1(std::vector<std::vector<std::vector<V3f>>> _bei_surv_dat1, std::vector<std::vector<V3f>> _nor_bei1)
	{
		bei_surv_dat1 = _bei_surv_dat1;
		nor_bei1 = _nor_bei1;
	}
	void setsurvframe(std::vector<std::vector<std::vector<V3f>>> _bei_surv_datframe, std::vector<std::vector<V3f>> _nor_beiframe)
	{
		bei_surv_datframe = _bei_surv_datframe;
		nor_beiframe = _nor_beiframe;
	}
	void draw_beisurDATsingularface1();
	void draw_beisurDATsingularfaceframe();
public:
	std::vector<beziersurface> bf;
	void setbf(std::vector<beziersurface> &_bf)
	{
		bf = _bf;
	}
	void drawbsvface();
	void drawbsvline();
	std::vector<mvh> vH;
	void setvH(std::vector<mvh> &_vH)
	{
		vH = _vH;
	}
	void draw_mesh_solidflatVH();
	std::vector<frame1> vframe;
	void setvF(std::vector<frame1> &_vframe)
	{
		vframe = _vframe;
	}
	void draw_beisurDATsingularfaceframeV();
public:
	std::vector<std::vector<CellHandle>> tempcell;
	std::vector<V3f> tempcen;
	std::vector<bool> xjlcheck;
};
