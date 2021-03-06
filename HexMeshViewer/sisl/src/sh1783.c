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
 * $Id: sh1783.c,v 1.2 2001-03-19 15:59:05 afr Exp $
 *
 */


#define SH1783

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
typedef void (*fevalProc)(SISLCurve *,int,double,int *,double[],int *);
#else
typedef void (*fevalProc)();
#endif

#if defined(SISLNEEDPROTOTYPES)
static void sh1783_s9relax (fevalProc fevalc1,fevalProc fevalc2,
			    SISLCurve *,SISLCurve *,int,double,double,int *,
			    double [],double,double *,int *,double [],int *);
#else
static void sh1783_s9relax ();
#endif

#if defined(SISLNEEDPROTOTYPES)
void
sh1783 (SISLCurve * pc1, SISLCurve * pc2, double aepsge, double epar[],
	int idir1, int idir2, double elast[], double enext[], int *jstat)
#else
void 
sh1783 (pc1, pc2, aepsge, epar, idir1, idir2, elast, enext, jstat)
     SISLCurve *pc1;
     SISLCurve *pc2;
     double aepsge;
     double epar[];
     int idir1;
     int idir2;
     double elast[];
     double enext[];
     int *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    : March along two curves as long as the coincide. Then return
*              the last points found that are closer to each other than
*              the tolerance, and the first points that are more distant.
*              Start the marching in a given intersection point.
*
*
* INPUT      : pc1      - Pointer to the first curve.
*              pc2      - Pointer to the second curve..
*              aepsge   - Geometry resolution.
*              epar[2]  - Parameter values for the intersection point.
*              idir1    - Direction to march first curve.
*                         = 1  : March in the direction of the parametrization.
*                         = -1 : March in the opposite direction.
*              idir2    - Direction to march second curve.
*
*
* OUTPUT     : elast[2] - Parameter values of the last point within the
*                         interval of coincidence.
*              enext[2] - Parameter values of the first point found outside
*                         this interval.
*              jstat    - status messages
*                             = 3   : Coincidence along both curves.
*                             = 2   : Coincidence along the entire second curve.
*                             = 1   : Coincidence along the entire first curve.
*                             = 0   : Ok. End of interval found.
*                             < 0   : Error.
*
*
* METHOD     : March along one curve, and for each step iterate to
*              the closest point of the other curve at the midpoint and
*              the endpoint of the step. The geometry and knot vectors of
*              both curves are considered when making the step, and we
*              march along the curve that has set the step.
*
* CALLS      : s1221  - Evaluate B-spline curve.
*              s1227  - Evaluate B-spline curve.
*              s1307  - Compute unit tangent and radius of curvature.
*              s1311  - Find current step length.
*              sh1992  - Find box-size of object.
*              s6lenght - Length of vector.
*
*
* REFERENCES :
*
* WRITTEN BY : Vibeke Skytt, SI, Oslo, Norway. 04.91.
*
*********************************************************************
*/
{
  int kstat;			/* Status variable                                 */
  int ki;			/* Counter.                                        */
  int kleftc1 = 0;		/* Left indicator for point calculation of curve 1.*/
  int kleftc2 = 0;		/* Left indicator for point calculation of curve 2.*/
  int kk1, kk2, kn1, kn2;	/* Orders and number of vertices of curves         */
  int kdim;			/* The dimension of the space in which the curves lie. */
  int kpos = 0;			/* Position of error                               */
  int kderc = 2;		/* Number of derivatives to be claculated on the curves */
  int kdum;			/* Temporary variable                              */
  int kchange;			/* Indicates which curve that is marched along.
				   = 0 : First curve.
				   = 1 : Second curve.                             */
  double s3dinf1[20];		/* Pointer to storage for point info of curve 1
				    (10 dobules pr point when idim=3, 7 when idim=3) */
  double s3dinf2[20];		/* Pointer to storage for point info of curve 2
				    (10 dobules pr point when idim=3, 7 when idim=3) */
  double *st1;			/* Knot vector of first curve                      */
  double *st2;			/* Knot vector of second curve                     */
  double tfirst1, tfirst2;	/* First parameter value on curves              */
  double tend1, tend2;		/* Last parameter on curves                        */
  double sderc1[20];		/* Position, first and second derivatives on curve 1 */
  double sderc2[20];		/* Position, first and second derivatives on curve 2 */
  double tx, tx1, tx2;		/* Parameter values of first curve.  */
  double ty, ty1, ty2;		/* Parameter value of second curve.  */
  double tstep;			/* Final step length     */
  double txstep, tystep;	/* Step length     */
  double txmaxinc, tymaxinc;	/* Maximal increment in parameter value along curve*/
  double txlengthend, tylengthend;	/* Length of 1st derivative at start of segment */
  double txincre, tyincre;	/* Parameter value increment */
  double txmax, tymax;		/* Local maximal step length                       */
  double tdist = DZERO;		/* Distance */
  double tpos;			/* New iteration  point on curve pc2     */

  /* Pointer to curve evaluator routines */

  fevalProc fevalc1;
  fevalProc fevalc2;

  /* Make maximal step length based on box-size of curve 1 */

  sh1992cu (pc1, 0, aepsge, &kstat);
  if (kstat < 0)
    goto error;

  txmax = MAX (pc1->pbox->e2max[0][0] - pc1->pbox->e2min[0][0],
	       pc1->pbox->e2max[0][1] - pc1->pbox->e2min[0][1]);
  txmax = MAX (txmax, pc1->pbox->e2max[0][2] - pc1->pbox->e2min[0][2]);

  /* Make maximal step length based on box-size of curve 2 */

  sh1992cu (pc2, 0, aepsge, &kstat);
  if (kstat < 0)
    goto error;

  tymax = MAX (pc2->pbox->e2max[0][0] - pc2->pbox->e2min[0][0],
	       pc2->pbox->e2max[0][1] - pc2->pbox->e2min[0][1]);
  tymax = MAX (tymax, pc2->pbox->e2max[0][2] - pc2->pbox->e2min[0][2]);

  /* Copy curve pc1 attributes to local parameters.  */

  kdim = pc1->idim;
  kk1 = pc1->ik;
  kn1 = pc1->in;
  st1 = pc1->et;

  /* Copy curve pc2 attributes to local parameters.  */

  kk2 = pc2->ik;
  kn2 = pc2->in;
  st2 = pc2->et;

  /* Check that dimensions are equal */

  if (kdim != pc2->idim || kdim > 3)
    goto err105;

  /* Copy interval description into local variables */

  tfirst1 = epar[0];
  tfirst2 = epar[1];
  tend1 = (idir1 == 1) ? st1[kn1] : st1[kk1 - 1];
  tend2 = (idir2 == 1) ? st2[kn2] : st2[kk2 - 1];

  /* To make sure we do not start outside or end outside the curve we
     truncate tfirst1 to the knot interval of the curve */

  tfirst1 = (idir1 == 1) ? MAX (tfirst1, st1[kk1 - 1]) : MIN (tfirst1, st1[kn1]);

  /* To make sure we do not start outside or end outside the curve we
     truncate tstart2 and tend2 to the knot interval of the curve */

  tfirst2 = (idir2 == 1) ? MAX (tfirst2, st2[kk2 - 1]) : MIN (tfirst2, st2[kn2]);

  /* Set curve evaluator of 1. curve.  */

  fevalc1 = (idir1 == 1) ? s1221 : s1227;

  /* Set curve evaluator of 2. curve.  */

  fevalc2 = (idir2 == 1) ? s1221 : s1227;

  /* Store knot values at start of curve */

  tx1 = tfirst1;
  kdum = MAX (kk1, kk2);
  txmaxinc = fabs (tend1 - tfirst1) / (kdum * kdum);

  /* Make start point and intital step length based on first curve  */

  fevalc1 (pc1, kderc, tx1, &kleftc1, sderc1, &kstat);
  if (kstat < 0) goto error;

  ty1 = tfirst2;
  tymaxinc = fabs (tend2 - tfirst2) / (kdum * kdum);

  /* Make start point and intital step length based on second curve  */

  fevalc2 (pc2, kderc, ty1, &kleftc2, sderc2, &kstat);
  if (kstat < 0) goto error;

  /* While end not reached */

  while (idir1 * tx1 < idir1 * tend1 && idir2 * ty1 < idir2 * tend2)
    {

      /* Calculate unit tangent and radius of curvature of first curve. */

      s1307 (sderc1, kdim, s3dinf1, &kstat);
      if (kstat < 0)
	goto error;

      /* Calculate step length based on curvature of first curve. */

      txstep = s1311 (s3dinf1[3 * kdim], aepsge, tymax, &kstat);
      if (kstat < 0)
	goto error;

      /* Remember length of start tangent, end of zero segment */

      txlengthend = s6length (sderc1 + kdim, kdim, &kstat);
      if (kstat < 0)
	goto error;

      /* Calculate unit tangent and radius of curvature of second curve. */

      s1307 (sderc2, kdim, s3dinf2, &kstat);
      if (kstat < 0)
	goto error;

      /* Calculate step length based on curvature */

      tystep = s1311 (s3dinf2[3 * kdim], aepsge, txmax, &kstat);
      if (kstat < 0)
	goto error;

      /* Remember length of start tangent, end of zero segment */

      tylengthend = s6length (sderc2 + kdim, kdim, &kstat);
      if (kstat < 0)
	goto error;

      /*  Find minimum step length.  */

      tstep = MIN (txstep, tystep);
      kchange = (txstep <= tystep) ? 0 : 1;

      /*  Find candidate end point, make sure that no breaks in tangent or
	  curvature exists between start and endpoints of the segment      */
      /* Compute increment in the parameter values.  Use REL_PAR_RES if the
         tangent has zero length.  */

      if (DEQUAL (txlengthend, DZERO))
	txincre = REL_PAR_RES;
      else
	txincre = MIN (tstep / txlengthend, txmaxinc);

      if (DEQUAL (tylengthend, DZERO))
	tyincre = REL_PAR_RES;
      else
	tyincre = MIN (tstep / tylengthend, tymaxinc);

      /*  Make sure that we don't pass any knots of curve 1. */

      /* VSK. 01-93. Is it possible that several knots might be passed. */
      
      if (idir1 > 0 && (tx1 + txincre) > (st1[kleftc1 + 1] + REL_PAR_RES) &&
	  !(tx1 > (st1[kleftc1 + 1] - REL_PAR_RES)))
	{
	  txincre = st1[kleftc1 + 1] - tx1;
	  tstep = txincre * txlengthend;
	  tyincre = (tylengthend > DZERO) ? tstep / tylengthend : REL_PAR_RES;
	  kchange = 0;
	}

/*
  guen      if (idir1 < 0 && (tx1 - txincre < st1[kleftc1] - REL_PAR_RES))
  guen fixed to:
*/
      /* VSK. 01-93. Is it possible that several knots might be passed. */
      
      if (idir1 < 0 && (tx1 - txincre) < (st1[kleftc1] - REL_PAR_RES) &&
	  !(tx1 < (st1[kleftc1] + REL_PAR_RES)))
	{
	  txincre = idir1 * (st1[kleftc1] - tx1);
	  tstep = txincre * txlengthend;
	  tyincre = (tylengthend > DZERO) ? tstep / tylengthend : REL_PAR_RES;
	  kchange = 0;
	}

      /* Avoid passing next knot of curve 2. */
      
      /* VSK. 01-93. Is it possible that several knots might be passed. */

      if (idir2 > 0 && (ty1 + tyincre) > (st2[kleftc2 + 1] + REL_PAR_RES) &&
	  !(ty1 > (st2[kleftc2 + 1] - REL_PAR_RES)))
	{
	  tyincre = st2[kleftc2 + 1] - ty1;
	  tstep = tyincre * tylengthend;
	  txincre = (txlengthend > DZERO) ? tstep / txlengthend : REL_PAR_RES;
	  kchange = 1;
	}

      /* Avoid passing previous knot of curve 2. */

/*
  guen      if (idir2 < 0 && (ty1 - tyincre < st2[kleftc2] - REL_PAR_RES))
  guen fixed to:
*/
      /* VSK. 01-93. Is it possible that several knots might be passed. */

      if (idir2 < 0 && (ty1 - tyincre) < (st2[kleftc2] - REL_PAR_RES) &&
	  !(ty1 > (st2[kleftc2] + REL_PAR_RES)))
	{
	  tyincre = idir2 * (st2[kleftc2] - ty1);
	  tstep = tyincre * tylengthend;
	  txincre = (txlengthend > DZERO) ? tstep / txlengthend : REL_PAR_RES;
	  kchange = 1;
	}


      /* Set endpoints of step.  */

      tx2 = tx1 + idir1 * txincre;
      ty2 = ty1 + idir2 * tyincre;

      for (tx = (tx1 + tx2) / (double) 2.0, ty = (ty1 + ty2) / (double) 2.0, ki = 0;
	   ki < 2; ki++, tx = tx2, ty = ty2)
	{
	  if (kchange == 0)
	    {
	      if (idir1 * tx >= idir1 * tend1)
		break;

	      /* March along first curve. Iterate down to the second.  */

	      sh1783_s9relax (fevalc1, fevalc2, pc1, pc2, kderc, aepsge, tx, &kleftc1, sderc1, ty,
			      &tpos, &kleftc2, sderc2, jstat);
	      if (kstat < 0)
		goto error;
	    }
	  else
	    {
	      if (idir2 * ty >= idir2 * tend2)
		break;

	      /* March along second curve. Iterate down to the first.  */

	      sh1783_s9relax (fevalc2, fevalc1, pc2, pc1, kderc, aepsge, ty, &kleftc2, sderc2, tx,
			      &tpos, &kleftc1, sderc1, jstat);
	      if (kstat < 0)
		goto error;
	    }

	  /*  Check if point on curve and surface are within positional and
	      angular tolerances */

	  tdist = s6dist (sderc1, sderc2, kdim);

	  if (tdist > aepsge)
	    break;		/*   Points not within tolerances */
	}

      if (tdist > aepsge)
	break;			/*   Points not within tolerances */

      /*   Update start parameter value of segment.  */

      if (kchange == 0)
	{
	  tx1 = tx2;
	  ty1 = (idir2 > 0) ? MAX(ty1,tpos) : MIN(ty1,tpos);
	}
      else
	{
	  tx1 = (idir1 > 0) ? MAX(tx1,tpos) : MIN(tx1,tpos);
	  ty1 = ty2;
	}
    }

  elast[0] = tx1;
  elast[1] = ty1;
  if (tdist > aepsge)
    {
      enext[0] = (kchange == 0) ? tx : tpos;
      enext[1] = (kchange == 1) ? ty : tpos;
      *jstat = 0;
    }
  else if (idir1 * tx1 >= idir1 * tend1 && idir2 * ty1 >= idir2 * tend2)
    *jstat = 3;
  else if (idir2 * ty1 >= idir2 * tend2)
    *jstat = 2;
  else
    *jstat = 1;

  goto out;

/* Error in input, dimension not equal to 2 or 3 */

err105:*jstat = -105;
  s6err ("sh1783", *jstat, kpos);
  goto out;

/* Error in lower level function */

error:*jstat = kstat;
  s6err ("sh1783", *jstat, kpos);
  goto out;

out:
  return;
}


