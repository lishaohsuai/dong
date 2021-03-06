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
 * $Id: s6idput.c,v 1.2 2001-03-19 15:59:01 afr Exp $
 *
 */


#define S6IDPUT

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void 
s6idput(SISLIntdat **rintdat,SISLIntdat *pintdat,int inr,double apar,int *jstat)
#else
void s6idput(rintdat,pintdat,inr,apar,jstat)
     SISLIntdat **rintdat;
     SISLIntdat *pintdat;
     int    inr;
     double apar;
     int    *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*                                                                   
* PURPOSE    : To insert all points in one intdat with one less number
*              of parameters into rintdat. New copies is made with
*              the missing parameter. All listes is also to
*              be uppdated.
*
*
*
* INPUT      : pintdat  - Pointer to intersection data with one less
*                         parameter than rintdat.
*              inr      - Number of the parameter that is missing in pintdat.
*              apar     - Parameter value of the missing parameter.
*
*
* OUTPUT     : rintdat  - Pointer to a pointer to intersection data.
*              jstat    - status messages  
*                               = 0      : Inserting done.
*                               < 0      : error
*
*
* METHOD     : 
*
*
* REFERENCES :
*
*-
* CALLS      : s6err      - Gives error message.
*              s6idnpt    - Insert a new intpt structure.
*              copyIntpt  - Copy an intpt structure.
*              newIntdat  - Create new intdat structure.
*
* WRITTEN BY : Arne Laksaa, 05.89.
*              UJK, 01.91 Changed a call to newIntpt, copying the
*                         attribute adist.
*********************************************************************
*/                                     
{
  int kstat;                    /* Local status variable.               */
/*guen  int kpos;    */                 /* Position of error.                   */
/*guen changed into: */
  int kpos=0;                     /* Position of error.                   */
  int ki,kj;                    /* Counters                             */
  int kant;                     /* Number of parameters in new points.  */
  double *spar = SISL_NULL;          /* Storing uppdated parametervalues.    */
  SISLIntpt **uintpt = SISL_NULL; /* Pointers to new intersection points. */
  
  
  /* We have to be sure that we have an intdat structure. */
  
  if (pintdat == SISL_NULL)
    {
      *jstat = 0;
      goto out;
    }
  
  /* Computing number of new parameter direction. */
  
  kant = pintdat->vpoint[0]->ipar + 1;
  
  
  if (inr<0 || inr>=kant) goto err191;
  
  
  /* Allocating an array for intersection points. */
  
  if ((uintpt = newarray(pintdat->ipoint,SISLIntpt *)) == SISL_NULL)
    goto err101;
  
  /* Allocating an array for parametervalues. */
  
  if ((spar = newarray(kant,double)) == SISL_NULL)
    goto err101;
  
  
  /* Making copies of all intersection points. */
  
  for (ki=0; ki<pintdat->ipoint; ki++)
    {
      /* First we have to insert the missing parameter value. */
      
      for(kj=0; kj<inr; kj++) spar[kj] = pintdat->vpoint[ki]->epar[kj];
      spar[kj] = apar;
      for(kj++; kj<kant; kj++) spar[kj] = pintdat->vpoint[ki]->epar[kj-1];
      
      
      /* UJK,01-91 bringing over the adist value ! */
      uintpt[ki] = newIntpt(kant,spar,pintdat->vpoint[ki]->adist);
    }
  
  
  /* Than we can insert all new intersection points in rintdat. */
  
  for (ki=0; ki<pintdat->ipoint; ki++)
    {
      s6idnpt(rintdat,&uintpt[ki],1,&kstat);
      if (kstat < 0) goto error;
    }
  
  /* Than we can uppdate all pcurve pointers (lists). */
  
  for (ki=0; ki<pintdat->ipoint; ki++)
    if (pintdat->vpoint[ki]->pcurve != SISL_NULL)
      {
	for (kj=0;kj<pintdat->ipoint;kj++)
	  if (pintdat->vpoint[ki]->pcurve == pintdat->vpoint[kj])
	    break;
	
	if (kj == pintdat->ipoint) goto err190;
	
	s6idcon(rintdat,&uintpt[ki],&uintpt[kj],&kstat);
	if (kstat < 0) goto error;
      }
  
  
  *jstat = 0;
  goto out;
  

/* Error in inserted parameter number.  */

err191: *jstat = -191;
        s6err("s6idput",*jstat,kpos);
        goto out;
/* Error in intersection list.  */

err190: *jstat = -190;
        s6err("s6idput",*jstat,kpos);
        goto out;

/* Error in space allocation.  */

err101: *jstat = -101;
        s6err("s6idput",*jstat,kpos);
        goto out;

/* Error in sub function.  */

error: *jstat = kstat;
        s6err("s6idput",*jstat,kpos);
        goto out;

 out: if (uintpt != SISL_NULL) freearray(uintpt);
      if (spar   != SISL_NULL) freearray(spar);
}
