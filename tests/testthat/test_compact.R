context("compactdata")

u <-
  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/AUS.RGDP_INDEX.Q/all?startTime=2007-Q1&endTime=2008-Q2&format=compact_v2"
f <- system.file("tests/testthat/compact_test.xml", package = "readsdmx")
k <- read_sdmx(u)
h <- read_sdmx(u)

test_that("compact data read", {
  expect_true(identical(h, k))
  expect_equal(nrow(k), 6)
  expect_equal(ncol(k), 8)
  expect_true(all(sapply(k, inherits, "character")))
})

