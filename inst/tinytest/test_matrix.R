
library(naarma)
library(nanoarrow)

values <- 1:9
expected_mat <- matrix(values, 3, 3, byrow=FALSE)

expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_int16()), 3), expected_mat)
expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_uint16()), 3), expected_mat)
expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_int32()), 3), expected_mat)
expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_uint32()), 3), expected_mat)
expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_int64()), 3), expected_mat)
expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_uint64()), 3), expected_mat)
expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_float()), 3), expected_mat)
expect_equal(armaMatrixExample(as_nanoarrow_array(values, schema=na_double()), 3), expected_mat)
