#  readsdmx

[![Build Status](https://travis-ci.org/mdequeljoe/readsdmx.svg?branch=master)](https://travis-ci.org/mdequeljoe/readsdmx)

Read sdmx data into R dataframes from either a local SDMX-ML file or from a SDMX web-service:

```r
u <-
  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/..Q"
d <- readsdmx::read_sdmx(u)

```
The [RapidXML](http://rapidxml.sourceforge.net) C++ library is used to parse the data.

## install

```r
devtools::install_github("mdequeljoe/readsdmx")
```

## Data messages

The follow data message types are supported:

- Compact (version 2.0)
- Generic (2.0 & 2.1)
- Structure Specific Data (2.1)
- Structure Code-lists (2.0 & 2.1)

## Performance

For the above example:

```r
microbenchmark::microbenchmark(
  d <- readsdmx::read_sdmx(u),
  d2 <- as.data.frame(rsdmx::readSDMX(u)),
  times = 3L,
  unit = "s"
)
#> Unit: seconds
#>                                     expr   min    lq  mean median   uq   max neval
#>              d <- readsdmx::read_sdmx(u)  1.08  1.12  1.16   1.17  1.2  1.23     3
#>  d2 <- as.data.frame(rsdmx::readSDMX(u)) 28.11 28.38 28.70  28.65 29.0 29.36     3
```

For a local file:

```r
download.file(u, path <- tempfile(fileext = ".xml"), quiet = TRUE)
microbenchmark::microbenchmark(
  d <- readsdmx::read_sdmx(path),
  d2 <- as.data.frame(rsdmx::readSDMX(path, FALSE)),
  times = 3L, 
  unit = "s"
)

#> Unit: seconds
#>                                               expr    min     lq   mean median     uq    max neval
#>                     d <- readsdmx::read_sdmx(path)  0.181  0.182  0.188  0.183  0.192  0.201     3
#>  d2 <- as.data.frame(rsdmx::readSDMX(path, FALSE)) 22.397 23.114 25.712 23.832 27.370 30.909     3

```

## Related

[rsdmx](https://github.com/opensdmx/rsdmx)
