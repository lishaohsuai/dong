#include "QGLViewerWidget.h"
#include <glut.h>
#include <iomanip> 
#include <windows.h>
#include <stdlib.h>
#include "ComHeader.h"

void QGLViewerWidget::draw_scene(int drawmode)
{
	//if (mesh->boundary_face_v.size() == 0) { return; }

	//assert(drawmode < N_DRAW_MODES);
	setDefaultMaterial();
	switch (drawmode)
	{
	case WIRE_FRAME:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SOLID_FLAT:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_LIGHTING);
		break;
	case TISU:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflattisu();
		draw_mesh_solidflatTran1();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case FLAT_LINES:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (bshowAxis) drawAxis();
		if (bshowGround) drawHorizon();
		if (bshowMirror) drawXOZ();
		if (bshowTriMeshChanged) draw_tri_mesh_solidflat();
		if (bshowProjectLinesChanged) draw_project_lines();
		break;
	case SKEL:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		if(bshowSKEL) drawSkel();
		if(bshowAxis) drawAxis();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(bshowGround) drawHorizon();
		if(bshowMirror) drawXOZ();
		if (bshowTriMeshChanged) draw_tri_mesh_solidflat();
		if (bshowProjectLinesChanged) draw_project_lines();
		break;
	case BLENDSKEL:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatTran1();
		draw_beisurDATsingularface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline1();
		//drawframe();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		drawframeface();
		if (bshowSKEL) drawSkel();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		
		if(bshowAxis) drawAxis();
		if(bshowGround) drawHorizon();
		if(bshowMirror) drawXOZ();
		if (bshowTriMeshChanged) draw_tri_mesh_solidflat();
		if (bshowProjectLinesChanged) draw_project_lines();
		break;
	case TRISKELPRO:
		//glEnable(GL_POLYGON_OFFSET_FILL);
		//glPolygonOffset(1.5f, 2.0f);
		//glEnable(GL_LIGHTING);
		//glShadeModel(GL_FLAT);
		draw_tri_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SELECT_CELL:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawpickedcell();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		if(bshowSKEL) drawSkel();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(bshowAxis) drawAxis();
		if(bshowGround) drawHorizon();
		if(bshowMirror) drawXOZ();
		if (bshowTriMeshChanged) draw_tri_mesh_solidflat();
		if (bshowProjectLinesChanged) draw_project_lines();
		break;
	case SELECT_CELLS:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawpickedcell();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		if(bshowSKEL) drawSkel();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(bshowAxis) drawAxis();
		if(bshowGround) drawHorizon();
		if (bshowTriMeshChanged) draw_tri_mesh_solidflat();
		if (bshowProjectLinesChanged) draw_project_lines();
		break;
	case Skeleton:
		DrawSkel();
		if (bshowTriMeshChanged) draw_tri_mesh_solidflat();
		break;
	case TESTLINES:
		drawTestLines();
		break;
	case ALL_POINTS:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_points();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ALL_LINES:
		// ����ʹ��
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_lines3();
		//drawotherline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_points2();
		//drawpickpoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ALL_LINES1:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_lines3();
		drawotherline();
		drawfaceline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glDisable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawfaceface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		break;
	case ALL_POLYHEDRALS:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_lines();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		draw_mesh_all_polys();
		//glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case SINGULARITY:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_singularity();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case VOLUME_TEXTURE:
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_3D);
		draw_mesh_texture();
		glDisable(GL_TEXTURE_3D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ORIV:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_oriv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ADD_BB:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_bb();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ADD_ORIBB:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_oribb();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case OV_V:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_points();
		draw_oriv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	case SE_V:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_lines();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_sv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SX:
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		draw_mesh_solidflat1();
		//glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_lines1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_points1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
		break;
	case BEISU:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat_bei();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe_bei();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
		break;
	case BEISUDAT:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_beisurDATface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case BEISUDAT1:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_beisurDATface1();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case BEIV:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_points_bei();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case BEI_FRAME:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe_bei();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case DAT1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesDAT1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		draw_mesh_all_polysDAT();
		//glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case DAT2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesDAT2();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		draw_mesh_all_polysDAT();
		//glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case DAT3:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesDAT1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_oriv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		draw_mesh_all_polysDAT();
		//glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case DAT4:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesDAT2();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_oriv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		draw_mesh_all_polysDAT();
		//glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case DAT5:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesDAT1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_oriv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatDAT();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case DAT5_1:
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesDAT2();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_oriv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatDAT();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case Isoline:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatIsoline();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesIsoline();
		//draw_mesh_all_polys_linesDAT2();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_oriv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatIsoline_notdraw();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case Isoline1:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatIsoline();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//draw_mesh_all_polys_linesDAT2();
		//draw_beisurDATline1();
		draw_mesh_all_polys_linesIsoline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatIsoline_notdraw();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case Isoline2:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatIsoline();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatIsoline_notdraw();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case DATONLY:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatdatonly();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_polys_linesDAT2();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatIsoline_notdraw();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case DatPoint:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframeDatpoint();
		draw_mesh_all_pointsDatpoint1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glDisable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_small_face1();
		glDisable(GL_POLYGON_OFFSET_FILL);

		drawzimu1();
		/*glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_pointsDatpoint1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);*/

		//glEnable(GL_LIGHTING);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDepthMask(GL_FALSE);
		//glShadeModel(GL_SMOOTH);
		////draw_mesh_solidflatDatpoint();
		//glDisable(GL_LIGHTING);
		//glDisable(GL_BLEND);
		//glDepthMask(GL_TRUE);
		break;
	case DatPoint2:
		//glDisable(GL_LIGHTING);
		//glEnable(GL_LIGHTING);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		//draw_mesh_all_pointsDatpoint2();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframeDatpoint();
		draw_small_line();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glDisable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_small_face();
		glDisable(GL_POLYGON_OFFSET_FILL);

		drawzimu();
		//glEnable(GL_LIGHTING);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDepthMask(GL_FALSE);
		//glShadeModel(GL_SMOOTH);
		////draw_mesh_solidflatDatpoint();
		//glDisable(GL_LIGHTING);
		//glDisable(GL_BLEND);
		//glDepthMask(GL_TRUE);
		break;
	case Interior_Point:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframeDatpoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_pointsInterior();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatInterior();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case Edge_Point:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframeEdge();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_pointsEdge();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatEdge();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case Corner_Point:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframeDatpoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_pointsCorner();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatEdge();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case Error:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		draw_mesh_compare_new();

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_errorv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
		break;
	case ErrorXJL:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		draw_mesh_compare_newXJL();
		break;
	case ERRORV:
		glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		draw_mesh_errorv();
		break;
	case Error1:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatTran();
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		//draw_mesh_solidflatTran();
		draw_mesh_solidflatTran1();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case View_singular:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe_view_singular();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_mesh_all_points_view_singular();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatEdge();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case TWOPOINT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawtwonpoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawtwonpointlines();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SOLIDFLAT_AND_PICKPOINT:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawpickpoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case PICKPOINT_RESULT:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawlinepoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawpickpointtoline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SOLIDFLAT_AND_PICKEDGE:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawpickedge();
		//draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SOLIDFLAT_AND_PICKEDFACE:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawpickedface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SOLIDFLAT_AND_PICKEDCELL:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawpickedcell();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SHOWTWOMESH:
		/*glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatTran1();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);*/

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatTran1();
		draw_beisurDATface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SHOWTWOMESH1:
		/*glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflatTran1();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);*/

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatTran1();
		draw_beisurDATsingularface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case SINGULARFACE:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_beisurDATsingularface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ADDFRAME:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatTran1();
		draw_beisurDATsingularface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline1();
		//drawframe();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		drawframeface();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case ADDFRAME1:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_beisurDATface();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_beisurDATline1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		drawframeface();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case FRAMETISU:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflattisu();
		draw_mesh_solidflatTran1();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_wireframe1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		drawframeface();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case SOLIDFLAT_FRAME:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		drawframeface();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case SPHERE:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflat();
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_mesh_solidflat_sphere();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_mesh_all_lines2();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case ERRORFRAME:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		draw_mesh_compare_new();

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		drawframeface();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case QUAD:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_quad();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_quadlines();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case QUADPICKV:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_quad();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		draw_quadlines();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_quadpickv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case QUADV:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		draw_quadv();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case TWOQUAD:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		//draw_quad();
		draw_quad1();
		draw_beisurDATsingularface();
		//draw_beisurDATface1();
		//draw_beisurDATsingularface1();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//draw_quadlines();
		draw_beisurDATline();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case TWOQUADFRAME:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		//draw_quad();
		draw_quad1();
		draw_beisurDATsingularface();
		//draw_beisurDATsingularface1();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//draw_quadlines1();
		draw_beisurDATline1();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		//drawframequad();
		draw_beisurDATsingularfaceframe();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case CURVE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawcurvepoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawcurve();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case BSVECTOR:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawbsvface();
		draw_beisurDATsingularface1();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_beisurDATsingularfaceframe();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case VH:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_solidflatVH();
		glDisable(GL_LIGHTING);
		break;
	case VFRAME:
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.5f, 2.0f);
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		drawbsvface();
		draw_beisurDATsingularface1();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_LIGHTING);

		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glShadeModel(GL_SMOOTH);
		draw_beisurDATsingularfaceframeV();
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		break;
	case TEST:
		/*glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		draw_mesh_test();*/
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		draw_mesh_test();
		glDisable(GL_LIGHTING);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
		drawtwonpoint();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
		break;
	default:
		break;
	}
}

void QGLViewerWidget::draw_mesh_wireframe() const
{
	glLineWidth(1.0);
	glColor4f(0.1, 0.1, 0.5, 1.0);
	glBegin(GL_QUADS);
	//���߽���
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}

	//���и���
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_wireframe1() const
{
	glLineWidth(0.005);
	glColor4f(0.1, 0.1, 0.5, 1.0);
	glBegin(GL_QUADS);
	//���߽���
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}

	//���и���
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_wireframe_bei() const
{
	glLineWidth(1.0);
	glColor4f(0.1, 0.1, 0.5, 1.0);
	glBegin(GL_QUADS);
	for (int i = 0; i < beisurv.size(); ++i)
	{
		for (int j = 0; j < beisurv[i].size(); ++j)
		{
			glVertex3d(beisurv[i][j].x, beisurv[i][j].y,
				beisurv[i][j].z);
		}
	}

	glEnd();
}


void QGLViewerWidget::drawTestLines() const {
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	//glDisable(GL_LIGHTING);
	glColor4f(0.5, 0.50, 0.50, 1.0);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//glEnable(GL_LIGHTING);
	//glBegin(GL_TRIANGLES);

	//glShadeModel(GL_FLAT);
	glColor3d(0.6, 0.0, 0.0);
	//glBegin(GL_LINE);
	int indexColor[][3] = { {0,0,1},{0,1,0}, {1, 0, 0}, {0,1,1}, {1,0,1}, {1,1,0}, {0,0,0} };
	for (int i = 0; i < skels.size(); ++i) {
		glLineWidth(5);
		glColor3d(indexColor[i % 7][0], indexColor[i % 7][1], indexColor[i % 7][2]);
		glBegin(GL_LINES);
		for (int j = 0; j < skels[i].size()-1; ++j) {
			glVertex3d(skels[i][j].x, skels[i][j].y, skels[i][j].z);
			glVertex3d(skels[i][j+1].x, skels[i][j+1].y, skels[i][j+1].z);
		}
		glEnd();
	}
	glColor3d(0.0, 0.0, 0.0); // black
	for (int i = 0; i < skels.size(); ++i) {
		glPointSize(10);
		glBegin(GL_POINTS);
		for (int j = 0; j < skels[i].size() - 1; ++j) {
			glVertex3d(skels[i][j].x, skels[i][j].y, skels[i][j].z);
			glVertex3d(skels[i][j + 1].x, skels[i][j + 1].y, skels[i][j + 1].z);
		}
		glEnd();
	}
	/*Test map skel surface*/
	/*
	glColor3f(1, 0, 0);
	for (int i = 0; i < lines.size(); ++i) {
		// glNormal3d(proj->triangleVector[i].x, proj->triangleVector[i].y, proj->triangleVector[i].z);
		glLineWidth(5);
		glBegin(GL_LINES);
		for (int j = 0; j < lines[i].size() - 1; ++j) {
			glVertex3d(lines[i][j].x, lines[i][j].y, lines[i][j].z);
			glVertex3d(lines[i][j + 1].x, lines[i][j + 1].y, lines[i][j + 1].z);
		}
		glEnd();
	}
	*/
}

void QGLViewerWidget::draw_project_lines() const {
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	//glDisable(GL_LIGHTING);
	glColor4f(0.5, 0.50, 0.50, 1.0);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//glEnable(GL_LIGHTING);
	//glBegin(GL_TRIANGLES);
	
	//glShadeModel(GL_FLAT);
	glColor3d(0.6, 0.0, 0.0);
	//glBegin(GL_LINE);
	for (int i = 0; i < proj->projectLines.size(); ++i) {
		// glNormal3d(proj->triangleVector[i].x, proj->triangleVector[i].y, proj->triangleVector[i].z);
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex3d(proj->projectLines[i].first.x, proj->projectLines[i].first.y, proj->projectLines[i].first.z);
		glVertex3d(proj->projectLines[i].second.x, proj->projectLines[i].second.y, proj->projectLines[i].second.z);
		glEnd();
	}

	glColor3d(0.0, 0.6, 0.0);
	//glBegin(GL_LINE);
	for (int i = 0; i < proj->projectLines2.size(); ++i) {
		// glNormal3d(proj->triangleVector[i].x, proj->triangleVector[i].y, proj->triangleVector[i].z);
		glLineWidth(5);
		glBegin(GL_LINES);
		glVertex3d(proj->projectLines2[i].first.x, proj->projectLines2[i].first.y, proj->projectLines2[i].first.z);
		glVertex3d(proj->projectLines2[i].second.x, proj->projectLines2[i].second.y, proj->projectLines2[i].second.z);
		glEnd();
	}
	//glLoadIdentity();
	//glPointSize(10);
	glColor3f(1, 0, 0);
	for (int i = 0; i < proj->projectLines.size(); ++i) {
		// glNormal3d(proj->triangleVector[i].x, proj->triangleVector[i].y, proj->triangleVector[i].z);
		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex3d(proj->projectLines[i].first.x, proj->projectLines[i].first.y, proj->projectLines[i].first.z);
		glVertex3d(proj->projectLines[i].second.x, proj->projectLines[i].second.y, proj->projectLines[i].second.z);
		glEnd();
		
	}
	//glVertex3d(0, 0, 0);
	//glVertex3d(0, 1, 0);
	//glVertex3d(1, 0, 0);
	//glEnd();

}

void QGLViewerWidget::draw_tri_mesh_solidflat() const
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	//glDisable(GL_LIGHTING);
	glColor4f(0.5,0.50,0.50,1.0);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//glEnable(GL_LIGHTING);
	//glBegin(GL_TRIANGLES);
	
	glShadeModel(GL_FLAT);
	glColor4d(0.2, 0.2, 0.2, 0.2);
	//glBegin(GL_LINE);
	for (int i = 0; i < proj->tv.size(); ++i) {
		// glNormal3d(proj->triangleVector[i].x, proj->triangleVector[i].y, proj->triangleVector[i].z);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex3d(proj->tv[i].v[0].x, proj->tv[i].v[0].y, proj->tv[i].v[0].z);
		glVertex3d(proj->tv[i].v[1].x, proj->tv[i].v[1].y, proj->tv[i].v[1].z);
		glVertex3d(proj->tv[i].v[0].x, proj->tv[i].v[0].y, proj->tv[i].v[0].z);
		glVertex3d(proj->tv[i].v[2].x, proj->tv[i].v[2].y, proj->tv[i].v[2].z);
		glVertex3d(proj->tv[i].v[1].x, proj->tv[i].v[1].y, proj->tv[i].v[1].z);
		glVertex3d(proj->tv[i].v[2].x, proj->tv[i].v[2].y, proj->tv[i].v[2].z);
		glEnd();
	}
	//glVertex3d(0, 0, 0);
	//glVertex3d(0, 1, 0);
	//glVertex3d(1, 0, 0);
	//glEnd();

	
}


