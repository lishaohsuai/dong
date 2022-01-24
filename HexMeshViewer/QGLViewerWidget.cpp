#include "QGLViewerWidget.h"
#include <glut.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include "FileHandler.h"
#include "solutionSkel.h"


#if !defined(M_PI)
#  define M_PI 3.1415926535897932
#endif

const double TRACKBALL_RADIUS = 0.6;
#define WIDTH 400
#define HEIGHT 400
#define BMP_Header_Length 54
//using namespace Qt;

QGLViewerWidget::QGLViewerWidget(QWidget* _parent)
	: QGLWidget(_parent)
{
	init();
	showRight = true; showUp = true; showBack = true;
	x_per = 0.0; y_per = 0.0; z_per = 0.0;
	Verterx_Texture_ID = 0;
	this->grabKeyboard();
	quad = new QuadV3fMesh();
	quad1 = new QuadV3fMesh();
	quadframe = new QuadV3fMesh();
	depth = 0;
	ctlPressed = false;
	xPressed = false;
	yPressed = false;
	zPressed = false;
	oPressed = false;
	bshowAxis = false;
	bshowGround = false;
	bshowSKEL = false;
	bshowMirror = false;
	bdragMode = false;
	bshowTriMeshChanged = false;
	bmirrorMode = false;
	bRotateByMouse = false;
	bVertexSnap = false;
	bshowProjectLinesChanged = false;
}

QGLViewerWidget::QGLViewerWidget(QGLFormat& _fmt, QWidget* _parent)
	: QGLWidget(_fmt, _parent)
{
	init();
	showRight = true; showUp = true; showBack = true;
	x_per = 0.0; y_per = 0.0; z_per = 0.0;
	Verterx_Texture_ID = 0;
	this->grabKeyboard();
	quad = new QuadV3fMesh();
	quad1 = new QuadV3fMesh();
	quadframe = new QuadV3fMesh();
	depth = 0;
}

QGLViewerWidget::QGLViewerWidget(HexV3fMesh* _mesh, QWidget* _parent)
	: QGLWidget(_parent), mesh(_mesh)
{
	init();
	showRight = true; showUp = true; showBack = true;
	x_per = 0.0; y_per = 0.0; z_per = 0.0;
	Verterx_Texture_ID = 0;
	this->grabKeyboard();
	quad = new QuadV3fMesh();
	quad1 = new QuadV3fMesh();
	quadframe = new QuadV3fMesh();
	depth = 0;
}

QGLViewerWidget::~QGLViewerWidget()
{

}

QSize QGLViewerWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize QGLViewerWidget::sizeHint() const
{
	QRect rect = QApplication::desktop()->screenGeometry();
	return QSize(int(rect.width()*0.9), int(rect.height()));
}

void QGLViewerWidget::init(void)
{
	//// qt stuff
	setAttribute(Qt::WA_NoSystemBackground, true);
	setFocusPolicy(Qt::StrongFocus);
	//setAcceptDrops( true );  
	//setCursor(PointingHandCursor);

	//// draw mode
	//draw_mode_ = FLAT_LINES;
	//mouse_mode_ = Qt::NoButton;
}

void QGLViewerWidget::setDefaultMaterial(void)
{
	// material
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
}

void QGLViewerWidget::setDefaultLight(void)
{
	// lighting
	/*GLfloat pos1[] = { 10.0f, 10.0f, -10.0f, 0.0f };
	GLfloat pos2[] = { -10.0f, -10.0f, -10.0f, 0.0f };
	GLfloat pos3[] = { 0.0f,  0.0f,  10.0f,  0.0f };
	GLfloat pos4[] = { -10.0f,  10.0f,  -10.0f,  0.0f };*/
	GLfloat col1[] = { 0.5f,  0.5f,  0.5f,  1.0f };
	GLfloat col2[] = { 0.5f,  0.5f,  0.5f,  1.0f };
	GLfloat col3[] = { 0.5f,  0.5f,  0.5f,  1.0f };
	GLfloat col4[] = { 0.5f,  0.5f,  0.5f,  1.0f };

	GLfloat pos1[] = { 10.0f, 0.0f, 0.0f, 0.0f };
	GLfloat pos2[] = { 0.0f, 10.0f, 0.0f, 0.0f };
	GLfloat pos3[] = { 0.0f,  0.0f,  10.0f,  0.0f };
	/*GLfloat pos4[] = { 0.0f,  -100.0f,  0.0f,  0.0f };
	GLfloat pos5[] = { 0.0f,  0.0f,  100.0f,  0.0f };
	GLfloat pos6[] = { 0.0f,  0.0f,  -100.0f,  0.0f };*/


	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, col1);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, col1);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col1);

	/*glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT2, GL_POSITION, pos4);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col1);*/

	/*glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT2, GL_POSITION, pos5);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col1);

	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT2, GL_POSITION, pos6);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col1);*/
}

void QGLViewerWidget::initializeGL()
{
	// OpenGL state
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glDisable(GL_DITHER);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points 
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Material
	//setDefaultMaterial();
	// Lighting
	glLoadIdentity();// ��������glLoadIdentity()֮����ʵ���Ͻ���ǰ���Ƶ�����Ļ���ģ�������һ����λ����
	setDefaultLight();

	// scene pos and size
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);
	set_scene_pos(V3f(0.0, 0.0, 0.0), 2.0);

}

void QGLViewerWidget::resizeGL(int width, int height)
{
	//if (height == 0)
	//{
	//	height = 1;
	//}
	////���õ�ǰ���ӿڣ�Viewport��  
	//glViewport(0, 0, (GLint)width, (GLint)height);
	//glClearColor(1.0f, 1.0f, 0.5f, 0.0f);
	////ѡ��ͶӰ����  
	//glMatrixMode(GL_PROJECTION);
	//
	////����ͶӰ����  
	//glLoadIdentity();
	//
	////����͸��ͶӰ����  
	//gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
	////gluPerspective(45.0,(GLfloat)width() / (GLfloat)height(),0.1,100.0);  
	//// ѡ��ģ�͹۲����  
	//glMatrixMode(GL_MODELVIEW);
	//
	////����ģ�͹۲����  
	//glLoadIdentity();
	glViewport(0, 0, width, height);
	update_projection_matrix();
	updateGL();

}

void QGLViewerWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(projection_matrix_);
	//glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
	//glLoadIdentity();

	draw_scene(draw_mode_);

}

void QGLViewerWidget::translate(const V3f& _trans)
{
	// Translate the object by _trans
	// Update modelview_matrix_
	makeCurrent();
	glLoadIdentity();
	//std::cout << "[DEBUG]" << _trans.x << " " << _trans.y << " " << _trans.z << std::endl;
	glTranslated(_trans.x, _trans.y, _trans.z);
	glMultMatrixd(modelview_matrix_);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);

}

