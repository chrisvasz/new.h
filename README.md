# new.h

## Introduction

This utility is used to locate memory leaks in C++. It reports the location
(file and line number) of the `new` call that generated the leak.

## Usage

Copy `new.h` into your project folder (or somewhere your compiler will locate
it), then `#include "new.h"` in your main file.

`new.h` is only operational if it is included *before* the location of the
memory operations you want to test. For example, if I want to test a new data
structure, but not the STL, my main file includes would look like this:

    #include <string>
    #include "new.h"
    #include "my-new-data-structure.h"

    using namespace std;
    ...

That way, only the calls to `new` in "my-new-data-structure.h" are tracked.

### Why?

This utility defines a preprocessor macro that replaces the `new` keyword with
a call to a custom new operator defined in `new.h`. The preprocessor doesn't
start replacing text until a macro has been defined.

## License

Licensed under the MIT license. Have fun!

## Credits

Idea taken from
http://www.flipcode.com/archives/How_To_Find_Memory_Leaks.shtml