void QGLViewerWidget::draw_mesh_solidflat() const
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	//glDisable(GL_LIGHTING);
	//glColor4f(0.5,0.50,0.50,1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	//���и���
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
	//glDisable(GL_LIGHTING);
	//glColor4f(1.0, 0.50, 0.50, 1.0);
	glBegin(GL_QUADS);
	//glColor4f(1.0, 0.50, 0.50, 1.0);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		glNormal3d(-1 * normal_cutting_face_v[i].x, -1 * normal_cutting_face_v[i].y, -1 * normal_cutting_face_v[i].z);
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);*/
	//glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_mesh_solidflatVH()
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);
	//draw boundary face
	glBegin(GL_QUADS);
	for (int k = 0; k < vH.size(); ++k)
	{
		for (int i = 0; i < vH[k].boundary_face_v.size(); ++i)
		{
			glNormal3d(vH[k].normal_boundary_face_v[i].x, vH[k].normal_boundary_face_v[i].y, vH[k].normal_boundary_face_v[i].z);
			for (int j = 0; j < vH[k].boundary_face_v[i].size(); ++j)
			{
				glVertex3d(vH[k].boundary_face_v[i][j].x, vH[k].boundary_face_v[i][j].y,
					vH[k].boundary_face_v[i][j].z);
			}
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_mesh_solidflattisu() const
{
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };
	//glDisable(GL_LIGHTING);
	//glColor4f(0.5,0.50,0.50,1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	//���и���
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
	//glDisable(GL_LIGHTING);
	//glColor4f(1.0, 0.50, 0.50, 1.0);
	glBegin(GL_QUADS);
	//glColor4f(1.0, 0.50, 0.50, 1.0);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);*/
	//glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_mesh_solidflat_bei() const
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };
	//glDisable(GL_LIGHTING);
	//glColor4f(0.5,0.50,0.50,1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < beisurv.size(); ++i)
	{
		glNormal3d(bei_nor_sur[i].x, bei_nor_sur[i].y, bei_nor_sur[i].z);
		for (int j = 0; j < beisurv[i].size(); ++j)
		{
			glVertex3d(beisurv[i][j].x, beisurv[i][j].y,
				beisurv[i][j].z);
		}
	}
	glEnd();


}

void QGLViewerWidget::draw_mesh_all_lines() const
{
	//glLineWidth(0.5);
	//glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	//glDrawBuffer(GL_NONE);
	//glDepthRange(0.01, 1.0);

	std::vector<Edge>::iterator e_it = mesh->edges_.begin();
	V3f v1;
	V3f v2;
	int i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		/*if(std::find(pickedge_e.begin(), pickedge_e.end(),EdgeHandle(i)) != pickedge_e.end())
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		else
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);*/
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			glLineWidth(1);
			//glColor4f(0.1f, 0.1f, 0.5f, 1.0f);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
			i++;
		}
		else
		{
			if (mesh->is_singular(EdgeHandle(i)))
			{
				glLineWidth(2);
				//glColor4f(0.5f, 0.0f, 0.0f, 1.0f);
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
			{
				glLineWidth(1);
				//glColor4f(0.1f, 0.1f, 0.5f, 1.0f);
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
		}


		/*else
		{
			glLineStipple(2, 0x5555);
			glEnable(GL_LINE_STIPPLE);
			v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			i++;
		}*/
		/*v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
		glBegin(GL_LINES);
		glVertex3d(v1.x,v1.y,v1.z); glVertex3d(v2.x, v2.y, v2.z);
		glEnd();*/
	}

	/*glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	e_it = mesh->edges_.begin();
	i = 0;
	for (e_it; e_it != mesh->edges_.end();++e_it)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			glColor4f(0.1f, 0.1f, 0.5f, 1.0f);
			v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
			i++;
		}
		else
		{
			if (mesh->is_singular(EdgeHandle(i)))
			{
				glColor4f(0.5f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
			{
				glColor4f(0.1f, 0.1f, 0.5f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
		}
	}

	glDepthFunc(GL_LESS);*/
}

void QGLViewerWidget::draw_mesh_all_lines2() const
{
	glLineWidth(5);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<Edge>::iterator e_it = mesh1->edges_.begin();
	V3f v1;
	V3f v2;
	int i = 0;
	for (e_it; e_it != mesh1->edges_.end(); ++e_it)
	{
		v1 = mesh1->vertices_[(*e_it).from_h]; v2 = mesh1->vertices_[(*e_it).to_h];
		glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		glEnd();
	}
}

void QGLViewerWidget::draw_mesh_all_lines3() const
{
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	std::vector<Edge>::iterator e_it = mesh->edges_.begin();
	V3f v1;
	V3f v2;
	int i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		if (!mesh->is_e_in_c(EdgeHandle(i), CellHandle(28)))
		{
			glLineWidth(1);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0x0F0F);
			v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
			i++;
			glDisable(GL_LINE_STIPPLE);
		}
		else
		{
			glLineWidth(2);
			v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
			i++;
		}
	}
}

void QGLViewerWidget::draw_mesh_all_polys() const
{
	glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
	V3f v;
	V3f temp_cell_center;
	int i = 0;
	for (std::vector<Cell>::iterator c_it = mesh->cells_.begin(); c_it != mesh->cells_.end(); ++c_it, ++i)
	{
		temp_cell_center = cell_center[i];
		std::vector<FaceHandle> face = (*c_it).faces_;
		for (std::vector<FaceHandle>::iterator f_it = face.begin(); f_it != face.end(); f_it++)
		{
			std::vector<VertexHandle> hfv = mesh->faces_[*f_it].vs_f;
			glBegin(GL_POLYGON);
			for (std::vector<VertexHandle>::iterator hfv_it = hfv.begin(); hfv_it != hfv.end(); ++hfv_it)
			{
				v = mesh->vertices_[*hfv_it];
				v.x -= temp_cell_center.x; v.y -= temp_cell_center.y;; v.z -= temp_cell_center.z;//v -= temp_cell_center;
				v.x *= 0.8; v.y *= 0.8; v.z *= 0.8;//v *= 0.8;
				v.x += temp_cell_center.x; v.y += temp_cell_center.y;; v.z += temp_cell_center.z;//v += temp_cell_center;
				glVertex3d(v.x, v.y, v.z);
			}
			glEnd();
		}
	}
}

