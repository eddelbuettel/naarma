
library(naarma)
library(nanoarrow)

values <- 4:7

expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_int16())), values)
expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_uint16())), values)
expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_int32())), values)
expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_uint32())), values)
expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_int64())), values)
expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_uint64())), values)
expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_float())), values)
expect_equal(armaVectorExample(as_nanoarrow_array(values, schema=na_double())), values)
