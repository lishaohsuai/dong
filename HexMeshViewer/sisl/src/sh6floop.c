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
 * $Id: sh6floop.c,v 1.3 2001-03-19 15:59:07 afr Exp $
 *
 */


#define SH6FLOOP

#include "sislP.h"


#if defined(SISLNEEDPROTOTYPES)
void
      sh6floop(SISLIntpt *vedgept[],int inum,int *jpt,int *jstat)
#else
void sh6floop(vedgept,inum,jpt,jstat)
   SISLIntpt *vedgept[];
   int       inum;
   int       *jpt;
   int       *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    : Given an array of edge intersection. Sort the array in
*              such a way that the intersection points connected to the
*              first point lie at the start of the array. Count the
*              number of intersections belonging to the first list, and
*              give a status dependant on the kind of list.
*
*
* INPUT      : vedgept  - Intersection points at the edges of the
*                         objects.
*              inum     - Number of intersections in vedgept.
*
* OUTPUT     : jpt      - Number of intersections in the first list.
*              jstat    - Status
*                         jstat = 0   => Successful. List is open.
*                         jstat = 1   => Successful. List is closed.
*                         jstat = 2   => First point is isolated.
*
*
*
* METHOD     :
*
*
* REFERENCES :
*
* WRITTEN BY : Vibeke Skytt, SI, 11.92.
* Revised by : Paal Fugelli, SINTEF, Oslo, Norway, Sept. 1994.  Initialized
*              gnext.
*
*********************************************************************
*/
{
   int kstat2 = -1;   /* Status of traversing the first list.           */
   int kpt = 0;       /* Current number of intersections in first loop. */
   int ki,kj;         /* Counters.                                      */
   SISLIntpt *qstart; /* First intersection point around the edges.     */
   SISLIntpt *qprev;  /* Previous intersection point found.             */
   SISLIntpt *qt;     /* Current intersection point in list.            */
   SISLIntpt *qnext = SISL_NULL;  /* The next point to enter the list.              */
   SISLIntpt *qhelp;  /* Help point used in sorting vedgept.            */

   /* Check if there is a list.  */

   *jpt = 0;
   if (inum == 0) goto out;

   /* Set start point. */

   qstart = vedgept[0];

   /* Traverse the edge intersections to fetch a list starting in qstart.
      The elements in the list must lie on the edges of the objects.    */

   for (qprev=SISL_NULL, qt=qstart; ; qt=vedgept[kpt])
   {
      if (kstat2 == 0)
      {
	 /* Open list. Travers in the opposite direction.  */

	 qt = qstart;
	 qprev = (kpt > 0) ? vedgept[1] : SISL_NULL;
      }

      for (ki=0; ki<qt->no_of_curves; ki++)
      {
	 /* Search all curves in this points to find the list.  */

	 qnext = sh6getnext(qt,ki);

	 if (qnext == SISL_NULL)
	 {
	    kstat2 = 0; break;  /* No point.  */
	 }
	 if (qnext == qprev) continue;  /* Traversing of list have turned.
				 	   Try next curve.                 */
	 if (qnext == qstart)
	 {
	    kstat2 = 1; break;  /* A closed loop is found.  */
	 }

	 /* An intersection is found. Check if it lies on the current
	    edges.               */

	 for (kj=kpt+1; kj<inum; kj++)
	    if (qnext == vedgept[kj]) break;

	 if (kj == inum) continue;  /* The point lies not at an edge. */

	 /* Change position in the array in such a way that the members
	    of the list are placed first.  */

	 kpt++;
	 qhelp = vedgept[kj];
	 vedgept[kj] = vedgept[kpt];
	 vedgept[kpt] = qhelp;

	 /* Set previous pointer.  */

	 qprev = qt;

	 /* Check if we are finished or may continue with the next point. */

	 if (qnext == SISL_NULL || (qnext == qstart && qnext != qprev) ||
	     qprev == qt) break;
      }

      /* Check if we have found the entire list.  */

      if (qnext == qstart || kpt >= inum) break;
      else if (ki == qt->no_of_curves)
      {
	 if (kstat2 < 0) kstat2 = 0;
	 else break;
      }
   }

   /* Set number of points in list and output status.  */

   *jpt = kpt + 1;
   if (kpt == 0) *jstat = 2;
   else if (kstat2 == 1) *jstat = 1;
   else *jstat = 0;

   out :
      return;
}
