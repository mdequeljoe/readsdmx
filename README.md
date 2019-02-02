#  readsdmx

[![Build Status](https://travis-ci.org/mdequeljoe/readsdmx.svg?branch=master)](https://travis-ci.org/mdequeljoe/readsdmx)
[![CRAN Status](https://www.r-pkg.org/badges/version/readsdmx)](https://cran.r-project.org/package=readsdmx)

Read sdmx data into R dataframes from either a local SDMX-ML file or from a SDMX web-service:

```r
u <-
  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/..Q"
d <- readsdmx::read_sdmx(u)

```
The [RapidXML](http://rapidxml.sourceforge.net) C++ library is used to parse the data.

## Install

From CRAN:
```r
install.packages("readsdmx")
```

From GitHub:
```r
devtools::install_github("mdequeljoe/readsdmx")
```

## Data messages

The follow data message types are supported:

- Compact (version 2.0)
- Generic (2.0 & 2.1)
- Structure Specific Data (2.1)
- Structure Code-lists (2.0 & 2.1)

## Benchmark

For the above example (locally):

```r
download.file(u, path <- tempfile(fileext = ".xml"), quiet = TRUE)
microbenchmark::microbenchmark(
  readsdmx = readsdmx::read_sdmx(path),
  rsdmx = as.data.frame(rsdmx::readSDMX(path, FALSE)),
  times = 5L,
  unit = "s"
)

#> Unit: seconds
#>      expr    min     lq   mean median     uq    max neval
#>  readsdmx  0.158  0.164  0.176  0.166  0.176  0.217     5
#>     rsdmx 26.321 26.992 30.762 30.757 34.341 35.397     5
```

## Related

[rsdmx](https://github.com/opensdmx/rsdmx)
