#include "PIA.h"

void PIA_FOR_CC_SOLID::PIA_FOR_CC_SOLID_ONCE() {
	uint64_t cell_num = mesh->cells_.size();
	uint64_t vertices_num = mesh->vertices_.size();
	std::vector<V3f> new_pos_vec(vertices_num);
	for (uint64_t i = 0; i < cell_num; i++) {
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++) {
			VertexHandle& vh = mesh->cells_[i].vertex_[mesh_vex_num];
			V3f new_pos(0.0, 0.0, 0.0);
			std::set<VertexHandle> ev_set, fv_set, cv_set;
			std::map<VertexHandle, EdgeHandle> v_f_map;
			if (mesh->is_bdy(vh)) {
				// ����Ǳ����
				int num_adjacent_edges = 0;
				// ���ڵ�ǰ���㣬 �����������ڽӱ߶��㣬����ev_set��
				for (const auto ve : mesh->neighbor_v[vh]) {
					if (mesh->is_bdy(mesh->edges_[ve].from_h)) {
						ev_set.insert(mesh->edges_[ve].from_h);
						v_f_map[mesh->edges_[ve].from_h] = ve;
					}
					if (mesh->is_bdy(mesh->edges_[ve].to_h)) {
						ev_set.insert(mesh->edges_[ve].to_h);
						v_f_map[mesh->edges_[ve].to_h] = ve;
					}
				}

				//���ڵ�ǰ���㣬�����������ڽ����ϵĶ���,������ǵ�ǰ�����Լ�����ev_set�еĵ㣬���뵽fv_set��
				for (const auto vf : mesh->neighbor_v_f[vh]) {
					if (mesh->is_bdy(vf)) {
						size_t vertices_number_on_face = mesh->faces_[vf].vs_f.size();
						for (int face_vertices_num = 0; face_vertices_num < vertices_number_on_face; ++face_vertices_num) {
							if (ev_set.find((mesh->faces_[vf].vs_f[face_vertices_num])) == ev_set.end()) {
								fv_set.insert(mesh->faces_[vf].vs_f[face_vertices_num]);
							}
						}
					}
				}
				//ɾ����ǰ��
				ev_set.erase(vh);
				fv_set.erase(vh);
				num_adjacent_edges = ev_set.size();

				new_pos = new_pos + mesh->vertices_[vh] * double(num_adjacent_edges) * double(num_adjacent_edges);
				for (const auto& ev : ev_set) {
					new_pos = new_pos + mesh->vertices_[ev] * 4;
				}
				for (const auto& fv : fv_set) {
					new_pos = new_pos + mesh->vertices_[fv];
				}

				new_pos = new_pos / (num_adjacent_edges * (num_adjacent_edges + 5));
				err_now += fabs(firstmesh->vertices_[vh].x - new_pos.x) + fabs(firstmesh->vertices_[vh].y - new_pos.y) 
						+ fabs(firstmesh->vertices_[vh].z - new_pos.z);
			}
			else {
				//new_pos = mesh->vertices_[vh];
				//������ڲ���
				//���ڵ�ǰ���㣬 �����������ڽӱ߶��㣬����ev_set��
				for (const auto ve : mesh->neighbor_v[vh]) {
					ev_set.insert(mesh->edges_[ve].from_h);
					v_f_map[mesh->edges_[ve].from_h] = ve;
					ev_set.insert(mesh->edges_[ve].to_h);
					v_f_map[mesh->edges_[ve].to_h] = ve;
				}

				//���ڵ�ǰ���㣬�����������ڽ����ϵĶ���,������ǵ�ǰ�����Լ�����ev_set�еĵ㣬���뵽fv_set��
				for (const auto vf : mesh->neighbor_v_f[vh]) {
					size_t vertices_number_on_face = mesh->faces_[vf].vs_f.size();
					for (int face_vertices_num = 0; face_vertices_num < vertices_number_on_face; ++face_vertices_num) {
						if (ev_set.find(mesh->faces_[vf].vs_f[face_vertices_num]) == ev_set.end()) {
							fv_set.insert(mesh->faces_[vf].vs_f[face_vertices_num]);
						}
					}
				}

				//���ڵ�ǰ���㣬�������������ϵĶ��㣬��������������㣬�����뵽cv_set��
				for (const auto vc : mesh->neighbor_v_c[vh]) {
					size_t vertices_number_on_cell = mesh->cells_[vc].vertex_.size();
					for (int cell_vertices_num = 0; cell_vertices_num < vertices_number_on_cell; ++cell_vertices_num) {
						if (ev_set.find(mesh->cells_[vc].vertex_[cell_vertices_num]) == ev_set.end() && fv_set.find(mesh->cells_[vc].vertex_[cell_vertices_num]) == fv_set.end()) {
							cv_set.insert(mesh->cells_[vc].vertex_[cell_vertices_num]);
						}
					}
				}
				//ɾ����ǰ��
				ev_set.erase(vh);
				fv_set.erase(vh);
				cv_set.erase(vh);
				size_t sum_m = 0;
				new_pos = new_pos + mesh->vertices_[vh] * 16 * (mesh->neighbor_v[vh].size() - 2);
				for (const auto& ev : ev_set) {
					new_pos = new_pos + mesh->vertices_[ev] * 4 * mesh->neighbor_e[v_f_map[ev]].size();
					sum_m += mesh->neighbor_e[v_f_map[ev]].size();
				}
				for (const auto& fv : fv_set) {
					new_pos = new_pos + mesh->vertices_[fv] * 4;
				}

				for (const auto& cv : cv_set) {
					new_pos = new_pos + mesh->vertices_[cv];
				}
				new_pos = new_pos / (30 * (mesh->neighbor_v[vh].size() - 2) + 4 * sum_m);
			}

			//���������в���ÿһ������
			/*err_now += fabs(firstmesh->vertices_[vh].x - new_pos.x) + fabs(firstmesh->vertices_[vh].y - new_pos.y) + fabs(firstmesh->vertices_[vh].z - new_pos.z);
			new_pos = firstmesh->vertices_[vh] - new_pos + mesh->vertices_[vh];*/
			//std::cout << "[DEBUG] " << oldmesh->vertices_.size() << " " << mesh->vertices_.size() << " " << vh << std::endl;
			new_pos = oldmesh->vertices_[vh] - new_pos + mesh->vertices_[vh];
			
			new_pos_vec[vh] = new_pos;
		}
	}
	for (uint64_t i = 0; i < vertices_num; i++) {
		mesh->vertices_[i] = new_pos_vec[i];
	}
}

