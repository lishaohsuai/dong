/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* (c) Copyright 2009 by                                                     */
/*     SINTEF, Oslo, Norway                                                  */
/*     All rights reserved. See the sisl-copyright.h for more details.            */
/*                                                                           */
/*****************************************************************************/


//===========================================================================
//
// File : s1631.c
//
// Created: Wed Jan 28 15:08:17 2009
//
// Author: Kjell Fredrik Pettersen
//
// Revision: $Id: s1631.c,v 1.1 2009-01-28 14:10:13 kfp Exp $
//
// Description:
//
//===========================================================================


#include "sisl-copyright.h"


#define S1631

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void 
s1631(SISLCurve *pc,double epoint[],double enorm[],double projdir[],int idim,SISLCurve **rc,int *jstat)
#else
void s1631(pc,epoint,enorm,projdir,idim,rc,jstat)
     SISLCurve  *pc;
     double epoint[];
     double enorm[];
     double projdir[];
     int    idim;
     SISLCurve  **rc;
     int    *jstat;
#endif
/*
*********************************************************************
*                                                                   
* PURPOSE    : To project a B-spline curve to a plane along a given direction
*             
*
* INPUT      : pc      - The input B-spline curve.   
*              epoint  - a point in the plane
*              enorm   - normal vector to the plane  
*              projdir - projection direction vector
*              idim    - The dimension of the space
*
* OUTPUT     : rc     - Pointer to the projected curve
*              jstat  - status messages
  
*                                         > 0      : warning
*                                         = 0      : ok
*                                         < 0      : error
*
* METHOD     : The vertices are projected. All other curve data are 
*              copied into the new curve. 
*
* EXAMPLE OF USE:
*
* REFERENCES :
*
*-                                                 
* CALLS      : s6diff,s6scpr,newCurve,s6err
*
*
* WRITTEN BY : Kjell Fredrik Pettersen, SINTEF IKT, Oslo, Norway. 28. Jan 2009
* REVISED BY : 
*
*********************************************************************
*/


{
  int kstat;          /* Status variable                                 */
  int kiv;            /* Counter for vertices                            */
  int kid;            /* Counter for dimension                           */
  int kn;             /* The number of B-splines, i.e., the dimension of
			 the spline space associated with the knot
			 vector.                                         */
  int kk;             /* The polynomial order of the curve.              */
  int kind;           /* Type of curve, 2 and 4 are rational curves.     */
  int kdim;           /* The dimension of the space in which the curve
			 lies. Equivalently, the number of components
			 of each B-spline coefficient.                   */
  int kvert;          /* Counter for position in projected vertex array   */
  int kpos=0;         /* Position of error                               */
  
  double *svecpv=SISL_NULL;/* Pointer to vector from pointin plane to vertex  */
  double *sproj=SISL_NULL; /* Pointer to projected vertex array                */
  double *st;         /* Pointer to the first element of the knot vector
			 of the curve. The knot vector has [kn+kk]
			 elements.                                       */
  double *svert;      /* Pointer to a vertex                             */
  double *scoef;      /* Pointer to the first element of the curve's
			 B-spline coefficients. This is assumed to be an
			 array with [kn*kdim] elements stored in the
			 following order:
			 First the kdim components of the first B-spline
			 coefficient, then the kdim components of the
			 second B-spline coefficient and so on.          */
  
  double recnormproj; /* The reciprocal of the scalar product of the
			 plane normal vector and the projection vector */
  double tdist;       /* Distance */
  
  /* Check if curve is correct */
  
  s1707(pc,&kstat);
  if (kstat < 0) goto error;

  /* Describe curve with local parameters.  */
  kn = pc -> in;
  kk = pc -> ik;
  st = pc -> et;
  kind = pc->ikind;
  if (kind == 2 || kind == 4)
    scoef = pc->rcoef;
  else
    scoef = pc->ecoef;
  kdim = pc -> idim;          

  /* Check if conflicting dimensions */  
  
  if (kdim != idim) goto err106;
  if (kind == 2 || kind == 4)
    kdim++;
  
  svecpv = newarray(idim,DOUBLE);
  if (svecpv == SISL_NULL) goto err101;
  
  sproj = newarray(kdim*kn,DOUBLE);
  if (sproj == SISL_NULL) goto err101;
  
  /* Find reciprocal of scalar product */
  recnormproj = 1.0 / s6scpr(enorm,projdir,idim);
  
  /* Do for all vertices */
  
  kvert = 0;
  for (kiv=0; kiv<kn; kiv++) 
    {
      /* Find vector from point in plane to vertex */
      
      svert = scoef + kiv*kdim;
      s6diff(svert,epoint,idim,svecpv);
      
      /* Find distance between the vertex and the plane
	 along the projection vector */
      
      tdist  =  s6scpr(svecpv,enorm,idim) * recnormproj;
      
      /* Find the projected vertex */
      
      for (kid=0; kid<idim; kid++,kvert++)
	{
	  sproj[kvert] = scoef[kvert] - tdist*projdir[kid];
	}  
      if (kind == 2 || kind == 4)
	{
	   sproj[kvert] = scoef[kvert];
	   kvert++;
	}
    }
  
  /* Make the projected curve */
  
  *rc = SISL_NULL;              
  *rc = newCurve(kn,kk,st,sproj,kind,idim,1);
  if (*rc == SISL_NULL) goto err101;                
  
  /* Copy cuopen flag.  */
  
  (*rc)->cuopen = pc->cuopen;
  
  *jstat = 0;
  goto out;
  
  /* Error in memory allocation */
  
 err101: *jstat = -101;
  s6err("s1631",*jstat,kpos);
  goto out;
  
  /* Error in input, conflicting dimensions */
  
 err106: *jstat = -106;
  s6err("s1631",*jstat,kpos);
  goto out;
  
  
  /* Error in lower level function */  
  
 error:  *jstat = kstat;
  s6err("s1631",*jstat,kpos); 
  goto out;
  
 out:
  if (svecpv != SISL_NULL) freearray(svecpv);
  if (sproj  != SISL_NULL) freearray(sproj);
  return;
}
