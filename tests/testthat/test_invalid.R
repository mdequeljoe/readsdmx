context("invalid files")

test_that("invalid files are detected", {
  f <- tempfile(fileext = ".xml")
  write('<?xml version="1.0" encoding="UTF-8"?>
        <notsdmx></notsdmx>', f)
  expect_error(read_sdmx(f))

  write('<message:GenericData></message:GenericData>', f)
  expect_error(read_sdmx(f))

  write('<message:CompactData></message:CompactData>', f)
  expect_error(read_sdmx(f))

  write(
      '<message:Structure>
      <message:invalid_id></message:invalid_id>
      </message:Structure>',
      f
  )
  expect_error(read_sdmx(f))

  write(
      '<message:Structure>
      <message:Structures>
      <message:invalid_id></message:invalid_id>
      </message:Structures>
      </message:Structure>',
      f
  )
  expect_error(read_sdmx(f))
  
})
