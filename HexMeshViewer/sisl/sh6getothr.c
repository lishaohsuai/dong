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
 * $Id: sh6getothr.c,v 1.2 2001-03-19 15:59:07 afr Exp $
 *
 */


#define SH6GETOTHER

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void 
      sh6getother(SISLIntpt *pt,SISLIntpt *pt1,SISLIntpt **pt2,int *jstat)
#else
void sh6getother(pt,pt1,pt2,jstat)
   SISLIntpt *pt;
   SISLIntpt *pt1;
   SISLIntpt **pt2;
   int       *jstat;
#endif   
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    : Given one neighbour pt1 of the point pt, find
*              the other neighbour if it is unique.
*              If pt is help point, look for both type of neighbours.
*              If main point, look only for main points.
*              pt and pt1 must be linked.
*
* INPUT      : pt       - SISLIntpt point.
*              pt1      - One neighbour.
*
* OUTPUT     :  pt2     - Second neighbour if unique.
*              jstat    - Error flag.
*                         jstat =  0  => successful, unique neighbour
*                         jstat =  1  => pt is end point, pt2 SISL_NULL
*                         jstat =  2  => pt is junction point, pt2 SISL_NULL
*                         jstat = -1  => pt1 and pt2 are not connected
*                         jstat = -2  => error in data structure.
*                         jstat <  0  => error in lower level routine
*
*
* METHOD     : 
*
*
* REFERENCES :
*
* WRITTEN BY : Michael Floater, SI, Oslo, Norway. June 91.
* CHANGED BY : Ulf J. Krystad, SI, Oslo, Norway. July 91.
*********************************************************************
*/
{
  int kstat;              /* Local status variable.    */
  int index,index1;      /* Indices for pt and pt1.   */
  int num;              /* count number of pointers    */
  int i;              /* Loop variable. */
  
   *pt2 = SISL_NULL;
   *jstat = 0;
  
  sh6getlist(pt,pt1,&index1,&index,&kstat);
  if(kstat < 0) goto error;
  if(kstat == 1) goto err1;

  if(sh6ismain(pt))  /* pt is main point. */
  {
      if(!sh6ismain(pt1)) goto err1;
      num=0;
      /* UJK, don't pass singular point ! */
      if (pt->iinter == SI_SING)
      {
	  *pt2 = SISL_NULL;
          *jstat = 2;
          goto out;
      }
	 
      for(i=0; i < pt->no_of_curves; i++)
      {
	  if(i != index1 && sh6ismain(pt->pnext[i]))
	  {
	      *pt2 = pt->pnext[i];
	      num++;
	  }
      }

      if(num == 0) *jstat = 1; /* pt is an end point. */
      else if(num > 1) /* pt is a junction point. */
      {
	  *pt2 = SISL_NULL;
          *jstat = 2;
      }
  }
  else  /* pt is help point. */
  {
      num=0;

      for(i=0; i < pt->no_of_curves; i++)
      {
	  if(i != index1)
	  {
	      *pt2 = pt->pnext[i];
	      num++;
	  }
      }

      if(num > 1) goto err2; /* Error in data structure. */
      if(num == 0) *jstat = 1; /* pt is an end point. */
  }
  
  goto out;
  

/* Error. pt1 and pt2 are not linked.  */

err1: *jstat = -1;
      s6err("sh6getother",*jstat,0);
      goto out;

/* Error in sub function.  */

/* Error in data structure. */

err2: *jstat = -2;
      s6err("sh6getother",*jstat,0);
      goto out;

/* Error in sub function.  */

error:  *jstat = kstat;
        s6err("sh6getother",*jstat,0);
        goto out;

   out:
      return;
}
