#include <Rcpp.h>
#include <SDMXReader.h>

// [[Rcpp::export]]
std::map<std::string, Rcpp::CharacterVector> read_sdmx_(std::string fname)
{
  SDMXReader d(fname);
  return d.data();
}

// [[Rcpp::export]]
std::map<std::string, Rcpp::CharacterVector> read_sdmx_connection_(List bdata)
{
  SDMXReader d(bdata);
  return d.data();
}
