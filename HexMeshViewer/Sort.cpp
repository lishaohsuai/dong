#include "Sort.h"
void Sort::sortmesh()
{
	std::vector<V3f> mv = mesh->vertices_;
	int i = 0;
	for (std::vector<V3f>::iterator mv_it = mv.begin();mv_it != mv.end();mv_it++)
	{
		newV3f tempmv(*mv_it, VertexHandle(i));
		sm.push_back(tempmv);
		i++;
	}
}

void Sort::sortmesh1()
{
	std::vector<V3f> mv1 = mesh1->vertices_;
	int i = 0;
	for (std::vector<V3f>::iterator mv1_it = mv1.begin();mv1_it != mv1.end();mv1_it++)
	{
		newV3f tempmv1(*mv1_it, VertexHandle(i));
		sm1.push_back(tempmv1);
		i++;
	}
}

void Sort::sorting()
{
	sortmesh();
	sortmesh1();
}

void Sort::comparing()
{
	//std::vector<newV3f>::iterator sm_id = sm.begin();
	//std::vector<newV3f>::iterator sm1_id = sm1.begin();
	double averr = 0;
	max = 0;
	min = 10;
	int num = sm.size();
	for (std::vector<newV3f>::iterator sm_id = sm.begin();sm_id != sm.end();sm_id++)
	{
		V3f v = (*sm_id).v;
		double error = 10;
		for (std::vector<newV3f>::iterator sm1_id = sm1.begin();sm1_id != sm1.end();sm1_id++)
		{
			V3f v1 = (*sm1_id).v;
			double err = sqrt((v.x - v1.x)*(v.x - v1.x) + (v.y - v1.y)*(v.y - v1.y) + (v.z - v1.z)*(v.z - v1.z));
			if (err < error)
				error = err;
		}
		(*sm_id).error = error;
		color.insert((*sm_id));
		averr += error;
		if (error > max)
			max = error;
		if (error < min)
			min = error;
		if (error > 0.03)
		{
			drawvbigid.push_back((*sm_id).vid);
			drawvbig.push_back(v);
		}
		else if (error > 0.05)
			drawvsmall.push_back(v);
	}
	averr /= num;
	for (std::set<newV3f>::iterator color_it = color.begin();color_it != color.end();color_it++)
	{
		colorverr.push_back((*color_it).error);
	}


	/*int num = sm.size();
	double max = 0;
	double min = 10;
	double average = 0;
	for (int i = 0;i < num;i++)
	{
		V3f smv = (*sm_id).v;
		V3f sm1v = (*sm1_id).v;
		double error = sqrt((smv.x - sm1v.x)*(smv.x - sm1v.x) + (smv.y - sm1v.y)*(smv.y - sm1v.y) + 
			(smv.z - sm1v.z)*(smv.z - sm1v.z));
		average += error;
		if (error > max)
			max = error;
		if (error < min)
			min = error;
		if (error > 5)
		{
			int a = 1;
		}
		if (error > 0.5)
		{
			drawvbig.push_back(smv);
		}
		else if (error > 0.3)
		{
			drawvsmall.push_back(smv);
		}
		sm_id++;
		sm1_id++;
	}
	average /= num;*/
}

void Sort::comparing1()
{
	averr = 0;
	max = 0;
	min = 10;
	int num = sm.size();
	double fenzi = 0, fenmu = 0;
	for (std::vector<newV3f>::iterator sm_id = sm.begin(); sm_id != sm.end(); sm_id++)
	{
		V3f v = (*sm_id).v;
		double error = 10;
		int vid = 0;
		for (std::vector<newV3f>::iterator sm1_id = sm1.begin(); sm1_id != sm1.end(); sm1_id++)
		{
			V3f v1 = (*sm1_id).v;
			double err = sqrt((v.x - v1.x)*(v.x - v1.x) + (v.y - v1.y)*(v.y - v1.y) + (v.z - v1.z)*(v.z - v1.z));
			if (err < error)
			{
				vid = (*sm1_id).vid;
				error = err;
			}		
		}
		(*sm_id).error = fabs(m_color1[(*sm_id).vid] - m_color2[vid]);
		colorverr.push_back((*sm_id).error);
		averr += (*sm_id).error;
		if (error > max)
			max = error;
		if (error < min)
			min = error;
		fenzi += (m_color1[(*sm_id).vid] - m_color2[vid])*(m_color1[(*sm_id).vid] - m_color2[vid]);
		fenmu += m_color1[(*sm_id).vid] * m_color1[(*sm_id).vid];
	}
	averr /= num;
	relerr = sqrt(fenzi / fenmu);
}