// ���ڸ߽�����
void PIA_FOR_CC_SOLID::PIA_FOR_HIGH_ORDERED_MESH() {
	uint64_t cell_num = mesh->cells_.size();
	uint64_t vertices_num = mesh->vertices_.size();
	std::vector<V3f> new_pos_vec(vertices_num);
	for (uint64_t i = 0; i < cell_num; i++) {
		for (int mesh_vex_num = 0; mesh_vex_num < 8; mesh_vex_num++) {
			VertexHandle& vh = mesh->cells_[i].vertex_[mesh_vex_num];
			V3f new_pos(0.0, 0.0, 0.0);
			std::set<VertexHandle> ev_set, fv_set, cv_set;
			std::map<VertexHandle, EdgeHandle> v_f_map;
			if (mesh->is_bdy(vh)) {
				//����Ǳ����
				int num_adjacent_edges = 0;
				//���ڵ�ǰ���㣬 �����������ڽӱ߶��㣬����ev_set��
				for (const auto ve : mesh->neighbor_v[vh]) {
					if (mesh->is_bdy(mesh->edges_[ve].from_h)) {
						ev_set.insert(mesh->edges_[ve].from_h);
						v_f_map[mesh->edges_[ve].from_h] = ve;
					}
					if (mesh->is_bdy(mesh->edges_[ve].to_h)) {
						ev_set.insert(mesh->edges_[ve].to_h);
						v_f_map[mesh->edges_[ve].to_h] = ve;
					}
				}

				//���ڵ�ǰ���㣬�����������ڽ����ϵĶ���,������ǵ�ǰ�����Լ�����ev_set�еĵ㣬���뵽fv_set��
				for (const auto vf : mesh->neighbor_v_f[vh]) {
					if (mesh->is_bdy(vf)) {
						size_t vertices_number_on_face = mesh->faces_[vf].vs_f.size();
						for (int face_vertices_num = 0; face_vertices_num < vertices_number_on_face; ++face_vertices_num) {
							if (ev_set.find((mesh->faces_[vf].vs_f[face_vertices_num])) == ev_set.end()) {
								fv_set.insert(mesh->faces_[vf].vs_f[face_vertices_num]);
							}
						}
					}
				}
				//ɾ����ǰ��
				ev_set.erase(vh);
				fv_set.erase(vh);
				num_adjacent_edges = ev_set.size();

				new_pos = new_pos + mesh->vertices_[vh] * double(num_adjacent_edges) * double(num_adjacent_edges);
				for (const auto& ev : ev_set) {
					new_pos = new_pos + mesh->vertices_[ev] * 4;
				}
				for (const auto& fv : fv_set) {
					new_pos = new_pos + mesh->vertices_[fv];
				}
				
				new_pos = new_pos / (num_adjacent_edges * (num_adjacent_edges + 5));
				//err_now += fabs(firstmesh->vertices_[vh].x - new_pos.x) + fabs(firstmesh->vertices_[vh].y - new_pos.y) + fabs(firstmesh->vertices_[vh].z - new_pos.z);
				/*if (vertex2vertex.find(vh) != vertex2vertex.end()) {
					intpPoint = oldmesh->vertices_[vertex2vertex[vh]];
				}
				else if (vertex2face.find(vh) != vertex2face.end()) {
					intpPoint = oldmesh->
				}else if()*/
				new_pos_vec[vh] = oldmesh->vertices_[vh] - new_pos + mesh->vertices_[vh];
				err_now += fabs(oldmesh->vertices_[vh].x - new_pos.x) + fabs(oldmesh->vertices_[vh].y - new_pos.y) + \
						   fabs(oldmesh->vertices_[vh].z - new_pos.z);

			}
			//else {
			//	//new_pos = mesh->vertices_[vh];
			//	//������ڲ���
			//	//���ڵ�ǰ���㣬 �����������ڽӱ߶��㣬����ev_set��
			//	for (const auto ve : mesh->neighbor_v[vh]) {
			//		ev_set.insert(mesh->edges_[ve].from_h);
			//		v_f_map[mesh->edges_[ve].from_h] = ve;
			//		ev_set.insert(mesh->edges_[ve].to_h);
			//		v_f_map[mesh->edges_[ve].to_h] = ve;
			//	}
			else {
				new_pos_vec[vh] = mesh->vertices_[vh];
			}

			//	//���ڵ�ǰ���㣬�����������ڽ����ϵĶ���,������ǵ�ǰ�����Լ�����ev_set�еĵ㣬���뵽fv_set��
			//	for (const auto vf : mesh->neighbor_v_f[vh]) {
			//		size_t vertices_number_on_face = mesh->faces_[vf].vs_f.size();
			//		for (int face_vertices_num = 0; face_vertices_num < vertices_number_on_face; ++face_vertices_num) {
			//			if (ev_set.find(mesh->faces_[vf].vs_f[face_vertices_num]) == ev_set.end()) {
			//				fv_set.insert(mesh->faces_[vf].vs_f[face_vertices_num]);
			//			}
			//		}
			//	}

			//	//���ڵ�ǰ���㣬�������������ϵĶ��㣬��������������㣬�����뵽cv_set��
			//	for (const auto vc : mesh->neighbor_v_c[vh]) {
			//		size_t vertices_number_on_cell = mesh->cells_[vc].vertex_.size();
			//		for (int cell_vertices_num = 0; cell_vertices_num < vertices_number_on_cell; ++cell_vertices_num) {
			//			if (ev_set.find(mesh->cells_[vc].vertex_[cell_vertices_num]) == ev_set.end() && fv_set.find(mesh->cells_[vc].vertex_[cell_vertices_num]) == fv_set.end()) {
			//				cv_set.insert(mesh->cells_[vc].vertex_[cell_vertices_num]);
			//			}
			//		}
			//	}
			//	//ɾ����ǰ��
			//	ev_set.erase(vh);
			//	fv_set.erase(vh);
			//	cv_set.erase(vh);
			//	size_t sum_m = 0;
			//	new_pos = new_pos + mesh->vertices_[vh] * 16 * (mesh->neighbor_v[vh].size() - 2);
			//	for (const auto& ev : ev_set) {
			//		new_pos = new_pos + mesh->vertices_[ev] * 4 * mesh->neighbor_e[v_f_map[ev]].size();
			//		sum_m += mesh->neighbor_e[v_f_map[ev]].size();
			//	}
			//	for (const auto& fv : fv_set) {
			//		new_pos = new_pos + mesh->vertices_[fv] * 4;
			//	}

			//	for (const auto& cv : cv_set) {
			//		new_pos = new_pos + mesh->vertices_[cv];
			//	}
			//	new_pos = new_pos / (30 * (mesh->neighbor_v[vh].size() - 2) + 4 * sum_m);

			//}

			//���������в���ÿһ������
			/*err_now += fabs(firstmesh->vertices_[vh].x - new_pos.x) + fabs(firstmesh->vertices_[vh].y - new_pos.y) + fabs(firstmesh->vertices_[vh].z - new_pos.z);
			new_pos = firstmesh->vertices_[vh] - new_pos + mesh->vertices_[vh];*/

			//�˴����
			/*new_pos = firstmesh->vertices_[vh] - new_pos + mesh->vertices_[vh];
			new_pos_vec[vh] = new_pos;*/
		}
	}
	for (uint64_t i = 0; i < vertices_num; i++) {
		mesh->vertices_[i] = new_pos_vec[i];
	}
}



