/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* (c) Copyright 1995 by                                                     */
/*     SINTEF, Oslo, Norway                                                  */
/*     All rights reserved. See the sisl-copyright.h for more details.            */
/*                                                                           */
/*****************************************************************************/

#include "sisl-copyright.h"

/*
 *
 * $Id: s2515.c,v 1.3 2005-02-28 09:04:49 afr Exp $
 *
 */


#define S2515

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void
   s2515(SISLSurf *surf, int ider, int iside1, int iside2, double parvalue[],
      int *leftknot1,int *leftknot2, double mehlum[], int *stat)
#else
 void s2515(surf, ider, iside1, iside2, parvalue, leftknot1, leftknot2, 
	    mehlum, stat)
      SISLSurf *surf;
      int    ider;
      int    iside1;
      int    iside2;
      double parvalue[];
      int *leftknot1;
      int *leftknot2;
      double mehlum[];
      int *stat;
#endif
/*
***************************************************************************
*
***************************************************************************
*  PURPOSE      :  To compute the numerator and denominator in the
*                  mehlum expression M(u,v) of a Surface for given
*                  values (u,v) = (parvalue[0],parvalue[1]), where:
*
*                          et1[leftknot1] <= parvalue[0] < et1[leftknot1+1],
*                          et2[leftknot2] <= parvalue[1] < et2[leftknot2+1].
*                  See also s2508(), s2509().
*
*  INPUT        :
*          surf     - Pointer to the surface to evaluate.
*          ider     - Number of derivatives to calculate.
*                     Only implemented for ider=0.
*                       < 0 : No derivative calculated.
*                       = 0 : Position calculated.
*                       = 1 : Position and first derivative calculated.
*                       etc.
*          iside1   - Indicator telling if the derivatives in the first
*                     parameter direction is to be calculated from the
*                     left or from the right:
*                        <  0 calculate derivative from the left hand side
*                        >= 0 calculate derivative from the right hand side.
*          iside2   - Indicator telling if the derivatives in the second
*                     parameter direction is to be calculated from the
*                     left or from the right:
*                        <  0 calculate derivative from the left hand side
*                        >= 0 calculate derivative from the right hand side.
*      parvalue     - Parameter-value at which to evaluate. Dimension of
*                     parvalue is 2.
*
*  INPUT/OUTPUT :
*     leftknot1     - Pointer to the interval in the knot vector in the
*                     first parameter direction where parvalue[0] is found,
*                     that is:
*                          et1[leftknot1] <= parvalue[0] < et1[leftknot1+1].
*                     leftknot1 should be set equal to zero at the first call
*                     to the routine.
*
*     leftknot2     - Pointer to the interval in the knot vector in the
*                     second parameter direction where parvalue[1] is found,
*                     that is:
*                          et2[leftknot2] <= parvalue[1] < et2[leftknot2+1].
*                     leftknot2 should be set equal to zero at the first call
*                     to the routine.
*
*  OUTPUT       :
*     mehlum        - The nominator and denominator of the Mehlum curvature for the 
*                     surface in (parvalue[0],parvalue[1]).
*                     Size = 2.
*     mehlum[0]     - 3((eG-2fF+gE)^2)/8 - (eg-f*f)(EG-F*F)/2, where e,g and f are
*                     calculated using real length normals.
*     mehlum[1]     - (EG-F*F)^3, where e,g and f are calculated using real length 
*                     normals.
*     		      The Mehlum curvature for the surface is mehlum[0]/mehlum[1]
*        stat       - Status messages
*                         = 2 : Surface is degenerate at the point, that is,
*                               the surface is not regular at this point.
*                         = 1 : Surface is close to degenerate at the point.
*                               Angle between tangents is less than the angular
*                               tolerance.
*                         = 0 : Ok.
*                         < 0 : Error.
*
*  METHOD       :  The Mehlum curvature is given by
*
*                      M(u,v) = (3((eG-2fF+gE)^2)/8 - (eg-f*f)(EG-F*F)/2)/
*                               (EG-F*F)^3,
*
*                  The variables E,F,G,e,f and g
*                  are the coefficients of the first and second fundamental form.
*                  They are given by: e = <N,Xuu>, f = <N,Xuv>, g = <N,Xvv>,
*                  E = <Xu,Xu>, F = <Xu,Xv> and G = <Xv,Xv>. The routine will
*                  test if the surface is degenerate (not regular) or close to
*                  degenerate. 
*
*  REFERENCES   :  Differential Geometry of Curves and Surfaces,
*                    (Manfredo P. Do Carmo, Prentice Hall,
*                      ISBN: 0-13-212589-7).
*-
*  CALLS        :  s1422(),s2516().
*
*  LIMITATIONS  :
*                (i) If the surface is degenerated (not regular) at the point
*                    (u,v), it makes no sense to speak about the Mehlum
*                    curvature M(u,v). The routine returns stat = 2.
*               (ii) If the surface is closed to degenerate, the Mehlum
*                    curvature M(u,v) can be numerical instable. The routine returns
*                    stat = 1.
*              (iii) If the surface is Cr the curvature calculated is C(r-2).
*               (iv) The dimension of the space in which the surface lies must
*                    be 1,2 or 3.
*
*
*  WRITTEN BY :  Johannes Kaasa, SINTEF, Oslo, Norway.            Date: 1995-8
******************************************************************************
*/
{
   double derive[18];     /* Array containing the computed derivatives.      */
   double normal[3];      /* Array containing the computed normalvektor.     */
      
      
   if (ider != 0) goto err178;
      
   if (surf == SISL_NULL)  goto err150;
   else
   {
	 
      /* Compute derivates and normal. */
	 
      s1422(surf,2,iside1,iside2,parvalue,leftknot1,leftknot2,derive,normal,
	    stat);
      if (*stat < 0)       /* Error in lower level routine. */
         goto error;
      else if (*stat != 2) /* The surface is not degenerate */
      {
	 s2516(surf, ider, derive, normal, mehlum, stat);
	 if (*stat < 0) goto error;
      }
      else if (*stat == 2) /* The surface is degenerated. */
	 goto out;
	 
   }
      
   /* Successful computations  */
      
   goto out;

   /* Error. Input (surface) pointer is SISL_NULL. */
 err150:
   *stat = -150;
   s6err("s2515", *stat, 0);
   goto out;
      
   /* Illegal derivative requested. */
 err178:
   *stat = -178;
   s6err("s2515", *stat, 0);
   goto out;
   
   /* Error in lower level routine.  */      
 error:
   s6err("s2515",*stat,0);
   goto out;
      
 out:
	 
   return;
      
}
