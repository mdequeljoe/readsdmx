
#' Read SDMX data
#'
#' read SDMX data into R as dataframes
#'
#' @param path the path to an SDMX-ML xml file or an url to connect to.
#' @return data.frame object
#'
#' @details read_sdmx returns a dataframe with the SDMX data 'as-is'
#' which means that all columns will be character vectors.
#'
#' @export
read_sdmx <- function(path, verbose = FALSE) {

  if (grepl("^http[s]?://", path)){
    path <- stream_con(path, verbose)
    return(read_sdmx_(path))
  }

  stopifnot(file.exists(path))
  read_sdmx_(path)
}

stream_con <- function(con, verbose = FALSE) {

  if (!is(con, "connection"))
    con <- url(con)
  open(con, "rb")
  on.exit(close(con))
  out <- list()
  n <- 1

  repeat {
    if (verbose)
      cat("\r reading chunk: ", n)
    txt <- readBin(con, "raw", n = 64 * 1024)
    if (!length(txt))
      break
    out[[n]] <- txt
    n <- n + 1
  }
  out
}

