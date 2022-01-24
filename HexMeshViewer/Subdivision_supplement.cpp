#include"Subdivision.h"
#include "BezierSolid.h"
#include "FileHandler.h"
#include <string>

void ApproximationCC::construct_bss1_new()
{
	uint64_t cell_num = mesh->cells_.size();
	std::vector<bool> check_vertex(mesh->vertices_.size(), false);
	std::vector<bool> check_edge(mesh->edges_.size(), false);
	std::vector<bool> check_face(mesh->faces_.size(), false);
	std::map<V3f, int> quick_v3f;
	int global_v = 0;
	bss.init(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		std::cout << "Schedule of construct Bsplinesolid " << i + 1 << ":" << cell_num << std::endl;
		bss.local_bdy[i].resize(27, false);
		bss.Local_V3f[i].resize(27);
		bss.Local_to_Global[i].resize(27);
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			V3f v = mesh->vertices_[mesh->cells_[i].vertex_[vn]];
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].vertex_[vn]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_vertex[mesh->cells_[i].vertex_[vn]] == false)
			{
				quick_v3f[v] = global_v++;
				bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
				check_vertex[mesh->cells_[i].vertex_[vn]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = quick_v3f[v];
		}
		for (int en = 0; en < 12; en++)
		{
			V3f v = cal_edge_midv(mesh->cells_[i].edges_[en]);
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].edges_[en]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_edge[mesh->cells_[i].edges_[en]] == false)
			{
				quick_v3f[v] = global_v++;
				bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
				check_edge[mesh->cells_[i].edges_[en]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = quick_v3f[v];
		}
		for (int fn = 0; fn < 6; fn++)
		{
			V3f v = cal_face_midv(mesh->cells_[i].faces_[fn]);
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].faces_[fn]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_face[mesh->cells_[i].faces_[fn]] == false)
			{
				quick_v3f[v] = global_v++;
				bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
				check_face[mesh->cells_[i].faces_[fn]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = quick_v3f[v];
		}
		V3f cell_midv = cal_cell_midv(CellHandle(i));
		bss.Local_V3f[i][tempsize++] = cell_midv;
		bss.Local_to_Global[i][tempsize - 1] = global_v++;

		//构建每一个块按顺序排序控制点的下标
		std::vector<std::vector<std::vector<int>>> sc(3);
		std::vector<std::vector<int>> sf(3);
		std::vector<int> sl(3);
		//第一层
		sl[0] = 5; sl[1] = 9; sl[2] = 1;
		sf[0] = sl;
		sl[0] = 17; sl[1] = 22; sl[2] = 16;
		sf[1] = sl;
		sl[0] = 6; sl[1] = 15; sl[2] = 2;
		sf[2] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = 10; sl[1] = 20; sl[2] = 8;
		sf[0] = sl;
		sl[0] = 24; sl[1] = 26; sl[2] = 25;
		sf[1] = sl;
		sl[0] = 14; sl[1] = 21; sl[2] = 12;
		sf[2] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = 4; sl[1] = 11; sl[2] = 0;
		sf[0] = sl;
		sl[0] = 19; sl[1] = 23; sl[2] = 18;
		sf[1] = sl;
		sl[0] = 7; sl[1] = 13; sl[2] = 3;
		sf[2] = sl;
		sc[2] = sf;
		bss.Control_Point[i] = sc;

		//构建局部的每个小单元的u,v,w三个方向的次数
		std::vector<int> order(3); order[0] = 2; order[1] = 2; order[2] = 2;
		bss.Local_Order[i] = order;

		//构建局部的每个小单元的u,v,w三个方向的节点向量
		std::vector<std::vector<double>> node(3);
		node[0].push_back(0); node[0].push_back(0); node[0].push_back(0);
		node[0].push_back(1); node[0].push_back(1); node[0].push_back(1);
		node[1].push_back(0); node[1].push_back(0); node[1].push_back(0);
		node[1].push_back(1); node[1].push_back(1); node[1].push_back(1);
		node[2].push_back(0); node[2].push_back(0); node[2].push_back(0);
		node[2].push_back(1); node[2].push_back(1); node[2].push_back(1);
		bss.Local_Node[i] = node;
	}
	int bdy_sum = 0;
	for (int i = 0; i < mesh->vertices_.size(); i++)
	{
		if (mesh->is_bdy(VertexHandle(i)))
			bdy_sum++;
	}
	for (int i = 0; i < mesh->edges_.size(); i++)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
			bdy_sum++;
	}
	for (int i = 0; i < mesh->faces_.size(); i++)
	{
		if (mesh->is_bdy(FaceHandle(i)))
			bdy_sum++;
	}
	bss.Global_size = global_v;
	bss.Global_size_without_bdy = bss.Global_size - bdy_sum;
}

void ApproximationCC::construct_bss_new()
{
	uint64_t cell_num = mesh->cells_.size();
	std::vector<bool> check_vertex(mesh->vertices_.size(), false);
	std::vector<int> check_edge(mesh->edges_.size(), 0);
	//std::vector<bool> check_vertex_and_edge(mesh->vertices_.size(), false);
	std::vector<int> check_face(mesh->faces_.size(), 0);
	//std::vector<bool> check_vertex_and_face(mesh->vertices_.size(), false);
	std::map<V3f, int> quick_v3f;
	int global_v = 0;
	bss.init(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		if (i % 1000 == 0)
			std::cout << "Schedule of construct Bsplinesolid " << i + 1 << ":" << cell_num << std::endl;
		bss.local_bdy[i].resize(64, false);
		bss.Local_V3f[i].resize(64);
		bss.Local_to_Global[i].resize(64);
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			V3f v = solid_corner_point(mesh->cells_[i].vertex_[vn]);
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].vertex_[vn]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_vertex[mesh->cells_[i].vertex_[vn]] == false)
			{
				quick_v3f[v] = global_v++;
				bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
				check_vertex[mesh->cells_[i].vertex_[vn]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = quick_v3f[v];
		}
		for (int vn = 0; vn < 8; vn++)
		{
			V3f v = cal_inter_point(CellHandle(i), mesh->cells_[i].vertex_[vn]);
			bss.Local_V3f[i][tempsize++] = v;
			quick_v3f[v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
		}
		V3f temp_v;
		//第一层
		temp_v = solid_edge_point(i, 9, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[9]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[9]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[5]] == false)
		if (check_edge[mesh->cells_[i].edges_[9]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[9]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[5]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 9, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[9]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[9]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[6]] == false)
		if (check_edge[mesh->cells_[i].edges_[9]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[9]]++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[6]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 1, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[1]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[5]] == false)
		if (check_edge[mesh->cells_[i].edges_[1]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[1]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[5]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 2, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[2]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[5]] == false)
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[2]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[5]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 2, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[2]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[6]] == false)
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[2]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[6]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 7, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[7]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[7]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[6]] == false)
		if (check_edge[mesh->cells_[i].edges_[7]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[7]]++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[6]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 1, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[1]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[1]] == false)
		if (check_edge[mesh->cells_[i].edges_[1]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[1]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[1]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 2, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[2]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[1]] == false)
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[2]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[1]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 2, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[2]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[2]] == false)
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[2]]++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[2]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 7, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[7]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[7]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[2]] == false)
		if (check_edge[mesh->cells_[i].edges_[7]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[7]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[2]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 8, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[8]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[8]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[1]] == false)
		if (check_edge[mesh->cells_[i].edges_[8]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[8]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[1]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 8, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[8]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[8]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[2]] == false)
		if (check_edge[mesh->cells_[i].edges_[8]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[8]]++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[2]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];
		//第二层
		temp_v = solid_edge_point(i, 2, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[2]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[5]] == false)
		if (check_edge[mesh->cells_[i].edges_[2]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[2]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[5]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 4, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[4]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[5]] == false)
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[4]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[5]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 4, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[4]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[6]] == false)
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[4]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[6]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 6, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[6]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[6]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[6]] == false)
		if (check_edge[mesh->cells_[i].edges_[6]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[6]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[6]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 0, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[0]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[5]] == false)
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[0]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[5]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 1, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[1]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[6]] == false)
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[1]]++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[6]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 0, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[0]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[1]] == false)
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[0]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[1]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 1, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[1]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[2]] == false)
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[1]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[2]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 0, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[0]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[1]] == false)
		if (check_edge[mesh->cells_[i].edges_[0]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[0]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[1]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 5, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[5]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[1]] == false)
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[5]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[1]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 5, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[5]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[2]] == false)
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[5]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[2]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 4, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[4]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[2]] == false)
		if (check_edge[mesh->cells_[i].edges_[4]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[4]]++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[2]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];
		//第三层
		temp_v = solid_edge_point(i, 2, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[2]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[4]] == false)
		if (check_edge[mesh->cells_[i].edges_[2]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[2]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[4]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 4, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[4]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[4]] == false)
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[4]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[4]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 4, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[4]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[7]] == false)
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[4]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[7]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 6, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[6]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[6]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[7]] == false)
		if (check_edge[mesh->cells_[i].edges_[6]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[6]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[7]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 0, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[0]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[4]] == false)
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[0]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[4]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 1, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[1]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[7]] == false)
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[1]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[7]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 0, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[0]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[0]] == false)
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[0]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[0]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 1, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[1]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[3]] == false)
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[1]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[3]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 0, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[0]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[0]] == false)
		if (check_edge[mesh->cells_[i].edges_[0]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[0]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[0]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 5, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[5]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[0]] == false)
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[5]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[0]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 5, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[5]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[3]] == false)
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[5]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[3]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 4, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[4]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[3]] == false)
		if (check_edge[mesh->cells_[i].edges_[4]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[4]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[3]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];
		//第4层
		temp_v = solid_edge_point(i, 11, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[11]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[11]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[4]] == false)
		if (check_edge[mesh->cells_[i].edges_[11]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[11]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[4]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 11, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[11]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[11]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[7]] == false)
		if (check_edge[mesh->cells_[i].edges_[11]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[11]]++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[7]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 3, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[3]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[4]] == false)
		if (check_edge[mesh->cells_[i].edges_[3]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[3]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[4]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 3, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[3]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[4]] == false)
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[3]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[4]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 3, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[3]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[7]] == false)
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[3]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[7]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 5, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[5]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[7]] == false)
		if (check_edge[mesh->cells_[i].edges_[5]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[5]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[7]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 3, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[3]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[0]] == false)
		if (check_edge[mesh->cells_[i].edges_[3]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[3]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[0]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 3, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[3]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[0]] == false)
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[3]] ++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[0]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_face_point(i, 3, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_face[mesh->cells_[i].faces_[3]] == false || check_vertex_and_face[mesh->cells_[i].vertex_[3]] == false)
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_face[mesh->cells_[i].faces_[3]]++;
			//check_vertex_and_face[mesh->cells_[i].vertex_[3]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 5, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[5]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[3]] == false)
		if (check_edge[mesh->cells_[i].edges_[5]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[5]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[3]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 10, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[10]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[10]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[0]] == false)
		if (check_edge[mesh->cells_[i].edges_[10]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[10]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[0]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		temp_v = solid_edge_point(i, 10, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[10]))
			bss.local_bdy[i][tempsize - 1] = true;
		//if (check_edge[mesh->cells_[i].edges_[10]] == false || check_vertex_and_edge[mesh->cells_[i].vertex_[3]] == false)
		if (check_edge[mesh->cells_[i].edges_[10]] != 2)
		{
			quick_v3f[temp_v] = global_v++;
			bss.Local_to_Global[i][tempsize - 1] = global_v - 1;
			check_edge[mesh->cells_[i].edges_[10]] ++;
			//check_vertex_and_edge[mesh->cells_[i].vertex_[3]] = true;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = quick_v3f[temp_v];

		//构建每一个块按顺序排序控制点的下标
		std::vector<std::vector<std::vector<int>>> sc(4);
		std::vector<std::vector<int>> sf(4);
		std::vector<int> sl(4);
		//第一层
		sl[0] = 5; sl[1] = 18; sl[2] = 22; sl[3] = 1;
		sf[0] = sl;
		sl[0] = 16; sl[1] = 19; sl[2] = 23; sl[3] = 26;
		sf[1] = sl;
		sl[0] = 17; sl[1] = 20; sl[2] = 24; sl[3] = 27;
		sf[2] = sl;
		sl[0] = 6; sl[1] = 21; sl[2] = 25; sl[3] = 2;
		sf[3] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = 28; sl[1] = 32; sl[2] = 34; sl[3] = 36;
		sf[0] = sl;
		sl[0] = 29; sl[1] = 13; sl[2] = 9; sl[3] = 37;
		sf[1] = sl;
		sl[0] = 30; sl[1] = 14; sl[2] = 10; sl[3] = 38;
		sf[2] = sl;
		sl[0] = 31; sl[1] = 33; sl[2] = 35; sl[3] = 39;
		sf[3] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = 40; sl[1] = 44; sl[2] = 46; sl[3] = 48;
		sf[0] = sl;
		sl[0] = 41; sl[1] = 12; sl[2] = 8; sl[3] = 49;
		sf[1] = sl;
		sl[0] = 42; sl[1] = 15; sl[2] = 11; sl[3] = 50;
		sf[2] = sl;
		sl[0] = 43; sl[1] = 45; sl[2] = 47; sl[3] = 51;
		sf[3] = sl;
		sc[2] = sf;
		//第四层
		sl[0] = 4; sl[1] = 54; sl[2] = 58; sl[3] = 0;
		sf[0] = sl;
		sl[0] = 52; sl[1] = 55; sl[2] = 59; sl[3] = 62;
		sf[1] = sl;
		sl[0] = 53; sl[1] = 56; sl[2] = 60; sl[3] = 63;
		sf[2] = sl;
		sl[0] = 7; sl[1] = 57; sl[2] = 61; sl[3] = 3;
		sf[3] = sl;
		sc[3] = sf;
		bss.Control_Point[i] = sc;

		//构建局部的每个小单元的u,v,w三个方向的次数
		std::vector<int> order(3); order[0] = 3; order[1] = 3; order[2] = 3;
		bss.Local_Order[i] = order;

		//构建局部的每个小单元的u,v,w三个方向的节点向量
		std::vector<std::vector<double>> node(3);
		node[0].push_back(0); node[0].push_back(0); node[0].push_back(0); node[0].push_back(0);
		node[0].push_back(1); node[0].push_back(1); node[0].push_back(1); node[0].push_back(1);
		node[1].push_back(0); node[1].push_back(0); node[1].push_back(0); node[1].push_back(0);
		node[1].push_back(1); node[1].push_back(1); node[1].push_back(1); node[1].push_back(1);
		node[2].push_back(0); node[2].push_back(0); node[2].push_back(0); node[2].push_back(0);
		node[2].push_back(1); node[2].push_back(1); node[2].push_back(1); node[2].push_back(1);
		bss.Local_Node[i] = node;
	}
	int bdy_sum = 0;
	int v_bdy = 0;
	for (int i = 0; i < mesh->vertices_.size(); i++)
	{
		if (mesh->is_bdy(VertexHandle(i)))
		{
			bdy_sum++;
			v_bdy++;
		}
	}
	int e_bdy = 0;
	for (int i = 0; i < mesh->edges_.size(); i++)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			bdy_sum += 2;
			e_bdy++;
		}
	}
	int f_bdy = 0;
	for (int i = 0; i < mesh->faces_.size(); i++)
	{
		if (mesh->is_bdy(FaceHandle(i)))
		{
			f_bdy++;
			bdy_sum += 4;
		}
	}
	bss.Global_size = global_v;
	bss.Global_size_without_bdy = bss.Global_size - bdy_sum;
}

void ApproximationCC::construct_bss_new_new()
{
	uint64_t cell_num = mesh->cells_.size();
	std::vector<bool> check_vertex(mesh->vertices_.size(), false);
	std::vector<int> check_edge(mesh->edges_.size(), 0);
	std::vector<int> check_face(mesh->faces_.size(), 0);
	std::vector<V3f> slow_v3f;
	int global_v = 0;
	bss.init(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		std::cout << "Schedule of construct Bsplinesolid " << i + 1 << ":" << cell_num << std::endl;
		bss.local_bdy[i].resize(64, false);
		bss.Local_V3f[i].resize(64);
		bss.Local_to_Global[i].resize(64);
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			V3f v = solid_corner_point(mesh->cells_[i].vertex_[vn]);
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].vertex_[vn]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_vertex[mesh->cells_[i].vertex_[vn]] == false)
			{
				slow_v3f.push_back(v);
				bss.Local_to_Global[i][tempsize - 1] = global_v++;
				check_vertex[mesh->cells_[i].vertex_[vn]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), v) - slow_v3f.begin();
		}
		for (int vn = 0; vn < 8; vn++)
		{
			V3f v = cal_inter_point(CellHandle(i), mesh->cells_[i].vertex_[vn]);
			bss.Local_V3f[i][tempsize++] = v;
			slow_v3f.push_back(v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
		}
		V3f temp_v;
		//第一层
		temp_v = solid_edge_point(i, 9, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[9]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[9]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[9]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 9, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[9]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[9]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[9]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 1, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[1]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[1]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 2, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[2]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 2, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[2]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 7, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[7]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[7]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[7]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 1, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[1]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[1]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 2, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[2]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 2, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[2]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[2]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 7, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[7]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[7]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[7]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 8, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[8]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[8]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[8]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 8, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[8]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[8]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[8]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();
		//第二层
		temp_v = solid_edge_point(i, 2, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[2]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[2]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 4, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[4]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 4, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[4]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 6, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[6]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[6]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[6]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 0, 5);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[0]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 1, 6);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[1]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 0, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[0]] ++;;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 1, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[1]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 0, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[0]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[0]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 5, 1);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[5]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 5, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[5]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 4, 2);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[4]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[4]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();
		//第三层
		temp_v = solid_edge_point(i, 2, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[2]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[2]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[2]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 4, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[4]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 4, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[4]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[4]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 6, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[6]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[6]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[6]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 0, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[0]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 1, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[1]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 0, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[0]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[0]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 1, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[1]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[1]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[1]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 0, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[0]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[0]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[0]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 5, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[5]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 5, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[5]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[5]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 4, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[4]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[4]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[4]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();
		//第4层
		temp_v = solid_edge_point(i, 11, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[11]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[11]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[11]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 11, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[11]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[11]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[11]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 3, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[3]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[3]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 3, 4);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[3]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 3, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[3]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 5, 7);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[5]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[5]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 3, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[3]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[3]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 3, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[3]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_face_point(i, 3, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].faces_[3]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_face[mesh->cells_[i].faces_[3]] != 4)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_face[mesh->cells_[i].faces_[3]]++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 5, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[5]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[5]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[5]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 10, 0);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[10]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[10]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[10]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		temp_v = solid_edge_point(i, 10, 3);
		bss.Local_V3f[i][tempsize++] = temp_v;
		if (mesh->is_bdy(mesh->cells_[i].edges_[10]))
			bss.local_bdy[i][tempsize - 1] = true;
		if (check_edge[mesh->cells_[i].edges_[10]] != 2)
		{
			slow_v3f.push_back(temp_v);
			bss.Local_to_Global[i][tempsize - 1] = global_v++;
			check_edge[mesh->cells_[i].edges_[10]] ++;
		}
		else
			bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), temp_v) - slow_v3f.begin();

		//构建每一个块按顺序排序控制点的下标
		std::vector<std::vector<std::vector<int>>> sc(4);
		std::vector<std::vector<int>> sf(4);
		std::vector<int> sl(4);
		//第一层
		sl[0] = 5; sl[1] = 18; sl[2] = 22; sl[3] = 1;
		sf[0] = sl;
		sl[0] = 16; sl[1] = 19; sl[2] = 23; sl[3] = 26;
		sf[1] = sl;
		sl[0] = 17; sl[1] = 20; sl[2] = 24; sl[3] = 27;
		sf[2] = sl;
		sl[0] = 6; sl[1] = 21; sl[2] = 25; sl[3] = 2;
		sf[3] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = 28; sl[1] = 32; sl[2] = 34; sl[3] = 36;
		sf[0] = sl;
		sl[0] = 29; sl[1] = 13; sl[2] = 9; sl[3] = 37;
		sf[1] = sl;
		sl[0] = 30; sl[1] = 14; sl[2] = 10; sl[3] = 38;
		sf[2] = sl;
		sl[0] = 31; sl[1] = 33; sl[2] = 35; sl[3] = 39;
		sf[3] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = 40; sl[1] = 44; sl[2] = 46; sl[3] = 48;
		sf[0] = sl;
		sl[0] = 41; sl[1] = 12; sl[2] = 8; sl[3] = 49;
		sf[1] = sl;
		sl[0] = 42; sl[1] = 15; sl[2] = 11; sl[3] = 50;
		sf[2] = sl;
		sl[0] = 43; sl[1] = 45; sl[2] = 47; sl[3] = 51;
		sf[3] = sl;
		sc[2] = sf;
		//第四层
		sl[0] = 4; sl[1] = 54; sl[2] = 58; sl[3] = 0;
		sf[0] = sl;
		sl[0] = 52; sl[1] = 55; sl[2] = 59; sl[3] = 62;
		sf[1] = sl;
		sl[0] = 53; sl[1] = 56; sl[2] = 60; sl[3] = 63;
		sf[2] = sl;
		sl[0] = 7; sl[1] = 57; sl[2] = 61; sl[3] = 3;
		sf[3] = sl;
		sc[3] = sf;
		bss.Control_Point[i] = sc;

		//构建局部的每个小单元的u,v,w三个方向的次数
		std::vector<int> order(3); order[0] = 3; order[1] = 3; order[2] = 3;
		bss.Local_Order[i] = order;

		//构建局部的每个小单元的u,v,w三个方向的节点向量
		std::vector<std::vector<double>> node(3);
		node[0].push_back(0); node[0].push_back(0); node[0].push_back(0); node[0].push_back(0);
		node[0].push_back(1); node[0].push_back(1); node[0].push_back(1); node[0].push_back(1);
		node[1].push_back(0); node[1].push_back(0); node[1].push_back(0); node[1].push_back(0);
		node[1].push_back(1); node[1].push_back(1); node[1].push_back(1); node[1].push_back(1);
		node[2].push_back(0); node[2].push_back(0); node[2].push_back(0); node[2].push_back(0);
		node[2].push_back(1); node[2].push_back(1); node[2].push_back(1); node[2].push_back(1);
		bss.Local_Node[i] = node;
	}
	int bdy_sum = 0;
	int v_bdy = 0;
	for (int i = 0; i < mesh->vertices_.size(); i++)
	{
		if (mesh->is_bdy(VertexHandle(i)))
		{
			bdy_sum++;
			v_bdy++;
		}
	}
	int e_bdy = 0;
	for (int i = 0; i < mesh->edges_.size(); i++)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
		{
			bdy_sum += 2;
			e_bdy++;
		}
	}
	int f_bdy = 0;
	for (int i = 0; i < mesh->faces_.size(); i++)
	{
		if (mesh->is_bdy(FaceHandle(i)))
		{
			f_bdy++;
			bdy_sum += 4;
		}
	}
	bss.Global_size = global_v;
	bss.Global_size_without_bdy = bss.Global_size - bdy_sum;
}

void ApproximationCC::construct_bss1_new_new()
{
	uint64_t cell_num = mesh->cells_.size();
	std::vector<bool> check_vertex(mesh->vertices_.size(), false);
	std::vector<bool> check_edge(mesh->edges_.size(), false);
	std::vector<bool> check_face(mesh->faces_.size(), false);
	std::vector<V3f> slow_v3f;
	int global_v = 0;
	bss.init(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		std::cout << "Schedule of construct Bsplinesolid " << i + 1 << ":" << cell_num << std::endl;
		bss.local_bdy[i].resize(27, false);
		bss.Local_V3f[i].resize(27);
		bss.Local_to_Global[i].resize(27);
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			V3f v = mesh->vertices_[mesh->cells_[i].vertex_[vn]];
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].vertex_[vn]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_vertex[mesh->cells_[i].vertex_[vn]] == false)
			{
				slow_v3f.push_back(v);
				bss.Local_to_Global[i][tempsize - 1] = global_v++;
				check_vertex[mesh->cells_[i].vertex_[vn]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), v) - slow_v3f.begin();
		}
		for (int en = 0; en < 12; en++)
		{
			V3f v = cal_edge_midv(mesh->cells_[i].edges_[en]);
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].edges_[en]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_edge[mesh->cells_[i].edges_[en]] == false)
			{
				slow_v3f.push_back(v);
				bss.Local_to_Global[i][tempsize - 1] = global_v++;
				check_edge[mesh->cells_[i].edges_[en]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), v) - slow_v3f.begin();
		}
		for (int fn = 0; fn < 6; fn++)
		{
			V3f v = cal_face_midv(mesh->cells_[i].faces_[fn]);
			bss.Local_V3f[i][tempsize++] = v;
			if (mesh->is_bdy(mesh->cells_[i].faces_[fn]))
				bss.local_bdy[i][tempsize - 1] = true;
			if (check_face[mesh->cells_[i].faces_[fn]] == false)
			{
				slow_v3f.push_back(v);
				bss.Local_to_Global[i][tempsize - 1] = global_v++;
				check_face[mesh->cells_[i].faces_[fn]] = true;
			}
			else
				bss.Local_to_Global[i][tempsize - 1] = std::find(slow_v3f.begin(), slow_v3f.end(), v) - slow_v3f.begin();
		}
		V3f cell_midv = cal_cell_midv(CellHandle(i));
		bss.Local_V3f[i][tempsize++] = cell_midv;
		bss.Local_to_Global[i][tempsize - 1] = global_v++;

		//构建每一个块按顺序排序控制点的下标
		std::vector<std::vector<std::vector<int>>> sc(3);
		std::vector<std::vector<int>> sf(3);
		std::vector<int> sl(3);
		//第一层
		sl[0] = 5; sl[1] = 9; sl[2] = 1;
		sf[0] = sl;
		sl[0] = 17; sl[1] = 22; sl[2] = 16;
		sf[1] = sl;
		sl[0] = 6; sl[1] = 15; sl[2] = 2;
		sf[2] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = 10; sl[1] = 20; sl[2] = 8;
		sf[0] = sl;
		sl[0] = 24; sl[1] = 26; sl[2] = 25;
		sf[1] = sl;
		sl[0] = 14; sl[1] = 21; sl[2] = 12;
		sf[2] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = 4; sl[1] = 11; sl[2] = 0;
		sf[0] = sl;
		sl[0] = 19; sl[1] = 23; sl[2] = 18;
		sf[1] = sl;
		sl[0] = 7; sl[1] = 13; sl[2] = 3;
		sf[2] = sl;
		sc[2] = sf;
		bss.Control_Point[i] = sc;

		//构建局部的每个小单元的u,v,w三个方向的次数
		std::vector<int> order(3); order[0] = 2; order[1] = 2; order[2] = 2;
		bss.Local_Order[i] = order;

		//构建局部的每个小单元的u,v,w三个方向的节点向量
		std::vector<std::vector<double>> node(3);
		node[0].push_back(0); node[0].push_back(0); node[0].push_back(0);
		node[0].push_back(1); node[0].push_back(1); node[0].push_back(1);
		node[1].push_back(0); node[1].push_back(0); node[1].push_back(0);
		node[1].push_back(1); node[1].push_back(1); node[1].push_back(1);
		node[2].push_back(0); node[2].push_back(0); node[2].push_back(0);
		node[2].push_back(1); node[2].push_back(1); node[2].push_back(1);
		bss.Local_Node[i] = node;
	}
	int bdy_sum = 0;
	for (int i = 0; i < mesh->vertices_.size(); i++)
	{
		if (mesh->is_bdy(VertexHandle(i)))
			bdy_sum++;
	}
	for (int i = 0; i < mesh->edges_.size(); i++)
	{
		if (mesh->is_bdy(EdgeHandle(i)))
			bdy_sum++;
	}
	for (int i = 0; i < mesh->faces_.size(); i++)
	{
		if (mesh->is_bdy(FaceHandle(i)))
			bdy_sum++;
	}
	bss.Global_size = global_v;
	bss.Global_size_without_bdy = bss.Global_size - bdy_sum;
}

//-----ApproximationCC-----//
V3f ApproximationCC::cal_corner_point(VertexHandle _v)
{
	V3f newv;
	int n = 0;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(1,*ve_it))
			n++;
	}
	newv = newv + mesh->vertices_[_v] * (n * n);
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(1,*ve_it))
		{
			if (_v == mesh->edges_[*ve_it].from_h)
				newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 4;
			else
				newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 4;
		}
	}
	std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
	for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
	{
		if (mesh->is_bdy(1,*vf_it))
		{
			V3f opv = mesh->vertices_[mesh->find_op_vertex(_v, *vf_it)];
			newv = newv + opv;
		}
	}
	newv = newv / (n * n + 5 * n);
	return newv;
}

V3f ApproximationCC::cal_corner_point2(VertexHandle _v)
{
	V3f newv;
	int n = 0;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(*ve_it))
			n++;
	}
	newv = newv + mesh->vertices_[_v] * (n * n);
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(*ve_it))
		{
			if (_v == mesh->edges_[*ve_it].from_h)
				newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 4;
			else
				newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 4;
		}
	}
	std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
	for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
	{
		if (mesh->is_bdy(*vf_it))
		{
			V3f opv = mesh->vertices_[mesh->find_op_vertex(_v, *vf_it)];
			newv = newv + opv;
		}
	}
	newv = newv / (n * n + 5 * n);
	return newv;
}

