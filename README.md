# new-tab-sand

A sand-themed new tab page for Google Chrome

## Usage

- Install emscripten (in particular, the `emcc` compiler is needed)
- Run `make release` to compile for release. 
- TODO: how to get page as new tab

## Development
- Run `make debug` to build with the address sanitizer and no optimizations.
- Compile flags (for an LSP) can be generated with `make compile_flags.txt`. Note that this file does not get removed with `make clean`, so if the compile flags change it will need to be manually removed.
