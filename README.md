#  readsdmx

Read sdmx data into R dataframes from either a local SDMX-ML file or connection:

```r
u <-
  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/..Q/all?format=compact_v2"
d <- readsdmx::read_sdmx(u)

```
The [RapidXML](http://rapidxml.sourceforge.net) C++ library is used to parse the data.

## install

work in progress

