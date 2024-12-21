
library(adbcdrivermanager)
library(duckdb)
db <- adbc_database_init(duckdb_adbc())
con <- adbc_connection_init(db)
execute_adbc(con, "LOAD httpfs")
execute_adbc(con, "CREATE TABLE covertypeData AS SELECT * FROM 'https://www.mlpack.org/datasets/covertype-small.data.csv.gz';")
execute_adbc(con, "CREATE TABLE covertypeLabels AS SELECT * FROM 'https://www.mlpack.org/datasets/covertype-small.labels.csv.gz';")
nadata <- read_adbc(con, "SELECT * from covertypeData")  # return a array stream object
D <- nanoarrow::convert_array_stream(nadata)  # now D is a data.frame
nadata <- read_adbc(con, "SELECT * from covertypeLabels")  # return a array stream object
L <- nanoarrow::convert_array_stream(nadata)  # now L is a one-column data.frame
