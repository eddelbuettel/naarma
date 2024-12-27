
library(naarma)
library(nanoarrow)

as <- basic_array_stream(list(data.frame(x=1:5, y=seq(10,6,by=-1)),
                              data.frame(x=6:10, y=seq(5,1,by=-1))))

mat <- collectMatrixFromStream(as)
expect_equal(dim(mat), c(10,2))
expect_equal(mat[,1], 1:10)
expect_equal(mat[,2], seq(10,1,by=-1))

## stream now empty, subsequent attempts error
expect_error(collectMatrixFromStream(as))
