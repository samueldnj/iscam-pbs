#include "../../include/utilities.h"
#include "../../include/Logger.h"

#ifndef NA
#define NA -99.0
#endif

adstring stripExtension(adstring fileName){
  /*
		This function strips the file extension
		from the fileName argument and returns
		the file name without the extension.
  */
  const int length = fileName.size();
  for (int i=length; i>=0; --i){
    if (fileName(i)=='.'){
      return fileName(1,i-1);
    }
  }
  return(fileName);
}

ivector getIndex(const dvector& a, const dvector& b){
  /*
    Returns a dvector which contains the elements of dvector a where
    the elements of b are not NA
    Assumes the length of a and b are the same.
   */
  int i,j,n;
  n = 0;
  j = 1;
  for(i = a.indexmin(); i <= a.indexmax(); i++){
    if(b(i) != NA){
      n++;
    }
  }
  ivector tmp(1,n);
  for(i = a.indexmin(); i <= a.indexmax(); i++){
    if(b(i) != NA){
      tmp(j++) = a(i);
    }
  }
  return(tmp);
}

void write_proj_headers(ofstream &ofsP,
                        int syr,
                        int nyr,
                        bool include_msy,
                        bool include_sbo){
  // Write the decision table headers for projection years
  ofsP<<"TAC"                  <<",";
  ofsP<<"B"<<nyr+1             <<",";
  ofsP<<"B"<<nyr+2             <<",";
  if(include_sbo){
    ofsP<<"B0"                   <<",";
    ofsP<<"04B0"                 <<",";
    ofsP<<"03B0"                 <<",";
    ofsP<<"02B0"                 <<",";
  }
  ofsP<<"B"<<syr               <<",";
  ofsP<<"B"<<nyr+2<<"B"<<nyr+1 <<",";  //want probability B2016<B2015 - this will be < 1 if true
  if(include_sbo){
    ofsP<<"B"<<nyr+2<<"04B0"     <<",";  //want probability B2016<0.4B0 - this will be < 1 if true
    ofsP<<"B"<<nyr+2<<"02B0"     <<",";  //want probability B2016<0.4B0 - this will be < 1 if true
  }
  ofsP<<"B"<<nyr+2<<"B"<<syr     <<",";
  ofsP<<"F"<<nyr                 <<",";
  ofsP<<"F"<<nyr+1               <<",";
  ofsP<<"F"<<nyr+1<<"F"<<nyr     <<",";  //want probability F2015>F2014     - this will be > 1 if true
  ofsP<<"U"<<nyr+1               <<",";
  ofsP<<"U"<<nyr+1<<"U"<<nyr     <<",";
  ofsP<<"PropAge3"               <<",";
  ofsP<<"PropAge4to10"           <<",";
  ofsP<<"UT"                     <<",";
  ofsP<<"U20";
  if(include_msy){
    //MSY based ref points
    ofsP<<","<<"BMSY"            <<",";
    ofsP<<"B"<<nyr+2<<"BMSY"     <<",";  //want probability B2016<BMSY - this will be < 1 if true
    ofsP<<"B"<<nyr+2<<"08BMSY"   <<",";  //want probability B2016<0.8BMSY - this will be< 1 if true
    ofsP<<"B"<<nyr+2<<"04BMSY"   <<",";  //want probability B2016<0.4BMSY - this will be < 1 if true
    ofsP<<"FMSY"                 <<",";
    ofsP<<"F"<<nyr+1<<"FMSY"     <<",";
    ofsP<<"UMSY"                 <<",";
    ofsP<<"U"<<nyr+1<<"UMSY";            //want probability F2015>FMSY - this will be > 1 if true
  }
  ofsP<<"\n";
}

void write_proj_output(ofstream &ofsP,
                       int syr,
                       int nyr,
                       int nage,
                       double tac,
                       int pyr,
                       dvector p_sbt,
                       dmatrix p_ft,
                       dmatrix p_N,
                       dvar3_array M,
                       dmatrix ma,
                       dmatrix dWt_bar,
                       dvar_matrix ft,
                       double sbo,
                       dmatrix fmsy,
                       dvector bmsy,
                       bool include_msy,
                       bool include_sbo){

  double  ut  = tac / ( tac + p_sbt(pyr) );
	double u20  = tac / ( (p_N(pyr)(3,nage)*exp(-value(M(1)(nyr,3))))* dWt_bar(1)(3,nage) );

  double NAge3 = (p_N(pyr)(3)*dWt_bar(1)(3)*ma(1)(3));
	dvar_vector NallWt = elem_prod(p_N(pyr)(2,nage),dWt_bar(1)(2,nage));
	dvar_vector NallWtMat= elem_prod(NallWt(2,nage),ma(1)(2,nage));

	double sumAge2to10 = value(sum(NallWtMat(2,nage)));
	double sumAge4to10 = value(sum(NallWtMat(4,nage)));
	double propAge3 = NAge3/sumAge2to10;
	double propAge4to10 = sumAge4to10/sumAge2to10;

  // Write the projection output to the file
  ofsP<<tac                        <<",";
  ofsP<<p_sbt(pyr)                 <<",";
  ofsP<<p_sbt(pyr+1)               <<",";
  if(include_sbo){
    ofsP<<sbo                      <<",";
    ofsP<<0.4*sbo                  <<",";
    ofsP<<0.3*sbo                  <<",";
    ofsP<<0.2*sbo                  <<",";
  }
  ofsP<<p_sbt(syr)                 <<",";
  ofsP<<p_sbt(pyr+1)/p_sbt(pyr)    <<",";
  if(include_sbo){
    ofsP<<p_sbt(pyr+1)/(0.4*sbo)   <<",";
    ofsP<<p_sbt(pyr+1)/(0.2*sbo)   <<",";
  }
  ofsP<<p_sbt(pyr+1)/p_sbt(syr)    <<",";
  //ofsP<<ft(1)(1,nyr)             <<",";
  ofsP<<ft(1,nyr)                  <<",";
  ofsP<<p_ft(pyr,1)                <<",";
  ofsP<<p_ft(pyr,1)/ft(1,nyr)      <<",";
  ofsP<<(1. - mfexp(-p_ft(pyr,1))) <<",";
  ofsP<<(1. - mfexp(-p_ft(pyr,1)))/(1. - mfexp(-ft(1,nyr))) <<",";
  ofsP<<propAge3                   <<",";
  ofsP<<propAge4to10               <<",";
  ofsP<<ut                         <<",";
  ofsP<<u20;
  if(include_msy){
    //MSY based ref points
    ofsP<<","<<bmsy                <<",";
    ofsP<<p_sbt(pyr+1)/bmsy        <<",";
    ofsP<<p_sbt(pyr+1)/(0.8*bmsy)  <<",";
    ofsP<<p_sbt(pyr+1)/(0.4*bmsy)  <<",";
    ofsP<<fmsy                     <<",";
    ofsP<<p_ft(pyr,1)/fmsy         <<",";
    ofsP<<(1. - mfexp(-fmsy))      <<",";
    ofsP<<(1. - mfexp(-p_ft(pyr,1)))/(1. - mfexp(-fmsy));
  }
  ofsP<<"\n";
}