//V3f ApproximationCC::cal_corner_point_1(VertexHandle _v,CellHandle _c)
//{
//	V3f newv;
//	int n = 0;
//	int bdy_edge = 0;
//	int not_bdy_edge = 0;
//	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
//	std::set<CellHandle> c_c_temp = mesh->neighbor_c_c(_c);
//	c_c_temp.insert(_c);
//	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin();ve_it != ve.end();ve_it++)
//	{
//		if (mesh->is_bdy(*ve_it))
//		{
//			n++;
//			for (std::set<CellHandle>::iterator c_c_temp_it = c_c_temp.begin();c_c_temp_it != c_c_temp.end();c_c_temp_it++)
//			{
//				if (mesh->is_e_in_c(*ve_it, *c_c_temp_it))
//				{
//					bdy_edge++;
//					break;
//				}
//			}	
//		}
//		else
//			not_bdy_edge++;	
//	}
//	if (n > 5 || not_bdy_edge < 1)
//	{
//		newv = newv + mesh->vertices_[_v] * (bdy_edge * bdy_edge);
//		for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin();ve_it != ve.end();ve_it++)
//		{
//			for (std::set<CellHandle>::iterator c_c_temp_it = c_c_temp.begin();c_c_temp_it != c_c_temp.end();c_c_temp_it++)
//			{
//				if (mesh->is_bdy(*ve_it) && mesh->is_e_in_c(*ve_it, *c_c_temp_it))
//				{
//					if (_v == mesh->edges_[*ve_it].from_h)
//					{
//						newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 4;
//						break;
//					}
//					else
//					{
//						newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 4;
//						break;
//					}		
//				}
//			}	
//		}
//		std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
//		for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin();vf_it != vf.end();vf_it++)
//		{
//			for (std::set<CellHandle>::iterator c_c_temp_it = c_c_temp.begin();c_c_temp_it != c_c_temp.end();c_c_temp_it++)
//			{
//				if (mesh->is_bdy(*vf_it) && mesh->is_f_in_c(*vf_it, *c_c_temp_it))
//				{
//					V3f opv = mesh->vertices_[mesh->find_op_vertex(_v, *vf_it)];
//					newv = newv + opv;
//					break;
//				}
//			}		
//		}
//		newv = newv / (bdy_edge * bdy_edge + 5 * bdy_edge);
//		return newv;
//	}
//	else
//	{
//		newv = newv + mesh->vertices_[_v] * (n * n);
//		for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin();ve_it != ve.end();ve_it++)
//		{
//			if (mesh->is_bdy(*ve_it))
//			{
//				if (_v == mesh->edges_[*ve_it].from_h)
//					newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 4;
//				else
//					newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 4;
//			}
//		}
//		std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
//		for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin();vf_it != vf.end();vf_it++)
//		{
//			if (mesh->is_bdy(*vf_it))
//			{
//				V3f opv = mesh->vertices_[mesh->find_op_vertex(_v, *vf_it)];
//				newv = newv + opv;
//			}
//		}
//		newv = newv / (n * n + 5 * n);
//		return newv;
//	}	
//}

V3f ApproximationCC::cal_edge_point(EdgeHandle _e, VertexHandle _v)
{
	V3f newv;
	int n = 0;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(1,*ve_it))
			n++;
	}
	newv = newv + mesh->vertices_[_v] * (2 * n);
	std::set<EdgeHandle, compare_OVM> opve;
	VertexHandle opv;
	if (mesh->edges_[_e].from_h == _v)
	{
		opv = mesh->edges_[_e].to_h;
		newv = newv + mesh->vertices_[opv] * 4;
		opve = mesh->neighbor_v[opv];
	}
	else
	{
		opv = mesh->edges_[_e].from_h;
		newv = newv + mesh->vertices_[opv] * 4;
		opve = mesh->neighbor_v[opv];
	}
	std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
	int m = 0;
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(1,*ve_it) && (*ve_it) != _e)
		{
			for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
			{
				if (mesh->is_bdy(1,*ef_it))
				{
					if (mesh->is_e_in_f(*ve_it, *ef_it))
					{
						m++;
						if (mesh->edges_[*ve_it].from_h == _v)
						{
							newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 2;
							break;
						}
						else
						{
							newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 2;
							break;
						}
					}
				}
			}
		}
	}
	for (std::set<EdgeHandle, compare_OVM>::iterator opve_it = opve.begin(); opve_it != opve.end(); opve_it++)
	{
		if (mesh->is_bdy(1,*opve_it) && (*opve_it) != _e)
		{
			for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
			{
				if (mesh->is_bdy(1,*ef_it))
				{
					if (mesh->is_e_in_f(*opve_it, *ef_it))
					{
						if (mesh->edges_[*opve_it].from_h == opv)
						{
							newv = newv + mesh->vertices_[mesh->edges_[*opve_it].to_h];
							break;
						}
						else
						{
							newv = newv + mesh->vertices_[mesh->edges_[*opve_it].from_h];
							break;
						}
					}
				}
			}
		}
	}
	newv = newv / (2 * n + 4 + 3 * m);
	return newv;
}

V3f ApproximationCC::cal_edge_point2(EdgeHandle _e, VertexHandle _v)
{
	V3f newv;
	int n = 0;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(*ve_it))
			n++;
	}
	newv = newv + mesh->vertices_[_v] * (2 * n);
	std::set<EdgeHandle, compare_OVM> opve;
	VertexHandle opv;
	if (mesh->edges_[_e].from_h == _v)
	{
		opv = mesh->edges_[_e].to_h;
		newv = newv + mesh->vertices_[opv] * 4;
		opve = mesh->neighbor_v[opv];
	}
	else
	{
		opv = mesh->edges_[_e].from_h;
		newv = newv + mesh->vertices_[opv] * 4;
		opve = mesh->neighbor_v[opv];
	}
	std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
	int m = 0;
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(*ve_it) && (*ve_it) != _e)
		{
			for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
			{
				if (mesh->is_bdy(*ef_it))
				{
					if (mesh->is_e_in_f(*ve_it, *ef_it))
					{
						m++;
						if (mesh->edges_[*ve_it].from_h == _v)
						{
							newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 2;
							break;
						}
						else
						{
							newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 2;
							break;
						}
					}
				}
			}
		}
	}
	for (std::set<EdgeHandle, compare_OVM>::iterator opve_it = opve.begin(); opve_it != opve.end(); opve_it++)
	{
		if (mesh->is_bdy(*opve_it) && (*opve_it) != _e)
		{
			for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
			{
				if (mesh->is_bdy(*ef_it))
				{
					if (mesh->is_e_in_f(*opve_it, *ef_it))
					{
						if (mesh->edges_[*opve_it].from_h == opv)
						{
							newv = newv + mesh->vertices_[mesh->edges_[*opve_it].to_h];
							break;
						}
						else
						{
							newv = newv + mesh->vertices_[mesh->edges_[*opve_it].from_h];
							break;
						}
					}
				}
			}
		}
	}
	newv = newv / (2 * n + 4 + 3 * m);
	return newv;
}

//V3f ApproximationCC::cal_edge_point_1(EdgeHandle _e, VertexHandle _v, CellHandle _c)
//{
//	V3f newv;
//	int n = 0;
//	int bdy_edge = 0;
//	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
//	std::set<CellHandle> c_c_temp = mesh->neighbor_c_c(_c);
//	c_c_temp.insert(_c);
//	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin();ve_it != ve.end();ve_it++)
//	{
//		if (mesh->is_bdy(*ve_it))
//		{
//			n++;
//			for (std::set<CellHandle>::iterator c_c_temp_it = c_c_temp.begin();c_c_temp_it != c_c_temp.end();c_c_temp_it++)
//			{
//				if (mesh->is_e_in_c(*ve_it, *c_c_temp_it))
//				{
//					bdy_edge++;
//					break;
//				}
//			}
//		}
//	}
//	std::set<FaceHandle, compare_OVM> e_f_temp = mesh->neighbor_e[_e];
//	int ef_temp = 0;
//	for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f_temp.begin();e_f_it != e_f_temp.end();e_f_it++)
//	{
//		if (mesh->is_bdy(*e_f_it))
//			ef_temp++;
//	}
//	if (ef_temp == 4)
//	{
//		newv = newv + mesh->vertices_[_v] * (2 * bdy_edge);
//		std::set<EdgeHandle, compare_OVM> opve;
//		VertexHandle opv;
//		if (mesh->edges_[_e].from_h == _v)
//		{
//			opv = mesh->edges_[_e].to_h;
//			newv = newv + mesh->vertices_[opv] * 4;
//			opve = mesh->neighbor_v[opv];
//		}
//		else
//		{
//			opv = mesh->edges_[_e].from_h;
//			newv = newv + mesh->vertices_[opv] * 4;
//			opve = mesh->neighbor_v[opv];
//		}
//		std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
//		int m = 0;
//		for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin();ve_it != ve.end();ve_it++)
//		{
//			if (mesh->is_bdy(*ve_it) && (*ve_it) != _e && mesh->is_e_in_c(*ve_it,_c))
//			{
//				for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin();ef_it != ef.end();ef_it++)
//				{
//					if (mesh->is_bdy(*ef_it))
//					{
//						if (mesh->is_e_in_f(*ve_it, *ef_it))
//						{
//							m++;
//							if (mesh->edges_[*ve_it].from_h == _v)
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 2;
//								break;
//							}
//							else
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 2;
//								break;
//							}
//						}
//					}
//				}
//			}
//		}
//		for (std::set<EdgeHandle, compare_OVM>::iterator opve_it = opve.begin();opve_it != opve.end();opve_it++)
//		{
//			if (mesh->is_bdy(*opve_it) && (*opve_it) != _e&& mesh->is_e_in_c(*opve_it, _c))
//			{
//				for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin();ef_it != ef.end();ef_it++)
//				{
//					if (mesh->is_bdy(*ef_it))
//					{
//						if (mesh->is_e_in_f(*opve_it, *ef_it))
//						{
//							if (mesh->edges_[*opve_it].from_h == opv)
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*opve_it].to_h];
//								break;
//							}
//							else
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*opve_it].from_h];
//								break;
//							}
//						}
//					}
//				}
//			}
//		}
//		newv = newv / (2 * bdy_edge + 4 + 3 * m);
//		return newv;
//	}
//	else
//	{
//		newv = newv + mesh->vertices_[_v] * (2 * n);
//		std::set<EdgeHandle, compare_OVM> opve;
//		VertexHandle opv;
//		if (mesh->edges_[_e].from_h == _v)
//		{
//			opv = mesh->edges_[_e].to_h;
//			newv = newv + mesh->vertices_[opv] * 4;
//			opve = mesh->neighbor_v[opv];
//		}
//		else
//		{
//			opv = mesh->edges_[_e].from_h;
//			newv = newv + mesh->vertices_[opv] * 4;
//			opve = mesh->neighbor_v[opv];
//		}
//		std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
//		int m = 0;
//		for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin();ve_it != ve.end();ve_it++)
//		{
//			if (mesh->is_bdy(*ve_it) && (*ve_it) != _e)
//			{
//				for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin();ef_it != ef.end();ef_it++)
//				{
//					if (mesh->is_bdy(*ef_it))
//					{
//						if (mesh->is_e_in_f(*ve_it, *ef_it))
//						{
//							m++;
//							if (mesh->edges_[*ve_it].from_h == _v)
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 2;
//								break;
//							}
//							else
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 2;
//								break;
//							}
//						}
//					}
//				}
//			}
//		}
//		for (std::set<EdgeHandle, compare_OVM>::iterator opve_it = opve.begin();opve_it != opve.end();opve_it++)
//		{
//			if (mesh->is_bdy(*opve_it) && (*opve_it) != _e)
//			{
//				for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin();ef_it != ef.end();ef_it++)
//				{
//					if (mesh->is_bdy(*ef_it))
//					{
//						if (mesh->is_e_in_f(*opve_it, *ef_it))
//						{
//							if (mesh->edges_[*opve_it].from_h == opv)
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*opve_it].to_h];
//								break;
//							}
//							else
//							{
//								newv = newv + mesh->vertices_[mesh->edges_[*opve_it].from_h];
//								break;
//							}
//						}
//					}
//				}
//			}
//		}
//		newv = newv / (2 * n + 4 + 3 * m);
//		return newv;
//	}	
//}

V3f ApproximationCC::cal_inter_point(FaceHandle _f, VertexHandle _v)
{
	V3f newv;
	int n = 0;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(1,*ve_it))
			n++;
	}
	newv = newv + mesh->vertices_[_v] * n;
	std::vector<EdgeHandle> fe = mesh->faces_[_f].edges_;
	for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end(); fe_it++)
	{
		if (mesh->edges_[*fe_it].from_h == _v)
			newv = newv + mesh->vertices_[mesh->edges_[*fe_it].to_h] * 2;
		else if (mesh->edges_[*fe_it].to_h == _v)
			newv = newv + mesh->vertices_[mesh->edges_[*fe_it].from_h] * 2;
	}
	VertexHandle opv = mesh->find_op_vertex(_v, _f);
	newv = newv + mesh->vertices_[opv];
	newv = newv / (n + 5);
	return newv;
}

V3f ApproximationCC::cal_inter_point2(FaceHandle _f, VertexHandle _v)
{
	V3f newv;
	int n = 0;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_bdy(*ve_it))
			n++;
	}
	newv = newv + mesh->vertices_[_v] * n;
	std::vector<EdgeHandle> fe = mesh->faces_[_f].edges_;
	for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end(); fe_it++)
	{
		if (mesh->edges_[*fe_it].from_h == _v)
			newv = newv + mesh->vertices_[mesh->edges_[*fe_it].to_h] * 2;
		else if (mesh->edges_[*fe_it].to_h == _v)
			newv = newv + mesh->vertices_[mesh->edges_[*fe_it].from_h] * 2;
	}
	VertexHandle opv = mesh->find_op_vertex(_v, _f);
	newv = newv + mesh->vertices_[opv];
	newv = newv / (n + 5);
	return newv;
}

V3f ApproximationCC::cal_inter_point(CellHandle _c, VertexHandle _v)
{
	/*V3f newv;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	std::vector<VertexHandle> temp;
	temp.push_back(_v);
	int n = ve.size();
	int sum = 0;
	if (mesh->is_bdy(_v))
	{
		int tn = 0;
		for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
		{
			if (mesh->is_bdy(*ve_it))
				tn++;
		}
		newv = newv + mesh->vertices_[_v] * 2 * tn;
		sum = sum + 2 * tn;
	}
	else
	{
		newv = newv + mesh->vertices_[_v] * 2 * (n - 2);
		sum = sum + 2 * (n - 2);
	}
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_e_in_c(*ve_it, _c))
		{
			if (mesh->is_singular(*ve_it) && !mesh->is_bdy(*ve_it))
			{
				int m = mesh->neighbor_e[*ve_it].size();
				if (mesh->edges_[*ve_it].from_h == _v)
				{
					newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * m;
					sum = sum + m;
					temp.push_back(mesh->edges_[*ve_it].to_h);
				}
				else
				{
					newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * m;
					sum = sum + m;
					temp.push_back(mesh->edges_[*ve_it].from_h);
				}
			}
			else
			{
				if (mesh->edges_[*ve_it].from_h == _v)
				{
					newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 4;
					sum = sum + 4;
					temp.push_back(mesh->edges_[*ve_it].to_h);
				}
				else
				{
					newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 4;
					sum = sum + 4;
					temp.push_back(mesh->edges_[*ve_it].from_h);
				}
			}
		}
	}
	std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
	for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
	{
		if (mesh->is_f_in_c(*vf_it, _c))
		{
			VertexHandle fopv = mesh->find_op_vertex(_v, *vf_it);
			newv = newv + mesh->vertices_[fopv] * 2;
			sum = sum + 2;
			temp.push_back(fopv);
		}
	}
	std::vector<VertexHandle> cv = mesh->cells_[_c].vertex_;
	for (std::vector<VertexHandle>::iterator cv_it = cv.begin(); cv_it != cv.end(); cv_it++)
	{
		if (std::find(temp.begin(), temp.end(), *cv_it) == temp.end())
		{
			newv = newv + mesh->vertices_[*cv_it];
			sum = sum + 1;
			break;
		}
	}
	newv = newv / sum;
	return newv;*/
	return cal_inter_point1(_c, _v);
}

V3f ApproximationCC::cal_corner_pointsharp(VertexHandle _v)
{
	bool check = false;//是否为尖锐边
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (auto ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (std::find(sharpedge.begin(), sharpedge.end(), *ve_it) != sharpedge.end())
		{
			check = true;
			break;
		}
	}
	V3f newv(0, 0, 0);
	if (check == true)
	{
		newv = newv + mesh->vertices_[_v] * 4;
		for (auto ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
		{
			if (std::find(sharpedge.begin(), sharpedge.end(), *ve_it) != sharpedge.end())
			{
				EdgeHandle e = *ve_it;
				if (mesh->edges_[e].from_h == _v)
					newv = newv + mesh->vertices_[mesh->edges_[e].to_h];
				else
					newv = newv + mesh->vertices_[mesh->edges_[e].from_h];
			}
		}
		newv = newv / 6;
	}
	else
		newv = cal_corner_point2(_v);
	return newv;
}

V3f ApproximationCC::cal_edge_pointsharp(EdgeHandle _e, VertexHandle _v)
{
	bool check = false;//是否为尖锐边
	if (std::find(sharpedge.begin(), sharpedge.end(), _e) != sharpedge.end())
		check = true;
	V3f newv(0, 0, 0);
	if (check == true)
	{
		newv = newv + mesh->vertices_[_v] * 2;
		if (mesh->edges_[_e].from_h == _v)
			newv = newv + mesh->vertices_[mesh->edges_[_e].to_h];
		else
			newv = newv + mesh->vertices_[mesh->edges_[_e].from_h];
		newv = newv / 3;
	}
	else
		newv = cal_edge_point2(_e, _v);
	return newv;
}

V3f ApproximationCC::cal_inter_pointsharp(FaceHandle _f, VertexHandle _v)
{
	bool check = false;//是否为尖锐边
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	for (auto ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (std::find(sharpedge.begin(), sharpedge.end(), *ve_it) != sharpedge.end())
		{
			check = true;
			break;
		}
	}
	V3f newv(0, 0, 0);
	if (check == true)
	{
		newv = newv + mesh->vertices_[_v] * 4;
		std::vector<EdgeHandle> fe = mesh->faces_[_f].edges_;
		for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end(); fe_it++)
		{
			if (mesh->edges_[*fe_it].from_h == _v)
				newv = newv + mesh->vertices_[mesh->edges_[*fe_it].to_h] * 2;
			else if (mesh->edges_[*fe_it].to_h == _v)
				newv = newv + mesh->vertices_[mesh->edges_[*fe_it].from_h] * 2;
		}
		VertexHandle opv = mesh->find_op_vertex(_v, _f);
		newv = newv + mesh->vertices_[opv];
		newv = newv / 9;
	}
	else
		newv = cal_inter_point2(_f, _v);
	return newv;
}

void ApproximationCC::approximationing(int)
{
	int face_num = mesh->faces_.size();
	/*std::vector<V3f> check;
	std::vector<V3f> check1;*/
	for (int i = 0; i < face_num; i++)//以面为单位遍历
	{
		if (mesh->is_bdy(1,FaceHandle(i)))
		{
			biaojiface.push_back(FaceHandle(i));
			std::vector<V3f> temp;
			std::vector<EdgeHandle> fe = mesh->faces_[i].edges_;
			//VertexHandle markv = mesh->edges_[*(fe.begin())].from_h;//确定方向
			VertexHandle fev, feopv;
			V3f cornerv, interv, epv, epopv;
			//边排序
			/*第1条边*/
			fev = mesh->edges_[fe[0]].from_h; feopv = mesh->edges_[fe[0]].to_h;
			cornerv = cal_corner_point(fev);
			interv = cal_inter_point(FaceHandle(i), fev);
			epv = cal_edge_point(fe[0], fev);
			epopv = cal_edge_point(fe[0], feopv);
			temp.push_back(cornerv);
			temp.push_back(interv);
			temp.push_back(epv);
			temp.push_back(epopv);
			std::vector<EdgeHandle> tempev;
			EdgeHandle tempe = fe[0];
			tempev.push_back(tempe);
			/*后3条边*/
			for (int laternum = 0; laternum < 3; laternum++)
			{
				for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end(); fe_it++)
				{
					if (find(tempev.begin(), tempev.end(), *fe_it) == tempev.end())
					{
						if (mesh->edges_[*fe_it].from_h == feopv)
						{
							tempe = *fe_it;
							tempev.push_back(tempe);
							fev = mesh->edges_[*fe_it].from_h;
							feopv = mesh->edges_[*fe_it].to_h;
							break;
						}
						else if (mesh->edges_[*fe_it].to_h == feopv)
						{
							tempe = *fe_it;
							tempev.push_back(tempe);
							fev = mesh->edges_[*fe_it].to_h;
							feopv = mesh->edges_[*fe_it].from_h;
							break;
						}
						else
							continue;
					}
				}
				cornerv = cal_corner_point(fev);
				interv = cal_inter_point(FaceHandle(i), fev);
				epv = cal_edge_point(tempe, fev);
				epopv = cal_edge_point(tempe, feopv);
				temp.push_back(cornerv);
				temp.push_back(interv);
				temp.push_back(epv);
				temp.push_back(epopv);
			}

			//for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end();fe_it++)//以边来遍历这16个控制点
			//{
			//	VertexHandle frv = mesh->edges_[*fe_it].from_h;
			//	VertexHandle tov = mesh->edges_[*fe_it].to_h;
			//	if (markv == frv)
			//	{
			//		fev = frv;
			//		feopv = tov;
			//		markv = tov;
			//	}
			//	else
			//	{
			//		fev = tov;
			//		feopv = frv;
			//		markv = frv;
			//	}
			//	V3f cornerv = cal_corner_point(fev);
			//	V3f interv = cal_inter_point(FaceHandle(i), fev);
			//	V3f epv = cal_edge_point(*fe_it, fev);
			//	V3f epopv = cal_edge_point(*fe_it, feopv);
			//	temp.push_back(cornerv);
			//	temp.push_back(interv);
			//	temp.push_back(epv);
			//	temp.push_back(epopv);
			//	if (find(check.begin(), check.end(), epv) == check.end())
			//		check.push_back(epv);
			//	check1.push_back(epv);
			//}
			/*int tempsize = temp.size();
			for (int i = 0; i < tempsize;i++)
			{
				if (find(check.begin(), check.end(), temp[i]) == check.end())
					check.push_back(temp[i]);
				check1.push_back(temp[i]);
			}*/
			std::vector<V3f> f1, f2, f3, f4, f5, f6, f7, f8, f9;
			f1.push_back(temp[0]); f1.push_back(temp[2]); f1.push_back(temp[1]); f1.push_back(temp[15]);
			f2.push_back(temp[2]); f2.push_back(temp[3]); f2.push_back(temp[5]); f2.push_back(temp[1]);
			f3.push_back(temp[3]); f3.push_back(temp[4]); f3.push_back(temp[6]); f3.push_back(temp[5]);
			f4.push_back(temp[15]); f4.push_back(temp[1]); f4.push_back(temp[13]); f4.push_back(temp[14]);
			f5.push_back(temp[1]); f5.push_back(temp[5]); f5.push_back(temp[9]); f5.push_back(temp[13]);
			f6.push_back(temp[5]); f6.push_back(temp[6]); f6.push_back(temp[7]); f6.push_back(temp[9]);
			f7.push_back(temp[14]); f7.push_back(temp[13]); f7.push_back(temp[11]); f7.push_back(temp[12]);
			f8.push_back(temp[13]); f8.push_back(temp[9]); f8.push_back(temp[10]); f8.push_back(temp[11]);
			f9.push_back(temp[9]); f9.push_back(temp[7]); f9.push_back(temp[8]); f9.push_back(temp[10]);
			surv.push_back(f1); surv.push_back(f2); surv.push_back(f3); surv.push_back(f4); surv.push_back(f5);
			surv.push_back(f6); surv.push_back(f7); surv.push_back(f8); surv.push_back(f9);
			std::vector<V3f> beiv1, beiv2, beiv3, beiv4;
			std::vector<std::vector<V3f>> beif;
			beiv1.push_back(temp[0]); beiv1.push_back(temp[15]); beiv1.push_back(temp[14]); beiv1.push_back(temp[12]);
			beiv2.push_back(temp[2]); beiv2.push_back(temp[1]); beiv2.push_back(temp[13]); beiv2.push_back(temp[11]);
			beiv3.push_back(temp[3]); beiv3.push_back(temp[5]); beiv3.push_back(temp[9]); beiv3.push_back(temp[10]);
			beiv4.push_back(temp[4]); beiv4.push_back(temp[6]); beiv4.push_back(temp[7]); beiv4.push_back(temp[8]);
			beif.push_back(beiv1); beif.push_back(beiv2); beif.push_back(beiv3); beif.push_back(beiv4);
			beiv.push_back(beif);
		}
	}
	/*std::vector<V3f> check;
	for (int i = 0; i < surv.size(); ++i)
	{
		for (int j = 0;j < surv[i].size();++j)
		{
			if (find(check.begin(), check.end(), surv[i][j]) == check.end())
				check.push_back(surv[i][j]);
		}
	}
	int checknum = check.size();*/
}

void ApproximationCC::approximationing()
{
	int face_num = mesh->faces_.size();
	for (int i = 0; i < face_num; i++)//以面为单位遍历
	{
		if (mesh->is_bdy(FaceHandle(i)))
		{
			biaojiface.push_back(FaceHandle(i));
			std::vector<V3f> temp;
			std::vector<EdgeHandle> fe = mesh->faces_[i].edges_;
			VertexHandle fev, feopv;
			V3f cornerv, interv, epv, epopv;
			//边排序
			/*第1条边*/
			fev = mesh->edges_[fe[0]].from_h; feopv = mesh->edges_[fe[0]].to_h;
			cornerv = cal_corner_point2(fev);
			interv = cal_inter_point2(FaceHandle(i), fev);
			epv = cal_edge_point2(fe[0], fev);
			epopv = cal_edge_point2(fe[0], feopv);
			temp.push_back(cornerv);
			temp.push_back(interv);
			temp.push_back(epv);
			temp.push_back(epopv);
			std::vector<EdgeHandle> tempev;
			EdgeHandle tempe = fe[0];
			tempev.push_back(tempe);
			/*后3条边*/
			for (int laternum = 0; laternum < 3; laternum++)
			{
				for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end(); fe_it++)
				{
					if (find(tempev.begin(), tempev.end(), *fe_it) == tempev.end())
					{
						if (mesh->edges_[*fe_it].from_h == feopv)
						{
							tempe = *fe_it;
							tempev.push_back(tempe);
							fev = mesh->edges_[*fe_it].from_h;
							feopv = mesh->edges_[*fe_it].to_h;
							break;
						}
						else if (mesh->edges_[*fe_it].to_h == feopv)
						{
							tempe = *fe_it;
							tempev.push_back(tempe);
							fev = mesh->edges_[*fe_it].to_h;
							feopv = mesh->edges_[*fe_it].from_h;
							break;
						}
						else
							continue;
					}
				}
				cornerv = cal_corner_point2(fev);
				interv = cal_inter_point2(FaceHandle(i), fev);
				epv = cal_edge_point2(tempe, fev);
				epopv = cal_edge_point2(tempe, feopv);
				temp.push_back(cornerv);
				temp.push_back(interv);
				temp.push_back(epv);
				temp.push_back(epopv);
			}
			std::vector<V3f> f1, f2, f3, f4, f5, f6, f7, f8, f9;
			f1.push_back(temp[0]); f1.push_back(temp[2]); f1.push_back(temp[1]); f1.push_back(temp[15]);
			f2.push_back(temp[2]); f2.push_back(temp[3]); f2.push_back(temp[5]); f2.push_back(temp[1]);
			f3.push_back(temp[3]); f3.push_back(temp[4]); f3.push_back(temp[6]); f3.push_back(temp[5]);
			f4.push_back(temp[15]); f4.push_back(temp[1]); f4.push_back(temp[13]); f4.push_back(temp[14]);
			f5.push_back(temp[1]); f5.push_back(temp[5]); f5.push_back(temp[9]); f5.push_back(temp[13]);
			f6.push_back(temp[5]); f6.push_back(temp[6]); f6.push_back(temp[7]); f6.push_back(temp[9]);
			f7.push_back(temp[14]); f7.push_back(temp[13]); f7.push_back(temp[11]); f7.push_back(temp[12]);
			f8.push_back(temp[13]); f8.push_back(temp[9]); f8.push_back(temp[10]); f8.push_back(temp[11]);
			f9.push_back(temp[9]); f9.push_back(temp[7]); f9.push_back(temp[8]); f9.push_back(temp[10]);
			surv.push_back(f1); surv.push_back(f2); surv.push_back(f3); surv.push_back(f4); surv.push_back(f5);
			surv.push_back(f6); surv.push_back(f7); surv.push_back(f8); surv.push_back(f9);
			std::vector<V3f> beiv1, beiv2, beiv3, beiv4;
			std::vector<std::vector<V3f>> beif;
			beiv1.push_back(temp[0]); beiv1.push_back(temp[15]); beiv1.push_back(temp[14]); beiv1.push_back(temp[12]);
			beiv2.push_back(temp[2]); beiv2.push_back(temp[1]); beiv2.push_back(temp[13]); beiv2.push_back(temp[11]);
			beiv3.push_back(temp[3]); beiv3.push_back(temp[5]); beiv3.push_back(temp[9]); beiv3.push_back(temp[10]);
			beiv4.push_back(temp[4]); beiv4.push_back(temp[6]); beiv4.push_back(temp[7]); beiv4.push_back(temp[8]);
			beif.push_back(beiv1); beif.push_back(beiv2); beif.push_back(beiv3); beif.push_back(beiv4);
			beiv.push_back(beif);
		}
	}
}

