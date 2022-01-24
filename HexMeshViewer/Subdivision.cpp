#include"Subdivision.h"
#include "BezierSolid.h"
#include "FileHandler.h"
#include <string>

//-----Split-----//
HexV3fMesh& Split::Spliting()
{
	uint64_t cell_num = mesh->cells_.size();
	std::vector<double> material_distribution1(8 * cell_num, 1);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex[mesh_vex_num] = newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
		}
		//再插入每条边的中点 暴力插入 以免顺序错误
		//第一条边的中点
		V3f edge_mid_1;
		edge_mid_1 = (mesh->vertices_[mesh->cells_[i].vertex_[0]] + mesh->vertices_[mesh->cells_[i].vertex_[1]])*1.0 / 2 * 1.0;
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2;
		edge_mid_2 = (mesh->vertices_[mesh->cells_[i].vertex_[1]] + mesh->vertices_[mesh->cells_[i].vertex_[5]])*1.0 / 2 * 1.0;
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3;
		edge_mid_3 = (mesh->vertices_[mesh->cells_[i].vertex_[5]] + mesh->vertices_[mesh->cells_[i].vertex_[4]])*1.0 / 2 * 1.0;
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4;
		edge_mid_4 = (mesh->vertices_[mesh->cells_[i].vertex_[4]] + mesh->vertices_[mesh->cells_[i].vertex_[0]])*1.0 / 2 * 1.0;
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5;
		edge_mid_5 = (mesh->vertices_[mesh->cells_[i].vertex_[3]] + mesh->vertices_[mesh->cells_[i].vertex_[2]])*1.0 / 2 * 1.0;
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6;
		edge_mid_6 = (mesh->vertices_[mesh->cells_[i].vertex_[3]] + mesh->vertices_[mesh->cells_[i].vertex_[7]])*1.0 / 2 * 1.0;
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7;
		edge_mid_7 = (mesh->vertices_[mesh->cells_[i].vertex_[6]] + mesh->vertices_[mesh->cells_[i].vertex_[7]])*1.0 / 2 * 1.0;
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8;
		edge_mid_8 = (mesh->vertices_[mesh->cells_[i].vertex_[2]] + mesh->vertices_[mesh->cells_[i].vertex_[6]])*1.0 / 2 * 1.0;
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9;
		edge_mid_9 = (mesh->vertices_[mesh->cells_[i].vertex_[1]] + mesh->vertices_[mesh->cells_[i].vertex_[2]])*1.0 / 2 * 1.0;
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10;
		edge_mid_10 = (mesh->vertices_[mesh->cells_[i].vertex_[5]] + mesh->vertices_[mesh->cells_[i].vertex_[6]])*1.0 / 2 * 1.0;
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11;
		edge_mid_11 = (mesh->vertices_[mesh->cells_[i].vertex_[0]] + mesh->vertices_[mesh->cells_[i].vertex_[3]])*1.0 / 2 * 1.0;
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12;
		edge_mid_12 = (mesh->vertices_[mesh->cells_[i].vertex_[4]] + mesh->vertices_[mesh->cells_[i].vertex_[7]])*1.0 / 2 * 1.0;
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);

		//再插入每个面的中点
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid;
			face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]]
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]]
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]]
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]])*1.0 / 4 * 1.0;
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}

		//插入单元的内心
		V3f cell_mid;
		for (int j = 0; j < 8; j++)
		{
			cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
		}
		cell_mid = cell_mid / 8.0;
		temp_vex[26] = newmesh->add_vertex(cell_mid);

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		if (material_distribution[i] != 1)
		{
			for (int t = 0; t < 8; t++) {
				material_distribution1[newmesh->add_cell(sub_cell[t])] = 0.0001;
			}
		}
		else
		{
			for (int t = 0; t < 8; t++) {
				newmesh->add_cell(sub_cell[t]);
			}
		}
	}
	material_distribution = material_distribution1;
	return *newmesh;
}

HexV3fMesh& Split::Spliting_crease()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		std::vector<V3f>::iterator v_id;

		// 傻逼了 直接for不就好了

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]])) == newmesh->vertices_.end())
			{
				temp_vex[mesh_vex_num] = newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
			}
			else
			{
				temp_vex[mesh_vex_num] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
			}
		}
		//再插入每条边的中点 暴力插入 以免顺序错误
		//第一条边的中点
		V3f edge_mid_1;
		edge_mid_1.x = (mesh->vertices_[mesh->cells_[i].vertex_[0]].x + mesh->vertices_[mesh->cells_[i].vertex_[1]].x)*1.0 / 2 * 1.0;
		edge_mid_1.y = (mesh->vertices_[mesh->cells_[i].vertex_[0]].y + mesh->vertices_[mesh->cells_[i].vertex_[1]].y)*1.0 / 2 * 1.0;
		edge_mid_1.z = (mesh->vertices_[mesh->cells_[i].vertex_[0]].z + mesh->vertices_[mesh->cells_[i].vertex_[1]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_1)) == newmesh->vertices_.end())
		{
			temp_vex[8] = newmesh->add_vertex(edge_mid_1);
		}
		else
		{
			temp_vex[8] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		//判断该点是否在折痕上
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[0]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[1]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[1];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_1);
		}
		//第二条边的中点
		V3f edge_mid_2;
		edge_mid_2.x = (mesh->vertices_[mesh->cells_[i].vertex_[1]].x + mesh->vertices_[mesh->cells_[i].vertex_[5]].x)*1.0 / 2 * 1.0;
		edge_mid_2.y = (mesh->vertices_[mesh->cells_[i].vertex_[1]].y + mesh->vertices_[mesh->cells_[i].vertex_[5]].y)*1.0 / 2 * 1.0;
		edge_mid_2.z = (mesh->vertices_[mesh->cells_[i].vertex_[1]].z + mesh->vertices_[mesh->cells_[i].vertex_[5]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_2)) == newmesh->vertices_.end())
		{
			temp_vex[9] = newmesh->add_vertex(edge_mid_2);
		}
		else
		{
			temp_vex[9] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[1]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[5]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[2];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_2);
		}
		//第三条边的中点
		V3f edge_mid_3;
		edge_mid_3.x = (mesh->vertices_[mesh->cells_[i].vertex_[5]].x + mesh->vertices_[mesh->cells_[i].vertex_[4]].x)*1.0 / 2 * 1.0;
		edge_mid_3.y = (mesh->vertices_[mesh->cells_[i].vertex_[5]].y + mesh->vertices_[mesh->cells_[i].vertex_[4]].y)*1.0 / 2 * 1.0;
		edge_mid_3.z = (mesh->vertices_[mesh->cells_[i].vertex_[5]].z + mesh->vertices_[mesh->cells_[i].vertex_[4]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_3)) == newmesh->vertices_.end())
		{
			temp_vex[10] = newmesh->add_vertex(edge_mid_3);
		}
		else
		{
			temp_vex[10] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[5]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[4]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[3];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_3);
		}
		//第四条边的中点
		V3f edge_mid_4;
		edge_mid_4.x = (mesh->vertices_[mesh->cells_[i].vertex_[4]].x + mesh->vertices_[mesh->cells_[i].vertex_[0]].x)*1.0 / 2 * 1.0;
		edge_mid_4.y = (mesh->vertices_[mesh->cells_[i].vertex_[4]].y + mesh->vertices_[mesh->cells_[i].vertex_[0]].y)*1.0 / 2 * 1.0;
		edge_mid_4.z = (mesh->vertices_[mesh->cells_[i].vertex_[4]].z + mesh->vertices_[mesh->cells_[i].vertex_[0]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_4)) == newmesh->vertices_.end())
		{
			temp_vex[11] = newmesh->add_vertex(edge_mid_4);
		}
		else
		{
			temp_vex[11] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[4]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[0]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[4];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_4);
		}
		//第五条边的中点
		V3f edge_mid_5;
		edge_mid_5.x = (mesh->vertices_[mesh->cells_[i].vertex_[3]].x + mesh->vertices_[mesh->cells_[i].vertex_[2]].x)*1.0 / 2 * 1.0;
		edge_mid_5.y = (mesh->vertices_[mesh->cells_[i].vertex_[3]].y + mesh->vertices_[mesh->cells_[i].vertex_[2]].y)*1.0 / 2 * 1.0;
		edge_mid_5.z = (mesh->vertices_[mesh->cells_[i].vertex_[3]].z + mesh->vertices_[mesh->cells_[i].vertex_[2]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_5)) == newmesh->vertices_.end())
		{
			temp_vex[12] = newmesh->add_vertex(edge_mid_5);
		}
		else
		{
			temp_vex[12] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[3]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[2]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[5];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_5);
		}
		//第六条边的中点
		V3f edge_mid_6;
		edge_mid_6.x = (mesh->vertices_[mesh->cells_[i].vertex_[3]].x + mesh->vertices_[mesh->cells_[i].vertex_[7]].x)*1.0 / 2 * 1.0;
		edge_mid_6.y = (mesh->vertices_[mesh->cells_[i].vertex_[3]].y + mesh->vertices_[mesh->cells_[i].vertex_[7]].y)*1.0 / 2 * 1.0;
		edge_mid_6.z = (mesh->vertices_[mesh->cells_[i].vertex_[3]].z + mesh->vertices_[mesh->cells_[i].vertex_[7]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_6)) == newmesh->vertices_.end())
		{
			temp_vex[13] = newmesh->add_vertex(edge_mid_6);
		}
		else
		{
			temp_vex[13] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[3]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[7]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[6];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_6);
		}
		//第七条边的中点
		V3f edge_mid_7;
		edge_mid_7.x = (mesh->vertices_[mesh->cells_[i].vertex_[6]].x + mesh->vertices_[mesh->cells_[i].vertex_[7]].x)*1.0 / 2 * 1.0;
		edge_mid_7.y = (mesh->vertices_[mesh->cells_[i].vertex_[6]].y + mesh->vertices_[mesh->cells_[i].vertex_[7]].y)*1.0 / 2 * 1.0;
		edge_mid_7.z = (mesh->vertices_[mesh->cells_[i].vertex_[6]].z + mesh->vertices_[mesh->cells_[i].vertex_[7]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_7)) == newmesh->vertices_.end())
		{
			temp_vex[14] = newmesh->add_vertex(edge_mid_7);
		}
		else
		{
			temp_vex[14] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[6]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[7]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[7];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_7);
		}
		//第八条边的中点
		V3f edge_mid_8;
		edge_mid_8.x = (mesh->vertices_[mesh->cells_[i].vertex_[2]].x + mesh->vertices_[mesh->cells_[i].vertex_[6]].x)*1.0 / 2 * 1.0;
		edge_mid_8.y = (mesh->vertices_[mesh->cells_[i].vertex_[2]].y + mesh->vertices_[mesh->cells_[i].vertex_[6]].y)*1.0 / 2 * 1.0;
		edge_mid_8.z = (mesh->vertices_[mesh->cells_[i].vertex_[2]].z + mesh->vertices_[mesh->cells_[i].vertex_[6]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_8)) == newmesh->vertices_.end())
		{
			temp_vex[15] = newmesh->add_vertex(edge_mid_8);
		}
		else
		{
			temp_vex[15] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[2]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[6]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[8];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_8);
		}
		//第九条边的中点
		V3f edge_mid_9;
		edge_mid_9.x = (mesh->vertices_[mesh->cells_[i].vertex_[1]].x + mesh->vertices_[mesh->cells_[i].vertex_[2]].x)*1.0 / 2 * 1.0;
		edge_mid_9.y = (mesh->vertices_[mesh->cells_[i].vertex_[1]].y + mesh->vertices_[mesh->cells_[i].vertex_[2]].y)*1.0 / 2 * 1.0;
		edge_mid_9.z = (mesh->vertices_[mesh->cells_[i].vertex_[1]].z + mesh->vertices_[mesh->cells_[i].vertex_[2]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_9)) == newmesh->vertices_.end())
		{
			temp_vex[16] = newmesh->add_vertex(edge_mid_9);
		}
		else
		{
			temp_vex[16] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[1]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[2]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[9];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_9);
		}
		//第十条边的中点
		V3f edge_mid_10;
		edge_mid_10.x = (mesh->vertices_[mesh->cells_[i].vertex_[5]].x + mesh->vertices_[mesh->cells_[i].vertex_[6]].x)*1.0 / 2 * 1.0;
		edge_mid_10.y = (mesh->vertices_[mesh->cells_[i].vertex_[5]].y + mesh->vertices_[mesh->cells_[i].vertex_[6]].y)*1.0 / 2 * 1.0;
		edge_mid_10.z = (mesh->vertices_[mesh->cells_[i].vertex_[5]].z + mesh->vertices_[mesh->cells_[i].vertex_[6]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_10)) == newmesh->vertices_.end())
		{
			temp_vex[17] = newmesh->add_vertex(edge_mid_10);
		}
		else
		{
			temp_vex[17] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[5]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[6]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[10];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_10);
		}
		//第十一条边的中点
		V3f edge_mid_11;
		edge_mid_11.x = (mesh->vertices_[mesh->cells_[i].vertex_[0]].x + mesh->vertices_[mesh->cells_[i].vertex_[3]].x)*1.0 / 2 * 1.0;
		edge_mid_11.y = (mesh->vertices_[mesh->cells_[i].vertex_[0]].y + mesh->vertices_[mesh->cells_[i].vertex_[3]].y)*1.0 / 2 * 1.0;
		edge_mid_11.z = (mesh->vertices_[mesh->cells_[i].vertex_[0]].z + mesh->vertices_[mesh->cells_[i].vertex_[3]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_11)) == newmesh->vertices_.end())
		{
			temp_vex[18] = newmesh->add_vertex(edge_mid_11);
		}
		else
		{
			temp_vex[18] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[0]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[3]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[11];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_11);
		}
		//第十二条边的中点
		V3f edge_mid_12;
		edge_mid_12.x = (mesh->vertices_[mesh->cells_[i].vertex_[4]].x + mesh->vertices_[mesh->cells_[i].vertex_[7]].x)*1.0 / 2 * 1.0;
		edge_mid_12.y = (mesh->vertices_[mesh->cells_[i].vertex_[4]].y + mesh->vertices_[mesh->cells_[i].vertex_[7]].y)*1.0 / 2 * 1.0;
		edge_mid_12.z = (mesh->vertices_[mesh->cells_[i].vertex_[4]].z + mesh->vertices_[mesh->cells_[i].vertex_[7]].z)*1.0 / 2 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), edge_mid_12)) == newmesh->vertices_.end())
		{
			temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		}
		else
		{
			temp_vex[19] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
		if ((std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[4]]) != v_crease.end()) &&
			(std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[mesh->cells_[i].vertex_[7]]) != v_crease.end()))
		{
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[12];
			if (ef.size() > 2)
				v_crease.push_back(edge_mid_12);
		}
		//再插入每个面的中点
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid;
			face_mid.x = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]].x
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]].x
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]].x
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]].x)*1.0 / 4 * 1.0;
			face_mid.y = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]].y
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]].y
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]].y
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]].y)*1.0 / 4 * 1.0;
			face_mid.z = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]].z
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]].z
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]].z
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]].z)*1.0 / 4 * 1.0;
			if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), face_mid)) == newmesh->vertices_.end())
			{
				temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
			}
			else
			{
				temp_vex[20 + mesh_face_num] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
			}
		}
		//插入单元的内心
		V3f cell_mid;
		double x_sum = 0;  double y_sum = 0;  double z_sum = 0;
		for (int j = 0; j < 8; j++)
		{
			x_sum += mesh->vertices_[mesh->cells_[i].vertex_[j]].x;
			y_sum += mesh->vertices_[mesh->cells_[i].vertex_[j]].y;
			z_sum += mesh->vertices_[mesh->cells_[i].vertex_[j]].z;
		}
		cell_mid.x = x_sum * 1.0 / 8 * 1.0;
		cell_mid.y = y_sum * 1.0 / 8 * 1.0;
		cell_mid.z = z_sum * 1.0 / 8 * 1.0;
		if ((v_id = std::find(newmesh->vertices_.begin(), newmesh->vertices_.end(), cell_mid)) == newmesh->vertices_.end())
		{
			temp_vex[26] = newmesh->add_vertex(cell_mid);
		}
		else
		{
			temp_vex[26] = VertexHandle(abs(std::distance(newmesh->vertices_.begin(), v_id)));
		}
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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

/**
 * @description: general averaging
 */
HexV3fMesh& Average::Averaging()
{
	std::vector<V3f> changed_vex;
	changed_vex.resize(mesh->vertices_.size());
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		if (mesh->is_bdy(VertexHandle(vex_num)))
		{
			std::set<FaceHandle, compare_OVM> neighbor_vf = mesh->neighbor_v_f[vex_num];
			std::vector<FaceHandle> bdy_face;
			std::set<FaceHandle, compare_OVM>::iterator vf_id;
			for (vf_id = neighbor_vf.begin(); vf_id != neighbor_vf.end(); vf_id++)
			{
				if (mesh->is_bdy(*vf_id))
				{
					bdy_face.push_back(*vf_id);
				}
			}
			int vf_num = bdy_face.size();
			std::vector<V3f> vf_cen(vf_num);
			std::vector<FaceHandle>::iterator bdy_face_id;
			int u = 0;
			for (bdy_face_id = bdy_face.begin(); bdy_face_id != bdy_face.end(); bdy_face_id++)
			{
				std::vector<VertexHandle> vf = mesh->faces_[*bdy_face_id].vs_f;
				vf_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
					+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x) / 4 * 1.0;
				vf_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
					+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y) / 4 * 1.0;
				vf_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
					+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z) / 4 * 1.0;
				u++;
			}
			V3f new_vex;
			for (int vf_cen_num = 0; vf_cen_num < vf_num; vf_cen_num++)
			{
				new_vex.x += vf_cen[vf_cen_num].x;
				new_vex.y += vf_cen[vf_cen_num].y;
				new_vex.z += vf_cen[vf_cen_num].z;
			}
			new_vex.x /= vf_num * 1.0;
			new_vex.y /= vf_num * 1.0;
			new_vex.z /= vf_num * 1.0;
			changed_vex[vex_num] = new_vex;
		}
		else
		{
			std::set<CellHandle, compare_OVM> neighbor_vc = mesh->neighbor_v_c[vex_num];
			std::set<CellHandle, compare_OVM>::iterator vc_id;
			int vf_num = neighbor_vc.size();
			std::vector<V3f> vc_cen(vf_num);
			int u = 0;
			for (vc_id = neighbor_vc.begin(); vc_id != neighbor_vc.end(); vc_id++)
			{
				std::vector<VertexHandle> vf = mesh->cells_[*vc_id].vertex_;
				vc_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
					+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x +
					mesh->vertices_[vf[4]].x + mesh->vertices_[vf[5]].x +
					mesh->vertices_[vf[6]].x + mesh->vertices_[vf[7]].x) / 8 * 1.0;
				vc_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
					+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y +
					mesh->vertices_[vf[4]].y + mesh->vertices_[vf[5]].y +
					mesh->vertices_[vf[6]].y + mesh->vertices_[vf[7]].y) / 8 * 1.0;
				vc_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
					+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z +
					mesh->vertices_[vf[4]].z + mesh->vertices_[vf[5]].z +
					mesh->vertices_[vf[6]].z + mesh->vertices_[vf[7]].z) / 8 * 1.0;
				u++;
			}
			V3f new_vex;
			for (int vf_cen_num = 0; vf_cen_num < vf_num; vf_cen_num++)
			{
				new_vex.x += vc_cen[vf_cen_num].x;
				new_vex.y += vc_cen[vf_cen_num].y;
				new_vex.z += vc_cen[vf_cen_num].z;
			}
			new_vex.x /= vf_num * 1.0;
			new_vex.y /= vf_num * 1.0;
			new_vex.z /= vf_num * 1.0;
			changed_vex[vex_num] = new_vex;
		}
	}
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		mesh->vertices_[vex_num] = changed_vex[vex_num];
	}
	return *mesh;
}

HexV3fMesh& Average::Averaging_creases()
{
	std::vector<V3f> changed_vex;
	changed_vex.resize(mesh->vertices_.size());
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		if (mesh->is_bdy(VertexHandle(vex_num)))
		{
			if (std::find(v_crease.begin(), v_crease.end(), mesh->vertices_[VertexHandle(vex_num)]) != v_crease.end())
			{
				std::vector<EdgeHandle> ve;
				std::set<EdgeHandle, compare_OVM> crease_ve = mesh->neighbor_v[VertexHandle(vex_num)];
				//只要边界的
				for (std::set<EdgeHandle, compare_OVM>::iterator it = crease_ve.begin(); it != crease_ve.end(); it++)
				{
					if (mesh->is_bdy(*it))
					{
						ve.push_back(*it);
					}
				}
				std::vector<V3f> temp_vex;
				for (std::vector<EdgeHandle>::iterator it = ve.begin(); it != ve.end(); it++)
				{
					V3f v;
					v.x = (mesh->vertices_[mesh->edges_[*it].from_h].x + mesh->vertices_[mesh->edges_[*it].to_h].x) / 2;
					v.y = (mesh->vertices_[mesh->edges_[*it].from_h].y + mesh->vertices_[mesh->edges_[*it].to_h].y) / 2;
					v.z = (mesh->vertices_[mesh->edges_[*it].from_h].z + mesh->vertices_[mesh->edges_[*it].to_h].z) / 2;
					temp_vex.push_back(v);
				}
				V3f result_v;
				int num = temp_vex.size();
				for (int temp_vex_id = 0; temp_vex_id < num; temp_vex_id++)
				{
					result_v.x += temp_vex[temp_vex_id].x;
					result_v.y += temp_vex[temp_vex_id].y;
					result_v.z += temp_vex[temp_vex_id].z;
				}
				result_v.x /= num;
				result_v.y /= num;
				result_v.z /= num;
				changed_vex[vex_num] = result_v;
				v_crease.push_back(result_v);
			}
			else if (std::find(v_c_crease.begin(), v_c_crease.end(), mesh->vertices_[VertexHandle(vex_num)]) != v_c_crease.end())
			{
				changed_vex[vex_num] = mesh->vertices_[vex_num];
			}
			else
			{
				std::set<FaceHandle, compare_OVM> neighbor_vf = mesh->neighbor_v_f[vex_num];
				std::vector<FaceHandle> bdy_face;
				std::set<FaceHandle, compare_OVM>::iterator vf_id;
				for (vf_id = neighbor_vf.begin(); vf_id != neighbor_vf.end(); vf_id++)
				{
					if (mesh->is_bdy(*vf_id))
					{
						bdy_face.push_back(*vf_id);
					}
				}
				int vf_num = bdy_face.size();
				std::vector<V3f> vf_cen(vf_num);
				std::vector<FaceHandle>::iterator bdy_face_id;
				int u = 0;
				for (bdy_face_id = bdy_face.begin(); bdy_face_id != bdy_face.end(); bdy_face_id++)
				{
					std::vector<VertexHandle> vf = mesh->faces_[*bdy_face_id].vs_f;
					vf_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
						+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x) / 4;
					vf_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
						+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y) / 4;
					vf_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
						+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z) / 4;
					u++;
				}
				V3f new_vex;
				for (int vf_cen_num = 0; vf_cen_num < vf_num; vf_cen_num++)
				{
					new_vex.x += vf_cen[vf_cen_num].x;
					new_vex.y += vf_cen[vf_cen_num].y;
					new_vex.z += vf_cen[vf_cen_num].z;
				}
				new_vex.x /= vf_num;
				new_vex.y /= vf_num;
				new_vex.z /= vf_num;
				changed_vex[vex_num] = new_vex;
			}
		}
		else
		{
			std::set<CellHandle, compare_OVM> neighbor_vc = mesh->neighbor_v_c[vex_num];
			std::set<CellHandle, compare_OVM>::iterator vc_id;
			int vf_num = neighbor_vc.size();
			std::vector<V3f> vc_cen(vf_num);
			int u = 0;
			for (vc_id = neighbor_vc.begin(); vc_id != neighbor_vc.end(); vc_id++)
			{
				std::vector<VertexHandle> vf = mesh->cells_[*vc_id].vertex_;
				vc_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
					+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x +
					mesh->vertices_[vf[4]].x + mesh->vertices_[vf[5]].x +
					mesh->vertices_[vf[6]].x + mesh->vertices_[vf[7]].x) / 8 * 1.0;
				vc_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
					+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y +
					mesh->vertices_[vf[4]].y + mesh->vertices_[vf[5]].y +
					mesh->vertices_[vf[6]].y + mesh->vertices_[vf[7]].y) / 8 * 1.0;
				vc_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
					+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z +
					mesh->vertices_[vf[4]].z + mesh->vertices_[vf[5]].z +
					mesh->vertices_[vf[6]].z + mesh->vertices_[vf[7]].z) / 8 * 1.0;
				u++;
			}
			V3f new_vex;
			for (int vf_cen_num = 0; vf_cen_num < vf_num; vf_cen_num++)
			{
				new_vex.x += vc_cen[vf_cen_num].x;
				new_vex.y += vc_cen[vf_cen_num].y;
				new_vex.z += vc_cen[vf_cen_num].z;
			}
			new_vex.x /= vf_num * 1.0;
			new_vex.y /= vf_num * 1.0;
			new_vex.z /= vf_num * 1.0;
			changed_vex[vex_num] = new_vex;
		}
	}
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		mesh->vertices_[vex_num] = changed_vex[vex_num];
	}
	return *mesh;
}

HexV3fMesh& Average::Averaging_singular()
{
	std::vector<V3f> changed_vex;
	changed_vex.resize(mesh->vertices_.size());
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		if (mesh->is_bdy(VertexHandle(vex_num)))
		{
			std::set<FaceHandle, compare_OVM> neighbor_vf = mesh->neighbor_v_f[vex_num];
			std::vector<FaceHandle> bdy_face;
			std::set<FaceHandle, compare_OVM>::iterator vf_id;
			for (vf_id = neighbor_vf.begin(); vf_id != neighbor_vf.end(); vf_id++)
			{
				if (mesh->is_bdy(*vf_id))
				{
					bdy_face.push_back(*vf_id);
				}
			}
			std::vector<FaceHandle>::iterator bdy_face_id;
			if (std::find(v3f_singular.begin(), v3f_singular.end(), mesh->vertices_[VertexHandle(vex_num)]) != v3f_singular.end())
			{
				V3f v = mesh->vertices_[VertexHandle(vex_num)];
				std::cout << "奇异点" << mesh->vertices_[VertexHandle(vex_num)].x << " " << mesh->vertices_[VertexHandle(vex_num)].y << " " << mesh->vertices_[VertexHandle(vex_num)].z << std::endl;
				std::set<VertexHandle, compare_OVM> v_bdy;
				for (bdy_face_id = bdy_face.begin(); bdy_face_id != bdy_face.end(); bdy_face_id++)
				{
					std::vector<VertexHandle> temp;
					temp = mesh->faces_[*bdy_face_id].vs_f;
					for (std::vector<VertexHandle>::iterator it = temp.begin(); it != temp.end(); it++)
					{
						v_bdy.insert(*it);
					}
				}
				int v_bdy_size = v_bdy.size();
				std::cout << v_bdy_size << std::endl;
				V3f new_vertex;
				for (std::set<VertexHandle, compare_OVM>::iterator tt = v_bdy.begin(); tt != v_bdy.end(); tt++)
				{
					double coe1 = 1 * 1.0 / ((mesh->val(VertexHandle(vex_num), true)) * (pow(4, 2)));
					//std::cout << "coe1:" << coe1 << std::endl;
					double coe2 = pow(3, (2 - mesh->dim(VertexHandle(vex_num), *tt)));
					//std::cout << "coe2:" << coe2 << std::endl;
					double coe3 = mesh->val(VertexHandle(vex_num), *tt, true);
					//std::cout << "coe3:" << coe3 << std::endl;
					double weight = coe1 * coe2 * coe3;
					//std::cout <<"weight:"<< weight << std::endl;
					new_vertex.x += weight * mesh->vertices_[*tt].x;
					new_vertex.y += weight * mesh->vertices_[*tt].y;
					new_vertex.z += weight * mesh->vertices_[*tt].z;
				}
				std::cout << "处理后的奇异点" << new_vertex.x << " " << new_vertex.y << " " << new_vertex.z << std::endl;
				changed_vex[vex_num] = new_vertex;
				v3f_singular.push_back(new_vertex);
				//经过测试 表面奇异点也根据全局计算
				//std::cout << "奇异点" << mesh->vertices_[VertexHandle(vex_num)].x << " " << mesh->vertices_[VertexHandle(vex_num)].y << " " << mesh->vertices_[VertexHandle(vex_num)].z << std::endl;
				//std::set<VertexHandle, compare_OVM> v_in;
				//for (auto v_in_id = mesh->neighbor_v_c[VertexHandle(vex_num)].begin();v_in_id != mesh->neighbor_v_c[VertexHandle(vex_num)].end();v_in_id++)
				//{
				//	std::vector<VertexHandle> temp;
				//	temp = mesh->cells_[*v_in_id].vertex_;
				//	for (std::vector<VertexHandle>::iterator it = temp.begin();it != temp.end();it++)
				//	{
				//		v_in.insert(*it);
				//	}
				//}
				//int v_in_size = v_in.size();
				//std::cout << v_in_size << std::endl;
				//V3f new_vertex;
				//for (std::set<VertexHandle, compare_OVM>::iterator tt = v_in.begin();tt != v_in.end();tt++)
				//{
				//	double coe1 = 1 * 1.0 / ((mesh->val(VertexHandle(vex_num), false)) * (pow(4, 3)));
				//	//std::cout << "coe1:" << coe1 << std::endl;
				//	double coe2 = pow(3, (3 - mesh->dim(VertexHandle(vex_num), *tt)));
				//	//std::cout << "coe2:" << coe2 << std::endl;
				//	double coe3 = mesh->val(VertexHandle(vex_num), *tt, false);
				//	//std::cout << "coe3:" << coe3 << std::endl;
				//	double weight = coe1 * coe2 * coe3;
				//	//std::cout << "weight:" << weight << std::endl;
				//	new_vertex.x += weight * mesh->vertices_[*tt].x;
				//	new_vertex.y += weight * mesh->vertices_[*tt].y;
				//	new_vertex.z += weight * mesh->vertices_[*tt].z;
				//}
				//std::cout << "处理后的奇异点" << new_vertex.x << " " << new_vertex.y << " " << new_vertex.z << std::endl;
				//changed_vex[vex_num] = new_vertex;
				//v3f_singular.push_back(new_vertex);
			}
			else
			{
				int vf_num = bdy_face.size();
				std::vector<V3f> vf_cen(vf_num);
				int u = 0;
				for (bdy_face_id = bdy_face.begin(); bdy_face_id != bdy_face.end(); bdy_face_id++)
				{
					std::vector<VertexHandle> vf = mesh->faces_[*bdy_face_id].vs_f;
					vf_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
						+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x) / 4 * 1.0;
					vf_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
						+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y) / 4 * 1.0;
					vf_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
						+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z) / 4 * 1.0;
					u++;
				}
				V3f new_vex;
				for (int vf_cen_num = 0; vf_cen_num < vf_num; vf_cen_num++)
				{
					new_vex.x += vf_cen[vf_cen_num].x;
					new_vex.y += vf_cen[vf_cen_num].y;
					new_vex.z += vf_cen[vf_cen_num].z;
				}
				new_vex.x /= vf_num * 1.0;
				new_vex.y /= vf_num * 1.0;
				new_vex.z /= vf_num * 1.0;
				changed_vex[vex_num] = new_vex;
			}
		}
		else
		{
			if (std::find(v3f_singular.begin(), v3f_singular.end(), mesh->vertices_[VertexHandle(vex_num)]) != v3f_singular.end())
			{
				std::cout << "奇异点" << mesh->vertices_[VertexHandle(vex_num)].x << " " << mesh->vertices_[VertexHandle(vex_num)].y << " " << mesh->vertices_[VertexHandle(vex_num)].z << std::endl;
				std::set<VertexHandle, compare_OVM> v_in;
				for (auto v_in_id = mesh->neighbor_v_c[VertexHandle(vex_num)].begin(); v_in_id != mesh->neighbor_v_c[VertexHandle(vex_num)].end(); v_in_id++)
				{
					std::vector<VertexHandle> temp;
					temp = mesh->cells_[*v_in_id].vertex_;
					for (std::vector<VertexHandle>::iterator it = temp.begin(); it != temp.end(); it++)
					{
						v_in.insert(*it);
					}
				}
				int v_in_size = v_in.size();
				std::cout << v_in_size << std::endl;
				V3f new_vertex;
				for (std::set<VertexHandle, compare_OVM>::iterator tt = v_in.begin(); tt != v_in.end(); tt++)
				{
					double coe1 = 1 * 1.0 / ((mesh->val(VertexHandle(vex_num), false)) * (pow(4, 3)));
					//std::cout << "coe1:" << coe1 << std::endl;
					double coe2 = pow(3, (3 - mesh->dim(VertexHandle(vex_num), *tt)));
					//std::cout << "coe2:" << coe2 << std::endl;
					double coe3 = mesh->val(VertexHandle(vex_num), *tt, false);
					//std::cout << "coe3:" << coe3 << std::endl;
					double weight = coe1 * coe2 * coe3;
					//std::cout << "weight:" << weight << std::endl;
					new_vertex.x += weight * mesh->vertices_[*tt].x;
					new_vertex.y += weight * mesh->vertices_[*tt].y;
					new_vertex.z += weight * mesh->vertices_[*tt].z;
				}
				std::cout << "处理后的奇异点" << new_vertex.x << " " << new_vertex.y << " " << new_vertex.z << std::endl;
				changed_vex[vex_num] = new_vertex;
				v3f_singular.push_back(new_vertex);
			}
			else
			{
				std::set<CellHandle, compare_OVM> neighbor_vc = mesh->neighbor_v_c[vex_num];
				std::set<CellHandle, compare_OVM>::iterator vc_id;
				int vf_num = neighbor_vc.size();
				std::vector<V3f> vc_cen(vf_num);
				int u = 0;
				for (vc_id = neighbor_vc.begin(); vc_id != neighbor_vc.end(); vc_id++)
				{
					std::vector<VertexHandle> vf = mesh->cells_[*vc_id].vertex_;
					vc_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
						+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x +
						mesh->vertices_[vf[4]].x + mesh->vertices_[vf[5]].x +
						mesh->vertices_[vf[6]].x + mesh->vertices_[vf[7]].x) / 8 * 1.0;
					vc_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
						+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y +
						mesh->vertices_[vf[4]].y + mesh->vertices_[vf[5]].y +
						mesh->vertices_[vf[6]].y + mesh->vertices_[vf[7]].y) / 8 * 1.0;
					vc_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
						+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z +
						mesh->vertices_[vf[4]].z + mesh->vertices_[vf[5]].z +
						mesh->vertices_[vf[6]].z + mesh->vertices_[vf[7]].z) / 8 * 1.0;
					u++;
				}
				V3f new_vex;
				for (int vf_cen_num = 0; vf_cen_num < vf_num; vf_cen_num++)
				{
					new_vex.x += vc_cen[vf_cen_num].x;
					new_vex.y += vc_cen[vf_cen_num].y;
					new_vex.z += vc_cen[vf_cen_num].z;
				}
				new_vex.x /= vf_num * 1.0;
				new_vex.y /= vf_num * 1.0;
				new_vex.z /= vf_num * 1.0;
				changed_vex[vex_num] = new_vex;
			}
		}
	}
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		mesh->vertices_[vex_num] = changed_vex[vex_num];
	}
	return *mesh;
}

