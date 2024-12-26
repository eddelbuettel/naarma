// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#if !defined(ARMA_64BIT_WORD)
//#define ARMA_32BIT_WORD 1       		// needed if we want int32_t instead of int64_t
#define ARMA_64BIT_WORD 1       		// already default on amd64, but making it explicit
#endif

#include <variant>
#include <nanoarrow/nanoarrow.hpp> 			// overall interface
#include <RcppArmadillo/Lightest>			// also for Rcout/Rcerr, stop

typedef std::variant<arma::Col<int16_t>,
                     arma::Col<uint16_t>,
                     arma::Col<int32_t>,
                     arma::Col<uint32_t>,
                     arma::Col<int64_t>,
                     arma::Col<uint64_t>,
                     arma::Col<float>,
                     arma::Col<double>> arma_vector_variant;

// declaration
enum ArrowType na_format_to_enum(const std::string& txt);
arma_vector_variant na_to_arma(const struct ArrowArray* arr, const struct ArrowSchema* sch);
template <typename T> arma::Col<T> na_array_to_arma_vec(const struct ArrowArray* arr);

inline void show_array(const struct ArrowArray* arr) {
    Rcpp::Rcout << "Array" << std::endl
                << "  Length: " << arr->length << std::endl
                << "  Null_count: " << arr->null_count << std::endl
                << "  Offset: " << arr->offset << std::endl
                << "  N_Buffers: " << arr->n_buffers << std::endl
                << "  N_Children: " << arr->n_children << std::endl
                << "  Dictionary: " << (arr->dictionary == nullptr ? "no" : "yes") << std::endl;
}

inline void show_schema(const struct ArrowSchema* sch) {
    Rcpp::Rcout << "Schema" << std::endl
                << "  Format: " << sch->format << std::endl
                << "  Name: " << (sch->name != nullptr ? sch->name : "") << std::endl
                << "  Flags: " << sch->flags << std::endl
                << "  N_Children: " << sch->n_children << std::endl;
}

inline void error_exit(const std::string& txt) {
    // in a larger application we might throw, here we just report and exit
    //std::cerr << "ERROR: " << txt << std::endl;
    //exit(-1);
    Rcpp::stop(txt);
}

inline void exitIfError(const ArrowErrorCode ec, const std::string& msg) {
    if (ec != NANOARROW_OK) error_exit(msg);
}