void ApproximationCC::cal_nor_sur()
{
	int sursize = surv.size();
	for (int i = 0; i < sursize; i++)
	{
		V3f a; V3f b; V3f n;
		a = surv[i][1] - surv[i][0];
		b = surv[i][3] - surv[i][0];
		n = a % b;
		n = n.normalize();
		nor_surv.push_back(n);
	}
}

void ApproximationCC::cal_nor_beisur()
{
	int beisursize = bei_surv.size();
	for (int i = 0; i < beisursize; i++)
	{
		V3f a; V3f b; V3f n;
		a = bei_surv[i][1] - bei_surv[i][0];
		b = bei_surv[i][3] - bei_surv[i][0];
		n = a % b;
		V3f c = mesh->center - a;
		double temp = n.x*c.x + n.y*c.y + n.z*c.z;
		if (temp > 0)
			n = n * -1;
		n = n.normalize();
		nor_bei_surv.push_back(n);
	}
}

void ApproximationCC::constructBezier()
{
	for (std::vector<std::vector<std::vector<V3f>>>::iterator beiv_it = beiv.begin(); beiv_it != beiv.end(); beiv_it++)
	{
		std::vector<std::vector<V3f>> f_beiv = *beiv_it;
		Wm4::Vector3<double> **bv;
		bv = WM4_NEW Wm4::Vector3<double>*[4];
		bv[0] = WM4_NEW Wm4::Vector3<double>[16];
		for (int i = 1; i < 4; i++)
		{
			bv[i] = &bv[0][i * 4];
		}
		/*int i = 0, j = 0;
		for (std::vector<std::vector<V3f>>::iterator f_beiv_it = f_beiv.begin();f_beiv_it != f_beiv.end();f_beiv_it++)
		{
			std::vector<V3f> f_beiv_v = *f_beiv_it;
			for (std::vector<V3f>::iterator f_beiv_v_it = f_beiv_v.begin();f_beiv_v_it != f_beiv_v.end();f_beiv_v_it++)
			{
				bv[i][j] = Wm4::Vector3<double>((*f_beiv_v_it).x, (*f_beiv_v_it).y, (*f_beiv_v_it).x);
				j++;
			}
			i++;
		}*/
		bv[0][0] = Wm4::Vector3<double>(f_beiv[0][0].x, f_beiv[0][0].y, f_beiv[0][0].z);
		bv[0][1] = Wm4::Vector3<double>(f_beiv[0][1].x, f_beiv[0][1].y, f_beiv[0][1].z);
		bv[0][2] = Wm4::Vector3<double>(f_beiv[0][2].x, f_beiv[0][2].y, f_beiv[0][2].z);
		bv[0][3] = Wm4::Vector3<double>(f_beiv[0][3].x, f_beiv[0][3].y, f_beiv[0][3].z);
		bv[1][0] = Wm4::Vector3<double>(f_beiv[1][0].x, f_beiv[1][0].y, f_beiv[1][0].z);
		bv[1][1] = Wm4::Vector3<double>(f_beiv[1][1].x, f_beiv[1][1].y, f_beiv[1][1].z);
		bv[1][2] = Wm4::Vector3<double>(f_beiv[1][2].x, f_beiv[1][2].y, f_beiv[1][2].z);
		bv[1][3] = Wm4::Vector3<double>(f_beiv[1][3].x, f_beiv[1][3].y, f_beiv[1][3].z);
		bv[2][0] = Wm4::Vector3<double>(f_beiv[2][0].x, f_beiv[2][0].y, f_beiv[2][0].z);
		bv[2][1] = Wm4::Vector3<double>(f_beiv[2][1].x, f_beiv[2][1].y, f_beiv[2][1].z);
		bv[2][2] = Wm4::Vector3<double>(f_beiv[2][2].x, f_beiv[2][2].y, f_beiv[2][2].z);
		bv[2][3] = Wm4::Vector3<double>(f_beiv[2][3].x, f_beiv[2][3].y, f_beiv[2][3].z);
		bv[3][0] = Wm4::Vector3<double>(f_beiv[3][0].x, f_beiv[3][0].y, f_beiv[3][0].z);
		bv[3][1] = Wm4::Vector3<double>(f_beiv[3][1].x, f_beiv[3][1].y, f_beiv[3][1].z);
		bv[3][2] = Wm4::Vector3<double>(f_beiv[3][2].x, f_beiv[3][2].y, f_beiv[3][2].z);
		bv[3][3] = Wm4::Vector3<double>(f_beiv[3][3].x, f_beiv[3][3].y, f_beiv[3][3].z);
		Wm4::BSplineRectangle<double> tempbv(4, 4, bv, 3, 3, false, false, true, true);
		int size = 10;//要生成size * size个面的控制网格 即(size +1) * (size + 1)个控制顶点
		std::vector<std::vector<V3f>> f(size * size);
		std::vector<V3f> bezierv((size + 1) * (size + 1));
		for (int i = 0; i < bezierv.size(); i++)
		{
			bezierv[i] = tempbv.P(i % (size + 1) * (1.0 / size), i / (size + 1) * (1.0 / size));
		}
		for (int i = 0; i < f.size(); i++)
		{
			int j = i + i / size;
			f[i].push_back(bezierv[j]); f[i].push_back(bezierv[j + 1]);
			f[i].push_back(bezierv[j + size + 2]); f[i].push_back(bezierv[j + size + 1]);
			bei_surv.push_back(f[i]);
		}
		/*bezierv[0] = tempbv.P(0, 0);bezierv[1] = tempbv.P(0.25, 0);bezierv[2] = tempbv.P(0.5, 0);bezierv[3] = tempbv.P(0.75, 0);bezierv[4] = tempbv.P(1, 0);
		bezierv[5] = tempbv.P(0, 0.25);bezierv[6] = tempbv.P(0.25, 0.25);bezierv[7] = tempbv.P(0.5, 0.25);bezierv[8] = tempbv.P(0.75, 0.25);bezierv[9] = tempbv.P(1, 0.25);
		bezierv[10] = tempbv.P(0, 0.5);bezierv[11] = tempbv.P(0.25, 0.5);bezierv[12] = tempbv.P(0.5, 0.5);bezierv[13] = tempbv.P(0.75, 0.5);bezierv[14] = tempbv.P(1, 0.5);
		bezierv[15] = tempbv.P(0, 0.75);bezierv[16] = tempbv.P(0.25, 0.75);bezierv[17] = tempbv.P(0.5, 0.75);bezierv[18] = tempbv.P(0.75, 0.75);bezierv[19] = tempbv.P(1, 0.75);
		bezierv[20] = tempbv.P(0, 1);bezierv[21] = tempbv.P(0.25, 1);bezierv[22] = tempbv.P(0.5, 1);bezierv[23] = tempbv.P(0.75, 1);bezierv[24] = tempbv.P(1, 1);*/
		/*f[0].push_back(bezierv[0]);f[0].push_back(bezierv[1]);f[0].push_back(bezierv[6]);f[0].push_back(bezierv[5]);
		f[1].push_back(bezierv[1]);f[1].push_back(bezierv[2]);f[1].push_back(bezierv[7]);f[1].push_back(bezierv[6]);
		f[2].push_back(bezierv[2]);f[2].push_back(bezierv[3]);f[2].push_back(bezierv[8]);f[2].push_back(bezierv[7]);
		f[3].push_back(bezierv[3]);f[3].push_back(bezierv[4]);f[3].push_back(bezierv[9]);f[3].push_back(bezierv[8]);
		f[4].push_back(bezierv[5]);f[4].push_back(bezierv[6]);f[4].push_back(bezierv[11]);f[4].push_back(bezierv[10]);
		f[5].push_back(bezierv[6]);f[5].push_back(bezierv[7]);f[5].push_back(bezierv[12]);f[5].push_back(bezierv[11]);
		f[6].push_back(bezierv[7]);f[6].push_back(bezierv[8]);f[6].push_back(bezierv[13]);f[6].push_back(bezierv[12]);
		f[7].push_back(bezierv[8]);f[7].push_back(bezierv[9]);f[7].push_back(bezierv[14]);f[7].push_back(bezierv[13]);
		f[8].push_back(bezierv[10]);f[8].push_back(bezierv[11]);f[8].push_back(bezierv[16]);f[8].push_back(bezierv[15]);
		f[9].push_back(bezierv[11]);f[9].push_back(bezierv[12]);f[9].push_back(bezierv[17]);f[9].push_back(bezierv[16]);
		f[10].push_back(bezierv[12]);f[10].push_back(bezierv[13]);f[10].push_back(bezierv[18]);f[10].push_back(bezierv[17]);
		f[11].push_back(bezierv[13]);f[11].push_back(bezierv[14]);f[11].push_back(bezierv[19]);f[11].push_back(bezierv[18]);
		f[12].push_back(bezierv[15]);f[12].push_back(bezierv[16]);f[12].push_back(bezierv[21]);f[12].push_back(bezierv[20]);
		f[13].push_back(bezierv[16]);f[13].push_back(bezierv[17]);f[13].push_back(bezierv[22]);f[13].push_back(bezierv[21]);
		f[14].push_back(bezierv[17]);f[14].push_back(bezierv[18]);f[14].push_back(bezierv[23]);f[14].push_back(bezierv[22]);
		f[15].push_back(bezierv[18]);f[15].push_back(bezierv[19]);f[15].push_back(bezierv[24]);f[15].push_back(bezierv[23]);*/
		/*bei_surv.push_back(f[0]);bei_surv.push_back(f[4]);bei_surv.push_back(f[8]);bei_surv.push_back(f[12]);
		bei_surv.push_back(f[1]);bei_surv.push_back(f[5]);bei_surv.push_back(f[9]);bei_surv.push_back(f[13]);
		bei_surv.push_back(f[2]);bei_surv.push_back(f[6]);bei_surv.push_back(f[10]);bei_surv.push_back(f[14]);
		bei_surv.push_back(f[3]);bei_surv.push_back(f[7]);bei_surv.push_back(f[11]);bei_surv.push_back(f[15]);*/
	}
}

void ApproximationCC::cal_nor_beisur(int)
{
	for (int j = 0; j < bei_surv_dat.size(); j++)
	{
		FaceHandle f = biaojiface[j];
		std::vector<VertexHandle> fv = mesh->faces_[f].vs_f;
		V3f tempn;
		for (int i = 0; i < fv.size(); ++i)
		{
			VertexHandle v = fv[i];
			tempn = tempn + vn[v];
		}
		tempn = tempn / 4;
		tempn = tempn.normalize();
		std::vector<V3f> temp_norm_sur;
		for (int i = 0; i < bei_surv_dat[j].size(); i++)
		{
			V3f a; V3f b; V3f n;
			a = bei_surv_dat[j][i][1] - bei_surv_dat[j][i][0];
			b = bei_surv_dat[j][i][3] - bei_surv_dat[j][i][0];
			n = a % b;
			V3f c = tempn - a;
			double temp = n.x*c.x + n.y*c.y + n.z*c.z;
			if (temp > 0)
				n = n * -1;
			n = n.normalize();
			temp_norm_sur.push_back(n);
		}
		nor_bei_surv_dat.push_back(temp_norm_sur);
	}	
}

void ApproximationCC::cal_nor_beisur1()
{
	for (int j = 0; j < bei_surv_dat.size(); j++)
	{
		FaceHandle f = biaojiface[j];
		//V3f tempn = mesh->normal_boundary_face_v[f];
		std::vector<V3f> temp_norm_sur;
		for (int i = 0; i < bei_surv_dat[j].size(); i++)
		{
			V3f a; V3f b; V3f n;
			a = bei_surv_dat[j][i][1] - bei_surv_dat[j][i][0];
			b = bei_surv_dat[j][i][3] - bei_surv_dat[j][i][0];
			n = a % b;
			V3f c = mesh->normal_boundary_face_v[j];
			double temp = n.x*c.x + n.y*c.y + n.z*c.z;
			if (temp > 0)
				n = n * -1;
			n = n.normalize();
			temp_norm_sur.push_back(n);
		}
		nor_bei_surv_dat.push_back(temp_norm_sur);
	}
}

void ApproximationCC::cal_nor_beisur1(int)
{
	for (int j = 0; j < bei_surv_dat.size(); j++)
	{
		std::vector<V3f> temp_norm_sur;
		for (int i = 0; i < bei_surv_dat[j].size(); i++)
		{
			V3f a; V3f b; V3f n;
			a = bei_surv_dat[j][i][1] - bei_surv_dat[j][i][0];
			b = bei_surv_dat[j][i][3] - bei_surv_dat[j][i][0];
			n = a % b;
			V3f c = mesh->vertices_[mesh->vertices_.size() - 1] - a;
			double temp = n.x*c.x + n.y*c.y + n.z*c.z;
			if (temp > 0)
				n = n * -1;
			n = n.normalize();
			temp_norm_sur.push_back(n);
		}
		nor_bei_surv_dat.push_back(temp_norm_sur);
	}
}

void ApproximationCC::cal_nor_beisur(double)
{
	for (int j = 0; j < bei_surv_dat.size(); j++)
	{
		std::vector<V3f> temp_norm_sur;
		for (int i = 0; i < bei_surv_dat[j].size(); i++)
		{
			V3f a; V3f b; V3f n;
			a = bei_surv_dat[j][i][1] - bei_surv_dat[j][i][0];
			b = bei_surv_dat[j][i][3] - bei_surv_dat[j][i][0];
			n = a % b;
			V3f c = mesh->normal_boundary_face_v[j];
			double temp = n.x*c.x + n.y*c.y + n.z*c.z;
			if (temp > 0)
				n = n * -1;
			n = n.normalize();
			temp_norm_sur.push_back(n);
		}
		nor_bei_surv_dat.push_back(temp_norm_sur);
	}
}

void ApproximationCC::constructBezier(int)
{
	for (std::vector<std::vector<std::vector<V3f>>>::iterator beiv_it = beiv.begin(); beiv_it != beiv.end(); beiv_it++)
	{
		std::vector<std::vector<V3f>> f_beiv = *beiv_it;
		Wm4::Vector3<double> **bv;
		bv = WM4_NEW Wm4::Vector3<double>*[4];
		bv[0] = WM4_NEW Wm4::Vector3<double>[16];
		for (int i = 1; i < 4; i++)
		{
			bv[i] = &bv[0][i * 4];
		}
		bv[0][0] = Wm4::Vector3<double>(f_beiv[0][0].x, f_beiv[0][0].y, f_beiv[0][0].z);
		bv[0][1] = Wm4::Vector3<double>(f_beiv[0][1].x, f_beiv[0][1].y, f_beiv[0][1].z);
		bv[0][2] = Wm4::Vector3<double>(f_beiv[0][2].x, f_beiv[0][2].y, f_beiv[0][2].z);
		bv[0][3] = Wm4::Vector3<double>(f_beiv[0][3].x, f_beiv[0][3].y, f_beiv[0][3].z);
		bv[1][0] = Wm4::Vector3<double>(f_beiv[1][0].x, f_beiv[1][0].y, f_beiv[1][0].z);
		bv[1][1] = Wm4::Vector3<double>(f_beiv[1][1].x, f_beiv[1][1].y, f_beiv[1][1].z);
		bv[1][2] = Wm4::Vector3<double>(f_beiv[1][2].x, f_beiv[1][2].y, f_beiv[1][2].z);
		bv[1][3] = Wm4::Vector3<double>(f_beiv[1][3].x, f_beiv[1][3].y, f_beiv[1][3].z);
		bv[2][0] = Wm4::Vector3<double>(f_beiv[2][0].x, f_beiv[2][0].y, f_beiv[2][0].z);
		bv[2][1] = Wm4::Vector3<double>(f_beiv[2][1].x, f_beiv[2][1].y, f_beiv[2][1].z);
		bv[2][2] = Wm4::Vector3<double>(f_beiv[2][2].x, f_beiv[2][2].y, f_beiv[2][2].z);
		bv[2][3] = Wm4::Vector3<double>(f_beiv[2][3].x, f_beiv[2][3].y, f_beiv[2][3].z);
		bv[3][0] = Wm4::Vector3<double>(f_beiv[3][0].x, f_beiv[3][0].y, f_beiv[3][0].z);
		bv[3][1] = Wm4::Vector3<double>(f_beiv[3][1].x, f_beiv[3][1].y, f_beiv[3][1].z);
		bv[3][2] = Wm4::Vector3<double>(f_beiv[3][2].x, f_beiv[3][2].y, f_beiv[3][2].z);
		bv[3][3] = Wm4::Vector3<double>(f_beiv[3][3].x, f_beiv[3][3].y, f_beiv[3][3].z);
		Wm4::BSplineRectangle<double> tempbv(4, 4, bv, 3, 3, false, false, true, true);
		int size = 20;//要生成size * size个面的控制网格 即(size +1) * (size + 1)个控制顶点
		std::vector<std::vector<V3f>> f(size * size);
		std::vector<V3f> bezierv((size + 1) * (size + 1));
		for (int i = 0; i < bezierv.size(); i++)
		{
			bezierv[i] = tempbv.P(i % (size + 1) * (1.0 / size), i / (size + 1) * (1.0 / size));
		}
		std::vector<std::vector<V3f>> tem_surv;
		for (int i = 0; i < f.size(); i++)
		{
			int j = i + i / size;
			f[i].push_back(bezierv[j]); f[i].push_back(bezierv[j + 1]);
			f[i].push_back(bezierv[j + size + 2]); f[i].push_back(bezierv[j + size + 1]);
			tem_surv.push_back(f[i]);
		}
		bei_surv_dat.push_back(tem_surv);

		std::vector<V3f> line1;
		for (int j = 0; j < size + 1; j++)//第1条边
		{
			line1.push_back(tempbv.P(0, j * (1.0 / size)));
		}
		std::vector<V3f> line2;
		for (int j = 0; j < size + 1; j++)//第2条边
		{
			line2.push_back(tempbv.P(1, j * (1.0 / size)));
		}
		std::vector<V3f> line3;
		for (int j = 0; j < size + 1; j++)//第3条边
		{
			line3.push_back(tempbv.P(j * (1.0 / size), 0));
		}
		std::vector<V3f> line4;
		for (int j = 0; j < size + 1; j++)//第4条边
		{
			line4.push_back(tempbv.P(j * (1.0 / size), 1));
		}
		edgeline.push_back(line1);
		edgeline.push_back(line2);
		edgeline.push_back(line3);
		edgeline.push_back(line4);
	}
}

void ApproximationCC::constructBezierSharp()
{
	int count = 0;
	for (std::vector<std::vector<std::vector<V3f>>>::iterator beiv_it = beiv.begin(); beiv_it != beiv.end(); beiv_it++)
	{
		FaceHandle face = biaojiface[count++];
		std::vector<std::vector<V3f>> f_beiv = *beiv_it;
		Wm4::Vector3<double> **bv;
		bv = WM4_NEW Wm4::Vector3<double>*[4];
		bv[0] = WM4_NEW Wm4::Vector3<double>[16];
		for (int i = 1; i < 4; i++)
		{
			bv[i] = &bv[0][i * 4];
		}
		bv[0][0] = Wm4::Vector3<double>(f_beiv[0][0].x, f_beiv[0][0].y, f_beiv[0][0].z);
		bv[0][1] = Wm4::Vector3<double>(f_beiv[0][1].x, f_beiv[0][1].y, f_beiv[0][1].z);
		bv[0][2] = Wm4::Vector3<double>(f_beiv[0][2].x, f_beiv[0][2].y, f_beiv[0][2].z);
		bv[0][3] = Wm4::Vector3<double>(f_beiv[0][3].x, f_beiv[0][3].y, f_beiv[0][3].z);
		bv[1][0] = Wm4::Vector3<double>(f_beiv[1][0].x, f_beiv[1][0].y, f_beiv[1][0].z);
		bv[1][1] = Wm4::Vector3<double>(f_beiv[1][1].x, f_beiv[1][1].y, f_beiv[1][1].z);
		bv[1][2] = Wm4::Vector3<double>(f_beiv[1][2].x, f_beiv[1][2].y, f_beiv[1][2].z);
		bv[1][3] = Wm4::Vector3<double>(f_beiv[1][3].x, f_beiv[1][3].y, f_beiv[1][3].z);
		bv[2][0] = Wm4::Vector3<double>(f_beiv[2][0].x, f_beiv[2][0].y, f_beiv[2][0].z);
		bv[2][1] = Wm4::Vector3<double>(f_beiv[2][1].x, f_beiv[2][1].y, f_beiv[2][1].z);
		bv[2][2] = Wm4::Vector3<double>(f_beiv[2][2].x, f_beiv[2][2].y, f_beiv[2][2].z);
		bv[2][3] = Wm4::Vector3<double>(f_beiv[2][3].x, f_beiv[2][3].y, f_beiv[2][3].z);
		bv[3][0] = Wm4::Vector3<double>(f_beiv[3][0].x, f_beiv[3][0].y, f_beiv[3][0].z);
		bv[3][1] = Wm4::Vector3<double>(f_beiv[3][1].x, f_beiv[3][1].y, f_beiv[3][1].z);
		bv[3][2] = Wm4::Vector3<double>(f_beiv[3][2].x, f_beiv[3][2].y, f_beiv[3][2].z);
		bv[3][3] = Wm4::Vector3<double>(f_beiv[3][3].x, f_beiv[3][3].y, f_beiv[3][3].z);
		Wm4::BSplineRectangle<double> tempbv(4, 4, bv, 3, 3, false, false, true, true);
		int size = 8;//要生成size * size个面的控制网格 即(size +1) * (size + 1)个控制顶点

		std::vector<V3f> line1;
		for (int j = 0; j < size + 1; j++)//第1条边
		{
			line1.push_back(tempbv.P(0, j * (1.0 / size)));
		}
		std::vector<V3f> line2;
		for (int j = 0; j < size + 1; j++)//第2条边
		{
			line2.push_back(tempbv.P(1, j * (1.0 / size)));
		}
		std::vector<V3f> line3;
		for (int j = 0; j < size + 1; j++)//第3条边
		{
			line3.push_back(tempbv.P(j * (1.0 / size), 0));
		}
		std::vector<V3f> line4;
		for (int j = 0; j < size + 1; j++)//第4条边
		{
			line4.push_back(tempbv.P(j * (1.0 / size), 1));
		}
		std::vector<std::vector<V3f>> line{ line1,line2,line3,line4 };
		std::vector<FaceHandle>::iterator findit = std::find(sharpface.begin(), sharpface.end(), face);
		int checklinnum = 0;
		bool check = false;
		if (findit != sharpface.end())
		{
			check = true;
			int curveid = sharpnum[findit - sharpface.begin()];
			std::vector<std::vector<V3f>> linetemp(4);
			std::vector<double> mintemp(4);
			for (int linenum = 0; linenum < 4; ++linenum)
			{
				double minn = 0;
				std::vector<V3f> linentemp(line[linenum].size());
				for (int i = 0; i < line[linenum].size(); ++i)
				{
					V3f v = line[linenum][i];
					double min = 999; int minid = 0;
					for (int j = 0; j < curve_v[curveid].size(); ++j)
					{
						double dis = distance(v, curve_v[curveid][j]);
						if (dis < min)
						{
							min = dis;
							minid = j;
						}
					}
					minn += min;
					linentemp[i] = curve_v[curveid][minid];
				}
				linetemp[linenum] = linentemp;
				mintemp[linenum] = minn;
			}
			std::vector<double>::iterator smallest = std::min_element(mintemp.begin(), mintemp.end());
			int smallline = smallest - mintemp.begin();
			line[smallline] = linetemp[smallline];
			checklinnum = smallline;
		}

		std::vector<std::vector<V3f>> f(size * size);
		std::vector<V3f> bezierv((size + 1) * (size + 1));
		for (int i = 0; i < bezierv.size(); i++)
		{
			bezierv[i] = tempbv.P(i % (size + 1) * (1.0 / size), i / (size + 1) * (1.0 / size));
		}
		if (check == true)
		{
			if (checklinnum == 0)
				for (int i = 0; i < size + 1; ++i)
					bezierv[i*(size + 1)] = line[checklinnum][i];
			else if (checklinnum == 1)
				for (int i = 0; i < size + 1; ++i)
					bezierv[size + i * (size + 1)] = line[checklinnum][i];
			else if (checklinnum == 2)
				for (int i = 0; i < size + 1; ++i)
					bezierv[i] = line[checklinnum][i];
			else 
				for (int i = 0; i < size + 1; ++i)
					bezierv[size*(size + 1) + i] = line[checklinnum][i];
		}

		std::vector<std::vector<V3f>> tem_surv;
		for (int i = 0; i < f.size(); i++)
		{
			int j = i + i / size;
			f[i].push_back(bezierv[j]); f[i].push_back(bezierv[j + 1]);
			f[i].push_back(bezierv[j + size + 2]); f[i].push_back(bezierv[j + size + 1]);
			tem_surv.push_back(f[i]);
		}
		bei_surv_dat.push_back(tem_surv);
		for (int i = 0; i < line.size(); ++i)
			edgeline.push_back(line[i]);
	}
}

void ApproximationCC::approximationingSharp()
{
	int face_num = mesh->faces_.size();
	for (int i = 0; i < face_num; i++)//以面为单位遍历
	{
		if (mesh->is_bdy(FaceHandle(i)))
		{
			biaojiface.push_back(FaceHandle(i));
			std::vector<V3f> temp;
			std::vector<EdgeHandle> fe = mesh->faces_[i].edges_;
			VertexHandle fev, feopv;
			V3f cornerv, interv, epv, epopv;
			//边排序
			/*第1条边*/
			EdgeHandle tempedge = fe[0];
			fev = mesh->edges_[fe[0]].from_h; feopv = mesh->edges_[fe[0]].to_h;
			cornerv = cal_corner_pointsharp(fev);
			interv = cal_inter_pointsharp(FaceHandle(i), fev);
			epv = cal_edge_pointsharp(fe[0], fev);
			epopv = cal_edge_pointsharp(fe[0], feopv);
			temp.push_back(cornerv);
			temp.push_back(interv);
			temp.push_back(epv);
			temp.push_back(epopv);
			std::vector<EdgeHandle> tempev;
			EdgeHandle tempe = fe[0];
			tempev.push_back(tempe);
			/*后3条边*/
			for (int laternum = 0; laternum < 3; laternum++)
			{
				for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end(); fe_it++)
				{
					if (find(tempev.begin(), tempev.end(), *fe_it) == tempev.end())
					{
						if (mesh->edges_[*fe_it].from_h == feopv)
						{
							tempe = *fe_it;
							tempev.push_back(tempe);
							fev = mesh->edges_[*fe_it].from_h;
							feopv = mesh->edges_[*fe_it].to_h;
							break;
						}
						else if (mesh->edges_[*fe_it].to_h == feopv)
						{
							tempe = *fe_it;
							tempev.push_back(tempe);
							fev = mesh->edges_[*fe_it].to_h;
							feopv = mesh->edges_[*fe_it].from_h;
							break;
						}
						else
							continue;
					}
				}
				cornerv = cal_corner_pointsharp(fev);
				interv = cal_inter_pointsharp(FaceHandle(i), fev);
				epv = cal_edge_pointsharp(tempe, fev);
				epopv = cal_edge_pointsharp(tempe, feopv);
				temp.push_back(cornerv);
				temp.push_back(interv);
				temp.push_back(epv);
				temp.push_back(epopv);
			}
			std::vector<V3f> f1, f2, f3, f4, f5, f6, f7, f8, f9;
			f1.push_back(temp[0]); f1.push_back(temp[2]); f1.push_back(temp[1]); f1.push_back(temp[15]);
			f2.push_back(temp[2]); f2.push_back(temp[3]); f2.push_back(temp[5]); f2.push_back(temp[1]);
			f3.push_back(temp[3]); f3.push_back(temp[4]); f3.push_back(temp[6]); f3.push_back(temp[5]);
			f4.push_back(temp[15]); f4.push_back(temp[1]); f4.push_back(temp[13]); f4.push_back(temp[14]);
			f5.push_back(temp[1]); f5.push_back(temp[5]); f5.push_back(temp[9]); f5.push_back(temp[13]);
			f6.push_back(temp[5]); f6.push_back(temp[6]); f6.push_back(temp[7]); f6.push_back(temp[9]);
			f7.push_back(temp[14]); f7.push_back(temp[13]); f7.push_back(temp[11]); f7.push_back(temp[12]);
			f8.push_back(temp[13]); f8.push_back(temp[9]); f8.push_back(temp[10]); f8.push_back(temp[11]);
			f9.push_back(temp[9]); f9.push_back(temp[7]); f9.push_back(temp[8]); f9.push_back(temp[10]);
			surv.push_back(f1); surv.push_back(f2); surv.push_back(f3); surv.push_back(f4); surv.push_back(f5);
			surv.push_back(f6); surv.push_back(f7); surv.push_back(f8); surv.push_back(f9);
			std::vector<V3f> beiv1, beiv2, beiv3, beiv4;
			std::vector<std::vector<V3f>> beif;
			beiv1.push_back(temp[0]); beiv1.push_back(temp[15]); beiv1.push_back(temp[14]); beiv1.push_back(temp[12]);
			beiv2.push_back(temp[2]); beiv2.push_back(temp[1]); beiv2.push_back(temp[13]); beiv2.push_back(temp[11]);
			beiv3.push_back(temp[3]); beiv3.push_back(temp[5]); beiv3.push_back(temp[9]); beiv3.push_back(temp[10]);
			beiv4.push_back(temp[4]); beiv4.push_back(temp[6]); beiv4.push_back(temp[7]); beiv4.push_back(temp[8]);
			beif.push_back(beiv1); beif.push_back(beiv2); beif.push_back(beiv3); beif.push_back(beiv4);
			beiv.push_back(beif);
		}
	}
}

