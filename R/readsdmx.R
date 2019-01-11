#' @useDynLib readsdmx, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL

#' Read SDMX data
#'
#' read SDMX data into R as dataframes
#'
#' @param path Character; the path to an SDMX-ML xml file
#' @inheritParams utils::download.file
#' @return data.frame object
#' @details read_sdmx returns a dataframe with the SDMX data 'as-is' which means
#'   that all columns will be character vectors. The base R function `download.file` is used when
#'   a url is provided to read_sdmx.
#'
#' @examples
#' \dontrun{
#' u <-
#'  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/..Q/all?format=compact_v2"
#' d <- readsdmx::read_sdmx(u)
#'
#' f <- system.file("extdata/compact_2.0.xml", package = "readsdmx")
#' d <- readsdmx::read_sdmx(f)
#' }
#' @importFrom utils download.file
#' @export
read_sdmx <-
  function(path,
           destfile = tempfile(fileext = ".xml"),
           quiet = TRUE,
           method = "libcurl",
           mode = "w",
           ...) {
    if (is_url(path)) {
      if (!capabilities("libcurl") && method == "libcurl")
        method <- "auto"
      df <- download.file(path, destfile, method, quiet, mode, ...)
      stopifnot(df == 0L)
      path <- destfile
    }
    stopifnot(file.exists(path))
    path <- normalizePath(path)
    d <- read_sdmx_(path)
    as.data.frame(d, stringsAsFactors = FALSE)
  }

is_url <- function(d)
  grepl("^http[s]?://", d)
