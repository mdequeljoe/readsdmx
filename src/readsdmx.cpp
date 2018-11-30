#include <Rcpp.h>
#include <readsdmx.h>
using namespace Rcpp;

// [[Rcpp::export]]
List read_sdmx_(std::string path, bool is_file = false){
  if (is_file){
    const char* path_str = strdup(path.c_str());
    ReadSDMX sdmx_file(path_str);
    return sdmx_file.read_sdmx();
  }
  ReadSDMX sdmx_file(path);
  return sdmx_file.read_sdmx();
}