void ApproximationCC::cal_beisurv_center(int)
{
	bei_surv_center.reserve(bei_surv_dat.size());
	for (int i = 0; i < bei_surv_dat.size(); ++i)
	{
		V3f center(0, 0, 0);
		std::vector<std::vector<V3f>> temp_beisurv = bei_surv_dat[i];
		for (int j = 0; j < temp_beisurv.size(); ++j)
		{
			V3f center1(0, 0, 0);
			std::vector<V3f> temp_temp = temp_beisurv[j];
			for (int k = 0; k < temp_temp.size(); ++k)
			{
				center1 = center1 + temp_temp[k];
			}
			center1 = center1 / temp_temp.size();
			center = center + center1;
		}
		center = center / temp_beisurv.size();
		bei_surv_center.push_back(center);
	}
}

V3f ApproximationCC::solid_edge_point(EdgeHandle _e, VertexHandle _v)
{
	if (mesh->is_bdy(_e))
	{
		/*V3f edp;
		std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
		int n = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
		{
			if (mesh->is_bdy(*ef_it))
			{
				edp = edp + cal_inter_point(*ef_it, _v);
				n++;
			}
		}
		edp = edp / n;
		return edp;*/
		return cal_edge_pointsharp(_e, _v);
	}
	else
	{
		V3f edp;
		std::set<CellHandle> ec = mesh->neighbor_e_c(_e);
		int n = ec.size();
		for (std::set<CellHandle>::iterator ec_it = ec.begin(); ec_it != ec.end(); ec_it++)
		{
			edp = edp + cal_inter_point(*ec_it, _v);
		}
		edp = edp / n;
		return edp;
	}
}

V3f ApproximationCC::solid_edge_point_1(EdgeHandle _e, VertexHandle _v, CellHandle _c)
{
	if (mesh->is_bdy(_e))
	{
		std::set<FaceHandle, compare_OVM> e_f_temp = mesh->neighbor_e[_e];
		int ef_temp = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f_temp.begin(); e_f_it != e_f_temp.end(); e_f_it++)
		{
			if (mesh->is_bdy(*e_f_it))
				ef_temp++;
		}
		if (ef_temp == 4)
		{
			V3f edp;
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
			int n = 0;
			for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
			{
				if (mesh->is_bdy(*ef_it) && mesh->is_f_in_c(*ef_it, _c))
				{
					edp = edp + cal_inter_point(*ef_it, _v);
					n++;
				}
			}
			edp = edp / n;
			return edp;
		}
		else
		{
			V3f edp;
			std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
			int n = 0;
			for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
			{
				if (mesh->is_bdy(*ef_it))
				{
					edp = edp + cal_inter_point(*ef_it, _v);
					n++;
				}
			}
			edp = edp / n;
			return edp;
		}
	}
	else
	{
		V3f edp;
		std::set<CellHandle> ec = mesh->neighbor_e_c(_e);
		int n = ec.size();
		for (std::set<CellHandle>::iterator ec_it = ec.begin(); ec_it != ec.end(); ec_it++)
		{
			edp = edp + cal_inter_point(*ec_it, _v);
		}
		edp = edp / n;
		return edp;
	}
}

V3f ApproximationCC::solid_face_point(FaceHandle _f, VertexHandle _v)
{
	if (mesh->is_bdy(_f))
	{
		/*V3f fp;
		fp = cal_inter_point(_f, _v);
		return fp;*/
		return cal_inter_pointsharp(_f, _v);
	}
	else
	{
		V3f fp;
		std::set<CellHandle, compare_OVM> fc = mesh->neighbor_f[_f];
		for (std::set<CellHandle, compare_OVM>::iterator fc_it = fc.begin(); fc_it != fc.end(); fc_it++)
		{
			fp = fp + cal_inter_point(*fc_it, _v);
		}
		fp = fp / 2;
		return fp;
	}
}

V3f ApproximationCC::solid_corner_point(VertexHandle _v)
{
	if (mesh->is_bdy(_v))
	{
		/*V3f cp;
		std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
		int n = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
		{
			if (mesh->is_bdy(*vf_it))
			{
				cp = cp + cal_inter_point(*vf_it, _v);
				n++;
			}
		}
		cp = cp / n;
		return cp;*/
		return cal_corner_pointsharp(_v);
	}
	else
	{
		V3f cp;
		std::set<CellHandle, compare_OVM> vc = mesh->neighbor_v_c[_v];
		int n = vc.size();
		for (std::set<CellHandle, compare_OVM>::iterator vc_it = vc.begin(); vc_it != vc.end(); vc_it++)
		{
			cp = cp + cal_inter_point(*vc_it, _v);
		}
		cp = cp / n;
		return cp;
	}
}

V3f ApproximationCC::solid_corner_point_1(VertexHandle _v, CellHandle _c)
{
	if (mesh->is_bdy(_v))
	{
		std::set<EdgeHandle, compare_OVM> v_e_temp = mesh->neighbor_v[_v];
		int bdy_edge = 0;
		int not_bdy_edge = 0;
		for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e_temp.begin(); v_e_it != v_e_temp.end(); v_e_it++)
		{
			if (mesh->is_bdy(*v_e_it))
				bdy_edge++;
			else
				not_bdy_edge++;
		}
		std::set<CellHandle> c_c_temp = mesh->neighbor_c_c(_c);
		c_c_temp.insert(_c);
		if (bdy_edge > 5 || not_bdy_edge < 1)
		{
			V3f cp;
			std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
			int n = 0;
			for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
			{
				if (mesh->is_bdy(*vf_it))
				{
					for (std::set<CellHandle>::iterator c_c_temp_it = c_c_temp.begin(); c_c_temp_it != c_c_temp.end(); c_c_temp_it++)
					{
						if (mesh->is_f_in_c(*vf_it, *c_c_temp_it))
						{
							cp = cp + cal_inter_point(*vf_it, _v);
							n++;
							break;
						}
					}
				}
			}
			cp = cp / n;
			return cp;
		}
		else
		{
			V3f cp;
			std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
			int n = 0;
			for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
			{
				if (mesh->is_bdy(*vf_it))
				{
					cp = cp + cal_inter_point(*vf_it, _v);
					n++;
				}
			}
			cp = cp / n;
			return cp;
		}
	}
	else
	{
		V3f cp;
		std::set<CellHandle, compare_OVM> vc = mesh->neighbor_v_c[_v];
		int n = vc.size();
		for (std::set<CellHandle, compare_OVM>::iterator vc_it = vc.begin(); vc_it != vc.end(); vc_it++)
		{
			cp = cp + cal_inter_point(*vc_it, _v);
		}
		cp = cp / n;
		return cp;
	}
}

V3f ApproximationCC::solid_edge_point(int cell_num, int edge_num, int vertex_num)
{
	return solid_edge_point(mesh->cells_[cell_num].edges_[edge_num], mesh->cells_[cell_num].vertex_[vertex_num]);
}

V3f ApproximationCC::solid_edge_point_1(int cell_num, int edge_num, int vertex_num)
{
	return solid_edge_point_1(mesh->cells_[cell_num].edges_[edge_num], mesh->cells_[cell_num].vertex_[vertex_num], CellHandle(cell_num));
}

V3f ApproximationCC::solid_face_point(int cell_num, int face_num, int vertex_num)
{
	return solid_face_point(mesh->cells_[cell_num].faces_[face_num], mesh->cells_[cell_num].vertex_[vertex_num]);
}

HexV3fMesh& ApproximationCC::CCapproximation()
{
	uint64_t cell_num = mesh->cells_.size();
	bss.init(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(solid_corner_point(mesh->cells_[i].vertex_[vn]));
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(cal_inter_point(CellHandle(i), mesh->cells_[i].vertex_[vn]));
		}
		//第一层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 9, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 9, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 1, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 7, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 1, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 7, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 8, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 8, 2));
		//第二层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 6, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 4, 2));
		//第三层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 2, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 6, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 4, 3));
		//第4层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 11, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 11, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 5, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 5, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 10, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 10, 3));

		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(27);
		/*第一层*/
		//第1个单元
		sub_cell[0].push_back(temp[5]); sub_cell[0].push_back(temp[16]); sub_cell[0].push_back(temp[19]); sub_cell[0].push_back(temp[18]);
		sub_cell[0].push_back(temp[28]); sub_cell[0].push_back(temp[29]); sub_cell[0].push_back(temp[13]); sub_cell[0].push_back(temp[32]);
		//第2个单元
		sub_cell[1].push_back(temp[16]); sub_cell[1].push_back(temp[17]); sub_cell[1].push_back(temp[20]); sub_cell[1].push_back(temp[19]);
		sub_cell[1].push_back(temp[29]); sub_cell[1].push_back(temp[30]); sub_cell[1].push_back(temp[14]); sub_cell[1].push_back(temp[13]);
		//第3个单元
		sub_cell[2].push_back(temp[17]); sub_cell[2].push_back(temp[6]); sub_cell[2].push_back(temp[21]); sub_cell[2].push_back(temp[20]);
		sub_cell[2].push_back(temp[30]); sub_cell[2].push_back(temp[31]); sub_cell[2].push_back(temp[33]); sub_cell[2].push_back(temp[14]);
		//第4个单元
		sub_cell[3].push_back(temp[18]); sub_cell[3].push_back(temp[19]); sub_cell[3].push_back(temp[23]); sub_cell[3].push_back(temp[22]);
		sub_cell[3].push_back(temp[32]); sub_cell[3].push_back(temp[13]); sub_cell[3].push_back(temp[9]); sub_cell[3].push_back(temp[34]);
		//第5个单元
		sub_cell[4].push_back(temp[19]); sub_cell[4].push_back(temp[20]); sub_cell[4].push_back(temp[24]); sub_cell[4].push_back(temp[23]);
		sub_cell[4].push_back(temp[13]); sub_cell[4].push_back(temp[14]); sub_cell[4].push_back(temp[10]); sub_cell[4].push_back(temp[9]);
		//第6个单元
		sub_cell[5].push_back(temp[20]); sub_cell[5].push_back(temp[21]); sub_cell[5].push_back(temp[25]); sub_cell[5].push_back(temp[24]);
		sub_cell[5].push_back(temp[14]); sub_cell[5].push_back(temp[33]); sub_cell[5].push_back(temp[35]); sub_cell[5].push_back(temp[10]);
		//第7个单元
		sub_cell[6].push_back(temp[22]); sub_cell[6].push_back(temp[23]); sub_cell[6].push_back(temp[26]); sub_cell[6].push_back(temp[1]);
		sub_cell[6].push_back(temp[34]); sub_cell[6].push_back(temp[9]); sub_cell[6].push_back(temp[37]); sub_cell[6].push_back(temp[36]);
		//第8个单元
		sub_cell[7].push_back(temp[23]); sub_cell[7].push_back(temp[24]); sub_cell[7].push_back(temp[27]); sub_cell[7].push_back(temp[26]);
		sub_cell[7].push_back(temp[9]); sub_cell[7].push_back(temp[10]); sub_cell[7].push_back(temp[38]); sub_cell[7].push_back(temp[37]);
		//第9个单元
		sub_cell[8].push_back(temp[24]); sub_cell[8].push_back(temp[25]); sub_cell[8].push_back(temp[2]); sub_cell[8].push_back(temp[27]);
		sub_cell[8].push_back(temp[10]); sub_cell[8].push_back(temp[35]); sub_cell[8].push_back(temp[39]); sub_cell[8].push_back(temp[38]);
		/*第二层*/
		//第10个单元
		sub_cell[9].push_back(temp[28]); sub_cell[9].push_back(temp[29]); sub_cell[9].push_back(temp[13]); sub_cell[9].push_back(temp[32]);
		sub_cell[9].push_back(temp[40]); sub_cell[9].push_back(temp[41]); sub_cell[9].push_back(temp[12]); sub_cell[9].push_back(temp[44]);
		//第11个单元
		sub_cell[10].push_back(temp[29]); sub_cell[10].push_back(temp[30]); sub_cell[10].push_back(temp[14]); sub_cell[10].push_back(temp[13]);
		sub_cell[10].push_back(temp[41]); sub_cell[10].push_back(temp[42]); sub_cell[10].push_back(temp[15]); sub_cell[10].push_back(temp[12]);
		//第12个单元
		sub_cell[11].push_back(temp[30]); sub_cell[11].push_back(temp[31]); sub_cell[11].push_back(temp[33]); sub_cell[11].push_back(temp[14]);
		sub_cell[11].push_back(temp[42]); sub_cell[11].push_back(temp[43]); sub_cell[11].push_back(temp[45]); sub_cell[11].push_back(temp[15]);
		//第13个单元
		sub_cell[12].push_back(temp[32]); sub_cell[12].push_back(temp[13]); sub_cell[12].push_back(temp[9]); sub_cell[12].push_back(temp[34]);
		sub_cell[12].push_back(temp[44]); sub_cell[12].push_back(temp[12]); sub_cell[12].push_back(temp[8]); sub_cell[12].push_back(temp[46]);
		//第14个单元
		sub_cell[13].push_back(temp[13]); sub_cell[13].push_back(temp[14]); sub_cell[13].push_back(temp[10]); sub_cell[13].push_back(temp[9]);
		sub_cell[13].push_back(temp[12]); sub_cell[13].push_back(temp[15]); sub_cell[13].push_back(temp[11]); sub_cell[13].push_back(temp[8]);
		//第15个单元
		sub_cell[14].push_back(temp[14]); sub_cell[14].push_back(temp[33]); sub_cell[14].push_back(temp[35]); sub_cell[14].push_back(temp[10]);
		sub_cell[14].push_back(temp[15]); sub_cell[14].push_back(temp[45]); sub_cell[14].push_back(temp[47]); sub_cell[14].push_back(temp[11]);
		//第16个单元
		sub_cell[15].push_back(temp[34]); sub_cell[15].push_back(temp[9]); sub_cell[15].push_back(temp[37]); sub_cell[15].push_back(temp[36]);
		sub_cell[15].push_back(temp[46]); sub_cell[15].push_back(temp[8]); sub_cell[15].push_back(temp[49]); sub_cell[15].push_back(temp[48]);
		//第17个单元
		sub_cell[16].push_back(temp[9]); sub_cell[16].push_back(temp[10]); sub_cell[16].push_back(temp[38]); sub_cell[16].push_back(temp[37]);
		sub_cell[16].push_back(temp[8]); sub_cell[16].push_back(temp[11]); sub_cell[16].push_back(temp[50]); sub_cell[16].push_back(temp[49]);
		//第18个单元
		sub_cell[17].push_back(temp[10]); sub_cell[17].push_back(temp[35]); sub_cell[17].push_back(temp[39]); sub_cell[17].push_back(temp[38]);
		sub_cell[17].push_back(temp[11]); sub_cell[17].push_back(temp[47]); sub_cell[17].push_back(temp[51]); sub_cell[17].push_back(temp[50]);
		/*第三层*/
		//第19个单元
		sub_cell[18].push_back(temp[40]); sub_cell[18].push_back(temp[41]); sub_cell[18].push_back(temp[12]); sub_cell[18].push_back(temp[44]);
		sub_cell[18].push_back(temp[4]); sub_cell[18].push_back(temp[52]); sub_cell[18].push_back(temp[55]); sub_cell[18].push_back(temp[54]);
		//第20个单元
		sub_cell[19].push_back(temp[41]); sub_cell[19].push_back(temp[42]); sub_cell[19].push_back(temp[15]); sub_cell[19].push_back(temp[12]);
		sub_cell[19].push_back(temp[52]); sub_cell[19].push_back(temp[53]); sub_cell[19].push_back(temp[56]); sub_cell[19].push_back(temp[55]);
		//第21个单元
		sub_cell[20].push_back(temp[42]); sub_cell[20].push_back(temp[43]); sub_cell[20].push_back(temp[45]); sub_cell[20].push_back(temp[15]);
		sub_cell[20].push_back(temp[53]); sub_cell[20].push_back(temp[7]); sub_cell[20].push_back(temp[57]); sub_cell[20].push_back(temp[56]);
		//第22个单元
		sub_cell[21].push_back(temp[44]); sub_cell[21].push_back(temp[12]); sub_cell[21].push_back(temp[8]); sub_cell[21].push_back(temp[46]);
		sub_cell[21].push_back(temp[54]); sub_cell[21].push_back(temp[55]); sub_cell[21].push_back(temp[59]); sub_cell[21].push_back(temp[58]);
		//第23个单元
		sub_cell[22].push_back(temp[12]); sub_cell[22].push_back(temp[15]); sub_cell[22].push_back(temp[11]); sub_cell[22].push_back(temp[8]);
		sub_cell[22].push_back(temp[55]); sub_cell[22].push_back(temp[56]); sub_cell[22].push_back(temp[60]); sub_cell[22].push_back(temp[59]);
		//第24个单元
		sub_cell[23].push_back(temp[15]); sub_cell[23].push_back(temp[45]); sub_cell[23].push_back(temp[47]); sub_cell[23].push_back(temp[11]);
		sub_cell[23].push_back(temp[56]); sub_cell[23].push_back(temp[57]); sub_cell[23].push_back(temp[61]); sub_cell[23].push_back(temp[60]);
		//第25个单元
		sub_cell[24].push_back(temp[46]); sub_cell[24].push_back(temp[8]); sub_cell[24].push_back(temp[49]); sub_cell[24].push_back(temp[48]);
		sub_cell[24].push_back(temp[58]); sub_cell[24].push_back(temp[59]); sub_cell[24].push_back(temp[62]); sub_cell[24].push_back(temp[0]);
		//第26个单元
		sub_cell[25].push_back(temp[8]); sub_cell[25].push_back(temp[11]); sub_cell[25].push_back(temp[50]); sub_cell[25].push_back(temp[49]);
		sub_cell[25].push_back(temp[59]); sub_cell[25].push_back(temp[60]); sub_cell[25].push_back(temp[63]); sub_cell[25].push_back(temp[62]);
		//第27个单元
		sub_cell[26].push_back(temp[11]); sub_cell[26].push_back(temp[47]); sub_cell[26].push_back(temp[51]); sub_cell[26].push_back(temp[50]);
		sub_cell[26].push_back(temp[60]); sub_cell[26].push_back(temp[61]); sub_cell[26].push_back(temp[3]); sub_cell[26].push_back(temp[63]);

		//插入单元
		for (int j = 0; j < 27; j++)
		{
			newmesh->add_cell(sub_cell[j]);
		}
	}
	return *newmesh;
}

HexV3fMesh& ApproximationCC::CCapproximation1()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(solid_corner_point(mesh->cells_[i].vertex_[vn]));
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(cal_inter_point(CellHandle(i), mesh->cells_[i].vertex_[vn]));
		}
		//第一层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 9, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 9, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 1, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 7, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 1, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 7, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 8, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 8, 2));
		//第二层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 6, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 4, 2));
		//第三层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 2, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 6, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 4, 3));
		//第4层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 11, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 11, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 5, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 5, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 10, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point(i, 10, 3));

		//构建Bezier体的数据
		std::vector<std::vector<std::vector<V3f>>> sc(4);
		std::vector<std::vector<V3f>> sf(4);
		std::vector<V3f> sl(4);
		//第一层
		sl[0] = newmesh->vertices_[temp[5]]; sl[1] = newmesh->vertices_[temp[18]];
		sl[2] = newmesh->vertices_[temp[22]]; sl[3] = newmesh->vertices_[temp[1]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[16]]; sl[1] = newmesh->vertices_[temp[19]];
		sl[2] = newmesh->vertices_[temp[23]]; sl[3] = newmesh->vertices_[temp[26]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[17]]; sl[1] = newmesh->vertices_[temp[20]];
		sl[2] = newmesh->vertices_[temp[24]]; sl[3] = newmesh->vertices_[temp[27]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[6]]; sl[1] = newmesh->vertices_[temp[21]];
		sl[2] = newmesh->vertices_[temp[25]]; sl[3] = newmesh->vertices_[temp[2]];
		sf[3] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = newmesh->vertices_[temp[28]]; sl[1] = newmesh->vertices_[temp[32]];
		sl[2] = newmesh->vertices_[temp[34]]; sl[3] = newmesh->vertices_[temp[36]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[29]]; sl[1] = newmesh->vertices_[temp[13]];
		sl[2] = newmesh->vertices_[temp[9]]; sl[3] = newmesh->vertices_[temp[37]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[30]]; sl[1] = newmesh->vertices_[temp[14]];
		sl[2] = newmesh->vertices_[temp[10]]; sl[3] = newmesh->vertices_[temp[38]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[31]]; sl[1] = newmesh->vertices_[temp[33]];
		sl[2] = newmesh->vertices_[temp[35]]; sl[3] = newmesh->vertices_[temp[39]];
		sf[3] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = newmesh->vertices_[temp[40]]; sl[1] = newmesh->vertices_[temp[44]];
		sl[2] = newmesh->vertices_[temp[46]]; sl[3] = newmesh->vertices_[temp[48]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[41]]; sl[1] = newmesh->vertices_[temp[12]];
		sl[2] = newmesh->vertices_[temp[8]]; sl[3] = newmesh->vertices_[temp[49]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[42]]; sl[1] = newmesh->vertices_[temp[15]];
		sl[2] = newmesh->vertices_[temp[11]]; sl[3] = newmesh->vertices_[temp[50]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[43]]; sl[1] = newmesh->vertices_[temp[45]];
		sl[2] = newmesh->vertices_[temp[47]]; sl[3] = newmesh->vertices_[temp[51]];
		sf[3] = sl;
		sc[2] = sf;
		//第四层
		sl[0] = newmesh->vertices_[temp[4]]; sl[1] = newmesh->vertices_[temp[54]];
		sl[2] = newmesh->vertices_[temp[58]]; sl[3] = newmesh->vertices_[temp[0]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[52]]; sl[1] = newmesh->vertices_[temp[55]];
		sl[2] = newmesh->vertices_[temp[59]]; sl[3] = newmesh->vertices_[temp[62]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[53]]; sl[1] = newmesh->vertices_[temp[56]];
		sl[2] = newmesh->vertices_[temp[60]]; sl[3] = newmesh->vertices_[temp[63]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[7]]; sl[1] = newmesh->vertices_[temp[57]];
		sl[2] = newmesh->vertices_[temp[61]]; sl[3] = newmesh->vertices_[temp[3]];
		sf[3] = sl;
		sc[3] = sf;

		SolidData.push_back(sc);
	}
	/*FileHandler filenew = FileHandler();
	std::string name("E://bezier.axl");
	filenew.write2AXLFile(name, SolidData);*/
	return *newmesh;
}

HexV3fMesh& ApproximationCC::CCapproximation_1()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(solid_corner_point_1(mesh->cells_[i].vertex_[vn], CellHandle(i)));
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(cal_inter_point(CellHandle(i), mesh->cells_[i].vertex_[vn]));
		}
		//第一层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 9, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 9, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 1, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 7, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 1, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 2, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 7, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 8, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 8, 2));
		//第二层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 6, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 4, 2));
		//第三层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 2, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 4, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 6, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 1, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 5, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 4, 3));
		//第4层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 11, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 11, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 5, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point(i, 3, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 5, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 10, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point_1(i, 10, 3));

		//构建Bezier体的数据
		std::vector<std::vector<std::vector<V3f>>> sc(4);
		std::vector<std::vector<V3f>> sf(4);
		std::vector<V3f> sl(4);
		//第一层
		sl[0] = newmesh->vertices_[temp[5]]; sl[1] = newmesh->vertices_[temp[18]];
		sl[2] = newmesh->vertices_[temp[22]]; sl[3] = newmesh->vertices_[temp[1]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[16]]; sl[1] = newmesh->vertices_[temp[19]];
		sl[2] = newmesh->vertices_[temp[23]]; sl[3] = newmesh->vertices_[temp[26]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[17]]; sl[1] = newmesh->vertices_[temp[20]];
		sl[2] = newmesh->vertices_[temp[24]]; sl[3] = newmesh->vertices_[temp[27]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[6]]; sl[1] = newmesh->vertices_[temp[21]];
		sl[2] = newmesh->vertices_[temp[25]]; sl[3] = newmesh->vertices_[temp[2]];
		sf[3] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = newmesh->vertices_[temp[28]]; sl[1] = newmesh->vertices_[temp[32]];
		sl[2] = newmesh->vertices_[temp[34]]; sl[3] = newmesh->vertices_[temp[36]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[29]]; sl[1] = newmesh->vertices_[temp[13]];
		sl[2] = newmesh->vertices_[temp[9]]; sl[3] = newmesh->vertices_[temp[37]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[30]]; sl[1] = newmesh->vertices_[temp[14]];
		sl[2] = newmesh->vertices_[temp[10]]; sl[3] = newmesh->vertices_[temp[38]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[31]]; sl[1] = newmesh->vertices_[temp[33]];
		sl[2] = newmesh->vertices_[temp[35]]; sl[3] = newmesh->vertices_[temp[39]];
		sf[3] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = newmesh->vertices_[temp[40]]; sl[1] = newmesh->vertices_[temp[44]];
		sl[2] = newmesh->vertices_[temp[46]]; sl[3] = newmesh->vertices_[temp[48]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[41]]; sl[1] = newmesh->vertices_[temp[12]];
		sl[2] = newmesh->vertices_[temp[8]]; sl[3] = newmesh->vertices_[temp[49]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[42]]; sl[1] = newmesh->vertices_[temp[15]];
		sl[2] = newmesh->vertices_[temp[11]]; sl[3] = newmesh->vertices_[temp[50]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[43]]; sl[1] = newmesh->vertices_[temp[45]];
		sl[2] = newmesh->vertices_[temp[47]]; sl[3] = newmesh->vertices_[temp[51]];
		sf[3] = sl;
		sc[2] = sf;
		//第四层
		sl[0] = newmesh->vertices_[temp[4]]; sl[1] = newmesh->vertices_[temp[54]];
		sl[2] = newmesh->vertices_[temp[58]]; sl[3] = newmesh->vertices_[temp[0]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[52]]; sl[1] = newmesh->vertices_[temp[55]];
		sl[2] = newmesh->vertices_[temp[59]]; sl[3] = newmesh->vertices_[temp[62]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[53]]; sl[1] = newmesh->vertices_[temp[56]];
		sl[2] = newmesh->vertices_[temp[60]]; sl[3] = newmesh->vertices_[temp[63]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[7]]; sl[1] = newmesh->vertices_[temp[57]];
		sl[2] = newmesh->vertices_[temp[61]]; sl[3] = newmesh->vertices_[temp[3]];
		sf[3] = sl;
		sc[3] = sf;

		SolidData.push_back(sc);
	}
	/*FileHandler filenew = FileHandler();
	std::string name("E://bezier.axl");
	filenew.write2AXLFile(name, SolidData);*/
	return *newmesh;
}

HexV3fMesh ApproximationCC::constructBezierSolid()
{
	HexV3fMesh beziermesh;
	for (std::vector<std::vector<std::vector<std::vector<V3f>>>>::iterator SolidData_it = SolidData.begin();
		SolidData_it != SolidData.end(); SolidData_it++)
	{
		std::vector<std::vector<std::vector<V3f>>> bsdata = *SolidData_it;
		BezierSolid BS(bsdata);
		int size = 2;//要生成size*size*size个体
		std::vector<std::vector<V3f>> bezierv(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1)*(size + 1); j++)
			{
				bezierv[i].push_back(BS.P(j % (size + 1) * (1.0 / size), j / (size + 1) * (1.0 / size), i % (size + 1) * (1.0 / size)));
			}
		}
		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size*size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t])); ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2])); ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t])); ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2])); ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				beziermesh.add_cell(ctemp);
			}
		}
	}
	/*int bdyfn = 0;
	for (int i = 0;i < beziermesh.faces_.size();i++)
	{
		if (beziermesh.is_bdy(FaceHandle(i)))
			bdyfn++;
	}*/
	return beziermesh;
}