void QGLViewerWidget::setxyView(){
	makeCurrent();
	glLoadIdentity();
	for(int i=0; i<16; i++){
		modelview_matrix_[i] = modelview_matrix_xy[i];
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
	std::cout << "[DEBUG ] here \n" << center_.x << " " << center_.y << center_.z << "\n";
	update_projection_matrix();
	view_all();
	updateGL();
}

void QGLViewerWidget::setyzView(){
	makeCurrent();
	glLoadIdentity();
	for(int i=0; i<16; i++){
		modelview_matrix_[i] = modelview_matrix_yz[i];
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
	std::cout << "[DEBUG ] here \n" << center_.x << " " << center_.y << center_.z << "\n";
	update_projection_matrix();
	view_all();
	updateGL();
}

void QGLViewerWidget::setxzView(){
	makeCurrent();
	glLoadIdentity();
	for(int i=0; i<16; i++){
		modelview_matrix_[i] = modelview_matrix_xz[i];
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
	std::cout << "[DEBUG ] here \n" << center_.x << " " << center_.y << center_.z << "\n";
	update_projection_matrix();
	view_all();
	updateGL();
}


void QGLViewerWidget::setxy_View(){
	makeCurrent();
	glLoadIdentity();
	for(int i=0; i<16; i++){
		modelview_matrix_[i] = modelview_matrix_xy_[i];
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
	std::cout << "[DEBUG ] here \n" << center_.x << " " << center_.y << center_.z << "\n";
	update_projection_matrix();
	view_all();
	updateGL();
}

void QGLViewerWidget::setyz_View(){
	makeCurrent();
	glLoadIdentity();
	for(int i=0; i<16; i++){
		modelview_matrix_[i] = modelview_matrix_yz_[i];
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
	std::cout << "[DEBUG ] here \n" << center_.x << " " << center_.y << center_.z << "\n";
	update_projection_matrix();
	view_all();
	updateGL();
}

void QGLViewerWidget::setxz_View(){
	makeCurrent();
	glLoadIdentity();
	for(int i=0; i<16; i++){
		modelview_matrix_[i] = modelview_matrix_xz_[i];
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
	std::cout << "[DEBUG ] here \n" << center_.x << " " << center_.y << center_.z << "\n";
	update_projection_matrix();
	view_all();
	updateGL();
}


void QGLViewerWidget::rotate(const V3f& _axis, double _angle)
{
	// Rotate around center center_, axis _axis, by angle _angle
	// Update modelview_matrix_

	V3f t(modelview_matrix_[0] * center_.x +
		modelview_matrix_[4] * center_.y +
		modelview_matrix_[8] * center_.z +
		modelview_matrix_[12],
		modelview_matrix_[1] * center_.x +
		modelview_matrix_[5] * center_.y +
		modelview_matrix_[9] * center_.z +
		modelview_matrix_[13],
		modelview_matrix_[2] * center_.x +
		modelview_matrix_[6] * center_.y +
		modelview_matrix_[10] * center_.z +
		modelview_matrix_[14]);

	makeCurrent();
	glLoadIdentity();
	glTranslatef(t.x, t.y, t.z);
	glRotated(_angle, _axis.x, _axis.y, _axis.z);
	glTranslatef(-t.x, -t.y, -t.z);
	glMultMatrixd(modelview_matrix_);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);
}

bool QGLViewerWidget::map_to_sphere(const QPoint& _v2D, V3f& _v3D)
{
	// This is actually doing the Sphere/Hyperbolic sheet hybrid thing,
	// based on Ken Shoemake's ArcBall in Graphics Gems IV, 1993.
	double x = (2.0*_v2D.x() - width()) / width();
	double y = -(2.0*_v2D.y() - height()) / height();
	double xval = x;
	double yval = y;
	double x2y2 = xval * xval + yval * yval;

	const double rsqr = TRACKBALL_RADIUS * TRACKBALL_RADIUS;
	_v3D.x = xval;
	_v3D.y = yval;
	if (x2y2 < 0.5*rsqr)
	{
		_v3D.z = sqrt(rsqr - x2y2);
	}
	else
	{
		_v3D.z = 0.5*rsqr / sqrt(x2y2);
	}

	return true;
}

void QGLViewerWidget::update_projection_matrix()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width() / (GLfloat)height(),
		0.01*radius_, 100.0*radius_);
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix_);
	glMatrixMode(GL_MODELVIEW);
}

void QGLViewerWidget::set_scene_pos(const V3f& _center, double _radius)
{
	center_ = _center;
	radius_ = _radius;

	update_projection_matrix();
	view_all();
	for(int i=0; i<16; i++){
		modelview_matrix_xy[i] = modelview_matrix_[i];
	}
	V3f t(1, 0, 0);
	rotate(t, -180);
	for(int i=0; i<16; i++){
		modelview_matrix_xy_[i] = modelview_matrix_[i];
	}

	rotate(t, 90);
	for(int i=0; i<16; i++){
		modelview_matrix_yz[i] = modelview_matrix_[i];
	}

	rotate(t, -180);
	for(int i=0; i<16; i++){
		modelview_matrix_yz_[i] = modelview_matrix_[i];
	}
// 
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_xy);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);

	V3f tt(0, 1, 0);
	rotate(tt, 90);
	for(int i=0; i<16; i++){
		modelview_matrix_xz[i] = modelview_matrix_[i];
	}
	rotate(tt, 180);
	for(int i=0; i<16; i++){
		modelview_matrix_xz_[i] = modelview_matrix_[i];
	}
	for(int i=0; i<16; i++){
		modelview_matrix_[i] = modelview_matrix_xy[i];
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(modelview_matrix_);
}

void QGLViewerWidget::view_all()
{
	translate(V3f(-(modelview_matrix_[0] * center_.x +
		modelview_matrix_[4] * center_.y +
		modelview_matrix_[8] * center_.z +
		modelview_matrix_[12]),
		-(modelview_matrix_[1] * center_.x +
			modelview_matrix_[5] * center_.y +
			modelview_matrix_[9] * center_.z +
			modelview_matrix_[13]),
		-(modelview_matrix_[2] * center_.x +
			modelview_matrix_[6] * center_.y +
			modelview_matrix_[10] * center_.z +
			modelview_matrix_[14] +
			3.0*radius_)));
}

void QGLViewerWidget::wheelEvent(QWheelEvent* _event)
{
	if(ctlPressed != true){
		// Use the mouse wheel to zoom in/out
		float d = -(float)_event->delta() / 120.0 * 0.1 * radius_;
		translate(V3f(0.0, 0.0, d));
		updateGL();
	}else{
		if(_event->delta()>0)
		{
			if (xPressed) {
				SingalFather::getInstance()->sscaleCell(1.1, 0);
			}
			else if (yPressed) {
				SingalFather::getInstance()->sscaleCell(1.1, 1);
			}
			else if (zPressed) {
				SingalFather::getInstance()->sscaleCell(1.1, 2);
			}
			else if (oPressed) {
				SingalFather::getInstance()->sscaleCell(1.1, 3);
			}
			else {
				SingalFather::getInstance()->sscaleCell(1.1, -1);
			}
			qDebug() << "up";
		}
		else
		{
			if (xPressed) {
				SingalFather::getInstance()->sscaleCell(0.9, 0);
			}
			else if (yPressed) {
				SingalFather::getInstance()->sscaleCell(0.9, 1);
			}
			else if (zPressed) {
				SingalFather::getInstance()->sscaleCell(0.9, 2);
			}
			else if (oPressed) {
				SingalFather::getInstance()->sscaleCell(0.9, 3);
			}
			else {
				SingalFather::getInstance()->sscaleCell(0.9, -1);
			}
			qDebug() << "down";
		}
	}
	_event->accept(); // Prevent propagation to the parent
}




void QGLViewerWidget::mousePressEvent(QMouseEvent* _event)
{
	if (mouse_event == DEFAULT)
	{
		//assert(mouse_mode_ < N_MOUSE_MODES);
		last_point_2D_ = _event->pos();
		last_point_ok_ = map_to_sphere(last_point_2D_, last_point_3D_);
		mouse_mode_ = _event->button();
		std::cout << "[DEBUG] DEFAULT ...\n";
	}
	else if (mouse_event == PICK_POINT)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int vsize = mesh->vertices_.size();
			double mindis = 999; int minvid = 0;
			for (int i = 0; i < vsize; i++)
			{
				if (mesh->is_bdy(VertexHandle(i)))
				{
					V3f meshv = mesh->vertices_[i];
					double dis = (temp_pick - meshv).norm();
					if (dis < mindis)
					{
						mindis = dis;
						minvid = i;
					}
				}
			}
			if (std::find(pickpoint_v.begin(), pickpoint_v.end(), VertexHandle(minvid)) == pickpoint_v.end())
			{
				pickpoint_v.push_back(VertexHandle(minvid));
				pickpoint_v3f.push_back(mesh->vertices_[minvid]);
				pickpoint_vtemp.push_back(VertexHandle(minvid));
			}
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			qDebug() << pickpoint_v.size();
			/*for (int i = 0; i < pickpoint_v.size(); i++)
				qDebug() << pickpoint_v[i];*/
			for (int i = 0; i < pickpoint_v3f.size(); i++)
			{
				qDebug() << pickpoint_v[i];
				qDebug() << pickpoint_v3f[i].x << pickpoint_v3f[i].y << pickpoint_v3f[i].z;
			}
			pickpoint_vtoline.push_back(pickpoint_vtemp);
			pickpoint_vtemp.clear();
			for (int i = 0; i < pickpoint_v.size(); ++i)
			{
				mesh->v_cs[pickpoint_v[i]] = true;
			}
			//vtoline();
			//draw_mode_ = PICKPOINT_RESULT;
			//pickpoint_v.swap(std::vector<VertexHandle>());
			updateGL();
		}
	}
	else if (mouse_event == PICK_POINT_SKEL) {
		static bool havePick = false;
		//static V3f triMeshPoint(0, 0, 0);
		static int indexOfProjectLines = -1;
		if (_event->button() == Qt::LeftButton)
		{
			if (havePick == false) { // 还没有进行选点
				double p[3];
				pick_point(_event->x(), _event->y(), p);
				V3f temp_pick(p[0], p[1], p[2]);
				int vsize = proj->projectLines.size();
				double mindis = 999; int minvid = 0;
				for (int i = 0; i < vsize; i++) {
					V3f meshv = proj->projectLines[i].second;
					double dis = (temp_pick - meshv).norm();
					if (dis < mindis) {
						mindis = dis;
						minvid = i;
					}
				}
				indexOfProjectLines = minvid;
				proj->projectLines[indexOfProjectLines].second = proj->projectLines[indexOfProjectLines].first;
				havePick = true;
				updateGL();
			}
			else {
				double p[3];
				pick_point(_event->x(), _event->y(), p);
				V3f temp_pick(p[0], p[1], p[2]);
				int vsize = proj->points.size();
				double mindis = 999; int minvid = 0;
				for (int i = 0; i < vsize; i++) {
					V3f meshv = V3f(proj->points[i].x, proj->points[i].y, proj->points[i].z);
					double dis = (temp_pick - meshv).norm();
					if (dis < mindis) {
						mindis = dis;
						minvid = i;
					}
				}
				proj->projectLines[indexOfProjectLines].second.x = proj->points[minvid].x;
				proj->projectLines[indexOfProjectLines].second.y = proj->points[minvid].y;
				proj->projectLines[indexOfProjectLines].second.z = proj->points[minvid].z;
				indexOfProjectLines = -1;
				havePick = false;
				updateGL();
			}
		}
		else if (_event->button() == Qt::RightButton)
		{
			mouse_event = DEFAULT;
			havePick = false;
			
			updateGL();
		}
	}
	else if (mouse_event == PICK_EDGE)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int esize = mesh->edges_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < esize; i++)
			{
				if (mesh->is_bdy(EdgeHandle(i)))
				{
					V3f meshv = (mesh->vertices_[mesh->edges_[i].from_h] + mesh->vertices_[mesh->edges_[i].to_h]) / 2;
					double dis = distance(meshv, temp_pick);
					if (dis < mindis)
					{
						mindis = dis;
						mineid = i;
					}
				}
			}
			auto tt = std::find(pickedge_e.begin(), pickedge_e.end(), EdgeHandle(mineid));
			if (tt != pickedge_e.end()) {
				pickedge_e.erase(tt);
			}
			else {
				pickedge_e.push_back(EdgeHandle(mineid));
			}
			
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			std::cout << pickedge_e.size() << std::endl;
			for (int i = 0; i < pickedge_e.size(); i++)
				std::cout << pickedge_e[i] << " ";
			//curve.push_back(pickedge_e);
			//pickedge_e.clear();
			updateGL();
		}
	}
	else if (mouse_event == PICK_FACE)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int esize = mesh->faces_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < esize; i++)
			{
				if (mesh->is_bdy(FaceHandle(i)))
				{
					V3f meshv(0, 0, 0);
					for (int face_it = 0; face_it < 4; face_it++)
						meshv = meshv + mesh->vertices_[mesh->faces_[i].vs_f[face_it]];
					meshv = meshv / 4;
					double dis = distance(meshv, temp_pick);
					if (dis < mindis)
					{
						mindis = dis;
						mineid = i;
					}
				}
			}
			if (std::find(pickface.begin(), pickface.end(), FaceHandle(mineid)) == pickface.end())
				pickface.push_back(FaceHandle(mineid));
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			std::cout << pickface.size() << std::endl;
			for (int i = 0; i < pickface.size(); i++)
				std::cout << pickface[i] << " ";
			std::cout << std::endl;
			updateGL();
		}
	}
	else if (mouse_event == DELETE_FACE)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int esize = mesh->faces_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < esize; i++)
			{
				if (mesh->is_bdy(FaceHandle(i)))
				{
					V3f meshv(0, 0, 0);
					for (int face_it = 0; face_it < 4; face_it++)
						meshv = meshv + mesh->vertices_[mesh->faces_[i].vs_f[face_it]];
					meshv = meshv / 4;
					double dis = distance(meshv, temp_pick);
					if (dis < mindis)
					{
						mindis = dis;
						mineid = i;
					}
				}
			}
			std::vector<FaceHandle>::iterator pickface_it = std::find(pickface.begin(), pickface.end(), FaceHandle(mineid));
			if (pickface_it != pickface.end())
				pickface.erase(pickface_it);
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			qDebug() << pickface.size();
			/*for (int i = 0; i < pickface.size(); i++)
				qDebug() << pickface[i];*/
			updateGL();
		}
	}
	else if (mouse_event == PICK_CELL)
	{
		if (_event->button() == Qt::LeftButton)// if the leftButton of the mouse
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			// std::cout << "[DEBUG pickpoint]" << p[0] << " " << p[1] << " " << p[2] << std::endl;
			// maybe false choosen.
			int csize = mesh->cells_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < csize; i++)
			{
				//if (mesh->is_bdy(CellHandle(i)))
				//{
				V3f meshv(0, 0, 0);
				for (int cell_it = 0; cell_it < 8; cell_it++)
					meshv = meshv + mesh->vertices_[mesh->cells_[i].vertex_[cell_it]];
				meshv = meshv / 8;
				double dis = distance(meshv, temp_pick);
				if (dis < mindis)
				{
					mindis = dis;
					mineid = i;
				}
			}

			if (std::find(pickcell.begin(), pickcell.end(), CellHandle(mineid)) == pickcell.end()) // I think use set maybe better.
				pickcell.push_back(CellHandle(mineid));
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			std::cout << pickcell.size() << std::endl;
			for (int i = 0; i < pickcell.size(); i++)
			{
				qDebug() << pickcell[i];
				std::cout << mesh->cell_center[pickcell[i]].x << " " << mesh->cell_center[pickcell[i]].y
					<< " " << mesh->cell_center[pickcell[i]].z << std::endl;
			}
			updateGL();
		}
	}
	else if (mouse_event == PICK_CELLS_SKEL) {
		if (_event->button() == Qt::LeftButton)
		{
			double p[3];
			pick_point_get_depth(_event->x(), _event->y(), p, depth);// get d
			//pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			// maybe false choosen.
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
			if ((it = std::find(pickcell.begin(), pickcell.end(), CellHandle(mineid))) != pickcell.end()) {
				pickcell.erase(it);
			}
			else {
				pickcell.push_back(CellHandle(mineid));
			}
			SingalFather::getInstance()->supdateSelectMutiNodes();
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)
		{
			mouse_event = DEFAULT;
			pickcell.clear();
			updateGL();
		}
	}
	else if (mouse_event == PICK_CELL_SKEL)
	{
		if (_event->button() == Qt::LeftButton)// if the leftButton of the mouse
		{
			double p[3];
			pick_point_get_depth(_event->x(), _event->y(), p, depth);// get d
			//pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			px = p[0];
			py = p[1];
			pz = p[2];
			std::cout << "[DEBUG last_point_ok_] " << last_point_ok_ << std::endl;
			std::cout << "[DEBUG pickpoint]" << p[0] << " " << p[1] << " " << p[2] << std::endl;
			// maybe false choosen.
			int csize = mesh->cells_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < csize; i++)
			{
				//if (mesh->is_bdy(CellHandle(i)))
				//{
				V3f meshv(0, 0, 0);
				for (int cell_it = 0; cell_it < 8; cell_it++)
					meshv = meshv + mesh->vertices_[mesh->cells_[i].vertex_[cell_it]];
				meshv = meshv / 8;
				double dis = distance(meshv, temp_pick);
				if (dis < mindis)
				{
					mindis = dis;
					mineid = i;
				}
			}

			if (pickcell.size() == 1 && pickcell[0] == CellHandle(mineid) && (!bRotateByMouse)) { // this time we should change the mouse mode
				if(bdragMode) {
					V3f meshv(0, 0, 0);
					for (int cell_it = 0; cell_it < 8; cell_it++)
						meshv = meshv + mesh->vertices_[mesh->cells_[mineid].vertex_[cell_it]];
					mousePosition = meshv / 8;
					mousePosition.x += 0.01;
					SingalFather::getInstance()->addCubeUseMouseImpl();

					int csize = mesh->cells_.size();
					double mindis = 999; int mineid = 0;
					for (int i = 0; i < csize; i++)
					{
						//if (mesh->is_bdy(CellHandle(i)))
						//{
						V3f meshv(0, 0, 0);
						for (int cell_it = 0; cell_it < 8; cell_it++)
							meshv = meshv + mesh->vertices_[mesh->cells_[i].vertex_[cell_it]];
						meshv = meshv / 8;
						double dis = distance(meshv, mousePosition);
						if (dis < mindis)
						{
							mindis = dis;
							mineid = i;
						}
					}
					pickcell.clear();
					pickcell.push_back(CellHandle(mineid));
					mouse_mode_ = MOVE_CELL_SKEL;
					std::cout << "mouse_mode_ = MOVE_CELL_SKEL;\n";
				}else {
					mouse_mode_ = MOVE_CELL_SKEL; // mouse mode equal mouse event??
				}
				
			}
			else {
				pickcell.clear();
				pickcell.push_back(CellHandle(mineid));
			}
			updateGL();
			// -- update data --
			if (pickcell.size() == 1) {
				SingalFather::getInstance()->reNewData();
			}
			if(bRotateByMouse) {
				mouse_event = DEFAULT;
			}

		}
		else if (_event->button() == Qt::RightButton)
		{
			mouse_event = DEFAULT;
			std::cout << pickcell.size() << std::endl;
			for (int i = 0; i < pickcell.size(); i++)
			{
				qDebug() << pickcell[i];
				//std::cout << mesh->cell_center[pickcell[i]].x << " " << mesh->cell_center[pickcell[i]].y
				//	<< " " << mesh->cell_center[pickcell[i]].z << std::endl;
			}
			updateGL();
		}
	}
	//else if (mouse_event == MOVE_CELL_SKEL) {
	//	if (_event->button() == Qt::LeftButton)// if the leftButton of the mouse
	//	{
	//		std::cout << "[DEBUG] do nothing.\n";
	//		mouse_mode_ = MOVE_CELL_SKEL; // mouse mode equal mouse event??
	//	}
	//	else if (_event->button() == Qt::RightButton)//������һ��¼�
	//	{
	//		mouse_event = DEFAULT;
	//		std::cout << pickcell.size() << std::endl;
	//		for (int i = 0; i < pickcell.size(); i++)
	//		{
	//			qDebug() << pickcell[i];
	//			std::cout << mesh->cell_center[pickcell[i]].x << " " << mesh->cell_center[pickcell[i]].y
	//				<< " " << mesh->cell_center[pickcell[i]].z << std::endl;
	//		}
	//		updateGL();
	//	}
	//}
	else if (mouse_event == SET_CELL_SKEL) {
		if (_event->button() == Qt::LeftButton)// if the leftButton of the mouse
		{
			double p[3];
			pick_point_with_depth(_event->x(), _event->y(), p, depth);// get d
			//pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			std::cout << "[DEBUG pickpoint]" << p[0] << " " << p[1] << " " << p[2] << std::endl;
			// maybe false choosen.
			mousePosition = V3f(p[0], p[1], p[2]);
			updateGL();
			// -- update data --
			SingalFather::getInstance()->addCubeUseMouseImpl();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			std::cout << pickcell.size() << std::endl;
			for (int i = 0; i < pickcell.size(); i++)
			{
				qDebug() << pickcell[i];
				std::cout << mesh->cell_center[pickcell[i]].x << " " << mesh->cell_center[pickcell[i]].y
					<< " " << mesh->cell_center[pickcell[i]].z << std::endl;
			}
			updateGL();
		}
	}
	else if (mouse_event == DELETE_CELL)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int csize = mesh->cells_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < csize; i++)
			{
				//if (mesh->is_bdy(CellHandle(i)))
				//{
				V3f meshv(0, 0, 0);
				for (int cell_it = 0; cell_it < 8; cell_it++)
					meshv = meshv + mesh->vertices_[mesh->cells_[i].vertex_[cell_it]];
				meshv = meshv / 8;
				double dis = distance(meshv, temp_pick);
				if (dis < mindis)
				{
					mindis = dis;
					mineid = i;
				}
				//}
			}
			std::vector<CellHandle>::iterator pickcell_it = std::find(pickcell.begin(), pickcell.end(), CellHandle(mineid));
			if (pickcell_it != pickcell.end())
				pickcell.erase(pickcell_it);
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			qDebug() << pickcell.size();
			/*for (int i = 0; i < pickcell.size(); i++)
				qDebug() << pickcell[i];*/
			updateGL();
		}
	}
	else if (mouse_event == DRAWBEISUR)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int size = bei_surv_center.size();
			double mindis = 999; int mineid = 0; V3f temp_center;
			for (int i = 0; i < size; ++i)
			{
				V3f v = bei_surv_center[i];
				double dis = distance(temp_pick, v);
				if (dis < mindis)
				{
					mindis = dis;
					mineid = i;
					temp_center = v;
				}
			}
			if (std::find(pickdrawface.begin(), pickdrawface.end(), biaojiface[mineid]) == pickdrawface.end())
				pickdrawface.push_back(biaojiface[mineid]);
			if (std::find(pickdrawface_center.begin(), pickdrawface_center.end(), temp_center) == pickdrawface_center.end())
				pickdrawface_center.push_back(temp_center);
			//tempdraw[biaojiface[mineid]] = 0;
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			qDebug() << "pickdrawface size: " << pickdrawface.size();
			mouse_event = DEFAULT;
			updateGL();
		}
	}
	else if (mouse_event == DRAWBEISUR1)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int size = bei_surv_center.size();
			double mindis = 999; int mineid = 0; V3f temp_center;
			for (int i = 0; i < size; ++i)
			{
				V3f v = bei_surv_center[i];
				double dis = distance(temp_pick, v);
				if (dis < mindis)
				{
					mindis = dis;
					mineid = i;
					temp_center = v;
				}
			}
			std::vector<int>::iterator it = std::find(pickdrawface.begin(), pickdrawface.end(), biaojiface[mineid]);
			if (it != pickdrawface.end())
			{
				pickdrawface.erase(it);
				//tempdraw[biaojiface[mineid]] = tempdraw1[biaojiface[mineid]];
			}
			std::vector<V3f>::iterator it1 = std::find(pickdrawface_center.begin(),
				pickdrawface_center.end(), temp_center);
			if (it1 != pickdrawface_center.end())
			{
				pickdrawface_center.erase(it1);
			}
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			qDebug() << "pickdrawface size: " << pickdrawface.size();
			mouse_event = DEFAULT;
			updateGL();
		}
	}
	else if (mouse_event == QUADPICK)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int esize = quad->faces_.size();
			double mindis = 999; int mineid = 0;
			for (int i = 0; i < esize; i++)
			{
				V3f meshv(0, 0, 0);
				for (int face_it = 0; face_it < 4; face_it++)
					meshv = meshv + quad->vertices_[quad->faces_[i].vs_f[face_it]];
				meshv = meshv / 4;
				double dis = distance(meshv, temp_pick);
				if (dis < mindis)
				{
					mindis = dis;
					mineid = i;
				}
			}
			if (std::find(pickface.begin(), pickface.end(), FaceHandle(mineid)) == pickface.end())
			{
				quad->normal[mineid] = quad->normal[mineid] * -1;
			}
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			qDebug() << "pickdrawface size: " << pickdrawface.size();
			mouse_event = DEFAULT;
			updateGL();
		}
	}
	else if (mouse_event == PICK_POINT1)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			int vsize = quad->vertices_.size();
			double mindis = 999; int minvid = 0;
			for (int i = 0; i < vsize; i++)
			{
				V3f meshv = quad->vertices_[i];
				double dis = (temp_pick - meshv).norm();
				if (dis < mindis)
				{
					mindis = dis;
					minvid = i;
				}
			}
			if (std::find(quadpick.begin(), quadpick.end(), VertexHandle(minvid)) == quadpick.end())
			{
				quadpick.push_back(VertexHandle(minvid));
			}
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			mouse_event = DEFAULT;
			for (int i = 0; i < quadpick.size(); ++i)
			{
				quad->v_cs[quadpick[i]] = true;
			}
			updateGL();
		}
	}
	else if (mouse_event == PICKDATONLY)
	{
		if (_event->button() == Qt::LeftButton)//���������¼�
		{
			double p[3];
			pick_point(_event->x(), _event->y(), p);
			V3f temp_pick(p[0], p[1], p[2]);
			double mindis = 999; V3f temp_center;
			int cellnum = 0; int facenum = 0;
			for (int i = 0; i < vsavemesh.size(); ++i)
			{
				for (int j = 0; j < vsavemesh[i].faces_.size(); ++j)
				{
					V3f v = vsavemesh[i].cal_face_center(FaceHandle(j));
					double dis = distance(temp_pick, v);
					if (dis < mindis)
					{
						mindis = dis;
						cellnum = i;
						facenum = j;
					}
				}
			}
			datnorm[cellnum][facenum] = datnorm[cellnum][facenum] * -1;
			updateGL();
		}
		else if (_event->button() == Qt::RightButton)//������һ��¼�
		{
			qDebug() << "pickdrawface size: " << pickdrawface.size();
			mouse_event = DEFAULT;
			updateGL();
		}
	}
}