HexV3fMesh& Average::Averaging_test()
{
	std::vector<V3f> changed_vex;
	changed_vex.resize(mesh->vertices_.size());
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		/*if (mesh->is_bdy(VertexHandle(vex_num)))
		{
			std::set<FaceHandle, compare_OVM> neighbor_vf = mesh->neighbor_v_f[vex_num];
			std::vector<FaceHandle> bdy_face;
			std::set<FaceHandle, compare_OVM>::iterator vf_id;
			for (vf_id = neighbor_vf.begin();vf_id != neighbor_vf.end();vf_id++)
			{
				if (mesh->is_bdy(*vf_id))
				{
					bdy_face.push_back(*vf_id);
				}
			}
			int vf_num = bdy_face.size();
			std::vector<V3f> vf_cen(vf_num);
			std::vector<FaceHandle>::iterator bdy_face_id;
			int u = 0;
			for (bdy_face_id = bdy_face.begin();bdy_face_id != bdy_face.end();bdy_face_id++)
			{
				std::vector<VertexHandle> vf = mesh->faces_[*bdy_face_id].vs_f;
				vf_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
					+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x) / 4 * 1.0;
				vf_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
					+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y) / 4 * 1.0;
				vf_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
					+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z) / 4 * 1.0;
				u++;
			}
			V3f new_vex;
			for (int vf_cen_num = 0;vf_cen_num < vf_num;vf_cen_num++)
			{
				new_vex.x += vf_cen[vf_cen_num].x;
				new_vex.y += vf_cen[vf_cen_num].y;
				new_vex.z += vf_cen[vf_cen_num].z;
			}
			new_vex.x /= vf_num*1.0;
			new_vex.y /= vf_num*1.0;
			new_vex.z /= vf_num*1.0;
			changed_vex[vex_num] = new_vex;
		}
		else
		{*/
		std::set<CellHandle, compare_OVM> neighbor_vc = mesh->neighbor_v_c[vex_num];
		std::set<CellHandle, compare_OVM>::iterator vc_id;
		int vf_num = neighbor_vc.size();
		std::vector<V3f> vc_cen(vf_num);
		int u = 0;
		for (vc_id = neighbor_vc.begin(); vc_id != neighbor_vc.end(); vc_id++)
		{
			std::vector<VertexHandle> vf = mesh->cells_[*vc_id].vertex_;
			vc_cen[u].x = (mesh->vertices_[vf[0]].x + mesh->vertices_[vf[1]].x
				+ mesh->vertices_[vf[2]].x + mesh->vertices_[vf[3]].x +
				mesh->vertices_[vf[4]].x + mesh->vertices_[vf[5]].x +
				mesh->vertices_[vf[6]].x + mesh->vertices_[vf[7]].x) / 8 * 1.0;
			vc_cen[u].y = (mesh->vertices_[vf[0]].y + mesh->vertices_[vf[1]].y
				+ mesh->vertices_[vf[2]].y + mesh->vertices_[vf[3]].y +
				mesh->vertices_[vf[4]].y + mesh->vertices_[vf[5]].y +
				mesh->vertices_[vf[6]].y + mesh->vertices_[vf[7]].y) / 8 * 1.0;
			vc_cen[u].z = (mesh->vertices_[vf[0]].z + mesh->vertices_[vf[1]].z
				+ mesh->vertices_[vf[2]].z + mesh->vertices_[vf[3]].z +
				mesh->vertices_[vf[4]].z + mesh->vertices_[vf[5]].z +
				mesh->vertices_[vf[6]].z + mesh->vertices_[vf[7]].z) / 8 * 1.0;
			u++;
		}
		V3f new_vex;
		for (int vf_cen_num = 0; vf_cen_num < vf_num; vf_cen_num++)
		{
			new_vex.x += vc_cen[vf_cen_num].x;
			new_vex.y += vc_cen[vf_cen_num].y;
			new_vex.z += vc_cen[vf_cen_num].z;
		}
		new_vex.x /= vf_num * 1.0;
		new_vex.y /= vf_num * 1.0;
		new_vex.z /= vf_num * 1.0;
		changed_vex[vex_num] = new_vex;
	}
	//}
	for (uint64_t vex_num = 0; vex_num < mesh->vertices_.size(); vex_num++)
	{
		mesh->vertices_[vex_num] = changed_vex[vex_num];
	}
	return *mesh;
}

//-----Subdivision-----//
HexV3fMesh& Subdivision::Subdivising()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex[mesh_vex_num] = newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_edge_mid(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_edge_mid(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_edge_mid(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_edge_mid(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_edge_mid(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_edge_mid(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_edge_mid(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_edge_mid(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_edge_mid(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_edge_mid(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_edge_mid(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_edge_mid(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		for (int t = 0; t < 8; t++) {
			newmesh->add_cell(sub_cell[t]);
		}
	}
	return *newmesh;
}

V3f Subdivision::cal_cell_mid(const int cell_num)
{
	V3f cell_mid;
	std::vector<V3f> cmp_vertex;
	std::vector<V3f> cmq_vertex;
	if (mesh->is_bdy(CellHandle(cell_num)))
	{
		////加入p点集合
		//std::vector<VertexHandle> cm_ver = mesh->cells_[cell_num].vertex_;
		//for (std::vector<VertexHandle>::iterator cmv_it = cm_ver.begin();cmv_it != cm_ver.end();cmv_it++)
		//{
		//	cmp_vertex.push_back(mesh->vertices_[*cmv_it]);
		//}
		////加入q点集合
		//std::vector<FaceHandle> cm_f = mesh->cells_[cell_num].faces_;
		//std::vector<FaceHandle> bdy_and_op_f;//只算同一层的
		////第一遍遍历寻找不加入q点的面
		//for (std::vector<FaceHandle>::iterator cmf_it = cm_f.begin();cmf_it != cm_f.end();cmf_it++)
		//{
		//	if (mesh->is_bdy(*cmf_it))
		//	{
		//		bdy_and_op_f.push_back(*cmf_it);
		//		bdy_and_op_f.push_back(mesh->get_opposite_face_in_cell(*cmf_it, CellHandle(cell_num)));
		//	}
		//}
		////第二遍遍历进行加入q点集合
		//for (std::vector<FaceHandle>::iterator cmf_it = cm_f.begin();cmf_it != cm_f.end();cmf_it++)
		//{
		//	if (std::find(bdy_and_op_f.begin(), bdy_and_op_f.end(), *cmf_it) == bdy_and_op_f.end())
		//	{
		//		std::set<CellHandle, compare_OVM> cmf_c = mesh->neighbor_f[*cmf_it];
		//		CellHandle cmfc;
		//		for (std::set<CellHandle, compare_OVM>::iterator cmf_c_it = cmf_c.begin();cmf_c_it != cmf_c.end(); cmf_c_it++)
		//		{
		//			if ((*cmf_c_it) != CellHandle(cell_num))
		//			{
		//				cmfc = *cmf_c_it;
		//				break;
		//			}
		//		}
		//		FaceHandle cmf_op = mesh->get_opposite_face_in_cell(*cmf_it, cmfc);
		//		std::vector<VertexHandle> cmf_op_v = mesh->faces_[cmf_op].vs_f;
		//		for (std::vector<VertexHandle>::iterator cmf_op_v_it = cmf_op_v.begin();cmf_op_v_it != cmf_op_v.end();cmf_op_v_it++)
		//		{
		//			cmq_vertex.push_back(mesh->vertices_[*cmf_op_v_it]);
		//		}
		//	}
		//}
		////计算块的中心点
		//int cmp_size = cmp_vertex.size();
		//int cmq_size = cmq_vertex.size();
		//*double cmp_weight = (6 * w + 1) / 8;
		//double cmq_weight = w / 4;*/
	 // /*double cmp_weight = ((cmq_size*1.0 / (cmq_size + cmp_size)) * 8 * w + 1) / 8;
		//double cmq_weight = ((cmp_size*1.0 / (cmq_size + cmp_size)) * 8 * w) / 8;*/
		//double cmp_weight = (6 * w + 1) / 8;
		//double cmq_weight;
		//if (cmq_size == 0) cmq_weight = 0;
		//else cmq_weight = w / (cmq_size *1.0 / 6);
		//V3f cmp_sum;V3f cmq_sum;
		//for (int cmp_i = 0;cmp_i < cmp_size;cmp_i++)
		//{
		//	cmp_sum = cmp_sum + cmp_vertex[cmp_i];
		//}
		//for (int cmq_i = 0;cmq_i < cmq_size;cmq_i++)
		//{
		//	cmq_sum = cmq_sum + cmq_vertex[cmq_i];
		//}
		//cell_mid = cmp_sum*cmp_weight - cmq_sum*cmq_weight;
		std::vector<VertexHandle> cm_ver = mesh->cells_[cell_num].vertex_;
		for (std::vector<VertexHandle>::iterator cmv_it = cm_ver.begin(); cmv_it != cm_ver.end(); cmv_it++)
		{
			cmp_vertex.push_back(mesh->vertices_[*cmv_it]);
		}
		int cmp_size = cmp_vertex.size();
		V3f cmp_sum;
		for (int cmp_i = 0; cmp_i < cmp_size; cmp_i++)
		{
			cmp_sum = cmp_sum + cmp_vertex[cmp_i];
		}
		cell_mid = cmp_sum * (1.0 / 8);
	}
	else
	{
		//加入p点集合
		std::vector<VertexHandle> cm_ver = mesh->cells_[cell_num].vertex_;
		for (std::vector<VertexHandle>::iterator cmv_it = cm_ver.begin(); cmv_it != cm_ver.end(); cmv_it++)
		{
			cmp_vertex.push_back(mesh->vertices_[*cmv_it]);
		}
		//加入q点集合
		std::vector<FaceHandle> cm_f = mesh->cells_[cell_num].faces_;
		for (std::vector<FaceHandle>::iterator cmf_it = cm_f.begin(); cmf_it != cm_f.end(); cmf_it++)
		{
			std::set<CellHandle, compare_OVM> cmf_c = mesh->neighbor_f[*cmf_it];
			CellHandle cmfc;
			for (std::set<CellHandle, compare_OVM>::iterator cmf_c_it = cmf_c.begin(); cmf_c_it != cmf_c.end(); cmf_c_it++)
			{
				if ((*cmf_c_it) != CellHandle(cell_num))
				{
					cmfc = *cmf_c_it;
					break;
				}
			}
			FaceHandle cmf_op = mesh->get_opposite_face_in_cell(*cmf_it, cmfc);
			std::vector<VertexHandle> cmf_op_v = mesh->faces_[cmf_op].vs_f;
			for (std::vector<VertexHandle>::iterator cmf_op_v_it = cmf_op_v.begin(); cmf_op_v_it != cmf_op_v.end(); cmf_op_v_it++)
			{
				cmq_vertex.push_back(mesh->vertices_[*cmf_op_v_it]);
			}
		}
		//计算块的中心点
		int cmp_size = cmp_vertex.size();
		int cmq_size = cmq_vertex.size();
		double cmp_weight = (6 * w + 1) / 8;
		double cmq_weight = w / 4;
		V3f cmp_sum; V3f cmq_sum;
		for (int cmp_i = 0; cmp_i < cmp_size; cmp_i++)
		{
			cmp_sum = cmp_sum + cmp_vertex[cmp_i];
		}
		for (int cmq_i = 0; cmq_i < cmq_size; cmq_i++)
		{
			cmq_sum = cmq_sum + cmq_vertex[cmq_i];
		}
		cell_mid = cmp_sum * cmp_weight - cmq_sum * cmq_weight;
	}
	return cell_mid;
}

V3f Subdivision::cal_face_mid(const int cell_num, const int face_num)
{
	V3f face_mid;
	std::vector<V3f> fmp_vertex;
	std::vector<V3f> fmq_vertex;
	std::vector<V3f> fmr_vertex;
	std::vector<V3f> fms_vertex;
	std::vector<V3f> fmt_vertex;
	if (mesh->is_bdy(mesh->cells_[cell_num].faces_[face_num]))
	{
		//加入p点集合
		std::vector<VertexHandle> fm_ver = mesh->faces_[mesh->cells_[cell_num].faces_[face_num]].vs_f;
		for (std::vector<VertexHandle>::iterator fmv_it = fm_ver.begin(); fmv_it != fm_ver.end(); fmv_it++)
		{
			fmp_vertex.push_back(mesh->vertices_[*fmv_it]);
		}
		//加入r点集合
		std::vector<EdgeHandle> fm_f_e = mesh->faces_[mesh->cells_[cell_num].faces_[face_num]].edges_;
		for (std::vector<EdgeHandle>::iterator fm_f_e_it = fm_f_e.begin(); fm_f_e_it != fm_f_e.end(); fm_f_e_it++)
		{
			std::set<FaceHandle, compare_OVM> fm_f_e_f = mesh->neighbor_e[*fm_f_e_it];
			for (std::set<FaceHandle, compare_OVM>::iterator fm_f_e_f_it = fm_f_e_f.begin(); fm_f_e_f_it != fm_f_e_f.end(); fm_f_e_f_it++)
			{
				if (mesh->is_bdy(*fm_f_e_f_it) && (*fm_f_e_f_it != mesh->cells_[cell_num].faces_[face_num]))
				{
					EdgeHandle fm_f_e_f_ope = mesh->get_opposite_edge_in_face(*fm_f_e_it, *fm_f_e_f_it);
					fmr_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_e_f_ope].from_h]);
					fmr_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_e_f_ope].to_h]);
					break;
				}
			}
		}
		//计算面的中心点
		int fmp_size = fmp_vertex.size();
		int fmr_size = fmr_vertex.size();
		/*double fmp_weight = ((fmr_size*1.0 / (fmr_size + fmp_size)) * 4 * w + 1) / 4;
		double fmr_weight = (fmp_size*1.0 / (fmr_size + fmp_size)) * 4 * w / 4;*/
		double fmp_weight = (3 * w + 1) / 4;
		double fmr_weight = (3 * w) / 8;
		V3f fmp_sum; V3f fmr_sum;
		for (int fmp_i = 0; fmp_i < fmp_size; fmp_i++)
		{
			fmp_sum = fmp_sum + fmp_vertex[fmp_i];
		}
		for (int fmr_i = 0; fmr_i < fmr_size; fmr_i++)
		{
			fmr_sum = fmr_sum + fmr_vertex[fmr_i];
		}
		face_mid = fmp_sum * fmp_weight - fmr_sum * fmr_weight;
	}
	else if (check_face(mesh->cells_[cell_num].faces_[face_num]))//当该面在边界块上时，用线型细分
	{
		std::vector<VertexHandle> fm_ver = mesh->faces_[mesh->cells_[cell_num].faces_[face_num]].vs_f;
		for (std::vector<VertexHandle>::iterator fmv_it = fm_ver.begin(); fmv_it != fm_ver.end(); fmv_it++)
		{
			fmp_vertex.push_back(mesh->vertices_[*fmv_it]);
		}
		int fmp_size = fmp_vertex.size();
		V3f fmp_sum;
		for (int fmp_i = 0; fmp_i < fmp_size; fmp_i++)
		{
			fmp_sum = fmp_sum + fmp_vertex[fmp_i];
		}
		face_mid = fmp_sum * (1.0 / 4);
	}
	else
	{
		//加入p点集合
		std::vector<VertexHandle> fm_ver = mesh->faces_[mesh->cells_[cell_num].faces_[face_num]].vs_f;
		for (std::vector<VertexHandle>::iterator fmv_it = fm_ver.begin(); fmv_it != fm_ver.end(); fmv_it++)
		{
			fmp_vertex.push_back(mesh->vertices_[*fmv_it]);
		}
		//加入q点集合
		std::set<CellHandle, compare_OVM> fm_c = mesh->neighbor_f[mesh->cells_[cell_num].faces_[face_num]];
		for (std::set<CellHandle, compare_OVM>::iterator fm_c_it = fm_c.begin(); fm_c_it != fm_c.end(); fm_c_it++)
		{
			FaceHandle fm_c_op = mesh->get_opposite_face_in_cell(mesh->cells_[cell_num].faces_[face_num], *fm_c_it);
			std::vector<VertexHandle> fm_c_op_v = mesh->faces_[fm_c_op].vs_f;
			for (std::vector<VertexHandle>::iterator fm_c_op_v_it = fm_c_op_v.begin(); fm_c_op_v_it != fm_c_op_v.end(); fm_c_op_v_it++)
			{
				fmq_vertex.push_back(mesh->vertices_[*fm_c_op_v_it]);
			}
		}
		//加入r、s和t点集合
		std::set<FaceHandle, compare_OVM> fm_c_fall;//用于插入面相邻块的所有面
		for (std::set<CellHandle, compare_OVM>::iterator fm_c_it = fm_c.begin(); fm_c_it != fm_c.end(); fm_c_it++)
		{
			std::vector<FaceHandle> fm_c_f = mesh->cells_[*fm_c_it].faces_;
			for (std::vector<FaceHandle>::iterator fm_c_f_it = fm_c_f.begin(); fm_c_f_it != fm_c_f.end(); fm_c_f_it++)
			{
				fm_c_fall.insert(*fm_c_f_it);
			}
		}
		std::vector<EdgeHandle> fm_f_e = mesh->faces_[mesh->cells_[cell_num].faces_[face_num]].edges_;
		int N = 0;//记录奇异边个数
		for (std::vector<EdgeHandle>::iterator fm_f_e_it = fm_f_e.begin(); fm_f_e_it != fm_f_e.end(); fm_f_e_it++)
		{
			if (!mesh->is_singular(*fm_f_e_it))
			{
				//加入点r
				std::set<FaceHandle, compare_OVM> fm_f_e_f = mesh->neighbor_e[*fm_f_e_it];
				for (std::set<FaceHandle, compare_OVM>::iterator fm_f_e_f_it = fm_f_e_f.begin(); fm_f_e_f_it != fm_f_e_f.end(); fm_f_e_f_it++)
				{
					if (std::find(fm_c_fall.begin(), fm_c_fall.end(), *fm_f_e_f_it) == fm_c_fall.end())
					{
						EdgeHandle fm_f_ope = mesh->get_opposite_edge_in_face(*fm_f_e_it, *fm_f_e_f_it);
						fmr_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_ope].from_h]);
						fmr_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_ope].to_h]);
						break;
					}
				}
				//加入点s
				//CellHandle fm_ci[2];//两个与该面相邻的块
				//int fm_c_size = fm_c.size();int fm_c_n = 0;
				//for (std::set<CellHandle, compare_OVM>::iterator fm_c_it = fm_c.begin();fm_c_it != fm_c.end();fm_c_it++)
				//{
				//	fm_ci[fm_c_n] = *fm_c_it;
				//	fm_c_n++;
				//}
				//std::set<FaceHandle, compare_OVM> fm_f_e_f = mesh->neighbor_e[*fm_f_e_it];
				for (std::set<FaceHandle, compare_OVM>::iterator fm_f_e_f_it = fm_f_e_f.begin(); fm_f_e_f_it != fm_f_e_f.end(); fm_f_e_f_it++)
				{
					if (*fm_f_e_f_it == mesh->cells_[cell_num].faces_[face_num])
						continue;
					//else if (mesh->is_f_in_c(*fm_f_e_f_it, fm_ci[0]) || mesh->is_f_in_c(*fm_f_e_f_it, fm_ci[1]))
					else if (std::find(fm_c_fall.begin(), fm_c_fall.end(), *fm_f_e_f_it) != fm_c_fall.end())
						continue;
					else
					{
						std::set<CellHandle, compare_OVM> fm_f_e_f_c = mesh->neighbor_f[*fm_f_e_f_it];//这两个块包含点s
						for (std::set<CellHandle, compare_OVM>::iterator fm_f_e_f_c_it = fm_f_e_f_c.begin(); fm_f_e_f_c_it != fm_f_e_f_c.end(); fm_f_e_f_c_it++)
						{
							FaceHandle fm_f_e_f_c_opf = mesh->get_opposite_face_in_cell(*fm_f_e_f_it, *fm_f_e_f_c_it);
							std::vector<VertexHandle> fm_f_e_f_c_opf_v = mesh->faces_[fm_f_e_f_c_opf].vs_f;
							for (std::vector<VertexHandle>::iterator fm_f_e_f_c_opf_v_it = fm_f_e_f_c_opf_v.begin();
								fm_f_e_f_c_opf_v_it != fm_f_e_f_c_opf_v.end(); fm_f_e_f_c_opf_v_it++)
							{
								if (std::find(fmr_vertex.begin(), fmr_vertex.end(), mesh->vertices_[*fm_f_e_f_c_opf_v_it]) == fmr_vertex.end())
									fms_vertex.push_back(mesh->vertices_[*fm_f_e_f_c_opf_v_it]);
							}
						}
					}
				}
			}
			else//若该奇异边度大于4.则r点分裂，s点不变，若该奇异边度数小于4，则s点合并，r点消失。
			{
				N += 1;
				CellHandle fm_ci[2];//两个与该 面相邻的块
				int fm_c_size = fm_c.size(); int fm_c_n = 0;
				for (std::set<CellHandle, compare_OVM>::iterator fm_c_it = fm_c.begin(); fm_c_it != fm_c.end(); fm_c_it++)
				{
					fm_ci[fm_c_n] = *fm_c_it;
					fm_c_n++;
				}
				std::set<FaceHandle, compare_OVM> fm_f_e_f = mesh->neighbor_e[*fm_f_e_it];
				int sin_size = fm_f_e_f.size();
				for (std::set<FaceHandle, compare_OVM>::iterator fm_f_e_f_it = fm_f_e_f.begin(); fm_f_e_f_it != fm_f_e_f.end(); fm_f_e_f_it++)
				{
					if (*fm_f_e_f_it == mesh->cells_[cell_num].faces_[face_num])
						continue;
					else if (mesh->is_f_in_c(*fm_f_e_f_it, fm_ci[0]))
					{
						std::set<CellHandle, compare_OVM> fm_f_e_f_c = mesh->neighbor_f[*fm_f_e_f_it];
						for (std::set<CellHandle, compare_OVM>::iterator fm_f_e_f_c_it = fm_f_e_f_c.begin(); fm_f_e_f_c_it != fm_f_e_f_c.end(); fm_f_e_f_c_it++)
						{
							if (*fm_f_e_f_c_it == fm_ci[0])
								continue;
							else
							{
								//该块为t、s点所在块
								std::vector<FaceHandle> fm_f_e_f_c_f = mesh->cells_[*fm_f_e_f_c_it].faces_;
								for (std::vector<FaceHandle>::iterator fm_f_e_f_c_f_it = fm_f_e_f_c_f.begin(); fm_f_e_f_c_f_it != fm_f_e_f_c_f.end(); fm_f_e_f_c_f_it++)
								{
									if ((std::find(fm_f_e_f.begin(), fm_f_e_f.end(), *fm_f_e_f_c_f_it) != fm_f_e_f.end()) &&
										(*fm_f_e_f_c_f_it != *fm_f_e_f_it))//若边的度数小于4，则不存在r点的面
									{
										//该面为t点所在面
										//加入点t
										if (sin_size > 4)
										{
											EdgeHandle fm_f_e_f_c_f_e = mesh->get_opposite_edge_in_face(*fm_f_e_it, *fm_f_e_f_c_f_it);
											fmt_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_e_f_c_f_e].from_h]);
											fmt_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_e_f_c_f_e].to_h]);
										}
										//该面所在块的对面为点s所在面
										//加入点s
										FaceHandle fm_f_e_f_c_opf = mesh->get_opposite_face_in_cell(*fm_f_e_f_c_f_it, *fm_f_e_f_c_it);
										std::vector<VertexHandle> fm_f_e_f_c_opf_v = mesh->faces_[fm_f_e_f_c_opf].vs_f;
										for (std::vector<VertexHandle>::iterator fm_f_e_f_c_opf_v_it = fm_f_e_f_c_opf_v.begin();
											fm_f_e_f_c_opf_v_it != fm_f_e_f_c_opf_v.end(); fm_f_e_f_c_opf_v_it++)
										{
											if ((std::find(fmq_vertex.begin(), fmq_vertex.end(), mesh->vertices_[*fm_f_e_f_c_opf_v_it]) == fmq_vertex.end())
												&& (std::find(fms_vertex.begin(), fms_vertex.end(), mesh->vertices_[*fm_f_e_f_c_opf_v_it]) == fms_vertex.end()))
												fms_vertex.push_back(mesh->vertices_[*fm_f_e_f_c_opf_v_it]);
										}
										break;
									}
								}
							}
						}
					}
					else if (mesh->is_f_in_c(*fm_f_e_f_it, fm_ci[1]))
					{
						std::set<CellHandle, compare_OVM> fm_f_e_f_c = mesh->neighbor_f[*fm_f_e_f_it];
						for (std::set<CellHandle, compare_OVM>::iterator fm_f_e_f_c_it = fm_f_e_f_c.begin(); fm_f_e_f_c_it != fm_f_e_f_c.end(); fm_f_e_f_c_it++)
						{
							if (*fm_f_e_f_c_it == fm_ci[1])
								continue;
							else
							{
								//该块为t、s点所在块
								std::vector<FaceHandle> fm_f_e_f_c_f = mesh->cells_[*fm_f_e_f_c_it].faces_;
								for (std::vector<FaceHandle>::iterator fm_f_e_f_c_f_it = fm_f_e_f_c_f.begin(); fm_f_e_f_c_f_it != fm_f_e_f_c_f.end(); fm_f_e_f_c_f_it++)
								{
									if ((std::find(fm_f_e_f.begin(), fm_f_e_f.end(), *fm_f_e_f_c_f_it) != fm_f_e_f.end()) &&
										(*fm_f_e_f_c_f_it != *fm_f_e_f_it))//若边的度数小于4，则不存在r点的面
									{
										//该面为t点所在面
										//加入点t
										if (sin_size > 4)
										{
											EdgeHandle fm_f_e_f_c_f_e = mesh->get_opposite_edge_in_face(*fm_f_e_it, *fm_f_e_f_c_f_it);
											fmt_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_e_f_c_f_e].from_h]);
											fmt_vertex.push_back(mesh->vertices_[mesh->edges_[fm_f_e_f_c_f_e].to_h]);
										}
										//该面所在块的对面为点s所在面
										//加入点s
										FaceHandle fm_f_e_f_c_opf = mesh->get_opposite_face_in_cell(*fm_f_e_f_c_f_it, *fm_f_e_f_c_it);
										std::vector<VertexHandle> fm_f_e_f_c_opf_v = mesh->faces_[fm_f_e_f_c_opf].vs_f;
										for (std::vector<VertexHandle>::iterator fm_f_e_f_c_opf_v_it = fm_f_e_f_c_opf_v.begin();
											fm_f_e_f_c_opf_v_it != fm_f_e_f_c_opf_v.end(); fm_f_e_f_c_opf_v_it++)
										{
											if ((std::find(fmq_vertex.begin(), fmq_vertex.end(), mesh->vertices_[*fm_f_e_f_c_opf_v_it]) == fmq_vertex.end())
												&& (std::find(fms_vertex.begin(), fms_vertex.end(), mesh->vertices_[*fm_f_e_f_c_opf_v_it]) == fms_vertex.end()))
												fms_vertex.push_back(mesh->vertices_[*fm_f_e_f_c_opf_v_it]);
										}
										break;
									}
								}
							}
						}
					}
					else
						continue;
				}
			}
		}
		//计算面的中心点
		int fmp_size = fmp_vertex.size();
		int fmq_size = fmq_vertex.size();
		int fmr_size = fmr_vertex.size();
		int fms_size = fms_vertex.size();
		int fmt_size = fmt_vertex.size();

		double fmp_weight = (2 * w + 1) / 4;
		double fmq_weight = w / 4;
		double fmr_weight;
		double fms_weight;
		if (fmr_size == 0 && fmt_size == 0) fmr_weight = 0;
		else fmr_weight = w / ((fmt_size / 2 + fmr_size) / 2);
		if (fms_size == 0) fms_weight = 0;
		else fms_weight = w / (fms_size / 2);
		double fmt_weight = fmr_weight / 2;

		/*double fmp_weight = (2 * w + 1) / 4;
		double fmq_weight = w / 4;
		double fmr_weight = w / 4;
		double fms_weight = w / 8;
		double fmt_weight = w / 8;*/

		/*double fmp_weight, fmq_weight, fmr_weight, fms_weight, fmt_weight;
		fmp_weight = (2 * w + 1) / 4;
		fmq_weight = w / 4;
		if((fmt_size / 2 + fmr_size) !=  8)
		fmr_weight = w / 4;
		else
		{
		if (fmr_size == 0 && fmt_size == 0) fmr_weight = 0;
		else fmr_weight = w / ((fmt_size / 2 + fmr_size) / 2);
		}
		if(fms_size != 16)
		fms_weight = w / 8;
		else
		{
		if (fms_size == 0) fms_weight = 0;
		else fms_weight = w / (fms_size / 2);
		}
		fmt_weight = fmr_weight / 2;*/

		V3f fmp_sum; V3f fmq_sum; V3f fmr_sum; V3f fms_sum; V3f fmt_sum;
		for (int fmp_i = 0; fmp_i < fmp_size; fmp_i++)
		{
			fmp_sum = fmp_sum + fmp_vertex[fmp_i];
		}
		for (int fmq_i = 0; fmq_i < fmq_size; fmq_i++)
		{
			fmq_sum = fmq_sum + fmq_vertex[fmq_i];
		}
		for (int fmr_i = 0; fmr_i < fmr_size; fmr_i++)
		{
			fmr_sum = fmr_sum + fmr_vertex[fmr_i];
		}
		for (int fms_i = 0; fms_i < fms_size; fms_i++)
		{
			fms_sum = fms_sum + fms_vertex[fms_i];
		}
		for (int fmt_i = 0; fmt_i < fmt_size; fmt_i++)
		{
			fmt_sum = fmt_sum + fmt_vertex[fmt_i];
		}
		face_mid = fmp_sum * fmp_weight + fmq_sum * fmq_weight - fmr_sum * fmr_weight - fms_sum * fms_weight - fmt_sum * fmt_weight;
	}
	return face_mid;
}

