context("utility data")

test_that("utility data 2.0 files read correctly", {
  d <- read_sdmx(
    system.file("extdata/utility_2.0.xml", package = "readsdmx")
  )
  expect_equal(nrow(d), 12)
  expect_equal(ncol(d), 14)
  expect_equal(d[1, "OBS_VALUE"], "3.14")
  expect_equal(d[1, "FREQ"], "M")
  expect_equal(unique(d[["JD_CATEGORY"]]), "A")
  expect_equal(unique(d[["JD_TYPE"]]), "P")
  expect_equal(unique(d[["VIS_CTY"]]), "MX")


  expect_equal(d[3, "TIME_PERIOD"], "2000-03")
  expect_equal(d[3, "OBS_VALUE"], "5.26")

  expect_equal(d[11, "OBS_VALUE"], "3.19")
  expect_equal(d[11, "TIME_PERIOD"], "2000-11")

  expect_equal(d[12, "OBS_VALUE"], "3.14")


})