void QGLViewerWidget::draw_mesh_all_polys_lines() const
{
	glLineWidth(2.0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	V3f v;
	V3f temp_cell_center;
	int i = 0;
	for (std::vector<Cell>::iterator c_it = mesh->cells_.begin(); c_it != mesh->cells_.end(); ++c_it, ++i)
	{
		temp_cell_center = cell_center[i];
		std::vector<FaceHandle> face = (*c_it).faces_;
		for (std::vector<FaceHandle>::iterator f_it = face.begin(); f_it != face.end(); f_it++)
		{
			std::vector<VertexHandle> hfv = mesh->faces_[*f_it].vs_f;
			glBegin(GL_LINE_STRIP);
			//std::vector<EdgeHandle> hfe = mesh->faces_[*f_it].edges_;
			//for (std::vector<EdgeHandle>::iterator hfe_it = hfe.begin();hfe_it != hfe.end();hfe_it != hfe.end())
			//{
			//	if (mesh->is_bdy(*hfe_it))
			//	{
			//		glBegin(GL_LINE_STRIP);
			//		v = mesh->vertices_[mesh->edges_[*hfe_it].from_h];
			//		v.x -= temp_cell_center.x;v.y -= temp_cell_center.y;;v.z -= temp_cell_center.z;//v -= temp_cell_center;
			//		v.x *= 0.8;v.y *= 0.8;v.z *= 0.8;//v *= 0.8;
			//		v.x += temp_cell_center.x;v.y += temp_cell_center.y;;v.z += temp_cell_center.z;//v += temp_cell_center;
			//		glVertex3d(v.x, v.y, v.z);
			//		v = mesh->vertices_[mesh->edges_[*hfe_it].to_h];
			//		v.x -= temp_cell_center.x;v.y -= temp_cell_center.y;;v.z -= temp_cell_center.z;//v -= temp_cell_center;
			//		v.x *= 0.8;v.y *= 0.8;v.z *= 0.8;//v *= 0.8;
			//		v.x += temp_cell_center.x;v.y += temp_cell_center.y;;v.z += temp_cell_center.z;//v += temp_cell_center;
			//		glVertex3d(v.x, v.y, v.z);
			//		glEnd();
			//	}
			//	else
			//	{
			//		glEnable(GL_LINE_STIPPLE);
			//		glLineStipple(2.0f, 0x0F0F);
			//		glBegin(GL_LINE_STRIP);
			//		v.x -= temp_cell_center.x;v.y -= temp_cell_center.y;;v.z -= temp_cell_center.z;//v -= temp_cell_center;
			//		v.x *= 0.8;v.y *= 0.8;v.z *= 0.8;//v *= 0.8;
			//		v.x += temp_cell_center.x;v.y += temp_cell_center.y;;v.z += temp_cell_center.z;//v += temp_cell_center;
			//		glVertex3d(v.x, v.y, v.z);
			//		v = mesh->vertices_[mesh->edges_[*hfe_it].to_h];
			//		v.x -= temp_cell_center.x;v.y -= temp_cell_center.y;;v.z -= temp_cell_center.z;//v -= temp_cell_center;
			//		v.x *= 0.8;v.y *= 0.8;v.z *= 0.8;//v *= 0.8;
			//		v.x += temp_cell_center.x;v.y += temp_cell_center.y;;v.z += temp_cell_center.z;//v += temp_cell_center;
			//		glVertex3d(v.x, v.y, v.z);
			//		glDisable(GL_LINE_STIPPLE);
			//		glEnd();
			//	}
			//}
			for (std::vector<VertexHandle>::iterator hfv_it = hfv.begin(); hfv_it != hfv.end(); ++hfv_it)
			{
				v = mesh->vertices_[*hfv_it];
				v.x -= temp_cell_center.x; v.y -= temp_cell_center.y;; v.z -= temp_cell_center.z;//v -= temp_cell_center;
				v.x *= 0.8; v.y *= 0.8; v.z *= 0.8;//v *= 0.8;
				v.x += temp_cell_center.x; v.y += temp_cell_center.y;; v.z += temp_cell_center.z;//v += temp_cell_center;
				glVertex3d(v.x, v.y, v.z);
			}
			glEnd();
		}
	}
}

void QGLViewerWidget::draw_mesh_all_points()
{
	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		if ((*v_it).is_valid == true)
			glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_all_points2()
{
	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	int i = 0;
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		if ((*v_it).is_valid == true)
		{
			if (mesh->is_v_in_c(VertexHandle(i), CellHandle(10)) || mesh->is_v_in_c(VertexHandle(i), CellHandle(11))
				|| mesh->is_v_in_c(VertexHandle(i), CellHandle(12)) || mesh->is_v_in_c(VertexHandle(i), CellHandle(13))
				|| mesh->is_v_in_c(VertexHandle(i), CellHandle(14)) || mesh->is_v_in_c(VertexHandle(i), CellHandle(25))
				|| mesh->is_v_in_c(VertexHandle(i), CellHandle(26)) || mesh->is_v_in_c(VertexHandle(i), CellHandle(27))
				|| mesh->is_v_in_c(VertexHandle(i), CellHandle(28)) || mesh->is_v_in_c(VertexHandle(i), CellHandle(29))
				|| mesh->is_v_in_c(VertexHandle(i), CellHandle(23)) || mesh->is_v_in_c(VertexHandle(i), CellHandle(22))
				|| mesh->is_v_in_c(VertexHandle(i), CellHandle(8)) || mesh->is_v_in_c(VertexHandle(i), CellHandle(7)))
			{
				glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
			}
		}
		++i;
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_all_points_bei()
{
	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < beisurv.size(); ++i)
	{
		for (int j = 0; j < beisurv[i].size(); ++j)
		{
			glVertex3d(beisurv[i][j].x, beisurv[i][j].y,
				beisurv[i][j].z);
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_texture()
{
	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < boundary_face_v.size(); ++i)
	{
		for (unsigned int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glTexCoord3d(vertex_texture[boundary_face_VID[i][j]].x, vertex_texture[boundary_face_VID[i][j]].y
				, vertex_texture[boundary_face_VID[i][j]].z);
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < cutting_face_v.size(); ++i)
	{
		for (unsigned int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glTexCoord3d(vertex_texture[cutting_face_VID[i][j]].x, vertex_texture[cutting_face_VID[i][j]].y
				, vertex_texture[cutting_face_VID[i][j]].z);
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_singularity()
{

}

void QGLViewerWidget::draw_oriv()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
	glPointSize(10);
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (std::vector<V3f>::iterator v_it = oriv.begin(); v_it != oriv.end(); ++v_it)
	{
		glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
	}
	glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void QGLViewerWidget::draw_sv()
{
	glPointSize(10);
	glColor3f(0.5, 0.0, 0.0);
	glBegin(GL_POINTS);
	int j = 0;
	for (std::vector<V3f>::iterator v_it = oriv.begin(); v_it != oriv.end(); ++v_it)
	{
		if (!mesh->is_bdy(VertexHandle(j)))
		{
			if (mesh->is_singular(VertexHandle(j)))
			{
				glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
				j++;
			}
			else
				j++;

		}
		else
			j++;

	}
	glEnd();
}

void  QGLViewerWidget::draw_oribb()
{
	glLineWidth(1.0);
	glColor4f(0.1, 0.1, 0.5, 1.0);
	glBegin(GL_QUADS);
	V3f v0(oribbMin.x, oribbMin.y, oribbMax.z);
	V3f v1(oribbMax.x, oribbMin.y, oribbMax.z);
	V3f v2(oribbMax.x, oribbMax.y, oribbMax.z);
	V3f v3(oribbMin.x, oribbMax.y, oribbMax.z);
	V3f v4(oribbMin.x, oribbMin.y, oribbMin.z);
	V3f v5(oribbMax.x, oribbMin.y, oribbMin.z);
	V3f v6(oribbMax.x, oribbMax.y, oribbMin.z);
	V3f v7(oribbMin.x, oribbMax.y, oribbMin.z);
	glBegin(GL_QUADS);
	//��һ����
	glVertex3d(v0.x, v0.y, v0.z);
	glVertex3d(v1.x, v1.y, v1.z);
	glVertex3d(v2.x, v2.y, v2.z);
	glVertex3d(v3.x, v3.y, v3.z);
	//�ڶ�����
	glVertex3d(v0.x, v0.y, v0.z);
	glVertex3d(v1.x, v1.y, v1.z);
	glVertex3d(v5.x, v5.y, v5.z);
	glVertex3d(v4.x, v4.y, v4.z);
	//��������
	glVertex3d(v0.x, v0.y, v0.z);
	glVertex3d(v3.x, v3.y, v3.z);
	glVertex3d(v7.x, v7.y, v7.z);
	glVertex3d(v4.x, v4.y, v4.z);
	//���ĸ���
	glVertex3d(v4.x, v4.y, v4.z);
	glVertex3d(v7.x, v7.y, v7.z);
	glVertex3d(v6.x, v6.y, v6.z);
	glVertex3d(v5.x, v5.y, v5.z);
	//�������?
	glVertex3d(v7.x, v7.y, v7.z);
	glVertex3d(v3.x, v3.y, v3.z);
	glVertex3d(v2.x, v2.y, v2.z);
	glVertex3d(v6.x, v6.y, v6.z);
	//��������
	glVertex3d(v1.x, v1.y, v1.z);
	glVertex3d(v2.x, v2.y, v2.z);
	glVertex3d(v6.x, v6.y, v6.z);
	glVertex3d(v5.x, v5.y, v5.z);
	glEnd();
}

void  QGLViewerWidget::draw_bb()
{
	glLineWidth(1.0);
	glColor4f(0.1, 0.1, 0.5, 1.0);
	glBegin(GL_QUADS);
	V3f v0(bbMin.x, bbMin.y, bbMax.z);
	V3f v1(bbMax.x, bbMin.y, bbMax.z);
	V3f v2(bbMax.x, bbMax.y, bbMax.z);
	V3f v3(bbMin.x, bbMax.y, bbMax.z);
	V3f v4(bbMin.x, bbMin.y, bbMin.z);
	V3f v5(bbMax.x, bbMin.y, bbMin.z);
	V3f v6(bbMax.x, bbMax.y, bbMin.z);
	V3f v7(bbMin.x, bbMax.y, bbMin.z);
	glBegin(GL_QUADS);
	//��һ����
	glVertex3d(v0.x, v0.y, v0.z);
	glVertex3d(v1.x, v1.y, v1.z);
	glVertex3d(v2.x, v2.y, v2.z);
	glVertex3d(v3.x, v3.y, v3.z);
	//�ڶ�����
	glVertex3d(v0.x, v0.y, v0.z);
	glVertex3d(v1.x, v1.y, v1.z);
	glVertex3d(v5.x, v5.y, v5.z);
	glVertex3d(v4.x, v4.y, v4.z);
	//��������
	glVertex3d(v0.x, v0.y, v0.z);
	glVertex3d(v3.x, v3.y, v3.z);
	glVertex3d(v7.x, v7.y, v7.z);
	glVertex3d(v4.x, v4.y, v4.z);
	//���ĸ���
	glVertex3d(v4.x, v4.y, v4.z);
	glVertex3d(v7.x, v7.y, v7.z);
	glVertex3d(v6.x, v6.y, v6.z);
	glVertex3d(v5.x, v5.y, v5.z);
	//�������?
	glVertex3d(v7.x, v7.y, v7.z);
	glVertex3d(v3.x, v3.y, v3.z);
	glVertex3d(v2.x, v2.y, v2.z);
	glVertex3d(v6.x, v6.y, v6.z);
	//��������
	glVertex3d(v1.x, v1.y, v1.z);
	glVertex3d(v2.x, v2.y, v2.z);
	glVertex3d(v6.x, v6.y, v6.z);
	glVertex3d(v5.x, v5.y, v5.z);
	glEnd();
}

void QGLViewerWidget::draw_mesh_all_lines1() const
{
	glLineWidth(1.0);
	//glColor4f(0.1f, 0.1f, 0.5f, 1.0f);

	glDrawBuffer(GL_NONE);
	glDepthRange(0.01, 1.0);

	std::vector<Edge>::iterator e_it = mesh->edges_.begin();
	V3f v1;
	V3f v2;
	int i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			if (mesh->is_singular(EdgeHandle(i)))
			{
				glColor4f(0.5f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
				i++;

		}
		else
		{
			if (mesh->is_singular(EdgeHandle(i)))
			{
				glColor4f(0.5f, 0.5f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
				i++;

		}
	}

	glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	e_it = mesh->edges_.begin();
	i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			if (mesh->is_singular(EdgeHandle(i)))
			{
				glColor4f(0.5f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
				i++;

		}
		else
		{
			if (mesh->is_singular(EdgeHandle(i)))
			{
				glColor4f(0.5f, 0.5f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
				i++;

		}
	}

	glDepthFunc(GL_LESS);
}

void QGLViewerWidget::draw_mesh_all_points1()
{
	glPointSize(5);
	//glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	int i = 0;
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		if ((*v_it).is_valid == true)
		{
			if (mesh->is_bdy(VertexHandle(i)))
			{
				if (mesh->is_singular(VertexHandle(i)))
				{
					glColor3f(0.0, 0.0, 0.0);
					glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
					i++;
				}
				else
				{
					/*glColor3f(0.0, 0.0, 0.0);
					glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);*/
					i++;
				}
			}
			else
			{
				if (mesh->is_singular(VertexHandle(i)))
				{
					glColor3f(0.0, 0.0, 0.0);
					glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
					i++;
				}
				else
				{
					/*glColor3f(0.0, 0.0, 0.0);
					glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);*/
					i++;
				}
			}
		}
		else
			i++;

	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_solidflat1() const
{
	/*GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };*/
	glDisable(GL_LIGHTING);
	glColor4f(0.2, 0.5, 0.50, 0.1);
	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);*/
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	//���и���
	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);*/
	glDisable(GL_LIGHTING);
	glColor4f(0.2, 0.5, 0.50, 0.1);
	glBegin(GL_QUADS);
	//glColor4f(1.0, 0.50, 0.50, 1.0);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	//glEnable(GL_LIGHTING);
	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);*/
	//glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_mesh_all_polysDAT()
{
	std::vector<GLfloat> color1 = { 1.0, 0.0, 0.0 };
	std::vector<GLfloat> color2 = { 0.0, 1.0, 0.0 };
	std::vector<GLfloat> color3 = { 0.0, 0.0, 1.0 };
	std::vector<GLfloat> color4 = { 1.0, 1.0, 0.0 };
	std::vector<std::vector<GLfloat>> color = { color1 ,color2 ,color3 ,color4 };
	int i = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin(); dat_it != vmesh.end(); dat_it++)
	{
		int j = datdrawnum[i] % 4;
		glDisable(GL_LIGHTING);
		glColor4f(color[j][0], color[j][1], color[j][2], 0.1);
		glBegin(GL_QUADS);
		std::vector<Face> datface = dat_it->faces_;
		int t = 0;
		for (std::vector<Face>::iterator datface_it = datface.begin(); datface_it != datface.end(); datface_it++)
		{
			if (dat_it->is_bdy(FaceHandle(t)))
			{
				//V3f norm = dat_it->cal_norm(FaceHandle(t));
				std::vector<VertexHandle> datfv = datface_it->vs_f;
				//glNormal3d(norm.x, norm.y, norm.z);
				for (std::vector<VertexHandle>::iterator datfv_it = datfv.begin(); datfv_it != datfv.end(); datfv_it++)
				{
					glVertex3d(dat_it->vertices_[*datfv_it].x, dat_it->vertices_[*datfv_it].y,
						dat_it->vertices_[*datfv_it].z);
				}
			}
			t++;
		}
		glEnd();
		i++;

	}
	//���и����othermesh
	int oi = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = othermesh.begin(); dat_it != othermesh.end(); dat_it++)
	{
		int oid = othermeshID[oi];
		glDisable(GL_LIGHTING);
		glColor4f(0.5, 0.5, 0.5, 0.1);
		glBegin(GL_QUADS);
		std::vector<Face> datface = dat_it->faces_;
		int t = 0;
		for (std::vector<Face>::iterator datface_it = datface.begin(); datface_it != datface.end(); datface_it++)
		{
			if (dat_it->is_bdy(FaceHandle(t)))
			{
				if (datisbdy(oid, FaceHandle(t)))
				{
					std::vector<VertexHandle> datfv = datface_it->vs_f;
					for (std::vector<VertexHandle>::iterator datfv_it = datfv.begin(); datfv_it != datfv.end(); datfv_it++)
					{
						glVertex3d(dat_it->vertices_[*datfv_it].x, dat_it->vertices_[*datfv_it].y,
							dat_it->vertices_[*datfv_it].z);
					}
				}

			}
			t++;
		}
		oi++;
		glEnd();

	}
}

void QGLViewerWidget::draw_mesh_all_polys_linesDAT1()
{
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin(); dat_it != vmesh.end(); dat_it++)
	{
		glLineWidth(0.5);
		//glColor4f(0.1f, 0.1f, 0.5f, 1.0f);

		glDrawBuffer(GL_NONE);
		glDepthRange(0.01, 1.0);

		std::vector<Edge>::iterator e_it = dat_it->edges_.begin();
		V3f v1;
		V3f v2;
		int i = 0;
		for (e_it; e_it != dat_it->edges_.end(); ++e_it)
		{
			if (dat_it->is_bdy(EdgeHandle(i)))
			{
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				v1 = dat_it->vertices_[(*e_it).from_h]; v2 = dat_it->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
			}
			i++;
		}

		glDrawBuffer(GL_BACK);
		glDepthRange(0.01, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthFunc(GL_LEQUAL);

		e_it = dat_it->edges_.begin();
		i = 0;
		for (e_it; e_it != dat_it->edges_.end(); ++e_it)
		{
			if (dat_it->is_bdy(EdgeHandle(i)))
			{
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				v1 = dat_it->vertices_[(*e_it).from_h]; v2 = dat_it->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
			}
			i++;
		}

		glDepthFunc(GL_LESS);
	}
}

void QGLViewerWidget::draw_mesh_all_polys_linesDAT2()
{
	int oi = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin(); dat_it != vmesh.end(); dat_it++)
	{
		int oid = datdrawnum[oi];
		glLineWidth(0.0001);
		//glColor4f(0.1f, 0.1f, 0.5f, 1.0f);

		glDrawBuffer(GL_NONE);
		glDepthRange(0.01, 1.0);

		std::vector<Edge>::iterator e_it = dat_it->edges_.begin();
		V3f v1;
		V3f v2;
		int i = 0;
		for (e_it; e_it != dat_it->edges_.end(); ++e_it)
		{
			//if (dat_it->is_bdy(EdgeHandle(i)))
			if (datisbdy(oid, EdgeHandle(i)))
			{
				glColor4f(0.0, 0.0, 0.0, 1.0);
				v1 = dat_it->vertices_[(*e_it).from_h]; v2 = dat_it->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
			}
			i++;
		}




		glDrawBuffer(GL_BACK);
		glDepthRange(0.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthFunc(GL_LEQUAL);

		e_it = dat_it->edges_.begin();
		i = 0;
		for (e_it; e_it != dat_it->edges_.end(); ++e_it)
		{
			//if (dat_it->is_bdy(EdgeHandle(i)))
			if (datisbdy(oid, EdgeHandle(i)))
			{
				glColor4f(0.0, 0.0, 0.0, 1.0);
				v1 = dat_it->vertices_[(*e_it).from_h]; v2 = dat_it->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
			}
			i++;
		}


		glDepthFunc(GL_LESS);
		oi++;
	}

	glDepthFunc(GL_LESS);
}

void QGLViewerWidget::draw_mesh_solidflatDAT()
{
	GLfloat color[7][4] = { { 1.0, 0.5, 0.5 ,0.3 } ,{ 0.5, 1.0, 0.5 ,0.3 } ,{ 0.5, 0.5, 1.0 ,0.3 } ,{ 1.0, 1.0, 0.5 ,0.3 },
	{ 0.5, 1.0, 1.0 ,0.3 } ,{ 1.0, 0.5, 1.0 ,0.3 },{ 1.0, 0.5, 0.0 ,0.3 } };
	GLfloat shine[] = { 120.0f };
	int i = 0;
	int vmeshsize = vmesh.size();
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin(); dat_it != vmesh.end(); dat_it++)
	{
		int j;
		if (colorcheck == false)
		{
			j = datdrawnum[i];
			j = j % 7;
			std::set<CellHandle> n_cc = mesh->neighbor_c_circle_c(CellHandle(i));
			/*for (std::set<CellHandle>::iterator n_cc_it = n_cc.begin();n_cc_it != n_cc.end();n_cc_it++)
			{
				if (tempdraw[*n_cc_it] == j)
					j = (j + 1) % 6;
			}*/
			std::vector<int> tempcolor(n_cc.size());
			int nccnum = 0;
			for (std::set<CellHandle>::iterator n_cc_it = n_cc.begin(); n_cc_it != n_cc.end(); n_cc_it++)
			{
				tempcolor[nccnum] = tempdraw[*n_cc_it];
				nccnum++;
			}
			while (std::find(tempcolor.begin(), tempcolor.end(), j) != tempcolor.end())
				j = (j + 1) % 7;
			/*if (checkcell[i] == false)
				j = 0;*/
			tempdraw[i] = j;
			if (i == vmeshsize - 1)
				colorcheck = true;
		}
		else
		{
			j = tempdraw[datdrawnum[i]];
		}

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		glBegin(GL_QUADS);
		std::vector<Face> datface = dat_it->faces_;
		int t = 0;
		for (std::vector<Face>::iterator datface_it = datface.begin(); datface_it != datface.end(); datface_it++)
		{
			if (dat_it->is_bdy(FaceHandle(t)))
			{
				V3f norm = dat_it->cal_norm(FaceHandle(t));
				std::vector<VertexHandle> datfv = datface_it->vs_f;
				glNormal3d(norm.x, norm.y, norm.z);
				for (std::vector<VertexHandle>::iterator datfv_it = datfv.begin(); datfv_it != datfv.end(); datfv_it++)
				{
					glVertex3d(dat_it->vertices_[*datfv_it].x, dat_it->vertices_[*datfv_it].y,
						dat_it->vertices_[*datfv_it].z);
				}
			}
			t++;
		}
		glEnd();
		i++;

	}
	//���и����othermesh
	int oi = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = othermesh.begin(); dat_it != othermesh.end(); dat_it++)
	{
		int oid = othermeshID[oi];
		glDisable(GL_LIGHTING);
		glColor4f(0.5, 0.5, 0.5, 0.1);
		glBegin(GL_QUADS);
		std::vector<Face> datface = dat_it->faces_;
		int t = 0;
		for (std::vector<Face>::iterator datface_it = datface.begin(); datface_it != datface.end(); datface_it++)
		{
			if (dat_it->is_bdy(FaceHandle(t)))
			{
				if (datisbdy(oid, FaceHandle(t)))
				{
					std::vector<VertexHandle> datfv = datface_it->vs_f;
					for (std::vector<VertexHandle>::iterator datfv_it = datfv.begin(); datfv_it != datfv.end(); datfv_it++)
					{
						glVertex3d(dat_it->vertices_[*datfv_it].x, dat_it->vertices_[*datfv_it].y,
							dat_it->vertices_[*datfv_it].z);
					}
				}

			}
			t++;
		}
		oi++;
		glEnd();

	}
}

void QGLViewerWidget::draw_mesh_solidflatIsoline()
{
	GLfloat color[7][4] = { { 1.0, 0.5, 0.5 ,1.0 } ,{ 0.5, 1.0, 0.5 ,1.0 } ,{ 0.5, 0.5, 1.0 ,1.0 } ,{ 1.0, 1.0, 0.5 ,1.0 },
	{ 0.5, 1.0, 1.0 ,1.0 } ,{ 1.0, 0.5, 1.0 ,1.0 },{ 1.0, 0.5, 0.0 ,1.0 } };
	GLfloat shine[] = { 120.0f };
	int i = 0;
	int vmeshsize = vmesh.size();
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin(); dat_it != vmesh.end(); dat_it++)
	{
		int j;
		if (colorcheck == false)
		{
			j = datdrawnum[i];
			j = j % 7;
			std::set<CellHandle> n_cc = mesh->neighbor_c_circle_c(CellHandle(i));
			/*for (std::set<CellHandle>::iterator n_cc_it = n_cc.begin();n_cc_it != n_cc.end();n_cc_it++)
			{
				if (tempdraw[*n_cc_it] == j)
					j = (j + 1) % 6;
			}*/
			std::vector<int> tempcolor(n_cc.size());
			int nccnum = 0;
			for (std::set<CellHandle>::iterator n_cc_it = n_cc.begin(); n_cc_it != n_cc.end(); n_cc_it++)
			{
				tempcolor[nccnum] = tempdraw[*n_cc_it];
				nccnum++;
			}
			while (std::find(tempcolor.begin(), tempcolor.end(), j) != tempcolor.end())
				j = (j + 1) % 7;
			/*if (checkcell[i] == false)
				j = 0;*/
			tempdraw[i] = j;
			if (i == vmeshsize - 1)
				colorcheck = true;
		}
		else
		{
			j = tempdraw[datdrawnum[i]];
		}

		std::vector<double> color_temp;
		color_temp.push_back(color[j][0] * 255);
		color_temp.push_back(color[j][1] * 255);
		color_temp.push_back(color[j][2] * 255);
		bezier_color.push_back(color_temp);

		/*qDebug()<< color[j][0] * 255 << " "
		<< color[j][1] * 255 << " "
		<< color[j][2] * 255 << " ";*/

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		glBegin(GL_QUADS);
		std::vector<Face> datface = dat_it->faces_;
		int t = 0;
		for (std::vector<Face>::iterator datface_it = datface.begin(); datface_it != datface.end(); datface_it++)
		{
			if (dat_it->is_bdy(FaceHandle(t)))
			{
				V3f norm = datnorm[datdrawnum[i]][t];
				std::vector<VertexHandle> datfv = datface_it->vs_f;
				glNormal3d(norm.x, norm.y, norm.z);
				for (std::vector<VertexHandle>::iterator datfv_it = datfv.begin(); datfv_it != datfv.end(); datfv_it++)
				{
					glVertex3d(dat_it->vertices_[*datfv_it].x, dat_it->vertices_[*datfv_it].y,
						dat_it->vertices_[*datfv_it].z);
				}
			}
			t++;
		}
		glEnd();
		i++;
	}

}

void QGLViewerWidget::draw_mesh_solidflatdatonly()
{
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	GLfloat shine[] = { 120.0f };
	int i = 0;
	int vmeshsize = vmesh.size();
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin(); dat_it != vmesh.end(); dat_it++)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		glBegin(GL_QUADS);
		std::vector<Face> datface = dat_it->faces_;
		int t = 0;
		for (std::vector<Face>::iterator datface_it = datface.begin(); datface_it != datface.end(); datface_it++)
		{
			if (dat_it->is_bdy(FaceHandle(t)))
			{
				V3f norm = datnorm[datdrawnum[i]][t];
				std::vector<VertexHandle> datfv = datface_it->vs_f;
				glNormal3d(norm.x, norm.y, norm.z);
				for (std::vector<VertexHandle>::iterator datfv_it = datfv.begin(); datfv_it != datfv.end(); datfv_it++)
				{
					glVertex3d(dat_it->vertices_[*datfv_it].x, dat_it->vertices_[*datfv_it].y,
						dat_it->vertices_[*datfv_it].z);
				}
			}
			t++;
		}
		glEnd();
		i++;
	}

}

void QGLViewerWidget::draw_mesh_all_polys_linesIsoline()
{

	glLineWidth(0.01);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	//glDrawBuffer(GL_NONE);
	//glDepthRange(0.01, 1.0);

	int i = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin(); dat_it != vmesh.end(); dat_it++)
	{
		int j = datdrawnum[i];
		std::vector<std::vector<V3f>> iline = isoline[j];
		for (std::vector<std::vector<V3f>>::iterator iline_it = iline.begin(); iline_it != iline.end(); iline_it++)
		{
			std::vector<V3f> ilv = *iline_it;
			int ilvsize = ilv.size();
			for (int ln = 0; ln < ilvsize - 1; ln++)
			{
				glBegin(GL_LINES);
				glVertex3d(ilv[ln].x, ilv[ln].y, ilv[ln].z); glVertex3d(ilv[ln + 1].x, ilv[ln + 1].y, ilv[ln + 1].z);
				glEnd();
			}
		}
		i++;
	}

	/*glDrawBuffer(GL_BACK);
	glDepthRange(0.01, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	i = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = vmesh.begin();dat_it != vmesh.end();dat_it++)
	{
		int j = datdrawnum[i];
		std::vector<std::vector<V3f>> iline = isoline[j];
		for (std::vector<std::vector<V3f>>::iterator iline_it = iline.begin();iline_it != iline.end();iline_it++)
		{
			std::vector<V3f> ilv = *iline_it;
			int ilvsize = ilv.size();
			for (int ln = 0;ln < ilvsize - 1;ln++)
			{

				glBegin(GL_LINES);
				glVertex3d(ilv[ln].x, ilv[ln].y, ilv[ln].z);glVertex3d(ilv[ln + 1].x, ilv[ln + 1].y, ilv[ln + 1].z);
				glEnd();

			}
		}
		i++;
	}
	glDepthFunc(GL_LESS);*/
}

void QGLViewerWidget::draw_mesh_solidflatIsoline_notdraw()
{
	//���и����othermesh
	int oi = 0;
	for (std::vector<HexV3fMesh>::iterator dat_it = othermesh.begin(); dat_it != othermesh.end(); dat_it++)
	{
		//std::cout << othermesh.size() << std::endl;
		int oid = othermeshID[oi];
		glDisable(GL_LIGHTING);
		glColor4f(0.5, 0.5, 0.5, 0.1);
		glBegin(GL_QUADS);
		std::vector<Face> datface = dat_it->faces_;
		int t = 0;
		int facecheck = 0;
		for (std::vector<Face>::iterator datface_it = datface.begin(); datface_it != datface.end(); datface_it++)
		{
			if (dat_it->is_bdy(FaceHandle(t)))
			{
				if (datisbdy(oid, FaceHandle(t)))
				{
					facecheck++;
					std::vector<VertexHandle> datfv = datface_it->vs_f;
					for (std::vector<VertexHandle>::iterator datfv_it = datfv.begin(); datfv_it != datfv.end(); datfv_it++)
					{
						glVertex3d(dat_it->vertices_[*datfv_it].x, dat_it->vertices_[*datfv_it].y,
							dat_it->vertices_[*datfv_it].z);
					}
				}

			}
			t++;
		}
		//std::cout << "facecheck:" << facecheck << std::endl;
		oi++;
		glEnd();

	}
}

void QGLViewerWidget::draw_mesh_solidflatDatpoint()
{
	GLfloat color[] = { 0.2,0.2,0.2,0.15 };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_wireframeDatpoint()
{
	glLineWidth(1);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	//glDrawBuffer(GL_NONE);
	//glDepthRange(0.01, 1.0);

	std::vector<Edge>::iterator e_it = mesh->edges_.begin();
	V3f v1;
	V3f v2;
	int i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
		glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		glEnd();
		i++;
	}

	/*glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	e_it = mesh->edges_.begin();
	i = 0;
	for (e_it; e_it != mesh->edges_.end();++e_it)
	{
		v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
		glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		glEnd();
		i++;
	}

	glDepthFunc(GL_LESS);*/
}

void QGLViewerWidget::draw_mesh_all_pointsDatpoint1()
{
	/*glPointSize(15);
	GLfloat color[] = { 0.0, 0.0, 0.0 };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
	glColor3f(0.0, 0.0, 0.0);
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		/*if ((*v_it).is_valid == true)
			glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);*/
		glBegin(GL_LINE_LOOP);
		draw_small_circle1(*v_it);
		glEnd();
	}
}

void QGLViewerWidget::draw_mesh_all_pointsDatpoint2()
{
	/*glPointSize(17);
	GLfloat color1[] = { 1.0, 0.0, 0.0 };
	GLfloat color2[] = { 0.0, 1.0, 0.0 };
	GLfloat color3[] = { 0.0, 0.0, 1.0 };
	GLfloat color4[] = { 1.0, 1.0, 0.0 };
	GLfloat shine[] = { 120.0f };
	glBegin(GL_POINTS);*/
	int i = 0;
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		V3f v = *v_it;
		if ((*v_it).is_valid == true)
		{
			if (mesh->neighbor_v[i].size() == 3)
			{
				//glBegin(GL_POINTS);
				/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color2);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
				//glColor3f(0.0, 1.0, 0.0);
				//glColor3f(0.0, 0.0, 0.0);
				//glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
				if (!small_check)
					circlev.push_back(v);
			}
			else if (mesh->neighbor_v[i].size() == 4)
			{
				/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color4);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color4);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color4);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
				//glColor3f(1.0, 1.0, 0.0);
				//draw_small_triangle(*v_it);
				//glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
				if (!small_check)
					trianglev.push_back(v);
			}
			else if (mesh->neighbor_v[i].size() == 5)
			{
				/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color3);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color3);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color3);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
				//glColor3f(0.0, 0.0, 1.0);
				//glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
				if (!small_check)
					diamondv.push_back(v);
			}
			else
			{
				/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
				//glColor3f(1.0, 0.0, 0.0);
				//glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
				if (!small_check)
					squarev.push_back(v);
			}
		}
		i++;
	}
	small_check = true;
	//glEnd();
}

