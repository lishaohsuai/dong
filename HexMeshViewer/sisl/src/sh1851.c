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
 * $Id: sh1851.c,v 1.2 2001-03-19 15:59:06 afr Exp $
 *
 */


#define SH1851

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void sh1851(SISLSurf * ps1, double epoint[], double enorm[], int idim, 
	    double aepsco, double aepsge,int trackflag, int *jtrack, 
	    SISLTrack *** wtrack,int *jpt, double **gpar, int **pretop, 
	    int *jcrv, SISLIntcurve *** wcurve, int *jsurf, 
	    SISLIntsurf ***wsurf, int *jstat)
#else
void sh1851(ps1, epoint, enorm, idim, aepsco, aepsge,trackflag, jtrack, 
	    wtrack, jpt, gpar, pretop, jcrv, wcurve,jsurf,wsurf, jstat)
     SISLSurf *ps1;
     double epoint[];
     double enorm[];
     int idim;
     double aepsco;
     double aepsge;
     int trackflag;
     int *jtrack;
     SISLTrack ***wtrack;
     int *jpt;
     double **gpar;
     int **pretop;
     int *jcrv;
     SISLIntcurve ***wcurve;
     int      *jsurf;
     SISLIntsurf ***wsurf;
     int *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    : Find all intersections between a tensor-product surface
