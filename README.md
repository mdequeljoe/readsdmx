#  readsdmx

Read sdmx data into R dataframes

```r
u <-
  "https://stats.oecd.org/restsdmx/sdmx.ashx/GetData/HH_DASH/..Q/all?format=compact_v2"
d <- readsdmx::read_sdmx(u)

```
