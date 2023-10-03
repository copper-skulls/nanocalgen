# Developer documentation

## C++ version

This project *requires* the `c++20` standard due to the `.contains()` call on a map and the `std::optional` type. If you're using a clang-based LSP, it should catch onto it thanks to the [compile_flags.txt](./compile_flags.txt) file. However it is still needed to be specified at compilation time, until clang bumps its default `C++` version to the 20 standard.

## General structure

1. Load config ([Config.hpp](./Config.hpp) and [Config.cpp](./Config.cpp))
2. Build an internal representation of the SVG document ([.hpp](./SVGDocument.hpp), [.cpp](./SVGDocument.cpp)) using CalendarBuilder from the given config ([.hpp](./CalendarBuilder.hpp), [.cpp](./CalendarBuilder.cpp))
3. Print the built calendar into file defined by the config

Details are documented as comments in the code itself.

### Config

Configuration loading and parsing.

If extending without adding more variables, just setting them differently, this is the only code you'll need to look into. A lot of things are handled by the enums at the very beginning of the `.cpp` file, so just extending them properly and the switch cases they participate in should be enough.

Adding more complicated things and new variables (ie. new goal types) will also require changes to the CalendarBuilder files.

### SVGDocument

Structures needed to create an internal representation of the calendar as an SVG document.

The `SVGElement` class is a very minimalistic abstract class to allow for polymorphism. The code in this entire class is *minimal* and it has some slightly awkward usage thanks to it (for use with vector, empty and move constructors have been marked as default, implicitly deleting the rest), it is enough for this use case. Extending with more sub-classes should be easy though, as long as the move and empty constructors are taken care of somehow.

The file only really covers the very minimal subset of SVG needed for this utility.

### CalendarBuilder

Takes care of actually crunching the config object into its SVG representation.
