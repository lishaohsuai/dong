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
 * $Id: s1382.c,v 1.2 2001-03-19 15:58:48 afr Exp $
 *
 */


#define S1382

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
s1382 (SISLSurf * psurf, double eview[], int idim, SISLSurf ** rsurf,
       int *jstat)
#else
void
s1382 (psurf, eview, idim, rsurf, jstat)
     SISLSurf *psurf;
     double eview[];
     int idim;
     SISLSurf **rsurf;
     int *jstat;
#endif
/*
*********************************************************************
*
* PURPOSE    : To make the function whose zeroes describes the silhouette
*              lines of a surface. The silhouette lines are described
*              by a viewing direction.
*
* INPUT      : psurf  - Pointer to input surface.
*              eview  - Viewing direction.
*              idim   - Dimension of the space in which the surface lies.
*
* OUTPUT     : rsurf  - The resulting surface
*              jstat  - status messages
*                                         > 0      : warning
*                                         = 0      : ok
*                                         < 0      : error
*
* METHOD     : We first make the appropriate knot vector, then we calulate
*              parameter values for the interpolation, then the normal
*              vectors of the surface are calculated at these parameter
*              values. The scalar product of the normal vector and the
*              viewing direction is made and these values are interpolated
*              to make the function describing the silhouette lines.
*
*              For nonrational surfaces the function whose zeroes form the
*              silhouette is
*
*                      f = N . eview = (P xP ) . eview
*                                        U  V
*
*              which, if P is degree m x n, is degree 2m-1 x 2n-1
*              or order 2(m+1)-2 x 2(n+1)-2.
*              In the rational case, setting  P(U,V) = T(U,V) / w(U,V), we find
*
*                                                     4
*                      P xP =(wT -w T) x (wT -w T) / w
*                       U  V    U  U        V  V
*
*                                                      3
*                           =(wT xT +w T xT+w TxT ) / w
*                               U  V  U V    V   U
*
*              so we find the zeroes of
*
*                      f = (wT xT +w T xT+w TxT ) . eview
*                             U  V  U V    V   U
*
*              which, if P is degree m x n, is degree 3m-1 x 3n-1
*              or order 3(m+1)-3 x 3(n+1)-3.
*
* REFERENCES :
*
* CALLS      : s1381,s1890,s1421,s6crss,c6scpr,s1891,s6err.
*
* WRITTEN BY : Tor Dokken, SI, 1988-11
* REVISED BY : Mike Floater, SI, 1991-04 for rational surfaces
*
*********************************************************************
*/
{
  int kn1;			/* Number of vertices of psurf in first par.dir     */
  int kk1;			/* Order in  psurf in first par.dir                 */
  int kn2;			/* Number of vertices of psurf in second par.dir    */
  int kk2;			/* Order in  psurf in second par.dir                */
  int kjkk1;			/* Order of interpolated basis in first par.dir     */
  int kjkn1;			/* Number of vertices in interpolated basis first.dr*/
  int kjkk2;			/* Order of interpolated basis in first par SISLdir */
  int kjkn2;			/* Number of vertices in interpolated basis secnd.dr*/
  int kdim;			/* Number of dimesions in psurf                     */
  int kstat = 0;		/* Local status variable                            */
  int kpos = 0;			/* Position indicator for errors                    */
  int kzero = 0;		/* Value 0 needed in call s1891   	          */
  int kone = 1;			/* Value 1 needed in call s1891		          */
  int cuopen;			/* Used as a logical parameter			  */
  int ki, kj;			/* Loop control variable                            */
  int kp;			/* Index of points put into conic equation          */
  int klfs = 0;			/* Pointer into knot vector                         */
  int klft = 0;			/* Pointer into knot vector                         */
  double *st1 = SISL_NULL;		/* First knot vector is psurf                       */
  double *st2 = SISL_NULL;		/* Second knot vector is psurf                      */
  double *sval1 = SISL_NULL;		/* Array of values of surface put into torus eq.    */
  double *sval2 = SISL_NULL;
  double *sval3 = SISL_NULL;
  double *sgt1 = SISL_NULL;		/* Knot vector in first parameter direction of
				   surface put into torus equation                  */
  double *sgt2 = SISL_NULL;		/* Knot vector in second parameter direction of
				   surface put into torus equation                  */
  double sder[12];		/* SISLPoint on the surface                             */
  double spar[2];		/* Current parameter pair                           */
  double snorm[3];		/* Normal vector                                    */
  int i;			/* a loop variable                                  */
  int ikind;			/* kind of surface psurf is                         */
  double tutv[3];		/* T_u x T_v  in rational case                      */
  double tvt[3];		/* T_v x T    in rational case                      */
  double ttu[3];		/* T   x T_u  in rational case                      */
  double *par1 = SISL_NULL;
  double *par2 = SISL_NULL;
  int *der1 = SISL_NULL;
  int *der2 = SISL_NULL;
  SISLSurf *tempsurf = SISL_NULL;	/* only used for rational surfaces                  */

  *jstat = 0;

  if (idim != psurf->idim)
    goto err104;

  /* Make local pointers */

  kn1 = psurf->in1;
  kk1 = psurf->ik1;
  kn2 = psurf->in2;
  kk2 = psurf->ik2;
  kdim = psurf->idim;
  st1 = psurf->et1;
  st2 = psurf->et2;
  ikind = psurf->ikind;


  /* Test input */

  if (kdim != 3)
    goto err104;

  if (ikind == 2 || ikind == 4)
    {
      /* A tricky way to evaluate the derivatives of the HOMOGENEOUS form
	 of psurf. In other words we need the derivs of T(u,v) where
	 p(u,v) = T(u,v) / w(u,v).
	 We should really have a separate evaluator for this
	 but I didn't want to mess around with the existing evaluator
	 which does the division automatically. MF 16/4/91 */

      tempsurf = newSurf (kn1, kn2, kk1, kk2, st1, st2, psurf->rcoef,
			  ikind - 1, kdim + 1, 0);
      if (tempsurf == SISL_NULL)
	goto err171;
      tempsurf->cuopen_1 = psurf->cuopen_1;
      tempsurf->cuopen_2 = psurf->cuopen_2;

      kjkk1 = 3 * kk1 - 3;
      kjkk2 = 3 * kk2 - 3;
    }
  else
    {
      tempsurf = psurf;
      kjkk1 = 2 * kk1 - 2;
      kjkk2 = 2 * kk2 - 2;
    }


  /* Make description of knot array for interpolation in first parameter
     direction */

  s1381 (st1, kn1, kk1, &sgt1, &kjkn1, kjkk1, &kstat);
  if (kstat < 0)
    goto error;


  /* Make parameter values and derivative indicators */

  s1890 (sgt1, kjkk1, kjkn1, &par1, &der1, &kstat);
  if (kstat < 0)
    goto error;


  /* Make description of knot array for interpolation in second parameter
     direction
     */

  s1381 (st2, kn2, kk2, &sgt2, &kjkn2, kjkk2, &kstat);
  if (kstat < 0)
    goto error;


  /* Make parameter values and derivative indicators */

  s1890 (sgt2, kjkk2, kjkn2, &par2, &der2, &kstat);
  if (kstat < 0)
    goto error;


  /* Allocate array for values of surface put into torus equation */

  sval1 = newarray (kjkn1 * kjkn2, DOUBLE);
  if (sval1 == SISL_NULL)
    goto err101;


  /* Calculate values to be interpolated */

  /* Index of point to be stored */

  kp = 0;

  for (kj = 0; kj < kjkn2; kj++)
    {

      spar[1] = par2[kj];

      for (ki = 0; ki < kjkn1; ki++)
	{
	  /* Calculate values on 3-D surface */

	  spar[0] = par1[ki];

	  s1421 (tempsurf, 1, spar, &klfs, &klft, sder, snorm, &kstat);
	  if (kstat < 0)
	    goto error;

	  if (ikind == 2 || ikind == 4)
	    {
	      /* Calculate f = (wT xT +w T xT+w TxT ) . eview
	                          U  V  U V    V   U
                 instead of normal to surface in rational case. */

	      s6crss (sder + 4, sder + 8, tutv);
	      s6crss (sder + 8, sder, tvt);
	      s6crss (sder, sder + 4, ttu);

	      for (i = 0; i < 3; i++)
		{
		  snorm[i] = sder[3] * tutv[i] + sder[7] * tvt[i] +
		    sder[11] * ttu[i];
		}
	    }

	  /* Make scalar product of normal vector and eview */

	  sval1[kp++] = s6scpr (snorm, eview, kdim);
	}
    }

  cuopen = TRUE;

  /* Interpolate in second parameter direction, the first parameter direction
     is treated as a point of dimension kjkn1 */

  s1891 (par2, sval1, kjkn1, kjkn2, kone, der2, cuopen, sgt2, &sval2,
	 &kjkn2, kjkk2, kzero, kzero, &kstat);
  if (kstat < 0)
    goto error;


  /* Interpolate in first parameter direction, perform kjkn2 interpolations
     of one dimensional data */

  s1891 (par1, sval2, kone, kjkn1, kjkn2, der1, cuopen, sgt1, &sval3,
	 &kjkn1, kjkk1, kzero, kzero, &kstat);
  if (kstat < 0)
    goto error;

  *rsurf = SISL_NULL;
  *rsurf = newSurf (kjkn1, kjkn2, kjkk1, kjkk2, sgt1, sgt2, sval3, 1, 1, 1);
  if (*rsurf == SISL_NULL)
    goto err171;
  (*rsurf)->cuopen_1 = psurf->cuopen_1;
  (*rsurf)->cuopen_2 = psurf->cuopen_2;

  /* Ok ! */

  goto out;


  /* Error in lower level function */

error:
  *jstat = kstat;
  s6err ("s1382", *jstat, kpos);
  goto out;

  /* Error in space allocation */
err101:
  *jstat = -101;
  s6err ("s1382", *jstat, kpos);
  goto out;

  /* Dimension not equal to 3 or conflicitng dim   */

err104:
  *jstat = -104;
  s6err ("s1382", *jstat, kpos);
  goto out;

  /* Could not create surface. */

err171:
  *jstat = -171;
  s6err ("s1382", *jstat, kpos);
  goto out;

out:

  /* Release allocated arrays */

  if (sgt1 != SISL_NULL)
    freearray (sgt1);
  if (sgt2 != SISL_NULL)
    freearray (sgt2);
  if (sval1 != SISL_NULL)
    freearray (sval1);
  if (sval2 != SISL_NULL)
    freearray (sval2);
  if (sval3 != SISL_NULL)
    freearray (sval3);
  if (par1 != SISL_NULL)
    freearray(par1);
  if (par2 != SISL_NULL)
    freearray(par2);
  if (der1 != SISL_NULL)
    freearray(der1);
  if (der2 != SISL_NULL)
    freearray(der2);
  if ((ikind == 2 || ikind == 4) && (tempsurf != SISL_NULL))
    freeSurf (tempsurf);

  return;
}
