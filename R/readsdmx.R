#' Read SDMX data
#'
#' read SDMX data into R as dataframes
#'
#' @param path Character; the path to an SDMX-ML xml file or an url to connect
#'   to.
#' @param n Integer; if reading data from a connection pass n number of records
#'   to be read via readBin.
#' @param verbose Logical; When connecting to large datasets verbose will
#'   display the progress of the records read as expressed by its object size.
#' @return data.frame object
#'
#' @details read_sdmx returns a dataframe with the SDMX data 'as-is' which means
#'   that all columns will be character vectors.
#'
#' @examples \dontrun{
#' u <-
#'  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/..Q/all?format=compact_v2"
#' d <- readsdmx::read_sdmx(u)
#'
#' f <- system.file("extdata/compact_2.0.xml", package = "readsdmx")
#' d <- readsdmx::read_sdmx(f)
#' }
#' @export
read_sdmx <- function(path,
                      n = 64 * 1024,
                      verbose = FALSE) {
  if (is_url(path)) {
    d <- read_sdmx_connection(path, n, verbose)
    return(as.data.frame(d, stringsAsFactors = FALSE))
  }
  stopifnot(file.exists(path))
  path <- normalizePath(path)
  d <- read_sdmx_(path)
  as.data.frame(d, stringsAsFactors = FALSE)
}

read_sdmx_connection <- function(con, n, verbose) {
  con <- stream_con(con, n, verbose)
  stopifnot(!!length(con))
  read_sdmx_connection_(con)
}

#' @importFrom utils object.size
stream_con <- function(con,
                       n = 64 * 1024,
                       verbose = FALSE) {
  if (!inherits(con, "connection"))
    con <- url(con)
  open(con, "rb")
  on.exit(close(con))
  out <- list()
  n <- 1
  b <- 0
  repeat {
    txt <- readBin(con, "raw", n = n)
    if (!length(txt))
      break
    if (verbose) {
      b <- b + object.size(txt)
      cat("\rread from connection:", round(b / 1e6, 2), "MB")
    }
    out[[n]] <- txt
    n <- n + 1
  }
  out
}

is_url <- function(d) grepl("^http[s]?://", d)
