context("genericdata")

test_that("generic data version 2.0 is read", {
  f <- system.file("extdata/generic_2.0.xml", package = "readsdmx")
  d <- read_sdmx(f)
  expected_cols <-
    c(paste0("C", 1:5),
      "ObsValue",
      "Time",
      "UNITS",
      "UNIT_MULTIPLIER")
  expect_true(all(colnames(d) %in% expected_cols))
  expect_equal(dim(d), c(19, 9))
  expect_equal(d[1, "Time"], "2000")
  expect_equal(d[1, "ObsValue"], "14114.0")
  expect_true(all(is.na(d[nrow(d), 6:ncol(d)])))
})

test_that("generic data version 2.1 is read", {
  f <- system.file("extdata/generic_2.1.xml", package = "readsdmx")
  d <- read_sdmx(f)
  expected_cols <-
    c("FOS07",
      "FREQ",
      "GEO",
      "OBS_STATUS",
      "ObsDimension",
      "ObsValue",
      "UNIT",
      "Y_GRAD")
  expect_true(all(colnames(d) %in% expected_cols))
  expect_equal(dim(d), c(4, 8))
  expect_true(all.equal(d$ObsValue, c("NaN", "NaN", "43.75", "NaN")))
  expect_true(all.equal(d$OBS_STATUS, c("na", "na", NA, "na")))
  expect_true(all.equal(d$Y_GRAD, c(
    "TOTAL", "TOTAL", "Y_GE1990", "Y_GE1990"
  )))
})

test_that("eurostat generic 2.0 is read", {
  f <-
    system.file("extdata/generic_eurostat_2.0.xml", package = "readsdmx")
  d <- read_sdmx(f)
  expect_equal(dim(d), c(2, 8))
  expected_cols <-
    c("ENTERPR",
      "FREQ",
      "GEO",
      "NACE_R2",
      "ObsValue",
      "REASON",
      "Time",
      "UNIT")
  expect_true(all(colnames(d) %in% expected_cols))
  expect_equal(d$NACE_R2, c("B-E", "M"))
  expect_equal(d$ENTERPR, c("OTH", "YHE"))
  expect_equal(d$ObsValue, c("16.8", "18.0"))
})


