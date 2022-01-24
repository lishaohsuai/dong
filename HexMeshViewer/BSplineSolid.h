#pragma once
#include "GeometryKernel.h"
#include "mkl.h"

typedef std::vector< std::vector<double> > Matrix;

class BSplineSolid
{
public:
	std::vector<HexV3fMesh> Local_BSplineSolid;//�ֲ���С��ÿ����Ԫ
	std::vector<std::vector<std::vector<std::vector<VertexHandle>>>> Control_Point_id;//ÿһ���鰴˳��������Ƶ���±�
	std::vector<std::vector<VertexHandle>> CP_Local_to_Global;//�ֲ���С��Ԫ��ÿ�����Ӧ��ȫ��ģ���еĵ�
	std::vector<std::vector<int>> Local_Order;//�ֲ���ÿ��С��Ԫ��u,v,w��������Ĵ���
	std::vector<std::vector<std::vector<double>>> Local_Node;//�ֲ���ÿ��С��Ԫ��u,v,w��������Ľڵ�����
public:
	HexV3fMesh Global_BSplineSolid;//ȫ�ֵĴ������ģ��
public://�µĽṹ
	std::vector<std::vector<std::vector<std::vector<int>>>> Control_Point;//ÿһ��С��Ŀ��Ƶ�
	std::vector<std::vector<V3f>> Local_V3f;
	std::vector<std::vector<int>> Local_to_Global;
	std::vector<std::vector<bool>> local_bdy;
	int Global_size;
	int Global_size_without_bdy;
public:
	BSplineSolid() {};
	~BSplineSolid()
	{
		Local_BSplineSolid.swap(std::vector<HexV3fMesh>());
		Control_Point_id.swap(std::vector<std::vector<std::vector<std::vector<VertexHandle>>>>());
		CP_Local_to_Global.swap(std::vector<std::vector<VertexHandle>>());
		Local_Order.swap(std::vector<std::vector<int>>());
		Local_Node.swap(std::vector<std::vector<std::vector<double>>>());

		Control_Point.swap(std::vector<std::vector<std::vector<std::vector<int>>>>());
		Local_V3f.swap(std::vector<std::vector<V3f>>());
		Local_to_Global.swap(std::vector<std::vector<int>>());
		local_bdy.swap(std::vector<std::vector<bool>>());
	}
	void init(int n)//��ʼ��BSplineSolid
	{
		Local_BSplineSolid.resize(n);
		Control_Point_id.resize(n);
		CP_Local_to_Global.resize(n);
		Local_Order.resize(n);
		Local_Node.resize(n);

		Control_Point.resize(n);//ÿһ��С��Ŀ��Ƶ�
		Local_V3f.resize(n);
		Local_to_Global.resize(n);
		local_bdy.resize(n);
		Global_size = 0;
		Global_size_without_bdy = 0;
	}
	BSplineSolid& operator=(const BSplineSolid& bsplinesolid)
	{
		this->Local_BSplineSolid = bsplinesolid.Local_BSplineSolid;
		this->Control_Point_id = bsplinesolid.Control_Point_id;
		this->CP_Local_to_Global = bsplinesolid.CP_Local_to_Global;
		this->Local_Order = bsplinesolid.Local_Order;
		this->Local_Node = bsplinesolid.Local_Node;
		this->Global_BSplineSolid = bsplinesolid.Global_BSplineSolid;

		this->Control_Point = bsplinesolid.Control_Point;
		this->Local_V3f = bsplinesolid.Local_V3f;
		this->Local_to_Global = bsplinesolid.Local_to_Global;
		this->local_bdy = bsplinesolid.local_bdy;
		this->Global_size = bsplinesolid.Global_size;
		this->Global_size_without_bdy = bsplinesolid.Global_size_without_bdy;
		return *this;
	}
public:
	//���x��T�е�λ��
	int X_LOCATION(int solid_id,int uvw, double x);
	//���㼸�α任.X=G(u,v),Ҳ����������u,v��ֵ��
	V3f Geometry(int solid_id, double u, double v, double w);
	//m��ʾ�ڵ�����������±꣬���ڵ���������m+1���ڵ㣬i��ʾ��Ni��i��0��ʼ�㣩��
	//n��ʾ�󵼵�n�Σ������ders�У�ders��n+1��Ԫ�أ���
	//void DersOneBasisFun(int degree,int m,vector<double> U, int i,double u,int n,double ders[])
	void DersOneBasisFun(int solid_id, int uvw, int i, double u, int n, double ders[]);
	//�Ա߽纯�����������ϵ����m_bdy[0]=1��ʾu����m_bdy[1]=1��ʾv����m_bdy[2]=1��ʾw����, m_para��ʾ��һ������Ĳ���ֵ
	void SolveCoef(int solid_id, std::vector<double> m_scoef, std::vector<int> m_para, std::vector<int> m_bdy, std::vector<double> &m_coef);
	//Ѱ�ҽ���������������������
	void IntegralArea(int solid_id, std::vector<double> &u_interval, std::vector<double> &v_interval, std::vector<double> &w_interval);
	//����һϵ�е��ϵ��ſɱȾ����ת�õ��漰����ʽ
	void JacobiMatrix(int solid_id, std::vector<double> u, std::vector<double> v, std::vector<double> w,
		std::vector<double> &jacobidet, std::vector<Matrix> &jacobi);
	double ExactSol(double x, double y, double z);
	double RHS(double x, double y, double z);
	double RHS1(double u, double v, double w);
public:
	void JacobiMatrix_new(int solid_id, std::vector<double> u, std::vector<double> v, std::vector<double> w,
		std::vector<double> &jacobidet, std::vector<Matrix> &jacobi);
	V3f Geometry_new(int solid_id, double u, double v, double w);
public:
	/*-----------------�����PDE-----------------*/
	const int U = 0, V = 1, W = 2;
	void JacobiMatrixFace(int solid_id, int uvw, int uvw_id, std::vector<double> u, std::vector<double> v,
		std::vector<double> &jacobidet, std::vector<Matrix> &jacobi);
};