/**********************************  PartPIA  ********************************/

PartPIA::PartPIA(HexV3fMesh* _mesh, std::vector<V3f>& _TargetPoint, std::vector<EdgeHandle>& _SharpEdge) : mesh(_mesh), SharpEdge(_SharpEdge), CurrentError(0.0) {
	GetMovablePart();
	GetTargetPoint( _TargetPoint);
}

void PartPIA::GetMovablePart() {
	std::unordered_map<int, int> CandidateVertice;
	for (EdgeHandle e : SharpEdge) {
		VertexHandle vh1 = mesh->edges_[e].to_h;
		VertexHandle vh2 = mesh->edges_[e].from_h;

		CandidateVertice[(int)vh1]++;
		CandidateVertice[(int)vh2]++;
		if (CandidateVertice[(int)vh1] == 2) MovablePart.push_back(vh1);
		if (CandidateVertice[(int)vh2] == 2) MovablePart.push_back(vh2);
	}
}

void PartPIA::PartUpDate() {
	int MovableNum = MovablePart.size();
	// ֱ��ͨ������ߵļ򵥹�ʽ (4*v + v1 + v2)/6 ;   v1,v2:   �õ��Ӧ�ļ���ߵĳ�v֮��Ķ���
	CurrentError = 0.0;
	std::vector<V3f> tempNewPos;
	for (int i = 0; i < MovableNum; ++i) {
		VertexHandle vh = MovablePart[i];
		auto ve = mesh->neighbor_v[vh];
		V3f newPos(0.0f, 0.0f, 0.0f);
		newPos = newPos + mesh->vertices_[vh] * 4;
		for (auto ve_it = ve.begin(); ve_it != ve.end(); ve_it++) {
			if (std::find(SharpEdge.begin(), SharpEdge.end(), *ve_it) != SharpEdge.end()) {
				EdgeHandle e = *ve_it;
				if (mesh->edges_[e].from_h == vh)
					newPos = newPos + mesh->vertices_[mesh->edges_[e].to_h];
				else
					newPos = newPos + mesh->vertices_[mesh->edges_[e].from_h];
			}
		}
		newPos = newPos / 6;
		CurrentError += distance(newPos, TargetPoint[i]);          // ���㵱ǰ���
		newPos = TargetPoint[i] - newPos + mesh->vertices_[vh];    
		tempNewPos.push_back(newPos);
	}
	// ���µ�����
	for (int i = 0; i < MovableNum; ++i) {
		mesh->vertices_[MovablePart[i]] = tempNewPos[i]; 
		//mesh->vertices_[MovablePart[i]] =TargetPoint[i];
	}
}

