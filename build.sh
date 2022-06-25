#!/bin/bash

CC="gcc"
CFLAGS="-Wall -Wextra -Wpedantic -std=c17 -march=native -O3 -flto -c"
LDFLAGSS="-O3 -flto"

SRCDIR="src"
BUILDDIR="build"

set -x
mkdir -p $BUILDDIR/$SRCDIR

# -s: assembly
# -c: binary 
# .c -> .s -> .o -> something executable

$CC $CFLAGS $SRCDIR/chunk.c -o $BUILDDIR/$SRCDIR/chunk.c.o 
$CC $CFLAGS $SRCDIR/clox.c -o $BUILDDIR/$SRCDIR/clox.c.o 
$CC $CFLAGS $SRCDIR/compiler.c -o $BUILDDIR/$SRCDIR/compiler.c.o 
$CC $CFLAGS $SRCDIR/debug.c -o $BUILDDIR/$SRCDIR/debug.c.o 
$CC $CFLAGS $SRCDIR/main.c -o $BUILDDIR/$SRCDIR/main.c.o 
$CC $CFLAGS $SRCDIR/memory.c -o $BUILDDIR/$SRCDIR/memory.c.o 
$CC $CFLAGS $SRCDIR/scanner.c -o $BUILDDIR/$SRCDIR/scanner.c.o 
$CC $CFLAGS $SRCDIR/value.c -o $BUILDDIR/$SRCDIR/value.c.o 
$CC $CFLAGS $SRCDIR/vm.c -o $BUILDDIR/$SRCDIR/vm.c.o 

$CC $LDFLAGSS $BUILDDIR/$SRCDIR/chunk.c.o \
              $BUILDDIR/$SRCDIR/clox.c.o \
              $BUILDDIR/$SRCDIR/compiler.c.o \
              $BUILDDIR/$SRCDIR/debug.c.o \
              $BUILDDIR/$SRCDIR/main.c.o \
              $BUILDDIR/$SRCDIR/memory.c.o \
              $BUILDDIR/$SRCDIR/scanner.c.o \
              $BUILDDIR/$SRCDIR/value.c.o \
              $BUILDDIR/$SRCDIR/vm.c.o \
              -o $BUILDDIR/clox
