
#include <nanoarrow/nanoarrow.hpp>

#include "utilities.h"
#include "borrowed.h"

template <typename T>
arma::Col<T> na_array_to_arma_vec(const struct ArrowArray* arr) {
    // the final buffer has the values, there may be a validity buffer preceding it
    // todo: flag (optional) validity values
    return arma::Col<T>((T*) arr->buffers[arr->n_buffers - 1], arr->length, true, true);
}

auto json_to_na(const std::string& json, enum ArrowType tp) {
    nanoarrow::UniqueArray arr;
    nanoarrow::UniqueSchema sch;
    struct ArrowError ec;
    if (NANOARROW_OK != ArrowSchemaInitFromType(sch.get(), tp))
        error_exit("Bad init");
    if (NANOARROW_OK != ArrowArrayInitFromSchema(arr.get(), sch.get(), &ec))
        error_exit(ec.message);

    read_vector_from_json(json, arr.get(), sch.get());

    return std::pair(std::move(arr), std::move(sch));
}

arma_vector_variant na_to_arma(const struct ArrowArray* arr, const struct ArrowSchema* sch) {
   if (sch->format == std::string_view("s")) {
        return na_array_to_arma_vec<int16_t>(arr);
    } else if (sch->format == std::string_view("S")) {
        return na_array_to_arma_vec<uint16_t>(arr);
    } else if (sch->format == std::string_view("i")) {
        return na_array_to_arma_vec<int32_t>(arr);
    } else if (sch->format == std::string_view("I")) {
        return na_array_to_arma_vec<uint32_t>(arr);
    } else if (sch->format == std::string_view("l")) {
        return na_array_to_arma_vec<int64_t>(arr);
    } else if (sch->format == std::string_view("L")) {
        return na_array_to_arma_vec<uint64_t>(arr);
    } else if (sch->format == std::string_view("f")) {
        return na_array_to_arma_vec<float>(arr);
    } else if (sch->format == std::string_view("g")) {
        return na_array_to_arma_vec<double>(arr);
    } else {
        error_exit(std::string("format '") + std::string(sch->format) + std::string("' not supported"));
        return na_array_to_arma_vec<int64_t>(arr); // not reached, but satisfies compiler ...
    }
}

arma_vector_variant json_to_arma_vec(const std::string& json, enum ArrowType tp, bool verbose = false) {
    auto pp = json_to_na(json, tp);
    auto arr = pp.first.get();
    auto sch = pp.second.get();

    if (verbose) {
        show_array(arr);
        show_schema(sch);
        display_as_json(arr, sch);
    }

    return na_to_arma(arr, sch);
}

// example 'variant to double' mapper
double reduce_arma_vec(arma_vector_variant v) {
    auto visitor = [](auto&& arg) -> double {
        return arma::as_scalar(arma::sum(arg));
    };
    return std::visit(visitor, v);
}

void show_arma_vec(arma_vector_variant v) {
    auto visitor = [](auto&& arg) {
        using T = std::decay_t<decltype(arg)>; // Get the underlying type of 'arg'

        if constexpr (std::is_same_v<T, arma::Col<int16_t>>) {
            arg.print("int16_t");
        } else if constexpr (std::is_same_v<T, arma::Col<uint16_t>>) {
            arg.print("uint16_t");
        } else if constexpr (std::is_same_v<T, arma::Col<int32_t>>) {
            arg.print("int32_t");
        } else if constexpr (std::is_same_v<T, arma::Col<uint32_t>>) {
            arg.print("uint32_t");
        } else if constexpr (std::is_same_v<T, arma::Col<int64_t>>) {
            arg.print("int64_t");
        } else if constexpr (std::is_same_v<T, arma::Col<uint64_t>>) {
            arg.print("uint64_t");
        } else if constexpr (std::is_same_v<T, arma::Col<float>>) {
            arg.print("float");
        } else if constexpr (std::is_same_v<T, arma::Col<double>>) {
            arg.print("double");
        }
    };
    std::visit(visitor, v);
    Rcpp::Rcout << "Sum is " << reduce_arma_vec(v) << std::endl;
}

enum ArrowType na_format_to_enum(const std::string& txt) {
    if (txt == std::string_view("s")) {
        return NANOARROW_TYPE_INT16;
    } else if (txt == std::string_view("S")) {
        return NANOARROW_TYPE_UINT16;
    } else if (txt == std::string_view("i")) {
        return NANOARROW_TYPE_INT32;
    } else if (txt == std::string_view("I")) {
        return NANOARROW_TYPE_UINT32;
    } else if (txt == std::string_view("l")) {
        return NANOARROW_TYPE_INT64;
    } else if (txt == std::string_view("L")) {
        return NANOARROW_TYPE_UINT64;
    } else if (txt == std::string_view("f")) {
        return NANOARROW_TYPE_FLOAT;
    } else if (txt == std::string_view("g")) {
        return NANOARROW_TYPE_DOUBLE;
    } else if (txt.substr(0, 3) == std::string_view("+w:")) {
        return NANOARROW_TYPE_FIXED_SIZE_LIST;
    } else {
        error_exit(std::string("format '") + txt + std::string("' not supported"));
        return NANOARROW_TYPE_UNINITIALIZED; // not reached, but satisfies compiler ...
    }
}
