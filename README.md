# docling-parse

## Install

### CXX

To build the parse, simply run the following command in the root folder,

```sh
rm -rf build; cmake -B ./build; cd build; make
```

### Python

To build the package, simply run (make sure [poetry](https://python-poetry.org/) is [installed](https://python-poetry.org/docs/#installing-with-the-official-installer)),

```
poetry build
```

## Run

### CXX

In the build-folder, run 

```
./parse.exe <input-file> <optional-logging:true>
```

If you dont have an input file, then a template input file will be printed on the terminal.

### Python

## Test

To test the package, run,

```
poetry run pytest ./tests/test_parse.py
```