V3f Subdivision::cal_edge_mid(const int cell_num, const int ver1, const int ver2)
{
	V3f edge_mid;
	std::vector<V3f> emp_vertex;
	std::vector<V3f> emq_vertex;
	std::vector<V3f> emr_vertex;
	std::vector<V3f> ems_vertex;
	//首先确定该边
	EdgeHandle em_e;
	bool is_find = false;
	std::vector<FaceHandle> em_c_f = mesh->cells_[cell_num].faces_;
	for (std::vector<FaceHandle>::iterator em_c_f_it = em_c_f.begin(); em_c_f_it != em_c_f.end(); em_c_f_it++)
	{
		if (is_find == true)
			break;
		std::vector<EdgeHandle> em_c_f_e = mesh->faces_[*em_c_f_it].edges_;
		for (std::vector<EdgeHandle>::iterator em_c_f_e_it = em_c_f_e.begin(); em_c_f_e_it != em_c_f_e.end(); em_c_f_e_it++)
		{
			if (((mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver2]))
				|| ((mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver2])))
			{
				em_e = *em_c_f_e_it;
				is_find = true;
				break;
			}
		}
	}
	if (mesh->is_bdy(em_e))
	{
		const int N = mesh->neighbor_e[em_e].size();
		//表面的不算入奇异边
		//加入p点集合
		emp_vertex.push_back(mesh->vertices_[mesh->cells_[cell_num].vertex_[ver1]]);
		emp_vertex.push_back(mesh->vertices_[mesh->cells_[cell_num].vertex_[ver2]]);
		//加入q、r点集合
		std::set<FaceHandle, compare_OVM> em_e_f = mesh->neighbor_e[em_e];
		for (std::set<FaceHandle, compare_OVM>::iterator em_e_f_it = em_e_f.begin(); em_e_f_it != em_e_f.end(); em_e_f_it++)
		{
			if (mesh->is_bdy(*em_e_f_it))
			{
				//加入q点集合
				EdgeHandle em_e_f_ope = mesh->get_opposite_edge_in_face(em_e, *em_e_f_it);
				emq_vertex.push_back(mesh->vertices_[mesh->edges_[em_e_f_ope].from_h]);
				emq_vertex.push_back(mesh->vertices_[mesh->edges_[em_e_f_ope].to_h]);
				/*std::vector<VertexHandle> em_e_f_v = mesh->faces_[*em_e_f_it].vs_f;
				for (std::vector<VertexHandle>::iterator em_e_f_v_it = em_e_f_v.begin();em_e_f_v_it != em_e_f_v.end();em_e_f_v_it++)
				{
					if ((*em_e_f_v_it != mesh->cells_[cell_num].vertex_[ver2]) && (*em_e_f_v_it != mesh->cells_[cell_num].vertex_[ver1]))
						emq_vertex.push_back(mesh->vertices_[*em_e_f_v_it]);
				}*/
				//加入r点集合
				std::vector<EdgeHandle> em_e_f_e = mesh->faces_[*em_e_f_it].edges_;
				//EdgeHandle em_ope = mesh->get_opposite_edge_in_face(em_e,*em_e_f_it);
				//确定r点
				for (std::vector<EdgeHandle>::iterator em_e_f_e_it = em_e_f_e.begin(); em_e_f_e_it != em_e_f_e.end(); em_e_f_e_it++)
				{
					if ((*em_e_f_e_it != em_e) && (*em_e_f_e_it != em_e_f_ope))
					{
						//确定这条边上的q点
						VertexHandle em_e_f_e_v;
						if (std::find(emq_vertex.begin(), emq_vertex.end(), mesh->vertices_[mesh->edges_[*em_e_f_e_it].from_h]) != emq_vertex.end())
							em_e_f_e_v = mesh->edges_[*em_e_f_e_it].from_h;
						else
							em_e_f_e_v = mesh->edges_[*em_e_f_e_it].to_h;
						std::set<FaceHandle, compare_OVM> em_e_f_e_f = mesh->neighbor_e[*em_e_f_e_it];
						for (std::set<FaceHandle, compare_OVM>::iterator em_e_f_e_f_it = em_e_f_e_f.begin(); em_e_f_e_f_it != em_e_f_e_f.end(); em_e_f_e_f_it++)
						{
							if (mesh->is_bdy(*em_e_f_e_f_it) && (*em_e_f_e_f_it != *em_e_f_it))
							{
								//该面为r点所在面
								//确定r点所在边
								std::vector<EdgeHandle> em_e_f_e_f_e = mesh->faces_[*em_e_f_e_f_it].edges_;
								for (std::vector<EdgeHandle>::iterator em_e_f_e_f_e_it = em_e_f_e_f_e.begin(); em_e_f_e_f_e_it != em_e_f_e_f_e.end(); em_e_f_e_f_e_it++)
								{
									if (*em_e_f_e_f_e_it == *em_e_f_e_it)
										continue;
									else if (mesh->edges_[*em_e_f_e_f_e_it].from_h == em_e_f_e_v)
										emr_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_e_f_e_it].to_h]);
									else if (mesh->edges_[*em_e_f_e_f_e_it].to_h == em_e_f_e_v)
										emr_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_e_f_e_it].from_h]);
									else
										continue;
								}
							}
						}
					}
				}
			}
		}
		//计算边的中心点
		int emp_size = emp_vertex.size();
		int emq_size = emq_vertex.size();
		int emr_size = emr_vertex.size();
		double emp_weight = 1.0 / 2;
		double emq_weight = w / 2;
		double emr_weight = w / 2;
		V3f emp_sum; V3f emq_sum; V3f emr_sum;
		for (int emp_i = 0; emp_i < emp_size; emp_i++)
		{
			emp_sum = emp_sum + emp_vertex[emp_i];
		}
		for (int emq_i = 0; emq_i < emq_size; emq_i++)
		{
			emq_sum = emq_sum + emq_vertex[emq_i];
		}
		for (int emr_i = 0; emr_i < emr_size; emr_i++)
		{
			emr_sum = emr_sum + emr_vertex[emr_i];
		}
		edge_mid = emp_sum * emp_weight + emq_sum * emq_weight - emr_sum * emr_weight;
	}
	else if (check_edge(em_e))//当该边在边界块上时，用线型细分
	{
		emp_vertex.push_back(mesh->vertices_[mesh->cells_[cell_num].vertex_[ver1]]);
		emp_vertex.push_back(mesh->vertices_[mesh->cells_[cell_num].vertex_[ver2]]);
		int emp_size = emp_vertex.size();
		V3f emp_sum;
		for (int emp_i = 0; emp_i < emp_size; emp_i++)
		{
			emp_sum = emp_sum + emp_vertex[emp_i];
		}
		edge_mid = emp_sum * (1.0 / 2);
	}
	else
	{
		//加入p点集合
		emp_vertex.push_back(mesh->vertices_[mesh->cells_[cell_num].vertex_[ver1]]);
		emp_vertex.push_back(mesh->vertices_[mesh->cells_[cell_num].vertex_[ver2]]);
		//加入q、r、s点集合
		std::set<FaceHandle, compare_OVM> em_e_f = mesh->neighbor_e[em_e];
		const int N = em_e_f.size();
		for (std::set<FaceHandle, compare_OVM>::iterator em_e_f_it = em_e_f.begin(); em_e_f_it != em_e_f.end(); em_e_f_it++)
		{
			//为判断将面的邻接块放入数组
			CellHandle em_e_f_cche[2];
			std::set<CellHandle, compare_OVM> em_e_f_c = mesh->neighbor_f[*em_e_f_it];
			int em_e_f_c_num = 0;
			for (std::set<CellHandle, compare_OVM>::iterator em_e_f_c_it = em_e_f_c.begin(); em_e_f_c_it != em_e_f_c.end(); em_e_f_c_it++)
			{
				em_e_f_cche[em_e_f_c_num++] = *em_e_f_c_it;
			}
			//加入q点集合
			EdgeHandle em_e_f_ope = mesh->get_opposite_edge_in_face(em_e, *em_e_f_it);
			emq_vertex.push_back(mesh->vertices_[mesh->edges_[em_e_f_ope].from_h]);
			emq_vertex.push_back(mesh->vertices_[mesh->edges_[em_e_f_ope].to_h]);
			/*std::vector<VertexHandle> em_e_f_v = mesh->faces_[*em_e_f_it].vs_f;
			for (std::vector<VertexHandle>::iterator em_e_f_v_it = em_e_f_v.begin();em_e_f_v_it != em_e_f_v.end();em_e_f_v_it++)
			{
				if ((*em_e_f_v_it != mesh->cells_[cell_num].vertex_[ver2]) && (*em_e_f_v_it != mesh->cells_[cell_num].vertex_[ver1]))
					emq_vertex.push_back(mesh->vertices_[*em_e_f_v_it]);
			}*/
			//加入r、s点集合
			std::vector<EdgeHandle> em_e_f_e = mesh->faces_[*em_e_f_it].edges_;
			//EdgeHandle em_ope = mesh->get_opposite_edge_in_face(em_e, *em_e_f_it);
			for (std::vector<EdgeHandle>::iterator em_e_f_e_it = em_e_f_e.begin(); em_e_f_e_it != em_e_f_e.end(); em_e_f_e_it++)
			{
				if ((*em_e_f_e_it != em_e) && (*em_e_f_e_it != em_e_f_ope))
				{
					int sin_size = mesh->neighbor_e[*em_e_f_e_it].size();
					//如果该邻接边不是奇异边，则加入r点
					if (!mesh->is_singular(*em_e_f_e_it))
					{
						//确定r点
						//确定这条边上的q点
						VertexHandle em_e_f_e_v;
						if (std::find(emq_vertex.begin(), emq_vertex.end(), mesh->vertices_[mesh->edges_[*em_e_f_e_it].from_h]) != emq_vertex.end())
							em_e_f_e_v = mesh->edges_[*em_e_f_e_it].from_h;
						else
							em_e_f_e_v = mesh->edges_[*em_e_f_e_it].to_h;
						std::set<FaceHandle, compare_OVM> em_e_f_e_f = mesh->neighbor_e[*em_e_f_e_it];
						for (std::set<FaceHandle, compare_OVM>::iterator em_e_f_e_f_it = em_e_f_e_f.begin(); em_e_f_e_f_it != em_e_f_e_f.end(); em_e_f_e_f_it++)
						{
							if (!mesh->is_f_in_c(*em_e_f_e_f_it, em_e_f_cche[0]) && !mesh->is_f_in_c(*em_e_f_e_f_it, em_e_f_cche[1]))
							{
								//该面为r点所在面
								//确定r点所在边
								std::vector<EdgeHandle> em_e_f_e_f_e = mesh->faces_[*em_e_f_e_f_it].edges_;
								for (std::vector<EdgeHandle>::iterator em_e_f_e_f_e_it = em_e_f_e_f_e.begin(); em_e_f_e_f_e_it != em_e_f_e_f_e.end(); em_e_f_e_f_e_it++)
								{
									if (*em_e_f_e_f_e_it == *em_e_f_e_it)
										continue;
									else if (mesh->edges_[*em_e_f_e_f_e_it].from_h == em_e_f_e_v)
										emr_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_e_f_e_it].to_h]);
									else if (mesh->edges_[*em_e_f_e_f_e_it].to_h == em_e_f_e_v)
										emr_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_e_f_e_it].from_h]);
									else
										continue;
								}
							}
						}
					}
					//如果为奇异边，则需加入s点
					if (mesh->is_singular(*em_e_f_e_it) && sin_size > 4)//度数小于4的奇异边没有s点
					{
						//确定s点
						//确定这条边上的q点
						VertexHandle em_e_f_e_v;
						if (std::find(emq_vertex.begin(), emq_vertex.end(), mesh->vertices_[mesh->edges_[*em_e_f_e_it].from_h]) != emq_vertex.end())
							em_e_f_e_v = mesh->edges_[*em_e_f_e_it].from_h;
						else
							em_e_f_e_v = mesh->edges_[*em_e_f_e_it].to_h;
						//首先取第一个位于em_e_f_cche[0]一侧的s点
						std::vector<FaceHandle> em_e_f_c_f0 = mesh->cells_[em_e_f_cche[0]].faces_;
						for (std::vector<FaceHandle>::iterator em_e_f_c_f_it = em_e_f_c_f0.begin(); em_e_f_c_f_it != em_e_f_c_f0.end(); em_e_f_c_f_it++)
						{
							if (mesh->is_e_in_f(*em_e_f_e_it, *em_e_f_c_f_it) && (*em_e_f_it != *em_e_f_c_f_it))
							{
								//该面为s点所在块的面，现在取这个块
								std::set<CellHandle, compare_OVM> em_e_f_c_f_c = mesh->neighbor_f[*em_e_f_c_f_it];
								for (std::set<CellHandle, compare_OVM>::iterator em_e_f_c_f_c_it = em_e_f_c_f_c.begin();
									em_e_f_c_f_c_it != em_e_f_c_f_c.end(); em_e_f_c_f_c_it++)
								{
									if (*em_e_f_c_f_c_it != em_e_f_cche[0])
									{
										//这个块为s点所在块，现在取s点所在面
										std::vector<FaceHandle> em_e_f_c_f_c_f = mesh->cells_[*em_e_f_c_f_c_it].faces_;
										for (std::vector<FaceHandle>::iterator em_e_f_c_f_c_f_it = em_e_f_c_f_c_f.begin();
											em_e_f_c_f_c_f_it != em_e_f_c_f_c_f.end(); em_e_f_c_f_c_f_it++)
										{
											if (mesh->is_e_in_f(*em_e_f_e_it, *em_e_f_c_f_c_f_it) && (*em_e_f_c_f_c_f_it != *em_e_f_c_f_it))
											{
												//该面为s点所在面
												//确定s点所在边
												std::vector<EdgeHandle> em_e_f_c_f_c_f_e = mesh->faces_[*em_e_f_c_f_c_f_it].edges_;
												for (std::vector<EdgeHandle>::iterator em_e_f_c_f_c_f_e_it = em_e_f_c_f_c_f_e.begin();
													em_e_f_c_f_c_f_e_it != em_e_f_c_f_c_f_e.end(); em_e_f_c_f_c_f_e_it++)
												{
													if (*em_e_f_c_f_c_f_e_it == *em_e_f_e_it)
														continue;
													else if (mesh->edges_[*em_e_f_c_f_c_f_e_it].from_h == em_e_f_e_v)
														ems_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_c_f_c_f_e_it].to_h]);
													else if (mesh->edges_[*em_e_f_c_f_c_f_e_it].to_h == em_e_f_e_v)
														ems_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_c_f_c_f_e_it].from_h]);
													else
														continue;
												}
											}
										}
									}
								}
							}
						}
						//再取第二个位于em_e_f_cche[1]一侧的s点
						std::vector<FaceHandle> em_e_f_c_f1 = mesh->cells_[em_e_f_cche[1]].faces_;
						for (std::vector<FaceHandle>::iterator em_e_f_c_f_it = em_e_f_c_f1.begin(); em_e_f_c_f_it != em_e_f_c_f1.end(); em_e_f_c_f_it++)
						{
							if (mesh->is_e_in_f(*em_e_f_e_it, *em_e_f_c_f_it) && (*em_e_f_it != *em_e_f_c_f_it))
							{
								//该面为s点所在块的面，现在取这个块
								std::set<CellHandle, compare_OVM> em_e_f_c_f_c = mesh->neighbor_f[*em_e_f_c_f_it];
								for (std::set<CellHandle, compare_OVM>::iterator em_e_f_c_f_c_it = em_e_f_c_f_c.begin();
									em_e_f_c_f_c_it != em_e_f_c_f_c.end(); em_e_f_c_f_c_it++)
								{
									if (*em_e_f_c_f_c_it != em_e_f_cche[1])
									{
										//这个块为s点所在块，现在取s点所在面
										std::vector<FaceHandle> em_e_f_c_f_c_f = mesh->cells_[*em_e_f_c_f_c_it].faces_;
										for (std::vector<FaceHandle>::iterator em_e_f_c_f_c_f_it = em_e_f_c_f_c_f.begin();
											em_e_f_c_f_c_f_it != em_e_f_c_f_c_f.end(); em_e_f_c_f_c_f_it++)
										{
											if (mesh->is_e_in_f(*em_e_f_e_it, *em_e_f_c_f_c_f_it) && (*em_e_f_c_f_c_f_it != *em_e_f_c_f_it))
											{
												//该面为s点所在面
												//确定s点所在边
												std::vector<EdgeHandle> em_e_f_c_f_c_f_e = mesh->faces_[*em_e_f_c_f_c_f_it].edges_;
												for (std::vector<EdgeHandle>::iterator em_e_f_c_f_c_f_e_it = em_e_f_c_f_c_f_e.begin();
													em_e_f_c_f_c_f_e_it != em_e_f_c_f_c_f_e.end(); em_e_f_c_f_c_f_e_it++)
												{
													if (*em_e_f_c_f_c_f_e_it == *em_e_f_e_it)
														continue;
													else if (mesh->edges_[*em_e_f_c_f_c_f_e_it].from_h == em_e_f_e_v)
														ems_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_c_f_c_f_e_it].to_h]);
													else if (mesh->edges_[*em_e_f_c_f_c_f_e_it].to_h == em_e_f_e_v)
														ems_vertex.push_back(mesh->vertices_[mesh->edges_[*em_e_f_c_f_c_f_e_it].from_h]);
													else
														continue;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		//计算边的中心点
		int emp_size = emp_vertex.size();
		int emq_size = emq_vertex.size();
		int emr_size = emr_vertex.size();
		int ems_size = ems_vertex.size();

		double emp_weight = 1.0 / 2;
		double emq_weight = w / N;
		/*double emr_weight = w / N;
		double ems_weight = w / (2 * N);*/
		double emr_weight = w / ((emr_size + ems_size / 2) / 2);
		double ems_weight = emr_weight / 2;

		/*double emp_weight, emq_weight, emr_weight, ems_weight;
		emp_weight = 1.0 / 2;
		emq_weight = w / N;
		if((emr_size + ems_size / 2) != 2*N)
			emr_weight = w / N;
		else
			emr_weight = w / ((emr_size + ems_size / 2) / 2);
		ems_weight = emr_weight / 2;*/

		V3f emp_sum; V3f emq_sum; V3f emr_sum; V3f ems_sum;
		for (int emp_i = 0; emp_i < emp_size; emp_i++)
		{
			emp_sum = emp_sum + emp_vertex[emp_i];
		}
		for (int emq_i = 0; emq_i < emq_size; emq_i++)
		{
			emq_sum = emq_sum + emq_vertex[emq_i];
		}
		for (int emr_i = 0; emr_i < emr_size; emr_i++)
		{
			emr_sum = emr_sum + emr_vertex[emr_i];
		}
		for (int ems_i = 0; ems_i < ems_size; ems_i++)
		{
			ems_sum = ems_sum + ems_vertex[ems_i];
		}
		edge_mid = emp_sum * emp_weight + emq_sum * emq_weight - emr_sum * emr_weight - ems_sum * ems_weight;
	}
	return edge_mid;
}

bool Subdivision::check_face(FaceHandle _f)
{
	std::set<CellHandle, compare_OVM> fm_c = mesh->neighbor_f[_f];
	bool check = false;
	for (std::set<CellHandle, compare_OVM>::iterator fm_c_it = fm_c.begin(); fm_c_it != fm_c.end(); fm_c_it++)
	{
		if (mesh->is_bdy(*fm_c_it))
			check = true;
		break;
	}
	return check;
}

bool Subdivision::check_edge(EdgeHandle _e)
{
	bool check = false;
	std::set<FaceHandle, compare_OVM> em_f = mesh->neighbor_e[_e];
	for (std::set<FaceHandle, compare_OVM>::iterator em_f_it = em_f.begin(); em_f_it != em_f.end(); em_f_it++)
	{
		if (check == true) break;
		std::vector<EdgeHandle> em_f_e = mesh->faces_[*em_f_it].edges_;
		for (std::vector<EdgeHandle>::iterator em_f_e_it = em_f_e.begin(); em_f_e_it != em_f_e.end(); em_f_e_it++)
		{
			if (mesh->is_bdy(*em_f_e_it))
			{
				check = true;
				break;
			}
		}
	}
	return check;
}

//-----CCSubdivision-----//
HexV3fMesh& CCSubdivision::CCSubdivising()
{
	//material_distribution_mutires.swap(std::vector<std::vector<double>>());
	uint64_t cell_num = mesh->cells_.size();
	if (material_distribution.size() < cell_num) {
		material_distribution.resize(cell_num);
	}
	std::vector<double> material_distribution1(8 * cell_num, 1);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_new_vertex(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_edge_mid(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_edge_mid(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_edge_mid(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_edge_mid(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_edge_mid(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_edge_mid(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_edge_mid(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_edge_mid(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_edge_mid(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_edge_mid(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_edge_mid(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_edge_mid(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		
		if (material_distribution[i] != 1)
		{
			material_distribution1[newmesh->add_cell(sub_cell[0])] = 0.0001;
			material_distribution1[newmesh->add_cell(sub_cell[1])] = 0.0001;
			material_distribution1[newmesh->add_cell(sub_cell[2])] = 0.0001;
			material_distribution1[newmesh->add_cell(sub_cell[3])] = 0.0001;
			material_distribution1[newmesh->add_cell(sub_cell[4])] = 0.0001;
			material_distribution1[newmesh->add_cell(sub_cell[5])] = 0.0001;
			material_distribution1[newmesh->add_cell(sub_cell[6])] = 0.0001;
			material_distribution1[newmesh->add_cell(sub_cell[7])] = 0.0001;
			//std::vector<double> material_distribution_temp(8, 0.0001);
			//material_distribution_mutires.push_back(material_distribution_temp);
		}
		else
		{
			newmesh->add_cell(sub_cell[0]);
			newmesh->add_cell(sub_cell[1]);
			newmesh->add_cell(sub_cell[2]);
			newmesh->add_cell(sub_cell[3]);
			newmesh->add_cell(sub_cell[4]);
			newmesh->add_cell(sub_cell[5]);
			newmesh->add_cell(sub_cell[6]);
			newmesh->add_cell(sub_cell[7]);
			//std::vector<double> material_distribution_temp(8, 1);
			//material_distribution_mutires.push_back(material_distribution_temp);
		}
	}
	material_distribution = material_distribution1;
	return *newmesh;
}

HexV3fMesh& CCSubdivision::CCSubdivising_1()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_new_vertex_1(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_edge_mid_1(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_edge_mid_1(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_edge_mid_1(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_edge_mid_1(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_edge_mid_1(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_edge_mid_1(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_edge_mid_1(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_edge_mid_1(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_edge_mid_1(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_edge_mid_1(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_edge_mid_1(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_edge_mid_1(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

HexV3fMesh& CCSubdivision::Money_right() {
	uint64_t cell_num = mesh->cells_.size();
	VertexHandle temp_vex_local[16];
	HexV3fMesh* newmesh_local = new HexV3fMesh();
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
			temp_vex[mesh_vex_num] = newmesh_local->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
			temp_vex_local[mesh_vex_num] =
				newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
		}
		if (!(mesh->neighbor_c_c(CellHandle(i)).size() == 2 || mesh->neighbor_c_c(CellHandle(i)).size() == 3)) {
			// 不进行优化
			std::vector<VertexHandle> sub_cell;
			sub_cell.push_back(temp_vex_local[0]); sub_cell.push_back(temp_vex_local[1]);
			sub_cell.push_back(temp_vex_local[2]); sub_cell.push_back(temp_vex_local[3]);
			sub_cell.push_back(temp_vex_local[4]); sub_cell.push_back(temp_vex_local[5]);
			sub_cell.push_back(temp_vex_local[6]); sub_cell.push_back(temp_vex_local[7]);
			newmesh->add_cell(sub_cell);
		}
		else {
			//再插入每条边的中点 暴力插入 以免顺序错误
			//第一条边的中点
			V3f edge_mid_1;
			edge_mid_1 = (mesh->vertices_[mesh->cells_[i].vertex_[0]] + mesh->vertices_[mesh->cells_[i].vertex_[1]]) * 1.0 / 2 * 1.0;
			temp_vex[8] = newmesh_local->add_vertex(edge_mid_1);

			//第二条边的中点
			V3f edge_mid_2;
			edge_mid_2 = (mesh->vertices_[mesh->cells_[i].vertex_[1]] + mesh->vertices_[mesh->cells_[i].vertex_[5]]) * 1.0 / 2 * 1.0;
			temp_vex[9] = newmesh_local->add_vertex(edge_mid_2);

			//第三条边的中点
			V3f edge_mid_3;
			edge_mid_3 = (mesh->vertices_[mesh->cells_[i].vertex_[5]] + mesh->vertices_[mesh->cells_[i].vertex_[4]]) * 1.0 / 2 * 1.0;
			temp_vex[10] = newmesh_local->add_vertex(edge_mid_3);

			//第四条边的中点
			V3f edge_mid_4;
			edge_mid_4 = (mesh->vertices_[mesh->cells_[i].vertex_[4]] + mesh->vertices_[mesh->cells_[i].vertex_[0]]) * 1.0 / 2 * 1.0;
			temp_vex[11] = newmesh_local->add_vertex(edge_mid_4);

			//第五条边的中点
			V3f edge_mid_5;
			edge_mid_5 = (mesh->vertices_[mesh->cells_[i].vertex_[3]] + mesh->vertices_[mesh->cells_[i].vertex_[2]]) * 1.0 / 2 * 1.0;
			temp_vex[12] = newmesh_local->add_vertex(edge_mid_5);

			//第六条边的中点
			V3f edge_mid_6;
			edge_mid_6 = (mesh->vertices_[mesh->cells_[i].vertex_[3]] + mesh->vertices_[mesh->cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
			temp_vex[13] = newmesh_local->add_vertex(edge_mid_6);

			//第七条边的中点
			V3f edge_mid_7;
			edge_mid_7 = (mesh->vertices_[mesh->cells_[i].vertex_[6]] + mesh->vertices_[mesh->cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
			temp_vex[14] = newmesh_local->add_vertex(edge_mid_7);

			//第八条边的中点
			V3f edge_mid_8;
			edge_mid_8 = (mesh->vertices_[mesh->cells_[i].vertex_[2]] + mesh->vertices_[mesh->cells_[i].vertex_[6]]) * 1.0 / 2 * 1.0;
			temp_vex[15] = newmesh_local->add_vertex(edge_mid_8);

			//第九条边的中点
			V3f edge_mid_9;
			edge_mid_9 = (mesh->vertices_[mesh->cells_[i].vertex_[1]] + mesh->vertices_[mesh->cells_[i].vertex_[2]]) * 1.0 / 2 * 1.0;
			temp_vex[16] = newmesh_local->add_vertex(edge_mid_9);

			//第十条边的中点
			V3f edge_mid_10;
			edge_mid_10 = (mesh->vertices_[mesh->cells_[i].vertex_[5]] + mesh->vertices_[mesh->cells_[i].vertex_[6]]) * 1.0 / 2 * 1.0;
			temp_vex[17] = newmesh_local->add_vertex(edge_mid_10);

			//第十一条边的中点
			V3f edge_mid_11;
			edge_mid_11 = (mesh->vertices_[mesh->cells_[i].vertex_[0]] + mesh->vertices_[mesh->cells_[i].vertex_[3]]) * 1.0 / 2 * 1.0;
			temp_vex[18] = newmesh_local->add_vertex(edge_mid_11);

			//第十二条边的中点
			V3f edge_mid_12;
			edge_mid_12 = (mesh->vertices_[mesh->cells_[i].vertex_[4]] + mesh->vertices_[mesh->cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
			temp_vex[19] = newmesh_local->add_vertex(edge_mid_12);

			//再插入每个面的中点
			for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
			{
				V3f face_mid;
				face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]]
					+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]]
					+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]]
					+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]]) * 1.0 / 4 * 1.0;
				temp_vex[20 + mesh_face_num] = newmesh_local->add_vertex(face_mid);
			}

			//插入单元的内心
			V3f cell_mid;
			for (int j = 0; j < 8; j++)
			{
				cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
			}
			cell_mid = cell_mid * 1.0 / 8 * 1.0;
			temp_vex[26] = newmesh_local->add_vertex(cell_mid);

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
					cell_mid += newmesh_local->vertices_[iit];
				}
				cell_mid = cell_mid * 1.0 / 8 * 1.0;
				temp_vex_local[index_local + 8] = newmesh->add_vertex(cell_mid);
				index_local++;
			}
			std::vector<std::vector<VertexHandle>> sub_cell_local(7);
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
			newmesh->add_cell(sub_cell_local[0]);
			newmesh->add_cell(sub_cell_local[1]);
			newmesh->add_cell(sub_cell_local[2]);
			newmesh->add_cell(sub_cell_local[3]);
			newmesh->add_cell(sub_cell_local[4]);
			newmesh->add_cell(sub_cell_local[5]);
			newmesh->add_cell(sub_cell_local[6]);
		}
	}
	return *newmesh;
}

HexV3fMesh& CCSubdivision::Padding() {
	uint64_t cell_num = mesh->cells_.size();
	VertexHandle temp_vex_local[16];
	HexV3fMesh* newmesh_local = new HexV3fMesh();

	//for (uint64_t i = 0; i < mesh->cells_.size(); i++) {
	//	std::cout << "[DEBUG] " << i << mesh->neighbor_c_circle_c(CellHandle(i)).size() << std::endl;
	//}
	// 标记 对每一个六面体的相邻面的情况进行标记
	std::map<CellHandle, std::vector<bool>> mapCellLinkCell;
	for (uint64_t i = 0; i < cell_num; i++) {
		std::set<CellHandle> temp;
		std::vector<FaceHandle> c_f = mesh->cells_[i].faces_;
		for (auto c_f_it = c_f.begin(); c_f_it != c_f.end(); c_f_it++)
		{
			std::set<CellHandle, compare_OVM> c_f_c = mesh->neighbor_f[*c_f_it];
			for (auto c_f_c_it = c_f_c.begin(); c_f_c_it != c_f_c.end(); c_f_c_it++)
				if (*c_f_c_it != i) {
					temp.insert(*c_f_c_it);
				}

		}
	}
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[8];
		VertexHandle temp_vex_edge[24];
		VertexHandle temp_vex_face[24];
		VertexHandle temp_vex_cell[8];

		VertexHandle temp_vex_[8];
		VertexHandle temp_vex_edge_[24];
		VertexHandle temp_vex_face_[24];
		VertexHandle temp_vex_cell_[8];
		std::vector<V3f>::iterator v_id;

		// 傻逼了 直接for不就好了

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex[mesh_vex_num] = newmesh_local->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
			temp_vex_[mesh_vex_num] =
				newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
		}
		//再插入每条边的三等分点 暴力插入 以免顺序错误
		// 
		std::vector<std::pair<int, int>> edgeFriends;
		edgeFriends.push_back({ 0,1 }); // 1
		edgeFriends.push_back({ 1,5 }); // 2
		edgeFriends.push_back({ 5,4 }); // 3
		edgeFriends.push_back({ 4,0 }); // 4
		edgeFriends.push_back({ 3,2 }); // 5
		edgeFriends.push_back({ 3,7 }); // 6
		edgeFriends.push_back({ 6,7 }); // 7
		edgeFriends.push_back({ 2,6 }); // 8
		edgeFriends.push_back({ 1,2 }); // 9
		edgeFriends.push_back({ 5,6 }); // 10
		edgeFriends.push_back({ 0,3 }); // 11
		edgeFriends.push_back({ 4,7 }); // 12
		int localIndex = 0;
		for (auto it : edgeFriends) {
			V3f edge_mid_1_0;
			edge_mid_1_0 = (mesh->vertices_[mesh->cells_[i].vertex_[it.first]] * 2.0 + mesh->vertices_[mesh->cells_[i].vertex_[it.second]]) * 1.0 / 3.0 * 1.0;
			temp_vex_edge[localIndex] = newmesh_local->add_vertex(edge_mid_1_0);
			temp_vex_edge_[localIndex] = newmesh->add_vertex(edge_mid_1_0);
			localIndex++;
			V3f edge_mid_1_1;
			edge_mid_1_1 = (mesh->vertices_[mesh->cells_[i].vertex_[it.first]] + mesh->vertices_[mesh->cells_[i].vertex_[it.second]] * 2.0) * 1.0 / 3.0 * 1.0;
			temp_vex_edge[localIndex] = newmesh_local->add_vertex(edge_mid_1_1);
			temp_vex_edge_[localIndex] = newmesh->add_vertex(edge_mid_1_1);
			localIndex++;
		}
		//const uint16_t FACE_LINK_V_INDEX[6 * 4] = {
		//	0,4,5,1, // up
		//	2,6,7,3, // down
		//	1,5,6,2, // front
		//	3,7,4,0, // back
		//	4,7,6,5, // right
		//	1,0,3,2  // left
		//};
		// 每个面再插入4个顶点
		localIndex = 0;
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid_1;
			face_mid_1 = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]] * 2.0 +
				mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]]) * 1.0 / 3.0 * 1.0;
			temp_vex_face[localIndex] = newmesh_local->add_vertex(face_mid_1);
			temp_vex_face_[localIndex] = newmesh->add_vertex(face_mid_1);
			localIndex++;

			V3f face_mid_2;
			face_mid_2 = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]] * 2.0 +
				mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]]) * 1.0 / 3.0 * 1.0;
			temp_vex_face[localIndex] = newmesh_local->add_vertex(face_mid_2);
			temp_vex_face_[localIndex] = newmesh->add_vertex(face_mid_2);
			localIndex++;

			V3f face_mid_3;
			face_mid_3 = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]] +
				mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]] * 2.0) * 1.0 / 3.0 * 1.0;
			temp_vex_face[localIndex] = newmesh_local->add_vertex(face_mid_3);
			temp_vex_face_[localIndex] = newmesh->add_vertex(face_mid_3);
			localIndex++;

			V3f face_mid_4;
			face_mid_4 = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]] +
				mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]] * 2.0) * 1.0 / 3.0 * 1.0;
			temp_vex_face[localIndex] = newmesh_local->add_vertex(face_mid_4);
			temp_vex_face_[localIndex] = newmesh->add_vertex(face_mid_4);
			localIndex++;
		}
		// 插入单元的内心 八个
		std::vector<std::pair<int, int>> cellFriends;
		// 对角
		cellFriends.push_back({ 0, 6 });
		cellFriends.push_back({ 1, 7 });
		cellFriends.push_back({ 2, 4 });
		cellFriends.push_back({ 3, 5 });
		cellFriends.push_back({ 4, 2 });
		cellFriends.push_back({ 5, 3 });
		cellFriends.push_back({ 6, 0 });
		cellFriends.push_back({ 7, 1 });
		localIndex = 0;
		for (auto it : cellFriends) {
			V3f cell_mid;
			cell_mid = (mesh->vertices_[mesh->cells_[i].vertex_[it.first]] * 2.0 + mesh->vertices_[mesh->cells_[i].vertex_[it.second]]) * 1.0 / 3.0 * 1.0;
			temp_vex_cell[localIndex] = newmesh_local->add_vertex(cell_mid);
			temp_vex_cell_[localIndex] = newmesh->add_vertex(cell_mid);
			localIndex++;
		}
		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(27);
		// 第一层 第一列
		sub_cell[0].push_back(temp_vex[0]);  sub_cell[0].push_back(temp_vex_edge[0]);
		sub_cell[0].push_back(temp_vex_face[21]); sub_cell[0].push_back(temp_vex_edge[20]);
		sub_cell[0].push_back(temp_vex_edge[7]); sub_cell[0].push_back(temp_vex_face[0]);
		sub_cell[0].push_back(temp_vex_cell[0]); sub_cell[0].push_back(temp_vex_face[15]);

		sub_cell[1].push_back(temp_vex_edge[0]);  sub_cell[1].push_back(temp_vex_edge[1]);
		sub_cell[1].push_back(temp_vex_face[20]); sub_cell[1].push_back(temp_vex_face[21]);
		sub_cell[1].push_back(temp_vex_face[0]); sub_cell[1].push_back(temp_vex_face[3]);
		sub_cell[1].push_back(temp_vex_cell[1]); sub_cell[1].push_back(temp_vex_cell[0]);

		sub_cell[2].push_back(temp_vex_edge[1]);  sub_cell[2].push_back(temp_vex[1]);
		sub_cell[2].push_back(temp_vex_edge[16]); sub_cell[2].push_back(temp_vex_face[20]);
		sub_cell[2].push_back(temp_vex_face[3]); sub_cell[2].push_back(temp_vex_edge[2]);
		sub_cell[2].push_back(temp_vex_face[8]); sub_cell[2].push_back(temp_vex_cell[1]);
		// 第一层 第二列
		sub_cell[3].push_back(temp_vex_edge[7]);  sub_cell[3].push_back(temp_vex_face[0]);
		sub_cell[3].push_back(temp_vex_cell[0]); sub_cell[3].push_back(temp_vex_face[15]);
		sub_cell[3].push_back(temp_vex_edge[6]); sub_cell[3].push_back(temp_vex_face[1]);
		sub_cell[3].push_back(temp_vex_cell[4]); sub_cell[3].push_back(temp_vex_face[14]);

		sub_cell[4].push_back(temp_vex_face[0]);  sub_cell[4].push_back(temp_vex_face[3]);
		sub_cell[4].push_back(temp_vex_cell[1]); sub_cell[4].push_back(temp_vex_cell[0]);
		sub_cell[4].push_back(temp_vex_face[1]); sub_cell[4].push_back(temp_vex_face[2]);
		sub_cell[4].push_back(temp_vex_cell[5]); sub_cell[4].push_back(temp_vex_cell[4]);

		sub_cell[5].push_back(temp_vex_face[3]);  sub_cell[5].push_back(temp_vex_edge[2]);
		sub_cell[5].push_back(temp_vex_face[8]); sub_cell[5].push_back(temp_vex_cell[1]);
		sub_cell[5].push_back(temp_vex_face[2]); sub_cell[5].push_back(temp_vex_edge[3]);
		sub_cell[5].push_back(temp_vex_face[9]); sub_cell[5].push_back(temp_vex_cell[5]);
		// 第一层 第三列
		sub_cell[6].push_back(temp_vex_edge[6]);  sub_cell[6].push_back(temp_vex_face[1]);
		sub_cell[6].push_back(temp_vex_cell[4]); sub_cell[6].push_back(temp_vex_face[14]);
		sub_cell[6].push_back(temp_vex[4]); sub_cell[6].push_back(temp_vex_edge[5]);
		sub_cell[6].push_back(temp_vex_face[16]); sub_cell[6].push_back(temp_vex_edge[22]);

		sub_cell[7].push_back(temp_vex_face[1]);  sub_cell[7].push_back(temp_vex_face[2]);
		sub_cell[7].push_back(temp_vex_cell[5]); sub_cell[7].push_back(temp_vex_cell[4]);
		sub_cell[7].push_back(temp_vex_edge[5]); sub_cell[7].push_back(temp_vex_edge[4]);
		sub_cell[7].push_back(temp_vex_face[19]); sub_cell[7].push_back(temp_vex_face[16]);

		sub_cell[8].push_back(temp_vex_face[2]);  sub_cell[8].push_back(temp_vex_edge[3]);
		sub_cell[8].push_back(temp_vex_face[9]); sub_cell[8].push_back(temp_vex_cell[5]);
		sub_cell[8].push_back(temp_vex_edge[4]); sub_cell[8].push_back(temp_vex[5]);
		sub_cell[8].push_back(temp_vex_edge[18]); sub_cell[8].push_back(temp_vex_face[19]);

		// 第二层 第一列
		sub_cell[9].push_back(temp_vex_edge[20]);  sub_cell[9].push_back(temp_vex_face[21]);
		sub_cell[9].push_back(temp_vex_face[22]); sub_cell[9].push_back(temp_vex_edge[21]);
		sub_cell[9].push_back(temp_vex_face[15]); sub_cell[9].push_back(temp_vex_cell[0]);
		sub_cell[9].push_back(temp_vex_cell[3]); sub_cell[9].push_back(temp_vex_face[12]);

		sub_cell[10].push_back(temp_vex_face[21]);  sub_cell[10].push_back(temp_vex_face[20]);
		sub_cell[10].push_back(temp_vex_face[23]); sub_cell[10].push_back(temp_vex_face[22]);
		sub_cell[10].push_back(temp_vex_cell[0]); sub_cell[10].push_back(temp_vex_cell[1]);
		sub_cell[10].push_back(temp_vex_cell[2]); sub_cell[10].push_back(temp_vex_cell[3]);

		sub_cell[11].push_back(temp_vex_face[20]);  sub_cell[11].push_back(temp_vex_edge[16]);
		sub_cell[11].push_back(temp_vex_edge[17]); sub_cell[11].push_back(temp_vex_face[23]);
		sub_cell[11].push_back(temp_vex_cell[1]); sub_cell[11].push_back(temp_vex_face[8]);
		sub_cell[11].push_back(temp_vex_face[11]); sub_cell[11].push_back(temp_vex_cell[2]);
		// 第二层 第二列
		sub_cell[12].push_back(temp_vex_face[15]);  sub_cell[12].push_back(temp_vex_cell[0]);
		sub_cell[12].push_back(temp_vex_cell[3]); sub_cell[12].push_back(temp_vex_face[12]);
		sub_cell[12].push_back(temp_vex_face[14]); sub_cell[12].push_back(temp_vex_cell[4]);
		sub_cell[12].push_back(temp_vex_cell[7]); sub_cell[12].push_back(temp_vex_face[13]);

		sub_cell[13].push_back(temp_vex_cell[0]);  sub_cell[13].push_back(temp_vex_cell[1]);
		sub_cell[13].push_back(temp_vex_cell[2]); sub_cell[13].push_back(temp_vex_cell[3]);
		sub_cell[13].push_back(temp_vex_cell[4]); sub_cell[13].push_back(temp_vex_cell[5]);
		sub_cell[13].push_back(temp_vex_cell[6]); sub_cell[13].push_back(temp_vex_cell[7]);

		sub_cell[14].push_back(temp_vex_cell[1]);  sub_cell[14].push_back(temp_vex_face[8]);
		sub_cell[14].push_back(temp_vex_face[11]); sub_cell[14].push_back(temp_vex_cell[2]);
		sub_cell[14].push_back(temp_vex_cell[5]); sub_cell[14].push_back(temp_vex_face[9]);
		sub_cell[14].push_back(temp_vex_face[10]); sub_cell[14].push_back(temp_vex_cell[6]);
		// 第二层 第三列
		sub_cell[15].push_back(temp_vex_face[14]);  sub_cell[15].push_back(temp_vex_cell[4]);
		sub_cell[15].push_back(temp_vex_cell[7]); sub_cell[15].push_back(temp_vex_face[13]);
		sub_cell[15].push_back(temp_vex_edge[22]); sub_cell[15].push_back(temp_vex_face[16]);
		sub_cell[15].push_back(temp_vex_face[17]); sub_cell[15].push_back(temp_vex_edge[23]);

		sub_cell[16].push_back(temp_vex_cell[4]);  sub_cell[16].push_back(temp_vex_cell[5]);
		sub_cell[16].push_back(temp_vex_cell[6]); sub_cell[16].push_back(temp_vex_cell[7]);
		sub_cell[16].push_back(temp_vex_face[16]); sub_cell[16].push_back(temp_vex_face[19]);
		sub_cell[16].push_back(temp_vex_face[18]); sub_cell[16].push_back(temp_vex_face[17]);

		sub_cell[17].push_back(temp_vex_cell[5]);  sub_cell[17].push_back(temp_vex_face[9]);
		sub_cell[17].push_back(temp_vex_face[10]); sub_cell[17].push_back(temp_vex_cell[6]);
		sub_cell[17].push_back(temp_vex_face[19]); sub_cell[17].push_back(temp_vex_edge[18]);
		sub_cell[17].push_back(temp_vex_edge[19]); sub_cell[17].push_back(temp_vex_face[18]);

		// 第三层 第一列
		sub_cell[18].push_back(temp_vex_edge[21]);  sub_cell[18].push_back(temp_vex_face[22]);
		sub_cell[18].push_back(temp_vex_edge[8]); sub_cell[18].push_back(temp_vex[3]);
		sub_cell[18].push_back(temp_vex_face[12]); sub_cell[18].push_back(temp_vex_cell[3]);
		sub_cell[18].push_back(temp_vex_face[7]); sub_cell[18].push_back(temp_vex_edge[10]);

		sub_cell[19].push_back(temp_vex_face[22]);  sub_cell[19].push_back(temp_vex_face[23]);
		sub_cell[19].push_back(temp_vex_edge[9]); sub_cell[19].push_back(temp_vex_edge[8]);
		sub_cell[19].push_back(temp_vex_cell[3]); sub_cell[19].push_back(temp_vex_cell[2]);
		sub_cell[19].push_back(temp_vex_face[4]); sub_cell[19].push_back(temp_vex_face[7]);

		sub_cell[20].push_back(temp_vex_face[23]);  sub_cell[20].push_back(temp_vex_edge[17]);
		sub_cell[20].push_back(temp_vex[2]); sub_cell[20].push_back(temp_vex_edge[9]);
		sub_cell[20].push_back(temp_vex_cell[2]); sub_cell[20].push_back(temp_vex_face[11]);
		sub_cell[20].push_back(temp_vex_edge[14]); sub_cell[20].push_back(temp_vex_face[4]);
		// 第三层 第二列
		sub_cell[21].push_back(temp_vex_face[12]);  sub_cell[21].push_back(temp_vex_cell[3]);
		sub_cell[21].push_back(temp_vex_face[7]); sub_cell[21].push_back(temp_vex_edge[10]);
		sub_cell[21].push_back(temp_vex_face[13]); sub_cell[21].push_back(temp_vex_cell[7]);
		sub_cell[21].push_back(temp_vex_face[6]); sub_cell[21].push_back(temp_vex_edge[11]);

		sub_cell[22].push_back(temp_vex_cell[3]);  sub_cell[22].push_back(temp_vex_cell[2]);
		sub_cell[22].push_back(temp_vex_face[4]); sub_cell[22].push_back(temp_vex_face[7]);
		sub_cell[22].push_back(temp_vex_cell[7]); sub_cell[22].push_back(temp_vex_cell[6]);
		sub_cell[22].push_back(temp_vex_face[5]); sub_cell[22].push_back(temp_vex_face[6]);

		sub_cell[23].push_back(temp_vex_cell[2]);  sub_cell[23].push_back(temp_vex_face[11]);
		sub_cell[23].push_back(temp_vex_edge[14]); sub_cell[23].push_back(temp_vex_face[4]);
		sub_cell[23].push_back(temp_vex_cell[6]); sub_cell[23].push_back(temp_vex_face[10]);
		sub_cell[23].push_back(temp_vex_edge[15]); sub_cell[23].push_back(temp_vex_face[5]);
		// 第三层 第三列
		sub_cell[24].push_back(temp_vex_face[13]);  sub_cell[24].push_back(temp_vex_cell[7]);
		sub_cell[24].push_back(temp_vex_face[6]); sub_cell[24].push_back(temp_vex_edge[11]);
		sub_cell[24].push_back(temp_vex_edge[23]); sub_cell[24].push_back(temp_vex_face[17]);
		sub_cell[24].push_back(temp_vex_edge[13]); sub_cell[24].push_back(temp_vex[7]);

		sub_cell[25].push_back(temp_vex_cell[7]);  sub_cell[25].push_back(temp_vex_cell[6]);
		sub_cell[25].push_back(temp_vex_face[5]); sub_cell[25].push_back(temp_vex_face[6]);
		sub_cell[25].push_back(temp_vex_face[17]); sub_cell[25].push_back(temp_vex_face[18]);
		sub_cell[25].push_back(temp_vex_edge[12]); sub_cell[25].push_back(temp_vex_edge[13]);

		sub_cell[26].push_back(temp_vex_cell[6]);  sub_cell[26].push_back(temp_vex_face[10]);
		sub_cell[26].push_back(temp_vex_edge[15]); sub_cell[26].push_back(temp_vex_face[5]);
		sub_cell[26].push_back(temp_vex_face[18]); sub_cell[26].push_back(temp_vex_edge[19]);
		sub_cell[26].push_back(temp_vex[6]); sub_cell[26].push_back(temp_vex_edge[12]);
		for (int i_dd = 0; i_dd < 27; i_dd++) {
			newmesh->add_cell(sub_cell[i_dd]);
		}
		//std::vector<std::vector<VertexHandle>> sub_cell_local(27);
		//sub_cell_local.assign(sub_cell.begin(), sub_cell.end());



		/*newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
		newmesh->add_cell(sub_cell[8]);
		newmesh->add_cell(sub_cell[9]);
		newmesh->add_cell(sub_cell[10]);
		newmesh->add_cell(sub_cell[11]);
		newmesh->add_cell(sub_cell[12]);
		newmesh->add_cell(sub_cell[13]);
		newmesh->add_cell(sub_cell[14]);
		newmesh->add_cell(sub_cell[15]);
		newmesh->add_cell(sub_cell[16]);
		newmesh->add_cell(sub_cell[17]);
		newmesh->add_cell(sub_cell[18]);
		newmesh->add_cell(sub_cell[19]);
		newmesh->add_cell(sub_cell[20]);
		newmesh->add_cell(sub_cell[21]);
		newmesh->add_cell(sub_cell[22]);
		newmesh->add_cell(sub_cell[23]);
		newmesh->add_cell(sub_cell[24]);
		newmesh->add_cell(sub_cell[25]);
		newmesh->add_cell(sub_cell[26]);*/


	}
	return *newmesh;
}

