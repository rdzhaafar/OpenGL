#!/bin/sh

build_directory=build

##===---------------------------------------------------------------------===##
# Compiler flags
##===---------------------------------------------------------------------===##

compiler=clang++

# Standard flags
compiler_flags="-c "
compiler_flags+="-g "
compiler_flags+="-O1 "
compiler_flags+="-Wall -Wextra "

# Disable deprecation warnings
compiler_flags+="-Wno-deprecated "

# Disable "unused parameter" warnings
compiler_flags+="-Wno-unused-parameter "

# GLFW
compiler_flags+="-Ivendor/glfw-3.3.6.bin.MACOS/include/ "
compiler_flags+="-Ivendor/glad/include "

##===---------------------------------------------------------------------===##
# Linker flags
##===---------------------------------------------------------------------===##

linker=clang++

# Link GLFW
linker_flags="-Lvendor/glfw-3.3.6.bin.MACOS/lib-x86_64/ "
linker_flags+="-lglfw3 "
linker_flags+="-framework OpenGL "
linker_flags+="-framework IOKit "
linker_flags+="-framework Cocoa "

##===---------------------------------------------------------------------===##
# Run the build
##===---------------------------------------------------------------------===##

# Create the build directory if it doesn't exist
mkdir -p $build_directory

# Compile sources
for source in *.cpp; do
    $compiler $compiler_flags -o $build_directory/$source.o $source
done

# Compile GLAD loader
$compiler $compiler_flags vendor/glad/src/glad.c -o $build_directory/glad.o

# Link
$linker $linker_flags -o $build_directory/graphics $build_directory/*.o