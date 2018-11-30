#include <Rcpp.h>
#include <readsdmx.h>
using namespace Rcpp;

// [[Rcpp::export]]
List read_sdmx_(SEXP path){
  if (Rf_isVectorList(path)){
    std::string pathdata = read_binlist(path);
    ReadSDMX sdmx_file(pathdata);
    return sdmx_file.read_sdmx();
  }
  //std::string p = as<std::string>(path);
  //const char* path_str = p.c_str();
  const char* path_str = as<std::string>(path).c_str();
  ReadSDMX sdmx_file(path_str);
  return sdmx_file.read_sdmx();
}