std::vector<HexV3fMesh>& ApproximationCC::CCapproximationDAT()
{
	uint64_t cell_num = mesh->cells_.size();
	vmesh.resize(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = vmesh[i].add_vertex(solid_corner_point(mesh->cells_[i].vertex_[vn]));
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = vmesh[i].add_vertex(cal_inter_point(CellHandle(i), mesh->cells_[i].vertex_[vn]));
		}
		//第一层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 9, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 9, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 1, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 2, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 2, 6));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 7, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 1, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 2, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 2, 2));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 7, 2));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 8, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 8, 2));
		//第二层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 2, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 4, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 4, 6));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 6, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 0, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 1, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 0, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 1, 2));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 0, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 5, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 5, 2));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 4, 2));
		//第三层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 2, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 4, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 4, 7));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 6, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 0, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 1, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 0, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 1, 3));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 0, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 5, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 5, 3));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 4, 3));
		//第4层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 11, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 11, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 3, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 3, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 3, 7));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 5, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 3, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 3, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point(i, 3, 3));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 5, 3));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 10, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point(i, 10, 3));
		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(27);
		/*第一层*/
		//第1个单元
		sub_cell[0].push_back(temp[5]); sub_cell[0].push_back(temp[16]); sub_cell[0].push_back(temp[19]); sub_cell[0].push_back(temp[18]);
		sub_cell[0].push_back(temp[28]); sub_cell[0].push_back(temp[29]); sub_cell[0].push_back(temp[13]); sub_cell[0].push_back(temp[32]);
		//第2个单元
		sub_cell[1].push_back(temp[16]); sub_cell[1].push_back(temp[17]); sub_cell[1].push_back(temp[20]); sub_cell[1].push_back(temp[19]);
		sub_cell[1].push_back(temp[29]); sub_cell[1].push_back(temp[30]); sub_cell[1].push_back(temp[14]); sub_cell[1].push_back(temp[13]);
		//第3个单元
		sub_cell[2].push_back(temp[17]); sub_cell[2].push_back(temp[6]); sub_cell[2].push_back(temp[21]); sub_cell[2].push_back(temp[20]);
		sub_cell[2].push_back(temp[30]); sub_cell[2].push_back(temp[31]); sub_cell[2].push_back(temp[33]); sub_cell[2].push_back(temp[14]);
		//第4个单元
		sub_cell[3].push_back(temp[18]); sub_cell[3].push_back(temp[19]); sub_cell[3].push_back(temp[23]); sub_cell[3].push_back(temp[22]);
		sub_cell[3].push_back(temp[32]); sub_cell[3].push_back(temp[13]); sub_cell[3].push_back(temp[9]); sub_cell[3].push_back(temp[34]);
		//第5个单元
		sub_cell[4].push_back(temp[19]); sub_cell[4].push_back(temp[20]); sub_cell[4].push_back(temp[24]); sub_cell[4].push_back(temp[23]);
		sub_cell[4].push_back(temp[13]); sub_cell[4].push_back(temp[14]); sub_cell[4].push_back(temp[10]); sub_cell[4].push_back(temp[9]);
		//第6个单元
		sub_cell[5].push_back(temp[20]); sub_cell[5].push_back(temp[21]); sub_cell[5].push_back(temp[25]); sub_cell[5].push_back(temp[24]);
		sub_cell[5].push_back(temp[14]); sub_cell[5].push_back(temp[33]); sub_cell[5].push_back(temp[35]); sub_cell[5].push_back(temp[10]);
		//第7个单元
		sub_cell[6].push_back(temp[22]); sub_cell[6].push_back(temp[23]); sub_cell[6].push_back(temp[26]); sub_cell[6].push_back(temp[1]);
		sub_cell[6].push_back(temp[34]); sub_cell[6].push_back(temp[9]); sub_cell[6].push_back(temp[37]); sub_cell[6].push_back(temp[36]);
		//第8个单元
		sub_cell[7].push_back(temp[23]); sub_cell[7].push_back(temp[24]); sub_cell[7].push_back(temp[27]); sub_cell[7].push_back(temp[26]);
		sub_cell[7].push_back(temp[9]); sub_cell[7].push_back(temp[10]); sub_cell[7].push_back(temp[38]); sub_cell[7].push_back(temp[37]);
		//第9个单元
		sub_cell[8].push_back(temp[24]); sub_cell[8].push_back(temp[25]); sub_cell[8].push_back(temp[2]); sub_cell[8].push_back(temp[27]);
		sub_cell[8].push_back(temp[10]); sub_cell[8].push_back(temp[35]); sub_cell[8].push_back(temp[39]); sub_cell[8].push_back(temp[38]);
		/*第二层*/
		//第10个单元
		sub_cell[9].push_back(temp[28]); sub_cell[9].push_back(temp[29]); sub_cell[9].push_back(temp[13]); sub_cell[9].push_back(temp[32]);
		sub_cell[9].push_back(temp[40]); sub_cell[9].push_back(temp[41]); sub_cell[9].push_back(temp[12]); sub_cell[9].push_back(temp[44]);
		//第11个单元
		sub_cell[10].push_back(temp[29]); sub_cell[10].push_back(temp[30]); sub_cell[10].push_back(temp[14]); sub_cell[10].push_back(temp[13]);
		sub_cell[10].push_back(temp[41]); sub_cell[10].push_back(temp[42]); sub_cell[10].push_back(temp[15]); sub_cell[10].push_back(temp[12]);
		//第12个单元
		sub_cell[11].push_back(temp[30]); sub_cell[11].push_back(temp[31]); sub_cell[11].push_back(temp[33]); sub_cell[11].push_back(temp[14]);
		sub_cell[11].push_back(temp[42]); sub_cell[11].push_back(temp[43]); sub_cell[11].push_back(temp[45]); sub_cell[11].push_back(temp[15]);
		//第13个单元
		sub_cell[12].push_back(temp[32]); sub_cell[12].push_back(temp[13]); sub_cell[12].push_back(temp[9]); sub_cell[12].push_back(temp[34]);
		sub_cell[12].push_back(temp[44]); sub_cell[12].push_back(temp[12]); sub_cell[12].push_back(temp[8]); sub_cell[12].push_back(temp[46]);
		//第14个单元
		sub_cell[13].push_back(temp[13]); sub_cell[13].push_back(temp[14]); sub_cell[13].push_back(temp[10]); sub_cell[13].push_back(temp[9]);
		sub_cell[13].push_back(temp[12]); sub_cell[13].push_back(temp[15]); sub_cell[13].push_back(temp[11]); sub_cell[13].push_back(temp[8]);
		//第15个单元
		sub_cell[14].push_back(temp[14]); sub_cell[14].push_back(temp[33]); sub_cell[14].push_back(temp[35]); sub_cell[14].push_back(temp[10]);
		sub_cell[14].push_back(temp[15]); sub_cell[14].push_back(temp[45]); sub_cell[14].push_back(temp[47]); sub_cell[14].push_back(temp[11]);
		//第16个单元
		sub_cell[15].push_back(temp[34]); sub_cell[15].push_back(temp[9]); sub_cell[15].push_back(temp[37]); sub_cell[15].push_back(temp[36]);
		sub_cell[15].push_back(temp[46]); sub_cell[15].push_back(temp[8]); sub_cell[15].push_back(temp[49]); sub_cell[15].push_back(temp[48]);
		//第17个单元
		sub_cell[16].push_back(temp[9]); sub_cell[16].push_back(temp[10]); sub_cell[16].push_back(temp[38]); sub_cell[16].push_back(temp[37]);
		sub_cell[16].push_back(temp[8]); sub_cell[16].push_back(temp[11]); sub_cell[16].push_back(temp[50]); sub_cell[16].push_back(temp[49]);
		//第18个单元
		sub_cell[17].push_back(temp[10]); sub_cell[17].push_back(temp[35]); sub_cell[17].push_back(temp[39]); sub_cell[17].push_back(temp[38]);
		sub_cell[17].push_back(temp[11]); sub_cell[17].push_back(temp[47]); sub_cell[17].push_back(temp[51]); sub_cell[17].push_back(temp[50]);
		/*第三层*/
		//第19个单元
		sub_cell[18].push_back(temp[40]); sub_cell[18].push_back(temp[41]); sub_cell[18].push_back(temp[12]); sub_cell[18].push_back(temp[44]);
		sub_cell[18].push_back(temp[4]); sub_cell[18].push_back(temp[52]); sub_cell[18].push_back(temp[55]); sub_cell[18].push_back(temp[54]);
		//第20个单元
		sub_cell[19].push_back(temp[41]); sub_cell[19].push_back(temp[42]); sub_cell[19].push_back(temp[15]); sub_cell[19].push_back(temp[12]);
		sub_cell[19].push_back(temp[52]); sub_cell[19].push_back(temp[53]); sub_cell[19].push_back(temp[56]); sub_cell[19].push_back(temp[55]);
		//第21个单元
		sub_cell[20].push_back(temp[42]); sub_cell[20].push_back(temp[43]); sub_cell[20].push_back(temp[45]); sub_cell[20].push_back(temp[15]);
		sub_cell[20].push_back(temp[53]); sub_cell[20].push_back(temp[7]); sub_cell[20].push_back(temp[57]); sub_cell[20].push_back(temp[56]);
		//第22个单元
		sub_cell[21].push_back(temp[44]); sub_cell[21].push_back(temp[12]); sub_cell[21].push_back(temp[8]); sub_cell[21].push_back(temp[46]);
		sub_cell[21].push_back(temp[54]); sub_cell[21].push_back(temp[55]); sub_cell[21].push_back(temp[59]); sub_cell[21].push_back(temp[58]);
		//第23个单元
		sub_cell[22].push_back(temp[12]); sub_cell[22].push_back(temp[15]); sub_cell[22].push_back(temp[11]); sub_cell[22].push_back(temp[8]);
		sub_cell[22].push_back(temp[55]); sub_cell[22].push_back(temp[56]); sub_cell[22].push_back(temp[60]); sub_cell[22].push_back(temp[59]);
		//第24个单元
		sub_cell[23].push_back(temp[15]); sub_cell[23].push_back(temp[45]); sub_cell[23].push_back(temp[47]); sub_cell[23].push_back(temp[11]);
		sub_cell[23].push_back(temp[56]); sub_cell[23].push_back(temp[57]); sub_cell[23].push_back(temp[61]); sub_cell[23].push_back(temp[60]);
		//第25个单元
		sub_cell[24].push_back(temp[46]); sub_cell[24].push_back(temp[8]); sub_cell[24].push_back(temp[49]); sub_cell[24].push_back(temp[48]);
		sub_cell[24].push_back(temp[58]); sub_cell[24].push_back(temp[59]); sub_cell[24].push_back(temp[62]); sub_cell[24].push_back(temp[0]);
		//第26个单元
		sub_cell[25].push_back(temp[8]); sub_cell[25].push_back(temp[11]); sub_cell[25].push_back(temp[50]); sub_cell[25].push_back(temp[49]);
		sub_cell[25].push_back(temp[59]); sub_cell[25].push_back(temp[60]); sub_cell[25].push_back(temp[63]); sub_cell[25].push_back(temp[62]);
		//第27个单元
		sub_cell[26].push_back(temp[11]); sub_cell[26].push_back(temp[47]); sub_cell[26].push_back(temp[51]); sub_cell[26].push_back(temp[50]);
		sub_cell[26].push_back(temp[60]); sub_cell[26].push_back(temp[61]); sub_cell[26].push_back(temp[3]); sub_cell[26].push_back(temp[63]);

		//插入单元
		for (int j = 0; j < 27; j++)
		{
			vmesh[i].add_cell(sub_cell[j]);
		}
	}
	return vmesh;
}

std::vector<HexV3fMesh>& ApproximationCC::constructBezierSolidDAT()
{
	int datsize = SolidData.size();
	int datnum = 0;
	isoline.resize(datsize);
	for (std::vector<std::vector<std::vector<std::vector<V3f>>>>::iterator SolidData_it = SolidData.begin();
		SolidData_it != SolidData.end(); SolidData_it++)
	{
		HexV3fMesh beziermesh;
		std::vector<std::vector<std::vector<V3f>>> bsdata = *SolidData_it;
		BezierSolid BS(bsdata);
		int size = 8;//要生成size*size*size个体
		std::vector<std::vector<V3f>> bezierv(size + 1);
		for (int i = 0; i < bezierv.size(); i++)
		{
			for (int j = 0; j < (size + 1)*(size + 1); j++)
			{
				bezierv[i].push_back(BS.P(j % (size + 1) * (1.0 / size), j / (size + 1) * (1.0 / size), i % (size + 1) * (1.0 / size)));
			}
		}
		//添加等参线
		std::vector<V3f> line1;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line1.push_back(BS.P(0.25, j * (1.0 / size), 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line1.push_back(BS.P(0.25, 1, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line1.push_back(BS.P(0.25, (size - j) * (1.0 / size), 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line1.push_back(BS.P(0.25, 0, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line2;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line2.push_back(BS.P(0.5, j * (1.0 / size), 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line2.push_back(BS.P(0.5, 1, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line2.push_back(BS.P(0.5, (size - j) * (1.0 / size), 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line2.push_back(BS.P(0.5, 0, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line3;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line3.push_back(BS.P(0.75, j * (1.0 / size), 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line3.push_back(BS.P(0.75, 1, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line3.push_back(BS.P(0.75, (size - j) * (1.0 / size), 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line3.push_back(BS.P(0.75, 0, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line11;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line11.push_back(BS.P(0.0, j * (1.0 / size), 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line11.push_back(BS.P(0.0, 1, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line11.push_back(BS.P(0.0, (size - j) * (1.0 / size), 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line11.push_back(BS.P(0.0, 0, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line12;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line12.push_back(BS.P(1.0, j * (1.0 / size), 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line12.push_back(BS.P(1.0, 1, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line12.push_back(BS.P(1.0, (size - j) * (1.0 / size), 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line12.push_back(BS.P(1.0, 0, (size - j) * (1.0 / size)));
		}


		std::vector<V3f> line4;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line4.push_back(BS.P(j * (1.0 / size), 0.25, 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line4.push_back(BS.P(1, 0.25, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line4.push_back(BS.P((size - j) * (1.0 / size), 0.25, 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line4.push_back(BS.P(0, 0.25, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line5;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line5.push_back(BS.P(j * (1.0 / size), 0.5, 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line5.push_back(BS.P(1, 0.5, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line5.push_back(BS.P((size - j) * (1.0 / size), 0.5, 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line5.push_back(BS.P(0, 0.5, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line6;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line6.push_back(BS.P(j * (1.0 / size), 0.75, 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line6.push_back(BS.P(1, 0.75, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line6.push_back(BS.P((size - j) * (1.0 / size), 0.75, 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line6.push_back(BS.P(0, 0.75, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line21;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line21.push_back(BS.P(j * (1.0 / size), 0.0, 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line21.push_back(BS.P(1, 0.0, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line21.push_back(BS.P((size - j) * (1.0 / size), 0.0, 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line21.push_back(BS.P(0, 0.0, (size - j) * (1.0 / size)));
		}
		std::vector<V3f> line22;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line22.push_back(BS.P(j * (1.0 / size), 1.0, 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line22.push_back(BS.P(1, 1.0, j * (1.0 / size)));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line22.push_back(BS.P((size - j) * (1.0 / size), 1.0, 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line22.push_back(BS.P(0, 1.0, (size - j) * (1.0 / size)));
		}


		std::vector<V3f> line7;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line7.push_back(BS.P(j * (1.0 / size), 1, 0.5));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line7.push_back(BS.P(1, (size - j) * (1.0 / size), 0.5));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line7.push_back(BS.P((size - j) * (1.0 / size), 0, 0.5));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line7.push_back(BS.P(0, j * (1.0 / size), 0.5));
		}
		std::vector<V3f> line8;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line8.push_back(BS.P(j * (1.0 / size), 1, 0.75));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line8.push_back(BS.P(1, (size - j) * (1.0 / size), 0.75));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line8.push_back(BS.P((size - j) * (1.0 / size), 0, 0.75));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line8.push_back(BS.P(0, j * (1.0 / size), 0.75));
		}
		std::vector<V3f> line9;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line9.push_back(BS.P(j * (1.0 / size), 1, 0.25));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line9.push_back(BS.P(1, (size - j) * (1.0 / size), 0.25));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line9.push_back(BS.P((size - j) * (1.0 / size), 0, 0.25));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line9.push_back(BS.P(0, j * (1.0 / size), 0.25));
		}
		std::vector<V3f> line31;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line31.push_back(BS.P(j * (1.0 / size), 1, 0));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line31.push_back(BS.P(1, (size - j) * (1.0 / size), 0));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line31.push_back(BS.P((size - j) * (1.0 / size), 0, 0));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line31.push_back(BS.P(0, j * (1.0 / size), 0));
		}
		std::vector<V3f> line32;
		for (int j = 0; j < size + 1; j++)//第1个面
		{
			line32.push_back(BS.P(j * (1.0 / size), 1, 1));
		}
		for (int j = 0; j < size + 1; j++)//第2个面
		{
			line32.push_back(BS.P(1, (size - j) * (1.0 / size), 1));
		}
		for (int j = 0; j < size + 1; j++)//第3个面
		{
			line32.push_back(BS.P((size - j) * (1.0 / size), 0, 1));
		}
		for (int j = 0; j < size + 1; j++)//第4个面
		{
			line32.push_back(BS.P(0, j * (1.0 / size), 1));
		}

		isoline[datnum].push_back(line1); isoline[datnum].push_back(line2); isoline[datnum].push_back(line3);
		isoline[datnum].push_back(line4); isoline[datnum].push_back(line5); isoline[datnum].push_back(line6);
		isoline[datnum].push_back(line7); isoline[datnum].push_back(line8); isoline[datnum].push_back(line9);
		isoline[datnum].push_back(line11); isoline[datnum].push_back(line12); isoline[datnum].push_back(line21); isoline[datnum].push_back(line22);
		isoline[datnum].push_back(line31); isoline[datnum].push_back(line32);

		//添加边界线
		/*std::vector<V3f> linequick;
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(0, 1, j * (1.0 / size)));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(1, 0, j * (1.0 / size)));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(0, 0, j * (1.0 / size)));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(1, 1, j * (1.0 / size)));
		}
		line_quick.push_back(linequick); linequick.clear();

		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(0, j * (1.0 / size), 1));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(1, j * (1.0 / size), 0));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(0, j * (1.0 / size), 0));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(1, j * (1.0 / size), 1));
		}
		line_quick.push_back(linequick); linequick.clear();

		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(j * (1.0 / size), 0, 1));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(j * (1.0 / size), 1, 0));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(j * (1.0 / size), 0, 0));
		}
		line_quick.push_back(linequick); linequick.clear();
		for (int j = 0; j < size + 1; j++)
		{
			linequick.push_back(BS.P(j * (1.0 / size), 1, 1));
		}
		line_quick.push_back(linequick); linequick.clear();*/


		for (int j = 0; j < size; j++)
		{
			for (int i = 0; i < size*size; i++)
			{
				std::vector<VertexHandle> ctemp;
				int t = i + i / size;
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t])); ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + 1]));
				ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 2])); ctemp.push_back(beziermesh.add_vertex(bezierv[j][t + size + 1]));
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t])); ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + 1]));
				ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 2])); ctemp.push_back(beziermesh.add_vertex(bezierv[j + 1][t + size + 1]));
				beziermesh.add_cell(ctemp);
			}
		}
		vBmesh.push_back(beziermesh);
		datnum++;
	}
	return vBmesh;
}

void ApproximationCC::cal_ccver()
{
	int mv_size = mesh->vertices_.size();
	for (int i = 0; i < mv_size; i++)
	{
		ccver.push_back(solid_corner_point(VertexHandle(i)));
	}
}

void ApproximationCC::cal_limitccver()
{
	int mv_size = mesh->vertices_.size();
	for (int i = 0; i < mv_size; i++)
	{
		VertexHandle vertex = VertexHandle(i);
		V3f new_vertex;
		V3f vertex_sum;
		if (mesh->is_bdy(vertex))
		{
			int en = 0;
			std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
			int v_e_n = v_e.size();
			V3f e_av;
			for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
			{
				if (mesh->is_bdy(*v_e_it))
				{
					en++;
					V3f temp_ev = cal_edge_mid(*v_e_it);
					e_av = e_av + temp_ev;
				}
			}
			//e_av = e_av / en;
			//vertex_sum = vertex_sum + mesh->vertices_[vertex] * en * en;
			std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
			V3f f_av;
			for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
			{
				if (mesh->is_bdy(*v_f_it))
				{
					V3f temp_fv = cal_face_mid(*v_f_it);
					f_av = f_av + temp_fv;
				}
			}
			//f_av = f_av / en;
			vertex_sum = vertex_sum + cal_new_vertex(vertex) * en * en + e_av * 4 + f_av;
			new_vertex = vertex_sum / ((en + 5) * en);
		}
		else
		{
			std::set<EdgeHandle, compare_OVM> v_e = mesh->neighbor_v[vertex];
			int en = v_e.size();
			int sum = 0;
			sum += 16 * (en - 2);
			for (std::set<EdgeHandle, compare_OVM>::iterator v_e_it = v_e.begin(); v_e_it != v_e.end(); v_e_it++)
			{
				V3f temp_ev = cal_edge_mid(*v_e_it);
				int m = mesh->neighbor_e[*v_e_it].size();
				sum += 4 * m;
				vertex_sum = vertex_sum + temp_ev * 4 * m;
			}
			std::set<FaceHandle, compare_OVM> v_f = mesh->neighbor_v_f[vertex];
			for (std::set<FaceHandle, compare_OVM>::iterator v_f_it = v_f.begin(); v_f_it != v_f.end(); v_f_it++)
			{
				V3f temp_fv = cal_face_mid(*v_f_it);
				vertex_sum = vertex_sum + temp_fv * 4;
				sum += 4;
			}
			std::set<CellHandle, compare_OVM> v_c = mesh->neighbor_v_c[vertex];
			for (std::set<CellHandle, compare_OVM>::iterator v_c_it = v_c.begin(); v_c_it != v_c.end(); v_c_it++)
			{
				V3f temp_cv = cal_cell_mid(*v_c_it);
				vertex_sum = vertex_sum + temp_cv;
				sum += 1;
			}
			vertex_sum = vertex_sum + cal_new_vertex(vertex) * ((en - 2) * 4 * 4);
			new_vertex = vertex_sum / sum;
		}
		ccver.push_back(new_vertex);
	}
}

V3f ApproximationCC::selectapoint()
{
	int i = 0;
	for (std::vector<V3f>::iterator mv = mesh->vertices_.begin(); mv != mesh->vertices_.end(); mv++)
	{
		if (!mesh->is_bdy(VertexHandle(i)))
		{
			if (mesh->neighbor_v[i].size() == 5)
			{
				V3f v = solid_corner_point(VertexHandle(i));
				return v;
			}
			else
				i++;
		}
		else
			i++;

	}
}

void ApproximationCC::construct_bss()
{
	uint64_t cell_num = mesh->cells_.size();
	bss.init(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		bss.CP_Local_to_Global[i].resize(64);
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_corner_point(mesh->cells_[i].vertex_[vn]));
			//构造局部的小单元的每个点对应于全局模型中的点
			bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
				bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_inter_point(CellHandle(i), mesh->cells_[i].vertex_[vn]));
			bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
				bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		}
		//第一层
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 9, 5));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 9, 6));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 1, 5));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 2, 5));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 2, 6));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 7, 6));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 1, 1));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 2, 1));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 2, 2));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 7, 2));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 8, 1));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 8, 2));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		//第二层
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 2, 5));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 4, 5));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 4, 6));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 6, 6));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 0, 5));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 1, 6));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 0, 1));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 1, 2));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 0, 1));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 5, 1));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 5, 2));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 4, 2));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		//第三层
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 2, 4));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 4, 4));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 4, 7));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 6, 7));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 0, 4));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 1, 7));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 0, 0));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 1, 3));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 0, 0));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 5, 0));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 5, 3));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 4, 3));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		//第4层
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 11, 4));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 11, 7));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 3, 4));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 3, 4));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 3, 7));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 5, 7));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 3, 0));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 3, 0));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_face_point(i, 3, 3));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 5, 3));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 10, 0));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(solid_edge_point(i, 10, 3));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//构建每一个块按顺序排序控制点的下标
		std::vector<std::vector<std::vector<VertexHandle>>> sc(4);
		std::vector<std::vector<VertexHandle>> sf(4);
		std::vector<VertexHandle> sl(4);
		//第一层
		sl[0] = temp[5]; sl[1] = temp[18]; sl[2] = temp[22]; sl[3] = temp[1];
		sf[0] = sl;
		sl[0] = temp[16]; sl[1] = temp[19]; sl[2] = temp[23]; sl[3] = temp[26];
		sf[1] = sl;
		sl[0] = temp[17]; sl[1] = temp[20]; sl[2] = temp[24]; sl[3] = temp[27];
		sf[2] = sl;
		sl[0] = temp[6]; sl[1] = temp[21]; sl[2] = temp[25]; sl[3] = temp[2];
		sf[3] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = temp[28]; sl[1] = temp[32]; sl[2] = temp[34]; sl[3] = temp[36];
		sf[0] = sl;
		sl[0] = temp[29]; sl[1] = temp[13]; sl[2] = temp[9]; sl[3] = temp[37];
		sf[1] = sl;
		sl[0] = temp[30]; sl[1] = temp[14]; sl[2] = temp[10]; sl[3] = temp[38];
		sf[2] = sl;
		sl[0] = temp[31]; sl[1] = temp[33]; sl[2] = temp[35]; sl[3] = temp[39];
		sf[3] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = temp[40]; sl[1] = temp[44]; sl[2] = temp[46]; sl[3] = temp[48];
		sf[0] = sl;
		sl[0] = temp[41]; sl[1] = temp[12]; sl[2] = temp[8]; sl[3] = temp[49];
		sf[1] = sl;
		sl[0] = temp[42]; sl[1] = temp[15]; sl[2] = temp[11]; sl[3] = temp[50];
		sf[2] = sl;
		sl[0] = temp[43]; sl[1] = temp[45]; sl[2] = temp[47]; sl[3] = temp[51];
		sf[3] = sl;
		sc[2] = sf;
		//第四层
		sl[0] = temp[4]; sl[1] = temp[54]; sl[2] = temp[58]; sl[3] = temp[0];
		sf[0] = sl;
		sl[0] = temp[52]; sl[1] = temp[55]; sl[2] = temp[59]; sl[3] = temp[62];
		sf[1] = sl;
		sl[0] = temp[53]; sl[1] = temp[56]; sl[2] = temp[60]; sl[3] = temp[63];
		sf[2] = sl;
		sl[0] = temp[7]; sl[1] = temp[57]; sl[2] = temp[61]; sl[3] = temp[3];
		sf[3] = sl;
		sc[3] = sf;
		bss.Control_Point_id[i] = sc;

		//构建Bezier体的数据
		std::vector<std::vector<std::vector<V3f>>> sc1(4);
		std::vector<std::vector<V3f>> sf1(4);
		std::vector<V3f> sl1(4);
		//第一层
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[5]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[18]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[22]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[1]];
		sf1[0] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[16]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[19]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[23]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[26]];
		sf1[1] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[17]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[20]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[24]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[27]];
		sf1[2] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[6]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[21]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[25]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[2]];
		sf1[3] = sl1;
		sc1[0] = sf1;
		//第二层
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[28]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[32]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[34]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[36]];
		sf1[0] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[29]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[13]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[9]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[37]];
		sf1[1] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[30]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[14]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[10]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[38]];
		sf1[2] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[31]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[33]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[35]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[39]];
		sf1[3] = sl1;
		sc1[1] = sf1;
		//第三层
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[40]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[44]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[46]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[48]];
		sf1[0] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[41]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[12]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[8]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[49]];
		sf1[1] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[42]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[15]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[11]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[50]];
		sf1[2] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[43]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[45]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[47]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[51]];
		sf1[3] = sl1;
		sc1[2] = sf1;
		//第四层
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[4]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[54]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[58]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[0]];
		sf1[0] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[52]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[55]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[59]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[62]];
		sf1[1] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[53]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[56]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[60]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[63]];
		sf1[2] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[7]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[57]];
		sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[61]]; sl1[3] = bss.Local_BSplineSolid[i].vertices_[temp[3]];
		sf1[3] = sl1;
		sc1[3] = sf1;
		SolidData.push_back(sc1);

		//构建局部的每个小单元的u,v,w三个方向的次数
		std::vector<int> order(3); order[0] = 3; order[1] = 3; order[2] = 3;
		bss.Local_Order[i] = order;

		//构建局部的每个小单元的u,v,w三个方向的节点向量
		std::vector<std::vector<double>> node(3);
		node[0].push_back(0); node[0].push_back(0); node[0].push_back(0); node[0].push_back(0);
		node[0].push_back(1); node[0].push_back(1); node[0].push_back(1); node[0].push_back(1);
		node[1].push_back(0); node[1].push_back(0); node[1].push_back(0); node[1].push_back(0);
		node[1].push_back(1); node[1].push_back(1); node[1].push_back(1); node[1].push_back(1);
		node[2].push_back(0); node[2].push_back(0); node[2].push_back(0); node[2].push_back(0);
		node[2].push_back(1); node[2].push_back(1); node[2].push_back(1); node[2].push_back(1);
		bss.Local_Node[i] = node;

		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(27);
		std::vector<std::vector<VertexHandle>> sub_cell_global(27);
		/*第一层*/
		//第1个单元
		sub_cell[0].push_back(temp[5]); sub_cell[0].push_back(temp[16]); sub_cell[0].push_back(temp[19]); sub_cell[0].push_back(temp[18]);
		sub_cell[0].push_back(temp[28]); sub_cell[0].push_back(temp[29]); sub_cell[0].push_back(temp[13]); sub_cell[0].push_back(temp[32]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[5]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[16]]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[19]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[18]]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[28]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[29]]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[13]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[32]]);
		//第2个单元
		sub_cell[1].push_back(temp[16]); sub_cell[1].push_back(temp[17]); sub_cell[1].push_back(temp[20]); sub_cell[1].push_back(temp[19]);
		sub_cell[1].push_back(temp[29]); sub_cell[1].push_back(temp[30]); sub_cell[1].push_back(temp[14]); sub_cell[1].push_back(temp[13]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[16]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[17]]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[20]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[19]]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[29]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[30]]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[14]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[13]]);
		//第3个单元
		sub_cell[2].push_back(temp[17]); sub_cell[2].push_back(temp[6]); sub_cell[2].push_back(temp[21]); sub_cell[2].push_back(temp[20]);
		sub_cell[2].push_back(temp[30]); sub_cell[2].push_back(temp[31]); sub_cell[2].push_back(temp[33]); sub_cell[2].push_back(temp[14]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[17]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[6]]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[21]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[20]]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[30]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[31]]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[33]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[14]]);
		//第4个单元
		sub_cell[3].push_back(temp[18]); sub_cell[3].push_back(temp[19]); sub_cell[3].push_back(temp[23]); sub_cell[3].push_back(temp[22]);
		sub_cell[3].push_back(temp[32]); sub_cell[3].push_back(temp[13]); sub_cell[3].push_back(temp[9]); sub_cell[3].push_back(temp[34]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[18]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[19]]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[23]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[22]]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[32]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[13]]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[9]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[34]]);
		//第5个单元
		sub_cell[4].push_back(temp[19]); sub_cell[4].push_back(temp[20]); sub_cell[4].push_back(temp[24]); sub_cell[4].push_back(temp[23]);
		sub_cell[4].push_back(temp[13]); sub_cell[4].push_back(temp[14]); sub_cell[4].push_back(temp[10]); sub_cell[4].push_back(temp[9]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[19]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[20]]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[24]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[23]]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[13]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[14]]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[10]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[9]]);
		//第6个单元
		sub_cell[5].push_back(temp[20]); sub_cell[5].push_back(temp[21]); sub_cell[5].push_back(temp[25]); sub_cell[5].push_back(temp[24]);
		sub_cell[5].push_back(temp[14]); sub_cell[5].push_back(temp[33]); sub_cell[5].push_back(temp[35]); sub_cell[5].push_back(temp[10]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[20]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[21]]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[25]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[24]]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[14]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[33]]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[35]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[10]]);
		//第7个单元
		sub_cell[6].push_back(temp[22]); sub_cell[6].push_back(temp[23]); sub_cell[6].push_back(temp[26]); sub_cell[6].push_back(temp[1]);
		sub_cell[6].push_back(temp[34]); sub_cell[6].push_back(temp[9]); sub_cell[6].push_back(temp[37]); sub_cell[6].push_back(temp[36]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[22]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[23]]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[26]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[1]]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[34]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[9]]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[37]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[36]]);
		//第8个单元
		sub_cell[7].push_back(temp[23]); sub_cell[7].push_back(temp[24]); sub_cell[7].push_back(temp[27]); sub_cell[7].push_back(temp[26]);
		sub_cell[7].push_back(temp[9]); sub_cell[7].push_back(temp[10]); sub_cell[7].push_back(temp[38]); sub_cell[7].push_back(temp[37]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[23]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[24]]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[27]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[26]]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[9]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[10]]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[38]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[37]]);
		//第9个单元
		sub_cell[8].push_back(temp[24]); sub_cell[8].push_back(temp[25]); sub_cell[8].push_back(temp[2]); sub_cell[8].push_back(temp[27]);
		sub_cell[8].push_back(temp[10]); sub_cell[8].push_back(temp[35]); sub_cell[8].push_back(temp[39]); sub_cell[8].push_back(temp[38]);
		sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[24]]); sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[25]]);
		sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[2]]); sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[27]]);
		sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[10]]); sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[35]]);
		sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[39]]); sub_cell_global[8].push_back(bss.CP_Local_to_Global[i][temp[38]]);
		/*第二层*/
		//第10个单元
		sub_cell[9].push_back(temp[28]); sub_cell[9].push_back(temp[29]); sub_cell[9].push_back(temp[13]); sub_cell[9].push_back(temp[32]);
		sub_cell[9].push_back(temp[40]); sub_cell[9].push_back(temp[41]); sub_cell[9].push_back(temp[12]); sub_cell[9].push_back(temp[44]);
		sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[28]]); sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[29]]);
		sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[13]]); sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[32]]);
		sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[40]]); sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[41]]);
		sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[12]]); sub_cell_global[9].push_back(bss.CP_Local_to_Global[i][temp[44]]);
		//第11个单元
		sub_cell[10].push_back(temp[29]); sub_cell[10].push_back(temp[30]); sub_cell[10].push_back(temp[14]); sub_cell[10].push_back(temp[13]);
		sub_cell[10].push_back(temp[41]); sub_cell[10].push_back(temp[42]); sub_cell[10].push_back(temp[15]); sub_cell[10].push_back(temp[12]);
		sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[29]]); sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[30]]);
		sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[14]]); sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[13]]);
		sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[41]]); sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[42]]);
		sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[15]]); sub_cell_global[10].push_back(bss.CP_Local_to_Global[i][temp[12]]);
		//第12个单元
		sub_cell[11].push_back(temp[30]); sub_cell[11].push_back(temp[31]); sub_cell[11].push_back(temp[33]); sub_cell[11].push_back(temp[14]);
		sub_cell[11].push_back(temp[42]); sub_cell[11].push_back(temp[43]); sub_cell[11].push_back(temp[45]); sub_cell[11].push_back(temp[15]);
		sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[30]]); sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[31]]);
		sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[33]]); sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[14]]);
		sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[42]]); sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[43]]);
		sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[45]]); sub_cell_global[11].push_back(bss.CP_Local_to_Global[i][temp[15]]);
		//第13个单元
		sub_cell[12].push_back(temp[32]); sub_cell[12].push_back(temp[13]); sub_cell[12].push_back(temp[9]); sub_cell[12].push_back(temp[34]);
		sub_cell[12].push_back(temp[44]); sub_cell[12].push_back(temp[12]); sub_cell[12].push_back(temp[8]); sub_cell[12].push_back(temp[46]);
		sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[32]]); sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[13]]);
		sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[9]]); sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[34]]);
		sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[44]]); sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[12]]);
		sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[8]]); sub_cell_global[12].push_back(bss.CP_Local_to_Global[i][temp[46]]);
		//第14个单元
		sub_cell[13].push_back(temp[13]); sub_cell[13].push_back(temp[14]); sub_cell[13].push_back(temp[10]); sub_cell[13].push_back(temp[9]);
		sub_cell[13].push_back(temp[12]); sub_cell[13].push_back(temp[15]); sub_cell[13].push_back(temp[11]); sub_cell[13].push_back(temp[8]);
		sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[13]]); sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[14]]);
		sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[10]]); sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[9]]);
		sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[12]]); sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[15]]);
		sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[11]]); sub_cell_global[13].push_back(bss.CP_Local_to_Global[i][temp[8]]);
		//第15个单元
		sub_cell[14].push_back(temp[14]); sub_cell[14].push_back(temp[33]); sub_cell[14].push_back(temp[35]); sub_cell[14].push_back(temp[10]);
		sub_cell[14].push_back(temp[15]); sub_cell[14].push_back(temp[45]); sub_cell[14].push_back(temp[47]); sub_cell[14].push_back(temp[11]);
		sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[14]]); sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[33]]);
		sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[35]]); sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[10]]);
		sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[15]]); sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[45]]);
		sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[47]]); sub_cell_global[14].push_back(bss.CP_Local_to_Global[i][temp[11]]);
		//第16个单元
		sub_cell[15].push_back(temp[34]); sub_cell[15].push_back(temp[9]); sub_cell[15].push_back(temp[37]); sub_cell[15].push_back(temp[36]);
		sub_cell[15].push_back(temp[46]); sub_cell[15].push_back(temp[8]); sub_cell[15].push_back(temp[49]); sub_cell[15].push_back(temp[48]);
		sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[34]]); sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[9]]);
		sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[37]]); sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[36]]);
		sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[46]]); sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[8]]);
		sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[49]]); sub_cell_global[15].push_back(bss.CP_Local_to_Global[i][temp[48]]);
		//第17个单元
		sub_cell[16].push_back(temp[9]); sub_cell[16].push_back(temp[10]); sub_cell[16].push_back(temp[38]); sub_cell[16].push_back(temp[37]);
		sub_cell[16].push_back(temp[8]); sub_cell[16].push_back(temp[11]); sub_cell[16].push_back(temp[50]); sub_cell[16].push_back(temp[49]);
		sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[9]]); sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[10]]);
		sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[38]]); sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[37]]);
		sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[8]]); sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[11]]);
		sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[50]]); sub_cell_global[16].push_back(bss.CP_Local_to_Global[i][temp[49]]);
		//第18个单元
		sub_cell[17].push_back(temp[10]); sub_cell[17].push_back(temp[35]); sub_cell[17].push_back(temp[39]); sub_cell[17].push_back(temp[38]);
		sub_cell[17].push_back(temp[11]); sub_cell[17].push_back(temp[47]); sub_cell[17].push_back(temp[51]); sub_cell[17].push_back(temp[50]);
		sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[10]]); sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[35]]);
		sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[39]]); sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[38]]);
		sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[11]]); sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[47]]);
		sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[51]]); sub_cell_global[17].push_back(bss.CP_Local_to_Global[i][temp[50]]);
		/*第三层*/
		//第19个单元
		sub_cell[18].push_back(temp[40]); sub_cell[18].push_back(temp[41]); sub_cell[18].push_back(temp[12]); sub_cell[18].push_back(temp[44]);
		sub_cell[18].push_back(temp[4]); sub_cell[18].push_back(temp[52]); sub_cell[18].push_back(temp[55]); sub_cell[18].push_back(temp[54]);
		sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[40]]); sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[41]]);
		sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[12]]); sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[44]]);
		sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[4]]); sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[52]]);
		sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[55]]); sub_cell_global[18].push_back(bss.CP_Local_to_Global[i][temp[54]]);
		//第20个单元
		sub_cell[19].push_back(temp[41]); sub_cell[19].push_back(temp[42]); sub_cell[19].push_back(temp[15]); sub_cell[19].push_back(temp[12]);
		sub_cell[19].push_back(temp[52]); sub_cell[19].push_back(temp[53]); sub_cell[19].push_back(temp[56]); sub_cell[19].push_back(temp[55]);
		sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[41]]); sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[42]]);
		sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[15]]); sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[12]]);
		sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[52]]); sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[53]]);
		sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[56]]); sub_cell_global[19].push_back(bss.CP_Local_to_Global[i][temp[55]]);
		//第21个单元
		sub_cell[20].push_back(temp[42]); sub_cell[20].push_back(temp[43]); sub_cell[20].push_back(temp[45]); sub_cell[20].push_back(temp[15]);
		sub_cell[20].push_back(temp[53]); sub_cell[20].push_back(temp[7]); sub_cell[20].push_back(temp[57]); sub_cell[20].push_back(temp[56]);
		sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[42]]); sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[43]]);
		sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[45]]); sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[15]]);
		sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[53]]); sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[7]]);
		sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[57]]); sub_cell_global[20].push_back(bss.CP_Local_to_Global[i][temp[56]]);
		//第22个单元
		sub_cell[21].push_back(temp[44]); sub_cell[21].push_back(temp[12]); sub_cell[21].push_back(temp[8]); sub_cell[21].push_back(temp[46]);
		sub_cell[21].push_back(temp[54]); sub_cell[21].push_back(temp[55]); sub_cell[21].push_back(temp[59]); sub_cell[21].push_back(temp[58]);
		sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[44]]); sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[12]]);
		sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[8]]); sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[46]]);
		sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[54]]); sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[55]]);
		sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[59]]); sub_cell_global[21].push_back(bss.CP_Local_to_Global[i][temp[58]]);
		//第23个单元
		sub_cell[22].push_back(temp[12]); sub_cell[22].push_back(temp[15]); sub_cell[22].push_back(temp[11]); sub_cell[22].push_back(temp[8]);
		sub_cell[22].push_back(temp[55]); sub_cell[22].push_back(temp[56]); sub_cell[22].push_back(temp[60]); sub_cell[22].push_back(temp[59]);
		sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[12]]); sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[15]]);
		sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[11]]); sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[8]]);
		sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[55]]); sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[56]]);
		sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[60]]); sub_cell_global[22].push_back(bss.CP_Local_to_Global[i][temp[59]]);
		//第24个单元
		sub_cell[23].push_back(temp[15]); sub_cell[23].push_back(temp[45]); sub_cell[23].push_back(temp[47]); sub_cell[23].push_back(temp[11]);
		sub_cell[23].push_back(temp[56]); sub_cell[23].push_back(temp[57]); sub_cell[23].push_back(temp[61]); sub_cell[23].push_back(temp[60]);
		sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[15]]); sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[45]]);
		sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[47]]); sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[11]]);
		sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[56]]); sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[57]]);
		sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[61]]); sub_cell_global[23].push_back(bss.CP_Local_to_Global[i][temp[60]]);
		//第25个单元
		sub_cell[24].push_back(temp[46]); sub_cell[24].push_back(temp[8]); sub_cell[24].push_back(temp[49]); sub_cell[24].push_back(temp[48]);
		sub_cell[24].push_back(temp[58]); sub_cell[24].push_back(temp[59]); sub_cell[24].push_back(temp[62]); sub_cell[24].push_back(temp[0]);
		sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[46]]); sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[8]]);
		sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[49]]); sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[48]]);
		sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[58]]); sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[59]]);
		sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[62]]); sub_cell_global[24].push_back(bss.CP_Local_to_Global[i][temp[0]]);
		//第26个单元
		sub_cell[25].push_back(temp[8]); sub_cell[25].push_back(temp[11]); sub_cell[25].push_back(temp[50]); sub_cell[25].push_back(temp[49]);
		sub_cell[25].push_back(temp[59]); sub_cell[25].push_back(temp[60]); sub_cell[25].push_back(temp[63]); sub_cell[25].push_back(temp[62]);
		sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[8]]); sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[11]]);
		sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[50]]); sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[49]]);
		sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[59]]); sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[60]]);
		sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[63]]); sub_cell_global[25].push_back(bss.CP_Local_to_Global[i][temp[62]]);
		//第27个单元
		sub_cell[26].push_back(temp[11]); sub_cell[26].push_back(temp[47]); sub_cell[26].push_back(temp[51]); sub_cell[26].push_back(temp[50]);
		sub_cell[26].push_back(temp[60]); sub_cell[26].push_back(temp[61]); sub_cell[26].push_back(temp[3]); sub_cell[26].push_back(temp[63]);
		sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[11]]); sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[47]]);
		sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[51]]); sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[50]]);
		sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[60]]); sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[61]]);
		sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[3]]); sub_cell_global[26].push_back(bss.CP_Local_to_Global[i][temp[63]]);

		//插入单元
		for (int j = 0; j < 27; j++)
		{
			bss.Local_BSplineSolid[i].add_cell(sub_cell[j]);
			bss.Global_BSplineSolid.add_cell(sub_cell_global[j]);
		}
	}
}