void  QGLViewerWidget::drawHorizon() {
	glColor3f(164 / 256.0, 128 / 256.0, 251 / 256.0);
	const int step = 1;
	const int maxLen = 100;
	glBegin(GL_LINES);
	for (int i = -maxLen; i < maxLen - step; i+= step) {
		for (int j = -maxLen; j < maxLen - step; j+= step) {
			glVertex3f(i, j, 0);
			glVertex3f(i, j+ step, 0);
			glVertex3f(i, j, 0);
			glVertex3f(i+ step, j, 0);
		}
	}
	glEnd();
}

void QGLViewerWidget::drawAxis() {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
}

void mutl(double lm_elements[], double rm_elements[], double rlt_elements[])
{
	rlt_elements[0] = lm_elements[0] * rm_elements[0] + lm_elements[1] * rm_elements[4] + lm_elements[2] * rm_elements[8] + lm_elements[3] * rm_elements[12];
	rlt_elements[1] = lm_elements[0] * rm_elements[1] + lm_elements[1] * rm_elements[5] + lm_elements[2] * rm_elements[9] + lm_elements[3] * rm_elements[13];
	rlt_elements[2] = lm_elements[0] * rm_elements[2] + lm_elements[1] * rm_elements[6] + lm_elements[2] * rm_elements[10] + lm_elements[3] * rm_elements[14];
	rlt_elements[3] = lm_elements[0] * rm_elements[3] + lm_elements[1] * rm_elements[7] + lm_elements[2] * rm_elements[11] + lm_elements[3] * rm_elements[15];

	rlt_elements[4] = lm_elements[4] * rm_elements[0] + lm_elements[5] * rm_elements[4] + lm_elements[6] * rm_elements[8] + lm_elements[7] * rm_elements[12];
	rlt_elements[5] = lm_elements[4] * rm_elements[1] + lm_elements[5] * rm_elements[5] + lm_elements[6] * rm_elements[9] + lm_elements[7] * rm_elements[13];
	rlt_elements[6] = lm_elements[4] * rm_elements[2] + lm_elements[5] * rm_elements[6] + lm_elements[6] * rm_elements[10] + lm_elements[7] * rm_elements[14];
	rlt_elements[7] = lm_elements[4] * rm_elements[3] + lm_elements[5] * rm_elements[7] + lm_elements[6] * rm_elements[11] + lm_elements[7] * rm_elements[15];

	rlt_elements[8] = lm_elements[8] * rm_elements[0] + lm_elements[9] * rm_elements[4] + lm_elements[10] * rm_elements[8] + lm_elements[11] * rm_elements[12];
	rlt_elements[9] = lm_elements[8] * rm_elements[1] + lm_elements[9] * rm_elements[5] + lm_elements[10] * rm_elements[9] + lm_elements[11] * rm_elements[13];
	rlt_elements[10] = lm_elements[8] * rm_elements[2] + lm_elements[9] * rm_elements[6] + lm_elements[10] * rm_elements[10] + lm_elements[11] * rm_elements[14];
	rlt_elements[11] = lm_elements[8] * rm_elements[3] + lm_elements[9] * rm_elements[7] + lm_elements[10] * rm_elements[11] + lm_elements[11] * rm_elements[15];

	rlt_elements[12] = lm_elements[12] * rm_elements[0] + lm_elements[13] * rm_elements[4] + lm_elements[14] * rm_elements[8] + lm_elements[15] * rm_elements[12];
	rlt_elements[13] = lm_elements[12] * rm_elements[1] + lm_elements[13] * rm_elements[5] + lm_elements[14] * rm_elements[9] + lm_elements[15] * rm_elements[13];
	rlt_elements[14] = lm_elements[12] * rm_elements[2] + lm_elements[13] * rm_elements[6] + lm_elements[14] * rm_elements[10] + lm_elements[15] * rm_elements[14];
	rlt_elements[15] = lm_elements[12] * rm_elements[3] + lm_elements[13] * rm_elements[7] + lm_elements[14] * rm_elements[11] + lm_elements[15] * rm_elements[15];
}


