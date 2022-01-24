#include"Transform.h"


//Êä³öÈý½Ç»¯Ä£ÐÍ
class triangleMesh {
public:
	std::vector<V3f> vertices;
	std::vector<std::vector<int>> faces;
	std::unordered_map<int, std::vector<int>> e;
	std::unordered_map<int, std::vector<int>> ef;
	std::unordered_map<int, std::vector<int>> fe;
	void ReorderMesh();
	void DFS(int f, int v1, int v2);
	std::vector<bool> flag;

};

void triangleMesh::ReorderMesh() {
	if (faces.size() == 0) return;
	flag.resize(faces.size(), 0);
	flag[0] = 1;
	for (int i = 0; i < fe[0].size(); ++i) {
		for (int j = 0; j < ef[fe[0][i]].size(); ++j) {
			if (ef[fe[0][i]][j] != 0) {
				DFS(ef[fe[0][i]][j], faces[0][i], faces[0][(i + 1) % 3]);
				break;
			}
		}
	}
}
void triangleMesh::DFS(int f, int v1, int v2) {

	if (flag[f]) return;
	//std::cout << f << " " << faces.size() << std::endl;
	flag[f] = true;
	if ((v1 == faces[f][0] && v2 == faces[f][1])) {
		faces[f][0] = v2;
		faces[f][1] = v1;
		int t = fe[f][1];
		fe[f][1] = fe[f][2];
		fe[f][2] = t;
	}
	else if ((v1 == faces[f][1] && v2 == faces[f][2])) {
		faces[f][1] = v2;
		faces[f][2] = v1;
		int t = fe[f][0];
		fe[f][0] = fe[f][2];
		fe[f][2] = t;
	}
	else if ((v1 == faces[f][2] && v2 == faces[f][0])) {
		faces[f][2] = v2;
		faces[f][0] = v1;
		int t = fe[f][0];
		fe[f][0] = fe[f][1];
		fe[f][1] = t;
	}
	//std::cout << "ok1" << std::endl;
	for (int i = 0; i < fe[f].size(); ++i) {
		//std::cout << i << "/" << fe[f].size() << std::endl;
		for (int j = 0; j < ef[fe[f][i]].size(); ++j) {
			//std::cout << j << "/" << ef[fe[f][i]].size() << std::endl;
			if (ef[fe[f][i]][j] != f) {
				DFS(ef[fe[f][i]][j], faces[f][i], faces[f][(i + 1) % 3]);
			}
		}
	}
	//std::cout << "ok2" << std::endl;
	return;
}

void Transform::Quad2Triangle() {

	triangleMesh tm;
	std::ofstream out(OutputPath);
	int bdyv = 0;
	int bdyf = 0;
	HexModel->examine_bdy();
	std::unordered_map<int, int> vv;
	for (int i = 0; i < HexModel->vertices_.size(); ++i) {
		if (HexModel->is_bdy(VertexHandle(i))) {
			out << "v " << HexModel->vertices_[i].x << " " << HexModel->vertices_[i].y << " " << HexModel->vertices_[i].z << std::endl;
			vv[i] = bdyv;
			bdyv++;
		}
	}
	for (int i = 0; i < HexModel->faces_.size(); ++i) {
		if (HexModel->is_bdy(FaceHandle(i))) {
			bdyf++;
			bdyf++;
		}
	}

	bdyv = 0;
	for (int i = 0; i < HexModel->vertices_.size(); ++i) {
		if (HexModel->is_bdy(VertexHandle(i))) {
			tm.vertices.push_back(HexModel->vertices_[i]);
		}
	}
	std::map<std::vector<int>, int> eID;
	for (int i = 0; i < HexModel->faces_.size(); ++i) {
		if (HexModel->is_bdy(FaceHandle(i))) {
			std::vector<int> f1{ vv[HexModel->faces_[i].vs_f[0]] ,vv[HexModel->faces_[i].vs_f[1]] ,vv[HexModel->faces_[i].vs_f[2]] };
			std::vector<int> f2{ vv[HexModel->faces_[i].vs_f[0]] ,vv[HexModel->faces_[i].vs_f[2]] ,vv[HexModel->faces_[i].vs_f[3]] };
			tm.faces.push_back(f1);
			for (int kk = 0; kk < 3; ++kk) {
				std::vector<int> e{ std::min(f1[kk], f1[(kk + 1) % 3]), std::max(f1[kk], f1[(kk + 1) % 3]) };
				if (eID[e] == 0) {
					eID[e] = eID.size();
				}
				tm.e[eID[e]] = e;
				tm.fe[tm.faces.size() - 1].push_back(eID[e]);
				tm.ef[eID[e]].push_back(tm.faces.size() - 1);
			}
			tm.faces.push_back(f2);
			for (int kk = 0; kk < 3; ++kk) {
				std::vector<int> e{ std::min(f2[kk], f2[(kk + 1) % 3]), std::max(f2[kk], f2[(kk + 1) % 3]) };
				if (eID[e] == 0) {
					eID[e] = eID.size();
				}
				tm.e[eID[e]] = e;
				tm.fe[tm.faces.size() - 1].push_back(eID[e]);
				tm.ef[eID[e]].push_back(tm.faces.size() - 1);
			}

		}
	}
	tm.ReorderMesh();
	for (int i = 0; i < tm.faces.size(); i += 2) {
		out << "f" << " " << tm.faces[i + 1][2] + 1 << " " << tm.faces[i + 1][1] + 1 << " " << tm.faces[i + 1][0] + 1 << std::endl;
		out << "f" << " " << tm.faces[i][2] + 1 << " " << tm.faces[i][1] + 1 << " " << tm.faces[i][0] + 1;
		out << std::endl;
	}
	out.close();
}