void ApproximationCC::construct_bss1()
{
	uint64_t cell_num = mesh->cells_.size();
	bss.init(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		bss.CP_Local_to_Global[i].resize(64);
		VertexHandle temp[27];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(mesh->vertices_[mesh->cells_[i].vertex_[vn]]);
			//构造局部的小单元的每个点对应于全局模型中的点
			bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
				bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		}

		//第一条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[0]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第二条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[1]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第三条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[2]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第四条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[3]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第五条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[4]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第六条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[5]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第七条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[6]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第八条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[7]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第九条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[8]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第十条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[9]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第十一条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[10]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//第十二条边的中点
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cal_edge_midv(mesh->cells_[i].edges_[11]));
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//再插入每个面的中点
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_face_midv(mesh->cells_[i].faces_[mesh_face_num]);
			temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(face_mid);
			bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
				bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);
		}

		//插入单元的内心
		V3f cell_mid = cal_cell_midv(CellHandle(i));
		temp[tempsize++] = bss.Local_BSplineSolid[i].add_vertex(cell_mid);
		bss.CP_Local_to_Global[i][temp[tempsize - 1]] =
			bss.Global_BSplineSolid.add_vertex(bss.Local_BSplineSolid[i].vertices_[temp[tempsize - 1]]);

		//构建每一个块按顺序排序控制点的下标
		std::vector<std::vector<std::vector<VertexHandle>>> sc(3);
		std::vector<std::vector<VertexHandle>> sf(3);
		std::vector<VertexHandle> sl(3);
		//第一层
		sl[0] = temp[5]; sl[1] = temp[9]; sl[2] = temp[1];
		sf[0] = sl;
		sl[0] = temp[17]; sl[1] = temp[22]; sl[2] = temp[16];
		sf[1] = sl;
		sl[0] = temp[6]; sl[1] = temp[15]; sl[2] = temp[2];
		sf[2] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = temp[10]; sl[1] = temp[20]; sl[2] = temp[8];
		sf[0] = sl;
		sl[0] = temp[24]; sl[1] = temp[26]; sl[2] = temp[25];
		sf[1] = sl;
		sl[0] = temp[14]; sl[1] = temp[21]; sl[2] = temp[12];
		sf[2] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = temp[4]; sl[1] = temp[11]; sl[2] = temp[0];
		sf[0] = sl;
		sl[0] = temp[19]; sl[1] = temp[23]; sl[2] = temp[18];
		sf[1] = sl;
		sl[0] = temp[7]; sl[1] = temp[13]; sl[2] = temp[3];
		sf[2] = sl;
		sc[2] = sf;
		bss.Control_Point_id[i] = sc;

		//构建Bezier体的数据
		std::vector<std::vector<std::vector<V3f>>> sc1(3);
		std::vector<std::vector<V3f>> sf1(3);
		std::vector<V3f> sl1(3);
		//第一层
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[5]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[9]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[1]];
		sf1[0] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[17]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[22]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[16]];
		sf1[1] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[6]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[15]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[2]];
		sf1[2] = sl1;
		sc1[0] = sf1;
		//第二层
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[10]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[20]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[8]];
		sf1[0] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[24]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[26]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[25]];
		sf1[1] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[14]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[21]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[12]];
		sf1[2] = sl1;
		sc1[1] = sf1;
		//第三层
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[4]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[11]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[0]];
		sf1[0] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[19]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[23]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[18]];
		sf1[1] = sl1;
		sl1[0] = bss.Local_BSplineSolid[i].vertices_[temp[7]]; sl1[1] = bss.Local_BSplineSolid[i].vertices_[temp[13]]; sl1[2] = bss.Local_BSplineSolid[i].vertices_[temp[3]];
		sf1[2] = sl1;
		sc1[2] = sf1;
		SolidData.push_back(sc1);

		//构建局部的每个小单元的u,v,w三个方向的次数
		std::vector<int> order(3); order[0] = 2; order[1] = 2; order[2] = 2;
		bss.Local_Order[i] = order;

		//构建局部的每个小单元的u,v,w三个方向的节点向量
		std::vector<std::vector<double>> node(3);
		node[0].push_back(0); node[0].push_back(0); node[0].push_back(0);
		node[0].push_back(1); node[0].push_back(1); node[0].push_back(1);
		node[1].push_back(0); node[1].push_back(0); node[1].push_back(0);
		node[1].push_back(1); node[1].push_back(1); node[1].push_back(1);
		node[2].push_back(0); node[2].push_back(0); node[2].push_back(0);
		node[2].push_back(1); node[2].push_back(1); node[2].push_back(1);
		bss.Local_Node[i] = node;

		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(8);
		std::vector<std::vector<VertexHandle>> sub_cell_global(8);
		//第一个单元
		sub_cell[0].push_back(temp[0]); sub_cell[0].push_back(temp[8]);
		sub_cell[0].push_back(temp[25]); sub_cell[0].push_back(temp[18]);
		sub_cell[0].push_back(temp[11]); sub_cell[0].push_back(temp[20]);
		sub_cell[0].push_back(temp[26]); sub_cell[0].push_back(temp[23]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[0]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[8]]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[25]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[18]]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[11]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[20]]);
		sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[26]]); sub_cell_global[0].push_back(bss.CP_Local_to_Global[i][temp[23]]);
		//第二个单元
		sub_cell[1].push_back(temp[8]); sub_cell[1].push_back(temp[1]);
		sub_cell[1].push_back(temp[16]); sub_cell[1].push_back(temp[25]);
		sub_cell[1].push_back(temp[20]); sub_cell[1].push_back(temp[9]);
		sub_cell[1].push_back(temp[22]); sub_cell[1].push_back(temp[26]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[8]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[1]]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[16]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[25]]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[20]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[9]]);
		sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[22]]); sub_cell_global[1].push_back(bss.CP_Local_to_Global[i][temp[26]]);
		//第三个单元
		sub_cell[2].push_back(temp[18]); sub_cell[2].push_back(temp[25]);
		sub_cell[2].push_back(temp[12]); sub_cell[2].push_back(temp[3]);
		sub_cell[2].push_back(temp[23]); sub_cell[2].push_back(temp[26]);
		sub_cell[2].push_back(temp[21]); sub_cell[2].push_back(temp[13]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[18]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[25]]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[12]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[3]]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[23]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[26]]);
		sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[21]]); sub_cell_global[2].push_back(bss.CP_Local_to_Global[i][temp[13]]);
		//第四个单元
		sub_cell[3].push_back(temp[25]); sub_cell[3].push_back(temp[16]);
		sub_cell[3].push_back(temp[2]); sub_cell[3].push_back(temp[12]);
		sub_cell[3].push_back(temp[26]); sub_cell[3].push_back(temp[22]);
		sub_cell[3].push_back(temp[15]); sub_cell[3].push_back(temp[21]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[25]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[16]]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[2]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[12]]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[26]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[22]]);
		sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[15]]); sub_cell_global[3].push_back(bss.CP_Local_to_Global[i][temp[21]]);
		//第五个单元
		sub_cell[4].push_back(temp[11]); sub_cell[4].push_back(temp[20]);
		sub_cell[4].push_back(temp[26]); sub_cell[4].push_back(temp[23]);
		sub_cell[4].push_back(temp[4]); sub_cell[4].push_back(temp[10]);
		sub_cell[4].push_back(temp[24]); sub_cell[4].push_back(temp[19]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[11]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[20]]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[26]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[23]]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[4]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[10]]);
		sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[24]]); sub_cell_global[4].push_back(bss.CP_Local_to_Global[i][temp[19]]);
		//第六个单元
		sub_cell[5].push_back(temp[20]); sub_cell[5].push_back(temp[9]);
		sub_cell[5].push_back(temp[22]); sub_cell[5].push_back(temp[26]);
		sub_cell[5].push_back(temp[10]); sub_cell[5].push_back(temp[5]);
		sub_cell[5].push_back(temp[17]); sub_cell[5].push_back(temp[24]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[20]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[9]]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[22]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[26]]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[10]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[5]]);
		sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[17]]); sub_cell_global[5].push_back(bss.CP_Local_to_Global[i][temp[24]]);
		//第七个单元
		sub_cell[6].push_back(temp[23]); sub_cell[6].push_back(temp[26]);
		sub_cell[6].push_back(temp[21]); sub_cell[6].push_back(temp[13]);
		sub_cell[6].push_back(temp[19]); sub_cell[6].push_back(temp[24]);
		sub_cell[6].push_back(temp[14]); sub_cell[6].push_back(temp[7]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[23]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[26]]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[21]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[13]]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[19]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[24]]);
		sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[14]]); sub_cell_global[6].push_back(bss.CP_Local_to_Global[i][temp[7]]);
		//第八个单元
		sub_cell[7].push_back(temp[26]); sub_cell[7].push_back(temp[22]);
		sub_cell[7].push_back(temp[15]); sub_cell[7].push_back(temp[21]);
		sub_cell[7].push_back(temp[24]); sub_cell[7].push_back(temp[17]);
		sub_cell[7].push_back(temp[6]); sub_cell[7].push_back(temp[14]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[26]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[22]]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[15]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[21]]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[24]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[17]]);
		sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[6]]); sub_cell_global[7].push_back(bss.CP_Local_to_Global[i][temp[14]]);

		//插入单元
		for (int j = 0; j < 8; j++)
		{
			bss.Local_BSplineSolid[i].add_cell(sub_cell[j]);
			bss.Global_BSplineSolid.add_cell(sub_cell_global[j]);
		}
	}

}

/*不考虑奇异情况*/
V3f ApproximationCC::cal_inter_point1(FaceHandle _f, VertexHandle _v)
{
	V3f newv;
	newv = newv + mesh->vertices_[_v] * 4;
	std::vector<EdgeHandle> fe = mesh->faces_[_f].edges_;
	for (std::vector<EdgeHandle>::iterator fe_it = fe.begin(); fe_it != fe.end(); fe_it++)
	{
		if (mesh->edges_[*fe_it].from_h == _v)
			newv = newv + mesh->vertices_[mesh->edges_[*fe_it].to_h] * 2;
		else if (mesh->edges_[*fe_it].to_h == _v)
			newv = newv + mesh->vertices_[mesh->edges_[*fe_it].from_h] * 2;
	}
	VertexHandle opv = mesh->find_op_vertex(_v, _f);
	newv = newv + mesh->vertices_[opv];
	newv = newv / 9;
	return newv;
}

V3f ApproximationCC::cal_inter_point1(CellHandle _c, VertexHandle _v)
{
	V3f newv;
	std::set<EdgeHandle, compare_OVM> ve = mesh->neighbor_v[_v];
	std::vector<VertexHandle> temp;
	temp.push_back(_v);
	int sum = 0;
	newv = newv + mesh->vertices_[_v] * 8;
	sum = sum + 8;
	for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++)
	{
		if (mesh->is_e_in_c(*ve_it, _c))
		{
			if (mesh->edges_[*ve_it].from_h == _v)
			{
				newv = newv + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 4;
				sum = sum + 4;
				temp.push_back(mesh->edges_[*ve_it].to_h);
			}
			else
			{
				newv = newv + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 4;
				sum = sum + 4;
				temp.push_back(mesh->edges_[*ve_it].from_h);
			}
		}
	}
	std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
	for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
	{
		if (mesh->is_f_in_c(*vf_it, _c))
		{
			VertexHandle fopv = mesh->find_op_vertex(_v, *vf_it);
			newv = newv + mesh->vertices_[fopv] * 2;
			sum = sum + 2;
			temp.push_back(fopv);
		}
	}
	std::vector<VertexHandle> cv = mesh->cells_[_c].vertex_;
	for (std::vector<VertexHandle>::iterator cv_it = cv.begin(); cv_it != cv.end(); cv_it++)
	{
		if (std::find(temp.begin(), temp.end(), *cv_it) == temp.end())
		{
			newv = newv + mesh->vertices_[*cv_it];
			sum = sum + 1;
			break;
		}
	}
	newv = newv / sum;
	return newv;
}

V3f ApproximationCC::solid_edge_point1(EdgeHandle _e, VertexHandle _v)
{
	if (mesh->is_bdy(_e))
	{
		V3f edp;
		std::set<FaceHandle, compare_OVM> ef = mesh->neighbor_e[_e];
		int n = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator ef_it = ef.begin(); ef_it != ef.end(); ef_it++)
		{
			if (mesh->is_bdy(*ef_it))
			{
				edp = edp + cal_inter_point1(*ef_it, _v);
				n++;
			}
		}
		edp = edp / n;
		return edp;
	}
	else
	{
		V3f edp;
		std::set<CellHandle> ec = mesh->neighbor_e_c(_e);
		int n = ec.size();
		for (std::set<CellHandle>::iterator ec_it = ec.begin(); ec_it != ec.end(); ec_it++)
		{
			edp = edp + cal_inter_point1(*ec_it, _v);
		}
		edp = edp / n;
		return edp;
	}
}

V3f ApproximationCC::solid_face_point1(FaceHandle _f, VertexHandle _v)
{
	if (mesh->is_bdy(_f))
	{
		V3f fp;
		fp = cal_inter_point1(_f, _v);
		return fp;
	}
	else
	{
		V3f fp;
		std::set<CellHandle, compare_OVM> fc = mesh->neighbor_f[_f];
		for (std::set<CellHandle, compare_OVM>::iterator fc_it = fc.begin(); fc_it != fc.end(); fc_it++)
		{
			fp = fp + cal_inter_point1(*fc_it, _v);
		}
		fp = fp / 2;
		return fp;
	}
}

V3f ApproximationCC::solid_corner_point1(VertexHandle _v)
{
	if (mesh->is_bdy(_v))
	{
		V3f cp;
		std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[_v];
		int n = 0;
		for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++)
		{
			if (mesh->is_bdy(*vf_it))
			{
				cp = cp + cal_inter_point1(*vf_it, _v);
				n++;
			}
		}
		cp = cp / n;
		return cp;
	}
	else
	{
		V3f cp;
		std::set<CellHandle, compare_OVM> vc = mesh->neighbor_v_c[_v];
		int n = vc.size();
		for (std::set<CellHandle, compare_OVM>::iterator vc_it = vc.begin(); vc_it != vc.end(); vc_it++)
		{
			cp = cp + cal_inter_point1(*vc_it, _v);
		}
		cp = cp / n;
		return cp;
	}
}

V3f ApproximationCC::solid_edge_point1(int cell_num, int edge_num, int vertex_num)
{
	return solid_edge_point1(mesh->cells_[cell_num].edges_[edge_num], mesh->cells_[cell_num].vertex_[vertex_num]);
}

V3f ApproximationCC::solid_face_point1(int cell_num, int face_num, int vertex_num)
{
	return solid_face_point1(mesh->cells_[cell_num].faces_[face_num], mesh->cells_[cell_num].vertex_[vertex_num]);
}