void QGLViewerWidget::setSkel(OurSkel *ourSkel){
	this->myskel = ourSkel;
}

void QGLViewerWidget::mouseReleaseEvent(QMouseEvent* /* _event */)
{
	//assert(mouse_mode_ < N_MOUSE_MODES);
	if(bdragMode) {
		mouse_event = PICK_CELL_SKEL;
	} else {
		mouse_mode_ = Qt::NoButton;
	}
	last_point_ok_ = false;
}

void QGLViewerWidget::mouseMoveEvent(QMouseEvent* _event)
{
	//assert(mouse_mode_ < N_MOUSE_MODES);

	QPoint newPoint2D = _event->pos();
	// enable GL context
	makeCurrent();
	V3f t;
	if (last_point_ok_)
	{

		switch (mouse_mode_)
		{
		case Qt::LeftButton:
			rotation(newPoint2D);
			break;
		case Qt::RightButton:
			translation(newPoint2D);
			break;
		case MOVE_CELL_SKEL:
			translationCell(newPoint2D);
			break;
		default:
			break;
		}
	} // end of if

	// remember this point
	last_point_2D_ = newPoint2D;
	last_point_ok_ = map_to_sphere(last_point_2D_, last_point_3D_);

	// trigger redraw
	updateGL();
}

void QGLViewerWidget::keyReleaseEvent(QKeyEvent* _event){
	if (_event->key() == Qt::Key_Control){
		ctlPressed = false;
	}
	else if (_event->key() == Qt::Key_X) {
		xPressed = false;
	}
	else if (_event->key() == Qt::Key_Y) {
		yPressed = false;
	}
	else if (_event->key() == Qt::Key_Z) {
		zPressed = false;
	}
	else if (_event->key() == Qt::Key_O) {
		oPressed = false;
	}
}

