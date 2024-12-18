// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#if !defined(ARMA_64BIT_WORD)
//#define ARMA_32BIT_WORD 1       		// needed if we want int32_t instead of int64_t
#define ARMA_64BIT_WORD 1       		// already default on amd64, but making it explicit
#endif

#include <armadillo>
#include <nanoarrow/nanoarrow.hpp> 		// overall interface
#include <nanoarrow/nanoarrow_testing.hpp>	// for from/to JSON utilities

// declaration
enum ArrowType na_format_to_enum(const std::string& txt);

inline void show_array(const struct ArrowArray* arr) {
    std::cout << "Array" << std::endl
              << "  Length: " << arr->length << std::endl
              << "  Null_count: " << arr->null_count << std::endl
              << "  Offset: " << arr->offset << std::endl
              << "  N_Buffers: " << arr->n_buffers << std::endl
              << "  N_Children: " << arr->n_children << std::endl
              << "  Dictionary: " << (arr->dictionary == nullptr ? "no" : "yes") << std::endl;
}

inline void show_schema(const struct ArrowSchema* sch) {
    std::cout << "Schema" << std::endl
              << "  Format: " << sch->format << std::endl
              << "  Name: " << (sch->name != nullptr ? sch->name : "") << std::endl
              << "  Flags: " << sch->flags << std::endl
              << "  N_Children: " << sch->n_children << std::endl;
}

inline void error_exit(const std::string& txt) {
    // in a larger application we might throw, here we just report and exit
    std::cerr << "ERROR: " << txt << std::endl;
    exit(-1);
}

inline void exitIfError(const ArrowErrorCode ec, const std::string& msg) {
    if (ec != NANOARROW_OK) error_exit(msg);
}

inline void read_vector_from_json(const std::string& txt, struct ArrowArray* arr, struct ArrowSchema* sch) {
    ArrowError error;
    error.message[0] = '\0';
    nanoarrow::testing::TestingJSONReader reader;
    auto rc = reader.ReadColumn(txt, sch, arr, &error);
    if (rc != NANOARROW_OK)
        error_exit(error.message);
}

inline void display_as_json(const struct ArrowArray* arr, const struct ArrowSchema* sch) {
    nanoarrow::UniqueArrayView array_view;
    struct ArrowError ec;
    if (NANOARROW_OK != ArrowArrayViewInitFromSchema(array_view.get(), sch, &ec))
        error_exit(ec.message);
    if (NANOARROW_OK != ArrowArrayViewSetArray(array_view.get(), arr, &ec))
        error_exit(ec.message);

    nanoarrow::testing::TestingJSONWriter jsonwriter;
    std::stringstream str_out;

    str_out << "COLUMN: ";
    jsonwriter.WriteColumn(str_out, sch, array_view.get());
    str_out << std::endl;

    std::cout << str_out.str();
}
