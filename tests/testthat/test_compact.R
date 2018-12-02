context("compactdata")

u <-
  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/AUS.RGDP_INDEX.Q/all?startTime=2007-Q1&endTime=2008-Q2&format=compact_v2"
f <- system.file("tests/testthat/compact_test.xml", package = "readsdmx")
k <- read_sdmx(u)
h <- read_sdmx(f)

test_that("compact data read", {
  expect_true(identical(h, k))
  expect_equal(nrow(k), 6)
  expect_equal(ncol(k), 8)
  expect_true(all(sapply(k, inherits, "character")))
})

test_that("series with no observations read as NA", {
  d <- read_sdmx(
    system.file("tests/testthat/compact_test2.xml", package = "readsdmx")
  )
  expect_equal(unique(d$LOCATION), c("FRA", "BEL", "EST"))
  expect_equal(d[nrow(d), "LOCATION"], "EST")
  expect_equal(d[nrow(d), "TIME"], NA_character_)
  expect_equal(d[nrow(d), "OBS_VALUE"], NA_character_)

})