void PartPIA::GetTargetPoint(std::vector<V3f>& _TargetPoint) {
	TargetPoint.clear();
	int PointCloudSize = _TargetPoint.size();
	//���Ŀ�������ƶ����������ȣ�ֱ�Ӷ�Ӧ
	if (_TargetPoint.size() == MovablePart.size()) {
		TargetPoint = _TargetPoint;
	}
	//���Ŀ�������ƶ������������ȣ�Ѱ������ƶ��������Ŀ���
	else {
		for (int i = 0; i < MovablePart.size(); ++i) {
			auto vh = MovablePart[i];
			auto MeshPoint = mesh->vertices_[vh];
			double MinID = -1;
			double MinDis = 999999999;
			for (int j = 0; j < PointCloudSize; ++j) {
				double CurDis = distance(MeshPoint, _TargetPoint[j]);
				if (MinDis > CurDis) {
					MinDis = CurDis;
					MinID = j;
				}
			}
			TargetPoint.push_back(_TargetPoint[MinID]);
		}
	}
}

SurfacePIA::SurfacePIA(HexV3fMesh* _mesh, std::vector<V3f>& _TargetPoint, std::vector<EdgeHandle>& _FixedEdge)
	:mesh(_mesh),CurrentError(0.0)
{
	GetFixedPart(_FixedEdge);          //�õ��̶��������
	//GetSharpPoints();                  //����SharpEdge�õ�������ϵĵ�
	GetMovablePart();                  //�õ����˹̶�������ı����
	GetTargetPoint(_TargetPoint);      //�õ�ÿ�����ƶ����Ӧ��Ŀ���
}

