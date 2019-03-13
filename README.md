# SimpleZip
A simple C++ wrapper around the "miniz" zip library.

## Introduction
For my OpenXLSX library, I have been looking around for a simple C++ library for handling .zip files. Unfortunately, that proved very difficult to find, so I ended up relying on a combination of zlib libzip and libzippp (https://github.com/ctabin/libzippp).

Handling so many external dependencies turned out to be very cumbersome, so I began looking for alternatives.

The 'miniz' library (https://github.com/richgel999/miniz) has a proven track record as a .zip library contained in a single source file. Unfortunately, it's written in C and is not very well documented.

There is another initiative for wrapping miniz in C++, callec miniz-cpp (https://github.com/tfussell/miniz-cpp), used as part of the XLNT library (https://github.com/tfussell/xlnt), but it doesn't fully satisfy the requirements I have, so I decided to make my own.

## Ambition
My ambition for SimpleZip is to enable reading, writing and modification of existing zip archives, as well as creation of new archives.

Some of the features are not natively available in miniz, so some workarounds has been made (e.g. deletion of archive entries).

My hope is that this can become a header-only library, but that has not yet been implemented.

## Current Status
The library already works fairly well, but additional work is required to ensure robustness. Also, a number of other things needs to be implemented, e.g.

 - Tests
 - Documentations
 
## Compatibility
The library has been tested on:
- MacOS (GCC and Clang)

In the future, it will be tested on Linux and Windows as well.
