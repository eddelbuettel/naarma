
suppressMessages({
    library(polars)
    library(arrow)
})

## we have to cheat a little as the polars R package cannot (yet ?) read from
## compresssed remote cvs files -- the python one can so the gap will likely be closed
remote_to_local_temp <- function(url) {
    setwd(tempdir())
    tgzfile <- tempfile(fileext=".csv.gz")
    download.file(url, tgzfile, quiet=TRUE)
    tfile <- gsub(".gz$", "", tgzfile)
    system(paste("gunzip", basename(tgzfile)))
    tfile
}

url <- "https://www.mlpack.org/datasets/covertype-small.labels.csv.gz"
plL <- pl$read_csv(remote_to_local_temp(url), has_header=FALSE)
url <- "https://www.mlpack.org/datasets/covertype-small.data.csv.gz"
plD <- pl$read_csv(remote_to_local_temp(url), has_header=FALSE)

arrL <- nanoarrow::as_nanoarrow_array_stream(as_arrow_table(plL))
arrD <- nanoarrow::as_nanoarrow_array_stream(as_arrow_table(plD))

L <- naarma::collectMatrixFromStream(arrL)
D <- naarma::collectMatrixFromStream(arrD)

trainInd <- 1:70000
testInd <- 70001:100000
fit <- mlpack::random_forest(training = D[trainInd,],
                             labels = L[trainInd, 1, drop=FALSE],
                             num_trees = 10,
                             minimum_leaf_size = 3,
                             print_training_accuracy = TRUE,
                             test = D[testInd,],
                             test_labels = L[testInd, 1, drop=FALSE],
                             verbose=TRUE)
