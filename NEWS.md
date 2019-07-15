# 0.3.0

- Adds the following data messages:
    - Utility (2.0)
    - Cross-sectional (2.0)
- Adds internal class system for encapsulating different message types
- Fixes character encodings for accents (#1)
- Codelist data now includes all attributes present, such as 'parentCode' (#3)

# 0.2.3

- Fixes issue in generic data message such that expected
attributes (e.g. "id", "value") are first checked for explicitly.
- Adds ECB generic data tests

# 0.2.2

- Add more checks for invalid SDMX-ML files and unsupported messages:
    - Checks that a 'structures' node exists for Code-lists messages
    - Checks that a 'DataSet' node exists for compact and generic data
- CRAN fix to include a runnable R example

# 0.2.1

- Performance improvements to reading compact and generic data
- Minor fixes for CRAN policies for first submission

# 0.2.0

- The follow data message types are supported:
    - Compact (version 2.0)
    - Generic (2.0 & 2.1)
    - Structure Specific Data (2.1)
    - Structure Code-lists (2.0 & 2.1)
- Data from web-services obtained via 'download.file'
