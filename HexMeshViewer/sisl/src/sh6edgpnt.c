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
 * $Id: sh6edgpnt.c,v 1.2 2001-03-19 15:59:07 afr Exp $
 *
 */


#define SH6EDGPOINT
#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
sh6edgpoint (SISLEdge * vedge[], SISLIntpt *** wintpt, int *jnum,int *jstat)
#else
void
sh6edgpoint (vedge, wintpt, jnum, jstat)
     SISLEdge *vedge[];
     SISLIntpt ***wintpt;
     int *jnum;
     int *jstat;
#endif
 /*
 *********************************************************************
 *
 *********************************************************************
 *
 * PURPOSE    : Make an array of pointers pointing to different
 *              intersection points on edges.
 *
 *
 *
 * INPUT      : vedge[]  - SISLEdge intersection.
 *
 *
 * OUTPUT     : jnum     - Number of intersection points,
 *              wintpt   - Array of pointers to intersection points.
 *              jstat    - status messages
 *                           = 1     : Coinside found.
 *                           = 0     : no coinside.
 *                           < 0     : error
 *
 *
 * METHOD     :
 *
 *
 * REFERENCES :
 *
 * CALLS      : sh6getmain - Get main point in chain of help points.
 *
 *
 * WRITTEN BY : Arne Laksaa, SI, 89-06.
 *
 *********************************************************************
 */
{
  int lant[2];

  if (vedge[0] == SISL_NULL)
    lant[0] = 0;
  else
    lant[0] = vedge[0]->ipoint;

  if (vedge[1] == SISL_NULL)
    lant[1] = 0;
  else
    lant[1] = vedge[1]->ipoint;

  if (lant[0] + lant[1] > 0)
    {
      int kn1;			/* Number of int. pt. found.   */
      int kn, ki, kj;		/* Counters.                   */
      SISLPtedge *qpt;
      SISLIntpt *qintpt;	/* Intersection point.         */
      SISLIntpt *qmain;		/* Main point in chain of help points.      */

      /* Allocate array of pointers to the points. */

      if (((*wintpt) = newarray (lant[0] + lant[1],
				 SISLIntpt *)) == SISL_NULL)
	goto err101;


      /* Update the array. */

      for (kn1 = 0, kn = 0; kn < 2; kn++)
	if (lant[kn] > 0)
	  for (kj = 0; kj < vedge[kn]->iedge; kj++)
	    for (qpt = vedge[kn]->prpt[kj]; qpt != SISL_NULL; qpt = qpt->pnext)
	      {
		for (ki = 0; ki < kn1; ki++)
		  {
		    if (qpt->ppt == (*wintpt)[ki])
		      break;
		  }
		if (ki == kn1)
		  (*wintpt)[kn1++] = qpt->ppt;
	      }

      /* Traverse the array and remove help points if the corresponding
	 main point also lies in the array.     */

      for (ki = 0; ki < kn1; ki++)
	{
	  qintpt = (*wintpt)[ki];
	  if (sh6ishelp (qintpt))
	    {
	      /* A help point is found. Fetch the corresponding main point. */

	      qmain = sh6getmain (qintpt);

	      /* Check if the main point lies in the array. */

	      if (qmain)
		{
		  for (kj = 0; kj < kn1; kj++)
		    if (qmain == (*wintpt)[kj])
		      break;
		  if (kj < kn1)
		    (*wintpt)[ki] = SISL_NULL;
		}
	    }
	}

      /* Make sure that the array of int.pt. is dense.  */

      for (ki = 0, kj = kn1; ki < kj; ki++)
	if ((*wintpt)[ki] == SISL_NULL)
	  (*wintpt)[ki] = (*wintpt)[--kj];

      *jnum = kn1 = kj;
    }
  else
    *jnum = 0;

  *jstat = 0;
  goto out;

  /* Error in memory allocation.      */

err101:*jstat = -101;
  s6err ("sh6edgpoint", *jstat, 0);
  goto out;


out:;
}