void QGLViewerWidget::draw_mesh_all_pointsInterior()
{
	GLfloat color1[] = { 1.0, 0.0, 0.0 };
	GLfloat color2[] = { 0.0, 1.0, 0.0 };
	GLfloat color3[] = { 0.0, 0.0, 1.0 };
	GLfloat color4[] = { 1.0, 1.0, 0.0 };
	GLfloat shine[] = { 120.0f };
	glPointSize(10);
	glBegin(GL_POINTS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glVertex3d(0.25, 0.25, 0.75);
	glVertex3d(0.25, 0.25, 1.25);
	glEnd();
	glPointSize(15);
	glBegin(GL_POINTS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glVertex3d(0.25, 0.25, 1);
	glEnd();
}

void QGLViewerWidget::draw_mesh_solidflatInterior()
{
	GLfloat color[] = { 0.2,0.2,0.2,0.15 };
	GLfloat color1[] = { 1.0,0.0,0.0,0.5 };
	GLfloat shine[] = { 120.0f };
	glBegin(GL_QUADS);
	int faces = mesh->faces_.size();
	for (int i = 0; i < faces; i++)
	{
		if (mesh->is_bdy(FaceHandle(i)))
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			V3f n = mesh->cal_norm(FaceHandle(i));
			glNormal3d(n.x, n.y, n.z);
			std::vector<VertexHandle> fv = mesh->faces_[i].vs_f;
			for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
			{
				glVertex3d(mesh->vertices_[*fv_it].x, mesh->vertices_[*fv_it].y,
					mesh->vertices_[*fv_it].z);
			}
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			std::vector<VertexHandle> fv = mesh->faces_[i].vs_f;
			for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
			{
				glVertex3d(mesh->vertices_[*fv_it].x, mesh->vertices_[*fv_it].y,
					mesh->vertices_[*fv_it].z);
			}
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_all_pointsEdge()
{
	GLfloat color1[] = { 1.0, 0.0, 0.0 };
	GLfloat color2[] = { 0.0, 1.0, 0.0 };
	GLfloat color3[] = { 0.0, 0.0, 1.0 };
	GLfloat color4[] = { 1.0, 1.0, 0.0 };
	GLfloat shine[] = { 120.0f };
	glPointSize(10);
	glBegin(GL_POINTS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glVertex3d(0.5, 0.375, 0.375);
	glVertex3d(0.375, 0.5625, 0.375);
	glVertex3d(0.625, 0.625, 0.375);
	glEnd();
	glPointSize(15);
	glBegin(GL_POINTS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color4);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glVertex3d(0.5, 0.5, 0.375);
	glEnd();
}

void QGLViewerWidget::draw_mesh_solidflatEdge()
{
	GLfloat color[] = { 0.2,0.2,0.2,0.15 };
	GLfloat color1[] = { 1.0,0.0,0.0,0.5 };
	GLfloat shine[] = { 120.0f };
	glBegin(GL_QUADS);
	int faces = mesh->faces_.size();
	for (int i = 0; i < faces; i++)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		V3f n = mesh->cal_norm(FaceHandle(i));
		glNormal3d(n.x, n.y, n.z);
		std::vector<VertexHandle> fv = mesh->faces_[i].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			glVertex3d(mesh->vertices_[*fv_it].x, mesh->vertices_[*fv_it].y,
				mesh->vertices_[*fv_it].z);
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_wireframeEdge()
{
	//glLineWidth(0.5);
	//glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glDrawBuffer(GL_NONE);
	glDepthRange(0.01, 1.0);

	std::vector<Edge>::iterator e_it = mesh->edges_.begin();
	V3f v1;
	V3f v2;
	int i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		glLineWidth(0.005);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
		glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		glEnd();
		i++;
	}
	glLineWidth(3);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3d(0.5, 0.5, 0); glVertex3d(0.5, 0.5, 0.5);
	glEnd();

	glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	e_it = mesh->edges_.begin();
	i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		glLineWidth(0.005);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
		glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		glEnd();
		i++;
	}
	glLineWidth(3);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3d(0.5, 0.5, 0); glVertex3d(0.5, 0.5, 0.5);
	glEnd();

	glDepthFunc(GL_LESS);
}

void QGLViewerWidget::draw_mesh_all_pointsCorner()
{
	GLfloat color1[] = { 1.0, 0.0, 0.0 };
	GLfloat color2[] = { 0.0, 1.0, 0.0 };
	GLfloat color3[] = { 0.0, 0.0, 1.0 };
	GLfloat color4[] = { 1.0, 1.0, 0.0 };
	GLfloat shine[] = { 120.0f };
	glPointSize(10);
	glBegin(GL_POINTS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	V3f centerv(1, 1, 1);
	std::vector<Cell> c = mesh->cells_;
	for (std::vector<Cell>::iterator c_it = c.begin(); c_it != c.end(); c_it++)
	{
		std::vector<VertexHandle> cv = (*c_it).vertex_;
		V3f ver;
		for (std::vector<VertexHandle>::iterator cv_it = cv.begin(); cv_it != cv.end(); cv_it++)
		{
			ver = ver + mesh->vertices_[*cv_it];
		}
		ver = ver / 8;
		ver = (ver + centerv) / 2;
		glVertex3d(ver.x, ver.y, ver.z);
	}
	glEnd();
	glPointSize(15);
	glBegin(GL_POINTS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glVertex3d(centerv.x, centerv.y, centerv.z);
	glEnd();
}

void QGLViewerWidget::draw_mesh_solidflatTran()
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat shine_i[] = { 120.0f };

	GLfloat color[] = { 0.2, 0.2, 0.8, 0.5 };
	//glDisable(GL_LIGHTING);
	//glColor4f(0.5,0.50,0.50,1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	//���и���
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::DrawSkel() {
	//// �? glPolygonOffset 设定面的深度偏移
	//// 深度偏移 使模型�??�?挡部分不会�??绘制出来
	// 设置着色模�?
	// GL_FLAT 最新的颜色值来规定
	// GL_SMOOTH 渐变�?
	/*if (flag) {
		myskel->OPDelete();
	}*/
	glShadeModel(GL_FLAT);
	glColor3d(0.2, 0.2, 0.2);
	for (int i = 0; i < myskel->SkelAll.size(); ++i) {
		if (myskel->SkelAll[i].is_valid == true) {
			for (auto iter : myskel->SkelAll[i].neighbor) {
				glLineWidth(2);
				glBegin(GL_LINES);
				glVertex3d(myskel->SkelAll[i].pos.x, myskel->SkelAll[i].pos.y, myskel->SkelAll[i].pos.z);
				glVertex3d(myskel->SkelAll[iter].pos.x, myskel->SkelAll[iter].pos.y, myskel->SkelAll[iter].pos.z);
				glEnd();
			}
		}
	}

	glColor3d(1.0, 0.0, 0.0);
	for (int i = 0; i < myskel->SkelAll.size(); i++) {
		if (myskel->SkelAll[i].is_valid == true) {
			glPointSize(5);
			glBegin(GL_POINTS);
			glVertex3d(myskel->SkelAll[i].pos.x, myskel->SkelAll[i].pos.y, myskel->SkelAll[i].pos.z);
			glEnd();
		}
	}
}


void QGLViewerWidget::draw_errorv()
{
	glPointSize(5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (std::vector<V3f>::iterator v_it = drawvbig.begin(); v_it != drawvbig.end(); ++v_it)
	{
		glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
	}
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POINTS);
	for (std::vector<V3f>::iterator v_it = drawvsmall.begin(); v_it != drawvsmall.end(); ++v_it)
	{
		glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
	}
	glEnd();
}

void QGLViewerWidget::draw_mesh_solidflatTran1()
{
	GLfloat color[] = { 0.5,0.0,0.0,1.0 };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v1.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v1[i].x, normal_boundary_face_v1[i].y, normal_boundary_face_v1[i].z);
		for (int j = 0; j < boundary_face_v1[i].size(); ++j)
		{
			glVertex3d(boundary_face_v1[i][j].x, boundary_face_v1[i][j].y,
				boundary_face_v1[i][j].z);
		}
	}
	glEnd();

	//���и���
	GLfloat color1[] = { 0.7,0.0,0.0,1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v1.size(); ++i)
	{
		glNormal3d(normal_cutting_face_v1[i].x, normal_cutting_face_v1[i].y, normal_cutting_face_v1[i].z);
		for (int j = 0; j < cutting_face_v1[i].size(); ++j)
		{
			glVertex3d(cutting_face_v1[i][j].x, cutting_face_v1[i][j].y,
				cutting_face_v1[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_mesh_compare()
{
	GLfloat shine_i[] = { 120.0f };
	//GLfloat color[] = { 0.2, 0.2, 0.6, 1.0 };
	//glDisable(GL_LIGHTING);
	//glColor4f(0.5,0.50,0.50,1.0);
	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);*/
	GLfloat colorB[] = { 0.0, 0.0, 0.8, 1.0 };
	GLfloat colorR[] = { 0.8, 0.0, 0.0, 1.0 };
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		bool check = true;
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			if (std::find(drawvbigid.begin(), drawvbigid.end(), boundary_face_VID[i][j]) != drawvbigid.end())
			{
				check = false;
				break;
			}
		}
		if (check == false)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorR);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorR);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorR);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorB);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorB);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorB);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
		}
		glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);

		}
	}
	glEnd();

	////���и���
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		bool check = true;
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			if (std::find(drawvbigid.begin(), drawvbigid.end(), cutting_face_VID[i][j]) != drawvbigid.end())
			{
				check = false;
				break;
			}
		}
		if (check == false)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorR);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorR);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorR);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorB);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorB);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorB);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine_i);
		}
		glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);

	//glBegin(GL_QUADS);
	//glColor3f(0.0, 0.0, 1.0);
	//glVertex3f(0.0, 0.0, 0.0);
	//
	//glColor3f(0.0, 0.0, 1.0);
	//glVertex3f(0.0, 1.0, 0.0);
	//
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex3f(1.0, 1.0, 0.0);
	//
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex3f(1.0, 0.0, 0.0);
	//
	//*glColor3f(0.0, 0.0, 1.0);
	//glVertex3f(-0.5, -1.0, 0.0);*/
	//glEnd();
}

