# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#' Given a JSON expressions and a string, return a vector
#'
#' @section Limitations:
#' As this aims at \pkg{armadillo} interoperation, thie functionality is limited to numeric
#' vector columns. The framework used here could of course be extended to other Arrow formats.
#'
#' @param jsontxt A JSON string (as understood by \pkg{nanoarrow} testing helper code)s
#' @param format A format string per the Arrow specification
#' @examples
#' vectorExample(R"({"name": "vec", "count": 3, "VALIDITY": [1, 1, 1], "DATA": ["2", "3", "4"]})", "S")
vectorExample <- function(jsontxt, format) {
    .Call(`_naarma_vectorExample`, jsontxt, format)
}

#' Given a vector of JSON expressions and a vector of format strings, return a struct
#'
#' @section Limitations:
#' As this aims at \pkg{armadillo} interoperation, thie functionality is limited to numeric
#' vector columns. The framework used here could of course be extended to other Arrow formats.
#'
#' @param jsontxt A vector of JSON strings (as understood by \pkg{nanoarrow} testing helper code)s
#' @param format A vector of format strings per the Arrow specification
#' @examples
#' structExample(rep(R"({"name": "vec", "count": 3, "VALIDITY": [1, 1, 1], "DATA": ["2", "3", "4"]})",2), c("S", "L"))
#' # also pipe above into either `tibble::as_tibble()` (converts data) or `arrow::as_record_batch()`
structExample <- function(jsontxt, format) {
    .Call(`_naarma_structExample`, jsontxt, format)
}

#' Given a (nano)arrow object via two pointers, return an arma vector
#'
#' @section Limitations:
#' As this aims at \pkg{armadillo} interoperation, thie functionality is limited to numeric
#' vector columns. The framework used here could of course be extended to other Arrow formats.
#'
#' @param vec A nanoarrow object
#' @param verbose A logical value, default is false
#' @examples
#' sv <- vectorExample(R"({"name": "vec", "count": 3, "VALIDITY": [1, 1, 1], "DATA": [2, 3, 4]})", "g")
#' armaVectorExample(sv)
armaVectorExample <- function(vec, verbose = FALSE) {
    .Call(`_naarma_armaVectorExample`, vec, verbose)
}

#' Given a (nano)arrow object via two pointers, and a column size, return an arma matrix
#'
#' @section Limitations:
#' As this aims at \pkg{armadillo} interoperation, thie functionality is limited to numeric
#' vector columns. The framework used here could of course be extended to other Arrow formats.
#'
#' @param vec A nanoarrow object
#' @param verbose A logical value, default is false
#' @examples
#' sv <- vectorExample(R"({"name": "vec", "count": 3, "VALIDITY": [1, 1, 1], "DATA": [2, 3, 4]})", "g")
#' armaVectorExample(sv)
armaMatrixExample <- function(vec, ncol, verbose = FALSE) {
    .Call(`_naarma_armaMatrixExample`, vec, ncol, verbose)
}