void SurfacePIA::GetSharpEdge(std::vector<EdgeHandle>& _SharpEdge) {
	for (int i = 0; i < _SharpEdge.size(); ++i) {
		SharpEdge.push_back(_SharpEdge[i]);
	}
}

void SurfacePIA::GetFixedPart(std::vector<EdgeHandle>& _FixedEdge) {
	std::unordered_map<int, int> CandidateVertice;
	for (EdgeHandle e : _FixedEdge) {
		VertexHandle vh1 = mesh->edges_[e].to_h;
		VertexHandle vh2 = mesh->edges_[e].from_h;

		CandidateVertice[(int)vh1]++;
		CandidateVertice[(int)vh2]++;
		if (CandidateVertice[(int)vh1] == 2) FixedPart[(int)vh1] = 1;
		if (CandidateVertice[(int)vh2] == 2) FixedPart[(int)vh2] = 1;
	}
}
void SurfacePIA::GetSharpPoints() {
	for (EdgeHandle e : SharpEdge) {
		VertexHandle vh1 = mesh->edges_[e].to_h;
		VertexHandle vh2 = mesh->edges_[e].from_h;

		SharpPoints[(int)vh1]++;
		SharpPoints[(int)vh2]++;
	}
}

void SurfacePIA::GetMovablePart() {
	for (int i = 0; i < mesh->vertices_.size(); ++i) {
		if (mesh->is_bdy(VertexHandle(i))&& FixedPart[i] != 1) {
			MovablePart.push_back(VertexHandle(i));
		}
	}
}