void QGLViewerWidget::draw_mesh_compare_new()
{
	errmax = -999; errmin = 999;
	int errnum = colorverr.size();
	for (int i = 0; i < errnum; i++)
	{
		if (errmax < colorverr[i])
			errmax = colorverr[i];
		if (errmin > colorverr[i])
			errmin = colorverr[i];
	}
	setcolor.set_value_range(errmax, errmin);
	//setcolor.set_value_range(0.05, 0);
	//qDebug() << errmax << errmin << endl;
	//setcolor.set_value_range(0.0823945, 0);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		//glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			int bv_id = boundary_face_VID[i][j];
			double bv_err = colorverr[bv_id];
			/*int q = 0;
			for (std::set<newV3f>::iterator color_it = color.begin();color_it!=color.end();color_it++)
			{
			if (q == bv_id)
			{
			bv_err = (*color_it).error;
			break;
			}
			q++;
			}*/
			double R, G, B;
			setcolor.value2rgb(bv_err, R, G, B);
			glColor3d(R, G, B);
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
			//}


		}
	}
	glEnd();

	////���и���
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		//glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);		
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			/*if (cutting_face_v[i][j] == selectpoint)
			{
				glColor3d(1, 0, 0);
				glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
					cutting_face_v[i][j].z);
			}
			else
			{*/
			int cv_id = cutting_face_VID[i][j];
			double cv_err = colorverr[cv_id];
			/*int q = 0;
			for (std::set<newV3f>::iterator color_it = color.begin();color_it != color.end();color_it++)
			{
			if (q == cv_id)
			{
			cv_err = (*color_it).error;
			break;
			}
			q++;
			}*/
			double R, G, B;
			setcolor.value2rgb(cv_err, R, G, B);
			glColor3d(R, G, B);
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
			//}

		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::drawXOZ() {
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glShadeModel(GL_SMOOTH);

	GLfloat color[] = { 0.5,0.5,0.5,0.1 };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glBegin(GL_QUADS);
	glNormal3d(0,1,0);
	glVertex3d(-1000, 0, -1000);
	glVertex3d(-1000, 0, 1000);
	glVertex3d(1000, 0, 1000);
	glVertex3d(1000, 0, -1000);
	// for (int i = 0; i < frameface.size(); ++i)
	// {
	// 	glNormal3d(framefacenrom[i].x, framefacenrom[i].y, framefacenrom[i].z);
	// 	for (int j = 0; j < frameface[i].size(); ++j)
	// 	{
	// 		glVertex3d(frameface[i][j].x, frameface[i][j].y,
	// 			frameface[i][j].z);
	// 	}
	// }
	glEnd();
	glEnable(GL_LIGHTING);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}



void QGLViewerWidget::draw_mesh_compare_newXJL()
{
	errmax = -999; errmin = 999;
	int errnum = colorverr.size();
	for (int i = 0; i < errnum; i++)
	{
		if (errmax < colorverr[i])
			errmax = colorverr[i];
		if (errmin > colorverr[i])
			errmin = colorverr[i];
	}
	setcolor.set_value_range(errmax, errmin);
	//qDebug() << errmax << errmin << endl;
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		FaceHandle bff = boundary_face[i];
		bool check_temp = true;
		std::set<CellHandle, compare_OVM> bff_c = mesh->neighbor_f[bff];
		for (std::set<CellHandle, compare_OVM>::iterator bff_c_it = bff_c.begin(); bff_c_it != bff_c.end(); ++bff_c_it)
		{
			if (xjlcheck[*bff_c_it])
			{
				check_temp = false;
				break;
			}
		}
		if (check_temp)
			continue;
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			int bv_id = boundary_face_VID[i][j];
			double bv_err = colorverr[bv_id];
			double R, G, B;
			setcolor.value2rgb(bv_err, R, G, B);
			glColor3d(R, G, B);
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	////���и���
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		FaceHandle bff = boundary_face[i];
		bool check_temp = true;
		std::set<CellHandle, compare_OVM> bff_c = mesh->neighbor_f[bff];
		for (std::set<CellHandle, compare_OVM>::iterator bff_c_it = bff_c.begin(); bff_c_it != bff_c.end(); ++bff_c_it)
		{
			if (xjlcheck[*bff_c_it])
			{
				check_temp = false;
				break;
			}
		}
		if (check_temp)
			continue;
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			int cv_id = cutting_face_VID[i][j];
			double cv_err = colorverr[cv_id];
			double R, G, B;
			setcolor.value2rgb(cv_err, R, G, B);
			glColor3d(R, G, B);
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::drawtest()
{
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, -1.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, -1.0, 0.0);

	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(1.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);


	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(1.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);


	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);


	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0.0, 2.0, 0.0);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 3.0, 0.0);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 3.0, 0.0);
	glEnd();
}

void QGLViewerWidget::draw_mesh_wireframe_view_singular()
{



	glDrawBuffer(GL_NONE);
	glDepthRange(0.01, 1.0);

	std::vector<Edge>::iterator e_it = mesh->edges_.begin();
	V3f v1;
	V3f v2;
	int i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		if (!mesh->is_bdy(EdgeHandle(i)))
		{
			if (mesh->neighbor_e[i].size() == 3)
			{
				glLineWidth(6);
				glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else if (mesh->neighbor_e[i].size() == 5)
			{
				glLineWidth(6);
				glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
			{
				glLineWidth(0.5);
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
		}
		else
		{
			glLineWidth(0.5);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
			i++;
		}

	}

	glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	e_it = mesh->edges_.begin();
	i = 0;
	for (e_it; e_it != mesh->edges_.end(); ++e_it)
	{
		if (!mesh->is_bdy(EdgeHandle(i)))
		{
			if (mesh->neighbor_e[i].size() == 3)
			{
				glLineWidth(6);
				glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else if (mesh->neighbor_e[i].size() == 5)
			{
				glLineWidth(6);
				glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
			else
			{
				glLineWidth(0.5);
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
				glBegin(GL_LINES);
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
				i++;
			}
		}
		else
		{
			glLineWidth(0.5);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			v1 = mesh->vertices_[(*e_it).from_h]; v2 = mesh->vertices_[(*e_it).to_h];
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
			i++;
		}
	}

	glDepthFunc(GL_LESS);
}

void QGLViewerWidget::draw_mesh_all_points_view_singular()
{
	GLfloat color1[] = { 1.0, 0.0, 0.0 };
	GLfloat shine[] = { 120.0f };
	glPointSize(30);
	glBegin(GL_POINTS);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	int i = 0;
	for (std::vector<V3f>::iterator mv_it = mesh->vertices_.begin(); mv_it != mesh->vertices_.end(); mv_it++)
	{
		if (!mesh->is_bdy(VertexHandle(i)))
		{
			if (mesh->is_singular(VertexHandle(i)))
			{
				glVertex3d((*mv_it).x, (*mv_it).y, (*mv_it).z);
				i++;
			}
			else
				i++;
		}
		else
			i++;
	}
	glEnd();
}

void QGLViewerWidget::drawtwonpoint()
{
	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	/*for (std::vector<V3f>::iterator v_it = twonpoint.begin();v_it != twonpoint.end();++v_it)
	{
		glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);

	}*/
	mesh->cal_cen();
	V3f cen = mesh->center;
	glVertex3d(cen.x, cen.y, cen.z);
	glEnd();
}

void QGLViewerWidget::drawtwonpointlines()
{
	glLineWidth(1.0);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glDrawBuffer(GL_NONE);
	glDepthRange(0.01, 1.0);

	V3f v1;
	V3f v2;
	int num = twonpoint.size();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num - 1; i++)
	{
		v1 = twonpoint[i];
		v2 = twonpoint[i + 1];
		//glBegin(GL_LINE_LOOP);
		glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		//glEnd();
	}
	glEnd();

	glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num - 1; i++)
	{
		v1 = twonpoint[i];
		v2 = twonpoint[i + 1];
		//glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		//glEnd();
	}
	glEnd();
	glDepthFunc(GL_LESS);

}

void QGLViewerWidget::draw_mesh_errorv()
{
	errmax = -999; errmin = 999;
	int errnum = colorverr.size();
	for (int i = 0; i < errnum; i++)
	{
		if (errmax < colorverr[i])
			errmax = colorverr[i];
		if (errmin > colorverr[i])
			errmin = colorverr[i];
	}
	setcolor.set_value_range(errmax, errmin);
	//qDebug() << errmax << errmin << endl;
	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	//���߽���
	glBegin(GL_POINTS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			int bv_id = boundary_face_VID[i][j];
			double bv_err = colorverr[bv_id];
			double R, G, B;
			setcolor.value2rgb(bv_err, R, G, B);
			glColor3d(R, G, B);
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	////���и���
	glBegin(GL_POINTS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			int cv_id = cutting_face_VID[i][j];
			double cv_err = colorverr[cv_id];
			double R, G, B;
			setcolor.value2rgb(cv_err, R, G, B);
			glColor3d(R, G, B);
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_mesh_test()
{
	glDisable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
		/*if(boundary_face[i] == FaceHandle(4)|| boundary_face[i] == FaceHandle(7)||
			boundary_face[i] == FaceHandle(12)||boundary_face[i] == FaceHandle(16))*/
		if (boundary_face[i] == FaceHandle(5) || boundary_face[i] == FaceHandle(6) ||
			boundary_face[i] == FaceHandle(11) || boundary_face[i] == FaceHandle(17))
			glColor4f(1, 0, 0, 1);
		else
			glColor4f(0, 0, 1, 1);
		for (int j = 0; j < boundary_face_v[i].size(); ++j)
		{
			glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
				boundary_face_v[i][j].z);
		}
	}
	glEnd();

	//���и���
	glDisable(GL_LIGHTING);
	glColor4f(0, 0, 1, 1);
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();

}

void QGLViewerWidget::drawpickpoint()
{
	glPointSize(10);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (std::vector<VertexHandle>::iterator v_it = pickpoint_v.begin(); v_it != pickpoint_v.end(); ++v_it)
	{
		V3f v = mesh->vertices_[*v_it];
		glVertex3d(v.x, v.y, v.z);
	}
	glEnd();
}

void QGLViewerWidget::drawlinepoint()
{
	glPointSize(10);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int j = 0; j < pickpoint_vtoline.size(); j++)
	{
		int num = pickpoint_vtoline[j].size();
		for (int i = 0; i < num; i++)
		{
			V3f v = mesh->vertices_[pickpoint_vtoline[j][i]];
			glVertex3d(v.x, v.y, v.z);
		}
	}
	glEnd();
}

void QGLViewerWidget::drawpickpointtoline()
{
	glLineWidth(1.0);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glDrawBuffer(GL_NONE);
	glDepthRange(0.01, 1.0);

	V3f v1;
	V3f v2;
	for (int j = 0; j < pickpoint_vtoline.size(); j++)
	{
		int num = pickpoint_vtoline[j].size();
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < num - 1; i++)
		{
			v1 = mesh->vertices_[pickpoint_vtoline[j][i]];
			v2 = mesh->vertices_[pickpoint_vtoline[j][i + 1]];
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		}
		glEnd();
	}


	glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < pickpoint_vtoline.size(); j++)
	{
		int num = pickpoint_vtoline[j].size();
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < num - 1; i++)
		{
			v1 = mesh->vertices_[pickpoint_vtoline[j][i]];
			v2 = mesh->vertices_[pickpoint_vtoline[j][i + 1]];
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		}
		glEnd();
	}
	glDepthFunc(GL_LESS);

}

void QGLViewerWidget::drawpickedge()
{
	glLineWidth(1.0);
	/*glDrawBuffer(GL_NONE);
	glDepthRange(0.01, 1.0);*/

	V3f v1;
	V3f v2;
	glBegin(GL_LINES);
	for (int i = 0; i < mesh->edges_.size(); i++)
	{
		//if (mesh->is_bdy(EdgeHandle(i)))
		//{
		if (std::find(pickedge_e.begin(), pickedge_e.end(), i) == pickedge_e.end())
		{
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			v1 = mesh->vertices_[mesh->edges_[i].from_h];
			v2 = mesh->vertices_[mesh->edges_[i].to_h];
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		}
		else
		{
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			v1 = mesh->vertices_[mesh->edges_[i].from_h];
			v2 = mesh->vertices_[mesh->edges_[i].to_h];
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		}
		//}
	}
	glEnd();

	/*glLineWidth(3.0);
	glBegin(GL_LINES);
	for (int i = 0; i < mesh->edges_.size(); i++)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			if (std::find(pickedge_e.begin(), pickedge_e.end(), i) != pickedge_e.end())
			{
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				v1 = mesh->vertices_[mesh->edges_[i].from_h];
				v2 = mesh->vertices_[mesh->edges_[i].to_h];
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
			}
		}
	}
	glEnd();*/


	/*glDrawBuffer(GL_BACK);
	glDepthRange(0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);

	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < pickpoint_vtoline.size(); j++)
	{
		int num = pickpoint_vtoline[j].size();
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < num - 1; i++)
		{
			v1 = mesh->vertices_[pickpoint_vtoline[j][i]];
			v2 = mesh->vertices_[pickpoint_vtoline[j][i + 1]];
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		}
		glEnd();
	}
	glDepthFunc(GL_LESS);*/
}

