/* SISL - SINTEF Spline Library version 4.4.                              */
/* Definition and interrogation of NURBS curves and surface.              */
/* Copyright (C) 1978-2005, SINTEF ICT, Applied Mathematics, Norway.      */

/* This program is free software; you can redistribute it and/or          */
/* modify it under the terms of the GNU General Public License            */
/* as published by the Free Software Foundation version 2 of the License. */

/* This program is distributed in the hope that it will be useful,        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          */
/* GNU General Public License for more details.                           */

/* You should have received a copy of the GNU General Public License      */
/* along with this program; if not, write to the Free Software            */
/* Foundation, Inc.,                                                      */
/* 59 Temple Place - Suite 330,                                           */
/* Boston, MA  02111-1307, USA.                                           */

/* Contact information: e-mail: tor.dokken@sintef.no                      */
/* SINTEF ICT, Department of Applied Mathematics,                         */
/* P.O. Box 124 Blindern,                                                 */
/* 0314 Oslo, Norway.                                                     */

/* SISL commercial licenses are available for:                            */
/* - Building commercial software.                                        */
/* - Building software whose source code you wish to keep private.        */

#include <vector>
#include <stdexcept>
#include <GoReadWrite.h>
#include <sisl.h>


using namespace std;

namespace {
    // Go-header-related info.
    const int HEADER_SIZE = 4;
    const int CURVE_INSTANCE_TYPE = 100;
    const int SURFACE_INSTANCE_TYPE = 200;
    const int POINTCLOUD_INSTANCE_TYPE = 400;
    const int MAJOR_VERSION = 1;
    const int MINOR_VERSION = 0;

    inline int determine_go_instance_type(istream& is) 
    {
	int result;
	is >> result;
	for (int dummy, i = 1; i < HEADER_SIZE; ++i)
	    is >> dummy;
	return result;
    }

    inline void read_basis(istream& is, int& n, int& k, vector<double>& knots) 
    {
	is >> n >> k;
	knots.resize(n + k);
	for (int i = 0; i < n + k; ++i) {
	     is >> knots[i];
	}
    }

    inline void write_basis(ostream& os, const int&n, const int& k, const double* knots)
    {
	os << n << ' ' << k << '\n';
	for (int i = 0; i < n + k; ++i) {
	    os << knots[i] << ' ';
	}
	os << '\n';
    }


}; // end anonymous namespace 

//===========================================================================
SISLSurf* readGoSurface(istream& go_stream)
//===========================================================================
{
    if (determine_go_instance_type(go_stream) != SURFACE_INSTANCE_TYPE) {
	throw runtime_error("stream given to readGoSurface() does not "
			    "contain surface type.");
    }
    
    // reading basic surface properties
    int kind, dim, rational;
    go_stream >> dim >> rational;
    kind = rational ? 2 : 1;
    
    // reading basis information (one basis for each parameter)
    int num_coefs_1, num_coefs_2;
    int order_1, order_2;
    vector<double> knots_1, knots_2;
    read_basis(go_stream, num_coefs_1, order_1, knots_1);
    read_basis(go_stream, num_coefs_2, order_2, knots_2);
    
    // reading control points
    int coef_size = num_coefs_1 * num_coefs_2 * (rational ? dim + 1 : dim);
    vector<double> coef(coef_size);
    for (int i = 0; i < coef_size; ++i) {
	go_stream >> coef[i];
    }
    
    int copy = 1; // flag that the input arrays should be _copied_
    return newSurf(num_coefs_1, 
		   num_coefs_2,
		   order_1,     
		   order_2, 
		   &knots_1[0], 
		   &knots_2[0],
		   &coef[0],
		   kind,
		   dim,
		   copy);
}

