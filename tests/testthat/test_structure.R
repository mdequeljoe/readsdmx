context("structuredata")

test_that("version 2.0 structure data is read", {
  d <-
    read_sdmx(system.file("extdata/codelist_2.0.xml", package = "readsdmx"))
  expect_true(all(vapply(d, inherits, logical(1), "character")))
  expect_true(all(
    colnames(d) %in%
      c("en", "en_description", "fr", "fr_description", "id", "value")
  ))
  expect_equal(d$en_description[[1]], "Australia")
  expect_equal(d$fr_description[[1]], "Australie")
})

test_that("version 2.1 structure data is read", {
  d <-
    read_sdmx(system.file("extdata/codelist_2.1.xml", package = "readsdmx"))
  expect_equal(d$en[[1]], "CL_FAO_MAJOR_AREA")
  expect_true(all(vapply(d, inherits, logical(1), "character")))
  expect_true(all(
    colnames(d) %in% c(
      "en",
      "en_description",
      "es_description",
      "fr_description",
      "id",
      "id_description"
    )
  ))
})
