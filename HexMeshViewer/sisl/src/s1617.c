/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* (c) Copyright 1989,1990,1991,1992 by                                      */
/*     Senter for Industriforskning, Oslo, Norway                            */
/*     All rights reserved. See the sisl-copyright.h for more details.            */
/*                                                                           */
/*****************************************************************************/

#include "sisl-copyright.h"

/*
 *
 * $Id: s1617.c,v 1.2 2001-03-19 15:58:52 afr Exp $
 *
 */


#define S1617

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void s1617(double epoint[], int inbpnt, int idim, int eptyp[],
	   double aepsge, double econic[],
	   double estart[], double etang[], double estop[],
	   double *ashape, int *jstat)
#else
void s1617(epoint, inbpnt, idim, eptyp, aepsge, econic,
	   estart, etang, estop, ashape, jstat)
     double epoint[];
     int    inbpnt;
     int    idim;
     int    eptyp[];
     double aepsge;
     double econic[];
     double estart[];
     double etang[];
     double estop[];
     double *ashape;
     int    *jstat;
#endif
/*
*************************************************************************
*
* PURPOSE: To produce interpolation conditions to interpolate the
*          conic curve described by Epoint, Eptyp and Econic.
*
* INPUT:
*        Epoint - The points/tangents describing the conic.
*        Inbpnt - No. of points/tangents in the epoint array.
*        Idim   - The dimension of the space in which the points lie.
*        Eptyp  - Type indicator for the points/tangents :
*                  1 - Ordinary point.
*                  2 - Knuckle point. (Is treated as an ordinary point.)
*                  3 - Tangent to next point.
*                  4 - Tangent to prior point.
*        Aepsge - The geometry reolution.
*        Econic - The conic coefficients of the points in the Epoint array.
*
* Output:
*        Estart - Start-point of conic segment.
*        Etang  - Intersection point of tangents from start- and endpoint.
*        Estop   - End-point of conic segment.
*        Ashape - Shape factor.
*        Jstat  - Status variable.
*
* Method:
* 	The conics are classified into ellipses, parabolas and hyperbolas.
* 	The description of the arc by the output type of parameters is produced.
*-
* Calls: s1619,s6err.
*
* Written by: A.M. Ytrehus, si Oslo, Oct.91.
* After FORTRAN, (P1617), written by: T. Dokken  SI.
*****************************************************************
*/
{
  int kk, kp;
  double ta11, ta12, ta13, ta22, ta23, ta33;
  double td13, td23, td33, tda;
  double tshape;
  int ktyp;
  int kstat = 0;
  int kpos = 0;

  *jstat = 0;


  /* Initiate variables. */

  ta11 = econic[0];
  ta12 = econic[1];
  ta13 = econic[3];		/* ? */
  ta22 = econic[2];		/* ? */
  ta23 = econic[4];
  ta33 = econic[5];


  /* Calculate determinants. */

  td13 = ta12 * ta23 - ta22 * ta13;
  td23 = ta11 * ta23 - ta12 * ta13;
  td33 = ta11 * ta22 - ta12 * ta12;
  tda = ta13 * td13 - ta23 * td23 + ta33 * td33;


  /* Discussion of the conic. */

  if (DEQUAL(tda + (double) 1.0, (double) 1.0))
    {
      /* Degenerate conic. Produce straight line. */

      *jstat = 1;
      goto out;
    }

  /* Decide if ellipse, parabola or hyperbola. */

  if (td33 > (double) 0.0)
    {
      /* Ellipse, decide if imaginary or real. */

      if (tda * ta11 > (double) 0.0)
	{
	  /* Imaginary ellipse. Produce straight line. */

	  *jstat = 1;
	  goto out;
	}
      else
	{
	  /* Real eelipse. */

	  ktyp = 2;
	}
    }
  else
    {
      /* Parabola or hyperbola. */

      if (td33 < (double) 0.0)
	{
	  /* Hyperbola. */

	  ktyp = 4;
	}
      else
	{
	  /* Parabola. */

	  ktyp = 3;
	}
    }


  /* Find rational description of conic. */

  s1619 (epoint, inbpnt, idim, eptyp, econic, ktyp,
	 etang, &tshape, &kstat);
  if (kstat < 0)
    goto error;
  if (kstat == 1)
    {
      /* Create straight line. */

      *jstat = 1;
      goto out;
    }

  kk = idim * (inbpnt - 1);

  for (kp = 0; kp < idim; kp++)
    {
      estart[kp] = epoint[kp];
      estop[kp] = epoint[kk + kp];
    }

  *ashape = tshape;

  goto out;


  /* Error in lower level routine. */

error:
  *jstat = kstat;
  s6err ("s1617", *jstat, kpos);
  goto out;

out:

  return;
}
