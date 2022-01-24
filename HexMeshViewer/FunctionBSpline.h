#pragma once
#include "BSplineSolid.h"
#include "GeometryKernel.h"
class BdyBase
{
public:
	BdyBase(int _local_id, int _global_id, int _solid_id) :local_id(_local_id), global_id(_global_id), solid_id(_solid_id) {};
public:
	int local_id;
	int global_id;
	int solid_id;
};

class FunctionBSpline
{
public:
	FunctionBSpline(BSplineSolid _m_solids) : m_solids(_m_solids) {};
	FunctionBSpline(BSplineSolid _m_solids, std::vector<double> _material_distribution) : m_solids(_m_solids), 
		material_distribution(_material_distribution){};
	~FunctionBSpline() {};
	void update_material_distribution(std::vector<double> &_material_distribution)
	{
		material_distribution.clear();
		material_distribution.resize(_material_distribution.size(), 0);
		for (int i = 0; i < _material_distribution.size(); i++)
		{
			//std::cout << "Schedule of update material distribution " << i + 1 << ":" << _material_distribution.size() << std::endl;
			material_distribution[i] = _material_distribution[i];
		}
	}
public:
	BSplineSolid m_solids;
	std::vector<double> material_distribution;//材料分布
	const double umin = 0.0001;
	const double p = 3;//惩罚因子
public:
	void BdyFunLine(std::vector< std::vector<double> > &m_coef);
	void Global_MatrixAndRhs(Matrix &m_bigmatrix, std::vector<double> &m_bigrhs);
	void Global_MatrixAndRhs_Small(Matrix &m_bigmatrix, std::vector<double> &m_bigrhs, std::vector<int> &r_rowids, 
		std::vector<int> &r_colids, std::vector<double> &r_vals, std::vector<std::vector<int>> &m_colids);
	void Global_MatrixAndRhs( Matrix &m_bigmatrix, std::vector<double> &m_bigrhs, std::vector<Matrix> &local_matrix, 
		std::vector< std::vector<double>> &local_bigrhs);
	void Global_MatrixAndRhs1(Matrix &m_bigmatrix, std::vector<double> &m_bigrhs, std::vector<Matrix> &local_matrix, 
		std::vector< std::vector<double>> &local_bigrhs, std::map<int, int> &base_check);
	//求高斯积分点, n表示积分点的个数，w是求出的权因子，gausspoint是求出的高斯点
	void GaussIntegalPoint(int n, std::vector<double> &w, std::vector<double> &gausspoint);
	//求解线性方程组
	void SolveLinearEquation(int mtype, const std::vector<std::vector<double>> &coeff_matrix,
		const std::vector<double> &load_vect, std::vector<double> &m_solute);
	//计算数值解的Ｌ２误差
	void Error(std::vector<double> m_coef, std::vector<double> &m_errors);
	double ExactSol(double x, double y, double z);
	double RHS(double x, double y, double z);
	double RHS1(double u, double v, double w);
	double uTKu(std::vector<double> u, Matrix K0);
	double PTu(std::vector<double> P, std::vector<double> u);
public:
	/*-----------------求解面PDE-----------------*/
	const int U = 0, V = 1, W = 2;
	void BdyFunFace(std::vector< std::vector<double> > &m_coef);
	//uvw判断是哪一个面
	void Global_MatrixAndRhsFace(int solid_id,int uvw, int uvw_id, Matrix &m_bigmatrix, std::vector<double> &m_bigrhs);
	void facePDE(int solid_id, int uvw, int uvw_id, std::vector< std::vector<double> > &m_coef,
		std::vector< std::vector<double> > m_coeftest);
public:
	void Global_MatrixAndRhs1_new(Matrix &m_bigmatrix, std::vector<double> &m_bigrhs, std::vector<Matrix> &local_matrix,
		 std::map<int, int> &base_check);
	void Global_MatrixAndRhs_new(Matrix &m_bigmatrix, std::vector<double> &m_bigrhs, std::vector<Matrix> &local_matrix);
	void Global_MatrixAndRhs_new(Matrix &m_bigmatrix, std::vector<Matrix> &local_matrix);
public:
	void Global_MatrixAndRhs_Sparse(std::vector<std::vector<double>> &m_bdycoef,std::vector<BdyBase> &bdybaseid, std::vector<double> &m_bigrhs, std::vector<int> &r_rowids,
		std::vector<int> &r_colids, std::vector<double> &r_vals);
	void Global_MatrixAndRhs_Sparse(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid, std::vector<Matrix> &local_matrix,
		std::vector<double> &m_bigrhs, std::vector<std::vector<double>> &local_bigrhs, 	std::vector<int> &r_rowids, std::vector<int> &r_colids, std::vector<double> &r_vals);
	void Global_MatrixAndRhs_SparseOld(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid, std::vector<double> &m_bigrhs, std::vector<int> &r_rowids,
		std::vector<int> &r_colids, std::vector<double> &r_vals);
	void Global_MatrixAndRhs_Sparse(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid, std::vector<Matrix> &local_matrix,
		std::vector<double> &m_bigrhs, std::vector<std::vector<double>> &local_bigrhs, std::vector<int> &r_rowids,
		std::vector<int> &r_colids, std::vector<double> &r_vals, std::vector<std::vector<int>> &m_colids, std::vector<std::vector<std::pair<int, int>>> &m_bdyquick);
	void Global_MatrixAndRhs_Sparse_quick(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid, std::vector<Matrix> &local_matrix,
		std::vector<double> &m_bigrhs, std::vector<std::vector<double>> &local_bigrhs, std::vector<int> &r_rowids,
		std::vector<int> &r_colids, std::vector<double> &r_vals, std::vector<std::vector<int>> &m_colids, std::vector<std::vector<std::pair<int, int>>> &m_bdyquick);
	bool InVecID(int m_id, std::vector<int> &m_vecid)
	{
		for (int i = 0; i < m_vecid.size(); i++)
		{
			if (m_id == m_vecid[i])
			{
				return true;
			}
		}
		return false;
	}
	int InVecIndex(int m_id, std::vector<int> &m_vecid)
	{
		for (int i = 0; i < m_vecid.size(); i++)
		{
			if (m_id == m_vecid[i])
			{
				return i;
			}
		}
		return -1;
	}
	void SolveLinearEquation(int mtype,int GlobalSize,std::vector<int> &rows, std::vector<int> &cols,
		std::vector<double> &elems, const std::vector<double> &load_vect, std::vector<double> &m_solute);
	/*--------------线弹性------------------*/
	const double E = 1.0e5;
	const double v = 0.3;
	const double lbda = v * E / ((1 + v)*(1 - 2 * v));
	const double mu = E / (2 * (1 - v));
	void Global_MatrixAndRhs_liner(std::vector<BdyBase> &bsybase, Matrix &m_bigmatrix, std::vector<double> &m_bigrhs, std::vector<Matrix> &local_matrix,
		std::vector<std::vector<double>> &local_bigrhs);
	void Global_MatrixAndRhs_liner_quick(Matrix &m_bigmatrix, std::vector<double> &m_bigrhs, std::vector<Matrix> &local_matrix,
		std::vector<std::vector<double>> &local_bigrhs);
	double f1(double x, double y, double z)
	{
		return 1;
	}
	double f2(double x, double y, double z)
	{
		return 1;
	}
	double f3(double x, double y, double z)
	{
		return 1;
	}
	int subnum;
	int forceorder;
	double maxsize = 4;
	void Global_MatrixAndRhs_Sparse_liner(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid, std::vector<Matrix> &local_matrix,
		std::vector<double> &m_bigrhs, std::vector<std::vector<double>> &local_bigrhs, std::vector<int> &r_rowids,
		std::vector<int> &r_colids, std::vector<double> &r_vals, std::vector<std::vector<int>> &m_colids, std::vector<std::vector<std::pair<int, int>>> &m_bdyquick);
	void Global_MatrixAndRhs_Sparse_liner_quick(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid, std::vector<Matrix> &local_matrix,
		std::vector<double> &m_bigrhs, std::vector<std::vector<double>> &local_bigrhs, std::vector<int> &r_rowids,
		std::vector<int> &r_colids, std::vector<double> &r_vals, std::vector<std::vector<int>> &m_colids, std::vector<std::vector<std::pair<int, int>>> &m_bdyquick);
	void Global_MatrixAndRhs_Sparse_liner1(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid, std::vector<Matrix> &local_matrix,
		std::vector<double> &m_bigrhs, std::vector<std::vector<double>> &local_bigrhs, std::vector<int> &r_rowids,
		std::vector<int> &r_colids, std::vector<double> &r_vals, std::vector<std::vector<int>> &m_colids, std::vector<std::vector<std::pair<int, int>>> &m_bdyquick);
public:
	std::vector<std::vector<double>> material_distribution_mutires;//材料分布
	void Global_MatrixAndRhs_Sparse_liner_mutires(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid,
		std::vector<std::vector<Matrix>> &local_matrixs, std::vector<double> &m_bigrhs, std::vector<std::vector<std::vector<double>>> &local_bigrhs,
		std::vector<int> &r_rowids, std::vector<int> &r_colids, std::vector<double> &r_vals,
		std::vector<std::vector<int>> &m_colids, std::vector<std::vector<std::pair<int, int>>> &m_bdyquick,
		std::vector<std::vector<V3f>> &centers);
	void Global_MatrixAndRhs_Sparse_liner_mutires_quick(std::vector<std::vector<double>> &m_bdycoef, std::vector<BdyBase> &bdybaseid,
		std::vector<std::vector<Matrix>> &local_matrixs, std::vector<double> &m_bigrhs, std::vector<std::vector<std::vector<double>>> &local_bigrhs,
		std::vector<int> &r_rowids, std::vector<int> &r_colids, std::vector<double> &r_vals,
		std::vector<std::vector<int>> &m_colids, std::vector<std::vector<std::pair<int, int>>> &m_bdyquick);
};


class simpsort
{
public:
	double senitiv;
	int cell_id;
};

class cube4sort
{
public:
	cube4sort(int _local_id, int _global_id, int _solid_id,double _key) :local_id(_local_id), global_id(_global_id), solid_id(_solid_id)
		,key(_key) {};
public:
	int local_id;
	int global_id;
	int solid_id;
	double key;
};