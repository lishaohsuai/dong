

#include "HSVcolormap.h"
#include <math.h>

HSVColor::HSVColor()
{
	bright = 0.8;// brightness
	saturation = 0.7;

}

HSVColor::~HSVColor()
{
}

void HSVColor::adjust_window(double wmin, double wmax)
{
	double old_max = max_v;
	double old_min = min_v;

	max_v = (old_max - old_min) * wmax + old_min;;
    min_v = (old_max - old_min) * wmin +old_min;

}

void HSVColor::set_value_range(double _max,  double _min)
{
	max_v = _max;
	min_v = _min;
}

void HSVColor::value2rgb(double value, double& r, double& g, double & b)
{
	double hu;

	if(value  < min_v)  hu = 0;
	else if(value > max_v)  hu  = 240;
	else hu = (value - min_v)/(max_v - min_v) * 240;

    // convert to [0, 240]
	hu  = 240 - hu;
	hsv2rgb(hu, saturation, bright, r,g,b);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HSVColor::hsv2rgb(double &cH, double &cS, double &cV, double &cR, double &cG, double &cB)
{
	double f, p, q, t;
	int hi;

	hi = (int)floor(cH/60) % 6;
	f = cH/60-hi;
	p = cV * (1-cS);
	q = cV *(1-cS*f);
	t = cV *(1-cS*(1-f));
	switch(hi)
	{
	case 0:
		cR = cV; cG = t; cB= p;
		break;
	case 1:
		cR = q; cG = cV; cB= p;
		break;
	case 2:
		cR = p; cG = cV; cB= t;
		break;
	case 3:
		cR = p; cG = q; cB= cV;
		break;
	case 4:
		cR = t; cG = p; cB= cV;
		break;
	case 5:
		cR = cV; cG = p; cB= q;
		break;
	default:
		break;
	}
}


