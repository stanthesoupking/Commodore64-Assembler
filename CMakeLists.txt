cmake_minimum_required (VERSION 2.6)
project (c64_assembler)

file(GLOB c64_assembler_SRC
    "src/*.c"
)

include_directories(include)

file(GLOB c64_assembler_INC
    "include/*.h"
)

add_executable(c64_assembler ${c64_assembler_SRC} ${c64_assembler_INC})