HexV3fMesh& ApproximationCC::CCapproximation2()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(solid_corner_point1(mesh->cells_[i].vertex_[vn]));
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(cal_inter_point1(CellHandle(i), mesh->cells_[i].vertex_[vn]));
		}
		//第一层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 9, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 9, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 1, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 7, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 1, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 7, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 8, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 8, 2));
		//第二层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 6, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 4, 2));
		//第三层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 2, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 6, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 4, 3));
		//第4层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 11, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 11, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 5, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 5, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 10, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 10, 3));
		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(27);
		/*第一层*/
		//第1个单元
		sub_cell[0].push_back(temp[5]); sub_cell[0].push_back(temp[16]); sub_cell[0].push_back(temp[19]); sub_cell[0].push_back(temp[18]);
		sub_cell[0].push_back(temp[28]); sub_cell[0].push_back(temp[29]); sub_cell[0].push_back(temp[13]); sub_cell[0].push_back(temp[32]);
		//第2个单元
		sub_cell[1].push_back(temp[16]); sub_cell[1].push_back(temp[17]); sub_cell[1].push_back(temp[20]); sub_cell[1].push_back(temp[19]);
		sub_cell[1].push_back(temp[29]); sub_cell[1].push_back(temp[30]); sub_cell[1].push_back(temp[14]); sub_cell[1].push_back(temp[13]);
		//第3个单元
		sub_cell[2].push_back(temp[17]); sub_cell[2].push_back(temp[6]); sub_cell[2].push_back(temp[21]); sub_cell[2].push_back(temp[20]);
		sub_cell[2].push_back(temp[30]); sub_cell[2].push_back(temp[31]); sub_cell[2].push_back(temp[33]); sub_cell[2].push_back(temp[14]);
		//第4个单元
		sub_cell[3].push_back(temp[18]); sub_cell[3].push_back(temp[19]); sub_cell[3].push_back(temp[23]); sub_cell[3].push_back(temp[22]);
		sub_cell[3].push_back(temp[32]); sub_cell[3].push_back(temp[13]); sub_cell[3].push_back(temp[9]); sub_cell[3].push_back(temp[34]);
		//第5个单元
		sub_cell[4].push_back(temp[19]); sub_cell[4].push_back(temp[20]); sub_cell[4].push_back(temp[24]); sub_cell[4].push_back(temp[23]);
		sub_cell[4].push_back(temp[13]); sub_cell[4].push_back(temp[14]); sub_cell[4].push_back(temp[10]); sub_cell[4].push_back(temp[9]);
		//第6个单元
		sub_cell[5].push_back(temp[20]); sub_cell[5].push_back(temp[21]); sub_cell[5].push_back(temp[25]); sub_cell[5].push_back(temp[24]);
		sub_cell[5].push_back(temp[14]); sub_cell[5].push_back(temp[33]); sub_cell[5].push_back(temp[35]); sub_cell[5].push_back(temp[10]);
		//第7个单元
		sub_cell[6].push_back(temp[22]); sub_cell[6].push_back(temp[23]); sub_cell[6].push_back(temp[26]); sub_cell[6].push_back(temp[1]);
		sub_cell[6].push_back(temp[34]); sub_cell[6].push_back(temp[9]); sub_cell[6].push_back(temp[37]); sub_cell[6].push_back(temp[36]);
		//第8个单元
		sub_cell[7].push_back(temp[23]); sub_cell[7].push_back(temp[24]); sub_cell[7].push_back(temp[27]); sub_cell[7].push_back(temp[26]);
		sub_cell[7].push_back(temp[9]); sub_cell[7].push_back(temp[10]); sub_cell[7].push_back(temp[38]); sub_cell[7].push_back(temp[37]);
		//第9个单元
		sub_cell[8].push_back(temp[24]); sub_cell[8].push_back(temp[25]); sub_cell[8].push_back(temp[2]); sub_cell[8].push_back(temp[27]);
		sub_cell[8].push_back(temp[10]); sub_cell[8].push_back(temp[35]); sub_cell[8].push_back(temp[39]); sub_cell[8].push_back(temp[38]);
		/*第二层*/
		//第10个单元
		sub_cell[9].push_back(temp[28]); sub_cell[9].push_back(temp[29]); sub_cell[9].push_back(temp[13]); sub_cell[9].push_back(temp[32]);
		sub_cell[9].push_back(temp[40]); sub_cell[9].push_back(temp[41]); sub_cell[9].push_back(temp[12]); sub_cell[9].push_back(temp[44]);
		//第11个单元
		sub_cell[10].push_back(temp[29]); sub_cell[10].push_back(temp[30]); sub_cell[10].push_back(temp[14]); sub_cell[10].push_back(temp[13]);
		sub_cell[10].push_back(temp[41]); sub_cell[10].push_back(temp[42]); sub_cell[10].push_back(temp[15]); sub_cell[10].push_back(temp[12]);
		//第12个单元
		sub_cell[11].push_back(temp[30]); sub_cell[11].push_back(temp[31]); sub_cell[11].push_back(temp[33]); sub_cell[11].push_back(temp[14]);
		sub_cell[11].push_back(temp[42]); sub_cell[11].push_back(temp[43]); sub_cell[11].push_back(temp[45]); sub_cell[11].push_back(temp[15]);
		//第13个单元
		sub_cell[12].push_back(temp[32]); sub_cell[12].push_back(temp[13]); sub_cell[12].push_back(temp[9]); sub_cell[12].push_back(temp[34]);
		sub_cell[12].push_back(temp[44]); sub_cell[12].push_back(temp[12]); sub_cell[12].push_back(temp[8]); sub_cell[12].push_back(temp[46]);
		//第14个单元
		sub_cell[13].push_back(temp[13]); sub_cell[13].push_back(temp[14]); sub_cell[13].push_back(temp[10]); sub_cell[13].push_back(temp[9]);
		sub_cell[13].push_back(temp[12]); sub_cell[13].push_back(temp[15]); sub_cell[13].push_back(temp[11]); sub_cell[13].push_back(temp[8]);
		//第15个单元
		sub_cell[14].push_back(temp[14]); sub_cell[14].push_back(temp[33]); sub_cell[14].push_back(temp[35]); sub_cell[14].push_back(temp[10]);
		sub_cell[14].push_back(temp[15]); sub_cell[14].push_back(temp[45]); sub_cell[14].push_back(temp[47]); sub_cell[14].push_back(temp[11]);
		//第16个单元
		sub_cell[15].push_back(temp[34]); sub_cell[15].push_back(temp[9]); sub_cell[15].push_back(temp[37]); sub_cell[15].push_back(temp[36]);
		sub_cell[15].push_back(temp[46]); sub_cell[15].push_back(temp[8]); sub_cell[15].push_back(temp[49]); sub_cell[15].push_back(temp[48]);
		//第17个单元
		sub_cell[16].push_back(temp[9]); sub_cell[16].push_back(temp[10]); sub_cell[16].push_back(temp[38]); sub_cell[16].push_back(temp[37]);
		sub_cell[16].push_back(temp[8]); sub_cell[16].push_back(temp[11]); sub_cell[16].push_back(temp[50]); sub_cell[16].push_back(temp[49]);
		//第18个单元
		sub_cell[17].push_back(temp[10]); sub_cell[17].push_back(temp[35]); sub_cell[17].push_back(temp[39]); sub_cell[17].push_back(temp[38]);
		sub_cell[17].push_back(temp[11]); sub_cell[17].push_back(temp[47]); sub_cell[17].push_back(temp[51]); sub_cell[17].push_back(temp[50]);
		/*第三层*/
		//第19个单元
		sub_cell[18].push_back(temp[40]); sub_cell[18].push_back(temp[41]); sub_cell[18].push_back(temp[12]); sub_cell[18].push_back(temp[44]);
		sub_cell[18].push_back(temp[4]); sub_cell[18].push_back(temp[52]); sub_cell[18].push_back(temp[55]); sub_cell[18].push_back(temp[54]);
		//第20个单元
		sub_cell[19].push_back(temp[41]); sub_cell[19].push_back(temp[42]); sub_cell[19].push_back(temp[15]); sub_cell[19].push_back(temp[12]);
		sub_cell[19].push_back(temp[52]); sub_cell[19].push_back(temp[53]); sub_cell[19].push_back(temp[56]); sub_cell[19].push_back(temp[55]);
		//第21个单元
		sub_cell[20].push_back(temp[42]); sub_cell[20].push_back(temp[43]); sub_cell[20].push_back(temp[45]); sub_cell[20].push_back(temp[15]);
		sub_cell[20].push_back(temp[53]); sub_cell[20].push_back(temp[7]); sub_cell[20].push_back(temp[57]); sub_cell[20].push_back(temp[56]);
		//第22个单元
		sub_cell[21].push_back(temp[44]); sub_cell[21].push_back(temp[12]); sub_cell[21].push_back(temp[8]); sub_cell[21].push_back(temp[46]);
		sub_cell[21].push_back(temp[54]); sub_cell[21].push_back(temp[55]); sub_cell[21].push_back(temp[59]); sub_cell[21].push_back(temp[58]);
		//第23个单元
		sub_cell[22].push_back(temp[12]); sub_cell[22].push_back(temp[15]); sub_cell[22].push_back(temp[11]); sub_cell[22].push_back(temp[8]);
		sub_cell[22].push_back(temp[55]); sub_cell[22].push_back(temp[56]); sub_cell[22].push_back(temp[60]); sub_cell[22].push_back(temp[59]);
		//第24个单元
		sub_cell[23].push_back(temp[15]); sub_cell[23].push_back(temp[45]); sub_cell[23].push_back(temp[47]); sub_cell[23].push_back(temp[11]);
		sub_cell[23].push_back(temp[56]); sub_cell[23].push_back(temp[57]); sub_cell[23].push_back(temp[61]); sub_cell[23].push_back(temp[60]);
		//第25个单元
		sub_cell[24].push_back(temp[46]); sub_cell[24].push_back(temp[8]); sub_cell[24].push_back(temp[49]); sub_cell[24].push_back(temp[48]);
		sub_cell[24].push_back(temp[58]); sub_cell[24].push_back(temp[59]); sub_cell[24].push_back(temp[62]); sub_cell[24].push_back(temp[0]);
		//第26个单元
		sub_cell[25].push_back(temp[8]); sub_cell[25].push_back(temp[11]); sub_cell[25].push_back(temp[50]); sub_cell[25].push_back(temp[49]);
		sub_cell[25].push_back(temp[59]); sub_cell[25].push_back(temp[60]); sub_cell[25].push_back(temp[63]); sub_cell[25].push_back(temp[62]);
		//第27个单元
		sub_cell[26].push_back(temp[11]); sub_cell[26].push_back(temp[47]); sub_cell[26].push_back(temp[51]); sub_cell[26].push_back(temp[50]);
		sub_cell[26].push_back(temp[60]); sub_cell[26].push_back(temp[61]); sub_cell[26].push_back(temp[3]); sub_cell[26].push_back(temp[63]);

		//插入单元
		for (int j = 0; j < 27; j++)
		{
			newmesh->add_cell(sub_cell[j]);
		}
	}
	return *newmesh;
}

HexV3fMesh& ApproximationCC::CCapproximation3()
{
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(solid_corner_point1(mesh->cells_[i].vertex_[vn]));
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = newmesh->add_vertex(cal_inter_point1(CellHandle(i), mesh->cells_[i].vertex_[vn]));
		}
		//第一层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 9, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 9, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 1, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 7, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 1, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 2, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 7, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 8, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 8, 2));
		//第二层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 2, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 6));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 6, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 5));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 6));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 2));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 0, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 1));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 2));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 4, 2));
		//第三层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 2, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 4, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 6, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 1, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 0, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 5, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 4, 3));
		//第4层
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 11, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 11, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 4));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 7));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 5, 7));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_face_point1(i, 3, 3));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 5, 3));

		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 10, 0));
		temp[tempsize++] = newmesh->add_vertex(solid_edge_point1(i, 10, 3));

		//构建Bezier体的数据
		std::vector<std::vector<std::vector<V3f>>> sc(4);
		std::vector<std::vector<V3f>> sf(4);
		std::vector<V3f> sl(4);
		//第一层
		sl[0] = newmesh->vertices_[temp[5]]; sl[1] = newmesh->vertices_[temp[18]];
		sl[2] = newmesh->vertices_[temp[22]]; sl[3] = newmesh->vertices_[temp[1]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[16]]; sl[1] = newmesh->vertices_[temp[19]];
		sl[2] = newmesh->vertices_[temp[23]]; sl[3] = newmesh->vertices_[temp[26]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[17]]; sl[1] = newmesh->vertices_[temp[20]];
		sl[2] = newmesh->vertices_[temp[24]]; sl[3] = newmesh->vertices_[temp[27]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[6]]; sl[1] = newmesh->vertices_[temp[21]];
		sl[2] = newmesh->vertices_[temp[25]]; sl[3] = newmesh->vertices_[temp[2]];
		sf[3] = sl;
		sc[0] = sf;
		//第二层
		sl[0] = newmesh->vertices_[temp[28]]; sl[1] = newmesh->vertices_[temp[32]];
		sl[2] = newmesh->vertices_[temp[34]]; sl[3] = newmesh->vertices_[temp[36]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[29]]; sl[1] = newmesh->vertices_[temp[13]];
		sl[2] = newmesh->vertices_[temp[9]]; sl[3] = newmesh->vertices_[temp[37]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[30]]; sl[1] = newmesh->vertices_[temp[14]];
		sl[2] = newmesh->vertices_[temp[10]]; sl[3] = newmesh->vertices_[temp[38]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[31]]; sl[1] = newmesh->vertices_[temp[33]];
		sl[2] = newmesh->vertices_[temp[35]]; sl[3] = newmesh->vertices_[temp[39]];
		sf[3] = sl;
		sc[1] = sf;
		//第三层
		sl[0] = newmesh->vertices_[temp[40]]; sl[1] = newmesh->vertices_[temp[44]];
		sl[2] = newmesh->vertices_[temp[46]]; sl[3] = newmesh->vertices_[temp[48]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[41]]; sl[1] = newmesh->vertices_[temp[12]];
		sl[2] = newmesh->vertices_[temp[8]]; sl[3] = newmesh->vertices_[temp[49]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[42]]; sl[1] = newmesh->vertices_[temp[15]];
		sl[2] = newmesh->vertices_[temp[11]]; sl[3] = newmesh->vertices_[temp[50]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[43]]; sl[1] = newmesh->vertices_[temp[45]];
		sl[2] = newmesh->vertices_[temp[47]]; sl[3] = newmesh->vertices_[temp[51]];
		sf[3] = sl;
		sc[2] = sf;
		//第四层
		sl[0] = newmesh->vertices_[temp[4]]; sl[1] = newmesh->vertices_[temp[54]];
		sl[2] = newmesh->vertices_[temp[58]]; sl[3] = newmesh->vertices_[temp[0]];
		sf[0] = sl;
		sl[0] = newmesh->vertices_[temp[52]]; sl[1] = newmesh->vertices_[temp[55]];
		sl[2] = newmesh->vertices_[temp[59]]; sl[3] = newmesh->vertices_[temp[62]];
		sf[1] = sl;
		sl[0] = newmesh->vertices_[temp[53]]; sl[1] = newmesh->vertices_[temp[56]];
		sl[2] = newmesh->vertices_[temp[60]]; sl[3] = newmesh->vertices_[temp[63]];
		sf[2] = sl;
		sl[0] = newmesh->vertices_[temp[7]]; sl[1] = newmesh->vertices_[temp[57]];
		sl[2] = newmesh->vertices_[temp[61]]; sl[3] = newmesh->vertices_[temp[3]];
		sf[3] = sl;
		sc[3] = sf;

		SolidData.push_back(sc);
	}
	return *newmesh;
}

std::vector<HexV3fMesh>& ApproximationCC::CCapproximationDAT1()
{
	uint64_t cell_num = mesh->cells_.size();
	vmesh.resize(cell_num);
	for (uint64_t i = 0; i < cell_num; i++)
	{
		VertexHandle temp[64];
		int tempsize = 0;
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = vmesh[i].add_vertex(solid_corner_point1(mesh->cells_[i].vertex_[vn]));
		}
		for (int vn = 0; vn < 8; vn++)
		{
			temp[tempsize++] = vmesh[i].add_vertex(cal_inter_point1(CellHandle(i), mesh->cells_[i].vertex_[vn]));
		}
		//第一层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 9, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 9, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 1, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 2, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 2, 6));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 7, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 1, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 2, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 2, 2));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 7, 2));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 8, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 8, 2));
		//第二层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 2, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 4, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 4, 6));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 6, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 0, 5));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 1, 6));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 0, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 1, 2));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 0, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 5, 1));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 5, 2));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 4, 2));
		//第三层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 2, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 4, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 4, 7));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 6, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 0, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 1, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 0, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 1, 3));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 0, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 5, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 5, 3));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 4, 3));
		//第4层
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 11, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 11, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 3, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 3, 4));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 3, 7));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 5, 7));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 3, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 3, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_face_point1(i, 3, 3));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 5, 3));

		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 10, 0));
		temp[tempsize++] = vmesh[i].add_vertex(solid_edge_point1(i, 10, 3));
		//接下来 插入分割后的子单元
		std::vector<std::vector<VertexHandle>> sub_cell(27);
		/*第一层*/
		//第1个单元
		sub_cell[0].push_back(temp[5]); sub_cell[0].push_back(temp[16]); sub_cell[0].push_back(temp[19]); sub_cell[0].push_back(temp[18]);
		sub_cell[0].push_back(temp[28]); sub_cell[0].push_back(temp[29]); sub_cell[0].push_back(temp[13]); sub_cell[0].push_back(temp[32]);
		//第2个单元
		sub_cell[1].push_back(temp[16]); sub_cell[1].push_back(temp[17]); sub_cell[1].push_back(temp[20]); sub_cell[1].push_back(temp[19]);
		sub_cell[1].push_back(temp[29]); sub_cell[1].push_back(temp[30]); sub_cell[1].push_back(temp[14]); sub_cell[1].push_back(temp[13]);
		//第3个单元
		sub_cell[2].push_back(temp[17]); sub_cell[2].push_back(temp[6]); sub_cell[2].push_back(temp[21]); sub_cell[2].push_back(temp[20]);
		sub_cell[2].push_back(temp[30]); sub_cell[2].push_back(temp[31]); sub_cell[2].push_back(temp[33]); sub_cell[2].push_back(temp[14]);
		//第4个单元
		sub_cell[3].push_back(temp[18]); sub_cell[3].push_back(temp[19]); sub_cell[3].push_back(temp[23]); sub_cell[3].push_back(temp[22]);
		sub_cell[3].push_back(temp[32]); sub_cell[3].push_back(temp[13]); sub_cell[3].push_back(temp[9]); sub_cell[3].push_back(temp[34]);
		//第5个单元
		sub_cell[4].push_back(temp[19]); sub_cell[4].push_back(temp[20]); sub_cell[4].push_back(temp[24]); sub_cell[4].push_back(temp[23]);
		sub_cell[4].push_back(temp[13]); sub_cell[4].push_back(temp[14]); sub_cell[4].push_back(temp[10]); sub_cell[4].push_back(temp[9]);
		//第6个单元
		sub_cell[5].push_back(temp[20]); sub_cell[5].push_back(temp[21]); sub_cell[5].push_back(temp[25]); sub_cell[5].push_back(temp[24]);
		sub_cell[5].push_back(temp[14]); sub_cell[5].push_back(temp[33]); sub_cell[5].push_back(temp[35]); sub_cell[5].push_back(temp[10]);
		//第7个单元
		sub_cell[6].push_back(temp[22]); sub_cell[6].push_back(temp[23]); sub_cell[6].push_back(temp[26]); sub_cell[6].push_back(temp[1]);
		sub_cell[6].push_back(temp[34]); sub_cell[6].push_back(temp[9]); sub_cell[6].push_back(temp[37]); sub_cell[6].push_back(temp[36]);
		//第8个单元
		sub_cell[7].push_back(temp[23]); sub_cell[7].push_back(temp[24]); sub_cell[7].push_back(temp[27]); sub_cell[7].push_back(temp[26]);
		sub_cell[7].push_back(temp[9]); sub_cell[7].push_back(temp[10]); sub_cell[7].push_back(temp[38]); sub_cell[7].push_back(temp[37]);
		//第9个单元
		sub_cell[8].push_back(temp[24]); sub_cell[8].push_back(temp[25]); sub_cell[8].push_back(temp[2]); sub_cell[8].push_back(temp[27]);
		sub_cell[8].push_back(temp[10]); sub_cell[8].push_back(temp[35]); sub_cell[8].push_back(temp[39]); sub_cell[8].push_back(temp[38]);
		/*第二层*/
		//第10个单元
		sub_cell[9].push_back(temp[28]); sub_cell[9].push_back(temp[29]); sub_cell[9].push_back(temp[13]); sub_cell[9].push_back(temp[32]);
		sub_cell[9].push_back(temp[40]); sub_cell[9].push_back(temp[41]); sub_cell[9].push_back(temp[12]); sub_cell[9].push_back(temp[44]);
		//第11个单元
		sub_cell[10].push_back(temp[29]); sub_cell[10].push_back(temp[30]); sub_cell[10].push_back(temp[14]); sub_cell[10].push_back(temp[13]);
		sub_cell[10].push_back(temp[41]); sub_cell[10].push_back(temp[42]); sub_cell[10].push_back(temp[15]); sub_cell[10].push_back(temp[12]);
		//第12个单元
		sub_cell[11].push_back(temp[30]); sub_cell[11].push_back(temp[31]); sub_cell[11].push_back(temp[33]); sub_cell[11].push_back(temp[14]);
		sub_cell[11].push_back(temp[42]); sub_cell[11].push_back(temp[43]); sub_cell[11].push_back(temp[45]); sub_cell[11].push_back(temp[15]);
		//第13个单元
		sub_cell[12].push_back(temp[32]); sub_cell[12].push_back(temp[13]); sub_cell[12].push_back(temp[9]); sub_cell[12].push_back(temp[34]);
		sub_cell[12].push_back(temp[44]); sub_cell[12].push_back(temp[12]); sub_cell[12].push_back(temp[8]); sub_cell[12].push_back(temp[46]);
		//第14个单元
		sub_cell[13].push_back(temp[13]); sub_cell[13].push_back(temp[14]); sub_cell[13].push_back(temp[10]); sub_cell[13].push_back(temp[9]);
		sub_cell[13].push_back(temp[12]); sub_cell[13].push_back(temp[15]); sub_cell[13].push_back(temp[11]); sub_cell[13].push_back(temp[8]);
		//第15个单元
		sub_cell[14].push_back(temp[14]); sub_cell[14].push_back(temp[33]); sub_cell[14].push_back(temp[35]); sub_cell[14].push_back(temp[10]);
		sub_cell[14].push_back(temp[15]); sub_cell[14].push_back(temp[45]); sub_cell[14].push_back(temp[47]); sub_cell[14].push_back(temp[11]);
		//第16个单元
		sub_cell[15].push_back(temp[34]); sub_cell[15].push_back(temp[9]); sub_cell[15].push_back(temp[37]); sub_cell[15].push_back(temp[36]);
		sub_cell[15].push_back(temp[46]); sub_cell[15].push_back(temp[8]); sub_cell[15].push_back(temp[49]); sub_cell[15].push_back(temp[48]);
		//第17个单元
		sub_cell[16].push_back(temp[9]); sub_cell[16].push_back(temp[10]); sub_cell[16].push_back(temp[38]); sub_cell[16].push_back(temp[37]);
		sub_cell[16].push_back(temp[8]); sub_cell[16].push_back(temp[11]); sub_cell[16].push_back(temp[50]); sub_cell[16].push_back(temp[49]);
		//第18个单元
		sub_cell[17].push_back(temp[10]); sub_cell[17].push_back(temp[35]); sub_cell[17].push_back(temp[39]); sub_cell[17].push_back(temp[38]);
		sub_cell[17].push_back(temp[11]); sub_cell[17].push_back(temp[47]); sub_cell[17].push_back(temp[51]); sub_cell[17].push_back(temp[50]);
		/*第三层*/
		//第19个单元
		sub_cell[18].push_back(temp[40]); sub_cell[18].push_back(temp[41]); sub_cell[18].push_back(temp[12]); sub_cell[18].push_back(temp[44]);
		sub_cell[18].push_back(temp[4]); sub_cell[18].push_back(temp[52]); sub_cell[18].push_back(temp[55]); sub_cell[18].push_back(temp[54]);
		//第20个单元
		sub_cell[19].push_back(temp[41]); sub_cell[19].push_back(temp[42]); sub_cell[19].push_back(temp[15]); sub_cell[19].push_back(temp[12]);
		sub_cell[19].push_back(temp[52]); sub_cell[19].push_back(temp[53]); sub_cell[19].push_back(temp[56]); sub_cell[19].push_back(temp[55]);
		//第21个单元
		sub_cell[20].push_back(temp[42]); sub_cell[20].push_back(temp[43]); sub_cell[20].push_back(temp[45]); sub_cell[20].push_back(temp[15]);
		sub_cell[20].push_back(temp[53]); sub_cell[20].push_back(temp[7]); sub_cell[20].push_back(temp[57]); sub_cell[20].push_back(temp[56]);
		//第22个单元
		sub_cell[21].push_back(temp[44]); sub_cell[21].push_back(temp[12]); sub_cell[21].push_back(temp[8]); sub_cell[21].push_back(temp[46]);
		sub_cell[21].push_back(temp[54]); sub_cell[21].push_back(temp[55]); sub_cell[21].push_back(temp[59]); sub_cell[21].push_back(temp[58]);
		//第23个单元
		sub_cell[22].push_back(temp[12]); sub_cell[22].push_back(temp[15]); sub_cell[22].push_back(temp[11]); sub_cell[22].push_back(temp[8]);
		sub_cell[22].push_back(temp[55]); sub_cell[22].push_back(temp[56]); sub_cell[22].push_back(temp[60]); sub_cell[22].push_back(temp[59]);
		//第24个单元
		sub_cell[23].push_back(temp[15]); sub_cell[23].push_back(temp[45]); sub_cell[23].push_back(temp[47]); sub_cell[23].push_back(temp[11]);
		sub_cell[23].push_back(temp[56]); sub_cell[23].push_back(temp[57]); sub_cell[23].push_back(temp[61]); sub_cell[23].push_back(temp[60]);
		//第25个单元
		sub_cell[24].push_back(temp[46]); sub_cell[24].push_back(temp[8]); sub_cell[24].push_back(temp[49]); sub_cell[24].push_back(temp[48]);
		sub_cell[24].push_back(temp[58]); sub_cell[24].push_back(temp[59]); sub_cell[24].push_back(temp[62]); sub_cell[24].push_back(temp[0]);
		//第26个单元
		sub_cell[25].push_back(temp[8]); sub_cell[25].push_back(temp[11]); sub_cell[25].push_back(temp[50]); sub_cell[25].push_back(temp[49]);
		sub_cell[25].push_back(temp[59]); sub_cell[25].push_back(temp[60]); sub_cell[25].push_back(temp[63]); sub_cell[25].push_back(temp[62]);
		//第27个单元
		sub_cell[26].push_back(temp[11]); sub_cell[26].push_back(temp[47]); sub_cell[26].push_back(temp[51]); sub_cell[26].push_back(temp[50]);
		sub_cell[26].push_back(temp[60]); sub_cell[26].push_back(temp[61]); sub_cell[26].push_back(temp[3]); sub_cell[26].push_back(temp[63]);

		//插入单元
		for (int j = 0; j < 27; j++)
		{
			vmesh[i].add_cell(sub_cell[j]);
		}
	}
	return vmesh;
}

