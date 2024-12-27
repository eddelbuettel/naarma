
// only this file has R interfaces

#if !defined(ARMA_64BIT_WORD)
#define ARMA_64BIT_WORD 1       		// even though do not use the package interface for ints
										// note we also need to define it in Makevars for all files
#endif

#include <RcppArmadillo/Lighter>

#include <nanoarrow/r.h>

#include "utilities.h"
#include "borrowed.h"

template <typename T>
arma::Col<T> na_array_to_arma_vec(const struct ArrowArray* arr) {
    // the final buffer has the values, there may be a validity buffer preceding it
    // todo: flag (optional) validity values
    return arma::Col<T>((T*) arr->buffers[arr->n_buffers - 1], arr->length, true, true);
}

arma::Col<double> na_pointers_to_arma_vec(struct ArrowArray* arr, struct ArrowSchema* sch) {
    arma::Col<double> v(arr->length);

    if (sch->format == std::string_view("g")) {
        v = na_array_to_arma_vec<double>(arr);
    } else if (sch->format == std::string_view("s")) {
        auto w = na_array_to_arma_vec<int16_t>(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("S")) {
        auto w = na_array_to_arma_vec<uint16_t>(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("i")) {
        auto w = na_array_to_arma_vec<int32_t>(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("I")) {
        auto w = na_array_to_arma_vec<uint32_t>(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("l")) {
        auto w = na_array_to_arma_vec<int64_t>(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("L")) {
        auto w = na_array_to_arma_vec<uint64_t>(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("f")) {
        auto w = na_array_to_arma_vec<float>(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("u") || // turn string vectors into factor ints
               sch->format == std::string_view("U")) {
        auto w = na_array_strings_to_arma_vec(arr);
        v = arma::conv_to<arma::Col<double>>::from(w);
    } else if (sch->format == std::string_view("tsu:UTC")) {   // this is but many timedata formats...
        auto w = na_array_to_arma_vec<int64_t>(arr);           // we need different scale factors
        v = arma::conv_to<arma::Col<double>>::from(w) * 1e-6;
    } else {
        error_exit(std::string("format '") + std::string(sch->format) + std::string("' not supported"));
    }
    return v;
}

// ' Create Arma Vector from Nanoarrow Vector Example
// '
//' Given a (nano)arrow array external pointer object via two pointers (with the second tucked
//' away as the tag), return an arma vector
//'
//' @section Limitations:
//' As this aims at \pkg{armadillo} interoperation, this functionality is limited to numeric
//' vector columns. The framework used here could of course be extended to other Arrow formats.
//'
//' @param vec A nanoarrow array object as an external pointer
//' @param verbose A logical value, default is false
//' @examples
//' sv <- nanoarrow::as_nanoarrow_array(c(4:7), nanoarrow::na_int16())  # internally int16
//' armaVectorExample(sv)
// [[Rcpp::export]]
SEXP armaVectorExample(Rcpp::RObject vec, bool verbose = false) {
    // We get an R-created 'nanoarrow_array', an S3 class around an external pointer
    if (!Rf_inherits(vec, "nanoarrow_array"))
        Rcpp::stop("Expected class 'nanoarrow_array' not found");

    // It is a straight up external pointer so we can use R_ExternalPtrAddr()
    struct ArrowArray* arr = (struct ArrowArray*)R_ExternalPtrAddr(vec);
    // As it is a nanoarrow thing, we can also get the schema from the array _tag_ xptr
    struct ArrowSchema* sch = (struct ArrowSchema*)R_ExternalPtrAddr(R_ExternalPtrTag(vec));

    if (verbose) {
        show_array(arr);
        show_schema(sch);
    }

    auto v = na_pointers_to_arma_vec(arr, sch);

    if (verbose)
        v.print("v");

    //auto sztvec = arma::conv_to<arma::Col<size_t>>::from(v);
    //return Rcpp::wrap(sztvec);
    return Rcpp::wrap(v);
}

// ' Create Arma Matrix from Nanoarrow Vector Example
// '
//' Given a (nano)arrow object via two pointers, and a column size, return an arma matrix
//'
//' @section Limitations:
//' As this aims at \pkg{armadillo} operations, this functionality is limited to numeric
//' vector columns. The framework used here could of course be extended to other Arrow formats.
//'
//' @param vec A nanoarrow object
//' @param ncol Number of columns in returned matrix
//' @param verbose A logical value, default is false
//' @examples
//' sv <- nanoarrow::as_nanoarrow_array(c(4:8), nanoarrow::na_int16())  # internally int16
//' armaMatrixExample(sv, 2)
// [[Rcpp::export]]
SEXP armaMatrixExample(Rcpp::RObject vec, int ncol, bool verbose = false) {
    // We get an R-created 'nanoarrow_array', an S3 class around an external pointer
    if (!Rf_inherits(vec, "nanoarrow_array"))
        Rcpp::stop("Expected class 'nanoarrow_array' not found");

    // It is a straight up external pointer so we can use R_ExternalPtrAddr()
    struct ArrowArray* arr = (struct ArrowArray*)R_ExternalPtrAddr(vec);
    // As it is a nanoarrow thing, we can also get the schema from the array _tag_ xptr
    struct ArrowSchema* sch = (struct ArrowSchema*)R_ExternalPtrAddr(R_ExternalPtrTag(vec));

    if (verbose) {
        show_array(arr);
        show_schema(sch);
    }

    auto v = na_pointers_to_arma_vec(arr, sch);

    if (verbose)
        v.print("v");

    auto n = arma::mat(v).reshape(v.n_elem/ncol, ncol);
    return Rcpp::wrap(n);
}

// ' Create Arma Matrix from Nanoarrow Array Stream
// '
//' Given a (nano)arrow stream object pointer, return an arma matrix
//'
//' @section Limitations:
//' As this aims at \pkg{armadillo} operation, the returned matrix is always double while
//' supported many possible column types.
//'
//' @param obj A nanoarrow array stream object via an external pointer
//' @param verbose A logical value, default is false
// [[Rcpp::export]]
arma::mat collectMatrixFromStream(Rcpp::RObject obj, bool verbose = false) {

    if (!Rf_inherits(obj, "nanoarrow_array_stream"))
        Rcpp::stop("Expected class 'nanoarrow_array_stream' not found");

    nanoarrow::UniqueArrayStream aas{(struct ArrowArrayStream*)R_ExternalPtrAddr(obj)};
    nanoarrow::UniqueSchema sch;
    int cnt = 0;
    struct ArrowError ec;
    exitIfError(ArrowArrayStreamGetSchema(aas.get(), sch.get(), &ec), "Bad schema");
    if (verbose) show_schema(sch.get());
    std::vector<nanoarrow::UniqueArray> vec;
    while (true) {
        nanoarrow::UniqueArray arr;
        exitIfError(ArrowArrayStreamGetNext(aas.get(), arr.get(), &ec), "Bad array");
        if (arr->length == 0)
            break;
        if (verbose) show_array(arr.get());
        cnt += arr->length;
        vec.emplace_back(std::move(arr));
        if (verbose) Rcpp::Rcout << "Count now " << cnt << std::endl;
    }
    if (verbose) Rcpp::Rcout << "Final Count is " << cnt << " "
                             << "Cols is " << sch->n_children << std::endl;

    // TODO: assert no missing values
    int n = cnt,                								// rows in the matrix we create
        k = sch->n_children,                					// columns in the matrix we create
        chunks = vec.size(),                					// number of arrow chunks used
        currpos = 0;                        					// current row
    if (verbose)
        for (auto i = 0; i < k; i++)
            Rcpp::Rcout << "Col " << i << " is " << std::string(sch->children[i]->format) << std::endl;

    arma::mat m(n, k);                      					// new matrix
    for (auto i = 0; i < chunks; i++) {                         // over all chunkss
        nanoarrow::UniqueArray arr = std::move(vec[i]);         // access array
        for (auto j = 0; j < k; j++) {                          // over all columns
            if (verbose)
                Rcpp::Rcout << "Chunk " << i << " "
                            << "currpos " << currpos << " "
                            << "col " << j << std::endl;
            arma::vec v = na_pointers_to_arma_vec(arr->children[j],
                                                  sch->children[j]);
            m.submat(currpos, j, currpos + arr->length - 1, j) = v;
        }
        currpos += arr->length; 								// account for
    }
    if (verbose) m.print("m");
    return m;
}