/**
 * @description: key pressevent, longtime press will run a lot of time
 * @param {*}
 * @return {*}
 */
void QGLViewerWidget::keyPressEvent(QKeyEvent* _event)
{
	if (_event->key() == Qt::Key_X && _event->modifiers() == (Qt::ControlModifier)) {// drag
		std::cout << "count\n"; 
	}
	else if (_event->key() == Qt::Key_X && _event->modifiers() == (Qt::AltModifier | Qt::ControlModifier)) {
		xPressed = true;
		ctlPressed = true;
	}
	else if (_event->key() == Qt::Key_Y && _event->modifiers() == (Qt::AltModifier | Qt::ControlModifier)) {
		yPressed = true;
		ctlPressed = true;
	}
	else if (_event->key() == Qt::Key_Z && _event->modifiers() == (Qt::AltModifier | Qt::ControlModifier)) {
		zPressed = true;
		ctlPressed = true;
	}
	else if (_event->key() == Qt::Key_O && _event->modifiers() == (Qt::AltModifier | Qt::ControlModifier)) {
		oPressed = true;
		ctlPressed = true;
	}
	else if (_event->key() == Qt::Key_X && _event->modifiers() == Qt::AltModifier) {
		xPressed = true;
	}
	else if (_event->key() == Qt::Key_Y && _event->modifiers() == Qt::AltModifier) {
		yPressed = true;
	}
	else if (_event->key() == Qt::Key_Z && _event->modifiers() == Qt::AltModifier) {
		zPressed = true;
	}
	else if (_event->key() == Qt::Key_O && _event->modifiers() == Qt::AltModifier) {
		oPressed = true;
	}else if (_event->key() == Qt::Key_X && _event->modifiers() == (Qt::AltModifier | Qt::ShiftModifier)) {
		SingalFather::getInstance()->sxRotateCell(-1);
	}else if(_event->key() == Qt::Key_Y && _event->modifiers() == (Qt::AltModifier | Qt::ShiftModifier)) {
		SingalFather::getInstance()->syRotateCell(-1);
	}else if(_event->key() == Qt::Key_Z && _event->modifiers() == (Qt::AltModifier | Qt::ShiftModifier)) {
		SingalFather::getInstance()->szRotateCell(-1);
	}else if (_event->key() == Qt::Key_X && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->sxRotateCell(1);
	}else if(_event->key() == Qt::Key_Y && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->syRotateCell(1);
	}else if(_event->key() == Qt::Key_Z && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->szRotateCell(1);
	}else if(_event->key() == Qt::Key_W && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->sfrontMoveCell(1);
	}else if(_event->key() == Qt::Key_S && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->sbackMoveCell(1);
	}else if(_event->key() == Qt::Key_A && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->sleftMoveCell(1);
	}else if(_event->key() == Qt::Key_D && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->srightMoveCell(1);
	}else if(_event->key() == Qt::Key_J && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->supMoveCell(1);
	}else if(_event->key() == Qt::Key_K && _event->modifiers() == Qt::ShiftModifier) {
		SingalFather::getInstance()->sdownMoveCell(1);
	}
	else if (_event->key() == Qt::Key_Z)
	{
		mouse_event = DEFAULT;
	}
	else if (_event->key() == Qt::Key_A)
	{
		mouse_event = DRAWBEISUR;
	}
	else if (_event->key() == Qt::Key_S)
	{
		mouse_event = DRAWBEISUR1;
	}
	else if (_event->key() == Qt::Key_D)
	{
		mouse_event = QUADPICK;
	}
	else if (_event->key() == Qt::Key_F)
	{
		draw_mode_ = QUADPICKV;
		mouse_event = PICK_POINT1;
	}
	else if (_event->key() == Qt::Key_X)
	{
		mouse_event = PICKDATONLY;
	}
	else if (_event->key() == Qt::Key_Q)
	{
		mouse_event = PICK_EDGE;
		draw_mode_ = SOLIDFLAT_AND_PICKEDGE;
	}
	else if (_event->key() == Qt::Key_W)
	{
		draw_mode_ = SOLIDFLAT_AND_PICKEDGE;
		pickedge_e.pop_back();
		updateGL();
	}
	else if (_event->key() == Qt::Key_E)
	{
		draw_mode_ = SOLIDFLAT_AND_PICKEDGE;
		pickedge_e.clear();
		updateGL();
	}
	else if (_event->key() == Qt::Key_R)
	{
		mouse_event = PICK_POINT;
		draw_mode_ = SOLIDFLAT_AND_PICKPOINT;
	}
	else if (_event->key() == Qt::Key_T)
	{
		draw_mode_ = SOLIDFLAT_AND_PICKPOINT;
		pickpoint_v.pop_back();
		pickpoint_v3f.pop_back();
		updateGL();
	}
	else if (_event->key() == Qt::Key_C)
	{
		mouse_event = PICK_FACE;
		draw_mode_ = SOLIDFLAT_AND_PICKEDFACE;
	}
	else if (_event->key() == Qt::Key_V)
	{
		draw_mode_ = SOLIDFLAT_AND_PICKEDFACE;
		pickface.pop_back();
		updateGL();
	}
	else if (_event->key() == Qt::Key_Delete) {
		SingalFather::getInstance()->sdelCell();
	}
	else if (_event->key() == Qt::Key_Control){
		ctlPressed = true;
	}
	else if (_event->key() == Qt::Key_H) {
		SingalFather::getInstance()->shelp();
	}


}




void QGLViewerWidget::translation(QPoint p)
{
	double z = -(modelview_matrix_[2] * center_.x +
		modelview_matrix_[6] * center_.y +
		modelview_matrix_[10] * center_.z +
		modelview_matrix_[14]) /
		(modelview_matrix_[3] * center_.x +
			modelview_matrix_[7] * center_.y +
			modelview_matrix_[11] * center_.z +
			modelview_matrix_[15]);

	double w = width(); double h = height();
	double aspect = w / h;
	double near_plane = 0.01 * radius_;
	double top = tan(fovy() / 2.0f*M_PI / 180.0f) * near_plane;
	double right = aspect * top;

	double dx = p.x() - last_point_2D_.x();
	double dy = p.y() - last_point_2D_.y();

	translate(V3f(2.0*dx / w * right / near_plane * z,
		-2.0*dy / h * top / near_plane * z,
		0.0f));
}

/**
 * @description: 应该改成相对
 * @param {QPoint} p
 * @return {*}
 */
