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
 * $Id: s6sratder.c,v 1.3 2001-03-19 15:59:02 afr Exp $
 *
 */


#define S6SRATDER

#include "sislP.h"

#if defined(SISLNEEDPROTOTYPES)
void 
s6sratder(double eder[],int idim,int ider1,int ider2,double gder[],int *jstat)
#else
void s6sratder(eder,idim,ider1,ider2,gder,jstat)
     double eder[];
     int    idim;
     int    ider1;
     int    ider2;
     double gder[];
     int    *jstat;
#endif
/*
*********************************************************************
*                                                                   
* PURPOSE    : To calculate the value and ider1*ider2 derivatives of 
*              a rational B-spline surface.
*
* INPUT      : eder    - Double array of dimenson [(ider2+1)*(ider1+1)*(idim+1)]
*                        containing the position and the derivative vectors
*                        of the surface at the point with parameter value
*                        (epar[0],epar[1]).
*                        (idim+1 is the number of components of each B-spline
*                        coefficient, i.e. the dimension of the homogemous
*                        space in which the surface lies.)
*                        These vectors are stored in the following order:
*                        First the idim+1 components of the position vector,
*                        then the idim+1 components of the D(1,0) vector,
*                        and so on up to the idim+1 components of the D(ider1,0)
*                        vector,
*                        then the idim+1 components of the D(0,1) vector etc.
*                        Equivalently, if eder is considered to be a
*                        three dimensional array, then its declaration in C
*                        would be eder[ider2+1,ider1+1,idim+1]
*              idim    - The dimension of the non homogenous space
*              ider1   - The number of input derivatives with respect
*                        to the first parameter direction.
*              ider2   - The number of input derivatives with respect
*                        to the second parameter direction.
*
*
* OUTPUT     : jstat   - Status message
*                                        >0      : Warning
*                                        =0      : ok
*                                        <0      : Error
*              gder    - Double array of dimension [(ider2+1)*(ider1+1)*idim]
*                        containing the position and the derivative vectors
*                        of the surface at the point with parameter value
*                        (epar[0],epar[1]).
*                        (idim is the number of components of each B-spline
*                        coefficient, i.e. the dimension of the Euclidean
*                        space in which the surface lies.)
*                        These vectors are stored in the following order:
*                        First the idim components of the position vector,
*                        then the idim components of the D(1,0) vector,
*                        and so on up to the idim components of the D(ider1,0)
*                        vector,
*                        then the idim components of the D(0,1) vector etc.
*                        Equivalently, if eder is considered to be a
*                        three dimensional array, then its declaration in C
*                        would be gder[ider2+1,ider1+1,idim]
*
*
* METHOD     :  The surface P(u,v) can be written as the quotient
*               P(u,v) = T(u,v) / w(u,v) where T and w are ordinary splines.
*               The dimensions of T and w are idim and 1
*               respectively. The array eder contains position
*               and derivatives of the idim+1 dimensional surface
*               (T(u,v),w(u,v)).
*
*               Now, since wP = T, we find, by the Leibnitz formula,
*
*      k   l
*                  k!       l!     (k-i,l-j) (i,j)         (k,l)
*     sum sum   -------- -------- w         P         =   T       .
*               i!(k-i)! j!(l-j)!
*     i=0 j=0
*
*               Therefore
*               
*
*              --            k   l                                     --
*      (k,l)   |   (k,l)                k!       l!     (k-i,l-j) (i,j) |    
*     P      = |  T      -  sum sum  -------- -------- w         P      | / w .
*              |                     i!(k-i)! j!(l-j)!                  |
*              --           i=0 j=0                                    --
*                               i+j<k+l
*
*               This formula is applied recursively to evaluate P's derivatives.
*
*                                                          MF.
*
*
*
* CALLS      :
*
* WRITTEN BY : Per Evensen, SI, Oslo, Norway. 1989-apr-1989
* REVISED BY : Michael Floater, 30/9/91 Bug fix for derivatives.
* REWRITTEN BY : Michael Floater, SI, 16/12/91. New algorithm.
* REWRITTEN BY : Michael Floater, SI, 25/8/92. Extend to arbitrary
*                   number of derivatives (by Leibnitz). Finally!
* REVISED BY : Michael Floater, SI, 8/9/92. Optimised.
*
*********************************************************************
*/
{
  int kpos=0;          /* Position of error.                     */
  double w0;           /* The denominator.                       */
  int ki;              /* Count through dimensions.              */
  int idu;             /* Count through derivatives in u.        */
  int idv;             /* Count through derivatives in v.        */
  int *binom=SISL_NULL;     /* Array for binomial coefficients.       */
  int *binomu=SISL_NULL;    /* Pointer to binomial coefficients in u. */
  int *binomv=SISL_NULL;    /* Pointer to binomial coefficients in v. */
  double *sum1=SISL_NULL;   /* Leibnitz expansion in u                */
  double *sum2=SISL_NULL;   /* Leibnitz expansion in u and v.         */
  double sumdum1[4];   /* Fixed space for sum1.                  */
  double sumdum2[4];   /* Fixed space for sum2.                  */
  int idimp1;          /* idim + 1.                              */
  int iw;              /* Pointer to a weight.                   */
  int iwbase;          /* Starting value of iw on each row.      */
  int igder;           /* Pointer to already calculated derivs.  */
  int igbase;          /* Starting value of igder on each row.   */
  int i,iu,iv,j,k;     /* Counters.                              */
  int ider1p1;         /* ider1 + 1.                             */
  int ider2p1;         /* ider2 + 1.                             */
  int igrow;           /* (ider1+1) * idim.                      */  
  int iwrow;           /* (ider1+1) * idimp1.                    */  
  int iwfix;           /* Index of initial weight in sum.        */
  int bidum[10];       /* Array for storing binomial coeffs.     */
  int idermax;         /* maximum of ider1 and ider2.            */
  double temp;         /* Temporary multiple.                    */
  
  if (ider1<0 || ider2<0) goto err178;
  if (idim<1) goto err102;
  
  *jstat = 0;

  /* Find denominator. */ 
  
  w0 = eder[idim];
  if (DEQUAL(w0,DZERO)) w0 = (double)1.0;

  /* If we're only asked for position, we'll do it
     now and exit for the sake of speed. */

  if(ider1 == 0 && ider2 == 0)
  {
      for(ki=0; ki<idim; ki++)
      {
          gder[ki] = eder[ki] / w0;
      }

      goto out;
  }

  /* Set up some constants. */

  idimp1  = idim + 1;
  ider1p1 = ider1 + 1;
  ider2p1 = ider2 + 1;
  igrow   = ider1p1 * idim;
  iwrow   = igrow + ider1p1;  /* = iderp1 * idimp1  */


  /* Set up  binomial coefficients.
     Use new array only when ider1 > 3 or ider2 > 3. */

  idermax = max(ider1,ider2);

  if (idermax > 3)
  { 
      binom = newarray(((idermax+1)*(idermax+2)) >> 1, INT);
      if(binom == SISL_NULL) goto err179;
  }
  else
  { 
      binom = bidum;
  }

  for(j=0,k=0; j<=idermax; j++,k+=j)
  {
      /* Calculate the new row of binomial coefficients. */
  
      binom[k] = 1;
  
      for(i=k+1; i<k+j; i++)
      {
          binom[i] = binom[i-j-1] + binom[i-j];
      }

      binom[k+j] = 1;
  }
  

  /* Set up space for sum1 and sum2 if necessary.
     Use new arrays only when idim > 4. */

  if (idim > 4)
  { 
      sum1 = newarray(idim, DOUBLE);
      if(sum1 == SISL_NULL) goto err179;
      sum2 = newarray(idim, DOUBLE);
      if(sum2 == SISL_NULL) goto err179;
  }
  else
  { 
      sum1=sumdum1;
      sum2=sumdum2;
  }


  /* Loop through derivatives in u and v. */

  for(idv=0,binomv=binom,j=0,k=0; idv<=ider2; idv++,binomv+=idv)
  {

      for(idu=0,binomu=binom; idu<=ider1; idu++,k++,binomu+=idu)
      {

    
          if(idu == 0 && idv == 0)
          {
  	      /* Position is a special case. */
    
              for(ki=0; ki<idim; ki++,j++,k++)
              {
                  gder[j] = eder[k] / w0;
              }
    
          }
          else
          {
    
              /* Calculate each coefficient of the (idu,idv)'th
		 derivative of the rational surface (in gder).
        
  	       This requires calculating the Liebnitz sum from
  	       the subarray of gder (0,..,idu, 0,...,idv) and
  	       the subarray of eder (0,..,idu, 0,...,idv). */
  
  	      iwfix = k + idim;
  
  	      /* Calculate the Leibnitz sum. */
        
              for(ki=0; ki<idim; ki++)
	      {
  	          sum2[ki] = (double)0.0;
	      }
        
              for(iv=0,igbase=0,iwbase=iwfix;
  	      iv<=idv;
  	      iv++,igbase+=igrow,iwbase-=iwrow)
              {
        
                  for(ki=0; ki<idim; ki++)
	          {
  	              sum1[ki] = (double)0.0;
	          }
 
                  for(iu=0,igder=igbase,iw=iwbase;
  	          iu<=idu;
  	          iu++,iw-=idimp1)
                  {
  		      /* Add the next Leibnitz term unless we
  		         have reached the last one (the unknown). */
  
  	              if(iu<idu || iv<idv)
  		      {
  			  /* If iu=0 or iu=idu, the u binomial
  			     coefficient is 1 so don't multiply. */
  
  		          if(iu>0 && iu<idu)
  			  {
  			    temp = (double)binomu[iu] * eder[iw];

                            for(ki=0; ki<idim; ki++,igder++)
                            {
  			      sum1[ki] += temp * gder[igder];
  			    }
  			  }
  			  else
  			  {
                            for(ki=0; ki<idim; ki++,igder++)
                            {
  			      sum1[ki] += eder[iw] * gder[igder];
  			    }
  			  }
  
  		      }
                  }
  
  		  /* If iv=0 or iv=idv, the v binomial
  		     coefficient is 1 so don't multiply. */
  
  		  if(iv>0 && iv<idv)
  		  {
                    for(ki=0; ki<idim; ki++)
		    {
  		        sum2[ki] += (double)binomv[iv] * sum1[ki];
		    }
  		  }
  		  else
  		  {
                    for(ki=0; ki<idim; ki++)
		    {
  		        sum2[ki] += sum1[ki];
		    }
  		  }
  
              }
        
              for(ki=0; ki<idim; ki++,j++,k++)
	      {
  	          gder[j] = (eder[k] - sum2[ki]) / w0;
	      }
        
          }
  
      }
    
  }


  /* Free arrays. */

  if (idermax > 3)
  { 
      freearray(binom);
  }
  
  if (idim > 4)
  { 
      freearray(sum1);
      freearray(sum2);
  }
  


  /* Done. */

  
  goto out;


/* idim less than 1. */
 err102: *jstat = -102;
         s6err("s6ratder",*jstat,kpos);
         goto out;

/* Derivative negative */
 err178: *jstat = -178;
         s6err("s6ratder",*jstat,kpos);
         goto out;


/* Not enough memory */
 err179: *jstat = -179;
         s6err("s6ratder",*jstat,kpos);
         goto out;


out: 
return;
}

