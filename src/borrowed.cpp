
// these functions are variants of something alredy in nanoarrow but not currently (or,
// at least not when I first needed them) exported to they are carried over here in a
// additional file marked 'borrowed.{cpp,h}'

#include "borrowed.h"

// Attaches a schema to an array external pointer. The nanoarrow R package
// attempts to do this whenever possible to avoid misinterpreting arrays.
void array_xptr_set_schema(SEXP array_xptr, SEXP schema_xptr) {
    R_SetExternalPtrTag(array_xptr, schema_xptr);
}

// Retrieves the schema embedded in the tag. The nanoarrow R packages does this
// to 'transport two XPtr in onw'
SEXP array_xptr_get_schema(SEXP array_xptr) {
    return R_ExternalPtrTag(array_xptr);
}


// nanoarrow_c_array_set_schema from array.c
SEXP nanoarrow_c_array_set_schema(SEXP array_xptr, SEXP schema_xptr, SEXP validate_sexp) {
    // Fair game to remove a schema from a pointer
    if (schema_xptr == R_NilValue) {
        array_xptr_set_schema(array_xptr, R_NilValue);
        return R_NilValue;
    }

    int validate = LOGICAL(validate_sexp)[0];
    if (validate) {
        // If adding a schema, validate the schema and the pair
        struct ArrowArray* array = nanoarrow_array_from_xptr(array_xptr);
        struct ArrowSchema* schema = nanoarrow_schema_from_xptr(schema_xptr);

        struct ArrowArrayView array_view;
        struct ArrowError error;
        int result = ArrowArrayViewInitFromSchema(&array_view, schema, &error);
        if (result != NANOARROW_OK) {
            ArrowArrayViewReset(&array_view);
            Rf_error("%s", ArrowErrorMessage(&error));
        }

        result = ArrowArrayViewSetArray(&array_view, array, &error);
        ArrowArrayViewReset(&array_view);
        if (result != NANOARROW_OK) {
            Rf_error("%s", ArrowErrorMessage(&error));
        }
    }
    array_xptr_set_schema(array_xptr, schema_xptr);
    return R_NilValue;
}

// from schema.c
SEXP nanoarrow_c_schema_init(SEXP type_id_sexp, SEXP nullable_sexp) {
    int type_id = INTEGER(type_id_sexp)[0];
    SEXP schema_xptr = PROTECT(nanoarrow_schema_owning_xptr());

    struct ArrowSchema* schema = (struct ArrowSchema*)R_ExternalPtrAddr(schema_xptr);
    int result = ArrowSchemaInitFromType(schema, (ArrowType)type_id);
    if (result != NANOARROW_OK) {
        Rf_error("ArrowSchemaInitFromType() failed");
    }

    result = ArrowSchemaSetName(schema, "");
    if (result != NANOARROW_OK) {
        Rf_error("ArrowSchemaSetName() failed");
    }

    if (!LOGICAL(nullable_sexp)[0]) {
        schema->flags &= ~ARROW_FLAG_NULLABLE;
    }

    UNPROTECT(1);
    return schema_xptr;
}
