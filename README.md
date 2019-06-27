# Commodore64 Assembler
This project's primary aim is to make Commodore 64 development on a PC more streamlined. The end goal is a Basic assembler that can automate many tasks which make Commodore 64 development frustrating, such as: line number management and Commodore-unique PETSCII characters.

For more details please read the [wiki](https://github.com/stanthesoupking/Commodore64-Assembler/wiki).

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
c64_assembler <input-file> <output-file>
```

### For example
Compiling the example program, `counter.bas`, would require the following command:
```
c64_assembler counter.bas counter.prg
```