// 实现padding 必须重新读取网格文件之后直接使用padding程序, 因为subdivision可能会出现某些数据结构没有更新的问题.
HexV3fMesh& CCSubdivision::Money() {
	uint64_t cell_num = mesh->cells_.size();
	VertexHandle temp_vex_local[16];
	HexV3fMesh* newmesh_local = new HexV3fMesh();
	/*
	std::unordered_map<VertexHandle, bool> check;
	std::unordered_map<VertexHandle, V3f> oldPosition;
	std::unordered_map<VertexHandle, V3f> newPosition;
	std::unordered_map<FaceHandle, V3f> faceVec;
	*/
	std::unordered_map<int, bool> check;
	std::unordered_map<int, V3f> oldPosition;
	std::unordered_map<int, V3f> newPosition;
	std::unordered_map<int, V3f> faceVec;
	
	for (uint64_t i = 0; i < cell_num; i++) {
		std::vector<VertexHandle> sub_cell;
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex_local[mesh_vex_num] =
				newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
			sub_cell.push_back(temp_vex_local[mesh_vex_num]);
		}
		newmesh->add_cell(sub_cell);
	}
	newmesh->examine_bdy();
	newmesh->cal_cen();
	newmesh->examine_bdfv();
	newmesh->cal_cell_cen();
	newmesh->cal_bdnorm();
	newmesh->cal_mami_ed();

	// 将边界面的法向量设定为面片中心指向体中心的向量和的均值
	for (uint64_t i = 0; i < cell_num; i++) {
		if (newmesh->neighbor_c_c(CellHandle(i)).size() < 6) {
			for (int index = 0; index < 6; index++) {
				if (newmesh->is_bdy(newmesh->cells_[CellHandle(i)].faces_[index])) {
					V3f faceMid = (newmesh->vertices_[newmesh->faces_[newmesh->cells_[CellHandle(i)].faces_[index]].vs_f[0]] +
						newmesh->vertices_[newmesh->faces_[newmesh->cells_[CellHandle(i)].faces_[index]].vs_f[1]] +
						newmesh->vertices_[newmesh->faces_[newmesh->cells_[CellHandle(i)].faces_[index]].vs_f[2]] +
						newmesh->vertices_[newmesh->faces_[newmesh->cells_[CellHandle(i)].faces_[index]].vs_f[3]]) * 1.0 / 4.0;
					V3f HexMid = (newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[0]] +
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[1]] +
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[2]] +
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[3]] +
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[4]] +
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[5]] +
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[6]] +
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[7]]) * 1.0 / 8.0;
					V3f vecTemp = HexMid - faceMid;
					faceVec[newmesh->cells_[CellHandle(i)].faces_[index]] = vecTemp;
				}
			}
		}
	}

	for (uint64_t i = 0; i < cell_num; i++) {
		// 将所有的边界面面片沿着周围的面片法向量和的均值的反方向进行移动一定的数值，
		// 然后将旧边界面所对应的六面体和新边界面所对应的六面体的面和成一个新的六面体
		if (newmesh->neighbor_c_c(CellHandle(i)).size() < 6) {
			for (int index = 0; index < 8; index++) {
				if (check[newmesh->cells_[CellHandle(i)].vertex_[index]]) {
					continue;
				}
				std::set<EdgeHandle> s = newmesh->neighbor_v_bdy(newmesh->cells_[CellHandle(i)].vertex_[index]);
				if (s.size() > 0) {
					// 计算相邻边界边的向量和
					check[newmesh->cells_[CellHandle(i)].vertex_[index]] = true; // 标记为已经迁移过
					V3f vec(0, 0, 0);
					std::set<FaceHandle, compare_OVM> ff; // 一个顶点连接的边界面
					for (auto it : s) {
						for (auto itt : newmesh->neighbor_e_bdy(it)) {
							ff.insert(itt);
						}
					}
					for (auto it : ff) {
						vec += faceVec[it];
					}
					vec /= ff.size();
					vec *= 0.5; // 迁移 0.1 个单位向量 不密集 0.5
					newPosition[newmesh->cells_[CellHandle(i)].vertex_[index]] =
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[index]] + vec;
					oldPosition[newmesh->cells_[CellHandle(i)].vertex_[index]] =
						newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[index]];
				}
			}	
		}
	}
	// 更改点
	for (uint64_t i = 0; i < cell_num; i++) {
		// 将所有的边界面面片沿着周围的面片法向量和的均值的反方向进行移动一定的数值，
		// 然后将旧边界面所对应的六面体和新边界面所对应的六面体的面和成一个新的六面体
		if (newmesh->neighbor_c_c(CellHandle(i)).size() < 6) {
			// 不进行优化
			for (int index = 0; index < 8; index++) {
				if (check[newmesh->cells_[CellHandle(i)].vertex_[index]]) {
					newmesh->vertices_[newmesh->cells_[CellHandle(i)].vertex_[index]] = 
						newPosition[newmesh->cells_[CellHandle(i)].vertex_[index]];
					newmesh->quick_v.erase(oldPosition[newmesh->cells_[CellHandle(i)].vertex_[index]]);
					newmesh->quick_v[newPosition[newmesh->cells_[CellHandle(i)].vertex_[index]]] = 
						newmesh->cells_[CellHandle(i)].vertex_[index];
		
				}
			}
		}
	}
	/*for (uint64_t i = 0; i < cell_num; i++) {
		std::vector<VertexHandle> sub_cell;
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex_local[mesh_vex_num] =
				newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
			sub_cell.push_back(temp_vex_local[mesh_vex_num]);
		}
		newmesh->add_cell(sub_cell);
	}*/
	// 新增点
	std::unordered_map<int, int> vv; // vertexHandle vertexHandle
	for (auto it : oldPosition) {
		vv[it.first] = newmesh->add_vertex(it.second);
	}
	std::unordered_map<int, bool> checkFace;
	std::vector<std::vector<VertexHandle>> ce;
	for (auto it : faceVec) {
		std::vector<VertexHandle> sub_cell;
		sub_cell.push_back(newmesh->faces_[it.first].vs_f[0]);
		sub_cell.push_back(newmesh->faces_[it.first].vs_f[1]);
		sub_cell.push_back(newmesh->faces_[it.first].vs_f[2]);
		sub_cell.push_back(newmesh->faces_[it.first].vs_f[3]);
		sub_cell.push_back((VertexHandle)vv[newmesh->faces_[it.first].vs_f[0]]);
		sub_cell.push_back((VertexHandle)vv[newmesh->faces_[it.first].vs_f[1]]);
		sub_cell.push_back((VertexHandle)vv[newmesh->faces_[it.first].vs_f[2]]);
		sub_cell.push_back((VertexHandle)vv[newmesh->faces_[it.first].vs_f[3]]);
		ce.push_back(sub_cell);
	}
	for (auto it : ce) {
		newmesh->add_cell(it);
	}
	return *newmesh;
}

/*

HexV3fMesh& CCSubdivision::Money() {
	uint64_t cell_num = mesh->cells_.size();
	VertexHandle temp_vex_local[16];
	HexV3fMesh* newmesh_local = new HexV3fMesh();
	
	//std::unordered_map<VertexHandle, bool> check;
	//std::unordered_map<VertexHandle, V3f> oldPosition;
	//std::unordered_map<VertexHandle, V3f> newPosition;
	//std::unordered_map<FaceHandle, V3f> faceVec;
	
std::unordered_map<int, bool> check;
std::unordered_map<int, V3f> oldPosition;
std::unordered_map<int, V3f> newPosition;
std::unordered_map<int, V3f> faceVec;

// 将边界面的法向量设定为面片中心指向体中心的向量和的均值
for (uint64_t i = 0; i < cell_num; i++) {
	if (mesh->neighbor_c_c(CellHandle(i)).size() < 6) {
		for (int index = 0; index < 6; index++) {
			if (mesh->is_bdy(mesh->cells_[CellHandle(i)].faces_[index])) {
				V3f faceMid = (mesh->vertices_[mesh->faces_[mesh->cells_[CellHandle(i)].faces_[index]].vs_f[0]] +
					mesh->vertices_[mesh->faces_[mesh->cells_[CellHandle(i)].faces_[index]].vs_f[1]] +
					mesh->vertices_[mesh->faces_[mesh->cells_[CellHandle(i)].faces_[index]].vs_f[2]] +
					mesh->vertices_[mesh->faces_[mesh->cells_[CellHandle(i)].faces_[index]].vs_f[3]]) * 1.0 / 4.0;
				V3f HexMid = (mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[0]] +
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[1]] +
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[2]] +
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[3]] +
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[4]] +
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[5]] +
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[6]] +
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[7]]) * 1.0 / 8.0;
				V3f vecTemp = HexMid - faceMid;
				faceVec[mesh->cells_[CellHandle(i)].faces_[index]] = vecTemp;
			}
		}
	}
}

for (uint64_t i = 0; i < cell_num; i++) {
	// 将所有的边界面面片沿着周围的面片法向量和的均值的反方向进行移动一定的数值，
	// 然后将旧边界面所对应的六面体和新边界面所对应的六面体的面和成一个新的六面体
	if (mesh->neighbor_c_c(CellHandle(i)).size() < 6) {
		// 不进行优化
		for (int index = 0; index < 8; index++) {
			if (check[mesh->cells_[CellHandle(i)].vertex_[index]]) {
				continue;
			}
			std::set<EdgeHandle> s = mesh->neighbor_v_bdy(mesh->cells_[CellHandle(i)].vertex_[index]);
			if (s.size() > 0) {
				// 计算相邻边界边的向量和
				check[mesh->cells_[CellHandle(i)].vertex_[index]] = true; // 标记为已经迁移过
				V3f vec(0, 0, 0);
				std::set<FaceHandle, compare_OVM> ff; // 一个顶点连接的边界面
				for (auto it : s) {
					for (auto itt : mesh->neighbor_e_bdy(it)) {
						ff.insert(itt);
					}
				}
				for (auto it : ff) {
					vec += faceVec[it];
				}
				vec /= ff.size();
				vec *= 0.5; // 迁移 0.1 个单位向量
				newPosition[mesh->cells_[CellHandle(i)].vertex_[index]] =
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[index]] + vec;
				oldPosition[mesh->cells_[CellHandle(i)].vertex_[index]] =
					mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[index]];
			}
		}
	}
}
// 更改点
for (uint64_t i = 0; i < cell_num; i++) {
	// 将所有的边界面面片沿着周围的面片法向量和的均值的反方向进行移动一定的数值，
	// 然后将旧边界面所对应的六面体和新边界面所对应的六面体的面和成一个新的六面体
	if (mesh->neighbor_c_c(CellHandle(i)).size() < 6) {
		// 不进行优化
		for (int index = 0; index < 8; index++) {
			if (check[mesh->cells_[CellHandle(i)].vertex_[index]]) {
				mesh->vertices_[mesh->cells_[CellHandle(i)].vertex_[index]] = newPosition[mesh->cells_[CellHandle(i)].vertex_[index]];
				mesh->quick_v.erase(oldPosition[mesh->cells_[CellHandle(i)].vertex_[index]]);
				mesh->quick_v[newPosition[mesh->cells_[CellHandle(i)].vertex_[index]]] = mesh->cells_[CellHandle(i)].vertex_[index];
			}
		}
	}
}
// 新增点
std::unordered_map<int, int> vv; // vertexHandle vertexHandle
for (auto it : oldPosition) {
	vv[it.first] = mesh->add_vertex(it.second);
}
std::unordered_map<int, bool> checkFace;
std::vector<std::vector<VertexHandle>> ce;
for (auto it : faceVec) {
	std::vector<VertexHandle> sub_cell;
	sub_cell.push_back(mesh->faces_[it.first].vs_f[0]);
	sub_cell.push_back(mesh->faces_[it.first].vs_f[1]);
	sub_cell.push_back(mesh->faces_[it.first].vs_f[2]);
	sub_cell.push_back(mesh->faces_[it.first].vs_f[3]);
	sub_cell.push_back((VertexHandle)vv[mesh->faces_[it.first].vs_f[0]]);
	sub_cell.push_back((VertexHandle)vv[mesh->faces_[it.first].vs_f[1]]);
	sub_cell.push_back((VertexHandle)vv[mesh->faces_[it.first].vs_f[2]]);
	sub_cell.push_back((VertexHandle)vv[mesh->faces_[it.first].vs_f[3]]);
	ce.push_back(sub_cell);
}
for (auto it : ce) {
	mesh->add_cell(it);
}
return *mesh;
}
*/


