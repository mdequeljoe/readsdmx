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

test_that("ecb generic data is read", {

 test_ecb <- function(d){
   expect_true(inherits(d, "data.frame"))
   expect_equal(nrow(d), 12)
   expect_equal(ncol(d), 9)
   expect_equal(d[1, "ObsValue"], "1.3413")
   expect_equal(d[nrow(d), "ObsValue"], "1.3898")
 }
 sf_ <- function(name) system.file(file.path('extdata', name), package = 'readsdmx')
 f <- sf_("generic_ecb_exr_rg_flat.xml")
 d <- readsdmx::read_sdmx(f)
 test_ecb(d)
 expect_equal(d[5, "ObsValue"], "0.83987")
 expect_equal(d[1, "TIME_PERIOD"], "2010-08")

 f <- sf_("generic_ecb_exr_rg_ts.xml")
 d <- readsdmx::read_sdmx(f)
 test_ecb(d)
 expect_equal(d[1, "ObsDimension"], "2010-08")
 expect_equal(d[7, "CURRENCY"], "JPY")

 f <- sf_("generic_ecb_exr_rg_ts_gf.xml")
 d <- readsdmx::read_sdmx(f)
 test_ecb(d)
 #question - should this be ObsDimension when an id is given in the file?
 expect_true("ObsDimension" %in% names(d))
 expect_true("CURRENCY_DENOM" %in% names(d))
 expect_equal(d[9, "ObsValue"], "113.67")

 f <- sf_("generic_ecb_exr_rg_xs.xml")
 d <- readsdmx::read_sdmx(f)
 test_ecb(d)
 expect_equal(d[11, "ObsDimension"], "JPY")
})