void SurfacePIA::GetTargetPoint(std::vector<V3f>& _TargetPoint) {
	TargetPoint.clear();
	int PointCloudSize = _TargetPoint.size();
	//���Ŀ�������ƶ����������ȣ�ֱ�Ӷ�Ӧ
	if (_TargetPoint.size() == MovablePart.size()) {
		TargetPoint = _TargetPoint;
	}
	//���Ŀ�������ƶ������������ȣ�Ѱ������ƶ��������Ŀ���
	else {
		for (int i = 0; i < MovablePart.size(); ++i) {
			auto vh = MovablePart[i];
			auto MeshPoint = mesh->vertices_[vh];
			double MinID = -1;
			double MinDis = 999999999;
			for (int j = 0; j < PointCloudSize; ++j) {
				double CurDis = distance(MeshPoint, _TargetPoint[j]);
				if (MinDis > CurDis) {
					MinDis = CurDis;
					MinID = j;
				}
			}
			TargetPoint.push_back(_TargetPoint[MinID]);
		}
	}
}

void SurfacePIA::SurfaceUpDate() {
	int MovableNum = MovablePart.size();
	CurrentError = 0.0;
	std::vector<V3f> tempNewPos;

	for (int i = 0; i < MovableNum; ++i) {
		VertexHandle vh = MovablePart[i];
		auto ve = mesh->neighbor_v[vh];
		V3f newPos(0.0f, 0.0f, 0.0f);
		if (SharpPoints[(int)vh] == 2) {
			newPos = newPos + mesh->vertices_[vh] * 4;
			for (auto ve_it = ve.begin(); ve_it != ve.end(); ve_it++) {
				if (std::find(SharpEdge.begin(), SharpEdge.end(), *ve_it) != SharpEdge.end())
				{
					EdgeHandle e = *ve_it;
					if (mesh->edges_[e].from_h == vh)
						newPos = newPos + mesh->vertices_[mesh->edges_[e].to_h];
					else
						newPos = newPos + mesh->vertices_[mesh->edges_[e].from_h];
				}
			}
			newPos = newPos / 6;
		}
		else {
			int n = 0;
			for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++) {
				if (mesh->is_bdy(*ve_it))
					n++;
			}
			newPos = newPos + mesh->vertices_[vh] * (n * n);
			for (std::set<EdgeHandle, compare_OVM>::iterator ve_it = ve.begin(); ve_it != ve.end(); ve_it++) {
				if (mesh->is_bdy(*ve_it)) {
					if (vh == mesh->edges_[*ve_it].from_h)
						newPos = newPos + mesh->vertices_[mesh->edges_[*ve_it].to_h] * 4;
					else
						newPos = newPos + mesh->vertices_[mesh->edges_[*ve_it].from_h] * 4;
				}
			}
			std::set<FaceHandle, compare_OVM> vf = mesh->neighbor_v_f[vh];
			for (std::set<FaceHandle, compare_OVM>::iterator vf_it = vf.begin(); vf_it != vf.end(); vf_it++) {
				if (mesh->is_bdy(*vf_it)) {
					V3f opv = mesh->vertices_[mesh->find_op_vertex(vh, *vf_it)];
					newPos = newPos + opv;
				}
			}
			newPos = newPos / (n * n + 5 * n);
		}
		CurrentError += distance(newPos, TargetPoint[i]);          //���㵱ǰ���
		newPos = TargetPoint[i] - newPos + mesh->vertices_[vh];
		tempNewPos.push_back(newPos);
	}

	//���µ�����
	for (int i = 0; i < MovableNum; ++i) {
		mesh->vertices_[MovablePart[i]] = tempNewPos[i];
		//mesh->vertices_[MovablePart[i]] =TargetPoint[i];
	}
}