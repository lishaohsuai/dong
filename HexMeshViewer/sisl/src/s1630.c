/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* (c) Copyright 1989,1990,1991,1992 by                                      */
/*     Senter for Industriforskning, Oslo, Norway                            */
/*     All rights reserved. See the sisl-copyright.h for more details.            */
/*                                                                           */
/*****************************************************************************/

#include "sisl-copyright.h"

#define S1630

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
   s1630(double epoint[],int inbpnt,double astpar,int iopen,int idim,int ik,
	   SISLCurve **rc,int *jstat)
#else
void s1630(epoint,inbpnt,astpar,iopen,idim,ik,rc,jstat)
	   double epoint[];
	   int    inbpnt;
	   double astpar;
	   int    iopen;
	   int    idim;
	   int    ik;
	   SISLCurve  **rc;
	   int    *jstat;
#endif
/*
*********************************************************************
*
* PURPOSE    : To calculate a B-spline curve using the input points as
*              controlling vertices. The distances between the points are
*              used as parametrization.
*
*
* INPUT      : epoint - The array containing the points to be used as
*                       controlling vertices of the B-spline curve.
*              inbpnt - No. of points in epoint.
*              astpar - Parameter value to be used at the start of the curve.
*              iopen  - Open/close condition (Open=1,Closed=0,Periodic=-1)
*              idim   - The dimension of the space
*              ik     - The order of the B-spline curve to be produced.
*
* OUTPUT     : rc     - Pointer to the curve
*              jstat  - status messages
*                                         > 0      : warning
*                                         = 0      : ok
*                                         < 0      : error
*
* METHOD     : First the parametrization of the curve is calculated.
*              If more than ik adjacent vertices are equal then the
*              superfluous vertices are removed. Then the knots are
*              calculated.
*
* EXAMPLE OF USE:
*
* REFERENCES :
*
*-
* CALLS      : s6dist,s1902,s1713,s1750,newCurve,s6err
*
*
* WRITTEN BY : Qyvind Hjelle, SI, Oslo, Norway. 22. Nov 1988
* REVISED BY : Bjoern Olav Hoset, SI, Oslo, Norway, Feb. 1992
*              Calls s1902 instead of fortran functions.
* REWISED BY: Vibeke Skytt, 03.94. This routine corresponds to s1604,
*                                  but differ in the use of the parameter
*                                  iopen.
* Revised by : Paal Fugelli, SINTEF, Oslo, Norway. 24/08-1994. Changed
*              icopy param. (from 0 to 2) to fix memory leak.
*********************************************************************
*/
{
  int kstat;          /* Status variable                                 */
  int kn;             /* The number of B-splines, i.e., the dimension of
			 the spline space associated with the knot
			 vector.                                         */
  int kk;             /* The polynomial order of the curve.              */
  int kpos=0;         /* Position of error                               */
  int ki;             /* Counter for loop control                        */
  int kopen;          /* Local open/closed parameter. Closed,
			 non-periodic is treated as an open curve.       */

  double *spara=SISL_NULL; /* Pointer to parameterization array               */
  double *scoef=SISL_NULL; /* Pointer to vertex array                         */
  double *sknot=SISL_NULL; /* Pointer to knot vector                          */
  double tdist;       /* Distance */
  double tlastpar;    /* Last value in the parameterization array        */
  SISLCurve *qc=SISL_NULL;

  /* Set local open/closed parameter. */

  kopen = (iopen == SISL_CRV_PERIODIC) ? 0 : 1;


  /* Control input     */

  kk = ik;
  if  (inbpnt < kk)
    kk = inbpnt;

  if  (kk < 2) goto err109;

  if  (iopen != SISL_CRV_OPEN && iopen != SISL_CRV_CLOSED
       && iopen != SISL_CRV_PERIODIC) goto err113;

  /* Allocate space for parameterization    */

  spara = newarray(inbpnt+1,DOUBLE);
  if (spara == SISL_NULL) goto err101;

  /* Calculate parameterization  */

  spara[0] = astpar;
  tlastpar = astpar;

  for (ki=1; ki<inbpnt; ki++)
    {
      tdist = s6dist(&epoint[ki*idim-idim],&epoint[ki*idim],idim);
      tlastpar = tlastpar + tdist;
      spara[ki] = tlastpar;
    }


  /* Calculate distance from first to last point and update
     parameterization array. To be useded if closed curve   */

  tdist = s6dist(epoint,&epoint[(inbpnt-1)*idim],idim);
  tlastpar = tlastpar + tdist;
  spara[inbpnt] = tlastpar;


  /* Find the knot vector     */

  s1902(spara,inbpnt+(iopen==SISL_CRV_CLOSED),kk,kopen,&sknot,&kstat);
  if (kstat < 0 || sknot == SISL_NULL) goto error;

  /* Allocate space for verice array   */

  scoef = newarray((inbpnt+kk-1)*idim,DOUBLE);
  if (scoef == SISL_NULL) goto err101;

  /* Copy vertices */

  memcopy (scoef,epoint,inbpnt*idim,DOUBLE);
  kn = inbpnt;

  /* In case of closed curve, add the (kk-1) first points to the vertice
     array.    */

  if (iopen == SISL_CRV_PERIODIC)
    {
      memcopy(&scoef[inbpnt*idim],epoint,(kk-1)*idim,DOUBLE);
      kn = kn + kk - 1;
    }
  else if (iopen == SISL_CRV_CLOSED)
  {
     memcopy(&scoef[inbpnt*idim],epoint,idim,DOUBLE);
     kn += 1;
  }

  /* Make curve */
  /* VSK, MESZ. Do not copy arrays.
  qc = newCurve(kn,kk,sknot,scoef,1,idim,1);  */
  qc = newCurve(kn,kk,sknot,scoef,1,idim,2); /* icopy=2, PFU 24/08-94 */
  if (!qc) goto err101;

  qc->cuopen = iopen;

  /* Increase the order if the order was lowered when controlling input */

  if (kk < ik)
    {
				/* -> guen: inserted acc. to SCCS */
      s1750(qc,ik,rc,&kstat);
				/* <- guen: inserted acc. to SCCS */

				/* -> guen: removed acc. to SCCS  */
				/*      s1750(qc,ik,&qc,&kstat);  */
				/* <- guen: removed acc. to SCCS  */
      if (kstat< 0) goto error;
    }
				/* -> guen: inserted acc. to SCCS */
  else
    {
      *rc = qc;
      qc = SISL_NULL;
    }
				/* <- guen: inserted acc. to SCCS */
				/* -> guen: removed acc. to SCCS  */
				/*  if (qc) *rc = qc;             */
				/* -> guen: inserted acc. to SCCS */

  *jstat = 0;
  goto out;

  /* Error in memory allocation */

 err101:
  *jstat = -101;
  s6err("s1630",*jstat,kpos);
  goto out;

  /* Error in input, order less than 2 */

 err109:
  *jstat = -109;
  s6err("s1630",*jstat,kpos);
  goto out;

  /* Error in input, unknown kind of curve */

 err113:
  *jstat = -113;
  s6err("s1630",*jstat,kpos);
  goto out;


  /* Error in lower level function */

 error:
  *jstat = kstat;
  s6err("s1630",*jstat,kpos);
  goto out;

 out:
  if (spara  != SISL_NULL) freearray(spara);
  /* if (scoef  != SISL_NULL) freearray(scoef); (Freed by freeCurve(qc). */
  if (qc != SISL_NULL) freeCurve(qc);
  return;
}