void QGLViewerWidget::translationCell(QPoint p) {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble winX = double(p.x());
	GLdouble winY = double(viewport[3] - p.y());
	double pLocal[3] = { 0 };
	gluUnProject(winX, winY, depth, modelview_matrix_, projection_matrix_, viewport, &pLocal[0], &pLocal[1], &pLocal[2]);
	double relativePosition[3] = {0};
	relativePosition[0] = pLocal[0] - px;
	relativePosition[1] = pLocal[1] - py;
	relativePosition[2] = pLocal[2] - pz;
	px = pLocal[0]; py = pLocal[1]; pz = pLocal[2];
	if (xPressed) {
		SingalFather::getInstance()->sdeltaEditCell(relativePosition[0], relativePosition[1], relativePosition[2], 0);
	}
	else if (yPressed) {
		SingalFather::getInstance()->sdeltaEditCell(relativePosition[0], relativePosition[1], relativePosition[2], 1);
	}
	else if (zPressed) {
		SingalFather::getInstance()->sdeltaEditCell(relativePosition[0], relativePosition[1], relativePosition[2], 2);
	}
	else if (oPressed) {
		SingalFather::getInstance()->sdeltaEditCell(relativePosition[0], relativePosition[1], relativePosition[2], 3);
	}
	else {
		if(bmirrorMode) {
			SingalFather::getInstance()->sdeltaEditCell(relativePosition[0], relativePosition[1], relativePosition[2], 4);
		} else {
			SingalFather::getInstance()->sdeltaEditCell(relativePosition[0], relativePosition[1], relativePosition[2], -1);
		}
	}
	// update_projection_matrix();
	
}

void QGLViewerWidget::rotation(QPoint p)
{
	V3f  newPoint3D;
	bool newPoint_hitSphere = map_to_sphere(p, newPoint3D);
	if (newPoint_hitSphere)
	{
		//last_point_3D_ % newPoint3D;
		/*V3f axis = V3f(last_point_3D_.y*newPoint3D.z- last_point_3D_.z*newPoint3D.y,
			last_point_3D_.z*newPoint3D.x - last_point_3D_.x*newPoint3D.z,
			last_point_3D_.x*newPoint3D.y - last_point_3D_.y*newPoint3D.x);*/
		V3f axis = last_point_3D_ % newPoint3D;
		if (axis.sqrnorm() < 1e-7)
		{
			axis = V3f(1, 0, 0);
		}
		else
		{
			axis = axis.normalize();
		}
		// find the amount of rotation
		V3f d = V3f(last_point_3D_.x - newPoint3D.x, last_point_3D_.y - newPoint3D.y, last_point_3D_.z - newPoint3D.z);
		double t = 0.5*d.norm() / TRACKBALL_RADIUS;
		if (t < -1.0) t = -1.0;
		else if (t > 1.0) t = 1.0;
		double phi = 2.0 * asin(t);
		double  angle = phi * 180.0 / M_PI;
		
		if(bRotateByMouse) {
			SingalFather::getInstance()->sRotateCubeByMouse(axis, angle);
		} else{
			rotate(axis, angle);
		}
	}
}

void QGLViewerWidget::rotation() {
	V3f t = V3f(0, 1, 0);
	rotate(t, 0.15);
	updateGL();
}

void QGLViewerWidget::xSliderShow(int value)
{
	assert(value <= 100 && value >= 0);
	x_per = value / 100.0;
	constructShowMesh();
}

void QGLViewerWidget::ySliderShow(int value)
{
	assert(value <= 100 && value >= 0);
	y_per = value / 100.0;
	constructShowMesh();
}

void QGLViewerWidget::zSliderShow(int value)
{
	assert(value <= 100 && value >= 0);
	z_per = value / 100.0;
	constructShowMesh();
}

