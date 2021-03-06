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
 * $Id: sh1369.c,v 1.4 2001-03-19 15:59:03 afr Exp $
 *
 */


#define SH1369

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
     sh1369(SISLSurf *ps,double ecentr[],double enorm[],double abigr,
	   double asmalr,int idim,double aepsco,double aepsge,
	int trackflag,int *jtrack,SISLTrack ***wtrack,
	int *jpt,double **gpar,int **pretop,int *jcrv,SISLIntcurve ***wcurve,int *jsurf,SISLIntsurf ***wsurf,int *jstat)
#else
void sh1369(ps,ecentr,enorm,abigr,asmalr,idim,aepsco,aepsge,
	trackflag,jtrack,wtrack,jpt,gpar,pretop,jcrv,wcurve,jsurf,wsurf,jstat)
     SISLSurf     *ps;
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
     int      *jsurf;
     SISLIntsurf ***wsurf;
     int      *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    : Find all intersections between a surface and a torus.
*
* INPUT      : ps     - Pointer to the surface.
*              ecentr - The center of the torus (lying in the symmetry plane)
*              enorm  - Normal of symmetry plane
*              abigr  - Distance from ecentr to center circle of torus
*              asmalr - The radius of the torus surface
*              idim   - Dimension of the space in which the torus
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
*              pretop - Topology info. for single intersection points.
*                       Intersection curves are stored in wcurve.
*              jcrv   - Number of intersection curves.
*              wcurve - Array containing descriptions of the intersection
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
* METHOD     : The vertices of the surface is put into the equation of the
*              torus achieving a surface in the one-dimensional space.
*              The zeroes of this surface are found.
*
*
* REFERENCES :  "How tolerances change in the topology part
*                of SISL intersections", MF 17/10/91.
*
*-
* CALLS      : sh1761       - Find point/object intersections.
*              hp_s1880       - Put intersections into output format.
*              s1378       - Put surface into torus equation
*              s6norm      - Normalize vector.
*              make_sf_kreg   - Ensure k-regularity of surface.
*              newObject   - Create new object.
*              newPoint    - Create new point.
*              freeObject  - Free the space occupied by a given object.
*              freeIntdat  - Free space occupied by an intersection data.
*
* WRITTEN BY : Vibeke Skytt, SI, 88-11.
* REWRITTEN BY : Bjoern Olav Hoset, SI, 89-06.
* REVISED BY:  Michael Floater, 93-02. Corrected 1D tolerance.
* Changed by: Per OEyvind Hvidsten, SINTEF, 94-11.
*             Added code to handle output parameters jcrv, jsurf
*             when no intersection was found.
* Chanbed by : Vibeke Skytt, SINTEF Oslo, 941202. Change in 1D tolerance
*                                                 after scaling.
*
*********************************************************************
*/
{
  int kstat = 0;              /* Local status variable.                      */
  int kpos = 0;               /* Position of error.                          */
  int kdim=1;                 /* Dimension of surface in surface/point
				 intersection.                               */
  int kdeg = 1001;            /* Code that the implice surface is a torus    */
  double simpli[16];           /* Array for representation of torus-surface   */
  double snorm[3];            /* Normalized normal vector                    */
  double spoint[1];           /* SISLPoint in surface/point intersection.        */
  double *spar = SISL_NULL;        /* Values of intersections in the parameter
				 area of the second object.
				 Empty in this case.   */
  SISLSurf *qs = SISL_NULL;        /* Pointer to surface in
			     surface/point intersection.*/
  SISLPoint *qp = SISL_NULL;       /* Pointer to point in
			     surface/point intersection.  */
  SISLObject *qo1 = SISL_NULL;     /* Pointer to surface in
			     object/point intersection. */
  SISLObject *qo2 = SISL_NULL;     /* Pointer to point in
			     object/point intersection    */
  SISLIntdat *qintdat = SISL_NULL; /* Intersection result */
  SISLObject *track_obj=SISL_NULL;
  SISLSurf *qkreg=SISL_NULL; /* Input surface ensured k-regularity. */
  double aepsge2;             /* 1 dimensional tolerance. */

  int ki;
  double nmax=(double)1.0;
  /* -------------------------------------------------------- */

  if (ps->cuopen_1 == SISL_SURF_PERIODIC ||
      ps->cuopen_2 == SISL_SURF_PERIODIC)
  {
     /* Cyclic surface. */

     make_sf_kreg(ps,&qkreg,&kstat);
     if (kstat < 0) goto error;
   }
  else
    qkreg = ps;

  /*
  * Create new object and connect surface to object.
  * ------------------------------------------------
  */

  if (!(track_obj = newObject (SISLSURFACE)))
    goto err101;
  track_obj->s1 = ps;

  /*
   * Check dimension.
   * ----------------
   */

  *jpt  = 0;
  *jcrv = 0;
  *jtrack = 0;

  if (idim != 3) goto err104;
  if (qkreg -> idim != idim) goto err106;

  /*
   * Normalize normal vector.
   * ------------------------
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
   * Put surface into torus equation
   * -------------------------------
   */

  s1378(qkreg,simpli,kdeg,idim,&qs,&kstat);
  if (kstat<0) goto error;

  /*
   * Create new object and connect surface to object.
   * ------------------------------------------------
   */

  if(!(qo1 = newObject(SISLSURFACE))) goto err101;
  qo1 -> s1 = qs;
  qo1 -> o1 = qo1;

  /*
   * Create new object and connect point to object.
   * ----------------------------------------------
   */

  if(!(qo2 = newObject(SISLPOINT))) goto err101;
  spoint[0] = DZERO;
  if(!(qp = newPoint(spoint,kdim,1))) goto err101;
  qo2 -> p1 = qp;

  /*
   * Find intersections.
   * -------------------
   */

  /* Apply corrected tolerance.
     Reference: "How tolerances change in the topology part
     of SISL intersections", MF 23/2/93. */

  aepsge2 = (double)8.0 * aepsge * asmalr * abigr * abigr;

  /* UJK,sept 93, Normalize to get angle tolerances correct */
  for(ki=0; ki<qs->in1*qs->in2;ki++)
     nmax = max(fabs(qs->ecoef[ki]),nmax);

  /* VSK, 941202. Justify nmax in order to avoid a too small tolerance. */
  
  nmax = MIN(nmax, aepsge2*1.0e11);
             
  if (nmax >(double)10.0)
  {
     for(ki=0; ki<qs->in1*qs->in2;ki++)
        qs->ecoef[ki] /= nmax;
     aepsge2 /= nmax;
  }

  /* UJK,sept 93, End of change */

  sh1761(qo1,qo2,aepsge2,&qintdat,&kstat);
  if (kstat < 0) goto error;

  /* Represent degenerated intersection curves as one point.  */

  sh6degen(track_obj,track_obj,&qintdat,aepsge,&kstat);
  if (kstat < 0) goto error;


  /* Create tracks */
  if (trackflag && qintdat)
    {

      refine_all (&qintdat, track_obj, track_obj, simpli, kdeg, aepsge, &kstat);
      if (kstat < 0)
	goto error;
    }

  /* Join periodic curves */
  int_join_per( &qintdat,track_obj,track_obj,simpli,kdeg,aepsge,&kstat);
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

  if (qintdat)/* Only if there were intersections found */
    {
      hp_s1880(track_obj, track_obj, kdeg,
	       2,0,qintdat,jpt,gpar,&spar,pretop,jcrv,wcurve,jsurf,wsurf,&kstat);
      if (kstat < 0) goto error;
    }
  else
  {
    *jcrv = 0;
    *jsurf = 0;
  }

  /*
   * Intersections found.
   * --------------------
   */

  *jstat = 0;
  goto out;

  /* Error in space allocation.  */

 err101: *jstat = -101;
         s6err("sh1369",*jstat,kpos);
         goto out;

  /* Dimensions conflicting.  */

 err106: *jstat = -106;
         s6err("sh1369",*jstat,kpos);
         goto out;

  /* Dimension not equal to three.  */

 err104: *jstat = -104;
         s6err("sh1369",*jstat,kpos);
         goto out;

 /* Error in lower level routine.  */

 error : *jstat = kstat;
         s6err("sh1369",*jstat,kpos);
         goto out;

 out:

  /* Free allocated space.  */

  if (spar)    freearray(spar);
  if (qo1)     freeObject(qo1);
  if (qo2)     freeObject(qo2);
  if (qintdat) freeIntdat(qintdat);
  if (track_obj)
    {
       track_obj->s1 = SISL_NULL;
       freeObject(track_obj);
    }

  /* Free local surface.  */
    if (qkreg != SISL_NULL && qkreg != ps) freeSurf(qkreg);

return;
}