/// <summary>
/// 发现出错了
/// </summary>
/// <returns></returns>
/// 
/*
HexV3fMesh& CCSubdivision::Money_false() {
	uint64_t cell_num = mesh->cells_.size();
	VertexHandle temp_vex_local[16];
	HexV3fMesh* newmesh_local = new HexV3fMesh();
	std::vector<CellHandle> needSub;
	std::vector<CellHandle> needSubAdd_1;
	std::vector<CellHandle> needSubAdd_2;
	for (uint64_t i = 0; i < mesh->cells_.size(); i++) {
		if (mesh->neighbor_c_circle_c(CellHandle(i)).size() == 5) {
			needSub.push_back(CellHandle(i));
		}
		else if (mesh->neighbor_c_circle_c(CellHandle(i)).size() == 4) {
			needSubAdd_1.push_back(CellHandle(i));
		}
		else if (mesh->neighbor_c_circle_c(CellHandle(i)).size() == 3) {
			needSubAdd_2.push_back(CellHandle(i));
		}
	}
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		VertexHandle temp_vex_local[27];
		std::vector<V3f>::iterator v_id;

		// 傻逼了 直接for不就好了

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex[mesh_vex_num] = newmesh_local->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
			temp_vex_local[mesh_vex_num] =
				newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
		}
		// 计算一个六面体的边界边的个数
		//is_bdy
		int bdyEdgeNum = mesh->cells_[CellHandle(i)].edges_.size();
		int bdyEdgeNums = 0;
		std::vector<int> bdyEdge;
		for (int j = 0; j < bdyEdgeNum; j++) {
			if (mesh->is_bdy(mesh->cells_[CellHandle(i)].edges_[j])) {
				bdyEdgeNums++;
				bdyEdge.push_back(j);
			}
		}
		if ((mesh->neighbor_c_c(CellHandle(i)).size() == 6) && bdyEdgeNums == 0) {
			// 不进行优化
			std::vector<VertexHandle> sub_cell;
			sub_cell.push_back(temp_vex_local[0]); sub_cell.push_back(temp_vex_local[1]);
			sub_cell.push_back(temp_vex_local[2]); sub_cell.push_back(temp_vex_local[3]);
			sub_cell.push_back(temp_vex_local[4]); sub_cell.push_back(temp_vex_local[5]);
			sub_cell.push_back(temp_vex_local[6]); sub_cell.push_back(temp_vex_local[7]);
			newmesh->add_cell(sub_cell);
		}
		else {
			//第一条边的中点
			V3f edge_mid_1;
			edge_mid_1 = (mesh->vertices_[mesh->cells_[i].vertex_[0]] + mesh->vertices_[mesh->cells_[i].vertex_[1]]) * 1.0 / 2 * 1.0;
			temp_vex[8] = newmesh_local->add_vertex(edge_mid_1);

			//第二条边的中点
			V3f edge_mid_2;
			edge_mid_2 = (mesh->vertices_[mesh->cells_[i].vertex_[1]] + mesh->vertices_[mesh->cells_[i].vertex_[5]]) * 1.0 / 2 * 1.0;
			temp_vex[9] = newmesh_local->add_vertex(edge_mid_2);

			//第三条边的中点
			V3f edge_mid_3;
			edge_mid_3 = (mesh->vertices_[mesh->cells_[i].vertex_[5]] + mesh->vertices_[mesh->cells_[i].vertex_[4]]) * 1.0 / 2 * 1.0;
			temp_vex[10] = newmesh_local->add_vertex(edge_mid_3);

			//第四条边的中点
			V3f edge_mid_4;
			edge_mid_4 = (mesh->vertices_[mesh->cells_[i].vertex_[4]] + mesh->vertices_[mesh->cells_[i].vertex_[0]]) * 1.0 / 2 * 1.0;
			temp_vex[11] = newmesh_local->add_vertex(edge_mid_4);

			//第五条边的中点
			V3f edge_mid_5;
			edge_mid_5 = (mesh->vertices_[mesh->cells_[i].vertex_[3]] + mesh->vertices_[mesh->cells_[i].vertex_[2]]) * 1.0 / 2 * 1.0;
			temp_vex[12] = newmesh_local->add_vertex(edge_mid_5);

			//第六条边的中点
			V3f edge_mid_6;
			edge_mid_6 = (mesh->vertices_[mesh->cells_[i].vertex_[3]] + mesh->vertices_[mesh->cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
			temp_vex[13] = newmesh_local->add_vertex(edge_mid_6);

			//第七条边的中点
			V3f edge_mid_7;
			edge_mid_7 = (mesh->vertices_[mesh->cells_[i].vertex_[6]] + mesh->vertices_[mesh->cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
			temp_vex[14] = newmesh_local->add_vertex(edge_mid_7);

			//第八条边的中点
			V3f edge_mid_8;
			edge_mid_8 = (mesh->vertices_[mesh->cells_[i].vertex_[2]] + mesh->vertices_[mesh->cells_[i].vertex_[6]]) * 1.0 / 2 * 1.0;
			temp_vex[15] = newmesh_local->add_vertex(edge_mid_8);

			//第九条边的中点
			V3f edge_mid_9;
			edge_mid_9 = (mesh->vertices_[mesh->cells_[i].vertex_[1]] + mesh->vertices_[mesh->cells_[i].vertex_[2]]) * 1.0 / 2 * 1.0;
			temp_vex[16] = newmesh_local->add_vertex(edge_mid_9);

			//第十条边的中点
			V3f edge_mid_10;
			edge_mid_10 = (mesh->vertices_[mesh->cells_[i].vertex_[5]] + mesh->vertices_[mesh->cells_[i].vertex_[6]]) * 1.0 / 2 * 1.0;
			temp_vex[17] = newmesh_local->add_vertex(edge_mid_10);

			//第十一条边的中点
			V3f edge_mid_11;
			edge_mid_11 = (mesh->vertices_[mesh->cells_[i].vertex_[0]] + mesh->vertices_[mesh->cells_[i].vertex_[3]]) * 1.0 / 2 * 1.0;
			temp_vex[18] = newmesh_local->add_vertex(edge_mid_11);

			//第十二条边的中点
			V3f edge_mid_12;
			edge_mid_12 = (mesh->vertices_[mesh->cells_[i].vertex_[4]] + mesh->vertices_[mesh->cells_[i].vertex_[7]]) * 1.0 / 2 * 1.0;
			temp_vex[19] = newmesh_local->add_vertex(edge_mid_12);
			//再插入每个面的中点
			for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++) {
				V3f face_mid;
				face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]]
					+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]]
					+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]]
					+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]]) * 1.0 / 4 * 1.0;
				temp_vex[20 + mesh_face_num] = newmesh_local->add_vertex(face_mid);
			}
			if (mesh->neighbor_c_c(CellHandle(i)).size() == 6 && bdyEdgeNums == 1) {
				// 找到那一条边
				// 边对应的面
				std::vector<std::vector<VertexHandle>> sub_cell(3);
				if (bdyEdge[0] == 0) {
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_4); // 40 
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_11); // 03
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_2); // 15
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_9); // 12
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[0]] // back
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[12] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[0]] // front
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[13] = newmesh->add_vertex(face_mid);
					// 带有边界边的体
					int pointIndex1[8] = { 0, 1, 11, 9, 8, 10, 13, 12 };
					for (auto it : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[it]);
					}
					// 不带有边界边的体
					int pointIndex2[8] = { 9, 11, 2, 3, 12, 13, 6, 7 };
					for (auto it : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[it]);
					}
					int pointIndex3[8] = { 8, 10, 13, 12, 4, 5, 6, 7 };
					for (auto it : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[it]);
					}
				}
				else if (bdyEdge[0] == 1) {
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1); // 10
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_9); // 12
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3); // 54
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_10); // 56
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]] // right
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[12] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]] // left
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[13] = newmesh->add_vertex(face_mid);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 带有边界边的体
					int pointIndex1[8] = { 8, 1, 9, 13, 10, 5, 11, 12 };
					for (auto it : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[it]);
					}
					// 不带有边界边的体
					int pointIndex2[8] = { 13, 9, 2, 3, 12, 11, 6, 7 };
					for (auto it : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[it]);
					}
					int pointIndex3[8] = { 0, 8, 13, 3, 4, 10, 12, 7 };
					for (auto it : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[it]);
					}
				}
				else if (bdyEdge[0] == 2) {
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_4); // 04
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_12); // 47
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_2); // 15
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_10); // 56
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[0]] // front
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[12] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[0]] // back
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[13] = newmesh->add_vertex(face_mid);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 带有边界边的体
					int pointIndex1[8] = { 8, 10, 12, 13, 4, 5, 11, 9 };
					for (auto it : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[it]);
					}
					// 不带有边界边的体
					int pointIndex2[8] = { 13, 12, 2, 3, 9, 11, 6, 7 };
					for (auto it : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[it]);
					}
					int pointIndex3[8] = { 0, 1, 2, 3, 8, 10, 12, 13 };
					for (auto it : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[it]);
					}
				}
				else if (bdyEdge[0] == 3) { // 40
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1); // 01
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_11); // 03
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3); // 45
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_12); // 47
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]] // left
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[12] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]] // right
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[13] = newmesh->add_vertex(face_mid);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 带有边界边的体   0, `01`, left, `03`, 4, `45`, right, `47`
					int pointIndex1[8] = { 0, 8, 12, 9, 4, 10, 12, 11 };
					for (auto it : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[it]);
					}
					// 不带有边界边的体 `01`, 1, 2, left, `45`, 5, 6, right
					int pointIndex2[8] = { 8, 1, 2, 12, 10, 5, 6, 13 };
					for (auto it : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[it]);
					}
					// `03`,left, 2, 3, `47`, right, 6, 7
					int pointIndex3[8] = { 9, 12, 2, 3, 11, 13, 6, 7 };
					for (auto it : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[it]);
					}
				}
				else if (bdyEdge[0] == 4) { // 32
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					// 16 15 13 18 边点
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的六面体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[16]); sub_cell[0].push_back(temp_vex_local[18]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[22]); sub_cell[0].push_back(temp_vex_local[23]);
					// 右侧的六面体
					sub_cell[1].push_back(temp_vex_local[4]); sub_cell[1].push_back(temp_vex_local[5]);
					sub_cell[1].push_back(temp_vex_local[22]); sub_cell[1].push_back(temp_vex_local[23]);
					sub_cell[1].push_back(temp_vex_local[7]); sub_cell[1].push_back(temp_vex_local[6]);
					sub_cell[1].push_back(temp_vex_local[15]); sub_cell[1].push_back(temp_vex_local[13]);
					// 角落的六面体
					sub_cell[2].push_back(temp_vex_local[18]); sub_cell[2].push_back(temp_vex_local[16]);
					sub_cell[2].push_back(temp_vex_local[2]); sub_cell[2].push_back(temp_vex_local[3]);
					sub_cell[2].push_back(temp_vex_local[23]); sub_cell[2].push_back(temp_vex_local[22]);
					sub_cell[2].push_back(temp_vex_local[15]); sub_cell[2].push_back(temp_vex_local[13]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (bdyEdge[0] == 5) { // 37
					// 上面和前面
					// 面点 4 右边 5 左边
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 14 19 12 18 边点
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[25]); sub_cell[0].push_back(temp_vex_local[18]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[24]); sub_cell[0].push_back(temp_vex_local[19]);
					// 前面的体
					sub_cell[1].push_back(temp_vex_local[25]); sub_cell[1].push_back(temp_vex_local[1]);
					sub_cell[1].push_back(temp_vex_local[2]); sub_cell[1].push_back(temp_vex_local[12]);
					sub_cell[1].push_back(temp_vex_local[24]); sub_cell[1].push_back(temp_vex_local[5]);
					sub_cell[1].push_back(temp_vex_local[6]); sub_cell[1].push_back(temp_vex_local[14]);
					// 角落的体
					sub_cell[2].push_back(temp_vex_local[18]); sub_cell[2].push_back(temp_vex_local[25]);
					sub_cell[2].push_back(temp_vex_local[12]); sub_cell[2].push_back(temp_vex_local[3]);
					sub_cell[2].push_back(temp_vex_local[19]); sub_cell[2].push_back(temp_vex_local[24]);
					sub_cell[2].push_back(temp_vex_local[14]); sub_cell[2].push_back(temp_vex_local[7]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (bdyEdge[0] == 6) { // 76
					// 上面 & 左边
					// 面点 2 & 3  前面 和 后面
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					// 16 15 13 18 边点
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的六面体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[22]); sub_cell[0].push_back(temp_vex_local[23]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[17]); sub_cell[0].push_back(temp_vex_local[19]);
					// 左侧的六面体
					sub_cell[1].push_back(temp_vex_local[0]); sub_cell[1].push_back(temp_vex_local[1]);
					sub_cell[1].push_back(temp_vex_local[2]); sub_cell[1].push_back(temp_vex_local[3]);
					sub_cell[1].push_back(temp_vex_local[23]); sub_cell[1].push_back(temp_vex_local[22]);
					sub_cell[1].push_back(temp_vex_local[15]); sub_cell[1].push_back(temp_vex_local[13]);
					// 角落的六面体
					sub_cell[2].push_back(temp_vex_local[23]); sub_cell[2].push_back(temp_vex_local[22]);
					sub_cell[2].push_back(temp_vex_local[15]); sub_cell[2].push_back(temp_vex_local[13]);
					sub_cell[2].push_back(temp_vex_local[19]); sub_cell[2].push_back(temp_vex_local[17]);
					sub_cell[2].push_back(temp_vex_local[6]); sub_cell[2].push_back(temp_vex_local[7]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (bdyEdge[0] == 7) {// 62
					// 上面 & 后面
					// 面点 4 & 5
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 16 17 14 12 边点
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[16]); sub_cell[0].push_back(temp_vex_local[25]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[17]); sub_cell[0].push_back(temp_vex_local[24]);
					// 后面的体
					sub_cell[1].push_back(temp_vex_local[0]); sub_cell[1].push_back(temp_vex_local[25]);
					sub_cell[1].push_back(temp_vex_local[12]); sub_cell[1].push_back(temp_vex_local[3]);
					sub_cell[1].push_back(temp_vex_local[4]); sub_cell[1].push_back(temp_vex_local[24]);
					sub_cell[1].push_back(temp_vex_local[14]); sub_cell[1].push_back(temp_vex_local[7]);
					// 角落的体
					sub_cell[2].push_back(temp_vex_local[25]); sub_cell[2].push_back(temp_vex_local[16]);
					sub_cell[2].push_back(temp_vex_local[2]); sub_cell[2].push_back(temp_vex_local[12]);
					sub_cell[2].push_back(temp_vex_local[24]); sub_cell[2].push_back(temp_vex_local[17]);
					sub_cell[2].push_back(temp_vex_local[6]); sub_cell[2].push_back(temp_vex_local[14]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (bdyEdge[0] == 8) {
					// 后面 & 右边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 后面的六面体
					int pointIndex1[8] = { 0, 8, 12, 3, 4, 20, 21, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 20, 9, 15, 21, 4, 5, 6, 7 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 8, 1, 2, 12, 20, 9, 15, 21 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (bdyEdge[0] == 9) { // 65
					// 后面 & 左边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 后面的六面体
					int pointIndex1[8] = { 0, 20, 21, 3, 4, 10, 14, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 20, 9, 15, 21 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 20, 9, 15, 21, 10, 5, 6, 14 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (bdyEdge[0] == 10) {// 30
					// 前面 & 右边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 前面的六面体
					int pointIndex1[8] = { 8, 1, 2, 12, 20, 5, 6, 21 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 11, 20, 21, 13, 4, 5, 6, 7 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 0, 8, 12, 3, 11, 20, 21, 13 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (bdyEdge[0] == 11) { // 47
					// 前面 & 左边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 前面的六面体
					int pointIndex1[8] = { 20, 1, 2, 21, 10, 5, 6, 14 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 11, 20, 21, 13 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 11, 20, 21, 13, 4, 10, 14, 7 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
			}
			else if (mesh->neighbor_c_c(CellHandle(i)).size() == 5 && bdyEdgeNums == 4) {
				int cf_size = mesh->cells_[CellHandle(i)].faces_.size();
				int il = 0;
				for (il = 0; il < cf_size; il++)
				{
					std::set<CellHandle, compare_OVM> cfc = mesh->neighbor_f[mesh->cells_[CellHandle(i)].faces_[il]];
					if (cfc.size() != 2) {
						break;
					}
				}


				// 假设 il 是边界面
				if (il == 0 || il == 1) {
					std::vector<std::vector<VertexHandle>> sub_cell(2);
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_9); // 16
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_10); // 17
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_11); // 18
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_12); // 19
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[8]); sub_cell[0].push_back(temp_vex_local[10]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[9]); sub_cell[0].push_back(temp_vex_local[11]);
					sub_cell[1].push_back(temp_vex_local[10]); sub_cell[1].push_back(temp_vex_local[8]);
					sub_cell[1].push_back(temp_vex_local[2]); sub_cell[1].push_back(temp_vex_local[3]);
					sub_cell[1].push_back(temp_vex_local[11]); sub_cell[1].push_back(temp_vex_local[9]);
					sub_cell[1].push_back(temp_vex_local[6]); sub_cell[1].push_back(temp_vex_local[7]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]);
				}
				else if (il == 2 || il == 3) {
					std::vector<std::vector<VertexHandle>> sub_cell(2);
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1); // 8
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_3); // 10
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_5); // 12
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_7); // 14
					sub_cell[0].push_back(temp_vex_local[8]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[2]); sub_cell[0].push_back(temp_vex_local[10]);
					sub_cell[0].push_back(temp_vex_local[9]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[6]); sub_cell[0].push_back(temp_vex_local[11]);
					sub_cell[1].push_back(temp_vex_local[0]); sub_cell[1].push_back(temp_vex_local[8]);
					sub_cell[1].push_back(temp_vex_local[10]); sub_cell[1].push_back(temp_vex_local[3]);
					sub_cell[1].push_back(temp_vex_local[4]); sub_cell[1].push_back(temp_vex_local[9]);
					sub_cell[1].push_back(temp_vex_local[11]); sub_cell[1].push_back(temp_vex_local[7]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]);
				}
				else {
					std::vector<std::vector<VertexHandle>> sub_cell(2);
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[2]); sub_cell[0].push_back(temp_vex_local[3]);
					sub_cell[0].push_back(temp_vex_local[11]); sub_cell[0].push_back(temp_vex_local[9]);
					sub_cell[0].push_back(temp_vex_local[15]); sub_cell[0].push_back(temp_vex_local[13]);
					sub_cell[1].push_back(temp_vex_local[11]); sub_cell[1].push_back(temp_vex_local[9]);
					sub_cell[1].push_back(temp_vex_local[15]); sub_cell[1].push_back(temp_vex_local[13]);
					sub_cell[1].push_back(temp_vex_local[4]); sub_cell[1].push_back(temp_vex_local[5]);
					sub_cell[1].push_back(temp_vex_local[6]); sub_cell[1].push_back(temp_vex_local[7]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]);
				}
			}
			else if (mesh->neighbor_c_c(CellHandle(i)).size() == 5 && bdyEdgeNums == 5) {

			}
			else if (mesh->neighbor_c_c(CellHandle(i)).size() == 4) {
				int cf_size = mesh->cells_[CellHandle(i)].faces_.size();
				int il = 0;
				std::vector<int> rlt;
				for (il = 0; il < cf_size; il++) {
					std::set<CellHandle, compare_OVM> cfc = mesh->neighbor_f[mesh->cells_[CellHandle(i)].faces_[il]];
					if (cfc.size() != 2) {
						rlt.push_back(il);
					}
				}
				std::map<int, int> oppoFaces;
				oppoFaces[0] = 1; oppoFaces[1] = 0;
				oppoFaces[2] = 3; oppoFaces[3] = 2;
				oppoFaces[4] = 5; oppoFaces[5] = 4;
				// 使用笨方法排列组合出12中情况
				if (rlt[0] == 0 && rlt[1] == 2) {
					// 上面和前面
					// 面点 4 右边 5 左边
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 14 19 12 18 边点
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[25]); sub_cell[0].push_back(temp_vex_local[18]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[24]); sub_cell[0].push_back(temp_vex_local[19]);
					// 前面的体
					sub_cell[1].push_back(temp_vex_local[25]); sub_cell[1].push_back(temp_vex_local[1]);
					sub_cell[1].push_back(temp_vex_local[2]); sub_cell[1].push_back(temp_vex_local[12]);
					sub_cell[1].push_back(temp_vex_local[24]); sub_cell[1].push_back(temp_vex_local[5]);
					sub_cell[1].push_back(temp_vex_local[6]); sub_cell[1].push_back(temp_vex_local[14]);
					// 角落的体
					sub_cell[2].push_back(temp_vex_local[18]); sub_cell[2].push_back(temp_vex_local[25]);
					sub_cell[2].push_back(temp_vex_local[12]); sub_cell[2].push_back(temp_vex_local[3]);
					sub_cell[2].push_back(temp_vex_local[19]); sub_cell[2].push_back(temp_vex_local[24]);
					sub_cell[2].push_back(temp_vex_local[14]); sub_cell[2].push_back(temp_vex_local[7]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 0 && rlt[1] == 3) {
					// 上面 & 后面
					// 面点 4 & 5
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 16 17 14 12 边点
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[16]); sub_cell[0].push_back(temp_vex_local[25]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[17]); sub_cell[0].push_back(temp_vex_local[24]);
					// 后面的体
					sub_cell[1].push_back(temp_vex_local[0]); sub_cell[1].push_back(temp_vex_local[25]);
					sub_cell[1].push_back(temp_vex_local[12]); sub_cell[1].push_back(temp_vex_local[3]);
					sub_cell[1].push_back(temp_vex_local[4]); sub_cell[1].push_back(temp_vex_local[24]);
					sub_cell[1].push_back(temp_vex_local[14]); sub_cell[1].push_back(temp_vex_local[7]);
					// 角落的体
					sub_cell[2].push_back(temp_vex_local[25]); sub_cell[2].push_back(temp_vex_local[16]);
					sub_cell[2].push_back(temp_vex_local[2]); sub_cell[2].push_back(temp_vex_local[12]);
					sub_cell[2].push_back(temp_vex_local[24]); sub_cell[2].push_back(temp_vex_local[17]);
					sub_cell[2].push_back(temp_vex_local[6]); sub_cell[2].push_back(temp_vex_local[14]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 0 && rlt[1] == 4) {
					// 上面 & 右边
					// 面点 2 & 3  前面 和 后面
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					// 16 15 13 18 边点
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的六面体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[16]); sub_cell[0].push_back(temp_vex_local[18]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[22]); sub_cell[0].push_back(temp_vex_local[23]);
					// 右侧的六面体
					sub_cell[1].push_back(temp_vex_local[4]); sub_cell[1].push_back(temp_vex_local[5]);
					sub_cell[1].push_back(temp_vex_local[22]); sub_cell[1].push_back(temp_vex_local[23]);
					sub_cell[1].push_back(temp_vex_local[7]); sub_cell[1].push_back(temp_vex_local[6]);
					sub_cell[1].push_back(temp_vex_local[15]); sub_cell[1].push_back(temp_vex_local[13]);
					// 角落的六面体
					sub_cell[2].push_back(temp_vex_local[18]); sub_cell[2].push_back(temp_vex_local[16]);
					sub_cell[2].push_back(temp_vex_local[2]); sub_cell[2].push_back(temp_vex_local[3]);
					sub_cell[2].push_back(temp_vex_local[23]); sub_cell[2].push_back(temp_vex_local[22]);
					sub_cell[2].push_back(temp_vex_local[15]); sub_cell[2].push_back(temp_vex_local[13]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 0 && rlt[1] == 5) {
					// 上面 & 左边
					// 面点 2 & 3  前面 和 后面
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					// 16 15 13 18 边点
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 上面的六面体
					sub_cell[0].push_back(temp_vex_local[0]); sub_cell[0].push_back(temp_vex_local[1]);
					sub_cell[0].push_back(temp_vex_local[22]); sub_cell[0].push_back(temp_vex_local[23]);
					sub_cell[0].push_back(temp_vex_local[4]); sub_cell[0].push_back(temp_vex_local[5]);
					sub_cell[0].push_back(temp_vex_local[17]); sub_cell[0].push_back(temp_vex_local[19]);
					// 左侧的六面体
					sub_cell[1].push_back(temp_vex_local[0]); sub_cell[1].push_back(temp_vex_local[1]);
					sub_cell[1].push_back(temp_vex_local[2]); sub_cell[1].push_back(temp_vex_local[3]);
					sub_cell[1].push_back(temp_vex_local[23]); sub_cell[1].push_back(temp_vex_local[22]);
					sub_cell[1].push_back(temp_vex_local[15]); sub_cell[1].push_back(temp_vex_local[13]);
					// 角落的六面体
					sub_cell[2].push_back(temp_vex_local[23]); sub_cell[2].push_back(temp_vex_local[22]);
					sub_cell[2].push_back(temp_vex_local[15]); sub_cell[2].push_back(temp_vex_local[13]);
					sub_cell[2].push_back(temp_vex_local[19]); sub_cell[2].push_back(temp_vex_local[17]);
					sub_cell[2].push_back(temp_vex_local[6]); sub_cell[2].push_back(temp_vex_local[7]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 1 && rlt[1] == 2) {
					// 下面 & 前边
					// 面点 4 & 5  左边 和 右边
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 下面的六面体
					sub_cell[0].push_back(temp_vex_local[18]); sub_cell[0].push_back(temp_vex_local[25]);
					sub_cell[0].push_back(temp_vex_local[2]); sub_cell[0].push_back(temp_vex_local[3]);
					sub_cell[0].push_back(temp_vex_local[19]); sub_cell[0].push_back(temp_vex_local[24]);
					sub_cell[0].push_back(temp_vex_local[6]); sub_cell[0].push_back(temp_vex_local[7]);
					// 前的六面体
					sub_cell[1].push_back(temp_vex_local[8]); sub_cell[1].push_back(temp_vex_local[1]);
					sub_cell[1].push_back(temp_vex_local[2]); sub_cell[1].push_back(temp_vex_local[25]);
					sub_cell[1].push_back(temp_vex_local[10]); sub_cell[1].push_back(temp_vex_local[5]);
					sub_cell[1].push_back(temp_vex_local[6]); sub_cell[1].push_back(temp_vex_local[24]);
					// 角落的六面体
					sub_cell[2].push_back(temp_vex_local[0]); sub_cell[2].push_back(temp_vex_local[8]);
					sub_cell[2].push_back(temp_vex_local[25]); sub_cell[2].push_back(temp_vex_local[18]);
					sub_cell[2].push_back(temp_vex_local[4]); sub_cell[2].push_back(temp_vex_local[10]);
					sub_cell[2].push_back(temp_vex_local[24]); sub_cell[2].push_back(temp_vex_local[19]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 1 && rlt[1] == 3) {
					// 下面 & 后边
					// 面点 4 & 5  左边 和 右边
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[4]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[5]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 下面的六面体
					sub_cell[0].push_back(temp_vex_local[25]); sub_cell[0].push_back(temp_vex_local[16]);
					sub_cell[0].push_back(temp_vex_local[2]); sub_cell[0].push_back(temp_vex_local[3]);
					sub_cell[0].push_back(temp_vex_local[24]); sub_cell[0].push_back(temp_vex_local[17]);
					sub_cell[0].push_back(temp_vex_local[6]); sub_cell[0].push_back(temp_vex_local[7]);
					// 后面的六面体
					sub_cell[1].push_back(temp_vex_local[0]); sub_cell[1].push_back(temp_vex_local[8]);
					sub_cell[1].push_back(temp_vex_local[25]); sub_cell[1].push_back(temp_vex_local[3]);
					sub_cell[1].push_back(temp_vex_local[4]); sub_cell[1].push_back(temp_vex_local[10]);
					sub_cell[1].push_back(temp_vex_local[24]); sub_cell[1].push_back(temp_vex_local[7]);
					// 角落的六面体
					sub_cell[2].push_back(temp_vex_local[8]); sub_cell[2].push_back(temp_vex_local[1]);
					sub_cell[2].push_back(temp_vex_local[16]); sub_cell[2].push_back(temp_vex_local[25]);
					sub_cell[2].push_back(temp_vex_local[10]); sub_cell[2].push_back(temp_vex_local[5]);
					sub_cell[2].push_back(temp_vex_local[17]); sub_cell[2].push_back(temp_vex_local[24]);
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 1 && rlt[1] == 4) {
					// 下面 & 右边
					// 面点 2 & 3  前后
					int chooseFaceId1 = 2;
					int chooseFaceId2 = 3;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 下面的六面体
					int pointIndex1[8] = { 18, 16, 2, 3, 23, 22, 6, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 11, 9, 22, 23, 4, 5, 6, 7 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 0, 1, 16, 18, 11, 9, 22, 23 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 1 && rlt[1] == 5) {
					// 下面 & 左边边
					// 面点 2 & 3  前后
					int chooseFaceId1 = 2;
					int chooseFaceId2 = 3;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 下面的六面体
					int pointIndex1[8] = { 23, 22, 2, 3, 19, 17, 6, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 11, 9, 22, 23 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 11, 9, 22, 23, 4, 5, 17, 19 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 2 && rlt[1] == 4) {
					// 前面 & 右边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 前面的六面体
					int pointIndex1[8] = { 8, 1, 2, 12, 20, 5, 6, 21 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 11, 20, 21, 13, 4, 5, 6, 7 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 0, 8, 12, 3, 11, 20, 21, 13 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 2 && rlt[1] == 5) {
					// 前面 & 左边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 前面的六面体
					int pointIndex1[8] = { 20, 1, 2, 21, 10, 5, 6, 14 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 11, 20, 21, 13 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 11, 20, 21, 13, 4, 10, 14, 7 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 3 && rlt[1] == 4) {
					// 后面 & 右边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 后面的六面体
					int pointIndex1[8] = { 0, 8, 12, 3, 4, 20, 21, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 20, 9, 15, 21, 4, 5, 6, 7 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 8, 1, 2, 12, 20, 9, 15, 21 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}
				else if (rlt[0] == 3 && rlt[1] == 5) {
					// 后面 & 左边边
					// 面点 0 & 1  上下
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 1;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					std::vector<std::vector<VertexHandle>> sub_cell(3);
					// 后面的六面体
					int pointIndex1[8] = { 0, 20, 21, 3, 4, 10, 14, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 20, 9, 15, 21 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex3[8] = { 20, 9, 15, 21, 10, 5, 6, 14 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]);
				}





				// 因为rlt的两个面必定是连续的两个面所以可以通过寻找相对的面来构建
				//const uint16_t FACE_LINK_V_INDEX[6 * 4] = {
				//	0,4,5,1, // up
				//	2,6,7,3, // down
				//	1,5,6,2, // front
				//	3,7,4,0, // back
				//	4,7,6,5, // right
				//	1,0,3,2  // left
				//};
			}
			else if (mesh->neighbor_c_c(CellHandle(i)).size() == 3) {
				int cf_size = mesh->cells_[CellHandle(i)].faces_.size();
				int il = 0;
				std::vector<int> rlt;
				for (il = 0; il < cf_size; il++) {
					std::set<CellHandle, compare_OVM> cfc = mesh->neighbor_f[mesh->cells_[CellHandle(i)].faces_[il]];
					if (cfc.size() != 2) {
						rlt.push_back(il);
					}
				}
				// 8种情况分类讨论

				if (rlt[0] == 0 && rlt[1] == 3 && rlt[2] == 5) {
					// 围绕着点0 上后左 
					// 面点 1 & 2 & 4 下 前 右
					int chooseFaceId1 = 1;
					int chooseFaceId2 = 2;
					int chooseFaceId3 = 4;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 上面的六面体
					int pointIndex1[8] = { 0, 1, 22, 26, 4, 5, 17, 24 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 26, 22, 15, 21 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 后边的六面体
					int pointIndex3[8] = { 0, 26, 21, 3, 4, 24, 14, 7 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					// 角落的六面体
					int pointIndex4[8] = { 26, 22, 15, 21, 24, 17, 6, 14 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[3]);
					newmesh->add_cell(sub_cell[0]);
					newmesh->add_cell(sub_cell[1]);
					newmesh->add_cell(sub_cell[2]);
				}

				else if (rlt[0] == 0 && rlt[1] == 2 && rlt[2] == 5) {
					// 围绕着点1 上前左 
					// 面点 1 & 3 & 4 下 后 右
					int chooseFaceId1 = 1;
					int chooseFaceId2 = 3;
					int chooseFaceId3 = 4;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[14] = newmesh->add_vertex(edge_mid_7);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 角落的六面体
					int pointIndex4[8] = { 23, 26, 21, 13, 19, 24, 14, 7 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					// 上面的六面体
					int pointIndex1[8] = { 0, 1, 26, 23, 4, 5, 24, 19 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 23, 26, 21, 13 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 前边的六面体
					int pointIndex3[8] = { 26, 1, 2, 21, 24, 5, 6, 14 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]); newmesh->add_cell(sub_cell[3]);
				}
				else if (rlt[0] == 1 && rlt[1] == 2 && rlt[2] == 5) {
					// 围绕着点2 下前左 
					// 面点 0 & 3 & 4 下 后 右
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 3;
					int chooseFaceId3 = 4;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[19] = newmesh->add_vertex(edge_mid_12);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 角落的六面体
					int pointIndex4[8] = { 11, 20, 26, 23, 4, 10, 24, 19 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					// 下面的六面体
					int pointIndex1[8] = { 23, 26, 2, 3, 19, 24, 6, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 11, 20, 26, 23 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 前边的六面体
					int pointIndex3[8] = { 20, 1, 2, 26, 10, 5, 6, 24 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]); newmesh->add_cell(sub_cell[3]);
				}
				else if (rlt[0] == 1 && rlt[1] == 3 && rlt[2] == 5) {
					// 围绕着点3 下后左 
					// 面点 0 & 3 & 4 下 后 右
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 2;
					int chooseFaceId3 = 4;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[24] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[10] = newmesh->add_vertex(edge_mid_3);
					temp_vex_local[17] = newmesh->add_vertex(edge_mid_10);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 角落的六面体
					int pointIndex4[8] = { 20, 9, 22, 26, 10, 5, 17, 24 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					// 下面的六面体
					int pointIndex1[8] = { 26, 22, 2, 3, 24, 17, 6, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 左边的六面体
					int pointIndex2[8] = { 0, 1, 2, 3, 20, 9, 22, 26 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 后边的六面体
					int pointIndex3[8] = { 0, 20, 26, 3, 4, 10, 24, 7 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]); newmesh->add_cell(sub_cell[3]);
				}
				else if (rlt[0] == 0 && rlt[1] == 3 && rlt[2] == 4) {
					// 围绕着点4 上右后 
					// 面点 1 & 2 & 5 下前左
					int chooseFaceId1 = 1;
					int chooseFaceId2 = 2;
					int chooseFaceId3 = 5;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[15] = newmesh->add_vertex(edge_mid_8);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 角落的六面体
					int pointIndex4[8] = { 25, 16, 2, 12, 26, 22, 15, 21 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					// 上面的六面体
					int pointIndex1[8] = { 0, 1, 16, 25, 4, 5, 22, 26 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 4, 5, 22, 26, 7, 6, 15, 21 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 后边的六面体
					int pointIndex3[8] = { 0, 25, 12, 3, 4, 26, 21, 7 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]); newmesh->add_cell(sub_cell[3]);
				}
				else if (rlt[0] == 0 && rlt[1] == 2 && rlt[2] == 4) {
					// 围绕着点5 上前右 
					// 面点 1 & 3 & 5 下后左
					int chooseFaceId1 = 1;
					int chooseFaceId2 = 3;
					int chooseFaceId3 = 5;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[21] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[12] = newmesh->add_vertex(edge_mid_5);
					temp_vex_local[13] = newmesh->add_vertex(edge_mid_6);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 角落的六面体
					int pointIndex4[8] = { 18, 25, 12, 3, 23, 26, 21, 13 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					// 上面的六面体
					int pointIndex1[8] = { 0, 1, 25, 18, 4, 5, 26, 23 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 4, 5, 26, 23, 7, 6, 21, 13 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 前边的六面体
					int pointIndex3[8] = { 25, 1, 2, 12, 26, 5, 6, 21 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]); newmesh->add_cell(sub_cell[3]);
				}
				else if (rlt[0] == 1 && rlt[1] == 2 && rlt[2] == 4) {
					// 围绕着点6 前下右 
					// 面点 0 & 3 & 5 上后左
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 3;
					int chooseFaceId3 = 5;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[23] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[11] = newmesh->add_vertex(edge_mid_4);
					temp_vex_local[18] = newmesh->add_vertex(edge_mid_11);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 角落的六面体
					int pointIndex4[8] = { 0, 8, 25, 18, 11, 20, 26, 23 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					// 下面的六面体
					int pointIndex1[8] = { 18, 25, 2, 3, 23, 26, 6, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 11, 20, 26, 23, 4, 5, 6, 7 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 前边的六面体
					int pointIndex3[8] = { 8, 1, 2, 25, 20, 5, 6, 26 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]); newmesh->add_cell(sub_cell[3]);
				}
				else if (rlt[0] == 1 && rlt[1] == 3 && rlt[2] == 4) {
					// 围绕着点7 下后右边 
					// 面点 0 & 2 & 5 上后左
					int chooseFaceId1 = 0;
					int chooseFaceId2 = 2;
					int chooseFaceId3 = 5;
					V3f face_mid;
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId1]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[20] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId2]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[22] = newmesh->add_vertex(face_mid);
					face_mid = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[0]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[1]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[2]]
						+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[chooseFaceId3]].vs_f[3]]) * 1.0 / 4 * 1.0;
					temp_vex_local[25] = newmesh->add_vertex(face_mid);
					// 8 10 19 18 边点
					temp_vex_local[8] = newmesh->add_vertex(edge_mid_1);
					temp_vex_local[9] = newmesh->add_vertex(edge_mid_2);
					temp_vex_local[16] = newmesh->add_vertex(edge_mid_9);
					//插入单元的内心
					V3f cell_mid;
					for (int j = 0; j < 8; j++)
					{
						cell_mid += mesh->vertices_[mesh->cells_[i].vertex_[j]];
					}
					cell_mid = cell_mid * 1.0 / 8 * 1.0;
					temp_vex_local[26] = newmesh->add_vertex(cell_mid);

					std::vector<std::vector<VertexHandle>> sub_cell(4);
					// 角落的六面体
					int pointIndex4[8] = { 8, 1, 16, 25, 20, 9, 22, 26 };
					for (auto itx : pointIndex4) {
						sub_cell[3].push_back(temp_vex_local[itx]);
					}
					// 下面的六面体
					int pointIndex1[8] = { 25, 16, 2, 3, 26, 22, 6, 7 };
					for (auto itx : pointIndex1) {
						sub_cell[0].push_back(temp_vex_local[itx]);
					}
					// 右边的六面体
					int pointIndex2[8] = { 4, 5, 9, 20, 7, 6, 22, 26 };
					for (auto itx : pointIndex2) {
						sub_cell[1].push_back(temp_vex_local[itx]);
					}
					// 后面边的六面体
					int pointIndex3[8] = { 0, 8, 25, 3, 4, 20, 26, 7 };
					for (auto itx : pointIndex3) {
						sub_cell[2].push_back(temp_vex_local[itx]);
					}
					newmesh->add_cell(sub_cell[0]); newmesh->add_cell(sub_cell[1]); newmesh->add_cell(sub_cell[2]); newmesh->add_cell(sub_cell[3]);
				}

			}
		}
	}
	return *newmesh;
}
*/

