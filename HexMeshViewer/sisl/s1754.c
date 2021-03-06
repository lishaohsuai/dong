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
 * $Id: s1754.c,v 1.3 2005-02-28 09:04:48 afr Exp $
 *
 */


#define S1754

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
s1754 (double *et, int in, int ik, int ikh, double **iknt,
       int *inh, int *jstat)

#else
void
s1754 (et, in, ik, ikh, iknt, inh, jstat)
     double *et;
     int in;
     int ik;
     int ikh;
     double **iknt;
     int *inh;
     int *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    : 	To produce a knot vector of order higher than an other knot
*		vector reflecting the same continuity as the original knot
*		vector.
*
*
* INPUT      : 	et		- The original knot vector.
*				  Dimension (1:in+ik).
*		in		- The number of degrees of freedom in the
*				  B-spline given by the knot vector.
*		ik		- The order of the basis.
*		ikh		- The order of the knot vector to be produced.
*
* OUTPUT     :  iknt		- The 'extended' knot vector.
*				  Dimension (1:inh+ikh).
*		inh		- The number of degrees of freedom in the knot
*				  vector produced.
*               jstat           - Output status:
*                                  < 0: Error.
*                                  = 0: Ok.
*                                  > 0: Warning.
*
* METHOD     :	At et[ik-1], ikh knots are inserted. For internal knot values,
*		the multiplicity 'M' is found, the continuity determined
*		'ik-M-1', and the new multiplicity 'ikh-ik+M' deternined.
*		At et[in], ikh knots are inserted.
*
* REFERENCES :	Fortran version:
*		T.Dokken, SI, 1981-10
*
* CALLS      :  s6err.
*
* WRITTEN BY : 	Christophe R. Birkeland, SI, 1991-07
* REWRITTEN BY :
* REVISED BY :
*
*********************************************************************
*/
{
  int ki, kj;			/* Loop control parameters 		*/
  int kstart, kstop;
  int numb;
  int kpos = 0;			/* Position indicator for errors	*/
  int kant;			/* Equals ikh-ik			*/
  double prev, par;		/* Parameters used to find consecutive
				   distinct knotvector values		*/
  double tstart, tstop;		/* tstart=et[ik-1], tstop=et[in]	*/

  *jstat = 0;


  /* Test if legal input. */

  if (ik < 1 || ikh < ik || in <ik)
    goto err112;


  /* Test if input knot vector degenerate. */

  if (et[ik - 1] >= et[in])
    goto err112;


  /* Allocate internal array arr. */

  *iknt = newarray ((in +ik) *(ikh - ik + 1), DOUBLE);
  if (*iknt == SISL_NULL)
    goto err101;


  /* If ik=ikh, just copy knots. */

  kstop = in +ik;
  if (ik == ikh)
    {
      *inh = in;
      memcopy (*iknt, et, kstop, DOUBLE);
      goto out;
    }

  /* PRODUCTION OF KNOTS: First we fill in extra knots at each
     distinct knot value, then we remove the superfluous knots. */

  numb = 0;
  kant = ikh - ik;
  prev = et[0] - 1;
  for (ki = 0; ki < kstop; ki++)
    {
      par = et[ki];
      if (par < prev)
	goto err112;

      if (par != prev)
	{
	  /* New distinct knot value, insert additional knots. */

	  for (kj = 0; kj < kant; kj++, numb++)
	    (*iknt)[numb] = par;
	}
      (*iknt)[numb] = par;
      prev = par;
      numb++;
    }

  /* Remove superfluous knots at start. Find greatest start knot. */

  kstart = 0;
  tstart = et[ik - 1];
  while ((*iknt)[kstart] <= tstart)
    kstart++;
  kstart--;


  /* Find smallest end knot. */

  kstop = numb - 1;
  tstop = et[in];
  while ((*iknt)[kstop] >= tstop)
    kstop--;
  kstop++;


  /* The knots from kstart-ikh+1 up to
   * kstop+ikh-1 are the knots to be kept. */

  *inh = kstop - kstart + ikh - 1;


  /* Copy the knots to be kept to the start of the knot array. */

  kstart -= ikh - 1;
  kstop = *inh + ikh;
  memcopy (*iknt, &(*iknt)[kstart], kstop, double);

  goto out;

  /* Memory error or error in allocation. */

err101:
  *jstat = -101;
  s6err ("s1754", *jstat, kpos);
  goto out;

  /* Error in description of B-spline. */

err112:
  *jstat = -112;
  s6err ("s1754", *jstat, kpos);
  goto out;

out:
  if (*iknt != SISL_NULL)
    {
      *iknt = increasearray (*iknt, *inh + ikh, DOUBLE);
      if (*iknt == SISL_NULL)
	goto err101;
    }
  return;
}
