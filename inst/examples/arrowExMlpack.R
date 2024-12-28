
suppressMessages({
    library(arrow)
})

url <- "https://www.mlpack.org/datasets/covertype-small.labels.csv.gz"
arrL <- read_csv_arrow(url, col_names = FALSE, as_data_frame = FALSE)
url <- "https://www.mlpack.org/datasets/covertype-small.data.csv.gz"
arrD <- read_csv_arrow(url, col_names = FALSE, as_data_frame = FALSE)

L <- naarma::collectMatrixFromStream(nanoarrow::as_nanoarrow_array_stream(arrL))
D <- naarma::collectMatrixFromStream(nanoarrow::as_nanoarrow_array_stream(arrD))

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
