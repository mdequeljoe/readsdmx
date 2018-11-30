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
#' @export
read_sdmx <- function(path,
                      n = 64 * 1024,
                      verbose = FALSE) {
  if (grepl("^http[s]?://", path)) {
    path <- stream_con(path, n, verbose)
    return(read_sdmx_(path))
  }

  stopifnot(file.exists(path))
  read_sdmx_(path)
}

stream_con <- function(con,
                       n = 64 * 1024,
                       verbose = FALSE) {
  if (!is(con, "connection"))
    con <- url(con)
  open(con, "rb")
  on.exit(close(con))
  out <- list()
  n <- 1
  b <- 0
  class(b) <- "object_size"

  repeat {
    txt <- readBin(con, "raw", n = n)
    if (!length(txt))
      break
    if (verbose) {
      b <- b + object.size(txt)
      cat("\r read from connection: ",
          format(b, unit = "Mb", standard = "SI"))
    }
    out[[n]] <- txt
    n <- n + 1
  }
  out
}
