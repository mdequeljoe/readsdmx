#include <Rcpp.h>
#include <readsdmx.h>
using namespace Rcpp;

// [[Rcpp::export]]
List read_sdmx_(const char* path){
  ReadSDMX sdmx_file(path);
  return sdmx_file.read_sdmx();
}