HexV3fMesh& CCSubdivision::CCSubdivising_new()
{
	uint64_t cell_num = mesh->cells_.size();
	std::vector<bool> check_vertex(mesh->vertices_.size(), false);
	std::vector<bool> check_edge(mesh->edges_.size(), false);
	std::vector<bool> check_face(mesh->faces_.size(), false);
	std::map<V3f, int> quick_v3f;
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_new_vertex(i, mesh_vex_num);
			if (check_vertex[mesh->cells_[i].vertex_[mesh_vex_num]] == false)
			{
				temp_vex[mesh_vex_num] = newmesh->add_vertex_new(new_v);
				check_vertex[mesh->cells_[i].vertex_[mesh_vex_num]] = true;
				quick_v3f[new_v] = temp_vex[mesh_vex_num];
			}
			else
				temp_vex[mesh_vex_num] = VertexHandle(quick_v3f[new_v]);
		}
		//-------插入边的中点-------//
		for (int mesh_edge_num = 0; mesh_edge_num < 12; mesh_edge_num++)
		{
			EdgeHandle edge = mesh->cells_[i].edges_[mesh_edge_num];
			V3f edge_mid = cal_edge_mid(edge);
			if (check_edge[edge] == false)
			{
				temp_vex[8 + mesh_edge_num] = newmesh->add_vertex_new(edge_mid);
				check_edge[edge] = true;
				quick_v3f[edge_mid] = temp_vex[8 + mesh_edge_num];
			}
			else
				temp_vex[8 + mesh_edge_num] = VertexHandle(quick_v3f[edge_mid]);
		}
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			if (check_face[mesh->cells_[i].faces_[mesh_face_num]] == false)
			{
				temp_vex[20 + mesh_face_num] = newmesh->add_vertex_new(face_mid);
				check_face[mesh->cells_[i].faces_[mesh_face_num]] = true;
				quick_v3f[face_mid] = temp_vex[20 + mesh_face_num];
			}
			else
				temp_vex[20 + mesh_face_num] = VertexHandle(quick_v3f[face_mid]);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		temp_vex[26] = newmesh->add_vertex_new(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

V3f CCSubdivision::cal_cell_mid(const int cell_num)
{
	V3f cell_mid;
	std::vector<VertexHandle> c_v = mesh->cells_[cell_num].vertex_;
	for (std::vector<VertexHandle>::iterator c_v_it = c_v.begin(); c_v_it != c_v.end(); c_v_it++)
	{
		cell_mid = cell_mid + mesh->vertices_[*c_v_it];
	}
	cell_mid = cell_mid / 8;
	return cell_mid;
}

V3f CCSubdivision::cal_cell_midv(CellHandle cell_id)
{
	V3f cell_mid;
	std::vector<VertexHandle> c_v = mesh->cells_[cell_id].vertex_;
	for (std::vector<VertexHandle>::iterator c_v_it = c_v.begin(); c_v_it != c_v.end(); c_v_it++)
	{
		cell_mid = cell_mid + mesh->vertices_[*c_v_it];
	}
	cell_mid = cell_mid / 8;
	return cell_mid;
}

V3f CCSubdivision::cal_face_midv(FaceHandle face_id)
{
	V3f face_mid;
	std::vector<VertexHandle> f_v = mesh->faces_[face_id].vs_f;
	for (std::vector<VertexHandle>::iterator f_v_it = f_v.begin(); f_v_it != f_v.end(); f_v_it++)
	{
		face_mid = face_mid + mesh->vertices_[*f_v_it];
	}
	face_mid = face_mid / 4;
	return face_mid;
}

V3f CCSubdivision::cal_face_mid(const int cell_num, const int face_num)
{
	V3f face_mid;
	V3f face_sum;
	FaceHandle face = mesh->cells_[cell_num].faces_[face_num];//确定该面
	if (mesh->is_bdy(face))
	{
		face_mid = cal_face_midv(face);
	}
	else
	{
		V3f face_ver = cal_face_midv(face);//面点
		face_sum = face_sum + face_ver * 2;
		//计算邻接块点
		std::set<CellHandle, compare_OVM> f_c = mesh->neighbor_f[face];
		for (std::set<CellHandle, compare_OVM>::iterator f_c_it = f_c.begin(); f_c_it != f_c.end(); f_c_it++)
		{
			face_sum = face_sum + cal_cell_midv(*f_c_it);
		}
		face_mid = face_sum / 4;
	}
	return face_mid;
}

V3f CCSubdivision::cal_face_mid(FaceHandle face_id)
{
	V3f face_mid;
	V3f face_sum;
	if (mesh->is_bdy(face_id))
	{
		face_mid = cal_face_midv(face_id);
	}
	else
	{
		V3f face_ver = cal_face_midv(face_id);//面点
		face_sum = face_sum + face_ver * 2;
		//计算邻接块点
		std::set<CellHandle, compare_OVM> f_c = mesh->neighbor_f[face_id];
		for (std::set<CellHandle, compare_OVM>::iterator f_c_it = f_c.begin(); f_c_it != f_c.end(); f_c_it++)
		{
			face_sum = face_sum + cal_cell_midv(*f_c_it);
		}
		face_mid = face_sum / 4;
	}
	return face_mid;
}

V3f CCSubdivision::cal_edge_midv(EdgeHandle edge_id)
{
	V3f edge_mid;
	edge_mid = edge_mid + mesh->vertices_[mesh->edges_[edge_id].from_h];
	edge_mid = edge_mid + mesh->vertices_[mesh->edges_[edge_id].to_h];
	edge_mid = edge_mid / 2;
	return edge_mid;
}

V3f CCSubdivision::cal_edge_mid(const int cell_num, const int ver1, const int ver2)
{
	V3f edge_mid;
	V3f edge_sum;
	//首先确定该边
	EdgeHandle edge;
	bool is_find = false;
	std::vector<FaceHandle> em_c_f = mesh->cells_[cell_num].faces_;
	for (std::vector<FaceHandle>::iterator em_c_f_it = em_c_f.begin(); em_c_f_it != em_c_f.end(); em_c_f_it++)
	{
		if (is_find == true)
			break;
		std::vector<EdgeHandle> em_c_f_e = mesh->faces_[*em_c_f_it].edges_;
		for (std::vector<EdgeHandle>::iterator em_c_f_e_it = em_c_f_e.begin(); em_c_f_e_it != em_c_f_e.end(); em_c_f_e_it++)
		{
			if (((mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver2]))
				|| ((mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver2])))
			{
				edge = *em_c_f_e_it;
				is_find = true;
				break;
			}
		}
	}
	if (mesh->is_bdy(edge))
	{
		edge_sum = edge_sum + cal_edge_midv(edge) * 2;
		std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge];
		int e_f_n = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++) {
			if (mesh->is_bdy(*e_f_it))
			{
				e_f_n++;
				edge_sum = edge_sum + cal_face_midv(*e_f_it);
			}
		}
		edge_mid = edge_sum / (2 + e_f_n);
	}
	else
	{
		std::set<CellHandle, compare_OVM> e_c;//存边的邻接块
		std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge];
		const int n = e_f.size();
		edge_sum = edge_sum + cal_edge_midv(edge)*(n - 3);
		//edge_sum = edge_sum + cal_edge_midv(edge);
		V3f e_f_mid;//计算邻接平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
		{
			e_f_mid = e_f_mid + cal_face_midv(*e_f_it);
			std::set<CellHandle, compare_OVM> e_f_c = mesh->neighbor_f[*e_f_it];//根据边的邻接面的邻接块加入边的邻接块
			for (std::set<CellHandle, compare_OVM>::iterator e_f_c_it = e_f_c.begin(); e_f_c_it != e_f_c.end(); e_f_c_it++)
			{
				e_c.insert(*e_f_c_it);
			}
		}
		e_f_mid = e_f_mid / n;
		V3f e_c_mid;//计算边的邻接平均块点
		const int e_c_n = e_c.size();
		for (std::set<CellHandle, compare_OVM>::iterator e_c_it = e_c.begin(); e_c_it != e_c.end(); e_c_it++)
		{
			e_c_mid = e_c_mid + cal_cell_midv(*e_c_it);
		}
		e_c_mid = e_c_mid / e_c_n;
		edge_sum = edge_sum + e_f_mid * 2 + e_c_mid;
		edge_mid = edge_sum / n;
		//edge_mid = edge_sum / 4;
	}
	return edge_mid;
}

V3f CCSubdivision::cal_edge_mid_1(const int cell_num, const int ver1, const int ver2)
{
	V3f edge_mid;
	V3f edge_sum;
	//首先确定该边
	EdgeHandle edge;
	bool is_find = false;
	std::vector<FaceHandle> em_c_f = mesh->cells_[cell_num].faces_;
	for (std::vector<FaceHandle>::iterator em_c_f_it = em_c_f.begin(); em_c_f_it != em_c_f.end(); em_c_f_it++)
	{
		if (is_find == true)
			break;
		std::vector<EdgeHandle> em_c_f_e = mesh->faces_[*em_c_f_it].edges_;
		for (std::vector<EdgeHandle>::iterator em_c_f_e_it = em_c_f_e.begin(); em_c_f_e_it != em_c_f_e.end(); em_c_f_e_it++)
		{
			if (((mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver2]))
				|| ((mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver2])))
			{
				edge = *em_c_f_e_it;
				is_find = true;
				break;
			}
		}
	}
	if (mesh->is_bdy(edge))
	{
		std::set<FaceHandle, compare_OVM> e_f_temp = mesh->neighbor_e[edge];
		int ef_temp = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f_temp.begin(); e_f_it != e_f_temp.end(); e_f_it++)
		{
			if (mesh->is_bdy(*e_f_it))
				ef_temp++;
		}
		if (ef_temp == 4 /*|| ef_temp > 1*/)
		{
			edge_sum = edge_sum + cal_edge_midv(edge) * 2;
			std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge];
			int e_f_n = 0;
			for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
			{
				if (mesh->is_bdy(*e_f_it) && mesh->is_f_in_c(*e_f_it, CellHandle(cell_num)))
				{
					e_f_n++;
					edge_sum = edge_sum + cal_face_midv(*e_f_it);
				}
			}
			edge_mid = edge_sum / (2 + e_f_n);
		}
		else
		{
			edge_sum = edge_sum + cal_edge_midv(edge) * 2;
			std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge];
			int e_f_n = 0;
			for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
			{
				if (mesh->is_bdy(*e_f_it))
				{
					e_f_n++;
					edge_sum = edge_sum + cal_face_midv(*e_f_it);
				}
			}
			edge_mid = edge_sum / (2 + e_f_n);
		}

	}
	else
	{
		std::set<CellHandle, compare_OVM> e_c;//存边的邻接块
		std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge];
		const int n = e_f.size();
		edge_sum = edge_sum + cal_edge_midv(edge)*(n - 3);
		//edge_sum = edge_sum + cal_edge_midv(edge);
		V3f e_f_mid;//计算邻接平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
		{
			e_f_mid = e_f_mid + cal_face_midv(*e_f_it);
			std::set<CellHandle, compare_OVM> e_f_c = mesh->neighbor_f[*e_f_it];//根据边的邻接面的邻接块加入边的邻接块
			for (std::set<CellHandle, compare_OVM>::iterator e_f_c_it = e_f_c.begin(); e_f_c_it != e_f_c.end(); e_f_c_it++)
			{
				e_c.insert(*e_f_c_it);
			}
		}
		e_f_mid = e_f_mid / n;
		V3f e_c_mid;//计算边的邻接平均块点
		const int e_c_n = e_c.size();
		for (std::set<CellHandle, compare_OVM>::iterator e_c_it = e_c.begin(); e_c_it != e_c.end(); e_c_it++)
		{
			e_c_mid = e_c_mid + cal_cell_midv(*e_c_it);
		}
		e_c_mid = e_c_mid / e_c_n;
		edge_sum = edge_sum + e_f_mid * 2 + e_c_mid;
		edge_mid = edge_sum / n;
		//edge_mid = edge_sum / 4;
	}
	return edge_mid;
}

V3f CCSubdivision::cal_edge_mid(EdgeHandle edge_id)
{
	V3f edge_mid;
	V3f edge_sum;
	if (mesh->is_bdy(edge_id))
	{
		edge_sum = edge_sum + cal_edge_midv(edge_id) * 2;
		std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge_id];
		int e_f_n = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
		{
			if (mesh->is_bdy(*e_f_it))
			{
				e_f_n++;
				edge_sum = edge_sum + cal_face_midv(*e_f_it);
			}
		}
		edge_mid = edge_sum / (2 + e_f_n);
	}
	else
	{
		std::set<CellHandle, compare_OVM> e_c;//存边的邻接块
		std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge_id];
		const int n = e_f.size();
		edge_sum = edge_sum + cal_edge_midv(edge_id)*(n - 3);
		//edge_sum = edge_sum + cal_edge_midv(edge_id);
		V3f e_f_mid;//计算邻接平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
		{
			e_f_mid = e_f_mid + cal_face_midv(*e_f_it);
			std::set<CellHandle, compare_OVM> e_f_c = mesh->neighbor_f[*e_f_it];//根据边的邻接面的邻接块加入边的邻接块
			for (std::set<CellHandle, compare_OVM>::iterator e_f_c_it = e_f_c.begin(); e_f_c_it != e_f_c.end(); e_f_c_it++)
			{
				e_c.insert(*e_f_c_it);
			}
		}
		e_f_mid = e_f_mid / n;
		V3f e_c_mid;//计算边的邻接平均块点
		const int e_c_n = e_c.size();
		for (std::set<CellHandle, compare_OVM>::iterator e_c_it = e_c.begin(); e_c_it != e_c.end(); e_c_it++)
		{
			e_c_mid = e_c_mid + cal_cell_midv(*e_c_it);
		}
		e_c_mid = e_c_mid / e_c_n;
		edge_sum = edge_sum + e_f_mid * 2 + e_c_mid;
		edge_mid = edge_sum / n;
		//edge_mid = edge_sum / 4;
	}
	return edge_mid;
}

V3f CCSubdivision::cal_new_vertex(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(vertex))
	{
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int v_e_n = 0;
		V3f v_e_mid;//计算点邻接边的平均边点
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
			{
				v_e_n++;
				v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
			}
		}
		v_e_mid = v_e_mid / v_e_n;
		vertex_sum = vertex_sum + v_e_mid * 2;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		int v_f_n = 0;
		V3f v_f_mid;//计算点邻接面的平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			if (mesh->is_bdy(*v_f_it))
			{
				v_f_n++;
				v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
			}
		}
		v_f_mid = v_f_mid / v_f_n;
		vertex_sum = vertex_sum + v_f_mid;
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * (v_e_n - 3);
		new_vertex = vertex_sum / v_e_n;
	}
	else
	{
		std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[vertex];
		const int v_c_n = v_c.size();
		V3f v_c_mid;//计算点邻接块的平均块点
		for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
		{
			v_c_mid = v_c_mid + cal_cell_midv(*v_c_it);
		}
		v_c_mid = v_c_mid / v_c_n;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		const int v_f_n = v_f.size();
		V3f v_f_mid;//计算点邻接面的平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
		}
		v_f_mid = v_f_mid / v_f_n;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		const int v_e_n = v_e.size();
		V3f v_e_mid;//计算点邻接面的平均面点
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
		}
		v_e_mid = v_e_mid / v_e_n;
		vertex_sum = v_c_mid + v_f_mid * 3 + v_e_mid * 3 + mesh->vertices_[vertex];
		new_vertex = vertex_sum / 8;
	}
	return new_vertex;
}

V3f CCSubdivision::cal_new_vertex_1(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(vertex))
	{
		std::set<EdgeHandle, compare_OVM> v_e_temp = mesh->neighbor_v[vertex];
		int bdy_edge = 0;
		int not_bdy_edge = 0;
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e_temp.begin(); v_e_it != v_e_temp.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
				bdy_edge++;
			else
				not_bdy_edge++;
		}
		if (bdy_edge > 5 || not_bdy_edge < 1)
		{
			std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
			int v_e_n = 0;
			V3f v_e_mid;//计算点邻接边的平均边点
			std::set<CellHandle> c_c_temp = mesh->neighbor_c_c(CellHandle(cell_num));
			c_c_temp.insert(CellHandle(cell_num));
			for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
			{
				for (std::set<CellHandle>::iterator c_c_temp_it = c_c_temp.begin(); c_c_temp_it != c_c_temp.end(); c_c_temp_it++)
				{
					if (mesh->is_bdy(*v_e_it) && mesh->is_e_in_c(*v_e_it, *c_c_temp_it))
					{
						v_e_n++;
						v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
						break;
					}
				}

			}
			v_e_mid = v_e_mid / v_e_n;
			vertex_sum = vertex_sum + v_e_mid * 2;
			std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
			int v_f_n = 0;
			V3f v_f_mid;//计算点邻接面的平均面点
			for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
			{
				for (std::set<CellHandle>::iterator c_c_temp_it = c_c_temp.begin(); c_c_temp_it != c_c_temp.end(); c_c_temp_it++)
				{
					if (mesh->is_bdy(*v_f_it) && mesh->is_f_in_c(*v_f_it, *c_c_temp_it))
					{
						v_f_n++;
						v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
						break;
					}
				}

			}
			v_f_mid = v_f_mid / v_f_n;
			vertex_sum = vertex_sum + v_f_mid;
			vertex_sum = vertex_sum + mesh->vertices_[vertex] * (v_e_n - 3);
			new_vertex = vertex_sum / v_e_n;
		}
		else
		{
			std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
			int v_e_n = 0;
			V3f v_e_mid;//计算点邻接边的平均边点
			for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
			{
				if (mesh->is_bdy(*v_e_it))
				{
					v_e_n++;
					v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
				}
			}
			v_e_mid = v_e_mid / v_e_n;
			vertex_sum = vertex_sum + v_e_mid * 2;
			std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
			int v_f_n = 0;
			V3f v_f_mid;//计算点邻接面的平均面点
			for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
			{
				if (mesh->is_bdy(*v_f_it))
				{
					v_f_n++;
					v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
				}
			}
			v_f_mid = v_f_mid / v_f_n;
			vertex_sum = vertex_sum + v_f_mid;
			vertex_sum = vertex_sum + mesh->vertices_[vertex] * (v_e_n - 3);
			new_vertex = vertex_sum / v_e_n;
		}
	}
	else
	{
		//if (mesh->is_singular(vertex))
		//{
		//	int n = mesh->neighbor_v[vertex].size();//度数
		//	std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[vertex];
		//	const int v_c_n = v_c.size();
		//	V3f v_c_mid;//计算点邻接块的平均块点
		//	for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin();v_c_it != v_c.end();v_c_it++)
		//	{
		//		v_c_mid = v_c_mid + cal_cell_midv(*v_c_it);
		//	}
		//	v_c_mid = v_c_mid / ((n - 2) * 16);
		//	std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		//	V3f v_f_mid;//计算点邻接面的平均面点
		//	for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin();v_f_it != v_f.end();v_f_it++)
		//	{
		//		v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
		//	}
		//	v_f_mid = v_f_mid / ((n - 2) * 8);
		//	std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		//	V3f v_e_mid;//计算点邻接面的平均面点
		//	for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin();v_e_it != v_e.end();v_e_it++)
		//	{
		//		v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
		//	}
		//	v_e_mid = v_e_mid / ((n - 2) * 4);
		//	new_vertex = new_vertex + v_c_mid + v_f_mid + v_e_mid + mesh->vertices_[vertex] / 8;
		//}
		//else
		//{
		std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[vertex];
		const int v_c_n = v_c.size();
		V3f v_c_mid;//计算点邻接块的平均块点
		for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
		{
			v_c_mid = v_c_mid + cal_cell_midv(*v_c_it);
		}
		v_c_mid = v_c_mid / v_c_n;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		const int v_f_n = v_f.size();
		V3f v_f_mid;//计算点邻接面的平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
		}
		v_f_mid = v_f_mid / v_f_n;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		const int v_e_n = v_e.size();
		V3f v_e_mid;//计算点邻接面的平均面点
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
		}
		v_e_mid = v_e_mid / v_e_n;
		vertex_sum = v_c_mid + v_f_mid * 3 + v_e_mid * 3 + mesh->vertices_[vertex];
		new_vertex = vertex_sum / 8;
		//}
	}
	return new_vertex;
}

V3f CCSubdivision::cal_new_vertex(VertexHandle ver_id)
{
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(ver_id))
	{
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[ver_id];
		int v_e_n = 0;
		V3f v_e_mid;//计算点邻接边的平均边点
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
			{
				v_e_n++;
				v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
			}
		}
		v_e_mid = v_e_mid / v_e_n;
		vertex_sum = vertex_sum + v_e_mid * 2;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[ver_id];
		int v_f_n = 0;
		V3f v_f_mid;//计算点邻接面的平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			if (mesh->is_bdy(*v_f_it))
			{
				v_f_n++;
				v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
			}
		}
		v_f_mid = v_f_mid / v_f_n;
		vertex_sum = vertex_sum + v_f_mid;
		vertex_sum = vertex_sum + mesh->vertices_[ver_id] * (v_e_n - 3);
		new_vertex = vertex_sum / v_e_n;
	}
	else
	{
		//if (mesh->is_singular(ver_id))
		//{
		//	int n = mesh->neighbor_v[ver_id].size();//度数
		//	std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[ver_id];
		//	const int v_c_n = v_c.size();
		//	V3f v_c_mid;//计算点邻接块的平均块点
		//	for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin();v_c_it != v_c.end();v_c_it++)
		//	{
		//		v_c_mid = v_c_mid + cal_cell_midv(*v_c_it);
		//	}
		//	v_c_mid = v_c_mid / ((n - 2) * 16);
		//	std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[ver_id];
		//	V3f v_f_mid;//计算点邻接面的平均面点
		//	for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin();v_f_it != v_f.end();v_f_it++)
		//	{
		//		v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
		//	}
		//	v_f_mid = v_f_mid / ((n - 2) * 8);
		//	std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[ver_id];
		//	V3f v_e_mid;//计算点邻接面的平均面点
		//	for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin();v_e_it != v_e.end();v_e_it++)
		//	{
		//		v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
		//	}
		//	v_e_mid = v_e_mid / ((n - 2) * 4);
		//	new_vertex = new_vertex + v_c_mid + v_f_mid + v_e_mid + mesh->vertices_[ver_id] / 8;
		//}
		//else
		//{
		std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[ver_id];
		const int v_c_n = v_c.size();
		V3f v_c_mid;//计算点邻接块的平均块点
		for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
		{
			v_c_mid = v_c_mid + cal_cell_midv(*v_c_it);
		}
		v_c_mid = v_c_mid / v_c_n;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[ver_id];
		const int v_f_n = v_f.size();
		V3f v_f_mid;//计算点邻接面的平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
		}
		v_f_mid = v_f_mid / v_f_n;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[ver_id];
		const int v_e_n = v_e.size();
		V3f v_e_mid;//计算点邻接面的平均面点
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
		}
		v_e_mid = v_e_mid / v_e_n;
		vertex_sum = v_c_mid + v_f_mid * 3 + v_e_mid * 3 + mesh->vertices_[ver_id];
		new_vertex = vertex_sum / 8;
		//}
	}
	return new_vertex;
}

HexV3fMesh& CCSubdivision::CCSubWithCrease()
{
	/*int ecsn = 0;
	for (int i = 0;i < e_cs.size();i++)
	{
		if (e_cs[i] = true)
			ecsn++;
	}*/
	uint64_t cell_num = mesh->cells_.size();
	std::vector<bool> new_v_cs;//折痕点
	new_v_cs.resize(18 * cell_num + 9, false);
	int csv_num = 0;//用于调试
	int tmp_num = 0;
	int tme_num = 0;
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_new_csv(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
			if (v_cs[mesh->cells_[i].vertex_[mesh_vex_num]])
			{
				csv_num++;
				tmp_num++;
				new_v_cs[temp_vex[mesh_vex_num]] = true;
			}
			//new_v_cs[temp_vex[mesh_vex_num]] = true;
		}

		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_es_mid(i, 0, 1);
		EdgeHandle edm1 = check_edge(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);
		if (is_cs(edm1))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[8]] = true;
		}
		//new_v_cs[temp_vex[8]] = true;

		//第二条边的中点
		V3f edge_mid_2 = cal_es_mid(i, 1, 5);
		EdgeHandle edm2 = check_edge(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);
		if (is_cs(edm2))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[9]] = true;
		}
		//new_v_cs[temp_vex[9]] = true;

		//第三条边的中点
		V3f edge_mid_3 = cal_es_mid(i, 5, 4);
		EdgeHandle edm3 = check_edge(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);
		if (is_cs(edm3))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[10]] = true;
		}
		//new_v_cs[temp_vex[10]] = true;

		//第四条边的中点
		V3f edge_mid_4 = cal_es_mid(i, 4, 0);
		EdgeHandle edm4 = check_edge(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);
		if (is_cs(edm4))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[11]] = true;
		}
		//new_v_cs[temp_vex[11]] = true;

		//第五条边的中点
		V3f edge_mid_5 = cal_es_mid(i, 3, 2);
		EdgeHandle edm5 = check_edge(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);
		if (is_cs(edm5))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[12]] = true;
		}
		//new_v_cs[temp_vex[12]] = true;

		//第六条边的中点
		V3f edge_mid_6 = cal_es_mid(i, 3, 7);
		EdgeHandle edm6 = check_edge(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);
		if (is_cs(edm6))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[13]] = true;
		}
		//new_v_cs[temp_vex[13]] = true;

		//第七条边的中点
		V3f edge_mid_7 = cal_es_mid(i, 6, 7);
		EdgeHandle edm7 = check_edge(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);
		if (is_cs(edm7))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[14]] = true;
		}
		//new_v_cs[temp_vex[14]] = true;

		//第八条边的中点
		V3f edge_mid_8 = cal_es_mid(i, 2, 6);
		EdgeHandle edm8 = check_edge(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);
		if (is_cs(edm8))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[15]] = true;
		}
		//new_v_cs[temp_vex[15]] = true;

		//第九条边的中点
		V3f edge_mid_9 = cal_es_mid(i, 1, 2);
		EdgeHandle edm9 = check_edge(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);
		if (is_cs(edm9))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[16]] = true;
		}
		//new_v_cs[temp_vex[16]] = true;

		//第十条边的中点
		V3f edge_mid_10 = cal_es_mid(i, 5, 6);
		EdgeHandle edm10 = check_edge(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);
		if (is_cs(edm10))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[17]] = true;
		}
		//new_v_cs[temp_vex[17]] = true;

		//第十一条边的中点
		V3f edge_mid_11 = cal_es_mid(i, 0, 3);
		EdgeHandle edm11 = check_edge(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);
		if (is_cs(edm11))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[18]] = true;
		}
		//new_v_cs[temp_vex[18]] = true;

		//第十二条边的中点
		V3f edge_mid_12 = cal_es_mid(i, 4, 7);
		EdgeHandle edm12 = check_edge(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		if (is_cs(edm12))
		{
			csv_num++;
			tme_num++;
			new_v_cs[temp_vex[19]] = true;
		}
		//new_v_cs[temp_vex[19]] = true;
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	v_cs = new_v_cs;
	return *newmesh;
}