void QGLViewerWidget::constructShowMesh()
{
	//QTime time;
	//std::cout << "Begin to construct the Showing Mesh" <<std::endl;
	//time.start();
	if (mesh->vertices_.size() == 0)
		return;

	if (vertex_flag.size() != mesh->vertices_.size())
	{
		vertex_flag.resize(mesh->vertices_.size(), false);
	}

	//HexV3fMesh temp_VM = *mesh;
	double offset = minEdgeLen * 0.1;
	//VertexOnShowMesh.clear();
	if (showRight && showUp && showBack)
	{
		//x,y,z
		double x_plane = bbMax.x - x_per * (bbMax.x - bbMin.x) + offset;
		double y_plane = bbMax.y - y_per * (bbMax.y - bbMin.y) + offset;
		//double y_plane = y_per*(bbMax.y - bbMin.y) - offset;
		//double z_plane = bbMax.z - z_per*(bbMax.z - bbMin.z) + offset;
		double z_plane = bbMin.z + z_per * (bbMax.z - bbMin.z) - offset;

		//std::vector<V3f>::iterator v_it = temp_VM.vertices_.begin();
		//�ж�orgv�ĵ��Ƿ���Ի���
		//int orivCount = 0;
		oriv.swap(std::vector<V3f>());
		for (std::vector<V3f>::iterator oriv_it = mesh->oriv.begin(); oriv_it != mesh->oriv.end(); oriv_it++)
		{
			V3f v = *oriv_it;
			if (v.x <= x_plane && v.y <= y_plane && v.z >= z_plane)
			{
				oriv.push_back(v);
			}
		}
		//XJL
		xjlcheck.swap(std::vector<bool>());
		xjlcheck.resize(mesh->cells_.size(), true);
		for (int xjl_num = 0; xjl_num < tempcen.size(); ++xjl_num)
		{
			V3f v = tempcen[xjl_num];
			if (v.x <= x_plane && v.y <= y_plane && v.z >= z_plane)
			{
				for (int xjll = 0; xjll < tempcell[xjl_num].size(); ++xjll)
					xjlcheck[tempcell[xjl_num][xjll]] = true;
			}
			else
			{
				for (int xjll = 0; xjll < tempcell[xjl_num].size(); ++xjll)
					xjlcheck[tempcell[xjl_num][xjll]] = false;
			}
		}
		//�ж�DAT���Ƿ���Ի���
		vmesh.swap(std::vector<HexV3fMesh>());
		datdrawnum.swap(std::vector<int>());
		othermesh.swap(std::vector<HexV3fMesh>());
		othermeshID.swap(std::vector<int>());
		int datnum = 0;
		for (std::vector<HexV3fMesh>::iterator dat_it = vsavemesh.begin(); dat_it != vsavemesh.end(); dat_it++)
		{
			V3f v = datcellcen[datnum];
			if (v.x <= x_plane && v.y <= y_plane && v.z >= z_plane)
			{
				vmesh.push_back(*dat_it);
				datdrawnum.push_back(datnum);
			}
			else
			{
				othermesh.push_back(*dat_it);
				othermeshID.push_back(datnum);
			}
			datnum++;
		}
		int vertexCount = 0; //int temp_mesh_vertexCount = 0;
		for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)//�жϵ��Ƿ�Ϊ�ڲ�
		{
			V3f v = *v_it;
			//if( v.x <= x_plane && v.y <= y_plane && v.z <= z_plane )//in
			//if (v.x <= x_plane && v.y > y_plane && v.z <= z_plane)//in
			if (v.x <= x_plane && v.y <= y_plane && v.z >= z_plane)//in
			{
				vertex_flag[vertexCount] = true;
				//VertexOnShowMesh.push_back(v);
			}
			else
			{
				vertex_flag[vertexCount] = false;
			}
			++vertexCount;
		}

	}
	//�ж��ĸ����Ƿ���Ի���
	int cellCount = 0;
	if (ValidCell.size() != mesh->cells_.size())
	{
		ValidCell.resize(mesh->cells_.size());
	}
	for (std::vector<Cell>::iterator c_it = mesh->cells_.begin(); c_it != mesh->cells_.end(); c_it++)
	{
		bool valid_cell_OK = true;
		for (std::vector<VertexHandle>::iterator cv_it = (*c_it).vertex_.begin(); cv_it != (*c_it).vertex_.end(); cv_it++)
		{
			if (vertex_flag[*cv_it] == false)
				valid_cell_OK = false;
		}
		if (valid_cell_OK == true)
			ValidCell[cellCount] = true;
		else
			ValidCell[cellCount] = false;
		cellCount++;
	}
	//�ж��ĸ�����Ի���
	int faceCount = 0;
	boundary_face_v.swap(std::vector<std::vector<V3f>>());
	normal_boundary_face_v.swap(std::vector<V3f>());
	cutting_face_v.swap(std::vector<std::vector<V3f>>());
	normal_cutting_face_v.swap(std::vector<V3f>());
	boundary_face_VID.swap(std::vector<std::vector<int>>());
	cutting_face_VID.swap(std::vector<std::vector<int>>());
	boundary_face.swap(std::vector<FaceHandle>());
	cutting_face.swap(std::vector<FaceHandle>());
	/*boundary_face_v.clear();
	normal_boundary_face_v.clear();
	cutting_face_v.clear();
	normal_cutting_face_v.clear();*/
	for (std::vector<Face>::iterator f_it = mesh->faces_.begin(); f_it != mesh->faces_.end(); f_it++)
	{
		std::set<CellHandle, compare_OVM> f_lj_c = mesh->neighbor_f[faceCount];
		bool valid_face_OK = false;
		std::vector<bool> draw;
		int face_type;
		//enum {BOUNDART = 0,CUTTING};
		for (std::set<CellHandle, compare_OVM>::iterator f_lj_cv = f_lj_c.begin(); f_lj_cv != f_lj_c.end(); f_lj_cv++)
		{
			draw.push_back(ValidCell[*f_lj_cv]);
			/*if (ValidCell[*f_lj_cv] == true)
				valid_face_OK = true;*/

		}
		//���߽�����и���
		int drawsize = draw.size();
		if (drawsize == 2)
		{
			if ((draw[0] == false) && (draw[1] == false))
			{
				valid_face_OK = false;
			}
			else if ((draw[0] == true) && (draw[1] == true))
				valid_face_OK = false;
			else
			{
				valid_face_OK = true;
				face_type = CUTTING;
			}
		}
		else
		{
			if (draw[0] == true)
			{
				valid_face_OK = true;
				face_type = BOUNDART;
			}
			else
				valid_face_OK = false;
		}
		if (valid_face_OK == true)
		{
			std::vector<VertexHandle> fv = (*f_it).vs_f;
			std::vector<V3f> one_face;
			std::vector<int> one_face_vid;
			for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
			{
				one_face.push_back(mesh->vertices_[*fv_it]);
				one_face_vid.push_back(*fv_it);
			}
			if (face_type == BOUNDART)
			{
				boundary_face_v.push_back(one_face);
				boundary_face_VID.push_back(one_face_vid);
				normal_boundary_face_v.push_back(mesh->cal_norm(FaceHandle(faceCount)));
				boundary_face.push_back(FaceHandle(faceCount));
			}
			else if (face_type == CUTTING)
			{
				cutting_face_v.push_back(one_face);
				cutting_face_VID.push_back(one_face_vid);
				normal_cutting_face_v.push_back(mesh->cal_norm(FaceHandle(faceCount)));
				cutting_face.push_back(FaceHandle(faceCount));
			}
		}
		faceCount++;
	}

	/*mesh1*/
	if (mesh1 != NULL)
	{
		if (vertex_flag1.size() != mesh1->vertices_.size())
		{
			vertex_flag1.resize(mesh->vertices_.size(), false);
		}
		double offset1 = minEdgeLen1 * 0.1;
		if (showRight && showUp && showBack)
		{
			//x,y,z
			double x_plane = bbMax1.x - x_per * (bbMax1.x - bbMin1.x) + offset1;
			double y_plane = bbMax1.y - y_per * (bbMax1.y - bbMin1.y) + offset1;
			double z_plane = bbMin1.z + z_per * (bbMax1.z - bbMin1.z) - offset1;
			int vertexCount1 = 0;
			for (std::vector<V3f>::iterator v_it = mesh1->vertices_.begin(); v_it != mesh1->vertices_.end(); ++v_it)//�жϵ��Ƿ�Ϊ�ڲ�
			{
				V3f v = *v_it;
				if (v.x <= x_plane && v.y <= y_plane && v.z >= z_plane)//in
				{
					vertex_flag1[vertexCount1] = true;
				}
				else
				{
					vertex_flag1[vertexCount1] = false;
				}
				++vertexCount1;
			}
		}
		int cellCount1 = 0;
		if (ValidCell1.size() != mesh1->cells_.size())
		{
			ValidCell1.resize(mesh1->cells_.size());
		}
		for (std::vector<Cell>::iterator c_it = mesh1->cells_.begin(); c_it != mesh1->cells_.end(); c_it++)
		{
			bool valid_cell_OK = true;
			for (std::vector<VertexHandle>::iterator cv_it = (*c_it).vertex_.begin(); cv_it != (*c_it).vertex_.end(); cv_it++)
			{
				if (vertex_flag1[*cv_it] == false)
					valid_cell_OK = false;
			}
			if (valid_cell_OK == true)
				ValidCell1[cellCount1] = true;
			else
				ValidCell1[cellCount1] = false;
			cellCount1++;
		}
		//�ж��ĸ�����Ի���
		int faceCount1 = 0;
		boundary_face_v1.swap(std::vector<std::vector<V3f>>());
		normal_boundary_face_v1.swap(std::vector<V3f>());
		cutting_face_v1.swap(std::vector<std::vector<V3f>>());
		normal_cutting_face_v1.swap(std::vector<V3f>());
		boundary_face_VID1.swap(std::vector<std::vector<int>>());
		cutting_face_VID1.swap(std::vector<std::vector<int>>());
		for (std::vector<Face>::iterator f_it = mesh1->faces_.begin(); f_it != mesh1->faces_.end(); f_it++)
		{
			std::set<CellHandle, compare_OVM> f_lj_c = mesh1->neighbor_f[faceCount1];
			bool valid_face_OK = false;
			std::vector<bool> draw;
			int face_type;
			for (std::set<CellHandle, compare_OVM>::iterator f_lj_cv = f_lj_c.begin(); f_lj_cv != f_lj_c.end(); f_lj_cv++)
			{
				draw.push_back(ValidCell1[*f_lj_cv]);

			}
			//���߽�����и���
			int drawsize = draw.size();
			if (drawsize == 2)
			{
				if ((draw[0] == false) && (draw[1] == false))
				{
					valid_face_OK = false;
				}
				else if ((draw[0] == true) && (draw[1] == true))
					valid_face_OK = false;
				else
				{
					valid_face_OK = true;
					face_type = CUTTING;
				}
			}
			else
			{
				if (draw[0] == true)
				{
					valid_face_OK = true;
					face_type = BOUNDART;
				}
				else
					valid_face_OK = false;
			}
			if (valid_face_OK == true)
			{
				std::vector<VertexHandle> fv = (*f_it).vs_f;
				std::vector<V3f> one_face;
				std::vector<int> one_face_vid;
				for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
				{
					one_face.push_back(mesh1->vertices_[*fv_it]);
					one_face_vid.push_back(*fv_it);
				}
				if (face_type == BOUNDART)
				{
					boundary_face_v1.push_back(one_face);
					boundary_face_VID1.push_back(one_face_vid);
					normal_boundary_face_v1.push_back(mesh1->cal_norm(FaceHandle(faceCount1)));
				}
				else if (face_type == CUTTING)
				{
					cutting_face_v1.push_back(one_face);
					cutting_face_VID1.push_back(one_face_vid);
					normal_cutting_face_v1.push_back(mesh1->cal_norm(FaceHandle(faceCount1)));
				}
			}
			faceCount1++;
		}
	}


	////int t_ms = time.elapsed();
	////std::cout << "End setting vertex flag..... using time: " << t_ms <<std::endl;
	////time.start();

	//std::vector<Cell>::iterator valid_c_it = temp_VM.cells_.begin();
	//std::vector<VertexHandle>::iterator valid_cv_it = (*valid_c_it).vertex_.begin();
	//if (ValidCell.size() != temp_VM.cells_.size())
	//{
	//	ValidCell.resize(temp_VM.cells_.size());
	//}
	//int cellCount = 0;
	//for (valid_c_it; valid_c_it != temp_VM.cells_.end(); ++valid_c_it)//�жϿ��Ƿ�ɱ�����
	//{
	//	valid_cv_it = (*valid_c_it).vertex_.begin();
	//	bool valid_cell_OK = true;
	//	for (valid_cv_it; valid_cv_it != (*valid_c_it).vertex_.end(); ++valid_cv_it)
	//	{
	//		if (!vertex_flag[valid_cv_it->idx()])
	//		{
	//			valid_cell_OK = false;
	//			ValidCell[cellCount] = false;
	//			break;
	//		}
	//	}
	//	if (valid_cell_OK)
	//	{
	//		ValidCell[cellCount] = true;
	//	}
	//	++cellCount;
	//}

	//*for(valid_c_it = temp_VM.cells_begin();valid_c_it != temp_VM.cells_end(); )
	//{
	//if( !ValidCell[valid_c_it->idx()] )
	//{
	//temp_VM.delete_cell(*valid_c_it);
	//}
	//else
	//{
	//++valid_c_it;
	//}
	//}*/

	////t_ms = time.elapsed();
	////std::cout << "End deleting cell..... using time: " << t_ms <<std::endl;
	////time.start();

	//std::vector<Face>::iterator bf_it = temp_VM.faces_.begin();
	//std::vector<VertexHandle>::iterator hfv_it = (*bf_it).vs_f.begin();
	//boundary_face_v.clear();
	////mesh->boundary_face.clear();
	////mesh->normal_boundary_face_v.clear();
	//*BoundaryFaceOnShowMesh.clear(); InnerFaceOnShowMesh.clear();
	//BoundaryFaceOnShowMesh_VID.clear(); InnerFaceOnShowMesh_VID.clear();
	//mesh_.showing_face_id.clear();
	//NormalOfBoundaryFFaceOnShowMesh.clear(); NormalOfInnerFFaceOnShowMesh.clear();*/
	//std::vector<V3f> one_face; std::vector<int> one_face_vid;
	//V3f v1;
	//bool face_ok = true;
	//int TwoHalfFaceInValid = 0;
	//int faceCount = 0;
	//for (bf_it ; bf_it != temp_VM.faces_.end(); ++bf_it)
	//{
	//	Face hfHandle = *bf_it;
	//	std::set<CellHandle, compare_OVM> cell_ID = temp_VM.neighbor_f[FaceHandle(faceCount)];
	//	/*if(cell_ID == -1)
	//	{
	//	int fuxm = 1;
	//	}*/
	//	//int cell_ID_size = cell_ID.size();
	//*	if (cell_ID.size() == 0)
	//	{
	//		++TwoHalfFaceInValid;
	//		continue;
	//	}
	//	else
	//	{*/
	//	bool check = true;
	//		for (std::set<CellHandle, compare_OVM>::iterator it = cell_ID.begin(); 
	//			it != cell_ID.end();it++)
	//		{
	//			if (!ValidCell[(*it)])
	//				check = false;
	//		}
	//		if (!check) continue;
	//	//}
	//	
	//	/*if (cell_ID_size == 2)
	//	{
	//		if ((cell_ID[0] >= 0 && ValidCell[cell_ID[0]]) || (cell_ID[1] >= 0 && ValidCell[cell_ID[1]]))
	//		{
	//			continue;
	//		}
	//	}
	//	else
	//	{
	//		if ((cell_ID[0] >= 0 && ValidCell[cell_ID[0]]))
	//		{
	//			continue;
	//		}
	//	}*/

	//	//if (cell_ID.size() == 0)
	//	//{
	//	//	++TwoHalfFaceInValid;
	//	//	//continue;
	//	//}

	//	/*if (cell_ID < 0)
	//	{
	//	hfHandle = mesh_.opposite_halfface_handle(hfHandle);
	//	}*/
	//	hfv_it = (*bf_it).vs_f.begin();
	//	one_face.clear(); face_ok = true; one_face_vid.clear();
	//	for (hfv_it; hfv_it != (*bf_it).vs_f.end(); ++hfv_it)
	//	{
	//		if (!vertex_flag[(*hfv_it)])
	//		{
	//			face_ok = false;
	//			break;
	//		}
	//		v1 = temp_VM.vertices_[*hfv_it];
	//		one_face.push_back(v1); one_face_vid.push_back(hfv_it->idx());
	//	}
	//	if (face_ok)
	//	{
	//		//if (temp_VM.is_bdy(FaceHandle(faceCount)))//if the face is at the boundary
	//		//{
	//			boundary_face_v.push_back(one_face);
	//			//boundary_face.push_back(one_face_vid);
	//			//NormalOfBoundaryFFaceOnShowMesh.push_back(calculateNormalForQuad(one_face));
	//		//}
	//		//else
	//		//{
	//			//InnerFaceOnShowMesh.push_back(one_face);
	//			//InnerFaceOnShowMesh_VID.push_back(one_face_vid);
	//			//NormalOfInnerFFaceOnShowMesh.push_back(calculateNormalForQuad(one_face));
	//		//}
	//		//mesh_.showing_face_id.push_back(bf_it->idx());

	//	}
	//	++faceCount;
	//}

	//std::cout << "End constructing the Showing Mesh" <<std::endl;
	//std::cout << "The elapsed time is: " << time.elapsed() << " ms." <<std::endl;
	//constructShowMeshFaceCenter();
	//updateIndices();
	//setDrawMode(FLAT_LINES);
	updateGL();
}