void QGLViewerWidget::drawpickedface()
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.50, 0.50, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.50, 0.50, 1.0 };
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		if (std::find(pickface.begin(), pickface.end(), boundary_face[i]) == pickface.end())
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
			for (int j = 0; j < boundary_face_v[i].size(); ++j)
			{
				glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
					boundary_face_v[i][j].z);
			}
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
			for (int j = 0; j < boundary_face_v[i].size(); ++j)
			{
				glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
					boundary_face_v[i][j].z);
			}
		}
	}
	glEnd();

	//���и���
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
		for (int j = 0; j < cutting_face_v[i].size(); ++j)
		{
			glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
				cutting_face_v[i][j].z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::drawpickedcell()
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };
	GLfloat mat_a_i[] = { 1.0, 0.5, 0.0, 1.0 };
	GLfloat mat_d_i[] = { 1.0, 0.5, 0.0, 1.0 };
	GLfloat mat_s_i[] = { 1.0, 0.5, 0.0, 1.0 };
	GLfloat mat_a_p[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat mat_d_p[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat mat_s_p[] = { 1.0, 1.0, 0.0, 1.0 };
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v.size(); ++i)
	{
		std::set<CellHandle, compare_OVM> fc = mesh->neighbor_f[boundary_face[i]];
		bool check = false;
		for (auto fc_it = fc.begin(); fc_it != fc.end(); fc_it++)
		{
			if (std::find(pickcell.begin(), pickcell.end(), *fc_it) != pickcell.end())
			{
				check = true;
				break;
			}
		}
		if (check == true)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_p);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_p);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_p);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
			for (int j = 0; j < boundary_face_v[i].size(); ++j)
			{
				glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
					boundary_face_v[i][j].z);
			}
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			glNormal3d(normal_boundary_face_v[i].x, normal_boundary_face_v[i].y, normal_boundary_face_v[i].z);
			for (int j = 0; j < boundary_face_v[i].size(); ++j)
			{
				glVertex3d(boundary_face_v[i][j].x, boundary_face_v[i][j].y,
					boundary_face_v[i][j].z);
			}
		}
	}
	glEnd();

	//���и���
	glBegin(GL_QUADS);
	for (int i = 0; i < cutting_face_v.size(); ++i)
	{
		std::set<CellHandle, compare_OVM> fc = mesh->neighbor_f[cutting_face[i]];
		bool check = false;
		for (auto fc_it = fc.begin(); fc_it != fc.end(); fc_it++)
		{
			if (std::find(pickcell.begin(), pickcell.end(), *fc_it) != pickcell.end())
			{
				check = true;
				break;
			}
		}
		if (check == true)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_p);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_p);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_p);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
			for (int j = 0; j < cutting_face_v[i].size(); ++j)
			{
				glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
					cutting_face_v[i][j].z);
			}
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a_i);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d_i);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s_i);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
			glNormal3d(normal_cutting_face_v[i].x, normal_cutting_face_v[i].y, normal_cutting_face_v[i].z);
			for (int j = 0; j < cutting_face_v[i].size(); ++j)
			{
				glVertex3d(cutting_face_v[i][j].x, cutting_face_v[i][j].y,
					cutting_face_v[i][j].z);
			}
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_beisurDATface()
{
	GLfloat color[6][4] = { { 1.0, 0.5, 0.5 ,1.0 } ,{ 0.5, 1.0, 0.5 ,1.0 } ,{ 0.5, 0.5, 1.0 ,1.0 } ,{ 1.0, 1.0, 0.5 ,1.0 },
	{ 0.5, 1.0, 1.0 ,1.0 } ,{ 1.0, 0.5, 1.0 ,1.0 } };
	GLfloat shine[] = { 120.0f };
	int beisurvsize = bei_surv_dat.size();
	if (colorcheck == false)
	{
		for (int i = 0; i < beisurvsize; ++i)
		{
			FaceHandle f = biaojiface[i];
			int j = i % 6;
			std::set<FaceHandle> n_ff = mesh->neighbor_f_f(1, f);
			std::vector<int> tempcolor(n_ff.size());
			int nffnum = 0;
			for (std::set<FaceHandle>::iterator n_ff_it = n_ff.begin(); n_ff_it != n_ff.end(); n_ff_it++)
			{
				tempcolor[nffnum] = tempdraw[*n_ff_it];
				nffnum++;
			}
			while (std::find(tempcolor.begin(), tempcolor.end(), j) != tempcolor.end())
				j = (j + 1) % 6;
			tempdraw[f] = j;
			if (i == beisurvsize - 1)
			{
				colorcheck = true;
				tempdraw1 = tempdraw;
			}
		}
	}
	for (int i = 0; i < beisurvsize; ++i)
	{
		int j = tempdraw[biaojiface[i]];
		std::vector<double> color_temp;
		color_temp.push_back(color[j][0] * 255);
		color_temp.push_back(color[j][1] * 255);
		color_temp.push_back(color[j][2] * 255);
		bezier_color.push_back(color_temp);

		/*qDebug() << color[j][0] * 255 << " "
			<< color[j][1] * 255 << " "
			<< color[j][2] * 255 << " ";*/

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		glBegin(GL_QUADS);
		for (int j = 0; j < bei_surv_dat[i].size(); ++j)
		{
			glNormal3d(-1 * nor_bei_surv_dat[i][j].x, -1 * nor_bei_surv_dat[i][j].y, -1 * nor_bei_surv_dat[i][j].z);
			for (int k = 0; k < bei_surv_dat[i][j].size(); ++k)
			{
				glVertex3d(bei_surv_dat[i][j][k].x, bei_surv_dat[i][j][k].y,
					bei_surv_dat[i][j][k].z);
			}
		}
		glEnd();
	}
}

void QGLViewerWidget::draw_beisurDATface1()
{
	GLfloat color[6][4] = { { 1.0, 0.5, 0.5 ,1.0 } ,{ 0.5, 1.0, 0.5 ,1.0 } ,{ 0.5, 0.5, 1.0 ,1.0 } ,{ 1.0, 1.0, 0.5 ,1.0 },
	{ 0.5, 1.0, 1.0 ,1.0 } ,{ 1.0, 0.5, 1.0 ,1.0 } };
	GLfloat shine[] = { 120.0f };
	int beisurvsize = bei_surv_dat.size();
	if (colorcheck == false)
	{
		for (int i = 0; i < beisurvsize; ++i)
		{
			FaceHandle f = biaojiface[i];
			int j = i % 6;
			std::set<FaceHandle> n_ff = mesh->neighbor_f_f(f);
			std::vector<int> tempcolor(n_ff.size());
			int nffnum = 0;
			for (std::set<FaceHandle>::iterator n_ff_it = n_ff.begin(); n_ff_it != n_ff.end(); n_ff_it++)
			{
				tempcolor[nffnum] = tempdraw[*n_ff_it];
				nffnum++;
			}
			while (std::find(tempcolor.begin(), tempcolor.end(), j) != tempcolor.end())
				j = (j + 1) % 6;
			tempdraw[f] = j;
			if (i == beisurvsize - 1)
			{
				colorcheck = true;
				tempdraw1 = tempdraw;
			}
		}
	}

	for (int i = 0; i < beisurvsize; ++i)
	{
		int j = tempdraw[biaojiface[i]];
		std::vector<double> color_temp;
		color_temp.push_back(color[j][0] * 255);
		color_temp.push_back(color[j][1] * 255);
		color_temp.push_back(color[j][2] * 255);
		bezier_color.push_back(color_temp);

		/*qDebug() << color[j][0] * 255 << " "
			<< color[j][1] * 255 << " "
			<< color[j][2] * 255 << " ";*/

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[j]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		glBegin(GL_QUADS);
		for (int j = 0; j < bei_surv_dat[i].size(); ++j)
		{
			glNormal3d(-1 * nor_bei_surv_dat[i][j].x, -1 * nor_bei_surv_dat[i][j].y, -1 * nor_bei_surv_dat[i][j].z);
			for (int k = 0; k < bei_surv_dat[i][j].size(); ++k)
			{
				glVertex3d(bei_surv_dat[i][j][k].x, bei_surv_dat[i][j][k].y,
					bei_surv_dat[i][j][k].z);
			}
		}
		glEnd();
	}
}

void QGLViewerWidget::draw_beisurDATline1()
{
	glLineWidth(0.005);
	//glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < edgeline.size(); ++i)
	{
		glBegin(GL_LINES);
		int edgelinenum = edgeline[i].size() - 1;
		for (int j = 0; j < edgelinenum; ++j)
		{
			glVertex3d(edgeline[i][j].x, edgeline[i][j].y, edgeline[i][j].z);
			glVertex3d(edgeline[i][j + 1].x, edgeline[i][j + 1].y, edgeline[i][j + 1].z);
		}
		glEnd();
	}
}

void QGLViewerWidget::draw_beisurDATline()
{
	glLineWidth(0.3);
	glColor3f(0.0f, 0.0f, 0.0f);
	//glColor3f(0.2f, 0.8f, 1.0f);
	for (int i = 0; i < edgeline.size(); ++i)
	{
		glBegin(GL_LINES);
		int edgelinenum = edgeline[i].size() - 1;
		for (int j = 0; j < edgelinenum; ++j)
		{
			glVertex3d(edgeline[i][j].x, edgeline[i][j].y, edgeline[i][j].z);
			glVertex3d(edgeline[i][j + 1].x, edgeline[i][j + 1].y, edgeline[i][j + 1].z);
		}
		glEnd();
	}
}

void QGLViewerWidget::draw_beisurDATsingularface()
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };

	GLfloat color[6][4] = { { 0.7, 0.7, 0.7 ,1.0 } ,{ 0.7, 0.0, 0.7 ,1.0 },{ 0.0, 0.7, 0.7 ,1.0 },{ 138, 43, 226 ,1.0 } ,
	{ 0, 119, 140 ,1.0 },{ 0.765, 0.56, 0.56 ,1.0 } };

	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	GLfloat shine[] = { 120.0f };
	int beisurvsize = bei_surv_dat.size();
	for (int i = 0; i < beisurvsize; ++i)
	{
		//if (singularface[i] == 0)
		//{
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		//}
		//else if (singularface[i] == 1)
		//{
		//	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[1]);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[1]);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[1]);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		//}
		//else
		//{
		//	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[2]);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[2]);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[2]);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
		//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
		//}
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

		/*std::vector<double> color_temp;
		color_temp.push_back(color[j][0] * 255);
		color_temp.push_back(color[j][1] * 255);
		color_temp.push_back(color[j][2] * 255);
		bezier_color.push_back(color_temp);*/

		/*qDebug() << color[j][0] * 255 << " "
			<< color[j][1] * 255 << " "
			<< color[j][2] * 255 << " ";*/

			/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color[j]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color[j]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color[j]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);*/
		glBegin(GL_QUADS);
		FaceHandle f = biaojiface[i];
		V3f center = bei_surv_center[i];
		double mindis = 999;
		/*for (int k = 0; k < pickdrawface_center.size(); ++k)
		{
			double dis = distance(center, pickdrawface_center[k]);
			if (dis < mindis)
				mindis = dis;
		}*/
		for (int j = 0; j < bei_surv_dat[i].size(); ++j)
		{
			if (std::find(pickdrawface.begin(), pickdrawface.end(), f) != pickdrawface.end() /*|| mindis < 0.01*/)
				glNormal3d(1 * nor_bei_surv_dat[i][j].x, 1 * nor_bei_surv_dat[i][j].y, 1 * nor_bei_surv_dat[i][j].z);
			else
				glNormal3d(-1 * nor_bei_surv_dat[i][j].x, -1 * nor_bei_surv_dat[i][j].y, -1 * nor_bei_surv_dat[i][j].z);
			for (int k = 0; k < bei_surv_dat[i][j].size(); ++k)
			{
				glVertex3d(bei_surv_dat[i][j][k].x, bei_surv_dat[i][j][k].y,
					bei_surv_dat[i][j][k].z);
			}
		}
		glEnd();
	}
}

void QGLViewerWidget::draw_beisurDATsingularface1()
{
	GLfloat temp[] = { 0.5,0.0,0.0,1.0 };
	GLfloat shine[] = { 120.0f };
	int beisurvsize = bei_surv_dat1.size();
	for (int i = 0; i < beisurvsize; ++i)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

		glBegin(GL_QUADS);
		for (int j = 0; j < bei_surv_dat1[i].size(); ++j)
		{
			glNormal3d(-1 * nor_bei1[i][j].x, -1 * nor_bei1[i][j].y, -1 * nor_bei1[i][j].z);
			for (int k = 0; k < bei_surv_dat1[i][j].size(); ++k)
			{
				glVertex3d(bei_surv_dat1[i][j][k].x, bei_surv_dat1[i][j][k].y,
					bei_surv_dat1[i][j][k].z);
			}
		}
		glEnd();
	}
}

void QGLViewerWidget::draw_beisurDATsingularfaceframe()
{
	GLfloat color[] = { 0.5,0.5,0.5,0.1 };
	GLfloat shine[] = { 120.0f };
	int beisurvsize = bei_surv_datframe.size();
	for (int i = 0; i < beisurvsize; ++i)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

		glBegin(GL_QUADS);
		for (int j = 0; j < bei_surv_datframe[i].size(); ++j)
		{
			glNormal3d(-1 * nor_beiframe[i][j].x, -1 * nor_beiframe[i][j].y, -1 * nor_beiframe[i][j].z);
			for (int k = 0; k < bei_surv_datframe[i][j].size(); ++k)
			{
				glVertex3d(bei_surv_datframe[i][j][k].x, bei_surv_datframe[i][j][k].y,
					bei_surv_datframe[i][j][k].z);
			}
		}
		glEnd();
	}
}

void QGLViewerWidget::draw_beisurDATsingularfaceframeV()
{
	GLfloat color[] = { 0.5,0.5,0.5,0.1 };
	GLfloat shine[] = { 120.0f };
	for (int k = 0; k < vframe.size(); ++k)
	{
		int beisurvsize = vframe[k].bei_surv_datframe.size();
		for (int i = 0; i < beisurvsize; ++i)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

			glBegin(GL_QUADS);
			for (int j = 0; j < vframe[k].bei_surv_datframe[i].size(); ++j)
			{
				glNormal3d(-1 * vframe[k].nor_beiframe[i][j].x, -1 * vframe[k].nor_beiframe[i][j].y, -1 * vframe[k].nor_beiframe[i][j].z);
				for (int m = 0; m < vframe[k].bei_surv_datframe[i][j].size(); ++m)
				{
					glVertex3d(vframe[k].bei_surv_datframe[i][j][m].x, vframe[k].bei_surv_datframe[i][j][m].y,
						vframe[k].bei_surv_datframe[i][j][m].z);
				}
			}
			glEnd();
		}
	}
}

