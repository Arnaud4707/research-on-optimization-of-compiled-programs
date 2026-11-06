# Research on optimization of compiled programs

This repository has for goal to optimize more possible a compiled program ex: c/c++/rust/julia. To begin I will on c++ language with a custom_grep (with -r option) binary, the goal will be get to closer performance's rg tool.

Although custom_grep program don't really reproduce grep or rg behavior, it remains a good base for profiling function, analyze data, calcul of IPC and more globaly research of performance's gains in differents aspects, better use of RAM memory, parallelization with SIMD et multi threadings, etc. 

After to have done everythings we could to do in c++ code, we will see how the compiler does to compile and generate **.o** file, optimize, find the good target's machine for assembly code and linking of input file for return a binary conform to target machine.

In this project we will use **LLVM** ecosystem, so **clang** will be the frontend, llvm the backend, and **lldb** the debugger.

Flags of compilations are :

> - -Wall: *Turns on lots of compiler warning flags, specifically (-Waddress, -Wcomment, -Wformat, -Wbool-compare, -Wuninitialized, -Wunknown-pragmas, -Wunused-value, -Wunused-value …)*
> - -Wextra: *Enables extra flags not enabled by -Wall, and issue all warning required by ISO C and ISO C++ standard*
> - -Werror: *Turn any warning into a compilation error.*
> - O2: *Higher level of optmization. Slower compile-time, better for production builds.*
> - g3: *Builds executable with debugging symbols for GDB GNU Debugger or LLDB Clang/LLVM Debugger. It should only be used during development for debugging builds.*
> - -fno-omit-frame-pointer: *Don't omit frame pointer for debugging, it doesn’t guarantee the frame pointer is used in all functions. Several targets always omit the frame pointer in leaf functions. Enabled by default at -O1 and higher.*

For more informations about compiler flags, go see 
 - [CPP Compiler Flags](https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html).
 - [GNU Doc Optimize Options](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html).

## Commands tested

I choise a big ripository for target, "llvm-project".

```
rg --no-ignore --hidden "llvm" .
grep -rn "llvm" .
custom_grep -r "llvm" .
```

## Tools of analyse

List of some tools and commands who can be really usefull for profiling and engineering: *hyperfine*, *perf*, *strace*, *Flame Graph*, *readelf*.

> *You can have some problem if you have put **path** of program in your **PATH** environnment variable, for some tools, like perf, you have got to use sudo command administrator's right. But  **PATH** vaiable environment of sudo is not the same than than of for oders users so this: `sudo env PATH="$PAH"` it resolve the issue without create some vulnerability issues.*

For produce a reliable benchmark
```
hyperfine --metrics=all --warmup 3 -i COMMANDS
```

For profiling command

```
sudo perf record -g -o perf_.data COMMAND
```
> - g: *Enables call-graph (stack chain/backtrace) recording for both kernel space and user space.*

For analyse performance

```
perf report -i perf_.data
```

For view syscall

```
strace COMMAND
```

For the visualisation of time call's function and their children function in stack, Flame Graph

```
./stackcollapse-perf.pl data.perf > out.folded
./flamegraph.pl out.folded > out.svg
```

For display dependence of binary executable

```
readelf -d command
```

## Works

All branch of this project are a step of process of performance's optimization of custom grep, it's begun at initial-version to final-version.

The research are in `grep/Analyze.md`, [go see](/grep_cpp/Analyze.md).

