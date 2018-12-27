context("compactdata")

u <- readRDS(system.file("extdata/compact_con.RDS", package = "readsdmx"))
k <- as.data.frame(read_sdmx_connection_(u), stringsAsFactors = FALSE)
f <- system.file("extdata/compact_2.0.xml", package = "readsdmx")
h <- read_sdmx(f)

test_that("connection and local file produces same output", {
  expect_true(identical(h, k))
})

test_that("compact data read", {
  expect_equal(nrow(h), 6)
  expect_equal(ncol(h), 8)
  expect_true(all(vapply(h, inherits, logical(1), "character")))
})

test_that("series with no observations read as NA", {
  d <- read_sdmx(
    system.file("extdata/compact_nullobs_2.0.xml", package = "readsdmx")
  )
  expect_equal(unique(d$LOCATION), c("FRA", "BEL", "EST"))
  expect_equal(d[nrow(d), "LOCATION"], "EST")
  expect_equal(d[nrow(d), "TIME"], NA_character_)
  expect_equal(d[nrow(d), "OBS_VALUE"], NA_character_)
})

test_that("structure specific data read", {
  d <- read_sdmx(
    system.file("extdata/structure_specific_data_2.1.xml", package = "readsdmx")
  )
  expect_equal(unique(d$INDICATOR), c("LU_PE_NUM", "LUR_PE_NUM"))
  expect_equal(d$COUNTERPART_AREA[[1]], "_Z")
  expect_equal(d$REF_AREA[[1]], "ES")
  expect_equal(d$OBS_VALUE[[nrow(d)]], "23.78")
})
