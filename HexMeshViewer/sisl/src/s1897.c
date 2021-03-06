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
 * $Id: s1897.c,v 1.3 2005-02-28 09:04:49 afr Exp $
 *
 */


#define S1897

#include "sislP.h"

#define MAX_IK    50

#if defined(SISLNEEDPROTOTYPES)
void
s1897 (double et[], int ik, double ax, int left, int deriv,
       double ebiatx[], int *jstat)
#else
void
s1897 (et, ik, ax, left, deriv, ebiatx, jstat)
     double et[];
     int ik;
     double ax;
     int left;
     int deriv;
     double ebiatx[];
     int *jstat;
#endif
/*
*********************************************************************
*
*********************************************************************
*
* PURPOSE    :	To calculate the value of all the ideriv-th derivative
*		the posible nonzero B-splines at ax of order ik with knot
*		sequence et.
*
* INPUT      :	et	- Knot sequence, of length left + ik, assumed to be
*			  non decreasing.
*			  DIVISION BY ZERO will result if et[left]==et[left+1]
*		ik	- The order of the B-spline.
*		ax	- The point at wich the B-splines are to be evaluated.
*		left	- An integer chosen so that et[left] <= ax < et[left+1]
*		deriv	- Which derivative of the B-spline to evaluate.
*
* OUTPUT     :	ebiatx	- Array of length ik, with ebiatx[i] contining the value
*			  at ax of the B-spline B(left-ik+i,ik,t)
*		jstat	- Status variable:
*                                               > 0     : warning
*                                               = 0     : ok
*                                               < 0     : error
*
* METHOD     :
*
* REFERENCES :	Fortran version by Tor Dokken, SI, 1982-03
*
* CALLS      :
*
* WRITTEN BY :	Trond Vidar Stensby, SI, 1991-07
*
*********************************************************************
*/
{
  int kpos = 0;
  int local_array_allocated = FALSE;
  int j;			         /* Loop control variables.    */
  int count;
  double dummy;			         /* Used for temporary calculations.*/
  double fak;
  double term;
  double saved;
  double stmp[2*MAX_IK + 1];             /* temporary storage              */
  double *sltmp = SISL_NULL;                  /* temp storage allocated only 
					    if ik > MAX_IK                 */
  double *edltr = SISL_NULL;                  /* pointer into temporary storage */
  double *edltl = SISL_NULL;                  /* pointer into temporary storage */
  
  /*
   * Initialize.
   * -----------
   */

  *jstat = 0;

  if (ik > MAX_IK)
    {
       /*
        * We need to allocate a larger local tmp array; Do so.
	* ----------------------------------------------------
	*/
       
       if ((sltmp = newarray(2 * ik + 1, DOUBLE)) == SISL_NULL)
	 goto err101;
       local_array_allocated = TRUE;
       
    }
  else
    sltmp = stmp;

  /*
   * Set pointer into local array.
   * -----------------------------
   */
         
  edltr = sltmp;
  edltl = sltmp + ik;

  ebiatx[0] = (double) 1.0;

  for (j = 1; j <= deriv; j++)
    {
      edltr[j - 1] = et[left + j] - ax;
      edltl[j - 1] = ax - et[left + 1 - j];
      fak = (double) j;

      saved = (double) 0.0;
      for (count = 1; count <= j; count++)
	{
	  dummy = edltr[count - 1] + edltl[j - count];
	  if (dummy <= (double) 0.0)
	    goto err112;

	  term = fak * ebiatx[count - 1] / dummy;
	  ebiatx[count - 1] = saved - term;
	  saved = term;
	}
      ebiatx[j] = saved;
    }

  for (; j < ik; j++)
    {
      edltr[j - 1] = et[left + j] - ax;
      edltl[j - 1] = ax - et[left + 1 - j];
      fak = ((double) j) / (double) (j - deriv);

      saved = (double) 0.0;
      for (count = 1; count <= j; count++)
	{
	  dummy = edltr[count - 1] + edltl[j - count];
	  if (dummy <= (double) 0.0)
	    goto err112;

	  term = fak * ebiatx[count - 1] / dummy;
	  ebiatx[count - 1] = saved + edltr[count - 1] * term;
	  saved = edltl[j - count] * term;
	}
      ebiatx[j] = saved;
    }

  /* OK */

  goto out;

  /* Error in scratch allocation. */

err101:
  *jstat = -101;
  s6err ("s1897", *jstat, kpos);
  goto out;

  /* Error in knot vector. */

err112:
  *jstat = -112;
  s6err ("s1897", *jstat, kpos);
  goto out;

out:
  if (local_array_allocated)
    freearray (sltmp);
  return;
}

#undef MAX_IK