HexV3fMesh& CCSubdivision::CCSubWithCrease_sharpedge()
{
	uint64_t cell_num = mesh->cells_.size();
	std::vector<bool> new_v_cs;//折痕点
	new_v_cs.resize(18 * cell_num + 9, false);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_new_csv_se(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
			if (is_cs_se(mesh->cells_[i].vertex_[mesh_vex_num]))
			{
				new_v_cs[temp_vex[mesh_vex_num]] = true;
			}
		}

		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_es_mid_se(i, 0, 1);
		EdgeHandle edm1 = check_edge(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);
		if (is_cs_se(edm1))
		{
			new_v_cs[temp_vex[8]] = true;
		}

		//第二条边的中点
		V3f edge_mid_2 = cal_es_mid_se(i, 1, 5);
		EdgeHandle edm2 = check_edge(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);
		if (is_cs_se(edm2))
		{
			new_v_cs[temp_vex[9]] = true;
		}

		//第三条边的中点
		V3f edge_mid_3 = cal_es_mid_se(i, 5, 4);
		EdgeHandle edm3 = check_edge(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);
		if (is_cs_se(edm3))
		{
			new_v_cs[temp_vex[10]] = true;
		}

		//第四条边的中点
		V3f edge_mid_4 = cal_es_mid_se(i, 4, 0);
		EdgeHandle edm4 = check_edge(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);
		if (is_cs_se(edm4))
		{
			new_v_cs[temp_vex[11]] = true;
		}

		//第五条边的中点
		V3f edge_mid_5 = cal_es_mid_se(i, 3, 2);
		EdgeHandle edm5 = check_edge(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);
		if (is_cs_se(edm5))
		{
			new_v_cs[temp_vex[12]] = true;
		}

		//第六条边的中点
		V3f edge_mid_6 = cal_es_mid_se(i, 3, 7);
		EdgeHandle edm6 = check_edge(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);
		if (is_cs_se(edm6))
		{
			new_v_cs[temp_vex[13]] = true;
		}

		//第七条边的中点
		V3f edge_mid_7 = cal_es_mid_se(i, 6, 7);
		EdgeHandle edm7 = check_edge(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);
		if (is_cs_se(edm7))
		{
			new_v_cs[temp_vex[14]] = true;
		}

		//第八条边的中点
		V3f edge_mid_8 = cal_es_mid_se(i, 2, 6);
		EdgeHandle edm8 = check_edge(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);
		if (is_cs_se(edm8))
		{
			new_v_cs[temp_vex[15]] = true;
		}

		//第九条边的中点
		V3f edge_mid_9 = cal_es_mid_se(i, 1, 2);
		EdgeHandle edm9 = check_edge(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);
		if (is_cs_se(edm9))
		{
			new_v_cs[temp_vex[16]] = true;
		}

		//第十条边的中点
		V3f edge_mid_10 = cal_es_mid_se(i, 5, 6);
		EdgeHandle edm10 = check_edge(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);
		if (is_cs_se(edm10))
		{
			new_v_cs[temp_vex[17]] = true;
		}

		//第十一条边的中点
		V3f edge_mid_11 = cal_es_mid_se(i, 0, 3);
		EdgeHandle edm11 = check_edge(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);
		if (is_cs_se(edm11))
		{
			new_v_cs[temp_vex[18]] = true;
		}

		//第十二条边的中点
		V3f edge_mid_12 = cal_es_mid_se(i, 4, 7);
		EdgeHandle edm12 = check_edge(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		if (is_cs_se(edm12))
		{
			new_v_cs[temp_vex[19]] = true;
		}
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	/*for (auto it : sharpedge) {
		std::cout << it.idx() << std::endl;
	}*/
	sharpedge.clear();
	for (int i = 0; i < newmesh->edges_.size(); ++i)
	{
		if (new_v_cs[newmesh->edges_[i].from_h] && new_v_cs[newmesh->edges_[i].to_h])
			sharpedge.push_back(EdgeHandle(i));
	}
	return *newmesh;
}

V3f CCSubdivision::cal_new_csv(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	V3f new_vertex;
	if (is_cs(vertex))
	{
		new_vertex = new_vertex + mesh->vertices_[vertex] * (3.0 / 4);
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (is_cs(*v_e_it))
			{
				if (mesh->vertices_[mesh->edges_[*v_e_it].from_h] == mesh->vertices_[vertex])
				{
					//if (is_cs(mesh->edges_[*v_e_it].to_h))
					//{
					new_vertex = new_vertex + mesh->vertices_[mesh->edges_[*v_e_it].to_h] * (1.0 / 8);
					//}
				}
				else
				{
					//if (is_cs(mesh->edges_[*v_e_it].from_h))
					//{
					new_vertex = new_vertex + mesh->vertices_[mesh->edges_[*v_e_it].from_h] * (1.0 / 8);
					//}
				}
			}
		}
	}
	else
	{
		new_vertex = cal_new_vertex(cell_num, ver_num);
	}
	return new_vertex;
}

V3f CCSubdivision::cal_new_csv_se(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	V3f new_vertex;
	if (is_cs_se(vertex))
	{
		std::cout << "[ori new_vertex] " << new_vertex.x << " " << new_vertex.y << " " << new_vertex.z << std::endl;
		new_vertex = new_vertex + mesh->vertices_[vertex] * (3.0 / 4);
		std::cout << "[ori new_vertex] " << new_vertex.x << " " << new_vertex.y << " " << new_vertex.z << " " << mesh->vertices_[vertex].x  << 
			" " <<mesh->vertices_[vertex].y << " " << mesh->vertices_[vertex].z << std::endl;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (is_cs_se(*v_e_it))
			{
				if (mesh->vertices_[mesh->edges_[*v_e_it].from_h] == mesh->vertices_[vertex])
				{
					new_vertex = new_vertex + mesh->vertices_[mesh->edges_[*v_e_it].to_h] * (1.0 / 8);
					std::cout << "[ver_num new_vertex] " << ver_num << new_vertex.x << " " << new_vertex.y << " " << new_vertex.z 
						<< " " << mesh->vertices_[mesh->edges_[*v_e_it].to_h].x << " " << mesh->vertices_[mesh->edges_[*v_e_it].to_h].y
						<< " " << mesh->vertices_[mesh->edges_[*v_e_it].to_h].z << std::endl;
				}
				else
				{
					new_vertex = new_vertex + mesh->vertices_[mesh->edges_[*v_e_it].from_h] * (1.0 / 8);
					std::cout << "[ver_num new_vertex] " << ver_num << new_vertex.x << " " << new_vertex.y << " " << new_vertex.z
						<< " " << mesh->vertices_[mesh->edges_[*v_e_it].from_h].x << " " << mesh->vertices_[mesh->edges_[*v_e_it].from_h].y
						<< " " << mesh->vertices_[mesh->edges_[*v_e_it].from_h].z << std::endl;
				}
			}
		}
	}
	else
	{
		new_vertex = cal_new_vertex(cell_num, ver_num);
	}
	return new_vertex;
}

V3f CCSubdivision::cal_es_mid(const int cell_num, const int ver1, const int ver2)
{
	V3f edge_mid;
	EdgeHandle edge;
	bool is_find = false;
	std::vector<FaceHandle> em_c_f = mesh->cells_[cell_num].faces_;
	for (std::vector<FaceHandle>::iterator em_c_f_it = em_c_f.begin(); em_c_f_it != em_c_f.end(); em_c_f_it++)
	{
		if (is_find == true)
			break;
		std::vector<EdgeHandle> em_c_f_e = mesh->faces_[*em_c_f_it].edges_;
		for (std::vector<EdgeHandle>::iterator em_c_f_e_it = em_c_f_e.begin(); em_c_f_e_it != em_c_f_e.end(); em_c_f_e_it++)
		{
			if (((mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver2]))
				|| ((mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver2])))
			{
				edge = *em_c_f_e_it;
				is_find = true;
				break;
			}
		}
	}
	if (is_cs(edge))
		edge_mid = cal_edge_midv(edge);
	else
		edge_mid = cal_edge_mid(cell_num, ver1, ver2);
	return edge_mid;
}

V3f CCSubdivision::cal_es_mid_se(const int cell_num, const int ver1, const int ver2)
{
	V3f edge_mid;
	EdgeHandle edge;
	bool is_find = false;
	std::vector<FaceHandle> em_c_f = mesh->cells_[cell_num].faces_;
	for (std::vector<FaceHandle>::iterator em_c_f_it = em_c_f.begin(); em_c_f_it != em_c_f.end(); em_c_f_it++)
	{
		if (is_find == true)
			break;
		std::vector<EdgeHandle> em_c_f_e = mesh->faces_[*em_c_f_it].edges_;
		for (std::vector<EdgeHandle>::iterator em_c_f_e_it = em_c_f_e.begin(); em_c_f_e_it != em_c_f_e.end(); em_c_f_e_it++)
		{
			if (((mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver2]))
				|| ((mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver2])))
			{
				edge = *em_c_f_e_it;
				is_find = true;
				break;
			}
		}
	}
	if (is_cs_se(edge))
		edge_mid = cal_edge_midv(edge);
	else
		edge_mid = cal_edge_mid(cell_num, ver1, ver2);
	return edge_mid;
}

EdgeHandle CCSubdivision::check_edge(const int cell_num, const int ver1, const int ver2)
{
	EdgeHandle edge;
	bool is_find = false;
	std::vector<FaceHandle> em_c_f = mesh->cells_[cell_num].faces_;
	for (std::vector<FaceHandle>::iterator em_c_f_it = em_c_f.begin(); em_c_f_it != em_c_f.end(); em_c_f_it++)
	{
		if (is_find == true)
			break;
		std::vector<EdgeHandle> em_c_f_e = mesh->faces_[*em_c_f_it].edges_;
		for (std::vector<EdgeHandle>::iterator em_c_f_e_it = em_c_f_e.begin(); em_c_f_e_it != em_c_f_e.end(); em_c_f_e_it++)
		{
			if (((mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver2]))
				|| ((mesh->edges_[*em_c_f_e_it].to_h == mesh->cells_[cell_num].vertex_[ver1]) && (mesh->edges_[*em_c_f_e_it].from_h == mesh->cells_[cell_num].vertex_[ver2])))
			{
				edge = *em_c_f_e_it;
				is_find = true;
				break;
			}
		}
	}
	return edge;
}

HexV3fMesh& CCSubdivision::FirstCCSubdivising()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_new_vertex(i, mesh_vex_num);
			if (mesh_vex_num == 5)
				new_v.bezierid = 123;
			else if (mesh_vex_num == 1)
				new_v.bezierid = 23;
			else if (mesh_vex_num == 6)
				new_v.bezierid = 13;
			else if (mesh_vex_num == 4)
				new_v.bezierid = 12;
			else
				new_v.bezierid = -1;
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_edge_mid(i, 0, 1);
		edge_mid_1.bezierid = 2;
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_edge_mid(i, 1, 5);
		edge_mid_2.bezierid = 23;
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_edge_mid(i, 5, 4);
		edge_mid_3.bezierid = 12;
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_edge_mid(i, 4, 0);
		edge_mid_4.bezierid = 2;
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_edge_mid(i, 3, 2);
		edge_mid_5.bezierid = -1;
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_edge_mid(i, 3, 7);
		edge_mid_6.bezierid = -1;
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_edge_mid(i, 6, 7);
		edge_mid_7.bezierid = 1;
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_edge_mid(i, 2, 6);
		edge_mid_8.bezierid = 3;
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_edge_mid(i, 1, 2);
		edge_mid_9.bezierid = 3;
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_edge_mid(i, 5, 6);
		edge_mid_10.bezierid = 13;
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_edge_mid(i, 0, 3);
		edge_mid_11.bezierid = -1;
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_edge_mid(i, 4, 7);
		edge_mid_12.bezierid = 1;
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			if (mesh_face_num == 0)
				face_mid.bezierid = 2;
			else if (mesh_face_num == 2)
				face_mid.bezierid = 3;
			else if (mesh_face_num == 4)
				face_mid.bezierid = 1;
			else
				face_mid.bezierid = -1;
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

HexV3fMesh& CCSubdivision::SecondCCSubdivising()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_new_vertex(i, mesh_vex_num);
			new_v.bezierid = mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]].bezierid;
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_edge_mid(i, 0, 1);
		int id1 = mesh->vertices_[mesh->edges_[mesh->cells_[i].edges_[1]].from_h].bezierid;
		int id2 = mesh->vertices_[mesh->edges_[mesh->cells_[i].edges_[1]].to_h].bezierid;
		if (id1 == -1 || id2 == -1)
			edge_mid_1.bezierid = -1;
		//else if()
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_edge_mid(i, 1, 5);
		edge_mid_2.bezierid = 23;
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_edge_mid(i, 5, 4);
		edge_mid_3.bezierid = 12;
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_edge_mid(i, 4, 0);
		edge_mid_4.bezierid = 2;
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_edge_mid(i, 3, 2);
		edge_mid_5.bezierid = -1;
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_edge_mid(i, 3, 7);
		edge_mid_6.bezierid = -1;
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_edge_mid(i, 6, 7);
		edge_mid_7.bezierid = 1;
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_edge_mid(i, 2, 6);
		edge_mid_8.bezierid = 3;
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_edge_mid(i, 1, 2);
		edge_mid_9.bezierid = 3;
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_edge_mid(i, 5, 6);
		edge_mid_10.bezierid = 13;
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_edge_mid(i, 0, 3);
		edge_mid_11.bezierid = -1;
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_edge_mid(i, 4, 7);
		edge_mid_12.bezierid = 1;
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_mid(i, mesh_face_num);
			if (mesh_face_num == 0)
				face_mid.bezierid = 2;
			else if (mesh_face_num == 2)
				face_mid.bezierid = 3;
			else if (mesh_face_num == 4)
				face_mid.bezierid = 1;
			else
				face_mid.bezierid = -1;
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

//-----InterpolateCC-----//
/*-----------Push-Back法-----------*/
HexV3fMesh& InterpolateCC::CCInterpolatingPB()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_CIPB_nv(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
			//temp_vex[mesh_vex_num] = newmesh->add_vertex(firstver[mesh->cells_[i].vertex_[mesh_vex_num]]);
			//temp_vex[mesh_vex_num] = newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_CIPB_ev(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_CIPB_ev(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_CIPB_ev(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_CIPB_ev(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_CIPB_ev(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_CIPB_ev(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_CIPB_ev(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_CIPB_ev(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_CIPB_ev(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_CIPB_ev(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_CIPB_ev(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_CIPB_ev(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_CIPB_fv(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_CIPB_cv(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

HexV3fMesh& InterpolateCC::CCInterpolatingPB1()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			VertexHandle ver = mesh->cells_[i].vertex_[mesh_vex_num];
			V3f new_v = cal_CIPB_fnv(ver);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		EdgeHandle e1 = check_edge(i, 0, 1);
		V3f edge_mid_1 = ev[e1];
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		EdgeHandle e2 = check_edge(i, 1, 5);
		V3f edge_mid_2 = ev[e2];
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		EdgeHandle e3 = check_edge(i, 5, 4);
		V3f edge_mid_3 = ev[e3];
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		EdgeHandle e4 = check_edge(i, 4, 0);
		V3f edge_mid_4 = ev[e4];
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		EdgeHandle e5 = check_edge(i, 3, 2);
		V3f edge_mid_5 = ev[e5];
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		EdgeHandle e6 = check_edge(i, 3, 7);
		V3f edge_mid_6 = ev[e6];
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		EdgeHandle e7 = check_edge(i, 6, 7);
		V3f edge_mid_7 = ev[e7];
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		EdgeHandle e8 = check_edge(i, 2, 6);
		V3f edge_mid_8 = ev[e8];
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		EdgeHandle e9 = check_edge(i, 1, 2);
		V3f edge_mid_9 = ev[e9];
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		EdgeHandle e10 = check_edge(i, 5, 6);
		V3f edge_mid_10 = ev[e10];
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		EdgeHandle e11 = check_edge(i, 0, 3);
		V3f edge_mid_11 = ev[e11];
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		EdgeHandle e12 = check_edge(i, 4, 7);
		V3f edge_mid_12 = ev[e12];
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			FaceHandle new_fv = mesh->cells_[i].faces_[mesh_face_num];
			V3f face_mid = fv[new_fv];
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cv[i];
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

void InterpolateCC::PBfirstep()
{
	firstver.swap(std::vector<V3f>());
	detver.swap(std::vector<V3f>());
	/*firstedge.swap(std::vector<V3f>());
	detedge.swap(std::vector<V3f>());
	firstface.swap(std::vector<V3f>());
	detface.swap(std::vector<V3f>());*/
	//oriv.swap(std::vector<V3f>());
	int ver_size = mesh->vertices_.size();
	for (int i = 0; i < ver_size; i++)
	{
		V3f new_vertex = cal_new_vertex(VertexHandle(i));
		V3f det = mesh->vertices_[i] - new_vertex;
		firstver.push_back(new_vertex);
		detver.push_back(det);
		//oriv.push_back(mesh->vertices_[i]);
	}
	/*int edge_size = mesh->edges_.size();
	for (int i = 0;i < edge_size;i++)
	{
		V3f new_vertex = cal_edge_mid(EdgeHandle(i));
		V3f det = cal_edge_midv(EdgeHandle(i)) - new_vertex;
		firstedge.push_back(new_vertex);
		detedge.push_back(det);
	}
	int face_size = mesh->faces_.size();
	for (int i = 0;i < face_size;i++)
	{
		V3f new_vertex = cal_face_mid(FaceHandle(i));
		V3f det = cal_face_midv(FaceHandle(i)) - new_vertex;
		firstface.push_back(new_vertex);
		detface.push_back(det);
	}*/
	/*vv.resize(ver_size);
	for (int i = 0; i < ver_size; i++)
	{
		V3f temp = cal_CIPB_nv(VertexHandle(i));
		vv[i] = temp;
	}
	int edge_size = mesh->edges_.size();
	ev.resize(edge_size);
	for (int i = 0;i < edge_size;i++)
	{
		V3f temp = cal_CIPB_ev(EdgeHandle(i));
		ev[i] = temp;
	}
	int face_size = mesh->faces_.size();
	fv.resize(face_size);
	for (int i = 0;i < face_size;i++)
	{
		V3f temp = cal_CIPB_fv(FaceHandle(i));
		fv[i] = temp;
	}
	int cell_size = mesh->cells_.size();
	cv.resize(cell_size);
	for (int i = 0;i < cell_size;i++)
	{
		V3f temp = cal_CIPB_cv(CellHandle(i));
		cv[i] = temp;
	}*/
}

V3f InterpolateCC::cal_CIPB_ev(const int cell_num, const int ver1, const int ver2)
{
	EdgeHandle edge = check_edge(cell_num, ver1, ver2);
	V3f edge_mid;
	V3f edge_sum;
	if (mesh->is_bdy(edge))
	{
		edge_mid = cal_edge_midv(edge) + (detver[mesh->edges_[edge].from_h] + detver[mesh->edges_[edge].to_h]) * le;
	}
	else
	{
		//edge_mid = cal_edge_mid(cell_num, ver1, ver2);
		//edge_mid = cal_edge_midv(edge) + detedge[edge] * le;
		edge_mid = cal_edge_midv(edge) + (detver[mesh->edges_[edge].from_h] + detver[mesh->edges_[edge].to_h]) * le;
	}
	return edge_mid;
}

V3f InterpolateCC::cal_CIPB_ev(EdgeHandle edge_id)
{
	V3f edge_mid;
	V3f edge_sum;
	if (mesh->is_bdy(edge_id))
	{
		edge_mid = cal_edge_midv(edge_id) + (detver[mesh->edges_[edge_id].from_h] + detver[mesh->edges_[edge_id].to_h]) * le;
	}
	else
	{
		//edge_mid = cal_edge_mid(edge_id);
		//edge_mid = cal_edge_midv(edge_id) + detedge[edge_id] * le;
		edge_mid = cal_edge_midv(edge_id) + (detver[mesh->edges_[edge_id].from_h] + detver[mesh->edges_[edge_id].to_h]) * le;
	}
	return edge_mid;
}

V3f InterpolateCC::cal_CIPB_fv(const int cell_num, const int face_num)
{
	V3f face_mid;
	V3f face_sum;
	FaceHandle face = mesh->cells_[cell_num].faces_[face_num];//确定该面
	if (mesh->is_bdy(face))
	{
		std::vector<VertexHandle> fv = mesh->faces_[face].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + mesh->vertices_[*fv_it] / 4.0;
			face_sum = face_sum + detver[*fv_it] * 2 * uf;
		}
		face_mid = face_sum;
	}
	else
	{
		//face_mid = cal_face_mid(cell_num, face_num);
		//face_mid = cal_face_midv(face) + detface[face] * 2 * uf;
		face_mid = face_mid + cal_face_midv(face);
		std::vector<VertexHandle> fv = mesh->faces_[face].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + detver[*fv_it] * 2 * uf;
		}
	}
	return face_mid;
}

V3f InterpolateCC::cal_CIPB_fv(FaceHandle face_id)
{
	V3f face_mid;
	V3f face_sum;
	if (mesh->is_bdy(face_id))
	{
		std::vector<VertexHandle> fv = mesh->faces_[face_id].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + mesh->vertices_[*fv_it] / 4.0;
			face_sum = face_sum + detver[*fv_it] * 2 * uf;
		}
		face_mid = face_sum;
	}
	else
	{
		//face_mid = cal_face_mid(face_id);
		//face_mid = cal_face_midv(face_id) + detface[face_id] * 2 * uf;
		face_mid = face_mid + cal_face_midv(face_id);
		std::vector<VertexHandle> fv = mesh->faces_[face_id].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + detver[*fv_it] * 2 * uf;
		}
	}
	return face_mid;
}

V3f InterpolateCC::cal_CIPB_cv(const int cell_num)
{
	V3f cell_mid;
	//cell_mid = cal_cell_mid(cell_num);
	std::vector<VertexHandle> c_v = mesh->cells_[cell_num].vertex_;
	for (std::vector<VertexHandle>::iterator c_v_it = c_v.begin(); c_v_it != c_v.end(); c_v_it++)
	{
		cell_mid = cell_mid + mesh->vertices_[*c_v_it] / 8.0;
		cell_mid = cell_mid + detver[*c_v_it] * 2 * dc;
	}
	//std::set<EdgeHandle> c_e;
	////将块的所含边放入容器
	//std::vector<FaceHandle> c_f = mesh->cells_[cell_num].faces_;
	//for (std::vector<FaceHandle>::iterator c_f_it = c_f.begin();c_f_it != c_f.end();c_f_it++)
	//{
	//	std::vector<EdgeHandle> c_f_e = mesh->faces_[*c_f_it].edges_;
	//	for (std::vector<EdgeHandle>::iterator c_f_e_it = c_f_e.begin();c_f_e_it != c_f_e.end();c_f_e_it++)
	//	{
	//		c_e.insert(*c_f_e_it);
	//	}
	//}
	//for (std::set<EdgeHandle>::iterator c_e_it = c_e.begin();c_e_it != c_e.end();c_e_it++)
	//{
	//	cell_mid = cell_mid + cal_edge_midv(*c_e_it) / 8.0;
	//	cell_mid = cell_mid + detedge[*c_e_it] * 2 * dc;
	//}
	return cell_mid;
}

V3f InterpolateCC::cal_CIPB_cv(CellHandle cell_id)
{
	V3f cell_mid;
	//cell_mid = cal_cell_mid(cell_id);
	std::vector<VertexHandle> c_v = mesh->cells_[cell_id].vertex_;
	for (std::vector<VertexHandle>::iterator c_v_it = c_v.begin(); c_v_it != c_v.end(); c_v_it++)
	{
		cell_mid = cell_mid + mesh->vertices_[*c_v_it] / 8.0;
		cell_mid = cell_mid + detver[*c_v_it] * 2 * dc;
	}
	//std::set<EdgeHandle> c_e;
	////将块的所含边放入容器
	//std::vector<FaceHandle> c_f = mesh->cells_[cell_id].faces_;
	//for (std::vector<FaceHandle>::iterator c_f_it = c_f.begin();c_f_it != c_f.end();c_f_it++)
	//{
	//	std::vector<EdgeHandle> c_f_e = mesh->faces_[*c_f_it].edges_;
	//	for (std::vector<EdgeHandle>::iterator c_f_e_it = c_f_e.begin();c_f_e_it != c_f_e.end();c_f_e_it++)
	//	{
	//		c_e.insert(*c_f_e_it);
	//	}
	//}
	//for (std::set<EdgeHandle>::iterator c_e_it = c_e.begin();c_e_it != c_e.end();c_e_it++)
	//{
	//	cell_mid = cell_mid + cal_edge_midv(*c_e_it) / 8.0;
	//	cell_mid = cell_mid + detedge[*c_e_it] * 2 * dc;
	//}
	return cell_mid;
}

V3f InterpolateCC::cal_CIPB_nv(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(vertex))
	{
		//const int n = mesh->neighbor_v[vertex].size();
		//vertex_sum = vertex_sum + mesh->vertices_[vertex] * n * (n + 5);
		int en = 0;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int v_e_n = v_e.size();
		V3f e_av;
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
			{
				en++;
				V3f temp_ev = cal_CIPB_ev(*v_e_it);
				e_av = e_av + temp_ev;
				//vertex_sum = vertex_sum - temp_ev * 4;
			}
		}
		e_av = e_av / en;
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * (en + 5);
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		V3f f_av;
		//int fn = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			if (mesh->is_bdy(*v_f_it))
			{
				//fn++;
				V3f temp_fv = cal_CIPB_fv(*v_f_it);
				f_av = f_av + temp_fv;
				//vertex_sum = vertex_sum - temp_fv;
			}
		}
		f_av = f_av / en;
		new_vertex = vertex_sum - e_av * 4 - f_av;
		new_vertex = new_vertex / en;
		//new_vertex = mesh->vertices_[vertex];
	}
	else
	{
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int en = v_e.size();
		int sum = 0;
		sum += 16 * (en - 2);
		//sum += 2 * (en - 2)*(en - 2);
		//vertex_sum = vertex_sum + mesh->vertices_[vertex] * (4 * 4 * (en-2)+ 38 * (en - 2));
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			V3f temp_ev = cal_CIPB_ev(*v_e_it);
			int m = mesh->neighbor_e[*v_e_it].size();
			sum += 4 * m;
			vertex_sum = vertex_sum - temp_ev * 4 * m;
			/*if (mesh->is_singular(*v_e_it))
			{
				vertex_sum = vertex_sum - temp_ev * 4 * (en - 2);
			}
			else
			{
				vertex_sum = vertex_sum - temp_ev * 4 * 4;
			}*/
		}
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			V3f temp_fv = cal_CIPB_fv(*v_f_it);
			vertex_sum = vertex_sum - temp_fv * 4;
			sum += 4;
		}
		std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[vertex];
		for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
		{
			V3f temp_cv = cal_CIPB_cv(*v_c_it);
			vertex_sum = vertex_sum - temp_cv;
			sum += 1;
		}
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * sum;
		new_vertex = vertex_sum / ((en - 2) * 4 * 4);
		//new_vertex = vertex_sum / ((en - 2) * (en - 2) * 2);
	}
	return new_vertex;
}

V3f InterpolateCC::cal_CIPB_nv(VertexHandle ver_id)
{
	VertexHandle vertex = ver_id;
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(vertex))
	{
		//const int n = mesh->neighbor_v[vertex].size();
		//vertex_sum = vertex_sum + mesh->vertices_[vertex] * n * (n + 5);
		int en = 0;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int v_e_n = v_e.size();
		V3f e_av;
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
			{
				en++;
				V3f temp_ev = cal_CIPB_ev(*v_e_it);
				e_av = e_av + temp_ev;
				//vertex_sum = vertex_sum - temp_ev * 4;
			}
		}
		e_av = e_av / en;
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * (en + 5);
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		V3f f_av;
		//int fn = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			if (mesh->is_bdy(*v_f_it))
			{
				//fn++;
				V3f temp_fv = cal_CIPB_fv(*v_f_it);
				f_av = f_av + temp_fv;
				//vertex_sum = vertex_sum - temp_fv;
			}
		}
		f_av = f_av / en;
		new_vertex = vertex_sum - e_av * 4 - f_av;
		new_vertex = new_vertex / en;
		//new_vertex = mesh->vertices_[vertex];
	}
	else
	{
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int en = v_e.size();
		int sum = 0;
		sum += 16 * (en - 2);
		//sum += 2 * (en - 2)*(en - 2);;
		//vertex_sum = vertex_sum + mesh->vertices_[vertex] * (4 * 4 * (en-2)+ 38 * (en - 2));
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			V3f temp_ev = cal_CIPB_ev(*v_e_it);
			int m = mesh->neighbor_e[*v_e_it].size();
			sum += 4 * m;
			vertex_sum = vertex_sum - temp_ev * 4 * m;
			/*if (mesh->is_singular(*v_e_it))
			{
			vertex_sum = vertex_sum - temp_ev * 4 * (en - 2);
			}
			else
			{
			vertex_sum = vertex_sum - temp_ev * 4 * 4;
			}*/
		}
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			V3f temp_fv = cal_CIPB_fv(*v_f_it);
			vertex_sum = vertex_sum - temp_fv * 4;
			sum += 4;
		}
		std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[vertex];
		for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
		{
			V3f temp_cv = cal_CIPB_cv(*v_c_it);
			vertex_sum = vertex_sum - temp_cv;
			sum += 1;
		}
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * sum;
		new_vertex = vertex_sum / ((en - 2) * 4 * 4);
		//new_vertex = vertex_sum / ((en - 2) * (en - 2) * 2);
	}
	return new_vertex;
}

V3f InterpolateCC::cal_CIPB_fnv(VertexHandle ver_id)
{
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(ver_id))
	{
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[ver_id];
		int v_e_n = 0;
		V3f v_e_mid;//计算点邻接边的平均边点
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
			{
				v_e_n++;
				v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
			}
		}
		v_e_mid = v_e_mid / v_e_n;
		vertex_sum = vertex_sum + v_e_mid * 2;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[ver_id];
		int v_f_n = 0;
		V3f v_f_mid;//计算点邻接面的平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			if (mesh->is_bdy(*v_f_it))
			{
				v_f_n++;
				v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
			}
		}
		v_f_mid = v_f_mid / v_f_n;
		vertex_sum = vertex_sum + v_f_mid;
		vertex_sum = vertex_sum + vv[ver_id] * (v_e_n - 3);
		new_vertex = vertex_sum / v_e_n;
	}
	else
	{
		std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[ver_id];
		const int v_c_n = v_c.size();
		V3f v_c_mid;//计算点邻接块的平均块点
		for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
		{
			v_c_mid = v_c_mid + cal_cell_midv(*v_c_it);
		}
		v_c_mid = v_c_mid / v_c_n;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[ver_id];
		const int v_f_n = v_f.size();
		V3f v_f_mid;//计算点邻接面的平均面点
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
		}
		v_f_mid = v_f_mid / v_f_n;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[ver_id];
		const int v_e_n = v_e.size();
		V3f v_e_mid;//计算点邻接面的平均面点
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
		}
		v_e_mid = v_e_mid / v_e_n;
		vertex_sum = v_c_mid + v_f_mid * 3 + v_e_mid * 3 + vv[ver_id];
		new_vertex = vertex_sum / 8;
		//}
	}
	return new_vertex;
}

/*-----------Normal-Based法-----------*/
HexV3fMesh& InterpolateCC::CCInterpolatingNB()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		//std::vector<V3f>::iterator v_id;

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_CINB_nv(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
			//temp_vex[mesh_vex_num] = newmesh->add_vertex(firstver[mesh->cells_[i].vertex_[mesh_vex_num]]);
			//temp_vex[mesh_vex_num] = newmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_CINB_ev(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_CINB_ev(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_CINB_ev(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_CINB_ev(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_CINB_ev(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_CINB_ev(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_CINB_ev(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_CINB_ev(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_CINB_ev(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_CINB_ev(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_CINB_ev(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_CINB_ev(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_CINB_fv(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		newmesh->add_cell(sub_cell[0]);
		newmesh->add_cell(sub_cell[1]);
		newmesh->add_cell(sub_cell[2]);
		newmesh->add_cell(sub_cell[3]);
		newmesh->add_cell(sub_cell[4]);
		newmesh->add_cell(sub_cell[5]);
		newmesh->add_cell(sub_cell[6]);
		newmesh->add_cell(sub_cell[7]);
	}
	return *newmesh;
}

void InterpolateCC::NBfirstep()
{
	normalver.swap(std::vector<V3f>());
	int ver_size = mesh->vertices_.size();
	for (int i = 0; i < ver_size; i++)
	{
		VertexHandle vertex = VertexHandle(i);
		double fi; V3f nor; V3f nx;
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			std::vector<EdgeHandle> ne;
			if (mesh->is_bdy(*v_f_it))
			{
				std::vector<EdgeHandle> v_f_e = mesh->faces_[*v_f_it].edges_;
				for (std::vector<EdgeHandle>::iterator v_f_e_it = v_f_e.begin(); v_f_e_it != v_f_e.end(); v_f_e_it++)
				{
					if (mesh->edges_[*v_f_e_it].from_h == vertex || mesh->edges_[*v_f_e_it].to_h == vertex)
						ne.push_back(*v_f_e_it);
				}
				fi = mesh->cal_angle(ne[0], ne[1], vertex);
				nor = mesh->cal_norm(*v_f_it);
				//修正向量
				std::set<CellHandle, compare_OVM> v_f_c = mesh->neighbor_f[*v_f_it];
				CellHandle vfc = *(v_f_c.begin());
				V3f vfc_mid = cal_cell_midv(vfc);
				V3f checkn = vfc_mid - mesh->vertices_[vertex];
				double temp = nor.x*checkn.x + nor.y*checkn.y + nor.z*checkn.z;
				if (temp > 0)
					nor = nor * -1;
				nx = nx + nor * fi;
			}
		}
		nx = nx.normalize();
		normalver.push_back(nx);
	}
}

