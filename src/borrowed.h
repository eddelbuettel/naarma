
#include <Rcpp/Lighter>

#include <nanoarrow/r.h>
#include "nanoarrow/nanoarrow.hpp"

void array_xptr_set_schema(SEXP array_xptr, SEXP schema_xptr);

SEXP nanoarrow_c_array_set_schema(SEXP array_xptr, SEXP schema_xptr, SEXP validate_sexp);
SEXP nanoarrow_c_schema_init(SEXP type_id_sexp, SEXP nullable_sexp);
