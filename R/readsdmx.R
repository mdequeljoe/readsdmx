
#' @export
read_sdmx <- function(path) {
 read_sdmx_(path)
}


download_sdmx <- function(url,
                          file = tempfile(pattern = "SDMX_", fileext = ".xml"),
                          quiet = TRUE,
                          mode = "wb",
                          handle = curl::new_handle()) {
  curl::curl_download(url,
                      file,
                      quiet = quiet,
                      mode = mode,
                      handle = handle)

  invisible(file)
}
