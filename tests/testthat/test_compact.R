context("compactdata")

test_that("compact data read", {
  d <- read_sdmx(
    system.file("extdata/compact_2.0.xml", package = "readsdmx")
  )
  expect_true(inherits(d, "data.frame"))
  expect_equal(nrow(d), 6L)
  expect_equal(ncol(d), 8L)
  expect_true(all(vapply(d, inherits, logical(1), "character")))

  d <- read_sdmx(
    system.file("extdata/compact_2.0_BIS.xml", package = "readsdmx")
  )
  expect_true(inherits(d, "data.frame"))
  expect_equal(nrow(d), 26L)
  expect_equal(ncol(d), 9L)
  expect_true(all(vapply(d, inherits, logical(1), "character")))
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

