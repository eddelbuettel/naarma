// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// vectorExample
Rcpp::RObject vectorExample(std::string jsontxt, std::string format);
RcppExport SEXP _naarma_vectorExample(SEXP jsontxtSEXP, SEXP formatSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type jsontxt(jsontxtSEXP);
    Rcpp::traits::input_parameter< std::string >::type format(formatSEXP);
    rcpp_result_gen = Rcpp::wrap(vectorExample(jsontxt, format));
    return rcpp_result_gen;
END_RCPP
}
// structExample
Rcpp::RObject structExample(std::vector<std::string> jsontxt, std::vector<std::string> format);
RcppExport SEXP _naarma_structExample(SEXP jsontxtSEXP, SEXP formatSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::vector<std::string> >::type jsontxt(jsontxtSEXP);
    Rcpp::traits::input_parameter< std::vector<std::string> >::type format(formatSEXP);
    rcpp_result_gen = Rcpp::wrap(structExample(jsontxt, format));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_naarma_vectorExample", (DL_FUNC) &_naarma_vectorExample, 2},
    {"_naarma_structExample", (DL_FUNC) &_naarma_structExample, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_naarma(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
