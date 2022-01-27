# download-include-files.py

This script downloads header files with special notation in C/C++ code automatically.

For example, the following code downloads xxhash.h from GitHub and save it at `third_party/include/xxhash.h`.

```C++
#include "xxhash.h" //--download-from--:https://github.com/Cyan4973/xxHash/blob/v0.8.0/xxhash.h
```

Special comment in C/C++ code
=============================
`download-include-files.py` recognizes the following `#include` and special comment pair.

```C++
#include "(FILENAME)" // --download-from--:(URL)
```

or

```
#include "(FILENAME)" /* --download-from--:(URL) */
```

These comments indicate to download `(URL)` as `(FILENAME)`.

The following example downloads xxhash.h from github.

```C++
#include "xxhash.h" // --download-from--:https://github.com/Cyan4973/xxHash/blob/dev/xxhash.h
```

And the local filepath of it is

```C++
third_party/include/xxhash.h
```

You can change the prefix `third_party/include` by the `--include` command line option (see below).


Special form `#include "github.com/..."` in C/C++ code
======================================================
`download-include-files.py` recognizes the following special form of `#include`.

```C++
#include "github.com/(FILENAME)"
```

This `#include` line indicates to download `(FILENAME)` from GitHub.

For example, the following `#include` downloads `stb_image.h` from github.

```C++
#include "github.com/nothings/stb/blob/master/stb_image.h"
```

And the local file name of it is

```C++
third_party/include/github.com/nothings/stb/blob/master/stb_image.h
```

You can change `third_party/include` by `--include` (see below).


`--include` (or `-I`)
=====================
The root directory of `(FILENAME)` can be specified by the following command line option

```
--include (DIR)
```

when you specify `--include`, actual downloaded file path becomes

```
(DIR)/(FILENAME)
```

Default (DIR) is "`third_party/include`".

The following example downloads files under `external/include/`

```
download-include-files.py *.c *.h *.cpp *.hpp --include external/include
```

And above examples are downloaded as

```
external/include/xxhash.h
external/include/github.com/nothings/stb/blob/master/stb_image.h
```


Download only form
==================
When you just want to download a file but avoid to be compiled, you can
put them within comment line.

```C++
// #include "github.com/..."
```

See `examples/rapidjson/rapidjson-example.cpp` for details.


Glob
====

Since this program uses standard Python glob module,
you can use usual glob notations.  (Even on Windows Command Prompt)
