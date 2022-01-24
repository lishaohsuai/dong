#pragma once
#include "GeometryKernel.h"
using namespace std;
class Transformation
{
public:
	Transformation(HexV3fMesh *_mesh) :mesh(_mesh) {}
	void translation(double x, double y, double z)
	{
		vector<vector<double>> matrix{ {1,0,0,x}, {0,1,0,y},{0,0,1,z},{0,0,0,1} };
		for (int i = 0; i < mesh->vertices_.size(); ++i)
		{
			V3f v = mesh->vertices_[i];
			vector<double> V{ v.x,v.y,v.z,1 };
			mesh->vertices_[i] = matrix_muti(V, matrix);
		}
	}
	void rotate_y(double x, double angle)
	{
		translation(-x, 0, 0);
		//vector<vector<double>> matrix1{ {1,0,0,-x}, {0,1,0,0},{0,0,1,0},{0,0,0,1} };
		vector<vector<double>> matrix2{ {cos(angle),0,sin(angle),0}, {0,1,0,0},{-sin(angle),0,cos(angle),0},{0,0,0,1} };
		//vector<vector<double>> matrix3{ {1,0,0,x}, {0,1,0,0},{0,0,1,0},{0,0,0,1} };
		for (int i = 0; i < mesh->vertices_.size(); ++i)
		{
			V3f v = mesh->vertices_[i];
			vector<double> V{ v.x,v.y,v.z,1 };
			//v = matrix_muti(V, matrix1);
			//vector<double> V1{ v.x,v.y,v.z,1 };
			//v = matrix_muti(V1, matrix2);
			//vector<double> V2{ v.x,v.y,v.z,1 };
			mesh->vertices_[i] = matrix_muti(V, matrix2);
			//mesh->vertices_[i] = matrix_muti(V2, matrix3);
		}
		translation(x, 0, 0);
	}
	void scale(double x,double y,double z)
	{
		translation(-mesh->center.x, -mesh->center.y, -mesh->center.z);
		vector<vector<double>> matrix{ {x,0,0,0}, {0,y,0,0},{0,0,z,0},{0,0,0,1} };
		for (int i = 0; i < mesh->vertices_.size(); ++i)
		{
			V3f v = mesh->vertices_[i];
			vector<double> V{ v.x,v.y,v.z,1 };
			mesh->vertices_[i] = matrix_muti(V, matrix);
		}
		translation(mesh->center.x, mesh->center.y, mesh->center.z);
	}
private:
	V3f matrix_muti(vector<double> &V, vector<vector<double>> &matrix)
	{
		V3f v;
		for (int i = 0; i < 4; ++i)
		{
			v.x += matrix[0][i] * V[i];
			v.y += matrix[1][i] * V[i];
			v.z += matrix[2][i] * V[i];
		}
		return v;
	}
	HexV3fMesh *mesh;
};