# mkhas.h

single-header hash table implementation

## features

- c99
- uses FNV-1a hash algorithm
- uses linked lists to solve collisions
- no dependencies other than libc
- uses results+error codes where it can (see issues section)
- can customize `malloc` and `free` functions using macros

## usage

### install

copy and paste [mkhas.h](./mkhas.h) into your project

in a single c source file, include the header like this:

```
#define mkhas_IMPL
#include "mkhas.h"
```

this will load all implementations of the functions in the source file

you can now compile it and use the table

### example

see examples in [example.c](./example.c) and the test scripts

## building

use `make example` to build the example

use `make test` to build and run all tests

## issues

- failing to insert a key and using `mkhas_free` might result in a memory leak
or a double-free. failing to insert a key should never actually insert it into 
the table, so `mkhas_free` might miss the data and cause a memory leak.
- the `mkhas_free` and `mkhas_delete` functions might crash the program if used
incorrectly
- i have not implemented results in the `mkhas_remove` and `mkhas_clear` 
functions since i'm assuming the existence of the key implies a successful 
insertion. creating keys outside of `mkhas_get` isn't recommended

## license and copyright

licensed under the GNU GPLv3.0 or any later version, at your option. see 
LICENSE file for license and copyright information
