
## naarma: nanoarrow to armadillo integration

[![CI](https://github.com/eddelbuettel/naarma/workflows/ci/badge.svg)](https://github.com/eddelbuettel/naarma/actions?query=workflow%3Aci)

### Motivation

[mlpack][mlpack] provides numerous machine learning algorithms in easy-to-use, compact, and portable
C++ code that is easy to deploy.  Getting data into [mlpack][mlpack] can be straightforward, for
example from csv files.  These days, data can live in many places and formats and many excellent
data readers are available: [duckdb][duckdb], [polars][polars], and many more are available and
generally provide [Arrow][arrow] (columnar) binary representation interfaces making the choice of
data reader less relevant as _the interface_ is what matters.

Here we examine the use of the neat [nanoarrow][nanoarrow] package and its use of the (portable,
lightweight, no need for `libarrow`) [C data inteface][cdata].  In particular, we look at growing
matrices (and vectors) from the 'streamed' representation available from (nano)arrow.

Our [first example][example1] accesses the [mlpack][mlpack] data for its introductory random forest
example _directly off the compressed files on the website_ into [duckdb][duckdb] (using standard
in-memory representation), exports to [nanoarrow][nanarrow] _array streams_ which are then converted
to [armadillo][armadillo] matrices. We note that while [mlpack][mlpack] is heavily templated, the
standard representation is still `double`. But more importantly
- no data is ever manifested on disk, the example can live 'on the edge'
- while driven from R (because that is what I like) all the data sits in Arrow types allowing a
  fuller vocabulary of types where needed (_i.e._ `uint16_t`)
- this should be easily extensible to 'Arrow over RPC'

### Installation

The code is provided as an R package so a standard installation from the repository via

```r
remotes::install_packages("naarma")
```

should work. The only dependencies to install the package are three other standard packages, namely
`Rcpp`, `RcppArmadillo`, `nanoarrow`.

### Author

Dirk Eddelbuettel

### License

GPL (>= 2)




[mlpack]: https://mlpack.org
[duckdb]: https://duckdb.org
[polars]: https://pola.rs
[arrow]: https://arrow.apache.org/
[nanoarrow]: https://github.com/apache/arrow-nanoarrow
[cdata]: https://arrow.apache.org/docs/format/CDataInterface.html
[example1]: https://github.com/eddelbuettel/naarma/blob/master/inst/examples/adbcExMlpack.R
