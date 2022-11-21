# lepk
Lepk is my collection of single header libraries. So basically a ripoff of [nothings stb](https://github.com/nothings/stb).

Every library has a test built into the header. These tests are ran with test.c and a Makefile.

Everything is written in pedantic C99.

## Current libraries
| Library | Version | Usage |
| - | - | - |
| [lepk_da.h](libs/lepk_da.h) | 1.1 | Dynamic arrays. | 
| [lepk_window.h](libs/lepk_window.h) | 1.2 | Windowing library. |
| [lepk_type.h](libs/lepk_type.h) | 1.0 | Generic types and boolean operations. |
| [lepk_file.h](libs/lepk_file.h) | 1.0 | Interacting with the filesystem. |

## Lepkc
Lepkc or the lepk compiler is a compiler which takes a header and a source file, combines them into a single header.

### Installation
Installing lepkc will copy the compiled binary to /usr/bin.
```shell
$ make lepkc_install
```

### Compile
Compiling will produce a binary in the current directory.
```shell
$ make lepkc
```
