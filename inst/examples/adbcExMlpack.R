
## Use duckdb with the adbc driver (suppressing the automatic 'Loading ... DBI')
suppressMessages({
    library(adbcdrivermanager)
    library(duckdb)
})
db <- adbc_database_init(duckdb_adbc())
con <- adbc_connection_init(db)

## Data into duckdb
execute_adbc(con, "LOAD httpfs")
execute_adbc(con, "CREATE TABLE covertypeData AS SELECT * FROM 'https://www.mlpack.org/datasets/covertype-small.data.csv.gz';")
execute_adbc(con, "CREATE TABLE covertypeLabels AS SELECT * FROM 'https://www.mlpack.org/datasets/covertype-small.labels.csv.gz';")

## Query duckdb via adbc, get arrow streams, convert those
nadata <- read_adbc(con, "SELECT * from covertypeData")  # return a array stream object
#D <- nanoarrow::convert_array_stream(nadata)  	# now D is a data.frame
D <- naarma::collectFromStream(nadata)

nalabels <- read_adbc(con, "SELECT * from covertypeLabels")  # return a array stream object
#L <- nanoarrow::convert_array_stream(nalabels)  # now L is a one-column data.frame
L <- naarma::collectFromStream(nalabels)


trainInd <- 1:70000
testInd <- 70001:100000
fit <- mlpack::random_forest(training = D[trainInd,],
                             labels = L[trainInd, 1, drop=FALSE],
                             num_trees = 10, minimum_leaf_size = 3,
                             print_training_accuracy = TRUE,
                             test = D[testInd,],
                             test_labels = L[testInd, 1, drop=FALSE],
                             verbose=TRUE)
