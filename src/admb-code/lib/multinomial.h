#ifndef __MULTINOMIAL_H
#define __MULTINOMIAL_H

#define TINY     1.e-08

dvariable mult_likelihood(const dmatrix &o, const dvar_matrix &p, dvar_matrix &nu, 
                          const dvariable &log_vn);


void dfcholeski_solve(void);
  
dvar_vector choleski_solve(_CONST dvar_matrix& MM,const dvar_vector& vv,
                           const prevariable& det,const int& sgn);

dvariable multivariate_t_likelihood(const dmatrix &o, const dvar_matrix &p);



#endif