//-----TwoNSubdivision-----//
void TwoNSubdivision::cal_edgepoint_withn()
{
	edgepoint.resize(iteration_num + 1, std::vector<V3f>(mesh->edges_.size(), V3f(0, 0, 0)));
	std::vector<std::vector<V3f>> D(iteration_num + 1, std::vector<V3f>(mesh->edges_.size(), V3f(0, 0, 0)));
	for (int i = 1; i <= iteration_num; i++)
	{
		if (i == 1)
		{
			for (int edge_num = 0; edge_num < mesh->edges_.size(); edge_num++)
			{
				/*if (!mesh->is_bdy(EdgeHandle(edge_num)))
					continue;*/
				edgepoint[i][edge_num] = cal_edge_midv(EdgeHandle(edge_num));
				D[i][edge_num] = edgepoint[i][edge_num] - edgepoint[i - 1][edge_num];
			}
		}
		else
		{
			for (int edge_num = 0; edge_num < mesh->edges_.size(); edge_num++)
			{
				if (!mesh->is_bdy(EdgeHandle(edge_num)))
				{
					VertexHandle vfrom = mesh->edges_[edge_num].from_h;
					VertexHandle vto = mesh->edges_[edge_num].to_h;
					std::set<EdgeHandle, compare_OVM> vfrom_e = mesh->neighbor_v[vfrom];
					std::set<EdgeHandle, compare_OVM> vto_e = mesh->neighbor_v[vto];
					int Nvfrom = vfrom_e.size();
					int Nvto = vto_e.size();
					std::vector<EdgeHandle> vfrom_e_sort;
					std::vector<EdgeHandle> vto_e_sort;//放置排序之后的边
					vfrom_e_sort.push_back(EdgeHandle(edge_num));
					vto_e_sort.push_back(EdgeHandle(edge_num));
					EdgeHandle from_e_first;//用于标记一个点的排序之后的邻接边的除了本边之外的第一条边
					V3f tempfrom, tempto;
					int temp_id = 0;
					EdgeHandle opfrom, opto;//用于记录与初始边不在一个块内的边
					for (std::set<EdgeHandle, compare_OVM>::iterator vfrom_e_it = vfrom_e.begin(); vfrom_e_it != vfrom_e.end(); vfrom_e_it++)
					{
						if (!mesh->is_two_e_in_same_cell(EdgeHandle(edge_num), *vfrom_e_it))
							opfrom = *vfrom_e_it;
					}
					for (std::set<EdgeHandle, compare_OVM>::iterator vto_e_it = vto_e.begin(); vto_e_it != vto_e.end(); vto_e_it++)
					{
						if (!mesh->is_two_e_in_same_cell(EdgeHandle(edge_num), *vto_e_it))
							opto = *vto_e_it;
					}
					//进行两个点的邻接边的排序
					bool check = false;//判断时候排序完成
					while (check == false)
					{
						for (std::set<EdgeHandle, compare_OVM>::iterator vfrom_e_it = vfrom_e.begin(); vfrom_e_it != vfrom_e.end(); vfrom_e_it++)
						{
							if (mesh->is_two_e_in_same_cell(*(vfrom_e_sort.end() - 1), *vfrom_e_it) && *vfrom_e_it != *(vfrom_e_sort.end() - 1)
								&& (*vfrom_e_it) != opfrom)
							{
								if (vfrom_e_sort.size() == Nvfrom - 1 && Nvfrom % 2 == 0)
								{
									vfrom_e_sort.push_back(opfrom);
									break;
								}
								else if (vfrom_e_sort.size() > 1)//要与上一条邻接边不同
								{
									if (std::find(vfrom_e_sort.begin(), vfrom_e_sort.end(), *vfrom_e_it) == vfrom_e_sort.end())
									{
										vfrom_e_sort.push_back(*vfrom_e_it);
										break;
									}
								}
								else
								{
									vfrom_e_sort.push_back(*vfrom_e_it);
									from_e_first = *vfrom_e_it;
									break;
								}
							}
						}
						if (vfrom_e_sort.size() == Nvfrom)
							check = true;
					}
					for (std::vector<EdgeHandle>::iterator vfrom_e_sort_it = vfrom_e_sort.begin(); vfrom_e_sort_it != vfrom_e_sort.end();
						vfrom_e_sort_it++)
					{
						double a_from = (4 / Nvfrom * 1.0)*cos(2 * M_PI*temp_id / Nvfrom * 1.0);
						temp_id++;
						tempfrom = tempfrom + D[i - 1][*vfrom_e_sort_it] * a_from;
					}
					//排序第二个点的邻接边
					check = false;
					while (check == false)
					{
						for (std::set<EdgeHandle, compare_OVM>::iterator vto_e_it = vto_e.begin(); vto_e_it != vto_e.end(); vto_e_it++)
						{
							if (mesh->is_two_e_in_same_cell(*(vto_e_sort.end() - 1), *vto_e_it) && *vto_e_it != *(vto_e_sort.end() - 1)
								&& (*vto_e_it) != opto)
							{
								if (vto_e_sort.size() == Nvto - 1 && Nvto % 2 == 0)
								{
									vto_e_sort.push_back(opto);
									break;
								}
								else if (vto_e_sort.size() > 1)//要与上一条邻接边不同
								{
									if (std::find(vto_e_sort.begin(), vto_e_sort.end(), *vto_e_it) == vto_e_sort.end())
									{
										vto_e_sort.push_back(*vto_e_it);
										break;
									}
								}
								else
								{
									if (mesh->is_two_e_in_same_cell(from_e_first, *vto_e_it))
									{
										vto_e_sort.push_back(*vto_e_it);
										break;
									}
								}
							}
						}
						if (vto_e_sort.size() == Nvto)
							check = true;
					}
					temp_id = 0;
					for (std::vector<EdgeHandle>::iterator vto_e_sort_it = vto_e_sort.begin(); vto_e_sort_it != vto_e_sort.end(); vto_e_sort_it++)
					{
						double a_to = (4 / Nvto * 1.0)*cos(2 * M_PI*temp_id / Nvto * 1.0);
						temp_id++;
						tempto = tempto + D[i - 1][*vto_e_sort_it] * a_to;
					}
					edgepoint[i][edge_num] = edgepoint[i - 1][edge_num] + (tempfrom + tempto)*((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
					D[i][edge_num] = edgepoint[i][edge_num] - edgepoint[i - 1][edge_num];
				}
				else
				{
					VertexHandle vfrom = mesh->edges_[edge_num].from_h;
					VertexHandle vto = mesh->edges_[edge_num].to_h;
					std::set<EdgeHandle> vfrom_e = mesh->neighbor_v_bdy(vfrom);
					std::set<EdgeHandle> vto_e = mesh->neighbor_v_bdy(vto);
					int Nvfrom = vfrom_e.size();
					int Nvto = vto_e.size();
					std::vector<EdgeHandle> vfrom_e_sort;
					std::vector<EdgeHandle> vto_e_sort;//放置排序之后的边
					vfrom_e_sort.push_back(EdgeHandle(edge_num));
					vto_e_sort.push_back(EdgeHandle(edge_num));
					EdgeHandle from_e_first;//用于标记一个点的排序之后的邻接边的除了本边之外的第一条边
					V3f tempfrom, tempto;
					int temp_id = 0;
					//进行两个点的邻接边的排序
					bool check = false;//判断时候排序完成
					while (check == false)
					{
						for (std::set<EdgeHandle>::iterator vfrom_e_it = vfrom_e.begin(); vfrom_e_it != vfrom_e.end(); vfrom_e_it++)
						{
							//EdgeHandle edgetemp = *(vfrom_e_sort.end() - 1);
							//EdgeHandle edgetemp1 = *vfrom_e_it;
							if (mesh->is_two_e_in_same_face(*(vfrom_e_sort.end() - 1), *vfrom_e_it) && *vfrom_e_it != *(vfrom_e_sort.end() - 1))
							{
								if (vfrom_e_sort.size() > 1)//要与上一条邻接边不同
								{
									if (*vfrom_e_it != *(vfrom_e_sort.end() - 2)  /*std::find(vfrom_e_sort.begin(), vfrom_e_sort.end(),*vfrom_e_it) == vfrom_e_sort.end()*/)
									{
										vfrom_e_sort.push_back(*vfrom_e_it);
										break;
									}
								}
								else
								{
									vfrom_e_sort.push_back(*vfrom_e_it);
									from_e_first = *vfrom_e_it;
									break;
								}
							}
						}
						if (vfrom_e_sort.size() == Nvfrom)
							check = true;
					}
					for (std::vector<EdgeHandle>::iterator vfrom_e_sort_it = vfrom_e_sort.begin(); vfrom_e_sort_it != vfrom_e_sort.end();
						vfrom_e_sort_it++)
					{
						double a_from = (4 / Nvfrom * 1.0)*cos(2 * M_PI*temp_id / Nvfrom * 1.0);
						temp_id++;
						tempfrom = tempfrom + D[i - 1][*vfrom_e_sort_it] * a_from;
					}
					//排序第二个点的邻接边
					check = false;
					while (check == false)
					{
						for (std::set<EdgeHandle>::iterator vto_e_it = vto_e.begin(); vto_e_it != vto_e.end(); vto_e_it++)
						{
							if (mesh->is_two_e_in_same_face(*(vto_e_sort.end() - 1), *vto_e_it) && *vto_e_it != *(vto_e_sort.end() - 1))
							{
								if (vto_e_sort.size() > 1)//要与上一条邻接边不同
								{
									if (*vto_e_it != *(vto_e_sort.end() - 2)  /*std::find(vto_e_sort.begin(), vto_e_sort.end(), *vto_e_it) == vto_e_sort.end()*/)
									{
										vto_e_sort.push_back(*vto_e_it);
										break;
									}
								}
								else
								{
									if (mesh->is_two_e_in_same_face(from_e_first, *vto_e_it))
									{
										vto_e_sort.push_back(*vto_e_it);
										break;
									}
								}
							}
						}
						if (vto_e_sort.size() == Nvto)
							check = true;
					}
					temp_id = 0;
					for (std::vector<EdgeHandle>::iterator vto_e_sort_it = vto_e_sort.begin(); vto_e_sort_it != vto_e_sort.end(); vto_e_sort_it++)
					{
						double a_to = (4 / Nvto * 1.0)*cos(2 * M_PI*temp_id / Nvto * 1.0);
						temp_id++;
						tempto = tempto + D[i - 1][*vto_e_sort_it] * a_to;
					}
					/*V3f t1 = tempfrom + tempto;
					double t2 = (2 * i - 1) / (8 * i * 1.0);
					V3f temp = t1*t2;*/
					edgepoint[i][edge_num] = edgepoint[i - 1][edge_num] + (tempfrom + tempto)*((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
					D[i][edge_num] = edgepoint[i][edge_num] - edgepoint[i - 1][edge_num];
				}
			}
		}
	}
}

V3f TwoNSubdivision::cal_facepoint_withn(FaceHandle _f)
{
	V3f face_point;
	std::vector<V3f> f_direction1 = select_face_line(_f, mesh->faces_[_f].edges_[0]);
	int f_direction1_num = f_direction1.size();
	std::vector<V3f> f_direction2 = select_face_line(_f, mesh->faces_[_f].edges_[1]);
	int f_direction2_num = f_direction2.size();
	//计算第一个方向
	std::vector<std::vector<V3f>> facepoint1(iteration_num + 1, std::vector<V3f>(f_direction1_num, V3f(0, 0, 0)));
	std::vector<std::vector<V3f>> D1(iteration_num + 1, std::vector<V3f>(f_direction1_num, V3f(0, 0, 0)));
	for (int i = 1; i <= iteration_num; i++)
	{
		if (i == 1)
		{
			for (int face_num = 0; face_num < f_direction1_num; face_num++)
			{
				facepoint1[i][face_num] = (f_direction1[face_num] + f_direction1[(face_num + 1) % f_direction1_num]) / 2;
				D1[i][face_num] = facepoint1[i][face_num] - facepoint1[i - 1][face_num];
			}
		}
		else
		{
			for (int face_num = 0; face_num < f_direction1_num; face_num++)
			{
				facepoint1[i][face_num] = facepoint1[i - 1][face_num] + (D1[i - 1][face_num] * 2
					- D1[i - 1][(face_num - 1 + f_direction1_num) % f_direction1_num] - D1[i - 1][(face_num + 1) % f_direction1_num])
					*((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
				D1[i][face_num] = facepoint1[i][face_num] - facepoint1[i - 1][face_num];
			}
		}
	}
	//计算第二个方向
	std::vector<std::vector<V3f>> facepoint2(iteration_num + 1, std::vector<V3f>(f_direction2_num, V3f(0, 0, 0)));
	std::vector<std::vector<V3f>> D2(iteration_num + 1, std::vector<V3f>(f_direction2_num, V3f(0, 0, 0)));
	for (int i = 1; i <= iteration_num; i++)
	{
		if (i == 1)
		{
			for (int face_num = 0; face_num < f_direction2_num; face_num++)
			{
				facepoint2[i][face_num] = (f_direction2[face_num] + f_direction2[(face_num + 1) % f_direction2_num]) / 2;
				D2[i][face_num] = facepoint2[i][face_num] - facepoint2[i - 1][face_num];
			}
		}
		else
		{
			for (int face_num = 0; face_num < f_direction2_num; face_num++)
			{
				facepoint2[i][face_num] = facepoint2[i - 1][face_num] + (D2[i - 1][face_num] * 2
					- D2[i - 1][(face_num - 1 + f_direction2_num) % f_direction2_num] - D2[i - 1][(face_num + 1) % f_direction2_num])
					*((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
				D2[i][face_num] = facepoint2[i][face_num] - facepoint2[i - 1][face_num];
			}
		}
	}
	face_point = (facepoint1[iteration_num][f_direction1_num - 1] + facepoint2[iteration_num][f_direction2_num - 1]) / 2;
	return face_point;
}

V3f TwoNSubdivision::cal_facepoint_withn(FaceHandle _f, int n)
{
	V3f face_point;
	std::vector<V3f> f_direction1 = select_face_line(_f, mesh->faces_[_f].edges_[0]);
	int f_direction1_num = f_direction1.size();
	std::vector<V3f> f_direction2 = select_face_line(_f, mesh->faces_[_f].edges_[1]);
	int f_direction2_num = f_direction2.size();
	//计算第一个方向
	std::vector<std::vector<V3f>> facepoint1(n + 1, std::vector<V3f>(f_direction1_num, V3f(0, 0, 0)));
	std::vector<std::vector<V3f>> D1(n + 1, std::vector<V3f>(f_direction1_num, V3f(0, 0, 0)));
	for (int i = 1; i <= n; i++)
	{
		if (i == 1)
		{
			for (int face_num = 0; face_num < f_direction1_num; face_num++)
			{
				facepoint1[i][face_num] = (f_direction1[face_num] + f_direction1[(face_num + 1) % f_direction1_num]) / 2;
				D1[i][face_num] = facepoint1[i][face_num] - facepoint1[i - 1][face_num];
			}
		}
		else
		{
			for (int face_num = 0; face_num < f_direction1_num; face_num++)
			{
				facepoint1[i][face_num] = facepoint1[i - 1][face_num] + (D1[i - 1][face_num] * 2
					- D1[i - 1][(face_num - 1 + f_direction1_num) % f_direction1_num] - D1[i - 1][(face_num + 1) % f_direction1_num])
					*((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
				D1[i][face_num] = facepoint1[i][face_num] - facepoint1[i - 1][face_num];
			}
		}
	}
	//计算第二个方向
	std::vector<std::vector<V3f>> facepoint2(n + 1, std::vector<V3f>(f_direction2_num, V3f(0, 0, 0)));
	std::vector<std::vector<V3f>> D2(n + 1, std::vector<V3f>(f_direction2_num, V3f(0, 0, 0)));
	for (int i = 1; i <= n; i++)
	{
		if (i == 1)
		{
			for (int face_num = 0; face_num < f_direction2_num; face_num++)
			{
				facepoint2[i][face_num] = (f_direction2[face_num] + f_direction2[(face_num + 1) % f_direction2_num]) / 2;
				D2[i][face_num] = facepoint2[i][face_num] - facepoint2[i - 1][face_num];
			}
		}
		else
		{
			for (int face_num = 0; face_num < f_direction2_num; face_num++)
			{
				facepoint2[i][face_num] = facepoint2[i - 1][face_num] + (D2[i - 1][face_num] * 2
					- D2[i - 1][(face_num - 1 + f_direction2_num) % f_direction2_num] - D2[i - 1][(face_num + 1) % f_direction2_num])
					*((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
				D2[i][face_num] = facepoint2[i][face_num] - facepoint2[i - 1][face_num];
			}
		}
	}
	face_point = (facepoint1[n][f_direction1_num - 1] + facepoint2[n][f_direction2_num - 1]) / 2;
	return face_point;
}

std::vector<V3f> TwoNSubdivision::select_face_line(FaceHandle _f, EdgeHandle _e)
{
	std::vector<V3f> face_line;
	EdgeHandle last_edge = mesh->get_opposite_edge_in_face(_e, _f);
	face_line.push_back(edgepoint[iteration_num][_e]);
	FaceHandle previous_f = _f;
	EdgeHandle previous_e = _e;
	while (previous_e != last_edge)
	{
		std::set<FaceHandle, compare_OVM> e_f = mesh->neighbor_e[previous_e];
		for (std::set<FaceHandle, compare_OVM>::iterator e_f_it = e_f.begin(); e_f_it != e_f.end(); e_f_it++)
		{
			if (mesh->is_bdy(*e_f_it) && *e_f_it != previous_f)
			{
				EdgeHandle op_e = mesh->get_opposite_edge_in_face(previous_e, *e_f_it);
				face_line.push_back(edgepoint[iteration_num][op_e]);
				previous_e = op_e;
				previous_f = *e_f_it;
				break;
			}
		}
	}
	return face_line;
}

void TwoNSubdivision::cal_facepoint_withn_solid()
{
	facepointsolid.resize(iteration_num + 1, std::vector<V3f>(mesh->faces_.size(), V3f(0, 0, 0)));
	std::vector<std::vector<V3f>> D(iteration_num + 1, std::vector<V3f>(mesh->faces_.size(), V3f(0, 0, 0)));
	for (int i = 1; i <= iteration_num; i++)
	{
		if (i == 1)
		{
			for (int face_num = 0; face_num < mesh->faces_.size(); face_num++)
			{
				facepointsolid[i][face_num] = cal_face_midv(FaceHandle(face_num));
				D[i][face_num] = facepointsolid[i][face_num] - facepointsolid[i - 1][face_num];
			}
		}
		else
		{
			for (int face_num = 0; face_num < mesh->faces_.size(); face_num++)
			{
				if (mesh->is_bdy(FaceHandle(face_num)))
				{
					V3f t_result;
					std::vector<EdgeHandle> f_e = mesh->faces_[face_num].edges_;
					for (std::vector<EdgeHandle>::iterator f_e_it = f_e.begin(); f_e_it != f_e.end(); f_e_it++)
					{
						std::set<FaceHandle, compare_OVM> f_e_f = mesh->neighbor_e[*f_e_it];
						for (std::set<FaceHandle, compare_OVM>::iterator f_e_f_it = f_e_f.begin(); f_e_f_it != f_e_f.end(); f_e_f_it++)
						{
							if (mesh->is_bdy(*f_e_f_it) && *f_e_f_it != FaceHandle(face_num))
							{
								t_result = t_result - D[i - 1][*f_e_f_it];
								break;
							}
						}
					}
					t_result = t_result + D[i - 1][face_num] * 4;
					facepointsolid[i][face_num] = facepointsolid[i - 1][face_num] + t_result * ((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
					D[i][face_num] = facepointsolid[i][face_num] - facepointsolid[i - 1][face_num];
					continue;
				}
				std::vector<std::vector<FaceHandle>> fe_sort;
				V3f temp_result;
				FaceHandle first_face;
				std::vector<EdgeHandle> f_e = mesh->faces_[face_num].edges_;
				int j = 0;
				for (std::vector<EdgeHandle>::iterator f_e_it = f_e.begin(); f_e_it != f_e.end(); f_e_it++)
				{
					std::set<FaceHandle, compare_OVM> f_e_f = mesh->neighbor_e[*f_e_it];
					std::vector<FaceHandle> fe_sort_temp;
					fe_sort_temp.push_back(FaceHandle(face_num));
					bool check = false;
					while (check == false)
					{
						for (std::set<FaceHandle, compare_OVM>::iterator f_e_f_it = f_e_f.begin(); f_e_f_it != f_e_f.end(); f_e_f_it++)
						{
							if (mesh->is_two_f_in_same_cell(*(fe_sort_temp.end() - 1), *f_e_f_it) && *f_e_f_it != *(fe_sort_temp.end() - 1))
							{
								if (fe_sort_temp.size() > 1)//要与上一条邻接面不同
								{
									if (*f_e_f_it != *(fe_sort_temp.end() - 2))
									{
										fe_sort_temp.push_back(*f_e_f_it);
										break;
									}
								}
								else
								{
									if (j == 0)
									{
										first_face = *f_e_f_it;
										fe_sort_temp.push_back(*f_e_f_it);
										j++;
										break;
									}
									else
									{
										if (mesh->is_two_f_in_same_cell(first_face, *f_e_f_it))
										{
											fe_sort_temp.push_back(*f_e_f_it);
											break;
										}
									}
								}
							}
							else
							{
								if (fe_sort_temp.size() == (f_e_f.size() - 1)
									&& std::find(fe_sort_temp.begin(), fe_sort_temp.end(), *f_e_f_it) == fe_sort_temp.end())
								{
									fe_sort_temp.push_back(*f_e_f_it);
									break;
								}
							}
						}
						if (fe_sort_temp.size() == f_e_f.size())
						{
							check = true;
						}
					}
					fe_sort.push_back(fe_sort_temp);
				}
				for (std::vector<std::vector<FaceHandle>>::iterator fe_sort_it = fe_sort.begin(); fe_sort_it != fe_sort.end(); fe_sort_it++)
				{
					std::vector<FaceHandle> temp = *fe_sort_it;
					int temp_id = 0;
					int N = temp.size();
					V3f tempto;
					for (std::vector<FaceHandle>::iterator temp_it = temp.begin(); temp_it != temp.end(); temp_it++)
					{
						double a = (4 / N * 1.0)*cos(2 * M_PI*temp_id / N * 1.0);
						temp_id++;
						tempto = tempto + D[i - 1][*temp_it] * a;
					}
					temp_result = temp_result + tempto;
				}
				facepointsolid[i][face_num] = facepointsolid[i - 1][face_num] + temp_result * ((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
				D[i][face_num] = facepointsolid[i][face_num] - facepointsolid[i - 1][face_num];
			}
		}
	}
}

V3f TwoNSubdivision::cal_twon_edge_point(const int cell_num, const int ver1, const int ver2)
{
	EdgeHandle edge = check_edge(cell_num, ver1, ver2);
	//V3f edge1 = edgepoint[iteration_num][edge];
	//V3f edge2 = cal_edge_mid(edge);
	return edgepoint[iteration_num][edge];
	/*if (mesh->is_bdy(edge))
		return edgepoint[iteration_num][edge];
	else
		return cal_edge_mid(edge);*/
}

V3f TwoNSubdivision::cal_twon_face_point(const int cell_num, const int face_num)
{
	FaceHandle face = mesh->cells_[cell_num].faces_[face_num];//确定该面
	/*return facepointsolid[iteration_num][face];*/
	if (mesh->is_bdy(face))
		return cal_facepoint_withn(face);
	else
		return facepointsolid[iteration_num][face];
}

V3f TwoNSubdivision::cal_twon_new_vertex(const int cell_num, const int ver_num)
{
	VertexHandle vertex = mesh->cells_[cell_num].vertex_[ver_num];
	return mesh->vertices_[vertex];
	/*if (mesh->is_bdy(vertex))
		return mesh->vertices_[vertex];
	else
		return cal_new_vertex(vertex);*/
}

V3f TwoNSubdivision::cal_cellpoint_withn(CellHandle _c)
{
	V3f cell_point;
	std::vector<std::vector<V3f>> cell_line(3);//一个块的3个方向
	cell_line[0] = select_cell_line(_c, mesh->cells_[_c].faces_[0]);
	cell_line[1] = select_cell_line(_c, mesh->cells_[_c].faces_[2]);
	cell_line[2] = select_cell_line(_c, mesh->cells_[_c].faces_[4]);
	for (std::vector<std::vector<V3f>>::iterator cell_line_it = cell_line.begin(); cell_line_it != cell_line.end(); cell_line_it++)
	{
		std::vector<V3f> cellline_temp = *cell_line_it;
		int temp_num = cellline_temp.size();
		std::vector<std::vector<V3f>> cellpoint(iteration_num + 1, std::vector<V3f>(temp_num, V3f(0, 0, 0)));
		std::vector<std::vector<V3f>> D(iteration_num + 1, std::vector<V3f>(temp_num, V3f(0, 0, 0)));
		for (int i = 1; i <= iteration_num; i++)
		{
			if (i == 1)
			{
				for (int cell_num = 0; cell_num < temp_num; cell_num++)
				{
					cellpoint[i][cell_num] = (cellline_temp[cell_num] + cellline_temp[(cell_num + 1) % temp_num]) / 2;
					D[i][cell_num] = cellpoint[i][cell_num] - cellpoint[i - 1][cell_num];
				}
			}
			else
			{
				for (int cell_num = 0; cell_num < temp_num; cell_num++)
				{
					cellpoint[i][cell_num] = cellpoint[i - 1][cell_num] + (D[i - 1][cell_num] * 2
						- D[i - 1][(cell_num - 1 + temp_num) % temp_num] - D[i - 1][(cell_num + 1) % temp_num])
						*((2 * (i - 1) - 1) / (8 * (i - 1) * 1.0));
					D[i][cell_num] = cellpoint[i][cell_num] - cellpoint[i - 1][cell_num];
				}
			}
		}
		cell_point = cell_point + cellpoint[iteration_num][temp_num - 1];
	}
	cell_point = cell_point / 3;
	return cell_point;
}

std::vector<V3f> TwoNSubdivision::select_cell_line(CellHandle _c, FaceHandle _f)
{
	std::vector<V3f> cell_line;
	std::vector<FaceHandle> check_face;
	FaceHandle last_face = mesh->get_opposite_face_in_cell(_f, _c);
	if (mesh->is_bdy(_f))
		cell_line.push_back(cal_facepoint_withn(_f));
	else
		cell_line.push_back(facepointsolid[iteration_num][_f]);
	check_face.push_back(_f);
	//cell_line.push_back(facepointsolid[iteration_num][_f]);
	CellHandle previous_c = _c;
	FaceHandle previous_f = _f;
	while (previous_f != last_face)
	{
		std::set<CellHandle, compare_OVM> f_c = mesh->neighbor_f[previous_f];
		for (std::set<CellHandle, compare_OVM>::iterator f_c_it = f_c.begin(); f_c_it != f_c.end(); f_c_it++)
		{
			if (*f_c_it != previous_c)
			{
				FaceHandle op_f = mesh->get_opposite_face_in_cell(previous_f, *f_c_it);
				if (mesh->is_bdy(op_f))
					cell_line.push_back(cal_facepoint_withn(op_f));
				else
					cell_line.push_back(facepointsolid[iteration_num][op_f]);
				check_face.push_back(op_f);
				previous_f = op_f;
				previous_c = *f_c_it;
				break;
			}
		}
		if (mesh->is_bdy(previous_f))
		{
			std::vector<V3f> temp_cell_line;
			std::vector<FaceHandle> temp_check;
			if (mesh->is_bdy(last_face))
				temp_cell_line.insert(temp_cell_line.begin(), cal_facepoint_withn(last_face));
			else
				temp_cell_line.insert(temp_cell_line.begin(), facepointsolid[iteration_num][last_face]);
			temp_check.insert(temp_check.begin(), last_face);
			//temp_cell_line.insert(temp_cell_line.begin(),facepointsolid[iteration_num][last_face]);
			CellHandle previous_c_1 = _c;
			FaceHandle previous_f_1 = last_face;
			while (!mesh->is_bdy(previous_f_1))
			{
				std::set<CellHandle, compare_OVM> f_c_1 = mesh->neighbor_f[previous_f_1];
				for (std::set<CellHandle, compare_OVM>::iterator f_c_1_it = f_c_1.begin(); f_c_1_it != f_c_1.end(); f_c_1_it++)
				{
					if (*f_c_1_it != previous_c_1)
					{
						FaceHandle op_f = mesh->get_opposite_face_in_cell(previous_f_1, *f_c_1_it);
						if (mesh->is_bdy(op_f))
							temp_cell_line.insert(temp_cell_line.begin(), cal_facepoint_withn(op_f));
						else
							temp_cell_line.insert(temp_cell_line.begin(), facepointsolid[iteration_num][op_f]);
						temp_check.insert(temp_check.begin(), op_f);
						previous_f_1 = op_f;
						previous_c_1 = *f_c_1_it;
						break;
					}
				}
			}
			cell_line.insert(cell_line.end(), temp_cell_line.begin(), temp_cell_line.end());
			check_face.insert(check_face.end(), temp_check.begin(), temp_check.end());
			previous_f = last_face;
		}
	}
	return cell_line;
}

HexV3fMesh& TwoNSubdivision::TwoNSubdiving()
{
	cal_edgepoint_withn();
	cal_facepoint_withn_solid();
	uint64_t cell_num = mesh->cells_.size();
	for (uint64_t i = 0; i < cell_num; i++)
	{

		//暴力方法 将每个网格的各点顺序固定死 插入27个点
		VertexHandle temp_vex[27];

		//首先插入原始的8个点
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++)
		{
			V3f new_vertex = cal_twon_new_vertex(i, mesh_vex_num);
			temp_vex[mesh_vex_num] = newmesh->add_vertex(new_vertex);
		}
		//-------插入边的中点-------//
		//第一条边的中点
		V3f edge_mid_1 = cal_twon_edge_point(i, 0, 1);
		temp_vex[8] = newmesh->add_vertex(edge_mid_1);

		//第二条边的中点
		V3f edge_mid_2 = cal_twon_edge_point(i, 1, 5);
		temp_vex[9] = newmesh->add_vertex(edge_mid_2);

		//第三条边的中点
		V3f edge_mid_3 = cal_twon_edge_point(i, 5, 4);
		temp_vex[10] = newmesh->add_vertex(edge_mid_3);

		//第四条边的中点
		V3f edge_mid_4 = cal_twon_edge_point(i, 4, 0);
		temp_vex[11] = newmesh->add_vertex(edge_mid_4);

		//第五条边的中点
		V3f edge_mid_5 = cal_twon_edge_point(i, 3, 2);
		temp_vex[12] = newmesh->add_vertex(edge_mid_5);

		//第六条边的中点
		V3f edge_mid_6 = cal_twon_edge_point(i, 3, 7);
		temp_vex[13] = newmesh->add_vertex(edge_mid_6);

		//第七条边的中点
		V3f edge_mid_7 = cal_twon_edge_point(i, 6, 7);
		temp_vex[14] = newmesh->add_vertex(edge_mid_7);

		//第八条边的中点
		V3f edge_mid_8 = cal_twon_edge_point(i, 2, 6);
		temp_vex[15] = newmesh->add_vertex(edge_mid_8);

		//第九条边的中点
		V3f edge_mid_9 = cal_twon_edge_point(i, 1, 2);
		temp_vex[16] = newmesh->add_vertex(edge_mid_9);

		//第十条边的中点
		V3f edge_mid_10 = cal_twon_edge_point(i, 5, 6);
		temp_vex[17] = newmesh->add_vertex(edge_mid_10);

		//第十一条边的中点
		V3f edge_mid_11 = cal_twon_edge_point(i, 0, 3);
		temp_vex[18] = newmesh->add_vertex(edge_mid_11);

		//第十二条边的中点
		V3f edge_mid_12 = cal_twon_edge_point(i, 4, 7);
		temp_vex[19] = newmesh->add_vertex(edge_mid_12);
		//-------插入边的中点结束-------//

		//-------插入面的中心-------//
		for (int mesh_face_num = 0; mesh_face_num < 6; mesh_face_num++)
		{
			V3f face_mid = cal_twon_face_point(i, mesh_face_num);
			//判断加入数组
			temp_vex[20 + mesh_face_num] = newmesh->add_vertex(face_mid);
		}
		//-------插入面的中心结束-------//

		//-------插入单元的内心-------//
		V3f cell_mid = cal_cellpoint_withn(CellHandle(i));
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