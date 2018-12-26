#  readsdmx

Read sdmx data into R dataframes from either a local SDMX-ML file or connection:

```r
u <-
  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/..Q/all?format=compact_v2"
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

## Related

[rsdmx](https://github.com/opensdmx/rsdmx)
