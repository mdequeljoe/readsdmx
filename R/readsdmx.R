
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
read_sdmx <- function(path) {

  if (grepl("^http[s]?://", path)){
    path <- stream_con(path)
    return(read_sdmx_(path))
  }

  stopifnot(file.exists(path))
  read_sdmx_(path, TRUE)
}

stream_con <- function(con) {

  if (!is(con, "connection"))
    con <- url(con)
  open(con, "rb")
  on.exit(close(con))
  out <- new.env()
  n <- 1

  repeat {
    txt <- readLines(con,
                     n = 1000,
                     encoding = "UTF-8",
                     warn = FALSE)
    if (!length(txt))
      break
    out[[as.character(n)]] <- txt
    n <- n + 1
  }
  do.call(paste0, as.list(out))
}
