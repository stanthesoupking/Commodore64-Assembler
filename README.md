# Commodore64 Assembler
Assembles plain text files into C64-readable, tokenised format.  

*Note: This project is in its early stages of development and currently only supports translating plain text to the tokenised C64 format. Assembling features are coming soon(hopefully) :)*

## Compiling
To compile, run the following commands from the root directory of the project:
```bash
mkdir build
cd build
cmake ../
make
```

## Usage
```
c64_assembler [input-file] [output-file]
```

### For example
Compiling the example program, `counter.basic`, would require the following command:
```
c64_assembler ../examples/counter.bas ../examples/counter.prg
```