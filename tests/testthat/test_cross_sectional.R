context("cross sectional data")

test_that("cross sectional data message read", {

  d <- read_sdmx(
    system.file("extdata/cross_sectional_2.0.xml", package = "readsdmx")
  )

  expect_equal(nrow(d), 8)
  expect_equal(ncol(d), 13)
  expect_equal(d[1, 'value'], "3.14")
  expect_equal(d[5, 'value'], "2.34")
  expect_equal(d[8, 'value'], "-1.06")

  expect_equal(d[1, 'OBS_KEY'], "STOCKS")

  expect_equal(unique(d$TIME), "2000")
  expect_equal(unique(d$COLLECTION), "B")

})
