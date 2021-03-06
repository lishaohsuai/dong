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
 * $Id: sh1375.c,v 1.2 2001-03-19 15:59:03 afr Exp $
 *
 */


#define SH1375

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
     sh1375(SISLCurve *pc1,double ecentr[],double enorm[],double abigr,
	   double asmalr,int idim,double aepsco,double aepsge,
	int trackflag,int *jtrack,SISLTrack ***wtrack,
	int *jpt,double **gpar,int **pretop,int *jcrv,SISLIntcurve ***wcurve,int *jstat)
#else
void sh1375(pc1,ecentr,enorm,abigr,asmalr,idim,aepsco,aepsge,
	trackflag,jtrack,wtrack,jpt,gpar,pretop,jcrv,wcurve,jstat)
     SISLCurve    *pc1;
     double   ecentr[];
     double   enorm[];
     double   abigr;
     double   asmalr;
     int      idim;
     double   aepsco;
     double   aepsge;
     int       trackflag;
     int       *jtrack;
     SISLTrack ***wtrack;
     int      *jpt;
     double   **gpar;
     int      **pretop;
     int      *jcrv;
     SISLIntcurve ***wcurve;
     int      *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*                                                                   
* PURPOSE    : Find all intersections between a curve and a torus. 
*
* INPUT      : pc1    - Pointer to the curve.
*              ecentr - The center of the torus (lying in the symmetri plane)
*              enorm  - Normal of symmetri plane
*              abigr  - Distance fro ecentr to center circle of torus
*              asmalr - The radius of the torus surface
*              idim   - Dimension of the space in which the plane/line
*                       lies. idim should be equal to two or three.
*              aepsco - Computational resolution.
*              aepsge - Geometry resolution.
*              trackflag - If true, create tracks.
*
*
*
* OUTPUT     : jtrack - Number of tracks created
*              wtrack - Array of pointers to tracks
*              jpt    - Number of single intersection points.
*              gpar   - Array containing the parameter values of the
*                       single intersection points in the parameter
*                       interval of the curve. The points lie continuous. 
*                       Intersection curves are stored in wcurve.
*              pretop - Topology info. for single intersection points.
*              *jcrv  - Number of intersection curves.
*              wcurve  - Array containing descriptions of the intersection
*                       curves. The curves are only described by points
*                       in the parameter interval. The curve-pointers points
*                       to nothing. (See description of Intcurve
*                       in intcurve.dcl).
*              jstat  - status messages  
*                                         > 0      : warning
*                                         = 0      : ok
*                                         < 0      : error
*
*
* METHOD     : The vertices of the curve is put into the equation of the
*              torus achieving a curve in the one-dimentional space of order
*              4*(ik-1) + 1, when the order of the original curve is ik.
*              The zeroes of this curve are found.
*
*
* REFERENCES :
*
*- 
* CALLS      : sh1761 - Find point/curve intersections.
*              hp_s1880 - Put intersections into output format.
*              s1377 - Put curve into torus equation
*              make_cv_kreg - Ensure k-regularity of input curve.
*              newCurve   - Create new curve.                        
*              newPoint   - Create new point.                        
*              newObject  - Create new object.
*              freeObject - Free the space occupied by a given object.
*              freeIntdat - Free the space occupied by intersection result.
*
* WRITTEN BY : Tor Dokken, SI, 88-05.
* REWRITTEN BY : Bjoern Olav Hoset, SI, 89-06.
*
*********************************************************************
*/                                                               
{                                                                     
  double *nullp = SISL_NULL;
  int kstat = 0;           /* Local status variable.                       */
  int kpos = 0;            /* Position of error.                           */
  int kdim=1;              /* Dimension of curve in curve/point intersection.*/
  int kdeg = 1001;         /* Code that the implict surface is a torus     */
  double simpli[8];        /* Array for represetnation of torus    surface */
  double snorm[3];         /* Normalized normal vector                     */
  double spoint[1];        /* SISLPoint in curve/point intersection.           */
  double *spar = SISL_NULL;     /* Values of intersections in the parameter 
			      area of the second object. 
			      Empty in this case. */
  SISLCurve *qc = SISL_NULL;        /* Pointer to curve in 
			      curve/point intersection.  */
  SISLPoint *qp = SISL_NULL;        /* Pointer to point in 
			      curve/point intersection.  */
  SISLObject *qo1 = SISL_NULL;      /* Pointer to object in 
			      object/point intersection.*/
  SISLObject *qo2 = SISL_NULL;      /* Pointer to object in 
			      object/point intersection.*/
  SISLIntdat *qintdat = SISL_NULL;  /* Pointer to intersection data */
  int      ksurf=0;         /* Dummy number of Intsurfs. */
  SISLIntsurf **wsurf=0;    /* Dummy array of Intsurfs. */
  SISLObject *track_obj=SISL_NULL;
  SISLCurve *qkreg=SISL_NULL; /* Input surface ensured k-regularity. */

  /* -------------------------------------------------------- */  

  if (pc1->cuopen == SISL_CRV_PERIODIC)
  {
     /* Cyclic curve. */

     make_cv_kreg(pc1,&qkreg,&kstat);
     if (kstat < 0) goto error;
   }
  else
    qkreg = pc1;

  /*
  * Create new object and connect curve to object.
  * ------------------------------------------------
  */
  
  if (!(track_obj = newObject (SISLCURVE)))
    goto err101;
  track_obj->c1 = pc1;


  /* 
   * Check dimension.  
   * ----------------
   */

  *jpt  = 0;
  *jcrv = 0;
  *jtrack = 0;

  if (idim != 3) goto err104;
  if (qkreg -> idim != idim) goto err103;

  /* 
   * Normalize normal vector 
   * ----------------------
   */

  (void)s6norm(enorm,idim,snorm,&kstat);
  if (kstat<0) goto error;

  /* 
   * Put the information concerning the torus in the following sequence
   * into simpli: Center, normal, big radius, small radius 
   * ------------------------------------------------------------------
   */

  memcopy(simpli,ecentr,3,DOUBLE);
  memcopy(simpli+3,snorm,3,DOUBLE);
  simpli[6] = abigr;
  simpli[7] = asmalr;

  /* 
   * Put curve into torus equation 
   * -----------------------------
   */ 

  s1377(qkreg,simpli,kdeg,idim,&qc,&kstat);
  if (kstat<0) goto error;

  /* 
   * Create new object and connect curve to object.  
   * ----------------------------------------------
   */

  if (!(qo1 = newObject(SISLCURVE))) goto err101;
  qo1 -> c1 = qc;
  qo1 -> o1 = qo1;

  /*
   * Create new object and connect point to object.
   * ----------------------------------------------
   */

  if (!(qo2 = newObject(SISLPOINT))) goto err101;
  spoint[0] = DZERO;
  if (!(qp = newPoint(spoint,kdim,1))) goto err101;
  qo2 -> p1 = qp;

  /* 
   * Find intersections.  
   * -------------------
   */

  sh1761(qo1,qo2,aepsge,&qintdat,&kstat);
  if (kstat < 0) goto error;

  /* Join periodic curves */
  int_join_per( &qintdat,track_obj,track_obj,nullp,2000,aepsge,&kstat);
  if (kstat < 0)
    goto error;

  /* Create tracks */
  if (trackflag && qintdat)
    {
      make_tracks (qo1, qo2, 0, nullp,
		   qintdat->ilist, qintdat->vlist,
		   jtrack, wtrack, aepsge, &kstat);
      if (kstat < 0)
	goto error;
    }

  /* 
   * Express intersections on output format.  
   * ---------------------------------------
   */

  if (qintdat)/* Only if there were intersections found */
    {
      hp_s1880(track_obj, track_obj, 2000,
	       1,0,qintdat,jpt,gpar,&spar,pretop,jcrv,wcurve,&ksurf,&wsurf,&kstat);
      if (kstat < 0) goto error;
    }
  
  /* 
   * Intersections found.  
   * --------------------
   */

  *jstat = 0;
  goto out;

  /* Error in space allocation.  */

 err101: *jstat = -101;
        s6err("sh1375",*jstat,kpos);
        goto out;

  /* Dimensions conflicting.  */

 err103: *jstat = -103;
        s6err("sh1375",*jstat,kpos);
        goto out;

  /* Dimension not equal to two or three.  */

 err104: *jstat = -104;                          
        s6err("sh1375",*jstat,kpos);
        goto out;

  /* Error in lower level routine.  */

  error : *jstat = kstat;
        s6err("sh1375",*jstat,kpos);
        goto out;

 out:

  /* Free allocated space.  */

  if (spar)    freearray(spar);
  if (qo1)     freeObject(qo1);
  if (qo2)     freeObject(qo2);
  if (qintdat) freeIntdat(qintdat);
  if (track_obj)
    {
       track_obj->c1 = SISL_NULL;
       freeObject(track_obj);
    }

  /* Free local curve.  */

  if (qkreg != SISL_NULL && qkreg != pc1) freeCurve(qkreg);
return;
}                                               
                                           
                       

