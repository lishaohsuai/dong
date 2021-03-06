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
 * $Id: s1312.c,v 1.2 2001-03-19 15:58:44 afr Exp $
 *
 */


#define S1312

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void 
s1312(double egeo[],int idim,int inbinf,int ipar,double epar[],
	   SISLCurve **rcurve,int *jstat)
#else
void s1312(egeo,idim,inbinf,ipar,epar,rcurve,jstat)
     double egeo[];
     int    idim;
     int    inbinf;
     int    ipar;
     double epar[];
     SISLCurve  **rcurve;
     int    *jstat;
#endif
/*
*********************************************************************
*                                                                   
* PURPOSE    : To represent the curve described in egeo as
*              an Hermit curve on a B-spline format.
*
*
* INPUT      : egeo   - The geometry of the point to be interpolated
*                       The sequence of the information for each point
*                       is: position, unit tangent, curvature vector
*                           and radius of curvature.
*                       When the dimension is 2 this is 7 doubles
*                       When the dimension is 3 this is 10 doubles
*                       Total size of egeo is thus:
*                        idim=2 :  7*inbinf doubles
*                        idim=3 : 10*inbinf doubles
*              idim   - Dimension of the spcae the points lie in
*                       only 2 and 3 is legal
*              inbinf - Number of points
*              ipar   - Array telling if input parametrization (in epar)
*                       is to be used:
*                        ipar = 0 : Don't use input parametrization
*                        ipar = 1 : Use input parametrization
*
* INPUT/OUTPUT:
*              epar   - Parametrization of the points. ipar determines
*                       if this is input or output
*
* OUTPUT     : rcurve - The curve produced
*              jstat  - status messages  
*                                         > 0      : warning
*                                         = 0      : ok
*                                         < 0      : error
*
*
* METHOD     : 
*
* REFERENCES :
*
*-
* CALLS      :
*
* WRITTEN BY : Tor Dokken, SI, OSLO, Norway, 30. June 1988
*
*********************************************************************
*/
{
  int kn;             /* Number of vertices                                 */
  int kk = 4;         /* Order of b-spline basis                            */
  int knt;            /* Number of knots produced so far                    */
  int kvert;          /* Pointer to first free variable in vertex array     */
  int kpos =1;        /* Position of error                                  */
  int kstat;          /* Local status variable                              */
  int ki,kj;          /* Running variables in loop                          */
  int kv1,kv2,kv3;    /* Running variables in loop                          */
  int kincre;         /* Number of doubles for each point in egeo           */
  double *sprevp;     /* Pointer to position at start of current segment    */
  double *sprevt;     /* Pointer to tangent  at start of current segment    */
  double *sprevc;     /* Pointer to curvature at start of current segment   */
  double *sprevr;     /* Pointer to radius of curvature start current segment*/
  double snprevt[3];  /* Nomralized version of sprevc                       */
  double *scurp;      /* Pointer to position at end   of current segment    */
  double *scurt;      /* Pointer to tangent  at end   of current segment    */
  double *scurc;      /* Pointer to curvature at end   of current segment   */
  double *scurr;      /* Pointer to radius of curvature end  current segment*/
  double sncurt[3];   /* Normalized version of scurc                        */
  double tcos;        /* Description of angle                               */
  double tl1,tl2;     /* Tangent lengths                                    */
  double tangle;      /* Arcus cosinus if tcos                              */
  double tdist;       /* Distance between start and end of current segment  */
  double tpar;        /* Parameter value at end of segment                  */
  double *st = SISL_NULL;  /* Pointer to knot vector                             */
  double *scoef=SISL_NULL; /* Pointer to vertices                                */
  
  /* Allocate space for knots and vertices */
  
  if (idim != 2 && idim != 3) goto err105;
  
  if (idim==2)
    kincre = 7;
  else
    kincre = 10;
  
  kn = 3*(inbinf-1) + 1;
  scoef = newarray(idim*kn,DOUBLE);
  if (scoef == SISL_NULL) goto err101;
  
  st = newarray(kk+kn,DOUBLE);
  if (st == SISL_NULL) goto err101;
  
  /* Make four first knots */
  if (ipar==0)
      epar[0] = DZERO;
  
  st[0] = epar[0];
  st[1] = epar[0];
  st[2] = epar[0];
  st[3] = epar[0];
  
  /* Make first vertex */
  memcopy(scoef,egeo,idim,DOUBLE);
  
  
  /* Set pointers to start point, tangent, curvature and radius of curvature */
  
  sprevp = egeo;
  sprevt = sprevp + idim;
  sprevc = sprevt + idim;
  sprevr = sprevc + idim;
  
  /* Normalize curvature vector at start */
  
  (void)s6norm(sprevt,idim,snprevt,&kstat);
  
  for (ki=1,knt=4,kvert=idim;ki<inbinf;ki++)
    {
      
      /* For each pair of adjacent points in egeo make an Hermit segment */
      
      /* Set pointers position, tangent, curvature and radius of end of
	 current segment segment */
      
      scurp = sprevp + kincre;
      scurt = sprevt + kincre;
      scurc = sprevc + kincre;
      scurr = sprevr + kincre;
      
      /*  Normalize curvature vector at end of segment */
      
      (void)s6norm(scurt,idim,sncurt,&kstat);
      
      /* Make cosine of angle between tangent vectors by making the scalar
	 product of the normalized versions of the two vectors */
      
      tcos = s6scpr(snprevt,sncurt,idim);
      
      /* Find the actual angle by making the arcus tangens of this value */
      
      if (tcos >= DZERO)             
	tcos = MIN((double)1.0,tcos);
      else
	tcos = MAX((double)-1.0,tcos);
      
      tangle = fabs(acos(tcos));
      
      if (tangle < ANGULAR_TOLERANCE) tangle = DZERO;
      
      tdist = s6dist(sprevp,scurp,idim);
      
      /*  Make tangent length of start of segment */
      
      if (tangle == DZERO || *sprevr <= DZERO)
        {
	  /* Parallel tangents or infinit radius of curvature use 1/3 of
	     the distance between the points as tangent length          */
	  tl1 = tdist/(double)3.0;
        }
      else
        {
	  /* Base tangent length on radius of curvature and opening angle */
	  tl1 = s1325(*sprevr,tangle);
        }
      
      /*  Make tangent length of end of segment */
      
      if (DEQUAL(tangle,DZERO) || *scurr < DZERO)
        {
	  /* Parallel tangents or infinit radius of curvature use 1/3 of
	     the distance between the points as tangent length          */
	  tl2 = tdist/(double)3.0;
        }
      else
        {
	  /* Base tangent length on radius of curvature and opening angle */
	  tl2 = s1325(*scurr,tangle);
        }
      
      /* Make sure that the tangent does not explode due to numeric errors,
	 and make a controlled tangent when the radius is zero or almost zero*/
      
      if ( tl1 > tdist) tl1 = tdist/(double)3.0;
      if ( tl2 > tdist) tl2 = tdist/(double)3.0;
      
      /* We want to have a parametrization that is as close as possible to an
	 arc length parametrization */                                             
      
      
      if (ipar==0)
        {
	  /* Make parametrization of segment by making an average of arc of a
	     circle with radius sprevr and scurr spanning an angle tangle.
	     If one or both radius infinit use the distance between the 
	     points */
	  
	  if (DNEQUAL(*sprevr,(double)-1.0) && 
	      DNEQUAL(*scurr,(double)-1.0))
            {
	      tpar = (double)0.5*tangle*(*sprevr+*scurr);
            }
	  else if (DNEQUAL(*sprevr,(double)-1.0) && 
		   DEQUAL(*scurr,(double)-1.0))
            {
	      tpar = (double)0.5*(*sprevr*tangle + tdist);
            }
	  else if (DEQUAL(*sprevr,(double)-1.0) && 
		   DNEQUAL(*scurr,(double)-1.0))
            {
	      tpar = (double)0.5*(tdist + tangle*(*scurr));
            }
	  else
            {
	      tpar =  tdist;
            }
	  
	  tpar = MAX(tpar,tdist);
	  
	  if (DEQUAL((epar[ki-1]+tpar),epar[ki-1]))
            {
	      tpar = fabs(epar[ki-1])*(double)0.1;
            }
	  
	  if (DEQUAL(tpar,DZERO))
            {
	      tpar = (double)1.0;
            }
	  
	  epar[ki] = epar[ki-1] + tpar;
	  
        }
      
      /*  Make 3 new knots */
      st[knt]   = epar[ki];
      st[knt+1] = epar[ki];
      st[knt+2] = epar[ki];
      
      /*  Make 3 new vertices of segment */
      
      for (kj=0,kv1=kvert,kv2=kv1+idim,kv3=kv2+idim ; kj<idim ;
	   kj++,kv1++,kv2++,kv3++)
        {
	  scoef[kv1] = sprevp[kj] + tl1*sprevt[kj];
	  scoef[kv2] = scurp[kj]  - tl2*scurt[kj];
	  scoef[kv3] = scurp[kj];
        }
      
      /*  Update pointers */
      sprevp = scurp;
      sprevt = scurt;
      sprevc = scurc;
      sprevr = scurr;
      for (kj=0;kj<idim;kj++) snprevt[kj] = sncurt[kj];
      
      /*  Only update number of vertices if epar[ki-1] != epar[ki] */ 
      
      if (DNEQUAL(epar[ki-1],epar[ki]))
        {
	  kvert+=(3*idim);
	  knt+=3;
        }
    }
  
  /* Insert last knot */
  
  st[kn+kk-1] = st[kn+kk-2];
  
  /* Update number of vertices */
  
  kn = kvert/idim;
  
  
  /* Make the curve */
  
  kpos = 1;
  *rcurve = SISL_NULL;
  *rcurve = newCurve(kn,kk,st,scoef,1,idim,1);
  if (*rcurve == SISL_NULL) goto err101;
  
  *jstat = 0;
  goto out;
  
  /* Error in space allocation.  */
  
 err101: *jstat = -101;
  s6err("s1312",*jstat,kpos);
  goto out;
  
  /* Error in input, negative relative tolerance given */
  
 err105: *jstat = -105;
  s6err("s1312",*jstat,kpos);
  goto out;
  
  /* Free allocated arrays */
 out:
  
  
  if (st != SISL_NULL)    freearray(st);
  if (scoef != SISL_NULL) freearray(scoef);
  
  
  return;
}