V3f InterpolateCC::cal_CINB_ev(const int cell_num, const int ver1, const int ver2)
{
	EdgeHandle edge = check_edge(cell_num, ver1, ver2);
	V3f edge_mid;
	V3f edge_sum;
	if (mesh->is_bdy(edge))
	{
		V3f edmid = cal_edge_midv(edge);
		edge_sum = edge_sum + edmid;
		V3f di = (mesh->vertices_[mesh->edges_[edge].from_h] - mesh->vertices_[mesh->edges_[edge].to_h]) * 0.5;
		V3f nori = normalver[mesh->edges_[edge].from_h];
		V3f dj = (mesh->vertices_[mesh->edges_[edge].to_h] - mesh->vertices_[mesh->edges_[edge].from_h]) * 0.5;
		V3f norj = normalver[mesh->edges_[edge].to_h];
		V3f temp = nori * (di.x*nori.x + di.y*nori.y + di.z*nori.z) + norj * (dj.x*norj.x + dj.y*norj.y + dj.z*norj.z);
		edge_sum = edge_sum + temp * we * 0.5;
		edge_mid = edge_sum;
	}
	else
		edge_mid = cal_edge_mid(edge);
	return edge_mid;
}

V3f InterpolateCC::cal_CINB_ev(EdgeHandle edge_id)
{
	V3f edge_mid;
	V3f edge_sum;
	if (mesh->is_bdy(edge_id))
	{
		V3f edmid = cal_edge_midv(edge_id);
		edge_sum = edge_sum + edmid;
		V3f di = (mesh->vertices_[mesh->edges_[edge_id].from_h] - mesh->vertices_[mesh->edges_[edge_id].to_h]) * 0.5;
		V3f nori = normalver[mesh->edges_[edge_id].from_h];
		V3f dj = (mesh->vertices_[mesh->edges_[edge_id].to_h] - mesh->vertices_[mesh->edges_[edge_id].from_h]) * 0.5;
		V3f norj = normalver[mesh->edges_[edge_id].to_h];
		V3f temp = nori * (di.x*nori.x + di.y*nori.y + di.z*nori.z) + norj * (dj.x*norj.x + dj.y*norj.y + dj.z*norj.z);
		edge_sum = edge_sum + temp * we * 0.5;
		edge_mid = edge_sum;
	}
	else
		edge_mid = cal_edge_mid(edge_id);
	return edge_mid;
}

V3f InterpolateCC::cal_CINB_fv(const int cell_num, const int face_num)
{
	V3f face_mid;
	V3f face_sum;
	FaceHandle face = mesh->cells_[cell_num].faces_[face_num];//确定该面
	if (mesh->is_bdy(face))
	{
		V3f face_mid = cal_face_midv(face);
		face_sum = face_sum + face_mid;
		std::vector<VertexHandle> fv = mesh->faces_[face].vs_f;
		V3f temp;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			V3f temp1 = mesh->vertices_[*fv_it] - face_mid;
			V3f nori = normalver[*fv_it];
			temp = temp + nori * (temp1.x*nori.x + temp1.y*nori.y + temp1.z*nori.z);
		}
		face_sum = face_sum + temp * vf / 4;
		face_mid = face_sum;
	}
	else
		face_mid = cal_face_mid(face);
	return face_mid;
}

V3f InterpolateCC::cal_CINB_fv(FaceHandle face_id)
{
	V3f face_mid;
	V3f face_sum;
	if (mesh->is_bdy(face_id))
	{
		V3f face_mid = cal_face_midv(face_id);
		face_sum = face_sum + face_mid;
		std::vector<VertexHandle> fv = mesh->faces_[face_id].vs_f;
		V3f temp;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			V3f temp1 = mesh->vertices_[*fv_it] - face_mid;
			V3f nori = normalver[*fv_it];
			temp = temp + nori * (temp1.x*nori.x + temp1.y*nori.y + temp1.z*nori.z);
		}
		face_sum = face_sum + temp * vf / 4;
		face_mid = face_sum;
	}
	else
		face_mid = cal_face_mid(face_id);
	return face_mid;
}

V3f InterpolateCC::cal_CINB_nv(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(vertex))
	{
		//const int n = mesh->neighbor_v[vertex].size();
		//vertex_sum = vertex_sum + mesh->vertices_[vertex] * n * (n + 5);
		int en = 0;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int v_e_n = v_e.size();
		V3f e_av;
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
			{
				en++;
				V3f temp_ev = cal_CINB_ev(*v_e_it);
				e_av = e_av + temp_ev;
				//vertex_sum = vertex_sum - temp_ev * 4;
			}
		}
		e_av = e_av / en;
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * (en + 5);
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		V3f f_av;
		//int fn = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			if (mesh->is_bdy(*v_f_it))
			{
				//fn++;
				V3f temp_fv = cal_CINB_fv(*v_f_it);
				f_av = f_av + temp_fv;
				//vertex_sum = vertex_sum - temp_fv;
			}
		}
		f_av = f_av / en;
		new_vertex = vertex_sum - e_av * 4 - f_av;
		new_vertex = new_vertex / en;
		//new_vertex = mesh->vertices_[vertex];
	}
	else
		new_vertex = cal_new_vertex(cell_num, ver_num);
	return new_vertex;
}

/*-----------MLCA部分-----------*/
void InterpolateCC::MLCAfirstep()
{
	Spliting();
	mlcadet.swap(std::vector<V3f>());
	int ver_size = mesh->vertices_.size();
	for (int i = 0; i < ver_size; i++)
	{
		V3f new_vertex = cal_mlca_vertex(VertexHandle(i));
		V3f det = mesh->vertices_[i] - new_vertex;
		mlcadet.push_back(det);
	}
}



/**************************************************
@brief   : 猜测应该是  将一个cell划分为8个cell
@author  : lee
@input   ：none
@output  ：none
@time    : none
**************************************************/
void InterpolateCC::Spliting()
{
	uint64_t cell_num = mesh->cells_.size();
	// 将会产生 27 * cell 个数的顶点。将会产生cell * 69 条边？？ QU: 感觉不是69条边
	vid.resize(cell_num * 27, VertexHandle(-1));
	/*eid.resize(cell_num * 69);
	fid.resize(cell_num * 36);
	cid.resize(cell_num * 8);*/
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];
		std::vector<V3f>::iterator v_id;
		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			temp_vex[mesh_vex_num] = tempmesh->add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[mesh_vex_num]]);
			vid[temp_vex[mesh_vex_num]] = mesh->cells_[i].vertex_[mesh_vex_num];
		}
		//再插入每条边的中点 暴力插入 以免顺序错误
		//第一条边的中点
		V3f edge_mid_1;
		edge_mid_1.x = (mesh->vertices_[mesh->cells_[i].vertex_[0]].x + mesh->vertices_[mesh->cells_[i].vertex_[1]].x)*1.0 / 2 * 1.0;
		edge_mid_1.y = (mesh->vertices_[mesh->cells_[i].vertex_[0]].y + mesh->vertices_[mesh->cells_[i].vertex_[1]].y)*1.0 / 2 * 1.0;
		edge_mid_1.z = (mesh->vertices_[mesh->cells_[i].vertex_[0]].z + mesh->vertices_[mesh->cells_[i].vertex_[1]].z)*1.0 / 2 * 1.0;
		temp_vex[8] = tempmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2;
		edge_mid_2.x = (mesh->vertices_[mesh->cells_[i].vertex_[1]].x + mesh->vertices_[mesh->cells_[i].vertex_[5]].x)*1.0 / 2 * 1.0;
		edge_mid_2.y = (mesh->vertices_[mesh->cells_[i].vertex_[1]].y + mesh->vertices_[mesh->cells_[i].vertex_[5]].y)*1.0 / 2 * 1.0;
		edge_mid_2.z = (mesh->vertices_[mesh->cells_[i].vertex_[1]].z + mesh->vertices_[mesh->cells_[i].vertex_[5]].z)*1.0 / 2 * 1.0;
		temp_vex[9] = tempmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3;
		edge_mid_3.x = (mesh->vertices_[mesh->cells_[i].vertex_[5]].x + mesh->vertices_[mesh->cells_[i].vertex_[4]].x)*1.0 / 2 * 1.0;
		edge_mid_3.y = (mesh->vertices_[mesh->cells_[i].vertex_[5]].y + mesh->vertices_[mesh->cells_[i].vertex_[4]].y)*1.0 / 2 * 1.0;
		edge_mid_3.z = (mesh->vertices_[mesh->cells_[i].vertex_[5]].z + mesh->vertices_[mesh->cells_[i].vertex_[4]].z)*1.0 / 2 * 1.0;
		temp_vex[10] = tempmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4;
		edge_mid_4.x = (mesh->vertices_[mesh->cells_[i].vertex_[4]].x + mesh->vertices_[mesh->cells_[i].vertex_[0]].x)*1.0 / 2 * 1.0;
		edge_mid_4.y = (mesh->vertices_[mesh->cells_[i].vertex_[4]].y + mesh->vertices_[mesh->cells_[i].vertex_[0]].y)*1.0 / 2 * 1.0;
		edge_mid_4.z = (mesh->vertices_[mesh->cells_[i].vertex_[4]].z + mesh->vertices_[mesh->cells_[i].vertex_[0]].z)*1.0 / 2 * 1.0;
		temp_vex[11] = tempmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5;
		edge_mid_5.x = (mesh->vertices_[mesh->cells_[i].vertex_[3]].x + mesh->vertices_[mesh->cells_[i].vertex_[2]].x)*1.0 / 2 * 1.0;
		edge_mid_5.y = (mesh->vertices_[mesh->cells_[i].vertex_[3]].y + mesh->vertices_[mesh->cells_[i].vertex_[2]].y)*1.0 / 2 * 1.0;
		edge_mid_5.z = (mesh->vertices_[mesh->cells_[i].vertex_[3]].z + mesh->vertices_[mesh->cells_[i].vertex_[2]].z)*1.0 / 2 * 1.0;
		temp_vex[12] = tempmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6;
		edge_mid_6.x = (mesh->vertices_[mesh->cells_[i].vertex_[3]].x + mesh->vertices_[mesh->cells_[i].vertex_[7]].x)*1.0 / 2 * 1.0;
		edge_mid_6.y = (mesh->vertices_[mesh->cells_[i].vertex_[3]].y + mesh->vertices_[mesh->cells_[i].vertex_[7]].y)*1.0 / 2 * 1.0;
		edge_mid_6.z = (mesh->vertices_[mesh->cells_[i].vertex_[3]].z + mesh->vertices_[mesh->cells_[i].vertex_[7]].z)*1.0 / 2 * 1.0;
		temp_vex[13] = tempmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7;
		edge_mid_7.x = (mesh->vertices_[mesh->cells_[i].vertex_[6]].x + mesh->vertices_[mesh->cells_[i].vertex_[7]].x)*1.0 / 2 * 1.0;
		edge_mid_7.y = (mesh->vertices_[mesh->cells_[i].vertex_[6]].y + mesh->vertices_[mesh->cells_[i].vertex_[7]].y)*1.0 / 2 * 1.0;
		edge_mid_7.z = (mesh->vertices_[mesh->cells_[i].vertex_[6]].z + mesh->vertices_[mesh->cells_[i].vertex_[7]].z)*1.0 / 2 * 1.0;
		temp_vex[14] = tempmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8;
		edge_mid_8.x = (mesh->vertices_[mesh->cells_[i].vertex_[2]].x + mesh->vertices_[mesh->cells_[i].vertex_[6]].x)*1.0 / 2 * 1.0;
		edge_mid_8.y = (mesh->vertices_[mesh->cells_[i].vertex_[2]].y + mesh->vertices_[mesh->cells_[i].vertex_[6]].y)*1.0 / 2 * 1.0;
		edge_mid_8.z = (mesh->vertices_[mesh->cells_[i].vertex_[2]].z + mesh->vertices_[mesh->cells_[i].vertex_[6]].z)*1.0 / 2 * 1.0;
		temp_vex[15] = tempmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9;
		edge_mid_9.x = (mesh->vertices_[mesh->cells_[i].vertex_[1]].x + mesh->vertices_[mesh->cells_[i].vertex_[2]].x)*1.0 / 2 * 1.0;
		edge_mid_9.y = (mesh->vertices_[mesh->cells_[i].vertex_[1]].y + mesh->vertices_[mesh->cells_[i].vertex_[2]].y)*1.0 / 2 * 1.0;
		edge_mid_9.z = (mesh->vertices_[mesh->cells_[i].vertex_[1]].z + mesh->vertices_[mesh->cells_[i].vertex_[2]].z)*1.0 / 2 * 1.0;
		temp_vex[16] = tempmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10;
		edge_mid_10.x = (mesh->vertices_[mesh->cells_[i].vertex_[5]].x + mesh->vertices_[mesh->cells_[i].vertex_[6]].x)*1.0 / 2 * 1.0;
		edge_mid_10.y = (mesh->vertices_[mesh->cells_[i].vertex_[5]].y + mesh->vertices_[mesh->cells_[i].vertex_[6]].y)*1.0 / 2 * 1.0;
		edge_mid_10.z = (mesh->vertices_[mesh->cells_[i].vertex_[5]].z + mesh->vertices_[mesh->cells_[i].vertex_[6]].z)*1.0 / 2 * 1.0;
		temp_vex[17] = tempmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11;
		edge_mid_11.x = (mesh->vertices_[mesh->cells_[i].vertex_[0]].x + mesh->vertices_[mesh->cells_[i].vertex_[3]].x)*1.0 / 2 * 1.0;
		edge_mid_11.y = (mesh->vertices_[mesh->cells_[i].vertex_[0]].y + mesh->vertices_[mesh->cells_[i].vertex_[3]].y)*1.0 / 2 * 1.0;
		edge_mid_11.z = (mesh->vertices_[mesh->cells_[i].vertex_[0]].z + mesh->vertices_[mesh->cells_[i].vertex_[3]].z)*1.0 / 2 * 1.0;
		temp_vex[18] = tempmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12;
		edge_mid_12.x = (mesh->vertices_[mesh->cells_[i].vertex_[4]].x + mesh->vertices_[mesh->cells_[i].vertex_[7]].x)*1.0 / 2 * 1.0;
		edge_mid_12.y = (mesh->vertices_[mesh->cells_[i].vertex_[4]].y + mesh->vertices_[mesh->cells_[i].vertex_[7]].y)*1.0 / 2 * 1.0;
		edge_mid_12.z = (mesh->vertices_[mesh->cells_[i].vertex_[4]].z + mesh->vertices_[mesh->cells_[i].vertex_[7]].z)*1.0 / 2 * 1.0;
		temp_vex[19] = tempmesh->add_vertex(edge_mid_12);

		//插入每个面的中点
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid;
			face_mid.x = (mesh->vertices_[ mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0] ].x
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]].x
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]].x
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]].x)*1.0 / 4 * 1.0;
			face_mid.y = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]].y
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]].y
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]].y
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]].y)*1.0 / 4 * 1.0;
			face_mid.z = (mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[0]].z
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[1]].z
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[2]].z
				+ mesh->vertices_[mesh->faces_[mesh->cells_[i].faces_[mesh_face_num]].vs_f[3]].z)*1.0 / 4 * 1.0;
			temp_vex[20 + mesh_face_num] = tempmesh->add_vertex(face_mid);
		}

		//插入单元的内心
		V3f cell_mid;
		double x_sum = 0;  double y_sum = 0;  double z_sum = 0;
		for (int j = 0; j < 8; j++)
		{
			x_sum += mesh->vertices_[mesh->cells_[i].vertex_[j]].x;
			y_sum += mesh->vertices_[mesh->cells_[i].vertex_[j]].y;
			z_sum += mesh->vertices_[mesh->cells_[i].vertex_[j]].z;
		}
		cell_mid.x = x_sum * 1.0 / 8 * 1.0;
		cell_mid.y = y_sum * 1.0 / 8 * 1.0;
		cell_mid.z = z_sum * 1.0 / 8 * 1.0;
		temp_vex[26] = tempmesh->add_vertex(cell_mid);

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		for (int j = 0; j < 8; j++) {
			tempmesh->add_cell(sub_cell[j]);
		}
	}
}



/**************************************************
@brief   : mlca 有两个操作   一个是split 另一个是average
@author  : lee
@input   ：none
@output  ：none
@time    : none
**************************************************/
V3f InterpolateCC::cal_mlca_vertex(VertexHandle ver_id)
{
	V3f new_vertex;
	if (mesh->is_bdy(ver_id))
		new_vertex = cal_new_vertex(ver_id);
	else
	{
		int size = vid.size();
		for (int i = 0; i < size; i++)
		{
			if (vid[i] == VertexHandle(ver_id))
			{
				std::set<CellHandle, compare_OVM> vc = tempmesh->neighbor_v_c[i];
				int vc_num = vc.size();
				std::vector<V3f> vc_cen(vc_num);
				int u = 0;
				for (std::set<CellHandle, compare_OVM>::iterator vc_it = vc.begin(); vc_it != vc.end(); vc_it++)
				{
					vc_cen[u] = cal_cell_midv(*vc_it);
					u++;
				}
				for (int vf_cen_num = 0; vf_cen_num < vc_num; vf_cen_num++)
				{
					new_vertex = new_vertex + vc_cen[vf_cen_num];
				}
				new_vertex = new_vertex / (vc_num*1.0);
				break;
			}
		}
	}
	return new_vertex;
}

V3f InterpolateCC::cal_MLCA_ev(const int cell_num, const int ver1, const int ver2)
{
	EdgeHandle edge = check_edge(cell_num, ver1, ver2);
	V3f edge_mid;
	V3f edge_sum;
	if (mesh->is_bdy(edge))
	{
		edge_mid = cal_edge_midv(edge) + (detver[mesh->edges_[edge].from_h] + detver[mesh->edges_[edge].to_h]) * le;
	}
	else
	{
		edge_mid = cal_edge_midv(edge) + (mlcadet[mesh->edges_[edge].from_h] + mlcadet[mesh->edges_[edge].to_h]) * le;
	}
	return edge_mid;
}

V3f InterpolateCC::cal_MLCA_ev(EdgeHandle edge_id)
{
	V3f edge_mid;
	V3f edge_sum;
	if (mesh->is_bdy(edge_id))
	{
		edge_mid = cal_edge_midv(edge_id) + (detver[mesh->edges_[edge_id].from_h] + detver[mesh->edges_[edge_id].to_h]) * le;
	}
	else
	{
		edge_mid = cal_edge_midv(edge_id) + (mlcadet[mesh->edges_[edge_id].from_h] + mlcadet[mesh->edges_[edge_id].to_h]) * le;
	}
	return edge_mid;
}

V3f InterpolateCC::cal_MLCA_fv(const int cell_num, const int face_num)
{
	V3f face_mid;
	V3f face_sum;
	FaceHandle face = mesh->cells_[cell_num].faces_[face_num];//确定该面
	if (mesh->is_bdy(face))
	{
		std::vector<VertexHandle> fv = mesh->faces_[face].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + mesh->vertices_[*fv_it] / 4.0;
			face_sum = face_sum + detver[*fv_it] * 2 * uf;
		}
		face_mid = face_sum;
	}
	else
	{
		face_mid = face_mid + cal_face_midv(face);
		std::vector<VertexHandle> fv = mesh->faces_[face].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + mlcadet[*fv_it] * 2 * uf;
		}
	}
	return face_mid;
}

V3f InterpolateCC::cal_MLCA_fv(FaceHandle face_id)
{
	V3f face_mid;
	V3f face_sum;
	if (mesh->is_bdy(face_id))
	{
		std::vector<VertexHandle> fv = mesh->faces_[face_id].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + mesh->vertices_[*fv_it] / 4.0;
			face_sum = face_sum + detver[*fv_it] * 2 * uf;
		}
		face_mid = face_sum;
	}
	else
	{
		face_mid = face_mid + cal_face_midv(face_id);
		std::vector<VertexHandle> fv = mesh->faces_[face_id].vs_f;
		for (std::vector<VertexHandle>::iterator fv_it = fv.begin(); fv_it != fv.end(); fv_it++)
		{
			face_sum = face_sum + mlcadet[*fv_it] * 2 * uf;
		}
	}
	return face_mid;
}

V3f InterpolateCC::cal_MLCA_cv(const int cell_num)
{
	V3f cell_mid;
	std::vector<VertexHandle> c_v = mesh->cells_[cell_num].vertex_;
	for (std::vector<VertexHandle>::iterator c_v_it = c_v.begin(); c_v_it != c_v.end(); c_v_it++)
	{
		cell_mid = cell_mid + mesh->vertices_[*c_v_it] / 8.0;
		cell_mid = cell_mid + mlcadet[*c_v_it] * 2 * dc;
	}
	return cell_mid;
}

V3f InterpolateCC::cal_MLCA_cv(CellHandle cell_id)
{
	V3f cell_mid;
	std::vector<VertexHandle> c_v = mesh->cells_[cell_id].vertex_;
	for (std::vector<VertexHandle>::iterator c_v_it = c_v.begin(); c_v_it != c_v.end(); c_v_it++)
	{
		cell_mid = cell_mid + mesh->vertices_[*c_v_it] / 8.0;
		cell_mid = cell_mid + mlcadet[*c_v_it] * 2 * dc;
	}
	return cell_mid;
}



/**************************************************
@brief   : MLCA学习
@author  : lee
@input   ：none
@output  ：none
@time    : none
**************************************************/
V3f InterpolateCC::cal_MLCA_nv(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	V3f new_vertex;
	V3f vertex_sum;
	if (mesh->is_bdy(vertex))
	{
		//const int n = mesh->neighbor_v[vertex].size();
		//vertex_sum = vertex_sum + mesh->vertices_[vertex] * n * (n + 5);
		int en = 0;
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int v_e_n = v_e.size();
		V3f e_av;
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
			{
				en++;
				V3f temp_ev = cal_CIPB_ev(*v_e_it);
				e_av = e_av + temp_ev;
				//vertex_sum = vertex_sum - temp_ev * 4;
			}
		}
		e_av = e_av / en;
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * (en + 5);
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		V3f f_av;
		//int fn = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			if (mesh->is_bdy(*v_f_it))
			{
				//fn++;
				V3f temp_fv = cal_CIPB_fv(*v_f_it);
				f_av = f_av + temp_fv;
				//vertex_sum = vertex_sum - temp_fv;
			}
		}
		f_av = f_av / en;
		new_vertex = vertex_sum - e_av * 4 - f_av;
		new_vertex = new_vertex / en;
		//new_vertex = mesh->vertices_[vertex];
	}
	else
	{
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
		int en = v_e.size();
		vertex_sum = vertex_sum + mesh->vertices_[vertex] * (27 + 20 * (en - 2));
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			V3f temp_ev = cal_CIPB_ev(*v_e_it);
			if (mesh->is_singular(*v_e_it))
			{
				vertex_sum = vertex_sum - temp_ev * (en - 2) * 4;
			}
			else
			{
				vertex_sum = vertex_sum - temp_ev * 3 * 3;
			}
		}
		std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
		for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
		{
			V3f temp_fv = cal_CIPB_fv(*v_f_it);
			vertex_sum = vertex_sum - temp_fv * 3;
		}
		std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[vertex];
		for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
		{
			V3f temp_cv = cal_CIPB_cv(*v_c_it);
			vertex_sum = vertex_sum - temp_cv;
		}
		new_vertex = vertex_sum / 27;
		if (mesh->is_singular(vertex))
		{
			//修正奇异点
			//new_vertex = new_vertex - detver[vertex] * 4 * sv;
			/*int size1 = mesh->neighbor_v_f[vertex].size();
			int size2 = mesh->neighbor_v_c[vertex].size();
			int size3 = mesh->neighbor_v[vertex].size();*/
		}
	}
	return new_vertex;
}

/**************************************************
@brief   : MLCA细分算法核心
@author  : lee
@input   ：none
@output  ：none
@time    : none
**************************************************/
HexV3fMesh& InterpolateCC::CCInterpolatingMLCA()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{
		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_v = cal_MLCA_nv(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_MLCA_ev(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_MLCA_ev(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_MLCA_ev(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_MLCA_ev(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_MLCA_ev(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_MLCA_ev(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_MLCA_ev(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_MLCA_ev(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_MLCA_ev(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_MLCA_ev(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_MLCA_ev(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_MLCA_ev(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_MLCA_fv(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cell_mid(i);
		//判断加入数组
		temp_vex[26] = newmesh->add_vertex(cell_mid);
		//-------插入单元的内心结束-------//

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
		sub_cell[2].push_back(temp_vex[18]); sub_cell[2].push_back(temp_vex[25]);
		sub_cell[2].push_back(temp_vex[12]); sub_cell[2].push_back(temp_vex[3]);
		sub_cell[2].push_back(temp_vex[23]); sub_cell[2].push_back(temp_vex[26]);
		sub_cell[2].push_back(temp_vex[21]); sub_cell[2].push_back(temp_vex[13]);
		//第四个单元
		sub_cell[3].push_back(temp_vex[25]); sub_cell[3].push_back(temp_vex[16]);
		sub_cell[3].push_back(temp_vex[2]); sub_cell[3].push_back(temp_vex[12]);
		sub_cell[3].push_back(temp_vex[26]); sub_cell[3].push_back(temp_vex[22]);
		sub_cell[3].push_back(temp_vex[15]); sub_cell[3].push_back(temp_vex[21]);
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
		sub_cell[6].push_back(temp_vex[23]); sub_cell[6].push_back(temp_vex[26]);
		sub_cell[6].push_back(temp_vex[21]); sub_cell[6].push_back(temp_vex[13]);
		sub_cell[6].push_back(temp_vex[19]); sub_cell[6].push_back(temp_vex[24]);
		sub_cell[6].push_back(temp_vex[14]); sub_cell[6].push_back(temp_vex[7]);
		//第八个单元
		sub_cell[7].push_back(temp_vex[26]); sub_cell[7].push_back(temp_vex[22]);
		sub_cell[7].push_back(temp_vex[15]); sub_cell[7].push_back(temp_vex[21]);
		sub_cell[7].push_back(temp_vex[24]); sub_cell[7].push_back(temp_vex[17]);
		sub_cell[7].push_back(temp_vex[6]); sub_cell[7].push_back(temp_vex[14]);
		//插入单元
		for (int j = 0; j < 8; j++) {
			newmesh->add_cell(sub_cell[j]);
		}
	}
	return *newmesh;
}





//------Quad------//
V3f QuadCC::cal_face_midv(FaceHandle face_id)
{
	V3f face_mid;
	std::vector<VertexHandle> f_v = mesh->faces_[face_id].vs_f;
	for (std::vector<VertexHandle>::iterator f_v_it = f_v.begin(); f_v_it != f_v.end(); f_v_it++)
	{
		face_mid = face_mid + mesh->vertices_[*f_v_it];
	}
	face_mid = face_mid / 4;
	return face_mid;
}

V3f QuadCC::cal_face_mid(FaceHandle face_id)
{
	return cal_face_midv(face_id);
}

V3f QuadCC::cal_edge_midv(EdgeHandle edge_id)
{
	V3f edge_mid;
	edge_mid = edge_mid + mesh->vertices_[mesh->edges_[edge_id].from_h];
	edge_mid = edge_mid + mesh->vertices_[mesh->edges_[edge_id].to_h];
	edge_mid = edge_mid / 2;
	return edge_mid;
}

V3f QuadCC::cal_edge_mid(EdgeHandle edge_id)
{
	V3f edge_mid;
	V3f edge_sum;
	edge_sum = edge_sum + cal_edge_midv(edge_id) * 2;
	std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[edge_id];
	int e_f_n = 0;
	for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
	{
		e_f_n++;
		edge_sum = edge_sum + cal_face_midv(*e_f_it);
	}
	edge_mid = edge_sum / (2 + e_f_n);
	return edge_mid;
}

V3f QuadCC::cal_new_vertex(VertexHandle ver_id)
{
	V3f new_vertex;
	V3f vertex_sum;
	std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[ver_id];
	int v_e_n = 0;
	V3f v_e_mid;//计算点邻接边的平均边点
	for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
	{
		v_e_n++;
		v_e_mid = v_e_mid + cal_edge_midv(*v_e_it);
	}
	v_e_mid = v_e_mid / v_e_n;
	vertex_sum = vertex_sum + v_e_mid * 2;
	std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[ver_id];
	int v_f_n = 0;
	V3f v_f_mid;//计算点邻接面的平均面点
	for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
	{
		v_f_n++;
		v_f_mid = v_f_mid + cal_face_midv(*v_f_it);
	}
	v_f_mid = v_f_mid / v_f_n;
	vertex_sum = vertex_sum + v_f_mid;
	vertex_sum = vertex_sum + mesh->vertices_[ver_id] * (v_e_n - 3);
	new_vertex = vertex_sum / v_e_n;
	return new_vertex;
}

V3f QuadCC::cal_new_csv(VertexHandle ver_id)
{
	V3f new_vertex;
	if (is_cs(ver_id))
	{
		new_vertex = new_vertex + mesh->vertices_[ver_id] * (3.0 / 4);
		std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[ver_id];
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
		{
			if (is_cs(*v_e_it))
			{
				if (mesh->vertices_[mesh->edges_[*v_e_it].from_h] == mesh->vertices_[ver_id])
				{
					new_vertex = new_vertex + mesh->vertices_[mesh->edges_[*v_e_it].to_h] * (1.0 / 8);
				}
				else
				{
					new_vertex = new_vertex + mesh->vertices_[mesh->edges_[*v_e_it].from_h] * (1.0 / 8);
				}
			}
		}
	}
	else
	{
		new_vertex = cal_new_vertex(ver_id);
	}
	return new_vertex;
}

V3f QuadCC::cal_es_mid(EdgeHandle edge_id)
{
	V3f edge_mid;
	if (is_cs(edge_id))
		edge_mid = cal_edge_midv(edge_id);
	else
		edge_mid = cal_edge_mid(edge_id);
	return edge_mid;
}

QuadV3fMesh& QuadCC::CCSubWithCrease()
{
	uint64_t face_num = mesh->faces_.size();
	std::vector<bool> new_v_cs;//折痕点
	std::vector<bool> new_vdraw;
	new_v_cs.resize(9 * face_num, false);
	new_vdraw.resize(9 * face_num, false);
	std::vector<V3f> tempnorm(4 * face_num);
	for (uint64_t i = 0; i < face_num; i++)
	{
		VertexHandle temp_vex[9];

		//首先插入原始的4个点
		for (int mesh_vex_num = 0; mesh_vex_num < 4; mesh_vex_num++)
		{
			V3f new_v = cal_new_csv(mesh->faces_[i].vs_f[mesh_vex_num]);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_v);
			if (v_cs[mesh->faces_[i].vs_f[mesh_vex_num]])
			{
				new_v_cs[temp_vex[mesh_vex_num]] = true;
			}
			if (vdraw[mesh->faces_[i].vs_f[mesh_vex_num]])
			{
				new_vdraw[temp_vex[mesh_vex_num]] = true;
			}
		}

		//-------插入边的中点-------//
		for (int j = 0; j < 4; ++j)
		{
			V3f edge_mid = cal_es_mid(mesh->faces_[i].edges_[j]);
			temp_vex[4 + j] = newmesh->add_vertex(edge_mid);
			if (is_cs(mesh->faces_[i].edges_[j]))
			{
				new_v_cs[temp_vex[4 + j]] = true;
			}
			if (vdraw[mesh->edges_[mesh->faces_[i].edges_[j]].from_h] && vdraw[mesh->edges_[mesh->faces_[i].edges_[j]].to_h])
			{
				new_vdraw[temp_vex[4 + j]] = true;
			}
		}

		//-------插入单元的内心-------//
		V3f face_mid = cal_face_mid(FaceHandle(i));
		//判断加入数组
		temp_vex[8] = newmesh->add_vertex(face_mid);
		//-------插入单元的内心结束-------//

		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_face(4);
		//第一个单元
		sub_face[0].push_back(temp_vex[0]);
		sub_face[0].push_back(temp_vex[4]);
		sub_face[0].push_back(temp_vex[8]);
		sub_face[0].push_back(temp_vex[7]);
		//第二个单元
		sub_face[1].push_back(temp_vex[4]);
		sub_face[1].push_back(temp_vex[1]);
		sub_face[1].push_back(temp_vex[5]);
		sub_face[1].push_back(temp_vex[8]);;
		//第三个单元
		sub_face[2].push_back(temp_vex[7]);
		sub_face[2].push_back(temp_vex[8]);
		sub_face[2].push_back(temp_vex[6]);
		sub_face[2].push_back(temp_vex[3]);
		//第四个单元
		sub_face[3].push_back(temp_vex[8]);
		sub_face[3].push_back(temp_vex[5]);
		sub_face[3].push_back(temp_vex[2]);
		sub_face[3].push_back(temp_vex[6]);
		//插入单元
		tempnorm[newmesh->add_face(sub_face[0])] = mesh->normal[i];
		tempnorm[newmesh->add_face(sub_face[1])] = mesh->normal[i];
		tempnorm[newmesh->add_face(sub_face[2])] = mesh->normal[i];
		tempnorm[newmesh->add_face(sub_face[3])] = mesh->normal[i];
	}
	newmesh->normal = tempnorm;
	v_cs = new_v_cs;
	vdraw = new_vdraw;
	return *newmesh;
}