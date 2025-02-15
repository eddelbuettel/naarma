% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{collectMatrixFromStream}
\alias{collectMatrixFromStream}
\title{Given a (nano)arrow stream object pointer, return an arma matrix}
\usage{
collectMatrixFromStream(obj, verbose = FALSE)
}
\arguments{
\item{obj}{A nanoarrow array stream object via an external pointer}

\item{verbose}{A logical value, default is false}
}
\description{
Given a (nano)arrow stream object pointer, return an arma matrix
}
\section{Limitations}{

As this aims at \pkg{armadillo} operation, the returned matrix is always double while
supported many possible column types.
}