void  QGLViewerWidget::construct_vertex_texture()
{
	vertex_texture.swap(std::vector<V3f>());
	int nv = mesh->vertices_.size();
	if (nv == 0) return;

	vertex_texture.resize(nv);
	V3f box_width = mesh->bbMax - mesh->bbMin;
	double width = box_width.norm() + 1e-14; double inv_width = 1.0 / width;
	int v_it_num = 0;
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		//int v_id = v_it->idx();
		V3f p = *v_it;
		vertex_texture[v_it_num] = (p - mesh->bbMin)*inv_width;
		v_it_num++;
	}

	PFNGLTEXIMAGE3DPROC glTexImage3D;
	//simple texture
	glEnable(GL_TEXTURE_3D);
	glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");

	int square_length = 256; int interval = 16;
	glBindTexture(GL_TEXTURE_3D, Verterx_Texture_ID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	unsigned char* RGBA = new unsigned char[4 * square_length*square_length* square_length];
	for (int i = 0; i < square_length; i++)
	{
		for (int j = 0; j < square_length; j++)
		{
			for (int k = 0; k < square_length; ++k)
			{
				int id = i * square_length*square_length + j * square_length + k;

				if ((i / interval + j / interval + k / interval) % 2 == 1)
				{
					RGBA[id * 4 + 0] = 200; RGBA[id * 4 + 1] = 200;
					RGBA[id * 4 + 2] = 200; RGBA[id * 4 + 3] = 255;
				}
				else
				{
					RGBA[id * 4 + 0] = 0; RGBA[id * 4 + 1] = 125;
					RGBA[id * 4 + 2] = 65; RGBA[id * 4 + 3] = 255;
				}
			}
		}
	}

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, square_length, square_length, square_length, 0, GL_RGBA, GL_UNSIGNED_BYTE, RGBA);

	glDisable(GL_TEXTURE_3D);
}

void QGLViewerWidget::write2AXLFile()
{
	FileHandler filenew = FileHandler();
	filenew.write2AXLFile(name, SolidData, bezier_color);
}

void QGLViewerWidget::pick_point(int x, int y, double p[3])
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);// get View Mat
	GLdouble winX = double(x);
	GLdouble winY = double(viewport[3] - y);
	GLfloat winZ = 0.0;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, (GLdouble)winZ, modelview_matrix_, projection_matrix_, viewport, &p[0], &p[1], &p[2]);
	std::cout << "[DEBUG] pp " << p[0] << " " << p[1] << " " << p[2] << std::endl;
}

void QGLViewerWidget::pick_point_get_depth(int x, int y, double p[3], double& d)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble winX = double(x);
	GLdouble winY = double(viewport[3] - y);
	GLfloat winZ = 0.0;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	d = (GLdouble)winZ;
	gluUnProject(winX, winY, (GLdouble)winZ, modelview_matrix_, projection_matrix_, viewport, &p[0], &p[1], &p[2]);
}

void QGLViewerWidget::pick_point_with_depth(int x, int y, double p[3], double d)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLdouble winX = double(x);
	GLdouble winY = double(viewport[3] - y);
	gluUnProject(winX, winY, d, modelview_matrix_, projection_matrix_, viewport, &p[0], &p[1], &p[2]);
}

void QGLViewerWidget::vtoline()
{
	for (std::vector<VertexHandle>::iterator pickpoint_v_it = pickpoint_v.begin(); pickpoint_v_it != pickpoint_v.end(); pickpoint_v_it++)
	{
		std::vector<VertexHandle> temp_edgev;
		VertexHandle v_id = *pickpoint_v_it;
		VertexHandle last_vid = *pickpoint_v_it;
		VertexHandle preadd_vid(-1);
		do
		{
			std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[last_vid];
			std::vector<double> temp1;
			//�ȼ������С�ı߳�
			for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
			{
				if (mesh->is_bdy(*v_e_it))
				{
					double e_length = mesh->cal_edge_length(*v_e_it);//������λ��Ч����
					temp1.push_back(e_length);
				}
			}
			std::sort(temp1.begin(), temp1.end());
			double standardlength = temp1[2];
			for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
			{
				if (mesh->is_bdy(*v_e_it))
				{
					double e_length = mesh->cal_edge_length(*v_e_it);//������λ��Ч����
					if (e_length >= standardlength)
					{
						if (mesh->edges_[*v_e_it].from_h == preadd_vid || mesh->edges_[*v_e_it].to_h == preadd_vid)
							continue;
						else
						{
							if (mesh->edges_[*v_e_it].from_h == last_vid)
							{
								temp_edgev.push_back(mesh->edges_[*v_e_it].to_h);
								preadd_vid = last_vid;
								last_vid = mesh->edges_[*v_e_it].to_h;
								break;
							}
							else if (mesh->edges_[*v_e_it].to_h == last_vid)
							{
								temp_edgev.push_back(mesh->edges_[*v_e_it].from_h);
								preadd_vid = last_vid;
								last_vid = mesh->edges_[*v_e_it].from_h;
								break;
							}
							else
							{
								qDebug() << "Find edge error";
								return;
							}
						}
					}
				}
			}
		} while (last_vid != v_id);
		pickpoint_vtoline.push_back(temp_edgev);
	}
}

void QGLViewerWidget::grab1()//����grab * ץȡ�����е����� * ���贰�ڿ���ΪWindowWidth���߶�ΪWindowHeight
{
	FILE* pDummyFile;
	FILE* pWritingFile;
	GLubyte* pPixelData;
	GLubyte BMP_Header[BMP_Header_Length];
	GLint i, j;
	GLint PixelDataLength;
	// �����������ݵ�ʵ�ʳ���
	i = WIDTH * 3; // �õ�ÿһ�е��������ݳ���
	while (i % 4 != 0) // �������ݣ�ֱ��i�ǵı���
		++i;// �������и�����㷨���������׷��ֱ�ۣ����ٶ�û��̫��Ҫ��
	PixelDataLength = i * HEIGHT;
	// �����ڴ�ʹ��ļ�
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);
	pDummyFile = fopen("pic.bmp", "rb");
	if (pDummyFile == 0)
		exit(0);
	pWritingFile = fopen("grab.bmp", "wb");
	if (pWritingFile == 0)
		exit(0);
	// ��ȡ����
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, WIDTH, HEIGHT, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
	// ��dummy.bmp���ļ�ͷ����Ϊ���ļ����ļ�ͷ
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	i = WIDTH;
	j = HEIGHT;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);
	// д���������� 
	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);
	// �ͷ��ڴ�͹ر��ļ�
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}