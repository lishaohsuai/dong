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
 * $Id: s1347.c,v 1.2 2001-03-19 15:58:46 afr Exp $
 *
 */

#define S1347

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void s1347(double ep[],double etang1[],double etang2[],double eder11[],
	   int im1,int im2,int idim,int ipar,double epar1[],double epar2[],
	   double eeps[],int nend[],double edgeps[],double aepsco,int iopt,
	   int itmax,SISLSurf **rs,double emxerr[],int *jstat)
#else
void s1347(ep,etang1,etang2,eder11,im1,im2,idim,ipar,epar1,epar2,
	   eeps,nend,edgeps,aepsco,iopt,itmax,rs,emxerr,jstat)
     double ep[];
     double etang1[];
     double etang2[];
     double eder11[];
     int    im1;
     int    im2;
     int    idim;
     int    ipar;
     double epar1[];
     double epar2[];
     double eeps[];
     int    nend[];
     double edgeps[];
     double aepsco;
     int    iopt;
     int    itmax;
     SISLSurf   **rs;
     double emxerr[];
     int    *jstat;
#endif
/*
********************************************************************
*
* Purpose: To compute a bicubic hermite spline-approximation to the
*          position and derivative data given by ep,etang1,etang2
*          and eder11.
*
* Input : Ep     - Array (length idim*im1*im2) containing the points
*                  to be approximated.
*         Etang1 - Array (length idim*im1*im2) containing the derivatives
*                  (tangents) in the first parameter-direction at the
*                  data-points.
*         Etang2 - Array (length idim*im1*im2) containing the derivatives
*                  (tangents) in the second parameter-direction at the
*                  data-points.
*         Eder11 - Array (length idim*im1*im2) containing the cross (twist)
*                  derivatives at the data-points.
*         Im1    - The no. of points in the first parameter-direction.
*         Im2    - The no. of points in the second parameter-direction.
*         Ipar   - Flag determining the parametrization of the data-points.
*                   = 1: Mean accumulated cord length parametrization.
*                   = 2: Uniform parametrization.
*                   = 3: Parametrization given by epar1 and epar2.
*         Epar1  - Array (length im1) containing a parametrization in the
*                  first parameter-direction. (Will only be used if ipar=3.)
*         Epar2  - Array (length im2) containing a parametrization in the
*                  surface lies.)
*         Eeps   - Array (length idim) containing the maximum deviation
*                  which is acceptable in each of the idim components of
*                  the surface (except possibly along the edges).
*         Nend   - Array (length 4) containing the no. of derivatives 
*                  to be kept fixed along each edge of the surface.
*                  The numbering of the edges is the same as for edeps below.
*                  All the derivatives of order < nend(i)-1 will be kept fixed
*                  along edge no. i. Hence nend(i)=0 indicates that nothing
*                  is to be kpet fixed along edge no. i.
*                  To be kept fixed here means to have error less than edgeps.
*                  In general, it is impossible to remove any knots and
*                  keep an edge completely fixed.
*         Edgeps - Array (length idim*4) containing the max. deviation
*                  which is acceptable along the edges of the surfaces.
*                  Edgeps(1,i):edgeps(idim,i) gives the tolerance along
*                  the edge corresponding to the i-th parameter having
*                  its min. or max value.
*                  i=1 : min value of first parameter.
*                  i=2 : max value of first parameter.
*                  i=3 : min value of second parameter.
*                  i=4 : max value of second parameter.
*                  Edgeps(kp,i) will only have any significance if nend(i)>0.
*         Aepsco - Two numbers differing by a relative amount less than
*                  aepsco will in some cases be considered equal.
*                  A suitable value is just above the unit roundoff
*                  of the machine. On VAX, 1.0E-6 is a reasonable choice.
*                  The computations are not guaranteed to have relative
*                  accuracy less than aepsco.
*         Iopt   - Flag indicationg the order in which tha data-reduction
*                  is to be performed.
*                   = 1 : Remove knots in parameter-direction 1 only.
*                   = 2 : Remove knots in parameter-direction 2 only.
*                   = 3 : Remove knots in parameter-direction 1 and
*                         and then in parameter-direction 2.
*                   = 4 : Remove knots in parameter-direction 2 and
*                         and then in parameter-direction 1.
*         Itmax  - Max. no. of iteration.
*
* Ouput:  Jstat  - Output status:
*                   < 0 : Error.
*                   = 0 : Ok.
*                   > 0 : Warning.
*         Rs     - Pointer to surface.
*         Emxerr - Array (length idim) (allocated outside this routine.)
*                  containing an upper bound for the error comitted in
*                  each component during the data reduction.
*
* Method: First the bicubic hermite spline-interpolant is computed
*         using the appropriate parametrization, and then knots
*         are removed from this approximation by a call to the
*         data-reduction routine for surfaces.
*
* Calls: s1530, s1345, s6err.
*
* Written by: C.R.Birkeland, Si, Oslo, Norway, May 1993.
* The datareduction routine, s1345, is written by: Knut M|rken,  SI.
**********************************************************************
*/
{
  int stat=0, kpos=0;         /* Error message parameters        */
  double *par1 = SISL_NULL;        /* Used to store parametrizations  */
  double *par2 = SISL_NULL;
  SISLSurf *osurf = SISL_NULL;     /* Hermite interp. surface         */ 

  /* Check Input */

  if (im1 < 2 || im2 < 2 || idim < 1) 
    goto err103; 
  if (ipar < 1 || ipar > 3) ipar = 1;

  /* Generate parametrization */

  if (ipar != 3) 
    { 
      /* Generate parametrization */
      
      s1528(idim, im1, im2, ep, ipar, SISL_CRV_OPEN, SISL_CRV_OPEN,
	    &par1, &par2, &stat);
      if (stat<0) goto error;
    }
  else 
    {
      /* Parametrization is passed as parameter */

      par1 = epar1;
      par2 = epar2;
    } 

  /* Perform bicubic hermite spline interpolation */

  s1530(ep, etang1, etang2, eder11, par1, par2,
	im1, im2, idim, &osurf, &stat);
  if (stat<0) goto error;

  /* Perform final datareduction step */

  s1345(osurf, eeps, nend, edgeps, aepsco, iopt, itmax,  
        rs, emxerr, &stat);
  if (stat<0) goto error;
  
  /* Success */

  *jstat = 0;
  goto out;
  
  
  /* Error in input */

  err103: 
    *jstat = -103;
    s6err("s1347",*jstat,kpos);
    goto out;
  
  /* Error in lower level routine. */

  error: 
    *jstat = stat;
    s6err("s1347",*jstat,kpos);
    goto out;
  
  /* Exit. */

  out:
    /* Free SISL-surface allocated in this routine */

    if(osurf != SISL_NULL) freeSurf(osurf);

    if (ipar != 3)
      {
	if(par1 != SISL_NULL) freearray(par1);
	if(par2 != SISL_NULL) freearray(par2);
      }
    return;
}
