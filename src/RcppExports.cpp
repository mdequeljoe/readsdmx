// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// read_sdmx_
std::map<std::string, Rcpp::CharacterVector> read_sdmx_(std::string fname);
RcppExport SEXP _readsdmx_read_sdmx_(SEXP fnameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type fname(fnameSEXP);
    rcpp_result_gen = Rcpp::wrap(read_sdmx_(fname));
    return rcpp_result_gen;
END_RCPP
}
// read_sdmx_connection_
std::map<std::string, Rcpp::CharacterVector> read_sdmx_connection_(List bdata);
RcppExport SEXP _readsdmx_read_sdmx_connection_(SEXP bdataSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< List >::type bdata(bdataSEXP);
    rcpp_result_gen = Rcpp::wrap(read_sdmx_connection_(bdata));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_readsdmx_read_sdmx_", (DL_FUNC) &_readsdmx_read_sdmx_, 1},
    {"_readsdmx_read_sdmx_connection_", (DL_FUNC) &_readsdmx_read_sdmx_connection_, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_readsdmx(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