void QGLViewerWidget::drawframe()
{
	glLineWidth(0.5);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < frame.size(); ++i)
	{
		glBegin(GL_LINES);
		glVertex3d(frame[i].first.x, frame[i].first.y, frame[i].first.z);
		glVertex3d(frame[i].second.x, frame[i].second.y, frame[i].second.z);
		glEnd();
	}
}

void QGLViewerWidget::drawframeface()
{
	GLfloat color[] = { 0,0,0.5,0.2 }; // GLfloat color[] = { 0.5,0.5,0.5,0.1 }
	GLfloat shine[] = { 120.0f };
	/*glPushAttrib(GL_LIGHTING_BIT);*/
	/*glPushMatrix();*/
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < frameface.size(); ++i)
	{
		glNormal3d(framefacenrom[i].x, framefacenrom[i].y, framefacenrom[i].z);
		for (int j = 0; j < frameface[i].size(); ++j)
		{
			glVertex3d(frameface[i][j].x, frameface[i][j].y,
				frameface[i][j].z);
		}
	}
	glEnd();

	glBegin(GL_LINES);
	for (int i = 0; i < frameface.size(); ++i)
	{
		//glNormal3d(framefacenrom[i].x, framefacenrom[i].y, framefacenrom[i].z);
		for (int j = 0; j < frameface[i].size() - 1; ++j)
		{
			glVertex3d(frameface[i][j].x, frameface[i][j].y,
				frameface[i][j].z);
			glVertex3d(frameface[i][j+1].x, frameface[i][j+1].y,
				frameface[i][j+1].z);
		}
		glVertex3d(frameface[i][frameface[i].size() - 1].x, frameface[i][frameface[i].size() - 1].y,
			frameface[i][frameface[i].size() - 1].z);
		glVertex3d(frameface[i][0].x, frameface[i][0].y,
			frameface[i][0].z);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	/*glPopMatrix();*/
	/*glPopAttrib();*/
}

void QGLViewerWidget::draw_mesh_solidflat_sphere() const
{
	GLfloat mat[] = { 0.5f, 0.5f, 0.5f, 0.1f };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < boundary_face_v1.size(); ++i)
	{
		glNormal3d(normal_boundary_face_v1[i].x, normal_boundary_face_v1[i].y, normal_boundary_face_v1[i].z);
		for (int j = 0; j < boundary_face_v1[i].size(); ++j)
		{
			glVertex3d(boundary_face_v1[i][j].x, boundary_face_v1[i][j].y,
				boundary_face_v1[i][j].z);
		}
	}
	glEnd();
}