#if defined(SISLNEEDPROTOTYPES)
static void
  sh1783_s9relax(fevalProc fevalc1,fevalProc fevalc2,
		 SISLCurve * pc1, SISLCurve * pc2,int ider, double aepsge, 
		 double ax1, int *jleft1, double eder1[],double anext,
		 double *cx2, int *jleft2, double eder2[], int *jstat)
#else
static void 
sh1783_s9relax (fevalc1, fevalc2, pc1, pc2, ider, aepsge, ax1, jleft1, eder1, 
		anext,cx2, jleft2, eder2, jstat)
     fevalProc  fevalc1;
     fevalProc  fevalc2;
     SISLCurve  *pc1;
     SISLCurve  *pc2;
     int        ider;
     double     aepsge;
     double     ax1;
     int        *jleft1;
     double     eder1[];
     double     anext;
     double     *cx2;
     int        *jleft2;
     double     eder2[];
     int        *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    : Evaluate the first curve in a given parameter value and
*              iterate down to the closest point on the second curve
*              to the position on the first curve.
*
*
* INPUT      : fevalc1 - Curve evaluator corresponding to first curve.
*              fevalc1 - Curve evaluator corresponding to second curve.
*              pc1     - Pointer to the first curve.
*              pc2     - Pointer to the second curve.
*              ider    - Number of derivatives to compute. 0 <= ider <= 2.
*              aepsge  - Geometry resolution.
*              ax1     - Parameter value at which to evaluate curve 1.
*              anext   - Start parameter to the iteration on curve 2.
*
*
* INPUT/OUTPUT : jleft1  - Parameter used to set knot interval of curve1.
*                          Used in s1221.
*                jleft2  - Parameter used to set knot interval of curve2.
*
*
* OUTPUT     : eder1   - 0-ider'th derivative of curve 1 evaluated in ax1.
*                        Dimension is (ider+1)*pc1->idim.
*              cx2     - Parameter value of the point on curve 2 closest
*                        to the point given by eder1.
*              eder2   - 0-ider'th derivative of curve 2 evaluated in *cx2.
*                        Dimension is (ider+1)*pc2->idim.
*              jstat   - status messages
*                                > 0   : Warning.
*                                = 0   : Ok.
*                                < 0   : Error.
*
*
* METHOD     :
*
*
* CALLS      :  s1221     - Evaluate curve.
*               s1771     - Closest point between a curve and a point.
*               newPoint  - Create new point object.
*               freePoint - Free point object.
*
*
* REFERENCES :
*
* WRITTEN BY : Vibeke Skytt, SI, Oslo, Norway. Oct. 1990
*
*********************************************************************
*/
{
  int kstat = 0;		/* Status variable.  */
  double tstart;		/* Start parameter value of curve 2.  */
  double tend;			/* End parameter value of curve 2.    */
  SISLPoint *qpoint = SISL_NULL;	/* SISLPoint instance used to represent point on curve 1. */

  /* Find endpoints of the parameter interval of curve 2.  */

  tstart = *(pc2->et + pc2->ik - 1);
  tend = *(pc2->et + pc2->in);

  /*  Make point sderc at curve at ax1 */

  fevalc1 (pc1, ider, ax1, jleft1, eder1, &kstat);
  if (kstat < 0) goto error;

  /* Find closest point on curve 2 to eder1 */

  qpoint = newPoint (eder1, pc1->idim, 0);
  if (qpoint == SISL_NULL) goto err101;

  s1771 (qpoint, pc2, aepsge, tstart, tend, anext, cx2, &kstat);
  if (kstat < 0)
    goto error;

  /* Calculate point and derivatives in second curve */

  fevalc2 (pc2, ider, *cx2, jleft2, eder2, &kstat);
  if (kstat < 0) goto error;

  *jstat = 0;
  goto out;

  /* Error in space allocation.  */

err101:
  *jstat = -101;
  goto out;

  /* Error in lower level routine.  */

error:
  *jstat = kstat;
  goto out;

out:
  if (qpoint != SISL_NULL)
    freePoint (qpoint);

  return;
}