//===========================================================================
void writeGoSurface(SISLSurf* surf, ostream& go_stream)
//===========================================================================
{
    if (!surf) {
	throw runtime_error("zero pointer given to writeGoSurface()");
    }

    // write standard header
    go_stream << SURFACE_INSTANCE_TYPE << ' ' << MAJOR_VERSION << ' ' 
	      << MINOR_VERSION << " 0\n";

    // write basic surface properties
    const int& dim = surf->idim;
    const int rational = (surf->ikind % 2 == 0) ? 1 : 0;
    go_stream << dim << ' ' << rational << '\n';

    // writing basis information
    write_basis(go_stream, surf->in1, surf->ik1, surf->et1);
    write_basis(go_stream, surf->in2, surf->ik2, surf->et2);
    
    // writing control points
    int coef_size = surf->in1 * surf->in2 * (rational ? dim + 1 : dim);
    const double* coef_pointer = rational ? surf->rcoef : surf->ecoef;
    for (int i = 0; i < coef_size; ++i) {
	go_stream << coef_pointer[i] << ' ';
    }
    go_stream << endl;
}


//===========================================================================
void writeGoCurve(SISLCurve* curve, std::ostream& go_stream)
//===========================================================================
{
    if (!curve) {
	throw runtime_error("zero pointer given to writeGoCurve()");
    }

    // write standard header
    go_stream << CURVE_INSTANCE_TYPE << ' ' << MAJOR_VERSION << ' ' 
	      << MINOR_VERSION << " 0\n";

    // write basic curve properties
    const int& dim = curve->idim;
    const int rational = (curve->ikind % 2 == 0) ? 1 : 0;
    go_stream << dim << ' ' << rational << '\n';

    // write bspline basis information
    write_basis(go_stream, curve->in, curve->ik, curve->et);
   
    // write control points
    int coef_size = curve->in * (rational ? (dim + 1) : dim);
    const double* coef_pointer = rational ? curve->rcoef : curve->ecoef;
    for (int i = 0; i < coef_size; ++i) {
	go_stream << coef_pointer[i] << ' ';
    }
    go_stream << endl;
}


//===========================================================================
SISLCurve* readGoCurve(istream& go_stream)
//===========================================================================
{
    if (determine_go_instance_type(go_stream) != CURVE_INSTANCE_TYPE) {
	throw runtime_error("stream given to readGoCurve() does not contain curve.");
    }
    
    // reading basic curve properties
    int kind, dim, rational;
    go_stream >> dim >> rational;
    kind = rational ? 2 : 1;

    // reading bspline basis information
    int num_coefs, order;
    vector<double> knots;
    read_basis(go_stream, num_coefs, order, knots);
    
    // reading control points
    int coef_size = num_coefs * (rational ? (dim + 1) : dim);
    vector<double> coef(coef_size);
    for (int i = 0; i < coef_size; ++i) {
	go_stream >> coef[i];
    }

    int copy = 1; // flag that the input arrays (knots, coef) should be _copied_
    return newCurve(num_coefs, order, &knots[0], &coef[0], kind, dim, copy);

}


//===========================================================================
void writeGoPoints(int num_points, double* coords, std::ostream& go_stream)
//===========================================================================
{
    if (!coords) {
	throw runtime_error("zero coordinate pointer given to writeGoPoints.");
    }
    // write standard header
    go_stream << POINTCLOUD_INSTANCE_TYPE << ' ' << MAJOR_VERSION << ' '
	      << MINOR_VERSION << " 4 255 255 0 255\n";

    // write the number of points
    go_stream << num_points << '\n';
    
    // write point coordinates
    for (int i = 0; i < num_points * 3; ++i) {
	go_stream << coords[i];
	if ((i+1) % 3) {
	    go_stream << ' ';
	} else {
	    go_stream << '\n';
	}
    }
    go_stream << endl;
}

//===========================================================================
void readGoPoints(std::vector<double>& coords, std::istream& go_stream)
//===========================================================================
{
    if (determine_go_instance_type(go_stream) != POINTCLOUD_INSTANCE_TYPE) {
	throw runtime_error("stream given to readGoPoints() does not contain pointcloud.");
    }
    // read away extra header information
    for (int dummy, i = 0; i < 4; ++i) {
	go_stream >> dummy;
    }

    int num_points;
    go_stream >> num_points;
    coords.resize(num_points * 3);

    // read point coordinates
    for (int i = 0; i < coords.size(); ++i) {
	go_stream >> coords[i];
    }
}