*              and a plane.
*
*
*
* INPUT      : ps1    - Pointer to surface.
*              epoint - SISLPoint in the plane.
*              enorm  - Normal to the plane.
*              idim   - Dimension of the space in which the plane lies.
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
*                       plane of the surface. The points lie continuous.
*                       Intersection curves are stored in wcurve.
*              pretop - Topology info. for single intersection points.
*              *jcrv  - Number of intersection curves.
*              wcurve  - Array containing descriptions of the intersection
*                       curves. The curves are only described by points
*                       in the parameter plane. The curve-pointers points
*                       to nothing. (See description of Intcurve
*                       in intcurve.dcl).
*              jstat  - status messages
*                                         > 0      : warning
*                                         = 0      : ok
*                                         < 0      : error
*
*
* METHOD     : The vertices of the surface are put into the equation of the
*              plane achieving a surface in the one-dimentional space.
*              Then the zeroes of this surface is found.
*
*
* REFERENCES :
*
*-
* CALLS      : sh1761       - Perform point object-intersection.
*              s1329       - Equation of surface into equation of plane.
*              hp_s1880       - Put intersections on output format.
*              make_sf_kreg   - Ensure k-regularity of surface.
*              newObject   - Create new object.
*              newPoint    - Create new point.
*              freeObject  - Free space occupied by an object.
*              freeIntdat  - Free space occupied by an intersection data.
*
* WRITTEN BY : Vibeke Skytt, SI, 88-06.
* REWRITTEN BY : Bjoern Olav Hoset, SI, 89-06.
*                UJK, SI, 91-07
*********************************************************************
*/
{
  int kstat = 0;		/* Local status varible.                      */
  int kpos = 0;			/* Position of error.                         */
  int kdim = 1;			/* Dimension of space in which the point in the
			           intersect point and surface problem lies.  */
  double *spar = SISL_NULL;		/* Dummy array containing parameter values of
			           second object of single intersection points.*/
  double spoint[1];		/* SISLPoint to intersect with object.            */
  SISLSurf *qs = SISL_NULL;		/* Pointer to surface in
			       surface/point intersection.*/
  SISLPoint *qp = SISL_NULL;		/* Pointer to point in
			       surface/point intersection.  */
  SISLObject *qo1 = SISL_NULL;	/* Pointer to surface in
			       object/point intersection. */
  SISLObject *qo2 = SISL_NULL;	/* Pointer to point in
			       object/point intersection    */
  SISLIntdat *qintdat = SISL_NULL;	/* Intersection result */
  int kdeg = 1;			/* Implisit descr. to track  */
  double simpli[16];
  double snorm[3];
  SISLObject *track_obj=SISL_NULL;
  SISLSurf *qkreg=SISL_NULL; /* Input surface ensured k-regularity. */

  /* -------------------------------------------------------- */  

  if (ps1->cuopen_1 == SISL_SURF_PERIODIC ||
      ps1->cuopen_2 == SISL_SURF_PERIODIC)
  {
     /* Cyclic surface. */

     make_sf_kreg(ps1,&qkreg,&kstat);
     if (kstat < 0) goto error;
   }
  else
    qkreg = ps1;
  
  /*
  * Create new object and connect surface to object.
  * ------------------------------------------------
  */
  
  if (!(track_obj = newObject (SISLSURFACE)))
    goto err101;
  track_obj->s1 = ps1;
  
  /*
  * Check dimension.
  * ----------------
  */
  
  *jpt = 0;
  *jcrv = 0;
  *jtrack = 0;

  if (idim != qkreg->idim)
    goto err106;

  /*
   * Put surface into plane-equation.
   * --------------------------------
   */

  s1329 (qkreg, epoint, enorm, idim, &qs, &kstat);
  if (kstat < 0)
    goto error;

  /*
   * Create new object and connect surface to object.
   * ------------------------------------------------
   */

  if (!(qo1 = newObject (SISLSURFACE)))
    goto err101;
  qo1->s1 = qs;
  qo1->o1 = qo1;

  /*
   * Create new object and connect point to object.
   * ----------------------------------------------
   */

  if (!(qo2 = newObject (SISLPOINT)))
    goto err101;
  spoint[0] = DZERO;
  if (!(qp = newPoint (spoint, kdim, 1)))
    goto err101;
  qo2->p1 = qp;

  /*
   * Find intersections.
   * -------------------
   */

  sh1761 (qo1, qo2, aepsge, &qintdat, &kstat);
  if (kstat < 0)
    goto error;

  /* Represent degenerated intersection curves as one point.  */

  sh6degen(track_obj,track_obj,&qintdat,aepsge,&kstat);
  if (kstat < 0) goto error;

  /* BUG BEOrd13026, simpli is now always in use, UJK */
  /* Normalize plane normal */
  
  (void) s6norm (enorm, 3, snorm, &kstat);
  
  simpli[0] = snorm[0];
  simpli[1] = snorm[1];
  simpli[2] = snorm[2];
  simpli[3] = -s6scpr (epoint, snorm, 3);
       
  
  /* Create tracks */
  if (trackflag && qintdat)
    {
       refine_all (&qintdat, track_obj, track_obj, simpli, kdeg, aepsge, &kstat);
       if (kstat < 0)
	 goto error;
    }
  
  /* Join periodic curves */
  int_join_per( &qintdat,track_obj, track_obj, simpli, kdeg,aepsge,&kstat);
  if (kstat < 0)
    goto error;
  
  if (trackflag && qintdat)
    {
       make_tracks (track_obj, track_obj, kdeg, simpli,
		    qintdat->ilist, qintdat->vlist, 
		    jtrack, wtrack, aepsge, &kstat);
       if (kstat < 0)
	 goto error;
    }

  /*
   * Express intersections on output format.
   * ---------------------------------------
   */

  if (qintdat)			/* Only if there were intersections found */
    {
      hp_s1880 (track_obj, track_obj, kdeg,
		2, 0, qintdat, jpt, gpar, &spar, pretop, jcrv, wcurve, jsurf,wsurf,&kstat);
      if (kstat < 0)
	goto error;
    }

  /*
   * Intersections found.
   * --------------------
   */

  *jstat = 0;
  goto out;

  /* Error in space allocation.  */

err101:*jstat = -101;
  s6err ("sh1851", *jstat, kpos);
  goto out;

  /* Dimensions conflicting.  */

err106:*jstat = -106;
  s6err ("sh1851", *jstat, kpos);
  goto out;

  /* Error in lower level routine.  */

error:*jstat = kstat;
  s6err ("sh1851", *jstat, kpos);
  goto out;

out:

  /* Free allocated space.  */

  if (spar)
    freearray (spar);
  if (qo1)
    freeObject (qo1);
  if (qo2)
    freeObject (qo2);
  if (qintdat)
    freeIntdat (qintdat);
  if (track_obj)
    {
       track_obj->s1 = SISL_NULL;
       freeObject(track_obj);
    }

  /* Free local surface.  */
    if (qkreg != SISL_NULL && qkreg != ps1) freeSurf(qkreg);
  
  return;
}