void QGLViewerWidget::draw_quad()
{
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < quad->faces_.size(); ++i)
	{
		glNormal3d(quad->normal[i].x, quad->normal[i].y, quad->normal[i].z);
		for (int j = 0; j < 4; ++j)
		{
			V3f v = quad->vertices_[quad->faces_[i].vs_f[j]];
			glVertex3d(v.x, v.y, v.z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_quad1()
{
	GLfloat color[] = { 0.5,0.0,0.0,1.0 };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	glEnable(GL_LIGHTING);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < quad1->faces_.size(); ++i)
	{
		glNormal3d(quad1->normal[i].x, quad1->normal[i].y, quad1->normal[i].z);
		for (int j = 0; j < 4; ++j)
		{
			V3f v = quad1->vertices_[quad1->faces_[i].vs_f[j]];
			glVertex3d(v.x, v.y, v.z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::draw_quadlines()
{
	glLineWidth(1.0);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	for (int i = 0; i < drawedge.size(); ++i)
	{
		V3f v1 = quad->vertices_[quad->edges_[drawedge[i]].from_h];
		V3f v2 = quad->vertices_[quad->edges_[drawedge[i]].to_h];
		glVertex3d(v1.x, v1.y, v1.z);
		glVertex3d(v2.x, v2.y, v2.z);
	}
	/*for (int i = 0; i < quad->faces_.size(); ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			V3f v = quad->vertices_[quad->faces_[i].vs_f[j]];
			glVertex3d(v.x, v.y, v.z);
		}
	}*/
	glEnd();
}

void QGLViewerWidget::draw_quadlines1()
{
	glLineWidth(0.005);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	for (int i = 0; i < drawedge.size(); ++i)
	{
		V3f v1 = quad->vertices_[quad->edges_[drawedge[i]].from_h];
		V3f v2 = quad->vertices_[quad->edges_[drawedge[i]].to_h];
		glVertex3d(v1.x, v1.y, v1.z);
		glVertex3d(v2.x, v2.y, v2.z);
	}
	/*for (int i = 0; i < quad->faces_.size(); ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			V3f v = quad->vertices_[quad->faces_[i].vs_f[j]];
			glVertex3d(v.x, v.y, v.z);
		}
	}*/
	glEnd();
}

void QGLViewerWidget::draw_quadpickv()
{
	glPointSize(10);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (std::vector<VertexHandle>::iterator v_it = quadpick.begin(); v_it != quadpick.end(); ++v_it)
	{
		V3f v = quad->vertices_[*v_it];
		glVertex3d(v.x, v.y, v.z);
	}
	glEnd();
}

void QGLViewerWidget::draw_quadv()
{
	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (std::vector<V3f>::iterator v_it = quad->vertices_.begin(); v_it != quad->vertices_.end(); ++v_it)
	{
		glVertex3d((*v_it).x, (*v_it).y, (*v_it).z);
	}
	glEnd();
}

void QGLViewerWidget::drawframequad()
{
	GLfloat color[] = { 0.5,0.5,0.5,0.1 };
	GLfloat shine[] = { 120.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//���߽���
	glBegin(GL_QUADS);
	for (int i = 0; i < quadframe->faces_.size(); ++i)
	{
		glNormal3d(quadframe->normal[i].x, quadframe->normal[i].y, quadframe->normal[i].z);
		for (int j = 0; j < 4; ++j)
		{
			V3f v = quadframe->vertices_[quadframe->faces_[i].vs_f[j]];
			glVertex3d(v.x, v.y, v.z);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void QGLViewerWidget::drawcurvepoint()
{
	glPointSize(10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < curve_v.size(); ++i)
	{
		for (int j = 0; j < curve_v[i].size(); ++j)
			glVertex3d(curve_v[i][j].x, curve_v[i][j].y, curve_v[i][j].z);
	}
	glEnd();
}

void QGLViewerWidget::drawcurve()
{
	glLineWidth(1.0);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	for (int j = 0; j < curve_v.size(); ++j)
	{
		V3f v1;
		V3f v2;
		int num = curve_v[j].size();
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < num - 1; i++)
		{
			v1 = curve_v[j][i];
			v2 = curve_v[j][i + 1];
			glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
		}
		glEnd();
	}
}

void QGLViewerWidget::drawbsvface()
{
	for (int bf_it = 0; bf_it < bf.size(); ++bf_it)
	{
		GLfloat temp[] = { 171.0 / 255,149.0 / 255,124.0 / 255,1.0 };
		GLfloat shine[] = { 120.0f };
		int beisurvsize = bf[bf_it].bei_surv_dat.size();
		for (int i = 0; i < beisurvsize; ++i)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, temp);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, temp);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, temp);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

			glBegin(GL_QUADS);
			FaceHandle f = bf[bf_it].biaojiface[i];
			V3f center = bf[bf_it].bei_surv_center[i];
			for (int j = 0; j < bf[bf_it].bei_surv_dat[i].size(); ++j)
			{
				if (std::find(pickdrawface.begin(), pickdrawface.end(), f) != pickdrawface.end())
					glNormal3d(1 * bf[bf_it].nor_bei_surv_dat[i][j].x, 1 * bf[bf_it].nor_bei_surv_dat[i][j].y, 1 * bf[bf_it].nor_bei_surv_dat[i][j].z);
				else
					glNormal3d(-1 * bf[bf_it].nor_bei_surv_dat[i][j].x, -1 * bf[bf_it].nor_bei_surv_dat[i][j].y, -1 * bf[bf_it].nor_bei_surv_dat[i][j].z);
				for (int k = 0; k < bf[bf_it].bei_surv_dat[i][j].size(); ++k)
				{
					glVertex3d(bf[bf_it].bei_surv_dat[i][j][k].x, bf[bf_it].bei_surv_dat[i][j][k].y,
						bf[bf_it].bei_surv_dat[i][j][k].z);
				}
			}
			glEnd();
		}
	}
}

void QGLViewerWidget::draw_small_line()
{
	//glColor3d(1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	//glBegin(GL_TRIANGLES);
	for (int i = 0; i < trianglev.size(); ++i)
	{
		glBegin(GL_LINE_LOOP);
		draw_small_triangle(trianglev[i]);
		glEnd();
	}

	//glColor3d(0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	//glBegin(GL_QUADS);
	for (int i = 0; i < diamondv.size(); ++i)
	{
		glBegin(GL_LINE_LOOP);
		draw_small_diamond(diamondv[i]);
		glEnd();
	}

	//glColor3d(1.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	//glBegin(GL_QUADS);
	for (int i = 0; i < squarev.size(); ++i)
	{
		glBegin(GL_LINE_LOOP);
		draw_small_square(squarev[i]);
		glEnd();
	}

	//glColor3d(1.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	//glBegin(GL_QUADS);
	for (int i = 0; i < circlev.size(); ++i)
	{
		glBegin(GL_LINE_LOOP);
		draw_small_circle(circlev[i]);
		glEnd();
	}
}

void QGLViewerWidget::draw_small_face()
{
	glLineWidth(1);
	//glColor3d(1.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < trianglev.size(); ++i)
	{
		draw_small_triangle(trianglev[i]);
	}
	glEnd();

	glLineWidth(1);
	//glColor3d(0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	for (int i = 0; i < diamondv.size(); ++i)
	{
		draw_small_diamond(diamondv[i]);
	}
	glEnd();

	glLineWidth(1);
	//glColor3d(1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	for (int i = 0; i < squarev.size(); ++i)
	{
		draw_small_square(squarev[i]);
	}
	glEnd();

	glLineWidth(1);
	//glColor3d(1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < circlev.size(); ++i)
	{
		glBegin(GL_POLYGON);
		draw_small_circle(circlev[i]);
		glEnd();
	}
}

void QGLViewerWidget::draw_small_face1()
{
	glColor3f(1.0, 1.0, 1.0);
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		glBegin(GL_POLYGON);
		draw_small_circle1(*v_it);
		glEnd();
	}
}

void QGLViewerWidget::draw_small_triangle(V3f v)
{
	glVertex3d(v.x - 0.1075, v.y - 0.0625, v.z + 0.01);
	glVertex3d(v.x, v.y + 0.125, v.z + 0.01);
	glVertex3d(v.x + 0.1075, v.y - 0.0625, v.z + 0.01);
}

void QGLViewerWidget::draw_small_diamond(V3f v)
{
	glVertex3d(v.x - 0.1075, v.y, v.z + 0.01);
	glVertex3d(v.x, v.y + 0.125, v.z + 0.01);
	glVertex3d(v.x + 0.1075, v.y, v.z + 0.01);
	glVertex3d(v.x, v.y - 0.125, v.z + 0.01);
}

void QGLViewerWidget::draw_small_square(V3f v)
{
	glVertex3d(v.x - 0.1075, v.y + 0.1075, v.z + 0.01);
	glVertex3d(v.x + 0.1075, v.y + 0.1075, v.z + 0.01);
	glVertex3d(v.x + 0.1075, v.y - 0.1075, v.z + 0.01);
	glVertex3d(v.x - 0.1075, v.y - 0.1075, v.z + 0.01);
}

void QGLViewerWidget::draw_small_circle(V3f v)
{
	double PI = 3.1415926;
	int n = 30;
	double R = 0.1075;
	for (int i = 0; i < n; i++)
	{
		glVertex3d(v.x + R * cos(2 * PI*i / n), v.y + R * sin(2 * PI*i / n), v.z + 0.01);  //���嶥��
	}
}

void QGLViewerWidget::draw_small_triangle1(V3f v)
{
	glVertex3d(v.x + 0.01, v.y - 0.05375, v.z - 0.03125);
	glVertex3d(v.x + 0.01, v.y, v.z + 0.0625);
	glVertex3d(v.x + 0.01, v.y + 0.05375, v.z - 0.03125);
	/*glVertex3d(v.x + 0.01, v.y - 0.03125, v.z - 0.05375);
	glVertex3d(v.x + 0.01, v.y + 0.0625, v.z);
	glVertex3d(v.x + 0.01, v.y - 0.03125, v.z + 0.05375);*/
}

void QGLViewerWidget::draw_small_diamond1(V3f v)
{
	glVertex3d(v.x + 0.01, v.y - 0.05375, v.z);
	glVertex3d(v.x + 0.01, v.y, v.z + 0.0625);
	glVertex3d(v.x + 0.01, v.y + 0.05375, v.z);
	glVertex3d(v.x + 0.01, v.y, v.z - 0.0625);
	/*glVertex3d(v.x + 0.01, v.y, v.z - 0.05375);
	glVertex3d(v.x + 0.01, v.y + 0.0625, v.z);
	glVertex3d(v.x + 0.01, v.y, v.z + 0.05375);
	glVertex3d(v.x + 0.01, v.y - 0.0625, v.z);*/
}

void QGLViewerWidget::draw_small_square1(V3f v)
{
	glVertex3d(v.x + 0.01, v.y - 0.05375, v.z + 0.05375);
	glVertex3d(v.x + 0.01, v.y + 0.05375, v.z + 0.05375);
	glVertex3d(v.x + 0.01, v.y + 0.05375, v.z - 0.05375);
	glVertex3d(v.x + 0.01, v.y - 0.05375, v.z - 0.05375);
	/*glVertex3d(v.x + 0.01, v.y + 0.05375, v.z - 0.05375);
	glVertex3d(v.x + 0.01, v.y + 0.05375, v.z + 0.05375);
	glVertex3d(v.x + 0.01, v.y - 0.05375, v.z + 0.05375);
	glVertex3d(v.x + 0.01, v.y - 0.05375, v.z - 0.05375);*/
}

void QGLViewerWidget::draw_small_circle2(V3f v)
{
	double PI = 3.1415926;
	int n = 30;
	double R = 0.05375;
	for (int i = 0; i < n; i++)
	{
		glVertex3d(v.x + 0.01, v.y + R * cos(2 * PI*i / n), v.z + R * sin(2 * PI*i / n));  //���嶥��
		//glVertex3d(v.x + 0.01, v.y + R * sin(2 * PI*i / n), v.z + R * cos(2 * PI*i / n));  //���嶥��
	}
}

void QGLViewerWidget::draw_small_circle1(V3f v)
{
	double PI = 3.1415926;
	int n = 30;
	double R = 0.03;
	for (int i = 0; i < n; i++)
	{
		glVertex3d(v.x + R * cos(2 * PI*i / n), v.y + R * sin(2 * PI*i / n), v.z + 0.01);  //���嶥��
	}
}

void QGLViewerWidget::drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��?
						// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б����?
		lists = glGenLists(128);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�������ÿ���ַ�
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void QGLViewerWidget::drawzimu1()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	char c = 'A';
	for (std::vector<V3f>::iterator v_it = mesh->vertices_.begin(); v_it != mesh->vertices_.end(); ++v_it)
	{
		glRasterPos3f(v_it->x - 0.07, v_it->y - 0.07, v_it->z + 0.02);
		drawString(&c);
		c++;
	}
}

void QGLViewerWidget::drawzimu()
{
	glColor3f(0.0, 0.0, 0.0);
	char c = 'e';
	for (int i = 0; i < trianglev.size(); ++i)
	{
		glRasterPos3f(trianglev[i].x - 0.15, trianglev[i].y - 0.18, trianglev[i].z + 0.1);
		int t = trianglev.size() - i - 1;
		if (t == 7)
			t = 0;
		else if (t == 0)
			t = 7;
		std::string s = c + std::to_string(t);
		drawString(s.c_str());
	}

	glColor3f(0.0, 0.0, 0.0);
	c = 'f';
	for (int i = 0; i < diamondv.size(); ++i)
	{
		glRasterPos3f(diamondv[i].x - 0.15, diamondv[i].y - 0.18, diamondv[i].z + 0.1);
		int t = diamondv.size() - i - 1;
		if (t == 3)
			t = 0;
		else if (t == 0)
			t = 3;
		std::string s = c + std::to_string(t);
		drawString(s.c_str());
	}

	glColor3f(0.0, 0.0, 0.0);
	c = 'i';
	for (int i = 0; i < squarev.size(); ++i)
	{
		glRasterPos3f(squarev[i].x - 0.15, squarev[i].y - 0.18, squarev[i].z + 0.1);
		int t = squarev.size() - i - 1;
		if (t == 3)
			t = 0;
		else if (t == 0)
			t = 3;
		std::string s = c + std::to_string(t);
		drawString(s.c_str());
	}

	glColor3f(0.0, 0.0, 0.0);
	c = 'c';
	for (int i = 0; i < circlev.size(); ++i)
	{
		glRasterPos3f(circlev[i].x - 0.15, circlev[i].y - 0.18, circlev[i].z + 0.1);
		int t = circlev.size() - i - 1;
		if (t == 3)
			t = 0;
		else if (t == 0)
			t = 3;
		std::string s = c + std::to_string(t);
		drawString(s.c_str());
	}
}

void QGLViewerWidget::drawotherline()
{
	glLineWidth(2);
	for (int i = 0; i < mesh->cells_[28].faces_.size(); ++i)
	{
		FaceHandle f = mesh->cells_[28].faces_[i];
		EdgeHandle e1 = mesh->faces_[f].edges_[0];
		EdgeHandle e2 = mesh->faces_[f].edges_[3];
		VertexHandle e1_vfrom = mesh->edges_[e1].from_h;
		VertexHandle e1_vto = mesh->edges_[e1].to_h;
		VertexHandle e2_vfrom = mesh->edges_[e2].from_h;
		VertexHandle e2_vto;
		if (mesh->is_two_v_in_same_edge(e1_vfrom, e2_vfrom))
		{
			e2_vto = mesh->edges_[e2].to_h;
		}
		else
		{
			e2_vfrom = mesh->edges_[e2].to_h;
			e2_vto = mesh->edges_[e2].from_h;
		}
		V3f v1 = (mesh->vertices_[e1_vto] - mesh->vertices_[e1_vfrom]) / 3 + mesh->vertices_[e1_vfrom];
		V3f v2 = (mesh->vertices_[e1_vto] - mesh->vertices_[e1_vfrom]) * 2 / 3 + mesh->vertices_[e1_vfrom];
		V3f v3 = (mesh->vertices_[e2_vto] - mesh->vertices_[e2_vfrom]) / 3 + mesh->vertices_[e2_vfrom];
		V3f v4 = (mesh->vertices_[e2_vto] - mesh->vertices_[e2_vfrom]) * 2 / 3 + mesh->vertices_[e2_vfrom];

		glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z);
		glVertex3d(v3.x, v3.y, v3.z);
		glVertex3d(v2.x, v2.y, v2.z);
		glVertex3d(v4.x, v4.y, v4.z);
		glEnd();

		e1 = mesh->faces_[f].edges_[1];
		e2 = mesh->faces_[f].edges_[2];
		e1_vfrom = mesh->edges_[e1].from_h;
		e1_vto = mesh->edges_[e1].to_h;
		e2_vfrom = mesh->edges_[e2].from_h;
		e2_vto;
		if (mesh->is_two_v_in_same_edge(e1_vfrom, e2_vfrom))
		{
			e2_vto = mesh->edges_[e2].to_h;
		}
		else
		{
			e2_vfrom = mesh->edges_[e2].to_h;
			e2_vto = mesh->edges_[e2].from_h;
		}
		v1 = (mesh->vertices_[e1_vto] - mesh->vertices_[e1_vfrom]) / 3 + mesh->vertices_[e1_vfrom];
		v2 = (mesh->vertices_[e1_vto] - mesh->vertices_[e1_vfrom]) * 2 / 3 + mesh->vertices_[e1_vfrom];
		v3 = (mesh->vertices_[e2_vto] - mesh->vertices_[e2_vfrom]) / 3 + mesh->vertices_[e2_vfrom];
		v4 = (mesh->vertices_[e2_vto] - mesh->vertices_[e2_vfrom]) * 2 / 3 + mesh->vertices_[e2_vfrom];

		glBegin(GL_LINES);
		glVertex3d(v1.x, v1.y, v1.z);
		glVertex3d(v3.x, v3.y, v3.z);
		glVertex3d(v2.x, v2.y, v2.z);
		glVertex3d(v4.x, v4.y, v4.z);
		glEnd();
	}
	for (int i = 0; i < mesh->cells_[28].faces_.size(); ++i)
	{
		FaceHandle f = mesh->cells_[28].faces_[i];
		for (int j = 0; j < mesh->faces_[f].vs_f.size(); ++j)
		{
			VertexHandle vid = mesh->faces_[f].vs_f[j];
			V3f v1;
			if (i == 4 || i == 5)
				v1 = cal_f_v1(f, vid);
			else
				v1 = cal_f_v(f, vid);
			V3f v2;
			FaceHandle opf = mesh->get_opposite_face_in_cell(f, CellHandle(28));
			std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vid];
			for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
			{
				if (!mesh->is_e_in_c(*v_e_it, CellHandle(28)))
					continue;
				else
				{
					if (!mesh->is_e_in_f(*v_e_it, f))
					{
						if (mesh->edges_[*v_e_it].from_h == vid)
						{
							if (i == 4 || i == 5)
								v2 = cal_f_v1(opf, mesh->edges_[*v_e_it].to_h);
							else
								v2 = cal_f_v(opf, mesh->edges_[*v_e_it].to_h);
						}
						else
						{
							if (i == 4 || i == 5)
								v2 = cal_f_v1(opf, mesh->edges_[*v_e_it].from_h);
							else
								v2 = cal_f_v(opf, mesh->edges_[*v_e_it].from_h);
						}
					}
				}
			}
			glBegin(GL_LINES);
			glVertex3d(v1.x, v1.y, v1.z);
			glVertex3d(v2.x, v2.y, v2.z);
			glEnd();
		}
	}
}

V3f QGLViewerWidget::cal_f_v(FaceHandle f, VertexHandle v)
{
	V3f v1 = mesh->vertices_[v];
	V3f v2;
	V3f v3;
	int k = 0;
	for (int i = 0; i < mesh->faces_[f].edges_.size(); ++i)
	{
		EdgeHandle e = mesh->faces_[f].edges_[i];
		if (mesh->edges_[e].from_h == v || mesh->edges_[e].to_h == v)
		{
			if (mesh->edges_[e].from_h == v)
			{
				if (k == 0)
				{
					v2 = mesh->vertices_[mesh->edges_[e].to_h];
					k++;
				}
				else if (k == 1)
				{
					v3 = mesh->vertices_[mesh->edges_[e].to_h];
					k++;
				}
			}
			else
			{
				if (k == 0)
				{
					v2 = mesh->vertices_[mesh->edges_[e].from_h];
					k++;
				}
				else if (k == 1)
				{
					v3 = mesh->vertices_[mesh->edges_[e].from_h];
					k++;
				}
			}
		}
	}
	V3f vv = (v1 + v2 + v3) / 3;
	return vv;
}

V3f QGLViewerWidget::cal_f_v1(FaceHandle f, VertexHandle v)
{
	V3f v1 = mesh->vertices_[v];
	V3f v2;
	V3f v3;
	int k = 0;
	for (int i = 0; i < mesh->faces_[f].edges_.size(); ++i)
	{
		EdgeHandle e = mesh->faces_[f].edges_[i];
		if (mesh->edges_[e].from_h == v || mesh->edges_[e].to_h == v)
		{
			if (mesh->edges_[e].from_h == v)
			{
				if (k == 0)
				{
					v2 = mesh->vertices_[mesh->edges_[e].to_h];
					k++;
				}
				else if (k == 1)
				{
					v3 = mesh->vertices_[mesh->edges_[e].to_h];
					k++;
				}
			}
			else
			{
				if (k == 0)
				{
					v2 = mesh->vertices_[mesh->edges_[e].from_h];
					k++;
				}
				else if (k == 1)
				{
					v3 = mesh->vertices_[mesh->edges_[e].from_h];
					k++;
				}
			}
		}
	}
	V3f v4 = mesh->vertices_[mesh->find_op_vertex(v, f)];
	V3f vv1 = v1 + (v2 - v1) / 3;
	V3f vv2 = v3 + (v4 - v3) / 3;
	V3f vv3 = v1 + (v3 - v1) / 3;
	V3f vv4 = v2 + (v4 - v2) / 3;
	double k1 = (vv1.y - vv2.y) / (vv1.x - vv2.x);
	double b1 = vv1.y - k1 * vv1.x;
	double k2 = (vv3.y - vv4.y) / (vv3.x - vv4.x);
	double b2 = vv3.y - k2 * vv3.x;
	V3f vv;
	vv.x = (b1 - b2) / (k2 - k1);
	vv.y = k1 * vv.x + b1;
	vv.z = vv1.z;
	return vv;
}

void QGLViewerWidget::drawfaceline()
{
	glLineWidth(2);
	glColor3f(0.0, 0.0, 0.0);
	V3f vfrom, vto;
	if (mesh->edges_[90].from_h == VertexHandle(41))
	{
		vfrom = mesh->vertices_[mesh->edges_[90].from_h];
		vto = mesh->vertices_[mesh->edges_[90].to_h];
	}
	else
	{
		vto = mesh->vertices_[mesh->edges_[90].from_h];
		vfrom = mesh->vertices_[mesh->edges_[90].to_h];
	}
	V3f vtriangle = (vto - vfrom) / 3 + vfrom;
	glBegin(GL_LINE_LOOP);
	draw_small_triangle1(vtriangle);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	V3f vcircle = mesh->vertices_[34];
	glBegin(GL_LINE_LOOP);
	draw_small_circle2(vcircle);
	glEnd();

	glLineWidth(1);
	glColor3f(0.0, 0.0, 0.0);
	vfrom, vto;
	if (mesh->edges_[142].from_h == VertexHandle(41))
	{
		vfrom = mesh->vertices_[mesh->edges_[142].from_h];
		vto = mesh->vertices_[mesh->edges_[142].to_h];
	}
	else
	{
		vto = mesh->vertices_[mesh->edges_[142].from_h];
		vfrom = mesh->vertices_[mesh->edges_[142].to_h];
	}
	vtriangle = (vto - vfrom) / 3 + vfrom;
	glBegin(GL_LINE_LOOP);
	draw_small_triangle1(vtriangle);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	V3f vdiamond = cal_f_v(FaceHandle(112), VertexHandle(41));
	glBegin(GL_LINE_LOOP);
	draw_small_diamond1(vdiamond);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	V3f opv = cal_f_v(mesh->get_opposite_face_in_cell(FaceHandle(112), CellHandle(28)), VertexHandle(26));
	V3f vsquare = (opv - vdiamond) / 3 + vdiamond;
	glBegin(GL_LINE_LOOP);
	draw_small_square1(vsquare);
	glEnd();


	glColor3f(0.0, 0.0, 0.0);
	vcircle = mesh->vertices_[41];
	glBegin(GL_LINE_LOOP);
	draw_small_circle2(vcircle);
	glEnd();
}

void QGLViewerWidget::drawfaceface()
{
	glColor3f(1.0, 1.0, 1.0);
	V3f vfrom, vto;
	if (mesh->edges_[90].from_h == VertexHandle(41))
	{
		vfrom = mesh->vertices_[mesh->edges_[90].from_h];
		vto = mesh->vertices_[mesh->edges_[90].to_h];
	}
	else
	{
		vto = mesh->vertices_[mesh->edges_[90].from_h];
		vfrom = mesh->vertices_[mesh->edges_[90].to_h];
	}
	V3f vtriangle = (vto - vfrom) / 3 + vfrom;
	glBegin(GL_TRIANGLES);
	draw_small_triangle1(vtriangle);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	V3f vcircle = mesh->vertices_[34];
	glBegin(GL_POLYGON);
	draw_small_circle2(vcircle);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	vfrom, vto;
	if (mesh->edges_[142].from_h == VertexHandle(41))
	{
		vfrom = mesh->vertices_[mesh->edges_[142].from_h];
		vto = mesh->vertices_[mesh->edges_[142].to_h];
	}
	else
	{
		vto = mesh->vertices_[mesh->edges_[142].from_h];
		vfrom = mesh->vertices_[mesh->edges_[142].to_h];
	}
	vtriangle = (vto - vfrom) / 3 + vfrom;
	glBegin(GL_TRIANGLES);
	draw_small_triangle1(vtriangle);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	V3f vdiamond = cal_f_v(FaceHandle(112), VertexHandle(41));
	glBegin(GL_QUADS);
	draw_small_diamond1(vdiamond);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	V3f opv = cal_f_v(mesh->get_opposite_face_in_cell(FaceHandle(112), CellHandle(28)), VertexHandle(26));
	V3f vsquare = (opv - vdiamond) / 3 + vdiamond;
	glBegin(GL_QUADS);
	draw_small_square1(vsquare);
	glEnd();


	glColor3f(1.0, 1.0, 1.0);
	vcircle = mesh->vertices_[41];
	glBegin(GL_POLYGON);
	draw_small_circle2(vcircle);
	glEnd();
}



/**
 * @description: set root node
 * @param {type}
 * @return {type}
 */
void QGLViewerWidget::setRoot(BMesh::Node *_root) {
	root = _root;
}

/**
 * @description: set root vec
 * @param {type}
 * @return {type}
 */
void QGLViewerWidget::setNodeVec(std::vector<BMesh::Node*>& v) {
	nodeVec = v;
}

/**
 * @description: set root node
 * @param {type}
 * @return {type}
 */
void QGLViewerWidget::drawSkel() {
	BFSTraverse(root);
}

/**
 * @description: BFSTraverse
 * @param {type}
 * @return {type}
 */
void QGLViewerWidget::BFSTraverse(BMesh::Node *root) {
	visited.clear();
	std::queue<BMesh::Node*> empty;
	std::swap(empty, Q);
	
	visited[root->id] = true;
	BFS(root);
	for (int i = 0; i < nodeVec.size(); i++) {
		if (!visited[nodeVec[i]->id]) {
			BFS(nodeVec[i]);
		}
	}
}

/**
 * @description: BFS
 * @param {type}
 * @return {type}
 */
void QGLViewerWidget::BFS(BMesh::Node *root) {
	Q.push(root);
	V3f v1;
	V3f v2;
	glLineWidth(3);

	while (!Q.empty()) {
		BMesh::Node *a = Q.front();
		visited[a->id] = true;
		v1.x = a->root->position.x;
		v1.y = a->root->position.y;
		v1.z = a->root->position.z;
		Q.pop();
		for (int i = 0; i < a->children.size(); i++) {
			if (visited[a->children[i]->id] == false) {
				Q.push(a->children[i]);
				v2.x = a->children[i]->root->position.x;
				v2.y = a->children[i]->root->position.y;
				v2.z = a->children[i]->root->position.z;
				glBegin(GL_LINES);
#if 1
				GLfloat color[] = { 0,0,1,0.5 }; // GLfloat color[] = { 0.5,0.5,0.5,0.1 }
				GLfloat shine[] = { 120.0f };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
#endif
				glVertex3d(v1.x, v1.y, v1.z); glVertex3d(v2.x, v2.y, v2.z);
				glEnd();
			}
		}
	}
	glLineWidth(1);
}