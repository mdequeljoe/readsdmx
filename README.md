#  readsdmx

[![Build Status](https://travis-ci.org/mdequeljoe/readsdmx.svg?branch=master)](https://travis-ci.org/mdequeljoe/readsdmx)
[![CRAN Status](https://www.r-pkg.org/badges/version/readsdmx)](https://cran.r-project.org/package=readsdmx)
[![Github tag](https://img.shields.io/github/tag/mdequeljoe/readsdmx.svg?style=flat)](https://github.com/mdequeljoe/readsdmx/tags)
[![lifecycle](https://img.shields.io/badge/lifecycle-maturing-blue.svg)](https://www.tidyverse.org/lifecycle/#maturing)

Read [sdmx](https://sdmx.org/) data into dataframes from either a local SDMX-ML file or from a SDMX web-service:

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
- Utility (2.0)
- Cross-sectional (2.0)
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
#>  readsdmx  0.152  0.153  0.159  0.161  0.165  0.165     5
#>     rsdmx 23.955 24.578 24.732 24.937 25.005 25.185     5
```

## Related

[rsdmx](https://github.com/opensdmx/rsdmx)

[pandasdmx](https://github.com/dr-leo/pandaSDMX) (python)
