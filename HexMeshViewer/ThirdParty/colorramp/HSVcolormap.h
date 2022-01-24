
#ifndef _HSV_COLOR_MAP_
#define _HSV_COLOR_MAP_

class   HSVColor
{
public:

	HSVColor();
	~HSVColor();

	void  set_value_range(double maxv, double minv);// set range of value
    void  value2rgb(double value, double& r, double & g, double& b);// convert a value t RGB
    void  adjust_window(double wmin, double wmax);// the whole window is [0,1]
private:

	void hsv2rgb(double &cH, double &cS, double &cV, double &cR, double &cG, double &cB);
	double min_v,max_v;
	double   bright;
	double   saturation;
	double   hue;
};
#endif