
// only this file has R interfaces

#if !defined(ARMA_64BIT_WORD)
#define ARMA_64BIT_WORD 1       		// even though do not use the package interface for ints
#endif

#include <RcppArmadillo/Lightest>

#include <nanoarrow/r.h>

#include "utilities.h"
#include "borrowed.h"

// ' Create Vector Example
// '
//' Given a JSON expressions and a string, return a vector
//'
//' @section Limitations:
//' As this aims at \pkg{armadillo} interoperation, thie functionality is limited to numeric
//' vector columns. The framework used here could of course be extended to other Arrow formats.
//'
//' @param jsontxt A JSON string (as understood by \pkg{nanoarrow} testing helper code)s
//' @param format A format string per the Arrow specification
//' @examples
//' vectorExample(R"({"name": "vec", "count": 3, "VALIDITY": [1, 1, 1], "DATA": ["2", "3", "4"]})", "S")
// [[Rcpp::export]]
Rcpp::RObject vectorExample(std::string jsontxt, std::string format) {
    auto tp = na_format_to_enum(format);

    auto sxpsch = nanoarrow_schema_owning_xptr();      				// allocates and wraps as XP
    auto sch = nanoarrow_output_schema_from_xptr(sxpsch);
    exitIfError(ArrowSchemaInitFromType(sch, tp), "Bad schema init");
    exitIfError(ArrowSchemaSetName(sch, ""), "Bad schema name");

    auto sxparr = nanoarrow_array_owning_xptr(); 					// allocates and wraps as XP
    auto arr = nanoarrow_output_array_from_xptr(sxparr);
    exitIfError(ArrowArrayInitFromType(arr, tp), "Bad array init");

    read_vector_from_json(jsontxt, arr, sch);

    // Nanoarrow special: stick schema into xptr tag to return single SEXP
    array_xptr_set_schema(sxparr, sxpsch); 							// embed schema in array

    return sxparr;
}

// ' Create Struct of Vectors Examples
// '
//' Given a vector of JSON expressions and a vector of format strings, return a struct
//'
//' @section Limitations:
//' As this aims at \pkg{armadillo} interoperation, thie functionality is limited to numeric
//' vector columns. The framework used here could of course be extended to other Arrow formats.
//'
//' @param jsontxt A vector of JSON strings (as understood by \pkg{nanoarrow} testing helper code)s
//' @param format A vector of format strings per the Arrow specification
//' @examples
//' structExample(rep(R"({"name": "vec", "count": 3, "VALIDITY": [1, 1, 1], "DATA": ["2", "3", "4"]})",2), c("S", "L"))
//' # also pipe above into either `tibble::as_tibble()` (converts data) or `arrow::as_record_batch()`
// [[Rcpp::export]]
Rcpp::RObject structExample(std::vector<std::string> jsontxt, std::vector<std::string> format) {
    if (jsontxt.size() != format.size()) {
        Rcpp::stop("Arguments must have equal length\n");
    }
    size_t ncol = jsontxt.size();

    auto sxpsch = nanoarrow_schema_owning_xptr();      				// allocates and wraps as XP
    auto sch = nanoarrow_output_schema_from_xptr(sxpsch);
    exitIfError(ArrowSchemaInitFromType(sch, NANOARROW_TYPE_STRUCT), "Bad schema init");
    exitIfError(ArrowSchemaSetName(sch, ""), "Bad schema name");
    exitIfError(ArrowSchemaAllocateChildren(sch, ncol), "Bad schema children alloc");

    auto sxparr = nanoarrow_array_owning_xptr(); 					// allocates and wraps as XP
    auto arr = nanoarrow_output_array_from_xptr(sxparr);
    exitIfError(ArrowArrayInitFromType(arr, NANOARROW_TYPE_STRUCT), "Bad array init");
    exitIfError(ArrowArrayAllocateChildren(arr, ncol), "Bad array children alloc");

    arr->length = 0;
    for (size_t i=0; i<ncol; i++) { // _if_ we had more columns they'd align here, for now n==1

        auto tp = na_format_to_enum(format[i]);
        nanoarrow::UniqueArray arr2;
        nanoarrow::UniqueSchema sch2;

        //auto sxpsch2 = nanoarrow_schema_owning_xptr();      				// allocates and wraps as XP
        //auto sch2 = nanoarrow_output_schema_from_xptr(sxpsch2);
        auto nm = std::string("col") + std::to_string(i + 1);
        exitIfError(ArrowSchemaInitFromType(sch2.get(), tp), "Bad schema init");
        exitIfError(ArrowSchemaSetName(sch2.get(), nm.c_str()), "Bad schema name");

        //auto sxparr2 = nanoarrow_array_owning_xptr(); 					// allocates and wraps as XP
        //auto arr2 = nanoarrow_output_array_from_xptr(sxparr2);
        exitIfError(ArrowArrayInitFromType(arr2.get(), tp), "Bad array init");

        read_vector_from_json(jsontxt[i], arr2.get(), sch2.get());

        ArrowArrayMove(arr2.get(), arr->children[i]);     // store child i array
        ArrowSchemaMove(sch2.get(), sch->children[i]);     // store child i schema

        arr->length = std::max(arr->length, arr2.get()->length);
    }

    // Nanoarrow special: stick schema into xptr tag to return single SEXP
    array_xptr_set_schema(sxparr, sxpsch); 							// embed schema in array

    return sxparr;
}
